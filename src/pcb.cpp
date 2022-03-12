#include "pcb.h"
#include <stdlib.h>
#include <dos.h>

#include "SCHEDULE.H"
#include "os_core.h"

#define MAX_STACK_SIZE 65535

ID PCB::globalId = 0;
Queue *PCB::globalPCBQueue = new Queue();

PCB::PCB(Thread* myT, StackSize stackSize, Time timeSlice) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myId = globalId++;
	globalPCBQueue->put(this);
	semWaitReturnValue = 0;
	semWaitTime = 0;
	state = NEW;
	if (pcbStackSize > MAX_STACK_SIZE) pcbStackSize = MAX_STACK_SIZE;
	pcbStackSize = stackSize;
	myThread = myT;
	pcbSP = 0;
	pcbTimeSlice = timeSlice;
	waitQueue = new Queue();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

PCB::~PCB() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete waitQueue;
	delete pcbSP;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void PCB::createStack() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	pcbStackSize /= sizeof(unsigned);

	pcbSP = new unsigned[pcbStackSize];
#ifndef BCC_BLOCK_IGNORE
	pcbSP[pcbStackSize-1] = 0x200;
	pcbSP[pcbStackSize-2] = FP_SEG(wrapper);
	pcbSP[pcbStackSize-3] = FP_OFF(wrapper);

	sp = FP_OFF(pcbSP+pcbStackSize-12);
	ss = FP_SEG(pcbSP+pcbStackSize-12);

	unlock();
#endif
}

void PCB::wrapper() {
	OSCore::running->myThread->run();

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	OSCore::running->state = PCB::OVER;

	PCB* temp = OSCore::running->waitQueue->get();
	while (temp) {
		temp->state = PCB::READY;
		Scheduler::put(temp);
		temp = OSCore::running->waitQueue->get();
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	dispatch();
}
