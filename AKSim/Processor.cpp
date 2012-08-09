//
//  Processor.cpp
//  AKSim
//
//  Created by Cícero Camargo on 09/08/12.
//  Copyright (c) 2012 Cícero Camargo. All rights reserved.
//

#include <iostream>
#include "Processor.h"
#include "Scheduler.h"
#include "Thread.h"
#include "Task.h"

int Processor::instanceCounter = 0;

void Processor::prepareForSchedulingPoint() {
	if (currentTask->isCreate()) {
		if (workingPolicy == HelpFirst) { // Anahy's policy
			Thread* t = currentTask->getAssociatedThread();
			t->setState(ThreadReady);
			this->threadCreated = t;
		}
		else { // workingPolicy == HelpFirst -> Cilk's policy
			currentThread->setState(ThreadReady);
			this->threadCreated = currentThread;
			currentThread = currentTask->getAssociatedThread();
			currentThread->setState(ThreadRunning);
		}
	} else if (currentTask->isJoin()) {
		this->threadJoined = currentTask->getAssociatedThread();
	} else { // completion Task
		currentThread->setState(ThreadCompleted);
		this->threadCompleted = currentThread; 
	}
}

Processor::Processor() {
	id = instanceCounter++;
	currentTask = NULL;
    currentThread = NULL;
    scheduler = NULL;
	clearRecentHistory();
}

/* class methods */
Processor* Processor::processorWithWorkFirstPolicy() {
	Processor* p = new Processor();
	p->workingPolicy = WorkFirst;
	p->migration = true;
    return p;
}

Processor* Processor::processorWithHelpFirstPolicyAndMigrationEnabled() {
	Processor* p = new Processor();
	p->workingPolicy = HelpFirst;
	p->migration = true;
    return p;
}

Processor* Processor::processorWithHelpFirstPolicyAndMigrationDisabled() {
	Processor* p = new Processor();
	p->workingPolicy = HelpFirst;
	p->migration = false;
    return p;
}


/* public methods */
void Processor::step() {

	if (currentThread) {
		ganttChartRow.push_back('#');

		if (!currentTask) {
			currentTask = currentThread->getCurrentTask();	
		}

		currentTask->step(); // when the task takes the last step,
							// it should update its thred's current task

		if (currentTask->isCompleted()) {
			this->prepareForSchedulingPoint();
			currentTask = NULL;
		}
	}
	else {
		ganttChartRow.push_back('-');
	}
}

void Processor::setScheduler(Scheduler* s) {
	scheduler = s;
}

void Processor::setCurrentThread(Thread* t) {
	currentThread = t;
}

bool Processor::isIdle() {
	return (currentThread ? false : true);
}

bool Processor::reachedSchedulingPoint() {
	if (this->createdAThread() || this->joinedAThread() || this->completedAThread()) {
		return true;
	}
	return false;
}

void Processor::communicateWithScheduler() {

	if (this->createdAThread()) {
		scheduler->newReadyThread(threadCreated);
	}
	else if (this->joinedAThread()) {

		// TODO: treat migration

		if (threadJoined->isCompleted() == false) {
			threadJoined->addThreadWaiting(currentThread);
			currentThread = NULL;
			scheduler->processorBecameIdle(this);
		}
	}
	else if (this->completedAThread()) {

		// TODO: treat migration

		list<Thread*> threadsToResume = this->threadCompleted->allThreadsWaiting();
		list<Thread*>::iterator it;
		for (it = threadsToResume.begin(); it != threadsToResume.end(); ++it) {
			scheduler->newReadyThread(*it);
		}
		scheduler->processorBecameIdle(this);
	}
	else {
        cout << "Processor " << id << " should'n have to communicate with scheduler..." << endl;
	}
}

void Processor::clearRecentHistory() {
	threadCreated = threadJoined = threadCompleted = NULL;
}

Thread* Processor::createdAThread() {
	return threadCreated;
}

Thread* Processor::joinedAThread() {
	return threadJoined;
}

Thread* Processor::completedAThread() {
	return threadCompleted;
}

