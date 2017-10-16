// Copyright (c) 2017 by Thomas Early N7TAE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <pwd.h>
#include <ctime>
#include <fstream>

#include "SmartServerThread.h"
//#include "SmartServerDefs.h"
#include "StarNetHandler.h"
#include "DExtraHandler.h"			// DEXTRA LINK
#include "DCSHandler.h"				// DCS LINK
#include "HeaderData.h"
#include "G2Handler.h"
#include "AMBEData.h"
//#include "HostFile.h"				// DEXTRA_LINK || DCS_LINK
#include "Utils.h"

const unsigned int REMOTE_DUMMY_PORT = 65015U;

CSmartServerThread::CSmartServerThread() :
m_killed(false),
m_stopped(true),
m_callsign(),
m_address(),

#if defined(DEXTRA_LINK)
m_dextraPool(NULL),
#endif

#if defined(DCS_LINK)
m_dcsPool(NULL),
#endif

m_g2Handler(NULL),
m_irc(NULL),
m_cache(),
m_logEnabled(false),
m_statusTimer(1000U, 1U),		// 1 second
m_lastStatus(IS_DISCONNECTED),
m_remoteEnabled(false),
m_remotePassword(),
m_remotePort(0U),
m_remote(NULL)
{
	CHeaderData::initialise();
	CG2Handler::initialise(MAX_ROUTES);
	CStarNetHandler::initialise(MAX_STARNETS);
	
#if defined(DEXTRA_LINK)
	CDExtraHandler::initialise(MAX_DEXTRA_LINKS);
#endif

#if defined(DCS_LINK)
	CDCSHandler::initialise(MAX_DCS_LINKS);
#endif

}

CSmartServerThread::~CSmartServerThread()
{
	CHeaderData::finalise();
	CG2Handler::finalise();
	CStarNetHandler::finalise();
	
#if defined(DEXTRA_LINK)
	CDExtraHandler::finalise();
#endif

#if defined(DCS_LINK)
	CDCSHandler::finalise();
#endif

}

void CSmartServerThread::run()
{
#if defined(DEXTRA_LINK)
	m_dextraPool = new CDExtraProtocolHandlerPool(MAX_DEXTRA_LINKS, DEXTRA_PORT, m_address);
	ret = m_dextraPool->open();
	if (!ret) {
		lprint("Could not open the DExtra protocol pool");
		delete m_dextraPool;
		m_dextraPool = NULL;
	}
#endif

#if defined(DCS_LINK)
	m_dcsPool = new CDCSProtocolHandlerPool(MAX_DCS_LINKS, DCS_PORT, m_address);
	ret = m_dcsPool->open();
	if (!ret) {
		lprint("Could not open the DCS protocol pool");
		delete m_dcsPool;
		m_dcsPool = NULL;
	}
#endif

	m_g2Handler = new CG2ProtocolHandler(G2_DV_PORT, m_address);
	ret = m_g2Handler->open();
	if (!ret) {
		lprint("Could not open the G2 protocol handler");
		delete m_g2Handler;
		m_g2Handler = NULL;
	}

	// Wait here until we have the essentials to run
#if defined(DEXTRA_LINK)
	while (!m_killed && (m_g2Handler == NULL || m_dextraPool == NULL || m_irc == NULL || m_callsign.IsEmpty()))
		::wxMilliSleep(500UL);		// 1/2 sec
#elif defined(DCS_LINK)
	while (!m_killed && (m_g2Handler == NULL || m_dcsPool == NULL || m_irc == NULL || m_callsign.IsEmpty()))
		::wxMilliSleep(500UL);		// 1/2 sec
#else
	while (!m_killed && (m_g2Handler == NULL || m_irc == NULL || m_callsign.IsEmpty()))
		::wxMilliSleep(500UL);		// 1/2 sec
#endif

	if (m_killed)
		return;

	m_stopped = false;

	lprint("Starting the StarNet Server thread");

	CHeaderLogger* headerLogger = NULL;
	if (m_logEnabled) {
		headerLogger = new CHeaderLogger(m_logDir);
		bool ret = headerLogger->open();
		if (!ret) {
			delete headerLogger;
			headerLogger = NULL;
		}
	}

#if defined(DEXTRA_LINK)
	loadReflectors(DEXTRA_HOSTS_FILE_NAME);
#endif
#if defined(DCS_LINK)
	loadReflectors(DCS_HOSTS_FILE_NAME);
#endif

	CG2Handler::setG2ProtocolHandler(m_g2Handler);
	CG2Handler::setHeaderLogger(headerLogger);

#if defined(DEXTRA_LINK)
	CDExtraHandler::setCallsign(m_callsign);
	CDExtraHandler::setDExtraProtocolHandlerPool(m_dextraPool);
	CDExtraHandler::setHeaderLogger(headerLogger);
#endif
#if defined(DCS_LINK)
	CDCSHandler::setDCSProtocolHandlerPool(m_dcsPool);
	CDCSHandler::setHeaderLogger(headerLogger);
	CDCSHandler::setGatewayType(GT_STARNET);
#endif

	CStarNetHandler::setCache(&m_cache);
	CStarNetHandler::setGateway(m_callsign);
	CStarNetHandler::setG2Handler(m_g2Handler);
	CStarNetHandler::setIRC(m_irc);
	CStarNetHandler::setLogging(m_logEnabled, m_logDir);
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	CStarNetHandler::link();
#endif

	if (m_remoteEnabled && !m_remotePassword.IsEmpty() && m_remotePort > 0U) {
		m_remote = new CRemoteHandler(m_remotePassword, m_remotePort);
		bool res = m_remote->open();
		if (!res) {
			delete m_remote;
			m_remote = NULL;
		}
	}

	time_t start;
	time(&start);

	m_statusTimer.start();

	try {
		while (!m_killed) {
			processIrcDDB();
			processG2();
#if defined(DEXTRA_LINK)
			processDExtra();
#endif
#if defined(DCS_LINK)
			processDCS();
#endif
			if (m_remote != NULL)
				m_remote->process();

			time_t now;
			time(&now);
			unsigned long ms = (unsigned long)(1000.0 * difftime(now, start));
			time(&start);

			m_statusTimer.clock(ms);

			CG2Handler::clock(ms);
			CStarNetHandler::clock(ms);
#if defined(DEXTRA_LINK)
			CDExtraHandler::clock(ms);
#endif
#if defined(DCS_LINK)
			CDCSHandler::clock(ms);
#endif

			usleep(1000*TIME_PER_TIC_MS);
		}
	}
	catch (std::exception& e) {
		lprint("Exception raised - \"%s\"", e.what());
	}
	catch (...) {
		lprint("Unknown exception raised");
	}

	lprint("Stopping the StarNet Server thread");

#if defined(DEXTRA_LINK)
	// Unlink from all reflectors
	CDExtraHandler::unlink();

	m_dextraPool->close();
	delete m_dextraPool;
#endif

#if defined(DCS_LINK)
	// Unlink from all reflectors
	CDCSHandler::unlink();

	m_dcsPool->close();
	delete m_dcsPool;
#endif

	m_g2Handler->close();
	delete m_g2Handler;

	m_irc->close();
	delete m_irc;

	if (m_remote != NULL) {
		m_remote->close();
		delete m_remote;
	}

	if (headerLogger != NULL) {
		headerLogger->close();
		delete headerLogger;
	}
}

void CSmartServerThread::kill()
{
	m_killed = true;
}

void CSmartServerThread::setCallsign(const std::string& callsign)
{
	if (!m_stopped)
		return;

	m_callsign = callsign;
}

void CSmartServerThread::setAddress(const std::string& address)
{
	m_address = address;
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CSmartServerThread::addStarNet(const std::string& callsign, const std::string& logoff, const std::string& repeater, const std::string& infoText, const std::string& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const std::string& reflector)
{
	CStarNetHandler::add(callsign, logoff, repeater, infoText, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}
#else
void CStarNetServerThread::addStarNet(const std::string& callsign, const std::string& logoff, const std::string& repeater, const std::string& infoText, const std::string& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
{
	CStarNetHandler::add(callsign, logoff, repeater, infoText, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}
#endif

void CSmartServerThread::setIRC(CIRCDDB* irc)
{
	assert(irc != NULL);

	m_irc = irc;
}

void CSmartServerThread::setRemote(bool enabled, const std::string& password, unsigned int port)
{
	if (enabled) {
		m_remoteEnabled  = true;
		m_remotePassword = password;
		m_remotePort     = port;
	} else {
		m_remoteEnabled  = false;
		m_remotePassword = password;
		m_remotePort     = REMOTE_DUMMY_PORT;
	}
}

void CSmartServerThread::processIrcDDB()
{
	// Once per second
	if (m_statusTimer.hasExpired()) {
		int status = m_irc->getConnectionState();
		switch (status) {
			case 0:
			case 10:
				if (m_lastStatus != IS_DISCONNECTED) {
					lprint("Disconnected from ircDDB");
					m_lastStatus = IS_DISCONNECTED;
				}
				break;
			case 7:
				if (m_lastStatus != IS_CONNECTED) {
					lprint("Connected to ircDDB");
					m_lastStatus = IS_CONNECTED;
				}
				break;
			default:
				if (m_lastStatus != IS_CONNECTING) {
					lprint("Connecting to ircDDB");
					m_lastStatus = IS_CONNECTING;
				}
				break;
		}

		m_statusTimer.start();
	}

	// Process all incoming ircDDB messages, updating the caches
	for (;;) {
		IRCDDB_RESPONSE_TYPE type = m_irc->getMessageType();

		switch (type) {
			case IDRT_NONE:
				return;

			case IDRT_USER: {
					std::string user, repeater, gateway, address, timestamp;
					bool res = m_irc->receiveUser(user, repeater, gateway, address, timestamp);
					if (!res)
						break;

					if (!address.IsEmpty()) {
						lprint("USER: %s %s %s %s", user.c_str(), repeater.c_str(), gateway.c_str(), address.c_str());
						m_cache.updateUser(user, repeater, gateway, address, timestamp, DP_DEXTRA, false, false);
					} else {
						lprint("USER: %s NOT FOUND", user.c_str());
					}
				}
				break;

			case IDRT_REPEATER: {
					std::string repeater, gateway, address;
					bool res = m_irc->receiveRepeater(repeater, gateway, address);
					if (!res)
						break;

					if (!address.IsEmpty()) {
						lprint("REPEATER: %s %s %s", repeater.c_str(), gateway.c_str(), address.c_str());
						m_cache.updateRepeater(repeater, gateway, address, DP_DEXTRA, false, false);
					} else {
						lprint("REPEATER: %s NOT FOUND", repeater.c_str());
					}
				}
				break;

			case IDRT_GATEWAY: {
					std::string gateway, address;
					bool res = m_irc->receiveGateway(gateway, address);
					if (!res)
						break;

#if defined(DEXTRA_LINK)
					CDExtraHandler::gatewayUpdate(gateway, address);
#endif
#if defined(DCS_LINK)
					CDCSHandler::gatewayUpdate(gateway, address);
#endif

					if (!address.IsEmpty()) {
						lprint("GATEWAY: %s %s"), gateway.c_str(), address.c_str();
						m_cache.updateGateway(gateway, address, DP_DEXTRA, false, false);
					} else {
						lprint("GATEWAY: %s NOT FOUND"), gateway.c_str();
					}
				}
				break;
		}
	}
}

#if defined(DEXTRA_LINK)
void CSmartServerThread::processDExtra()
{
	for (;;) {
		DEXTRA_TYPE type = m_dextraPool->read();

		switch (type) {
			case DE_NONE:
				return;

			case DE_POLL: {
					CPollData* poll = m_dextraPool->readPoll();
					if (poll != NULL) {
						CDExtraHandler::process(*poll);
						delete poll;
					}
				}
				break;

			case DE_CONNECT: {
					CConnectData* connect = m_dextraPool->readConnect();
					if (connect != NULL) {
						CDExtraHandler::process(*connect);
						delete connect;
					}
				}
				break;

			case DE_HEADER: {
					CHeaderData* header = m_dextraPool->readHeader();
					if (header != NULL) {
						// lprint("DExtra header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s", header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());
						CDExtraHandler::process(*header);
						delete header;
					}
				}
				break;

			case DE_AMBE: {
					CAMBEData* data = m_dextraPool->readAMBE();
					if (data != NULL) {
						CDExtraHandler::process(*data);
						delete data;
					}
				}
				break;
		}
	}
}
#endif

#if defined(DCS_LINK)
void CSmartServerThread::processDCS()
{
	for (;;) {
		DCS_TYPE type = m_dcsPool->read();

		switch (type) {
			case DC_NONE:
				return;

			case DC_POLL: {
					CPollData* poll = m_dcsPool->readPoll();
					if (poll != NULL) {
						CDCSHandler::process(*poll);
						delete poll;
					}
				}
				break;

			case DC_CONNECT: {
					CConnectData* connect = m_dcsPool->readConnect();
					if (connect != NULL) {
						CDCSHandler::process(*connect);
						delete connect;
					}
				}
				break;

			case DC_DATA: {
					CAMBEData* data = m_dcsPool->readData();
					if (data != NULL) {
						// lprint("DCS header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s", header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());
						CDCSHandler::process(*data);
						delete data;
					}
				}
				break;
		}
	}
}
#endif

void CSmartServerThread::processG2()
{
	for (;;) {
		G2_TYPE type = m_g2Handler->read();

		switch (type) {
			case GT_NONE:
				return;

			case GT_HEADER: {
					CHeaderData* header = m_g2Handler->readHeader();
					if (header != NULL) {
						// lprint("G2 header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X", header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());
						CG2Handler::process(*header);
						delete header;
					}
				}
				break;

			case GT_AMBE: {
					CAMBEData* data = m_g2Handler->readAMBE();
					if (data != NULL) {
						CG2Handler::process(*data);
						delete data;
					}
				}
				break;
		}
	}
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CSmartServerThread::loadReflectors(const char *fname)
{
	const char *directory = getenv("HOME");
	if (! directory)
		directory = DATA_DIR;
	std::string filepath(directory);
	filepath += "/" + fname;

	struct stat sbuf;
	if (stat(filepath, &sbuf)) {
		lprint("%s doesn't exist!", filepath.c_str());
		return;
	}

	std::ifstream hostfile;
	hostfile.open(filepath, std::ifstream::in);
	char line[256];
	hostfile.getline(line, 256);
	int count=0, tries=0;
	while (hostfile.good()) {
		const char *space = " /t/r";
		char *first = strtok(line, space);
		if (first) {
			if ('#' != first[0]) {
				tries++;
				char *second = strtok(NULL, space);
				if (second) {
					char *third = strtok(NULL, space);
					if (third && '#'==third[0])
						third = NULL;
					std::string name(first);
					name.resize(7, ' ');
					name.push_back('G');
					struct hostent *he = gethostbyname(second);
					if (he) {
						count++
						char straddr[16];
						unsigned char *puc = (unsigned char *)&he->h_addr_list[0];
						snprintf(straddr, 16, "%u.%u.%u.%u", puc[0], puc[1], puc[2], puc[3]);
						std::string address(straddr);
#if defined(DEXTRA_LINK)
						m_cache.updateGateway(name, address, DP_DEXTRA, third?1:0, true);
#else
						m_cache.updateGateway(name, address, DP_DCS, third?1:0, true);
#endif
						lprint("reflector:%s, address:%s lock:%s", name.c_str(), address.c_str(), third?"true":"false");
					}
				}
			}
		}
		hostfile.getline(line, 256);
	}


	lprint("Loaded %u of %u DExtra reflectors", count, tries);
}
#endif