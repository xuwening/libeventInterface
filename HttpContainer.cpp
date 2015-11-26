#include "HttpContainer.h"
#include "HttpRequest.h"
#include "ConnectionManager.h"
#include <event2/http_compat.h>
#include "LibeventLog.h"

void PersistHttpContainer::initContainer() {
	setEventBase(event_base_new());

	config = Configure::readConfigFile(CONFIG_FILE_PATH);
	string logswitch = config->getValue(LOG_SWITCH_NAME);
	if (!logswitch.empty()) {
		string logfile = config->getValue(LOG_FILE);
		if (logfile.empty()) {
			logfile = LOG_DEFAULT_NAME;
		}

		LibeventLog::turnOn(logfile.c_str());
	}
};

void PersistHttpContainer::loop() {
	startServers(getEventBase());
	event_base_dispatch(getEventBase());
};