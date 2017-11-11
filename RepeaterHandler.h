/*
 *   Copyright (C) 2010-2015 by Jonathan Naylor G4KLX
 * 	 Copyright (c) 2017 by Thomas A. Early N7TAE
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#pragma once

#include <string>
#include <list>
#include <netinet/in.h>

#include "RepeaterProtocolHandler.h"
#include "DExtraProtocolHandler.h"
#include "DPlusProtocolHandler.h"
#include "RemoteRepeaterData.h"
#include "G2ProtocolHandler.h"
#include "ReflectorCallback.h"
#include "RepeaterCallback.h"
#include "AnnouncementUnit.h"
#include "StarNetHandler.h"
#include "TextCollector.h"
#include "CacheManager.h"
#include "HeaderLogger.h"
#include "CallsignList.h"
//#include "DRATSServer.h"
#include "CCSCallback.h"
#include "VersionUnit.h"
#include "CCSHandler.h"
#include "StatusData.h"
//#include "APRSWriter.h"
#include "HeardData.h"
#include "AudioUnit.h"
#include "EchoUnit.h"
#include "PollData.h"
#include "DDData.h"
#include "IRCDDB.h"
#include "Timer.h"
#include "DTMF.h"
#include "Defs.h"

class CRepeaterHandler : public IRepeaterCallback, public IReflectorCallback, public ICCSCallback {
public:
	static void initialise(unsigned int maxRepeaters);

	static void add(const std::string& callsign, const std::string& band, const std::string& address, unsigned int port, HW_TYPE hwType, const std::string& reflector, bool atStartup, RECONNECT reconnect, bool dratsEnabled, double frequency, double offset, double range, double latitude, double longitude, double agl, const std::string& description1, const std::string& description2, const std::string& url, IRepeaterProtocolHandler* handler, unsigned char band1, unsigned char band2, unsigned char band3);

	static void setLocalAddress(const std::string& address);
	static void setG2Handler(CG2ProtocolHandler* handler);
	static void setIRC(CIRCDDB* irc);
	static void setCache(CCacheManager* cache);
	static void setGateway(const std::string& gateway);
	static void setLanguage(TEXT_LANG language);
	static void setDExtraEnabled(bool enabled);
	static void setDPlusEnabled(bool enabled);
	static void setDCSEnabled(bool enabled);
	static void setHeaderLogger(CHeaderLogger* logger);
	//static void setAPRSWriter(CAPRSWriter* writer);
	static void setInfoEnabled(bool enabled);
	static void setEchoEnabled(bool enabled);
	static void setDTMFEnabled(bool enabled);
	static void setWhiteList(CCallsignList* list);
	static void setBlackList(CCallsignList* list);
	static void setRestrictList(CCallsignList* list);

	static void startup();

	static bool getRepeater(unsigned int n, std::string& callsign, LINK_STATUS& linkStatus, std::string& linkCallsign);

	static std::list<std::string> listDVRepeaters();

	static CRepeaterHandler* findDVRepeater(const CHeaderData& header);
	static CRepeaterHandler* findDVRepeater(const CAMBEData& data, bool busy);
	static CRepeaterHandler* findDVRepeater(const std::string& callsign);

	static CRepeaterHandler* findRepeater(const CPollData& data);

	static CRepeaterHandler* findDDRepeater(const CDDData& data);
	static CRepeaterHandler* findDDRepeater();

	static void pollAllIcom(CPollData& data);

	static void writeStatus(CStatusData& statusData);

	static void resolveUser(const std::string& user, const std::string& repeater, const std::string& gateway, const std::string& address);
	static void resolveRepeater(const std::string& repeater, const std::string& gateway, const std::string& address, DSTAR_PROTOCOL protocol);

	static void finalise();

	static void clock(unsigned int ms);

	void processRepeater(CHeaderData& header);
	void processRepeater(CHeardData& heard);
	void processRepeater(CAMBEData& data);
	void processRepeater(CPollData& data);
	void processRepeater(CDDData& data);

	void processBusy(CHeaderData& header);
	void processBusy(CAMBEData& data);

	void link(RECONNECT reconnect, const std::string& reflector);
	void unlink(PROTOCOL protocol, const std::string& reflector);

	CRemoteRepeaterData* getInfo() const;

	virtual bool process(CHeaderData& header, DIRECTION direction, AUDIO_SOURCE source);
	virtual bool process(CAMBEData& data, DIRECTION direction, AUDIO_SOURCE source);
	virtual bool process(CDDData& data);

	virtual void linkUp(DSTAR_PROTOCOL protocol, const std::string& callsign);
	virtual void linkRefused(DSTAR_PROTOCOL protocol, const std::string& callsign);
	virtual bool linkFailed(DSTAR_PROTOCOL protocol, const std::string& callsign, bool isRecoverable);

	virtual void ccsLinkMade(const std::string& callsign, DIRECTION direction);
	virtual void ccsLinkFailed(const std::string& dtmf, DIRECTION direction);
	virtual void ccsLinkEnded(const std::string& callsign, DIRECTION direction);

protected:
	CRepeaterHandler(const std::string& callsign, const std::string& band, const std::string& address, unsigned int port, HW_TYPE hwType, const std::string& reflector, bool atStartup, RECONNECT reconnect, bool dratsEnabled, double frequency, double offset, double range, double latitude, double longitude, double agl, const std::string& description1, const std::string& description2, const std::string& url, IRepeaterProtocolHandler* handler, unsigned char band1, unsigned char band2, unsigned char band3);
	virtual ~CRepeaterHandler();

	void resolveUserInt(const std::string& user, const std::string& repeater, const std::string& gateway, const std::string& address);
	void resolveRepeaterInt(const std::string& repeater, const std::string& gateway, const std::string& address, DSTAR_PROTOCOL protocol);

	void startupInt();

	void setIndex(unsigned int index);

	void clockInt(unsigned int ms);

private:
	static unsigned int       m_maxRepeaters;
	static CRepeaterHandler** m_repeaters;

	static std::string  m_localAddress;
	static CG2ProtocolHandler* m_g2Handler;
	static CCacheManager* m_cache;
	static std::string    m_gateway;
	static CIRCDDB*  m_irc;
	static TEXT_LANG m_language;
	static bool      m_dextraEnabled;
	static bool      m_dplusEnabled;
	static bool      m_dcsEnabled;
	static bool      m_infoEnabled;
	static bool      m_echoEnabled;
	static bool      m_dtmfEnabled;

	static CHeaderLogger*   m_headerLogger;

	//static CAPRSWriter*     m_aprsWriter;

	static CCallsignList*   m_whiteList;
	static CCallsignList*   m_blackList;
	static CCallsignList*   m_restrictList;

	// Repeater info
	unsigned int              m_index;
	std::string               m_rptCallsign;
	std::string               m_gwyCallsign;
	unsigned char             m_band;
	in_addr                   m_address;
	unsigned int              m_port;
	HW_TYPE                   m_hwType;
	IRepeaterProtocolHandler* m_repeaterHandler;
	double                    m_frequency;
	double                    m_offset;
	double                    m_range;
	double                    m_latitude;
	double                    m_longitude;
	double                    m_agl;
	std::string               m_description1;
	std::string               m_description2;
	std::string               m_url;
	unsigned char             m_band1;
	unsigned char             m_band2;
	unsigned char             m_band3;
	unsigned int              m_repeaterId;
	unsigned int              m_busyId;
	CTimer                    m_watchdogTimer;
	bool                      m_ddMode;
	std::string               m_ddCallsign;
	CTimer                    m_queryTimer;

	// User details
	std::string               m_myCall1;
	std::string               m_myCall2;
	std::string               m_yourCall;
	std::string               m_rptCall1;
	std::string               m_rptCall2;
	unsigned char             m_flag1;
	unsigned char             m_flag2;
	unsigned char             m_flag3;
	bool                      m_restricted;

	// Statistics
	unsigned int              m_frames;
	unsigned int              m_silence;
	unsigned int              m_errors;

	// Slow data handling
	CTextCollector            m_textCollector;
	std::string               m_text;

	// Cross-band repeating
	CRepeaterHandler*         m_xBandRptr;

	// StarNet
	CStarNetHandler*          m_starNet;

	// G2 info
	G2_STATUS                 m_g2Status;
	std::string               m_g2User;
	std::string               m_g2Repeater;
	std::string               m_g2Gateway;
	CHeaderData*              m_g2Header;
	in_addr                   m_g2Address;

	// Link info
	LINK_STATUS               m_linkStatus;
	std::string               m_linkRepeater;
	std::string               m_linkGateway;
	RECONNECT                 m_linkReconnect;
	bool                      m_linkAtStartup;
	std::string               m_linkStartup;
	CTimer                    m_linkReconnectTimer;
	bool                      m_linkRelink;

	// Echoing
	CEchoUnit*                m_echo;

	// Voice messages
	CAudioUnit*               m_infoAudio;
	bool                      m_infoNeeded;
	CAnnouncementUnit*        m_msgAudio;
	bool                      m_msgNeeded;
	CAnnouncementUnit*        m_wxAudio;
	bool                      m_wxNeeded;

	// Version information
	CVersionUnit*             m_version;

	// D-RATS handler
//	CDRATSServer*             m_drats;

	// DTMF commands
	CDTMF                     m_dtmf;

	// Poll timer
	CTimer                    m_pollTimer;

	// CCS
	CCCSHandler*              m_ccsHandler;

	// Reflector restoration
	std::string               m_lastReflector;

	// Icom heard data
	std::string               m_heardUser;
	std::string               m_heardRepeater;
	CTimer                    m_heardTimer;

	void g2CommandHandler(const std::string& callsign, const std::string& user, CHeaderData& header);
	void ccsCommandHandler(const std::string& callsign, const std::string& user, const std::string& type);
	void reflectorCommandHandler(const std::string& callsign, const std::string& user, const std::string& type);
	void sendToOutgoing(const CHeaderData& header);
	void sendToOutgoing(const CAMBEData& data);
	void sendToIncoming(const CHeaderData& header);
	void sendToIncoming(const CAMBEData& data);

	void writeIsBusy(const std::string& callsign);
	void writeLinkedTo(const std::string& callsign);
	void writeLinkingTo(const std::string& callsign);
	void writeNotLinked();

	void sendHeard(const std::string& text = std::string(""));
	void sendStats();

	void linkInt(const std::string& callsign);

	void suspendLinks();
	bool restoreLinks();

	void triggerInfo();

	bool isCCSCommand(const std::string& command) const;
};
