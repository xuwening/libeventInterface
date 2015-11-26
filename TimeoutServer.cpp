#include "TimeoutServer.h"
#include "HttpRequest.h"
#include "LibeventLog.h"

int g_flag = 1;
int times = 0;
TimeoutServer::TimeoutServer(void) {
	timeout = NULL;
}


TimeoutServer::~TimeoutServer(void) {
	if (timeout != NULL) {
		event_free(timeout);
	}
	
	LibeventLog::userLog("TimeoutServer exit ...");
}


static void	timeout_cb(evutil_socket_t fd, short event, void *arg) {
	if (g_flag)
		g_flag = 0;
	times++;
	if (times >= 5)
		event_base_loopbreak((struct event_base *)arg);
}


int TimeoutServer::listen(struct event_base* base) {
	
	timeout = event_new(base, -1, EV_PERSIST, timeout_cb, base);

	evutil_timerclear(&tv);
	tv.tv_sec = 3;
	tv.tv_usec = 0;

	int ret = event_add(timeout, &tv);
	if (ret) {
		printf("err init event: %d\n", ret);
		return -1;
	}
	LibeventLog::userLog("TimeoutServer start ...");
	return 0;
}

MessageHandler *TimeoutServer::createMsgHandler() {
	//create MessageHandler
	return NULL;
}