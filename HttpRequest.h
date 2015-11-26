#pragma once

#include "ConnectionManager.h"
#include "httpCommon.h"
#include "Configure.h"
#include <time.h>

typedef enum {CONNECTION_EXCEPTION, REQUEST_TIMEOUT, RESPONSE_ERROR, RESPONSE_OK, REQUEST_MAX} E_RESPONSE_TYPE;
typedef void (*user_callback)(E_RESPONSE_TYPE resType, void *resData, unsigned int dataLen, void *arg);

class HttpRequest
{
public:

	HttpRequest(struct event_base *base) {
		_conn = NULL;
		_uri = NULL;
		_url = NULL;
		_post_data = NULL;
		_post_len = 0;
		_flag = 0;

		_base = base;

		Configure *cfg = Configure::readConfigFile(CONFIG_FILE_PATH);
		_time_out = cfg->timeout;
		_start_time = time(NULL);
		_end_time = 0;
		_statistic = 0;
		userCallback = NULL;
	}


	~HttpRequest(void) {
		//if (_request) {evhttp_request_free(_request);};
		if (_conn) {evhttp_connection_free(_conn);};
		free(_post_data);
		free(_url);
		if (_url) {evhttp_uri_free(_uri);};
	}

	void http_callback(struct evhttp_request *req, void *args);
	
	//post & get method
	int getHttpRequest(user_callback cb, void *cb_arg);
	int postHttpRequest(char *post_data, unsigned int data_len, user_callback cb, void *cb_arg);

	int postHttpRequestWithUrl(const char *http_url, char *post_data, unsigned int data_len, user_callback cb, void *cb_arg);
	int getHttpRequestWithUrl(const char *http_url, user_callback cb, void *cb_arg);

	int configureHttpHeader(struct evhttp_request *request);
	int createHttpRequest(const char *http_url, user_callback cb, void *cb_arg);
	int makeHttpRequest(const char *http_url, evhttp_cmd_type type
		, char *post_data, unsigned int data_len, user_callback cb, void *cb_arg);

	void setUrl(const char *url) {_url = strdup(url);};

	void setBase(struct event_base *base) {_base = base;};

	void set_cb_arg(void *arg) {_user_arg = arg;};
	
	bool isTimeout() {
		time_t diff = _end_time - _start_time;
		return diff >= _time_out;
	};

	bool reachTimeout(time_t s) {
		time_t diff = _end_time - _start_time;
		return diff >= _time_out-s;
	}

	void setStartTime(time_t new_time) {_start_time = new_time;}
	
	//user don't use to delete object
	void autoRelease(){};

	int setPostData(char *post_data, unsigned int data_len) {
		if (post_data == NULL) {
			return -1;
		}

		_post_data = strdup(post_data);
		_post_len = data_len;
		return evbuffer_add(_request->output_buffer, post_data, data_len);
	}

	void setUserCallback(user_callback cb, void *arg) {
		userCallback = cb;
		_user_arg = arg;
	}
	user_callback userCallback;

private:

	void RePostHttpRequest();

	struct event_base *_base;
	struct evhttp_connection *_conn;
	struct evhttp_request *_request;

	char *_url;
	struct evhttp_uri *_uri;

	int _flag; //0--get; 1--post

	char *_post_data;
	unsigned int _post_len;
	void *_user_arg;

	int _time_out;
	time_t _start_time;
	time_t _end_time;

	// reserved : request count per connection
	long long _statistic;
};
