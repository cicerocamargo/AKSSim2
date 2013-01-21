#include "AKThreadScheduler.h"
#include "AKThreadProcessor.h"
#include "AKThread.h"
#include <cstdlib>
#include <cassert>

bool compareThreads(AKThread* first, AKThread* second) {
	return first->priority() <= second->priority() ? true : false;
}

AKThreadScheduler::AKThreadScheduler() {

}

AKThreadScheduler::~AKThreadScheduler() {
	std::list<AKThreadProcessor*>::iterator it;
	for (it = _processors.begin(); it != _processors.end(); ++it) {
		delete *it;
	}
	_processors.clear();
}

AKThread* AKThreadScheduler::getJob() {
	AKThread* t = NULL;
	if (_readyThreads.empty() == false) {
		t = _readyThreads.front();
		_readyThreads.pop_front();
	}
	return t;
}

void AKThreadScheduler::putJob(AKThread* thread) {
	_readyThreads.push_back(thread);
	_readyThreads.sort(compareThreads);
}

void AKThreadScheduler::setRootThread(AKThread* rootThread) {
	assert(rootThread->isRoot());
	_readyThreads.clear();
	rootThread->setState(AKThreadStateReady);
	putJob(rootThread);
}

void AKThreadScheduler::addProcessor(AKThreadProcessor* processor) {
	_processors.push_back(processor);
	processor->setScheduler(this);
}