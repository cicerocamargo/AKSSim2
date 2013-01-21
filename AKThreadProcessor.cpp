#include "AKThreadProcessor.h"
#include "AKThread.h"
#include "AKSchedulingPoint.h"
#include <cstdlib>

AKThreadProcessor::AKThreadProcessor() {
	_scheduler = NULL;
	_currentThread = NULL;
	_schedulingPoint = NULL;
}

AKThreadProcessor::~AKThreadProcessor() {}

std::string AKThreadProcessor::finalHistory() { return _history.substr(0, _history.size()-10); }

void AKThreadProcessor::runStep() {

	if (this->isIdle()) {
		getJob();
	}

	if (_currentThread) {
		char str[40];
		sprintf(str, "%d::%d\t; ", _currentThread->id(), _currentThread->currentTaskId());
		_history += str;
		_activity += "*";
		_schedulingPoint = _currentThread->runStep();
	} else {
		_history += "----\t; ";
		_activity += "-";
	}
}

bool AKThreadProcessor::isIdle() {
	return _currentThread == NULL;
}