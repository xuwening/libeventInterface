#pragma once
#include "AbstractServer.h"
#include "httpCommon.h"

class TimeoutServer : public AbstractServer
{
public:
	TimeoutServer(void);
	~TimeoutServer(void);

	int listen(struct event_base* base);
	MessageHandler *createMsgHandler();
private:
	struct event *timeout;
	struct timeval tv;
};

