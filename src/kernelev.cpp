#include "kernelev.h"
#include "ivtentry.h"
#include "os_core.h"
#include "SCHEDULE.H"
#include "kernelse.h"

KernelEv::KernelEv(IVTNo ivtno) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	mySem = new KernelSem(0);
	ivtNum = ivtno;
	creator = OSCore::running;

	IVTEntry::entries[ivtNum]->myEvent = this;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

KernelEv::~KernelEv() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	IVTEntry::entries[ivtNum]->myEvent = 0;
	delete mySem;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void KernelEv::wait() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	if (OSCore::running != creator) {
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		#endif
		return;
	}
	mySem->wait(0);

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void KernelEv::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	if(mySem->val == 1) {
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		#endif
		return;
	}
	mySem->signal(0);

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}
