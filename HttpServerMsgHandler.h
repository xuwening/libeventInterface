#pragma once
#include "MessageHandler.h"


class HttpServerMsgHandler : public MessageHandler
{
public:
	HttpServerMsgHandler(void){};
	virtual ~HttpServerMsgHandler(void){};

	int processMessage(void *arg);
};

