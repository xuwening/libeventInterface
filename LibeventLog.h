#pragma once

#include "httpCommon.h"
#include <stdio.h>


class LibeventLog
{
public:
	LibeventLog(void);
	~LibeventLog(void);

	static void turnOn(const char *filepath);
	static void turnOff();
	static void userLog(const char *fmt, ...);
};

