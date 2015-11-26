#include "TcpServer.h"
#include "TcpServerMsgHandler.h"
#include "Configure.h"
#include "LibeventLog.h"
#include "ConnectionManager.h"
#include "Threadable.h"

TcpServer::TcpServer() {
	listener = NULL;
	base = NULL;
	bev = NULL;
}

TcpServer::~TcpServer() {
	if (bev != NULL) {
		bufferevent_free(bev);
	}

	if (listener != NULL) {
		evconnlistener_free(listener);
	}

	LibeventLog::userLog("TcpServer exit ...");
}

static void conn_readcb(struct bufferevent *bev, void *user_data)
{
	if (bev == NULL || user_data == NULL) {
		LibeventLog::userLog("conn_readcb bev or user_data is null.");
		return;
	}

	//user obj
	TcpServer *server = (TcpServer *)user_data;

	struct evbuffer *input = bufferevent_get_input(bev);
	if (evbuffer_get_length(input) == 0) {
		LibeventLog::userLog("input data len is 0");
		bufferevent_free(bev);
	}

	char ch = 0;
	int len = bufferevent_read(bev, &ch, sizeof(char));
	if (len != sizeof(char)) {
		return;
	}

	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	if (ch == cfg->notify[0]) {
		server->processMessage();
	} else if (ch == cfg->heartbeat[0]) {
	} else {
	}

	ConnectionManager *cmg = ConnectionManager::getInstance();
	cmg->recycleConnection();
}

//static void conn_writecb(struct bufferevent *bev, void *user_data)
//{
//	struct evbuffer *output = bufferevent_get_output(bev);
//	if (evbuffer_get_length(output) == 0) {
//		printf("flushed answer\n");
//		bufferevent_free(bev);
//	}
//}

static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	if (bev == NULL || user_data == NULL) {
		LibeventLog::userLog("tcpServer conn_eventcb arg[bev or user_data] is NULL.");
		return;
	}

	if (events & BEV_EVENT_EOF) {
		LibeventLog::userLog("Connection closed.");
	} else if (events & BEV_EVENT_ERROR) {
		LibeventLog::userLog("Got an error on the connection:");
	}

	bufferevent_free(bev);
}

static void	listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
struct sockaddr *sa, int socklen, void *arg) {

	TcpServer *server = (TcpServer *)arg;
	struct bufferevent *bev;
	bev = bufferevent_socket_new(server->getEventBase(), fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		LibeventLog::userLog("Error constructing bufferevent!");
		event_base_loopbreak(server->getEventBase());
		return;
	}
	server->setBufferevent(bev);
	bufferevent_setcb(bev, conn_readcb, NULL, conn_eventcb, arg);
	bufferevent_enable(bev, EV_WRITE | EV_READ);
	//bufferevent_disable(bev, EV_READ);

	//first write
	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	int ret = bufferevent_write(bev, cfg->heartbeat.c_str(), MESSAGE_CHAR_LENGHT);
	if (ret != 0) {
		LibeventLog::userLog("server send data failed");
	}
}

int TcpServer::listen(struct event_base* base) {
	
	struct sockaddr_in sin;

	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);

	this->base = base;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(cfg->heartbeat_port);

	listener = evconnlistener_new_bind(base, listener_cb, this,
		LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
		(struct sockaddr*)&sin,
		sizeof(sin));

	if (!listener) {
		LibeventLog::userLog("Could not create a listener!\n");
		return 1;
	}

	LibeventLog::userLog("TcpServer start ...");
	
	return 0;
}

MessageHandler *TcpServer::createMsgHandler() {
	return new TcpServerMsgHandler();
}

int TcpServer::processMessage() {
	MessageHandler *handler = getMsgHandler();
	if (handler == NULL) {
		LibeventLog::userLog("TcpServer getMsgHandler failed.");
		return -1;
	}

	ConnectionManager *cmg = ConnectionManager::getInstance();
	cmg->requestInc();
	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	if (cfg != NULL) {
		int conn = cmg->getConnections();
		int req_per = cmg->getCurrentRequest();
		if ((cfg->max_connections != -1 && conn >= cfg->max_connections) 
			|| (cfg->req_per_second != -1 && req_per >= cfg->req_per_second)) {
			Threadable::milliSleep(10);
		}
	}

	return handler->processMessage(base);
}