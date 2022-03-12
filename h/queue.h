#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "pcb.h"

class PCB;

class Elem {
public:
	PCB* pcb;
	Elem* next;
};

class Queue {
public:
	void put(PCB* pcb);
	PCB* get();

	Queue();
	~Queue();

	Elem* first;
};


#endif
