#include "ivtentry.h"
#include "kernelev.h"
#include "os_core.h"
#include <dos.h>

IVTEntry* IVTEntry::entries[256] = {0};
void interrupt (*IVTEntry::oldRoutine)(...) = 0;

IVTEntry::IVTEntry(IVTNo ivtno, void interrupt (*newRoutine)(...)) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	ivtNum = ivtno;

	entries[ivtno] = this;

	oldRoutine = getvect(ivtno);
	setvect(ivtno, newRoutine);

	myEvent = 0;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	oldRoutine();
	setvect(ivtNum, oldRoutine);

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void IVTEntry::signal() {
	if(myEvent != 0) {
		myEvent->signal();
	}
}

void IVTEntry::callOldInterrupt() {	oldRoutine(); }
