#ifndef kernelev_h
#define kernelev_h

#include "event.h"
#include "queue.h"

class Thread;
class KernelSem;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();

	IVTNo ivtNum;
	PCB *creator;
	KernelSem* mySem;
};



#endif
