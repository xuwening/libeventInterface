#include "HttpServer.h"
#include "ConnectionManager.h"
#include "HttpServerMsgHandler.h"
#include "Configure.h"
#include "LibeventLog.h"


HttpServer::HttpServer(void) {
	
	setUriRequestFlag(0);
	setRequestObj(NULL);
	http_serv = NULL;
};

HttpServer::~HttpServer() {
	if (http_serv != NULL) {
		evhttp_free(http_serv);
	}
	LibeventLog::userLog("HttpServer exit ...");
}

static void	statistic_request_cb(struct evhttp_request *req, void *arg)
{
	HttpServer *server = (HttpServer *)arg;
	if (server != NULL) {
		server->setRequestObj(req);
		server->processMessage();
		server->setUriRequestFlag(1);
	}
}

static void	statistic_clear_cb(struct evhttp_request *req, void *arg)
{
	ConnectionManager *cfg = ConnectionManager::getInstance();
	cfg->clearStatictis();
	statistic_request_cb(req, arg);
}

//SERVER CALLBACK
void generic_request_handler(struct evhttp_request *req, void *arg) {
	HttpServer *server = (HttpServer *)arg;
	if (server != NULL && server->getUriRequestFlag()) {
		return;
	}

	//statistic_request_cb(req, arg);
}

int HttpServer::listen(struct event_base* base) {
	int ret = 0;
	http_serv = evhttp_new(base);
	if (http_serv == NULL) {
		LibeventLog::userLog("evhttp_new failed.\n");
		return -1;
	}

	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	CHECK_CALL_RET(evhttp_bind_socket(http_serv, "0.0.0.0",cfg->statis_port), ret);
	CHECK_CALL_RET(evhttp_set_cb(http_serv, "/statistic", statistic_request_cb, this), ret);
	CHECK_CALL_RET(evhttp_set_cb(http_serv, "/clear", statistic_clear_cb, this), ret);
	evhttp_set_gencb(http_serv, generic_request_handler, this);
	LibeventLog::userLog("HttpServer start ...");
	return ret;
}

MessageHandler *HttpServer::createMsgHandler() {
	return new HttpServerMsgHandler();
}

int HttpServer::processMessage() {
	MessageHandler *handler = getMsgHandler();
	if (handler == NULL) {
		return -1;
	}

	return handler->processMessage(req);
}