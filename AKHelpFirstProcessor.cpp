#include "AKHelpFirstProcessor.h"
#include "AKSchedulingPoint.h"
#include "AKScheduler.h"
#include "AKThread.h"
#include <iostream>
#include <cstdlib>

AKHelpFirstProcessor::AKHelpFirstProcessor(bool migrationEnabled)
: AKThreadProcessor(), _migrationEnabled(migrationEnabled) {}

void AKHelpFirstProcessor::commitSchedulingPoint() {
	if (_schedulingPoint) {
		switch(_schedulingPoint->event) {
			case AKSchedulingEventThreadForked:
				_schedulingPoint->relatedThread->setState(AKSchedulingUnitStateReady);
				_scheduler->putJob((AKSchedulingUnit*)_schedulingPoint->relatedThread);
				break;
			case AKSchedulingEventThreadJoined:
				if (_schedulingPoint->relatedThread->state() != AKSchedulingUnitStateFinished) {
					currentThread()->setState(AKSchedulingUnitStateBlocked);
					_schedulingPoint->relatedThread->addThreadWaiting(currentThread());
					if (!_migrationEnabled) {
						_blockedThreads.push(currentThread());
					}
					setCurrentThread(NULL);
				}
				break;
			case AKSchedulingEventThreadCompleted: {
				std::list<AKThread*>& threadsToUnblock = currentThread()->threadsWaiting();
				std::list<AKThread*>::iterator it;
				for (it = threadsToUnblock.begin(); it != threadsToUnblock.end(); ++it) {
					(*it)->setState(AKSchedulingUnitStateReady);
					if (_migrationEnabled) {
						_scheduler->putJob(*it);
					}
				}
				threadsToUnblock.clear();
				setCurrentThread(NULL);
				break;
			}	
			default:
				std::cerr << "Unknown scheduling event...\n";
				abort();
		}
		delete _schedulingPoint;
		_schedulingPoint = NULL;
	}

}

void AKHelpFirstProcessor::getJob() {
	if (_blockedThreads.size() > 0) {
		AKThread* t = _blockedThreads.top();
		if (t->state() == AKSchedulingUnitStateReady) {
			setCurrentThread(t);
			_blockedThreads.pop();
		}	
	}

	if (!currentThread()) {
		setCurrentThread((AKThread*)_scheduler->getJob());
	}

	if (currentThread()) {
		currentThread()->setState(AKSchedulingUnitStateRunning);
	}
}