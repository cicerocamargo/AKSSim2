#include "AKWorkFirstProcessor.h"
#include "AKSchedulingPoint.h"
#include "AKThreadScheduler.h"
#include "AKThread.h"
#include <iostream>

AKWorkFirstProcessor::AKWorkFirstProcessor() : AKThreadProcessor() {

}

void AKWorkFirstProcessor::commitSchedulingPoint() {
	if (_schedulingPoint) {
		switch(_schedulingPoint->event) {
			case AKSchedulingEventThreadForked:
				_currentThread->setState(AKThreadStateReady);
				_scheduler->putJob(_currentThread);
				_currentThread = _schedulingPoint->relatedThread;
				_currentThread->setState(AKThreadStateRunning);
				break;
			case AKSchedulingEventThreadJoined:
				if (_schedulingPoint->relatedThread->state() != AKThreadStateFinished) {
					_currentThread->setState(AKThreadStateBlocked);
					_schedulingPoint->relatedThread->addThreadWaiting(_currentThread);
					_currentThread = NULL;
				}
				break;
			case AKSchedulingEventThreadCompleted: {
				std::list<AKThread*>& threadsToUnblock = _currentThread->threadsWaiting();
				std::list<AKThread*>::iterator it;
				for (it = threadsToUnblock.begin(); it != threadsToUnblock.end(); ++it) {
					(*it)->setState(AKThreadStateReady);
					_scheduler->putJob(*it);
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

void AKWorkFirstProcessor::getJob() {	
	if (_currentThread = _scheduler->getJob()) {
		_currentThread->setState(AKThreadStateRunning);
	}
}