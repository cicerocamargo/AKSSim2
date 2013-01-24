#include "AKTaskProcessor.h"
#include "AKScheduler.h"
#include "AKSchedulingPoint.h"
#include "AKTask.h"
#include "Defines.h"
#include <set>
#include <cstdio>

AKTaskProcessor::AKTaskProcessor() {

}

AKTaskProcessor::~AKTaskProcessor() {

}

void AKTaskProcessor::getJob() {
	_currentJob = _scheduler->getJob();
	// if (_currentJob) {
	// 	_currentJob->setState(AKSchedulingUnitStateRunning);
	// }
}

void AKTaskProcessor::runStep() {
	if (this->isIdle()) {
		getJob();
	}

	if (this->isBusy()) {
		char str[40];
		sprintf(str, "%d\t; ", _currentJob->id());
		_history += str;
		_activity += "*";
		_schedulingPoint = _currentJob->runStep();
	} else {
		_history += "--\t; ";
		_activity += "-";
	}

}

void AKTaskProcessor::commitSchedulingPoint() {
	if (_schedulingPoint) {
		AKTask* currTask = (AKTask*)_currentJob;
		std::set<AKTask*> successors = currTask->successors();
		std::set<AKTask*>::iterator suc;
		FOR_EACH(suc, successors) {
			if ((*suc)->state() == AKSchedulingUnitStateReady && !(*suc)->sentToScheduler()) {
				(*suc)->setSentToScheduler();
				_scheduler->putJob(*suc);
			}
		}

		delete _schedulingPoint;
		_schedulingPoint = NULL;
		_currentJob = NULL;
	}
}