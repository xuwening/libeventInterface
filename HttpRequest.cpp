#include "HttpRequest.h"
#include "LibeventLog.h"
#include "Threadable.h"
#include "Configure.h"

static void http_request_callback(struct evhttp_request *req, void *args) {
	HttpRequest *request = (HttpRequest *)args;
	if (request) {
		request->http_callback(req, args);
	} else {
		LibeventLog::userLog("http_request_get_callback args is NULL");
	}
}

 void HttpRequest::RePostHttpRequest() {

	HttpRequest *request = new HttpRequest(_base);
	request->setStartTime(_start_time);
	int ret = request->postHttpRequest(_post_data, _post_len, userCallback, _user_arg);
	if (ret != 0) {
		LibeventLog::userLog("RePostHttpRequest->postHttpRequest --> failed.");
		delete request;
	}
}

void HttpRequest::http_callback(struct evhttp_request *req, void *args) {
	HttpRequest *request = (HttpRequest *)args;
	request->_end_time = time(NULL);

	ConnectionManager *cmg = ConnectionManager::getInstance();
	cmg->responseInc();
	cmg->addRecycle(this);

	struct evhttp_request *response = req;
	if (response == NULL) {
		E_RESPONSE_TYPE resType = REQUEST_MAX;
		if (isTimeout() || reachTimeout(1)) {
			cmg->timeoutInc();
			resType = REQUEST_TIMEOUT;
			LibeventLog::userLog("[response to user] request timeout.");
		} else {
			cmg->connExcaptionInc();
			resType = CONNECTION_EXCEPTION;
			if (errno != 0) {
				LibeventLog::userLog("make request error : %s.", strerror(errno));
			}

			Threadable::milliSleep(10);
			RePostHttpRequest();
		}

		if (request->userCallback != NULL) {
			request->userCallback(resType, NULL, 0, _user_arg);
		}
		
		return;
	}

	int resCode = evhttp_request_get_response_code(response);
	if (resCode != HTTP_OK) {
		cmg->errorInc();
		 LibeventLog::userLog("[error] response code: %d ", resCode);
		if (request->userCallback != NULL) {
			request->userCallback(RESPONSE_ERROR, &resCode, sizeof(int), _user_arg);
		}
		
	} else {
		cmg->okInc();
		struct evbuffer* buf = evhttp_request_get_input_buffer(response);
		size_t len = evbuffer_get_length(buf);

		char *tmpbuf = (char *)malloc(len+1);
		memcpy(tmpbuf, evbuffer_pullup(buf, -1), len);
		tmpbuf[len] = '\0';

		if (request->userCallback != NULL) {
			request->userCallback(RESPONSE_OK, tmpbuf, len, _user_arg);
		}
		
		free(tmpbuf);
	}
}

int HttpRequest::configureHttpHeader(struct evhttp_request *request) {

	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	for (int i=0; i<cfg->http_header.size(); i+=2) {
		string key = cfg->http_header[i];
		string value = cfg->http_header[i+1];
		if (key.empty() || evhttp_add_header(request->output_headers, key.c_str(), value.c_str()) != 0) {
			continue;
		}
	}

	return 0;
}

int HttpRequest::createHttpRequest(const char *http_url, user_callback cb, void *cb_arg) {
	int ret = 0;
	setUrl(http_url);
	set_cb_arg(cb_arg);

	_uri = evhttp_uri_parse(http_url);
	_conn = evhttp_connection_base_new(_base, NULL, evhttp_uri_get_host(_uri), evhttp_uri_get_port(_uri));
	_request = evhttp_request_new(http_request_callback, this);
	evhttp_connection_set_timeout(_conn, _time_out);
	setUserCallback(cb, cb_arg);

	ret = configureHttpHeader(_request);	
	return ret;
}

int HttpRequest::makeHttpRequest(const char *http_url, evhttp_cmd_type type
	, char *post_data, unsigned int data_len, user_callback cb, void *cb_arg) {

	ConnectionManager *cmg = ConnectionManager::getInstance();

	int ret = 0;
	CHECK_CALL_RET(createHttpRequest(http_url, cb, cb_arg), ret);
	CHECK_CALL_RET(setPostData(post_data, data_len), ret);
	CHECK_CALL_RET(evhttp_make_request(_conn, _request, type, evhttp_uri_get_path(_uri)), ret);
	cmg->connectionInc();
	return ret;
}

//post
int HttpRequest::postHttpRequest(char *post_data, unsigned int data_len, user_callback cb, void *cb_arg) {
	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	return postHttpRequestWithUrl(cfg->url.c_str(), post_data, data_len, cb, cb_arg);
}

int HttpRequest::postHttpRequestWithUrl(const char *http_url, char *post_data, unsigned int data_len, user_callback cb, void *cb_arg) {
	return makeHttpRequest(http_url, EVHTTP_REQ_POST, post_data, data_len, cb, cb_arg);
}

//get
int HttpRequest::getHttpRequest(user_callback cb, void *cb_arg) {
	Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
	return getHttpRequestWithUrl(cfg->url.c_str(), cb, cb_arg);
}

int HttpRequest::getHttpRequestWithUrl(const char *http_url, user_callback cb, void *cb_arg) {
	return makeHttpRequest(http_url, EVHTTP_REQ_GET, NULL, 0, cb, cb_arg);
}
