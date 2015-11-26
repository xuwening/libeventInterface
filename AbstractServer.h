#pragma once

#include "MessageHandler.h"

class AbstractServer
{
public:
	AbstractServer(void);
	virtual ~AbstractServer(void);

	void run(struct event_base *base) {
		msgHandler = createMsgHandler();
		listen(base);
	};
	virtual int listen(struct event_base* base) = 0;

	virtual MessageHandler *createMsgHandler() = 0;
	MessageHandler *getMsgHandler() {
		return msgHandler;
	};
private:
	MessageHandler *msgHandler;
};

