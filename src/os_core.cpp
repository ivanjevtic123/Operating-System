#include "os_core.h"
#include "schedule.h"
#include <dos.h>
#include <stdlib.h>
#include "kernelse.h"

int OSCore::dispatched = 0;
void interrupt (*OSCore::oldRoutine)(...) = 0;
PCB* OSCore::running = 0;
Thread* OSCore::starting = 0;
IdleThread* OSCore::idle = 0;
Time OSCore::timePassedCounter = 0;

void tick();

void OSCore::load() { 
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(0x08);

	setvect(0x08, timerIR);
#endif
	starting = new Thread(1024, 1);
	starting->myPCB->state = PCB::READY;
	running = starting->myPCB;

	idle = new IdleThread();
	idle->start();

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void OSCore::unload() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, oldRoutine);
#endif
	delete starting;
	delete idle;

	delete PCB::globalPCBQueue;
	delete KernelSem::globalSemQueue;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


void interrupt OSCore::timerIR(...) {
	if(!dispatched ) {
		tick(); 
		(*oldRoutine)(); 

		KernelSem::decrementTimeAndUnblock();

		if (running->pcbTimeSlice != 0) timePassedCounter++;

		if (timePassedCounter < running->pcbTimeSlice || running->pcbTimeSlice == 0) return;
	} 

	dispatched = 0;

	if( running->state == PCB::READY && running != idle->myPCB )
		Scheduler::put(running);
#ifndef BCC_BLOCK_IGNORE
	running->sp = _SP;
	running->ss = _SS;
#endif
	running = Scheduler::get();
	if (running == 0) running = idle->myPCB;
#ifndef BCC_BLOCK_IGNORE
	_SP = running->sp;
	_SS = running->ss;
#endif
	timePassedCounter = 0;

}
