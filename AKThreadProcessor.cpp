#include "AKThreadProcessor.h"
#include "AKThread.h"

// AKThreadProcessor::AKThreadProcessor() : AKProcessor() {}

// AKThreadProcessor::~AKThreadProcessor() : ~AKProcessor() {}

void AKThreadProcessor::runStep() {

	if (this->isIdle()) {
		getJob();
	}

	if (this->isBusy()) {
		char str[40];
		sprintf(str, "%d::%d\t; ", currentThread()->id(), currentThread()->currentTaskId());
		_history += str;
		_activity += "*";
		_schedulingPoint = currentThread()->runStep();
	} else {
		_history += "----\t; ";
		_activity += "-";
	}
}

void AKThreadProcessor::setCurrentThread(AKThread* thread) { _currentJob = (AKSchedulingUnit*)thread; }

AKThread* AKThreadProcessor::currentThread() { return (AKThread*)_currentJob; }