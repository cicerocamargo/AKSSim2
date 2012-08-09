//
//  Scheduler.h
//  AKSim
//
//  Created by Cícero Camargo on 09/08/12.
//  Copyright (c) 2012 Cícero Camargo. All rights reserved.
//

#include <list>
using namespace std;

class Thread;
class Processor;

class Scheduler {
	Thread* rootThread;
	list<Thread*> readyThreads;
	list<Processor*> allProcessors, idleProcessors, busyProcessors;
	int numProcessors;


	bool allProcessorsWaiting();
	void findWorkForIdleProcessors();
	void findWorkForProcessor(Processor* proc);
	

public:

	// the constructor receives the main thread
	// and a list of processors.
	Scheduler(Thread* rootThread, list<Processor*> processors);

	void runProgram();
	
	// message received from a processor that just
	// executed a create task.
	void newReadyThread(Thread* thread);

	// message received from a processor that became idle
	// because of a non-satisfied join, or the end of a thread
	void processorBecameIdle(Processor* processor);
};