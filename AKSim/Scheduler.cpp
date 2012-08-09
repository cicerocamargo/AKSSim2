//
//  Scheduler.cpp
//  AKSim
//
//  Created by Cícero Camargo on 09/08/12.
//  Copyright (c) 2012 Cícero Camargo. All rights reserved.
//


#include "Scheduler.h"
#include "Thread.h"
#include "Processor.h"

bool Scheduler::allProcessorsWaiting() {
	return numProcessors == idleProcessors.size() ? true : false;
}

void Scheduler::findWorkForProcessor(Processor* processor) {

	// TODO: a remoção de elementos das listas aqui pode resultar em erros

	if (!readyThreads.empty()){
		processor->setCurrentThread(readyThreads.front());
		readyThreads.pop_front();
		idleProcessors.remove(processor);
		busyProcessors.push_back(processor);
	}
}

// look for idle procesors and try to get a job for them
void Scheduler::findWorkForIdleProcessors() {
    list<Processor*>::iterator processor;
	for (processor = idleProcessors.begin(); processor != idleProcessors.end(); processor++) {
		if ( (*processor)->isIdle() ) {
			this->findWorkForProcessor(*processor);
		}
	}
}

/* PUBLIC METHODS */

Scheduler::Scheduler(Thread* rootThread, list<Processor*> processors) {
	this->rootThread = rootThread;
	readyThreads.push_back(rootThread);

	numProcessors = (int)processors.size();
	idleProcessors = allProcessors = processors;
	for ( list<Processor*>::iterator processor = idleProcessors.begin();
		  processor != idleProcessors.end();
		  processor++ )
	{
		(*processor)->setScheduler(this);
	}
}

void Scheduler::runProgram() {
	int step=0;
	list<Processor*>::iterator processor;

	while (true) {

		this->findWorkForIdleProcessors();	// here, the recent history of
											// the processor may be important

		if (this->allProcessorsWaiting()) { // the end ...
			break;
		}

		for (processor = allProcessors.begin(); processor != allProcessors.end(); processor++) {
			(*processor)->step();
		}

		// if in the end of the step the processor reached
		// a scheduling point (end of any task), it communicate with the scheduler.
		for (processor = allProcessors.begin(); processor != allProcessors.end(); processor++) {
			if ( (*processor)->reachedSchedulingPoint() ) {
				(*processor)->communicateWithScheduler();
			}
		}

		step++;
	}

	printf("The execution took %d time steps.\n", step);
}

void Scheduler::newReadyThread(Thread* thread) {
	// TODO: we have to insert the thread in the list, according to the priority attribute
}

void Scheduler::processorBecameIdle(Processor* processor) {
	busyProcessors.remove(processor);
	idleProcessors.push_back(processor);
}