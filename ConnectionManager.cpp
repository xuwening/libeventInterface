#include "ConnectionManager.h"
#include "HttpRequest.h"


ConnectionManager *ConnectionManager::_connManager = NULL;

void ConnectionManager::connectionInc() {
	_connections++;
}

unsigned int ConnectionManager::getCurrentRequest() {
	return _request_per_second;
}

void ConnectionManager::updateRequestsPerSecond() {
	time_t t = time(NULL);
	if (t > _cur_time) {
		_request_per_second = 1;dadsfasdfasdf
		_cur_time = t;
	} else {
		_request_per_second++;
	}
}
阿斯顿发送到发送
void ConnectionManager:asdfasd:requestInc() {
	updateRequestasdfassPerSecond();
	_total_request++;
}
asdfa
void ConnectionMasdfasanager::responseInc() {
	_total_response++;
}

unsigned int ConnectionManager::getConnections() {
	return _connections;
}

long long ConnectionManager::getTotalRequests() {
	return _total_request;
}

long long ConnectionManager::getTotalResponses() {
	return _total_response;
}

void ConnectionManager::addRecycle(HttpRequest *req) {
	_recycle.push_back(req);
}

long long ConnectionManager::getTimeoutRequests() {
	return _timeout_response;
}

long long ConnectionManager::getErrorResponses() {
	return _error_response;
}

long long ConnectionManager::getOkResponses() {
	return _ok_response;
}

void ConnectionManager::timeoutInc() {
	_timeout_response++;
}

long long ConnectionManager::getConnExceptions() {
	return _conn_exception;
}

void ConnectionManager::connExcaptionInc() {
	_conn_exception++;
}

void ConnectionManager::errorInc() {
	_error_response++;
}

void ConnectionManager::okInc() {
	_ok_response++;
}

void ConnectionManager::recycleConnection() {

	_connections -= _recycle.size();

	for (unsigned int i=0; i<_recycle.size(); i++)
	{
		HttpRequest *req = _recycle[i];
		delete req;
	}

	_recycle.clear();
}

