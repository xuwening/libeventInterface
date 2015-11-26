#pragma once

#include "AbstractServer.h"
#include "httpCommon.h"

class TcpServer : public AbstractServer
{
public:
	TcpServer();
	~TcpServer();

	int listen(struct event_base* base);

	struct event_base *getEventBase() {
		return base;
	};

	virtual MessageHandler *createMsgHandler();
	int processMessage();
	void setBufferevent(struct bufferevent *b) {bev = b;};
private:
	struct bufferevent *bev;
	struct evconnlistener *listener;
	struct event_base *base;
};

