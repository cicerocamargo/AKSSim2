#include "AKSchedulingPoint.h"

const char* AKSchedulingEventNames[] = {
	"AKSchedulingEventThreadForked",
	"AKSchedulingEventThreadJoined",
	"AKSchedulingEventThreadCompleted"
};

AKSchedulingPoint::AKSchedulingPoint(AKThread* relatedThread, AKSchedulingEvent event) {
	this->relatedThread = relatedThread;
	this->event = event;
}

const char* AKSchedulingPoint::eventString() {
	return AKSchedulingEventNames[event];	
}

AKSchedulingPoint* AKSchedulingPoint::forkSchedulingPoint(AKThread* forkedThread) {
	return new AKSchedulingPoint(forkedThread, AKSchedulingEventThreadForked);
}

AKSchedulingPoint* AKSchedulingPoint::joinSchedulingPoint(AKThread* joinedThread) {
	return new AKSchedulingPoint(joinedThread, AKSchedulingEventThreadJoined);
}

AKSchedulingPoint* AKSchedulingPoint::completionSchedulingPoint(AKThread* completedThread) {
	return new AKSchedulingPoint(completedThread, AKSchedulingEventThreadCompleted);
}