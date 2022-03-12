#ifndef _OS_CORE_H_
#define _OS_CORE_H_

#include "pcb.h"
#include "thread.h"
#include "idlethr.h"
#include "event.h"

#define lock() {asm{pushf; cli;}}
#define unlock() {asm{popf}}

class OSCore {
public:
	static void load();
	static void unload();
	static void dispatch();

	static Thread* starting;
	static IdleThread* idle;
	static PCB* running;
	static int dispatched;
	static Time timePassedCounter;

	static void interrupt (*oldRoutine)(...);
	static void interrupt timerIR(...);
};

#endif
