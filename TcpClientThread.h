#pragma once

#include "httpCommon.h"
#include "Threadable.h"
#include "InputHandler.h"
#include "AbstractServer.h"

class TcpClientThread : public Threadable
{
public:
	TcpClientThread(InputHandler *handler) {
		base = event_base_new();
		bev = NULL;
		input = handler;
	};

	~TcpClientThread(void) {
		if (bev) {bufferevent_free(bev);};
		if (base) {event_base_free(base);};
		delete input;
	};

	virtual int run();
	int connectToServer();
	int replyToServer(struct bufferevent *b, const char *str);
	void exitThread() {event_base_loopbreak(base);};
	int scanQueueMessage();

private:
	
	void frob_socket(evutil_socket_t sock);
	struct event_base *base;
	struct bufferevent *bev;

	InputHandler *input;
};

