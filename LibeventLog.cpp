#include "LibeventLog.h"
#include <time.h>

static FILE *logfile = NULL;

LibeventLog::LibeventLog(void)
{
	logfile = NULL;
}


LibeventLog::~LibeventLog(void)
{
	fclose(logfile);
}

void write_to_file_fatal_cb(int err) {
	if (logfile == NULL) {
		return;
	}

	fprintf(logfile, "[**** fatal error ****] %d\n", err);
}

static void discard_log(int severity, const char *msg) {

}

static void write_to_file_cv(int severity, const char *msg) {
	const char *s;
	if (logfile == NULL) {
		return;
	}

	switch (severity) {
	case _EVENT_LOG_DEBUG:
		s = "debug";
		break;
	case _EVENT_LOG_MSG:
		s = "msg";
		break;
	case _EVENT_LOG_WARN:
		s = "warn";
		break;
	case _EVENT_LOG_ERR:
		s = "error";
		break;
	default:
		s = "?";
		break;
	}

	fprintf(logfile, "[%s] %s\n", s, msg);
	fflush(logfile);
}

void LibeventLog::turnOn(const char *filepath) {
	if (logfile != NULL) {
		event_set_log_callback(write_to_file_cv);
		return;
	}

	logfile = fopen(filepath, "a+");
	if (logfile == NULL) {
		event_set_log_callback(discard_log);
	} else {
		event_set_log_callback(write_to_file_cv);
	}

	event_set_fatal_callback(write_to_file_fatal_cb);
}

void LibeventLog::turnOff() {
	event_set_log_callback(discard_log);
	fclose(logfile);
	logfile = NULL;
}

void LibeventLog::userLog(const char *fmt, ...) {

	if (logfile == NULL) {
		return;
	}

	va_list ap;

	char buf[LOG_TEMP_BUFFER_SIZE];
	memset(buf, 0, sizeof(buf));
	time_t now;
	struct tm *curtime = NULL;
	time(&now);
	curtime = localtime(&now);
	sprintf(buf, "[%d-%02d-%02d %02d:%02d:%02d] ", (curtime->tm_year+1900), (curtime->tm_mon+1), curtime->tm_mday,
		curtime->tm_hour, curtime->tm_min, curtime->tm_sec);

	int len = strlen(buf);
	
	va_start(ap, fmt);
	vsnprintf(buf+len, LOG_TEMP_BUFFER_SIZE-len-1, fmt, ap);
	va_end(ap);

	fprintf(logfile, "%s\n", buf);
	fflush(logfile);
}