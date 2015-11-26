#pragma once

#include <map>
#include <string>
#include <fstream>
#include "httpCommon.h"

using namespace std;

class Configure
{
public:
	~Configure() {
		cfg = NULL;
	};
	static Configure *readConfigFile(const char *filePath);
	string &getValue(const char *key);

	string url;
	string heartbeat;
	string notify;
	unsigned short statis_port;
	unsigned short heartbeat_port;
	unsigned int timeout;
	unsigned int sleep_mill;

	int req_per_second;
	int max_connections;

	vector<string> http_header;
private:
	Configure(){ 
		info["server_url"] = SERVER_URL;
		info["statis_port"] = HTTP_STATISTIC_PORT;
		info["heartbeat_port"] = EVENT_LISTEN_PORT;
		info["timeout"] = TIME_OUT_SECONDS;
		info["sleep_mill"] = THREAD_SLEEP_MILLI;
		info["msg_heart_beat"] = MESSAGE_HEARTBEAT_ADDR;
		info["msg_notify"] = MESSAGE_NOTIFY_DATA_ADDR;
		info[LOG_SWITCH_NAME] = LOG_ON;
		info[LOG_FILE] = LOG_DEFAULT_NAME;
		info[REQUESTS_PER_SECOND_NAME] = REQUESTS_PER_SECOND;
		info[MAX_CONNECTIONS_NAME] = MAX_CONNECTIONS;
	};

	static void initMember(Configure *cfg);
	static void readCommonConfig(Configure *cfg, string &line);
	static void readHttpHeader(Configure *cfg, string &line);
	static bool changeFlag(string &line);

	int ToInt(string &val);
	unsigned short ToShort(string &val);

	map<string, string> info;
	
	static Configure *cfg;
};

