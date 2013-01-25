#include "AKWorkFirstProcessor.h"
#include "AKSchedulingPoint.h"
#include "AKScheduler.h"
#include "AKThread.h"
#include <iostream>
#include <cstdlib>

AKWorkFirstProcessor::AKWorkFirstProcessor() : AKThreadProcessor() {}

void AKWorkFirstProcessor::commitSchedulingPoint() {
	if (_schedulingPoint) {
		switch(_schedulingPoint->event) {
			case AKSchedulingEventThreadForked:
				currentThread()->setState(AKSchedulingUnitStateReady);
				_scheduler->putJob((AKSchedulingUnit*)currentThread());
				setCurrentThread(_schedulingPoint->relatedThread);
				currentThread()->setState(AKSchedulingUnitStateRunning);
				break;
			case AKSchedulingEventThreadJoined:
				if (_schedulingPoint->relatedThread->state() != AKSchedulingUnitStateFinished) {
					currentThread()->setState(AKSchedulingUnitStateBlocked);
					_schedulingPoint->relatedThread->addThreadWaiting(currentThread());
					setCurrentThread(NULL);
				}
				break;
			case AKSchedulingEventThreadCompleted: {
				std::list<AKThread*>& threadsToUnblock = currentThread()->threadsWaiting();
				std::list<AKThread*>::iterator it;
				for (it = threadsToUnblock.begin(); it != threadsToUnblock.end(); ++it) {
					(*it)->setState(AKSchedulingUnitStateReady);
					(*it)->setArrivalNumber(-1);
					_scheduler->putJob(*it);
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

void AKWorkFirstProcessor::getJob() {	
	setCurrentThread((AKThread*)_scheduler->getJob());
	if (currentThread()) {
		currentThread()->setState(AKSchedulingUnitStateRunning);
	}
}