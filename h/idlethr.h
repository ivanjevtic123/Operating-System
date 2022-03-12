#ifndef _IDLETHR_H_
#define _IDLETHR_H_

#include "thread.h"

class Thread;

class IdleThread : public Thread {
public:
	IdleThread();
	virtual void run();
	void start();
};

#endif
