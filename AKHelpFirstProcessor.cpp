#include "AKHelpFirstProcessor.h"
#include "AKSchedulingPoint.h"
#include "AKThreadScheduler.h"
#include "AKThread.h"
#include <iostream>

AKHelpFirstProcessor::AKHelpFirstProcessor(bool migrationEnabled)
: AKThreadProcessor(), _migrationEnabled(migrationEnabled) {}

void AKHelpFirstProcessor::commitSchedulingPoint() {
	if (_schedulingPoint) {
		switch(_schedulingPoint->event) {
			case AKSchedulingEventThreadForked:
				_schedulingPoint->relatedThread->setState(AKThreadStateReady);
				_scheduler->putJob(_schedulingPoint->relatedThread);
				break;
			case AKSchedulingEventThreadJoined:
				if (_schedulingPoint->relatedThread->state() != AKThreadStateFinished) {
					_currentThread->setState(AKThreadStateBlocked);
					_schedulingPoint->relatedThread->addThreadWaiting(_currentThread);
					if (!_migrationEnabled) {
						_blockedThreads.push(_currentThread);
					}
					_currentThread = NULL;
				}
				break;
			case AKSchedulingEventThreadCompleted: {
				std::list<AKThread*>& threadsToUnblock = _currentThread->threadsWaiting();
				std::list<AKThread*>::iterator it;
				for (it = threadsToUnblock.begin(); it != threadsToUnblock.end(); ++it) {
					(*it)->setState(AKThreadStateReady);
					if (_migrationEnabled) {
						_scheduler->putJob(*it);
					}
				}
				threadsToUnblock.clear();
				_currentThread = NULL;
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
		if (t->state() == AKThreadStateReady) {
			_currentThread = t;
			_blockedThreads.pop();
		}	
	}

	if (!_currentThread) {
		_currentThread = _scheduler->getJob();
	}

	if (_currentThread) {
		_currentThread->setState(AKThreadStateRunning);
	}
}