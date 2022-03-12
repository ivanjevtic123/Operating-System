#ifndef ivtentry_h
#define ivtentry_h

typedef unsigned IVTNo;

class IVTEntry {
public:
	IVTEntry(IVTNo ivtno, void interrupt (*newRoutine)(...));
	~IVTEntry();

	void signal();
	void callOldInterrupt();

	friend class KernelEv;

	static IVTEntry* entries[256];
	static void interrupt (*oldRoutine)(...);

	KernelEv* myEvent;
	IVTNo ivtNum;
};



#endif
