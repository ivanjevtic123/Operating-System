#include "pcb.h"
#include "thread.h"
#include "schedule.h"
#include <dos.h>
#include <stdlib.h>

#include "os_core.h"

class OSCore;
class Scheduler;

Thread::Thread(StackSize stackSize, Time timeSlice) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myPCB = new PCB(this, stackSize, timeSlice);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

Thread::~Thread() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	waitToComplete();
	delete myPCB;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void Thread::start() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myPCB->state = PCB::READY;
	myPCB->createStack();
	Scheduler::put(myPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void Thread::waitToComplete() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if (myPCB->state == PCB::OVER || myPCB->state == PCB::NEW) {
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		#endif
		return;
	}
	if (myPCB == OSCore::running) {
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		#endif
		return;
	}
	if (this == OSCore::starting) {
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		#endif
		return;
	}
	if (this == OSCore::idle) {
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		#endif
		return;
	}

	OSCore::running->state = PCB::BLOCKED;
	myPCB->waitQueue->put((PCB*)OSCore::running);
	dispatch();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

ID Thread::getId() { return myPCB->myId; }
ID Thread::getRunningId() { return OSCore::running->myId; }

Thread* Thread::getThreadById(ID id) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	for (Elem *tek = PCB::globalPCBQueue->first; tek; tek=tek->next)
		if (tek->pcb->myId == id) {
			#ifndef BCC_BLOCK_IGNORE
			unlock();
			#endif
			return tek->pcb->myThread;
		}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return 0;
}

void dispatch() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	OSCore::dispatched = 1;
	OSCore::timerIR();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
