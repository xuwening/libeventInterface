#pragma once
#include "AbstractServer.h"
#include "httpCommon.h"

class HttpServer : public AbstractServer
{
public:
	HttpServer(void);
	~HttpServer(void);

	void setUriRequestFlag(int flag) {
		uriRequestFlag = flag;
	}

	int getUriRequestFlag() {
		return uriRequestFlag;
	}

	void setRequestObj(struct evhttp_request * request) {
		req = request;
	}

	struct evhttp_request *getRequestObj() {
		return req;
	}

	int listen(struct event_base* base);
	virtual MessageHandler *createMsgHandler();
	int processMessage();
private:
	struct evhttp *http_serv;
	struct evhttp_request *req;
	int uriRequestFlag;
};

