#include "AKThread.h"
#include "AKMTTask.h"
#include "AKSchedulingPoint.h"
#include "Defines.h"
#include <cstdlib>
#include <cassert>
#include <iostream>

int AKThread::instanceCounter = 0;

AKThread::AKThread() : AKSchedulingUnit() {
	_id = instanceCounter++;
	_currentTask = NULL;
}

AKThread::~AKThread() {
	// TODO : oposite of AKThread::generateFullyStrictDCG
}

int AKThread::currentTaskId() {
	assert(_currentTask != NULL);
	return _currentTask->id();
}

int AKThread::priority() { // TODO!!!!!!
	return _currentTask->priority();
}

void AKThread::generateFullyStrictDCG(int width, int depth, int cost, bool isVariableCost) {
	if (depth == 0) {
		_tasks.push_back(new AKMTTask(GEN_TASK_COST(cost, isVariableCost), this));
	} else {
		AKMTTask* previousTask = 0;
		for (int i = 0; i < width; ++i) {
			// create fork tasks, the next thread level, and connect them
			AKThread* thread = new AKThread;
			AKMTTask* fork = new AKMTTask(GEN_TASK_COST(cost, isVariableCost), this);
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
			AKMTTask* join = new AKMTTask(GEN_TASK_COST(cost, isVariableCost), this);
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
		AKMTTask* __lastTask = new AKMTTask(GEN_TASK_COST(cost, isVariableCost), this);
		__lastTask->addPredecessor(previousTask);
		assert(previousTask->isJoin());
		__lastTask->addPredecessor(previousTask->joinedThread()->lastTask());
		_tasks.push_back(__lastTask);
	}
	_currentTask = firstTask();
}

void AKThread::calculatePriorityAttributes() {
	assert(this->isRoot());
	this->firstTask()->levelET();
	this->lastTask()->coLevelET();
	this->firstTask()->levelNET();
	this->lastTask()->coLevelNET();
}

AKSchedulingPoint* AKThread::runStep() {
	assert(_state == AKSchedulingUnitStateRunning);
	assert(_currentTask != NULL);
	AKSchedulingPoint* sp = _currentTask->runStep();
	if (sp) {
		_currentTask = (AKMTTask*)_currentTask->continuation();
		if (!_currentTask) {
			_state = AKSchedulingUnitStateFinished;
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
	return AKSchedulingUnitStateNames[_state];	
}

void AKThread::addThreadWaiting(AKThread* blockedThread) {
	_threadsWaiting.push_back(blockedThread);
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

bool AKThread::validateSchedule() {
	// validate my own state
	/*if (_state != AKSchedulingUnitStateFinished) {
		return false;
	}*/

	// validate my tasks' state
	for (taskIt = _tasks.begin(); taskIt != _tasks.end(); ++taskIt) {
		if ((*taskIt)->state() != AKSchedulingUnitStateFinished) {
			return false;
		}
	}
	// validate my forked threads' state
	std::list<AKThread*> _forkedThreads = this->forkedThreads();
	std::list<AKThread*>::iterator threadIt;
	for (threadIt = _forkedThreads.begin(); threadIt != _forkedThreads.end(); ++threadIt) {
		if ((*threadIt)->validateSchedule() == false) {
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
		std::cout << std::endl;
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

void AKThread::prepareForSimulation() {
	_currentTask = firstTask();
	if (this->isRoot()) {
		_state = AKSchedulingUnitStateReady;
		_currentTask->prepareForSimulation();
	} else {
		_state = AKSchedulingUnitStateDefault;
	}
	std::list<AKThread*> _forkedThreads = this->forkedThreads();
	std::list<AKThread*>::iterator threadIt;
	FOR_EACH(threadIt, _forkedThreads) {
		(*threadIt)->prepareForSimulation();
	}
}