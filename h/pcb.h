#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "queue.h"

class Thread;
class Queue;

class PCB
{
public:
	PCB(Thread* myT, StackSize stackSize, Time timeSlice);
	~PCB();

	Thread* myThread;
	unsigned int sp, ss;

	static ID globalId;
	ID myId;
	static Queue *globalPCBQueue;

	int semWaitReturnValue;
	Time semWaitTime;

	enum State { NEW, READY, BLOCKED, OVER };
	State state;

	Time pcbTimeSlice;

	StackSize pcbStackSize;

	unsigned *pcbSP;

	Queue* waitQueue;

	void createStack();

	static void wrapper();
};

#endif
