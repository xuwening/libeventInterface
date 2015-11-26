#pragma once

#include "httpCommon.h"
#include "AbstractServer.h"
#include "Configure.h"

using namespace std;

class HttpContainer {

public:
	HttpContainer(){
		servers.clear();
	};

	virtual ~HttpContainer(){
		for (unsigned int i=0; i<servers.size(); i++) {
			delete servers[i];
		}
		servers.clear();
		if (_base) {event_base_free(_base);}
	};

	struct event_base *getEventBase() {
		return _base;
	}

	void setEventBase(struct event_base *base) {
		_base = base;
	}

	virtual void loop() = 0;
	virtual void initContainer()=0;
	
	void addServer(AbstractServer *server) {
		servers.push_back(server);
	}

	AbstractServer *getServer(unsigned int idx) {
		if (idx >= servers.size())
			return NULL;
		return servers[idx];
	}

	void startServers(struct event_base *base) {
		for (unsigned int i=0; i<servers.size(); i++) {
			servers[i]->run(base);
		}
	}

private:
	struct event_base* _base;
	vector<AbstractServer *> servers;
};


//sub class
class PersistHttpContainer : public HttpContainer {

public:
	PersistHttpContainer(){
		initContainer();
	};

	virtual ~PersistHttpContainer() {
		
		delete config;
	};

	void initContainer();
	void loop();


private:
	Configure *config;;
};
