#include "TcpClientThread.h"
#include "Configure.h"
#include "LibeventLog.h"
#include "MessageHandler.h"

static void	readcb(struct bufferevent *b, void *arg)
{
	if (b == NULL || arg == NULL) {
		return;
	}

	TcpClientThread *client = (TcpClientThread *)arg;
	struct evbuffer *input = bufferevent_get_input(b);
	if (evbuffer_get_length(input) == 0) {
		LibeventLog::userLog("input data len is 0");
		bufferevent_free(b);
	}

	char ch = 0;
	int len = bufferevent_read(b, &ch, sizeof(char));
	if (len != sizeof(char)) {
		return;
	}

	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	if (ch == cfg->notify[0]) {		
	} else if (ch == cfg->heartbeat[0]) {
	} else {
	}
}

static void writecb(struct bufferevent *bev, void *arg)
{
	if (bev == NULL || arg == NULL) {
		return;
	}

	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	TcpClientThread *client = (TcpClientThread *)arg;
	int ret = client->scanQueueMessage();
	if (ret > 0) {
		client->replyToServer(bev, cfg->notify.c_str());
		client->yield();
		client->milliSleep(cfg->sleep_mill);
	} else {
		
		client->replyToServer(bev, cfg->heartbeat.c_str());
		client->milliSleep(cfg->sleep_mill);
	}
}

static void	errorcb(struct bufferevent *b, short what, void *arg)
{
	TcpClientThread *client = (TcpClientThread *)arg;
	
	if (what & BEV_EVENT_EOF) {
	} else {
	}

	bufferevent_setcb(b, NULL, NULL, NULL, NULL);
	bufferevent_disable(b, EV_READ|EV_WRITE);
	bufferevent_free(b);
	b = NULL;

	LibeventLog::userLog("TcpClientThread errorcb called.");

	//connect again
	int ret = 1;
	while (ret)	{
		client->yield();
		ret = client->connectToServer();
		LibeventLog::userLog("TcpClientThread reconnect server.");
	}
}

int TcpClientThread::replyToServer(struct bufferevent *b, const char *str) {

	int ret = bufferevent_write(b, str, MESSAGE_CHAR_LENGHT);
	if (ret != 0) {
		LibeventLog::userLog("bufferevent_write failed");
		return -1;
	}
	return 0;
}

void TcpClientThread::frob_socket(evutil_socket_t sock)
{
	struct linger l;
	int one = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one))<0) {
		LibeventLog::userLog("setsockopt(SO_REUSEADDR)");
	}

	l.l_onoff = 1;
	l.l_linger = 0;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&l, sizeof(l))<0) {
		LibeventLog::userLog("setsockopt(SO_LINGER)");
	}
}

int TcpClientThread::connectToServer() {
	evutil_socket_t sock;
	struct sockaddr_in sin;

	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0x7f000001);
	sin.sin_port = htons(cfg->heartbeat_port);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		LibeventLog::userLog("create socket failed");
		return -1;
	}

	if (evutil_make_socket_nonblocking(sock) < 0) {
		LibeventLog::userLog("evutil_make_socket_nonblocking failed");
		return -1;
	}

	frob_socket(sock);
	if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
#ifdef WIN32
		if ((errno == EINTR || errno == EINPROGRESS)) {
#else
		if (!(errno == EINTR || errno == EINPROGRESS)) {
#endif
			LibeventLog::userLog("errno == EINTR || errno == EINPROGRESS");
			return -1;
		}
	}
	
	bev = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_setcb(bev, readcb, writecb, errorcb, this);
	bufferevent_enable(bev, EV_READ|EV_WRITE);
	return 0;
}


int TcpClientThread::run() {
	LibeventLog::userLog("TcpClientThread start ...");
	int ret = connectToServer();

	event_base_dispatch(base);
	LibeventLog::userLog("TcpClientThread exit ...");
	return ret;
}

//return count in queue
int TcpClientThread::scanQueueMessage() {
	if (input != NULL) {
		return input->scanQueueMessage();
	}
	
	return 0;
}
