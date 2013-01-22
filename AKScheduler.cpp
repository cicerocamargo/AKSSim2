#include "AKScheduler.h"
#include "AKProcessor.h"
#include "AKSchedulingUnit.h"
#include "Defines.h"
#include <cstdlib>
#include <cassert>

bool compareSchedulingUnits(AKSchedulingUnit* first, AKSchedulingUnit* second) {
	return first->priority() <= second->priority() ? true : false;
}

AKSchedulingUnit* AKScheduler::getJob() {
	AKSchedulingUnit* job = NULL;
	if (_readyJobs.empty() == false) {
		job = _readyJobs.front();
		_readyJobs.pop_front();
	}
	return job;
}

void AKScheduler::putJob(AKSchedulingUnit* job) {
	job->setSentToScheduler();
	_readyJobs.push_back(job);
	_readyJobs.sort(compareSchedulingUnits);
}

void AKScheduler::prepareForSimulation(std::list<AKProcessor*> processorList, AKSchedulingUnit* rootJob) {
	assert(rootJob->isRoot());
	assert(processorList.size() > 0);
	
	rootJob->prepareForSimulation();
	_readyJobs.clear();
	putJob(rootJob);

	_processors.clear();
	_processors = processorList;
	FOR_EACH(procIt, _processors) {
		(*procIt)->setScheduler(this);
		(*procIt)->prepareForSimulation();
	}
}