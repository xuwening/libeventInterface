#include "Threadable.h"
#include "LibeventLog.h"

#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <stddef.h>
#include <iostream>
using namespace std;

Threadable::Threadable(void)
{
	_threadId = -1;
}


Threadable::~Threadable(void)
{
}

static void* ThreadFunc(void *args) {
	Threadable *self = (Threadable *)args;
	self->run();
	return NULL;
}


int Threadable::yield() {
#ifndef WIN32
	return sched_yield();
#else
	SwitchToThread();
#endif
	return 0;
}

static int startThread(void* (*threadFun) (void *), void *args) {
#ifndef WIN32
	int            hThread;
	pthread_t      recvthread;

	hThread = pthread_create(&recvthread, NULL, threadFun, args);
	if (hThread != 0) {
		LibeventLog::userLog("pthread create failed.");
		return -1;
	} else {
		pthread_detach(recvthread);
	}

	return recvthread;
#else
	DWORD	  ThreadId;	   
	DWORD   dwThreadId;
	ThreadId = (int)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadFun, args, 0, &dwThreadId);												 
	if (ThreadId == -1) {
		return -1;
	}

	SetThreadPriority((HANDLE)ThreadId,THREAD_PRIORITY_NORMAL);
	return ThreadId;
#endif
}

void Threadable::start() {
	//create thread
	_threadId = startThread(ThreadFunc, this);
}

void Threadable::join() {
#ifndef WIN32
	if (_threadId > 0)
		pthread_join(_threadId, NULL);
#endif
}

void Threadable::milliSleep(unsigned long milli) {
#ifndef WIN32
	usleep(milli*1000);
#else
	Sleep(milli);
#endif
}
