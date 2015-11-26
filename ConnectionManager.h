#pragma once

#include <stddef.h>
#include <time.h>
#include <vector>


using namespace std;

class HttpRequest;

class ConnectionManager
{
public:

	~ConnectionManager(void){
		_connManager = NULL;
	};

	static ConnectionManager *getInstance() {
		//multi thread lock
		if (_connManager == NULL) {
			_connManager = new ConnectionManager();
		}
		//unlock

		return _connManager;
	}

	void connectionInc();
	
	void requestInc();
	void responseInc();
	void timeoutInc();
	void errorInc();
	void connExcaptionInc();
	void okInc();
	
	long long getConnExceptions();
	unsigned int getConnections();
	long long getTotalRequests();
	long long getTotalResponses();
	long long getTimeoutRequests();
	long long getErrorResponses();
	long long getOkResponses();
	
	void updateRequestsPerSecond();
	unsigned int getCurrentRequest();
	void addRecycle(HttpRequest *req);
	void recycleConnection();

	void clearStatictis() {
		_total_request = 0;
		_total_response = 0;
		_timeout_response = 0;
		_error_response = 0;
		
		_conn_exception = 0;
		_ok_response = 0;
		_request_per_second = 0;
	};

private:
	ConnectionManager(void){
		_total_request = 0;
		_total_response = 0;
		_timeout_response = 0;
		_error_response = 0;
		_connections = 0;
		_conn_exception = 0;
		_ok_response = 0;

		_cur_time = time(NULL);
		_request_per_second = 0;
		_recycle.clear();
	};

	time_t _cur_time;
	unsigned int _request_per_second;

	long long _total_request;
	long long _total_response;
	long long _timeout_response;
	long long _error_response;
	long long _conn_exception;
	long long _ok_response;

	vector<HttpRequest *> _recycle;

	unsigned int _connections;
	static ConnectionManager *_connManager;
};

