# Copyright (c) 2017 by Thomas A. Early N7TAE
BINDIR=/usr/local/bin
CFGDIR=/usr/local/etc
LOGDIR=/var/log

CPPFLAGS=-W -Wall -I/usr/include -std=c++11 -DDATA_DIR=\"$(CFGDIR)\" -DDEXTRA_LINK
LDFLAGS=-L/usr/lib -lconfig++

OBJS = AMBEData.o AnnouncementUnit.o APRSCollector.o APRSWriter.o APRSWriterThread.o AudioUnit.o CacheManager.o CallsignList.o CCITTChecksum.o CCSData.o CCSHandler.o CCSProtocolHandler.o ConnectData.o DCSHandler.o DCSProtocolHandler.o DCSProtocolHandlerPool.o DDData.o DDHandler.o DExtraHandler.o DExtraProtocolHandler.o DExtraProtocolHandlerPool.o DPlusAuthenticator.o DPlusHandler.o DPlusProtocolHandler.o DPlusProtocolHandlerPool.o DRATSServer.o DTMF.o DVTOOLFileReader.o EchoUnit.o G2Handler.o G2ProtocolHandler.o GatewayCache.o HeaderData.o HeaderLogger.o HeardData.o PollData.o RemoteHandler.o RemoteLinkData.o RemoteProtocolHandler.o RemoteRepeaterData.o RemoteStarNetUser.o RemoteStarNetGroup.o RepeaterCache.o RepeaterHandler.o SHA256.o SlowDataEncoder.o StarNetHandler.o StarNetServerConfig.o StarNetServerThread.o StatusData.o TCPReaderWriterClient.o TCPReaderWriterServer.o TextCollector.o TextData.o Timer.o UserCache.o UDPReaderWriter.o Utils.o VersionUnit.o

IRCOBJS = IRCClient.o IRCDDB.o IRCDDBApp.o IRCDDBClient.o IRCMessage.o IRCMessageQueue.o IRCProtocol.o IRCReceiver.o

smartgroupserver : StarNetServerAppD.cpp StarNetServerAppD.h $(OBJS) $(IRCOBJS) StarNetServerConfig.h StarNetServerDefs.h APRSWriter.h Version.h IRCDDBClient.h Utils.h
	g++ $(CPPFLAGS) StarNetServerAppD.cpp -o smartgroupserver $(OBJS) $(IRCOBJS) -L/usr/lib -lconfig++ -pthread

AMBEData.o : AMBEData.cpp AMBEData.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) AMBEData.cpp

AnnouncementUnit.o : AnnouncementUnit.cpp AnnouncementUnit.h DStarDefines.h HeaderData.h Utils.h
	g++ -c $(CPPFLAGS) AnnouncementUnit.cpp

APRSCollector.o : APRSCollector.cpp APRSCollector.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) APRSCollector.cpp

APRSWriter.o : APRSWriter.cpp APRSWriter.h Utils.h
	g++ -c $(CPPFLAGS) APRSWriter.cpp

APRSWriterThread.o : APRSWriterThread.cpp APRSWriterThread.h DStarDefines.h Utils.h Defs.h
	g++ -c $(CPPFLAGS) APRSWriterThread.cpp

AudioUnit.o : AudioUnit.cpp AudioUnit.h DStarDefines.h HeaderData.h Utils.h
	g++ -c $(CPPFLAGS) AudioUnit.cpp

CacheManager.o : CacheManager.cpp CacheManager.h DStarDefines.h
	g++ -c $(CPPFLAGS) CacheManager.cpp

CallsignList.o : CallsignList.cpp CallsignList.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) CallsignList.cpp

CCITTChecksum.o : CCITTChecksum.cpp CCITTChecksum.h Utils.h
	g++ -c $(CPPFLAGS) CCITTChecksum.cpp

CCSData.o : CCSData.cpp CCSData.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) CCSData.cpp

CCSHandler.o : CCSHandler.cpp CCSHandler.h RepeaterHandler.h Utils.h
	g++ -c $(CPPFLAGS) CCSHandler.cpp

CCSProtocolHandler.o : CCSProtocolHandler.cpp CCSProtocolHandler.h Utils.h
	g++ -c $(CPPFLAGS) CCSProtocolHandler.cpp

ConnectData.o : ConnectData.cpp ConnectData.h DStarDefines.h Version.h Utils.h
	g++ -c $(CPPFLAGS) ConnectData.cpp

DCSHandler.o : DCSHandler.cpp DCSHandler.h RepeaterHandler.h Utils.h
	g++ -c $(CPPFLAGS) DCSHandler.cpp

DCSProtocolHandler.o : DCSProtocolHandler.cpp DCSProtocolHandler.h Utils.h
	g++ -c $(CPPFLAGS) DCSProtocolHandler.cpp

DCSProtocolHandlerPool.o : DCSProtocolHandlerPool.cpp DCSProtocolHandlerPool.h Utils.h
	g++ -c $(CPPFLAGS) DCSProtocolHandlerPool.cpp

DDData.o : DDData.cpp DDData.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) DDData.cpp

DDHandler.o : DDHandler.cpp DDHandler.h RepeaterHandler.h Defs.h Utils.h
	g++ -c $(CPPFLAGS) DDHandler.cpp

DExtraHandler.o : DExtraHandler.cpp DExtraHandler.h RepeaterHandler.h Utils.h
	g++ -c $(CPPFLAGS) DExtraHandler.cpp

DExtraProtocolHandler.o : DExtraProtocolHandler.cpp DExtraProtocolHandler.h Utils.h
	g++ -c $(CPPFLAGS) DExtraProtocolHandler.cpp

DExtraProtocolHandlerPool.o : DExtraProtocolHandlerPool.cpp DExtraProtocolHandlerPool.h Utils.h
	g++ -c $(CPPFLAGS) DExtraProtocolHandlerPool.cpp

DPlusAuthenticator.o : DPlusAuthenticator.cpp DPlusAuthenticator.h UDPReaderWriter.h DStarDefines.h Utils.h Defs.h
	g++ -c $(CPPFLAGS) DPlusAuthenticator.cpp

DPlusHandler.o : DPlusHandler.cpp DPlusHandler.h RepeaterHandler.h Utils.h
	g++ -c $(CPPFLAGS) DPlusHandler.cpp

DPlusProtocolHandler.o : DPlusProtocolHandler.cpp DPlusProtocolHandler.h Utils.h
	g++ -c $(CPPFLAGS) DPlusProtocolHandler.cpp

DPlusProtocolHandlerPool.o : DPlusProtocolHandlerPool.cpp DPlusProtocolHandlerPool.h Utils.h
	g++ -c $(CPPFLAGS) DPlusProtocolHandlerPool.cpp

DRATSServer.o : DRATSServer.cpp DRATSServer.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) DRATSServer.cpp

DTMF.o : DTMF.cpp DTMF.h
	g++ -c $(CPPFLAGS) DTMF.cpp

DVTOOLFileReader.o : DVTOOLFileReader.cpp DVTOOLFileReader.h DStarDefines.h
	g++ -c $(CPPFLAGS) DVTOOLFileReader.cpp

EchoUnit.o : EchoUnit.cpp EchoUnit.h DStarDefines.h Defs.h Utils.h
	g++ -c $(CPPFLAGS) EchoUnit.cpp

G2Handler.o : G2Handler.cpp G2Handler.h StarNetHandler.h Utils.h Defs.h
	g++ -c $(CPPFLAGS) G2Handler.cpp

G2ProtocolHandler.o : G2ProtocolHandler.cpp G2ProtocolHandler.h Utils.h
	g++ -c $(CPPFLAGS) G2ProtocolHandler.cpp

GatewayCache.o : GatewayCache.cpp GatewayCache.h
	g++ -c $(CPPFLAGS) GatewayCache.cpp

HeaderData.o : HeaderData.cpp HeaderData.h CCITTChecksum.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) HeaderData.cpp

HeaderLogger.o : HeaderLogger.cpp HeaderLogger.h Utils.h Defs.h
	g++ -c $(CPPFLAGS) HeaderLogger.cpp

HeardData.o : HeardData.cpp HeardData.h
	g++ -c $(CPPFLAGS) HeardData.cpp

IRCClient.o : IRCClient.cpp IRCClient.h Utils.h
	g++ -c $(CPPFLAGS) IRCClient.cpp

IRCDDB.o : IRCDDB.cpp IRCDDB.h
	g++ -c $(CPPFLAGS) IRCDDB.cpp

IRCDDBApp.o : IRCDDBApp.cpp IRCDDBApp.h Utils.h
	g++ -c $(CPPFLAGS) IRCDDBApp.cpp

IRCDDBClient.o : IRCDDBClient.cpp IRCDDBClient.h IRCClient.h IRCDDBApp.h Utils.h
	g++ -c $(CPPFLAGS) IRCDDBClient.cpp

IRCMessage.o : IRCMessage.cpp IRCMessage.h
	g++ -c $(CPPFLAGS) IRCMessage.cpp

IRCMessageQueue.o : IRCMessageQueue.cpp IRCMessageQueue.h
	g++ -c $(CPPFLAGS) IRCMessageQueue.cpp

IRCProtocol.o : IRCProtocol.cpp IRCProtocol.h Utils.h
	g++ -c $(CPPFLAGS) IRCProtocol.cpp

IRCReceiver.o : IRCReceiver.cpp IRCReceiver.h Utils.h
	g++ -c $(CPPFLAGS) IRCReceiver.cpp

PollData.o : PollData.cpp PollData.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) PollData.cpp

RemoteHandler.o : RemoteHandler.cpp RemoteHandler.h RepeaterHandler.h StarNetHandler.h DExtraHandler.h DPlusHandler.h DStarDefines.h DCSHandler.h Utils.h
	g++ -c $(CPPFLAGS) RemoteHandler.cpp

RemoteLinkData.o : RemoteLinkData.cpp RemoteLinkData.h
	g++ -c $(CPPFLAGS) RemoteLinkData.cpp

RemoteProtocolHandler.o : RemoteProtocolHandler.cpp RemoteProtocolHandler.h DStarDefines.h SHA256.h Utils.h
	g++ -c $(CPPFLAGS) RemoteProtocolHandler.cpp

RemoteRepeaterData.o : RemoteRepeaterData.cpp RemoteRepeaterData.h
	g++ -c $(CPPFLAGS) RemoteRepeaterData.cpp

RemoteStarNetUser.o : RemoteStarNetUser.cpp RemoteStarNetUser.h
	g++ -c $(CPPFLAGS) RemoteStarNetUser.cpp

RemoteStarNetGroup.o : RemoteStarNetGroup.cpp RemoteStarNetGroup.h RemoteStarNetUser.h
	g++ -c $(CPPFLAGS) RemoteStarNetGroup.cpp

RepeaterCache.o : RepeaterCache.cpp RepeaterCache.h
	g++ -c $(CPPFLAGS) RepeaterCache.cpp

RepeaterHandler.o : RepeaterHandler.cpp RepeaterHandler.h DExtraHandler.h DPlusHandler.h DStarDefines.h DCSHandler.h CCSHandler.h HeaderData.h DDHandler.h AMBEData.h Utils.h
	g++ -c $(CPPFLAGS) RepeaterHandler.cpp

SHA256.o : SHA256.cpp SHA256.h
	g++ -c $(CPPFLAGS) SHA256.cpp

SlowDataEncoder.o : SlowDataEncoder.cpp SlowDataEncoder.h CCITTChecksum.h DStarDefines.h
	g++ -c $(CPPFLAGS) SlowDataEncoder.cpp

StarNetHandler.o : StarNetHandler.cpp StarNetHandler.h SlowDataEncoder.h RepeaterHandler.h DExtraHandler.h DStarDefines.h DCSHandler.h Utils.h
	g++ -c $(CPPFLAGS) StarNetHandler.cpp

StarNetServerConfig.o : StarNetServerConfig.cpp StarNetServerConfig.h Utils.h
	g++ -c $(CPPFLAGS) StarNetServerConfig.cpp

StarNetServerThread.o : StarNetServerThread.cpp StarNetServerThread.h StarNetServerDefs.h StarNetHandler.h DExtraHandler.h DCSHandler.h HeaderData.h G2Handler.h AMBEData.h Utils.h
	g++ -c $(CPPFLAGS) StarNetServerThread.cpp

StatusData.o : StatusData.cpp StatusData.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) StatusData.cpp

TCPReaderWriterClient.o : TCPReaderWriterClient.cpp TCPReaderWriterClient.h UDPReaderWriter.h Utils.h
	g++ -c $(CPPFLAGS) TCPReaderWriterClient.cpp

TCPReaderWriterServer.o : TCPReaderWriterServer.cpp TCPReaderWriterServer.h Utils.h
	g++ -c $(CPPFLAGS) TCPReaderWriterServer.cpp

TextCollector.o : TextCollector.cpp TextCollector.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) TextCollector.cpp

TextData.o : TextData.cpp TextData.h DStarDefines.h Utils.h
	g++ -c $(CPPFLAGS) TextData.cpp

Timer.o : Timer.cpp Timer.h
	g++ -c $(CPPFLAGS) Timer.cpp

UDPReaderWriter.o : UDPReaderWriter.cpp
	g++ -c $(CPPFLAGS) UDPReaderWriter.cpp

UserCache.o : UserCache.cpp UserCache.h
	g++ -c $(CPPFLAGS) UserCache.cpp

Utils.o : Utils.cpp Utils.h
	g++ -c $(CPPFLAGS) Utils.cpp

VersionUnit.o : VersionUnit.cpp VersionUnit.h SlowDataEncoder.h DStarDefines.h HeaderData.h Version.h Utils.h
	g++ -c $(CPPFLAGS) VersionUnit.cpp

AnnouncementData.h : DVTOOLFileReader.h RepeaterCallback.h AMBEData.h Timer.h Defs.h
APRSCollector.h : Defs.h
APRSWriter.h : APRSWriterThread.h APRSCollector.h DStarDefines.h AMBEData.h Timer.h Defs.h
APRSWriterThread.h : TCPReaderWriterClient.h
AudioUnit.h : RepeaterCallback.h SlowDataEncoder.h AMBEData.h Timer.h Defs.h
CacheManager.h : RepeaterCache.h GatewayCache.h UserCache.h
CCSCallback.h : DStarDefines.h HeaderData.h AMBEData.h Defs.h
CCSHandler.h : CCSProtocolHandler.h DStarDefines.h HeaderLogger.h ConnectData.h CCSCallback.h AMBEData.h PollData.h Timer.h Defs.h
CCSProtocolHandler.h : UDPReaderWriter.h DStarDefines.h ConnectData.h HeardData.h AMBEData.h PollData.h CCSData.h
ConnectData.h : Defs.h
DCSHandler.h : DCSProtocolHandler.h ReflectorCallback.h DStarDefines.h HeaderLogger.h CallsignList.h ConnectData.h AMBEData.h PollData.h Timer.h Defs.h
DCSProtocolHandler.h : UDPReaderWriter.h DStarDefines.h ConnectData.h AMBEData.h PollData.h
DCSProtocolHandlerPool.h : DCSProtocolHandler.h
DDData.h : HeaderData.h
DDHandler.h : HeaderLogger.h DDData.h IRCDDB.h Timer.h
DExtraHandler.h : DExtraProtocolHandlerPool.h ReflectorCallback.h DStarDefines.h HeaderLogger.h CallsignList.h ConnectData.h HeaderData.h AMBEData.h PollData.h Timer.h Defs.h
DExtraProtocolHandler.h : UDPReaderWriter.h DStarDefines.h ConnectData.h HeaderData.h AMBEData.h PollData.h
DPlusAuthenticator.h : TCPReaderWriterClient.h CacheManager.h Timer.h
DPlusHandler.h : DPlusProtocolHandler.h DPlusAuthenticator.h ReflectorCallback.h CacheManager.h DStarDefines.h HeaderLogger.h CallsignList.h ConnectData.h HeaderData.h AMBEData.h PollData.h Timer.h Defs.h
DPlusProtocolHandler.h : UDPReaderWriter.h DStarDefines.h ConnectData.h HeaderData.h AMBEData.h PollData.h
DRATSServer.h : TCPReaderWriterServer.h RepeaterCallback.h HeaderData.h AMBEData.h Defs.h
DVTOOLFileReader.h : HeaderData.h AMBEData.h
EchoUnit.h : RepeaterCallback.h HeaderData.h AMBEData.h Timer.h
G2Handler.h : G2ProtocolHandler.h RepeaterHandler.h DStarDefines.h HeaderLogger.h HeaderData.h AMBEData.h Timer.h
G2ProtocolHandler.h : UDPReaderWriter.h DStarDefines.h HeaderData.h AMBEData.h
HeardData.h : DStarDefines.h HeaderData.h
HeaderLogger.h : HeaderData.h DDData.h
IRCApplication.h : IRCMessageQueue.h
IRCClient.h : IRCApplication.h IRCProtocol.h IRCMessageQueue.h IRCReceiver.h
IRCDDBApp.h : IRCApplication.h IRCDDB.h
IRCDDBClient.h : IRCDDB.h
IRCMessageQueue.h : IRCMessage.h
IRCProtocol.h : IRCMessageQueue.h IRCApplication.h
IRCReceiver.h : IRCMessageQueue.h
PollData.h : Defs.h
ReflectorCallback.h : DStarDefines.h HeaderData.h AMBEData.h Defs.h
RemoteHandler.h : RemoteProtocolHandler.h Timer.h
RemoteLinkData.h : Defs.h
RemoteProtocolHandler.h : RemoteStarNetGroup.h RemoteRepeaterData.h UDPReaderWriter.h Defs.h
RemoteRepeaterData.h : RemoteLinkData.h
RepeaterCallback.h : DStarDefines.h HeaderData.h AMBEData.h Defs.h
RepeaterHandler.h : RepeaterProtocolHandler.h DExtraProtocolHandler.h DPlusProtocolHandler.h RemoteRepeaterData.h G2ProtocolHandler.h ReflectorCallback.h RepeaterCallback.h AnnouncementUnit.h StarNetHandler.h TextCollector.h CacheManager.h HeaderLogger.h CallsignList.h DRATSServer.h CCSCallback.h VersionUnit.h CCSHandler.h StatusData.h APRSWriter.h HeardData.h AudioUnit.h EchoUnit.h PollData.h DDData.h IRCDDB.h Timer.h DTMF.h Defs.h
SlowDataEncoder.h : HeaderData.h
StarNetHandler.h : RemoteStarNetGroup.h G2ProtocolHandler.h ReflectorCallback.h RepeaterCallback.h TextCollector.h CacheManager.h HeaderData.h AMBEData.h IRCDDB.h Timer.h
StarNetServerAppD.h : StarNetServerThread.h
StarNetServerConfig.h : Defs.h
StarNetServerThread.h : DExtraProtocolHandlerPool.h DCSProtocolHandler.h G2ProtocolHandler.h RemoteHandler.h CacheManager.h IRCDDB.h Timer.h Defs.h
TCPReaderWriterServer.h : TCPReaderWriterClient.h
TextCollector.h : AMBEData.h Defs.h
TextData.h : Defs.h
VersionUnit.h : RepeaterCallback.h AMBEData.h Timer.h Defs.h

