#include "kernelse.h"
#include "os_core.h"
#include "queue.h"
#include "semaphor.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include <stdlib.h>

SemQueue::SemQueue() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	first = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

SemQueue::~SemQueue() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	SemElem* temp;
	while( first != 0 )	{
		temp = first;
		first = first->next;
		delete temp;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void SemQueue::put(KernelSem* sem) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	SemElem **dp = &first;
	while( (*dp) != 0 )
	{
		if( (*dp)->sem == sem ) {
			#ifndef BCC_BLOCK_IGNORE
			unlock();
			#endif
			return;
		}
		dp = &((*dp)->next);
	}
	(*dp) = new SemElem();
	(*dp)->next = 0;
	(*dp)->sem = sem;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

SemQueue* KernelSem::globalSemQueue=new SemQueue();

KernelSem::KernelSem(int v){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	globalSemQueue->put(this);
	val=v;
	timeWaitingQueue=new Queue();
	condWaitingQueue=new Queue();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}
KernelSem::	~KernelSem(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	delete timeWaitingQueue;
	delete condWaitingQueue;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

int KernelSem:: wait(Time maxTime){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	OSCore::running->semWaitReturnValue=1;

	if(--val<0){
		if(maxTime>0) { timeWaitingQueue->put(OSCore::running); }
		else { condWaitingQueue->put(OSCore::running); }

		OSCore::running->state = PCB::BLOCKED;
		OSCore::running->semWaitTime = maxTime;

		dispatch();
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return OSCore::running->semWaitReturnValue;
}

int KernelSem:: signal(int n){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	if (n<0) {
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		#endif
		return n;
	}

	if(n==0){
		if(++val<=0){
			PCB*t=condWaitingQueue->get();
			if (t == 0)
				timeWaitingQueue->get();
			t->state=PCB::READY;
			Scheduler::put(t);
			#ifndef BCC_BLOCK_IGNORE
			unlock();
			#endif
			return 0;
		}
	}
    if(n>0){
    	int numberOfUnblocked=0;
    	int i;
    	for(i=0;i<n;i++){
    		if(++val<=0){
    			PCB*t=condWaitingQueue->get();
    			if (t == 0)
    				timeWaitingQueue->get();
    			t->state=PCB::READY;
    			Scheduler::put(t);
    			numberOfUnblocked++;
    		}
    	}
		#ifndef BCC_BLOCK_IGNORE
    	unlock();
		#endif
    	return numberOfUnblocked;
    }

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return 0;
}

int KernelSem:: value()const{
	int ret;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	ret=val;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return ret;
}


void KernelSem::decrementTimeAndUnblock(){

	for (SemElem *tekSem = globalSemQueue->first; tekSem; tekSem=tekSem->next) {

		Elem* temp=tekSem->sem->timeWaitingQueue->first,* prev=0,* toDelete=0;
		while(temp != 0){
			temp->pcb->semWaitTime--;

			if (temp->pcb->semWaitTime > 0) {
				prev = temp;
				temp = temp->next;
			} else {
				temp->pcb->state = PCB::READY;
				temp->pcb->semWaitReturnValue = 0;
				Scheduler::put(temp->pcb);

				toDelete = temp;
				temp = temp->next;
				if (!prev)
					tekSem->sem->timeWaitingQueue->first = temp;
				else
					prev->next = temp;
				delete toDelete;
				tekSem->sem->val++;
			}
		}

	}
}

