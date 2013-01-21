#include "AKTask.h"
#include <iostream>
#include <cassert>

// const char* AKTaskStateNames[] = {
// 	"AKTaskStateDefault",
// 	"AKTaskStateReady",
// 	"AKTaskStateRunning",
// 	"AKTaskStateFinished"
// };

const char* AKTaskStateNames[] = {
	"Default",
	"Ready",
	"Running",
	"Finished"
};

AKTask::AKTask(int cost) {
	_id = -1;
	_cost = cost;
	_priority = 0;
	_stepsRemaining = -1;
	_state = AKTaskStateDefault;
}

AKTask::~AKTask() {
	_successors.clear();
	_predecessors.clear();
}

void AKTask::addSuccessor(AKTask* task) {
	_successors.insert(task);
	task->_predecessors.insert(this);
}

void AKTask::addPredecessor(AKTask* task) {
	_predecessors.insert(task);
	task->_successors.insert(this);
}

void AKTask::tryGettingReady() {
	for (it = _predecessors.begin(); it != _predecessors.end(); ++it) {
		if ((*it)->state() != AKTaskStateFinished) {
			return;
		}
	}
	_state = AKTaskStateReady;
}

void AKTask::updateSuccessors() {
	assert(_state == AKTaskStateFinished);
	for (it = _successors.begin(); it != _successors.end(); ++it) {
		(*it)->tryGettingReady();
	}
}

const char* AKTask::stateString() {
	return AKTaskStateNames[_state];
}

void AKTask::printSet(std::set<AKTask*>& aSet) {
	std::cout << "[";
	for (it = aSet.begin(); it != aSet.end(); ++it) {
		if (it != aSet.begin()) {
			std::cout << ", ";
		}
		std::cout << (*it)->id();
	}
	std::cout << "]";
}