#include "AKThread.h"
#include "AKMTTask.h"
#include "AKSchedulingPoint.h"
#include <cstdlib>
#include <cassert>
#include <iostream>

#define GEN_COST(c,v) v ? rand() % c + 1 : c

// const char* AKThreadStateNames[] = {
// 	"AKThreadStateDefault",
// 	"AKThreadStateReady",
// 	"AKThreadStateRunning",
// 	"AKThreadStateBlocked",
// 	"AKThreadStateFinished"
// };

const char* AKThreadStateNames[] = {
	"Default",
	"Ready",
	"Running",
	"Blocked",
	"Finished"
};

int AKThread::instanceCounter = 0;

AKThread::AKThread() {
	_id = instanceCounter++;
	_priority = 0;
	_state = AKThreadStateDefault;
	_currentTask = NULL;
}

AKThread::~AKThread() {}

int AKThread::currentTaskId() {
	assert(_currentTask != NULL);
	return _currentTask->id();
}

void AKThread::generateFullyStrictDCG(int width, int depth, int cost, bool isVariableCost) {
	if (depth == 0) {
		_tasks.push_back(new AKMTTask(GEN_COST(cost, isVariableCost), this));
	} else {
		AKMTTask* lastFork = 0;
		AKMTTask* lastJoin = 0;
		for (int i = 0; i < width; ++i) {
			// generate the next level first
			AKThread* thread = new AKThread;
			thread->generateFullyStrictDCG(width, depth-1, cost, isVariableCost);

			// create fork and join tasks
			AKMTTask* fork = new AKMTTask(GEN_COST(cost, isVariableCost), this);
			AKMTTask* join = new AKMTTask(GEN_COST(cost, isVariableCost), this);
			
			// connect them
			fork->setForkedThread(thread);
			join->setJoinedThread(thread);
			if (lastFork && lastJoin) {
				fork->addSuccessor(lastFork);
				join->addPredecessor(lastJoin);
				join->addPredecessor(lastJoin->joinedThread()->lastTask());
			} else {
				fork->addSuccessor(join);
			}
			fork->addSuccessor(thread->firstTask());
			_tasks.push_front(fork);
			_tasks.push_back(join);
			lastFork = fork;
			lastJoin = join;
		}
		AKMTTask* __lastTask = new AKMTTask(GEN_COST(cost, isVariableCost), this);
		__lastTask->addPredecessor(lastJoin);
		__lastTask->addPredecessor(lastJoin->joinedThread()->lastTask());
		_tasks.push_back(__lastTask);
	}
	_currentTask = _tasks.front();
}

AKSchedulingPoint* AKThread::runStep() {
	assert(_state == AKThreadStateRunning);
	assert(_currentTask != NULL);
	AKSchedulingPoint* sp = _currentTask->runStep();
	if (sp) {
		_currentTask = (AKMTTask*)_currentTask->continuation();
		if (!_currentTask) {
			_state = AKThreadStateFinished;
		}
	}
	return sp;
}

bool AKThread::isRoot() {
	return firstTask()->predecessors().empty();
}

AKMTTask* AKThread::firstTask() {
	return _tasks.front();	
}

AKMTTask* AKThread::lastTask() {
	return _tasks.back();	
}

const char* AKThread::stateString() {
	return AKThreadStateNames[_state];	
}

void AKThread::addThreadWaiting(AKThread* blockedThread) {
	_threadsWaiting.push_back(blockedThread);
}

void AKThread::setState(AKThreadState state) {
	if (state == AKThreadStateReady) {
		assert(_currentTask);
		_currentTask->setState(AKTaskStateReady);
	}
	_state = state;
}

std::list<AKThread*> AKThread::forkedThreads() {
	std::list<AKThread*> _forkedThreads;
	for (taskIt = _tasks.begin(); taskIt != _tasks.end(); ++taskIt) {
		if ((*taskIt)->isFork()) {
			_forkedThreads.push_back((*taskIt)->forkedThread());
		}
	}
	return _forkedThreads;	
}

bool AKThread::validate() {
	// validate my own state
	if (_state != AKThreadStateFinished) {
		return false;
	}
	// validate my tasks' state
	for (taskIt = _tasks.begin(); taskIt != _tasks.end(); ++taskIt) {
		if ((*taskIt)->state() != AKTaskStateFinished) {
			return false;
		}
	}
	// validate my forked threads' state
	std::list<AKThread*> _forkedThreads = this->forkedThreads();
	std::list<AKThread*>::iterator threadIt;
	for (threadIt = _forkedThreads.begin(); threadIt != _forkedThreads.end(); ++threadIt) {
		if ((*threadIt)->validate() == false) {
			return false;
		}
	}

	return true; // if I got here everything is OK
}

void AKThread::print() {
	if (this->isRoot()) {
		std::cout << "\nGraph:\n\n";
	}

	std::cout << "\tThread " << _id << ": (" << this->stateString() << ")\n";
	for (taskIt = _tasks.begin(); taskIt != _tasks.end(); ++taskIt) {
		(*taskIt)->print();
	}
	std::cout << "\n";
	if (_tasks.size() > 1) {
		for (taskIt = _tasks.begin(); taskIt != _tasks.end(); ++taskIt) {
			if ((*taskIt)->isFork()) {
				(*taskIt)->forkedThread()->print();
			}
		}
	}
}