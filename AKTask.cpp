#include <iostream>
#include <cassert>
#include "AKTask.h"
#include "Defines.h"

const char* AKTaskPriorityAttributeNames[] = {
	"Level",
	"Co-Level",
	"Dynamic Level"
};


const char* nameFromPriorityAttribute(AKTaskPriorityAttribute value) {
	return AKTaskPriorityAttributeNames[value];
}

AKTaskPriorityAttribute AKTask::priorityAttribute = AKTaskPriorityAttributeLevel;

AKTask::AKTask(int cost) : AKSchedulingUnit(), _cost(cost) {
	_level = 0;
	_coLevel = 0;
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
	std::cout << "; level: " << this->level();
	std::cout << "; co-level: " << this->coLevel();
	std::cout << "; state: " << AKSchedulingUnit::getNameFromState(_state) << ")";
}

bool AKTask::isInputTask() {
	return _predecessors.empty();
}

bool AKTask::isOutputTask() {
	return _successors.empty();
}

int AKTask::level() {
	if (_level == 0) { // calculate !
#ifdef DEBUG
		std::cout << "Calculating level for task " << _id << "...\n";
#endif
		if (this->isOutputTask()) {
			_level = -_cost;
		} else {
			for (it = _successors.begin(); it != _successors.end(); ++it) {
				_level = MIN(_level, (*it)->level());
			}
			_level -= _cost;
		}
	}
	return _level;
}

int AKTask::coLevel() {
		if (_coLevel == 0) { // calculate !
#ifdef DEBUG
		std::cout << "Calculating coLevel for task " << _id << "...\n";
#endif
		if (this->isInputTask()) {
			_coLevel = _cost;
		} else {
			for (it = _predecessors.begin(); it != _predecessors.end(); ++it) {
				_coLevel = MAX(_coLevel, (*it)->coLevel());
			}
			_coLevel += _cost;
		}
	}
	return _coLevel;
}

int AKTask::priority() {
	switch(priorityAttribute) {
		case AKTaskPriorityAttributeLevel:
			return this->level();
		case AKTaskPriorityAttributeCoLevel:
			return this->coLevel();
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
	_stepsRemaining = -1;
	_state = this->isInputTask() ? AKSchedulingUnitStateReady : AKSchedulingUnitStateDefault;
	FOR_EACH(it, _successors) {
		(*it)->prepareForSimulation();
	}	
}