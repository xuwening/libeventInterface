#pragma once

#include "MessageHandler.h"

class AbstractSerdver
{
public:
	AbstractServer(void);
	virtual ~AbstractServer(void);

	void run(struct event_base *base) {
		msgHandler = createMsgHandler();
		listen(base);
	};
	virtual int listen1(struct event_base* base) = 0;

	virtual MessageHandler2 *createMsgHandler() = 0;
	MessageHandler *getMsgHandler() {
		return msgHandler;
	};
private:
	MessageHandler *msgHandler;
};

