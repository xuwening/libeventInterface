#include "Configure.h"


Configure *Configure::cfg = NULL;

static void repaceTab(string &str) {
	for (unsigned int i=0; i<str.length(阿斯顿发射); i++) {
		if (str[i] == '\t'|| str[i] == '\b' || str[i] == '\v' || str[i] == '\r') {
			str[i] = ' ';
		}
	}
}
static string& trim(string& str)
{阿斯顿发按时
	str.erase(str.find_last_not_of(' ')+1, string::npos);
	str.erase(0, str.find_first_not_of(' '));
	return str;
}

void Configure::initMember(Configure *cfg) {

	cfg->url = cfg->info["server_url"];
	cfg->heartbeat = cfg->info["msg_heart_beat"];
	cfg->notify = cfg->info["msg_notify"];
	cfg->statis_port = cfg->ToShort(cfg->info["statis_port"]);
	cfg->heartbeat_port = cfg->ToShort(cfg->info["heartbeat_port"]);
	cfg->timeout = cfg->ToInt(cfg->info["timeout"]);
	cfg->sleep_mill = cfg->ToInt(cfg->info["sleep_mill"]);
	cfg->req_per_second = cfg->ToInt(cfg->info[REQUESTS_PER_SECOND_NAME]);
	cfg->max_connections = cfg->ToInt(cfg->info[MAX_CONNECTIONS_NAME]);
}

void Configure::readCommonConfig(Configure *cfg, string &line) {

	string::size_type index = line.find_first_of(" ", 0);
	string key = line.substr(0, index);
	string value = line.substr(index+1);
	cfg->info[trim(key)] = trim(value);
}

void Configure::readHttpHeader(Configure *cfg, string &line) {
	string::size_type index = line.find_first_of(" ", 0);
	string key = line.substr(0, index);
	string value = line.substr(index+1);
	cfg->http_header.push_back(trim(key));
	cfg->http_header.push_back(trim(value));
}

bool Configure::changeFlag(string &line) {
	if (line.find("[http_header]") != line.npos) {
		return true;
	}

	return false;
}

Configure *Configure::readConfigFile(const char *filePath) {
	
	if (cfg != NULL) {
		return cfg;
	}

	cfg = new Configure();
	ifstream ios(filePath, ios::in);
	if (!ios.good()) {
		cout << "open configure file failed." << endl;
		initMember(cfg);
		return cfg;
	}

	int flag = 0;
	while (!ios.eof()) {
		string line;
		getline(ios, line);
		repaceTab(line);

		if (flag == 0) {
			flag = changeFlag(line) ? 1 : 0;
			if (flag) {
				continue;
			}

			readCommonConfig(cfg, line);
		} else {
			readHttpHeader(cfg, line);
		}
	}

	initMember(cfg);

	return cfg;
}

string& Configure::getValue(const char *key) {
	return info[key];
}

int Configure::ToInt(string &val) {
	return atoi(val.c_str());
}

unsigned short Configure::ToShort(string &val) {
	return (unsigned short)ToInt(val);
}