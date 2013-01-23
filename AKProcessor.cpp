#include "AKProcessor.h"
#include "AKSchedulingUnit.h"
#include "AKSchedulingPoint.h"
#include <cstdlib>

AKProcessor::AKProcessor() {
	_scheduler = NULL;
}

AKProcessor::~AKProcessor() {}

// remove last step
std::string AKProcessor::history() {
	unsigned pos = _history.find_last_of(";");
  	return _history.substr(0, pos);
}

// remove last step
std::string AKProcessor::activity() {
  	return _activity.substr(0, _activity.size()-1);
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