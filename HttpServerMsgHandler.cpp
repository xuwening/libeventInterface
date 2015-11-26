#include "HttpServerMsgHandler.h"
#include "HttpRequest.h"


int HttpServerMsgHandler::processMessage(void *arg) {
	struct evhttp_request *req = (struct evhttp_request *)arg;

	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
			printf("  %s: %s\n", header->key, header->value);
	}

	ConnectionManager *conn = ConnectionManager::getInstance();
	struct evbuffer *return_buffer=evbuffer_new();

	long long total_req = conn->getTotalRequests();
	long long total_res = conn->getTotalResponses();
	unsigned int conns = conn->getConnections();

	long long timeouts = conn->getTimeoutRequests();
	long long err_res = conn->getErrorResponses();
	long long conn_except = conn->getConnExceptions();
	long long ok_res = conn->getOkResponses();

	evbuffer_add_printf(return_buffer,"    total request: %lld<br>	   total response: %lld<br>	   connnections: %u<br><br><hr><br>	ok: %lld	\
		<br>	timeout: %lld<br>	http error: %lld<br>    connExcept: %u<br>"
		, total_req, total_res, conns, ok_res, timeouts, err_res, conn_except);
	evhttp_send_reply(req, HTTP_OK, "OK", return_buffer);
	evbuffer_free(return_buffer);

	return 0;
}