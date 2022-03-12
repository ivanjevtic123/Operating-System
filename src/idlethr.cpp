#include "idlethr.h"
#include "os_core.h"
#include "pcb.h"

IdleThread::IdleThread(): Thread(1024, 1) {}

void IdleThread::run() {
	while (1);
}

void IdleThread::start() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myPCB->state = PCB::READY;
	myPCB->createStack();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
