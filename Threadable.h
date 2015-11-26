#pragma once
class Threadable
{
public:
	Threadable(void);
	~Threadable(void);

	virtual int run() = 0;
	void start();
	void join();
	int yield();
	static void milliSleep(unsigned long milli);
private:
	int _threadId;
};

