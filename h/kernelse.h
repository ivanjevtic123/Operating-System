#ifndef _kernelse_h_
#define _kernelse_h_

#include "pcb.h"

class KernelSem;

class SemElem {
public:
	KernelSem* sem;
	SemElem* next;
};

class SemQueue {
public:
	void put(KernelSem* pcb);

	SemQueue();
	~SemQueue();

	SemElem* first;
};

class Queue;
class Semaphore;

class KernelSem{
public:
	KernelSem(int v);
	~KernelSem();

	int wait(Time maxTime);

	int signal(int n);
	int value()const;
	void decrement();
    static SemQueue* globalSemQueue;

    static void decrementTimeAndUnblock();

    int val;
	Queue *timeWaitingQueue;
	Queue *condWaitingQueue;
};



#endif
