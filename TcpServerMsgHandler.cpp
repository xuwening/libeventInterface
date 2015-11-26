#include "TcpServerMsgHandler.h"
#include "HttpRequest.h"
#include "LibeventLog.h"

static void callback_func(E_RESPONSE_TYPE type, void *data, unsigned int len, void *arg) {

	switch (type) {
	case RESPONSE_OK:
		//LibeventLog::userLog("[response to user] res ok: ");
		break;
	case RESPONSE_ERROR:
		LibeventLog::userLog("[response to user] res err: %d ", *((int*)data));
		break;
	case REQUEST_TIMEOUT:
		LibeventLog::userLog("[response to user] request timeout.");
		break;
	case CONNECTION_EXCEPTION:
		LibeventLog::userLog("[response to user] connection exception.");
		break;
	default:
		LibeventLog::userLog("[response to user] unknown.");
		break;
	}
}

int TcpServerMsgHandler::processMessage(void *arg) {
	char *buf = "post xml data";
	char *code = "291";
	char *usr_arg = "ssssssssssssssssssssssss";
	
	HttpRequest *req = new HttpRequest((struct event_base *)arg);
	int ret = req->postHttpRequest(buf, strlen(buf), callback_func, usr_arg);
	if (ret != 0 ) {
		LibeventLog::userLog("[postHttpRequest] error.");
		delete req;
		return -1;
	}
	req->autoRelease();
	
	return 0;
}