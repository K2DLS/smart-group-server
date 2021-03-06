#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include "IRCClient.h"
#include "Utils.h"


IRCClient::IRCClient(IRCApplication *app, const std::string &update_channel, const std::string &hostName, unsigned int port, const std::string &callsign, const std::string &password, const std::string &versionInfo)
{
	safeStringCopy(host_name, hostName.c_str(), sizeof host_name);


	this->callsign = callsign;
	ToLower(this->callsign);
	this->port = port;
	this->password = password;

	this->app = app;

	proto = new IRCProtocol(app, this->callsign, password, update_channel, versionInfo);

	recvQ = NULL;
	sendQ = NULL;

	recv = NULL;
}

IRCClient::~IRCClient()
{
	delete proto;
}

bool IRCClient::startWork()
{

	terminateThread = false;
	client_thread = std::async(std::launch::async, &IRCClient::Entry, this);
	return true;
}

void IRCClient::stopWork()
{
	terminateThread = true;
    client_thread.get();
}

#define MAXIPV4ADDR 10
void IRCClient::Entry()
{
	int state = 0;
	int timer = 0;

	while (true) {

		if (timer > 0) {
			timer--;
		}

		switch (state) {
            case 0:
                if (terminateThread) {
                    printf("IRCClient::Entry: thread terminated at state=%d\n", state);
                    return;
                }

                if (timer == 0) {
                    timer = 30;

					if (! ircSock.Open(host_name, AF_UNSPEC, std::to_string(port)))  {
                        state = 4;
                        timer = 0;
                    }
                }
                break;


            case 4:
                recvQ = new IRCMessageQueue();
                sendQ = new IRCMessageQueue();

                recv = new IRCReceiver(&ircSock, recvQ);
                recv->startWork();

                proto->setNetworkReady(true);
                state = 5;
                timer = 0;
	            break;


            case 5:
                if (terminateThread) {
                    state = 6;
                } else {

                    if (recvQ->isEOF()) {
                        timer = 0;
                        state = 6;
                    } else if (proto->processQueues(recvQ, sendQ) == false) {
                        timer = 0;
                        state = 6;
                    }

                    while ((state == 5) && sendQ->messageAvailable()) {
                        IRCMessage * m = sendQ->getMessage();

                        std::string out;

                        m->composeMessage(out);

                        char buf[200];
                        safeStringCopy(buf, out.c_str(), sizeof buf);
                        int len = strlen(buf);

                        if (buf[len - 1] == 10) { // is there a NL char at the end?
                            if (ircSock.Write((unsigned char *)buf, len)) {
                                printf("IRCClient::Entry: short write\n");

                                timer = 0;
                                state = 6;
                            }
                        } else {
                            printf("IRCClient::Entry: no NL at end, len=%d\n", len);

                            timer = 0;
                            state = 6;
                        }

                        delete m;
                    }
                }
                break;

            case 6: {
                if (app != NULL) {
                    app->setSendQ(NULL);
                    app->userListReset();
                }

                proto->setNetworkReady(false);
                recv->stopWork();

                sleep(2);

                delete recv;
                delete recvQ;
                delete sendQ;

                ircSock.Close();

                if (terminateThread) { // request to end the thread
                    printf("IRCClient::Entry: thread terminated at state=%d\n", state);
                    return;
                }

                timer = 30;
                state = 0;  // reconnect to IRC server
            }
            break;
		}   // switch
		usleep(500000);

	}
	return;
}

int IRCClient::GetFamily()
{
	return ircSock.GetFamily();
}
