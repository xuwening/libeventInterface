#ifndef _HTTP_COMMON__H_
#define _HTTP_COMMON__H_


#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>
#include <vector>
#include <iostream>

#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event_struct.h>

#include <stddef.h>

using namespace std;

#define CONFIG_FILE_PATH "./config/data.cfg"

//#define SERVER_URL "http://www.baidu.com:80/"
//#define SERVER_URL "http://192.168.2.26:8888/"
#define SERVER_URL "http://192.168.1.232:8080/Boss/work"

#define HTTP_STATISTIC_PORT "8088"
#define EVENT_LISTEN_PORT "8089"

//http request timeout
#define TIME_OUT_SECONDS "10"
#define THREAD_SLEEP_MILLI "200"

#define MESSAGE_HEARTBEAT_ADDR "c"
#define MESSAGE_NOTIFY_DATA_ADDR "d"

#define MESSAGE_CHAR_LENGHT sizeof(char)

#define CHECK_CALL_RET(val, ret) \
	ret = val;				\
	if (ret != 0) {			\
	return ret;			\
	}

#define LOG_TEMP_BUFFER_SIZE 1024
#define LOG_SWITCH_NAME		"log_switch" 
#define LOG_FILE			"logfile"
#define LOG_ON				"on"
#define LOG_DEFAULT_NAME    "./event.log"

#define REQUESTS_PER_SECOND_NAME "requests_per_second"
#define REQUESTS_PER_SECOND		"-1"
#define MAX_CONNECTIONS_NAME	"max_connections"
#define MAX_CONNECTIONS			"-1"


#endif

