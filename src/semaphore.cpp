#include "os_core.h"
#include "semaphor.h"
#include "kernelse.h"
#include <iostream.h>
#include <stdlib.h>

Semaphore:: Semaphore (int i){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myImpl=new KernelSem(i);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
 }

Semaphore::~Semaphore (){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
 }

int Semaphore:: wait (Time maxTimeToWait){
	int ret;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	ret= myImpl->wait(maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return ret;
}

int Semaphore:: signal(int n){
	int ret;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	ret=myImpl->signal(n);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return ret;
}

 int Semaphore:: val () const{
	int ret;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	ret=myImpl->value();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return ret;
 }
