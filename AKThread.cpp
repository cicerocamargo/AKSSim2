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

int AKThread::priority() {
	return currentTaskId();
}

void AKThread::generateFullyStrictDCG(int width, int depth, int cost, bool isVariableCost) {
	if (depth == 0) {
		_tasks.push_back(new AKMTTask(GEN_COST(cost, isVariableCost), this));
	} else {
		AKMTTask* previousTask = 0;
		for (int i = 0; i < width; ++i) {
			// create fork tasks, the next thread level, and connect them
			AKThread* thread = new AKThread;
			AKMTTask* fork = new AKMTTask(GEN_COST(cost, isVariableCost), this);
			thread->generateFullyStrictDCG(width, depth-1, cost, isVariableCost);
			fork->setForkedThread(thread);
			fork->addSuccessor(thread->firstTask());
			if (previousTask) {
				previousTask->addSuccessor(fork);
			}
			_tasks.push_back(fork);
			previousTask = fork;
		}

		// create join tasks and connect them to the right forks
		std::list<AKMTTask*> joinTasks;
		std::list<AKMTTask*>::reverse_iterator it;
		for (it = _tasks.rbegin(); it != _tasks.rend(); ++it) {
			AKMTTask* join = new AKMTTask(GEN_COST(cost, isVariableCost), this);
			join->setJoinedThread((*it)->forkedThread());
			join->addPredecessor(previousTask);
			if (previousTask->isJoin()) {
				join->addPredecessor(previousTask->joinedThread()->lastTask());	
			}
			joinTasks.push_back(join);
			previousTask = join;
		}

		// add join tasks to the task list
		std::list<AKMTTask*>::iterator it2;
		for (it2 = joinTasks.begin(); it2 != joinTasks.end(); ++it2) {
			_tasks.push_back(*it2);
		}

		// create the last task and connect it to the last join
		AKMTTask* __lastTask = new AKMTTask(GEN_COST(cost, isVariableCost), this);
		__lastTask->addPredecessor(previousTask);
		assert(previousTask->isJoin());
		__lastTask->addPredecessor(previousTask->joinedThread()->lastTask());
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