#include <stdlib.h>
#include "queue.h"
#include "os_core.h"


Queue::Queue() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	first = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

Queue::~Queue() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	Elem* temp;
	while( first != 0 )	{
		temp = first;
		first = first->next;
		delete temp;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

PCB* Queue::get() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if( first == 0 ) {
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		#endif
		return 0;
	}
	Elem* temp = first;
	PCB *ret = temp->pcb;
	first = first->next;
	delete temp;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return ret;
}

/*
void Queue::put(PCB* pcb) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	Elem **dp = &first;
	while( (*dp) != 0 )
	{
		if( (*dp)->pcb == pcb ) {
			#ifndef BCC_BLOCK_IGNORE
			unlock();
			#endif
			return;
		}
		dp = &((*dp)->next);
	}
	(*dp) = new Elem();
	(*dp)->next = 0;
	(*dp)->pcb = pcb;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
*/

void Queue::put(PCB* pcb) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	Elem* newElem=new Elem();
	newElem->pcb=pcb;
	newElem->next=0;

	Elem* dp = first;
	if(dp==0){
		first=newElem;
	}
	else{
		while(dp->next != 0 )
		{
			dp = dp->next;
		}
		dp->next=newElem;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}



