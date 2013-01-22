#include "AKProcessor.h"
#include "AKSchedulingUnit.h"
#include "AKSchedulingPoint.h"
#include <cstdlib>

AKProcessor::AKProcessor() {
	_scheduler = NULL;
}

AKProcessor::~AKProcessor() {}

std::string AKProcessor::finalHistory() { // TODO
	unsigned pos = _history.find_last_of(";");
  	return _history.substr(0, pos);
}

bool AKProcessor::isIdle() {
	return _currentJob == NULL;
}

void AKProcessor::prepareForSimulation() {
	_history.clear();
	_activity.clear();
	_currentJob = NULL;
	_schedulingPoint = NULL;
}