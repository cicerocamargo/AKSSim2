#include "AKScheduler.h"
#include "AKProcessor.h"
#include "AKSchedulingUnit.h"
#include "Defines.h"
#include <cstdlib>
#include <cassert>
#include <iostream>

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

void AKScheduler::printJobList() {
	std::list<AKSchedulingUnit*>::iterator jobIt;
	std::cout << "\nJobs {\n";
	FOR_EACH(jobIt, _readyJobs) {
		std::cout << "\tid: " << (*jobIt)->id() << "\tpriority: " << (*jobIt)->priority() << std::endl;
	}
	std::cout << "}\n";
}

void AKScheduler::putJob(AKSchedulingUnit* job) {
	if (job->arrivalNumber() == -1) {
		job->setArrivalNumber(jobsPut++);
	}
	job->setSentToScheduler();
	_readyJobs.push_back(job);
	_readyJobs.sort(compareSchedulingUnits);
	//printJobList();
}

void AKScheduler::prepareForSimulation(std::list<AKProcessor*> processorList, AKSchedulingUnit* rootJob) {
	assert(rootJob->isRoot());
	assert(processorList.size() > 0);

	rootJob->prepareForSimulation();
	_readyJobs.clear();
	putJob(rootJob);
	jobsPut = 0;

	_processors.clear();
	_processors = processorList;
	FOR_EACH(procIt, _processors) {
		(*procIt)->setScheduler(this);
		(*procIt)->prepareForSimulation();
	}
}