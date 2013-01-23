#include <iostream>
#include <cassert>
#include "AKTask.h"
#include "Defines.h"

const char* AKTaskPriorityAttributeNames[] = {
	"LevelWithEstimatedTimes",
	"CoLevelWithEstimatedTimes",
	"LevelWithNonEstimatedTimes",
	"CoLevelWithNonEstimatedTimes",
	"Random",
	"DynamicLevel"
};


const char* nameFromPriorityAttribute(AKTaskPriorityAttribute value) {
	return AKTaskPriorityAttributeNames[value];
}

AKTaskPriorityAttribute AKTask::priorityAttribute = AKTaskPriorityAttributeLevelWithEstimatedTimes;

AKTask::AKTask(int cost) : AKSchedulingUnit(), _cost(cost) {
	_levelET=0;
	_coLevelET=0;
	_levelNET=0;
	_coLevelNET=0;
	_stepsRemaining = -1;
}

AKTask::~AKTask() {
	_successors.clear();
	_predecessors.clear();
}

void AKTask::setPriorityAttribute(AKTaskPriorityAttribute attr) {
	priorityAttribute = attr;
}

void AKTask::print() {
	std::cout << "\t\tTask " << _id << " (costs " << _cost;
	std::cout << "; pred: "; printSet(_predecessors);
	std::cout << "; succ: "; printSet(_successors);
	std::cout << "; level: " << this->levelET();
	std::cout << "; co-level: " << this->coLevelET();
	std::cout << "; state: " << AKSchedulingUnit::getNameFromState(_state) << ")";
}

bool AKTask::isInputTask() {
	return _predecessors.empty();
}

bool AKTask::isOutputTask() {
	return _successors.empty();
}

int AKTask::levelET() {
	if (_levelET == 0) { // calculate !
		if (this->isOutputTask()) {
			_levelET = -_cost;
		} else {
			for (it = _successors.begin(); it != _successors.end(); ++it) {
				_levelET = MIN(_levelET, (*it)->levelET());
			}
			_levelET -= _cost;
		}
	}
	return _levelET;
}

int AKTask::coLevelET() {
	if (_coLevelET == 0) { // calculate !
		if (this->isInputTask()) {
			_coLevelET = _cost;
		} else {
			for (it = _predecessors.begin(); it != _predecessors.end(); ++it) {
				_coLevelET = MAX(_coLevelET, (*it)->coLevelET());
			}
			_coLevelET += _cost;
		}
	}
	return _coLevelET;
}

int AKTask::levelNET() {
	if (_levelNET == 0) { // calculate !
		if (this->isOutputTask()) {
			_levelNET = -1;
		} else {
			for (it = _successors.begin(); it != _successors.end(); ++it) {
				_levelNET = MIN(_levelNET, (*it)->levelNET());
			}
			_levelNET -= 1;
		}
	}
	return _levelNET;
}

int AKTask::coLevelNET() {
	if (_coLevelNET == 0) { // calculate !
		if (this->isInputTask()) {
			_coLevelNET = 1;
		} else {
			for (it = _predecessors.begin(); it != _predecessors.end(); ++it) {
				_coLevelNET = MAX(_coLevelNET, (*it)->coLevelNET());
			}
			_coLevelNET += 1;
		}
	}
	return _coLevelNET;
}


int AKTask::priority() {
	switch(priorityAttribute) {
		case AKTaskPriorityAttributeLevelWithEstimatedTimes:
			return this->levelET();
		case AKTaskPriorityAttributeCoLevelWithEstimatedTimes:
			return this->coLevelET();
		case AKTaskPriorityAttributeLevelWithNonEstimatedTimes:
			return this->levelNET();
		case AKTaskPriorityAttributeCoLevelWithNonEstimatedTimes:
			return this->coLevelNET();
		case AKTaskPriorityAttributeRandom:
			return rand();
		default:
			std::cout << "Warning: unhandled priority attribute!\n";
			return 0;
	}
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
		if ((*it)->state() != AKSchedulingUnitStateFinished) {
			return;
		}
	}
	_state = AKSchedulingUnitStateReady;
}

void AKTask::updateSuccessors() {
	assert(_state == AKSchedulingUnitStateFinished);
	for (it = _successors.begin(); it != _successors.end(); ++it) {
		(*it)->tryGettingReady();
	}
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

void AKTask::prepareForSimulation() {
	_sentToScheduler = false;
	_stepsRemaining = -1;
	_state = this->isInputTask() ? AKSchedulingUnitStateReady : AKSchedulingUnitStateDefault;
	FOR_EACH(it, _successors) {
		(*it)->prepareForSimulation();
	}	
}

bool AKTask::validateSchedule() {
	if (_state != AKSchedulingUnitStateFinished) {
		return false;
	}
	FOR_EACH(it, _successors) {
		if (!(*it)->validateSchedule()) {
			return false;
		}
	}
	return true;
}