#ifndef AKPROCESSOR_H
#define AKPROCESSOR_H

#include <string>

class AKScheduler;
class AKSchedulingUnit;
class AKSchedulingPoint;

class AKProcessor {

protected:
	AKScheduler* _scheduler;
	AKSchedulingUnit* _currentJob;
	AKSchedulingPoint* _schedulingPoint;
	std::string _history;
	std::string _activity;
	virtual void getJob() = 0;
public:
	AKProcessor();
	~AKProcessor();
	void setScheduler(AKScheduler* scheduler) { _scheduler = scheduler; }
	std::string history();
	std::string activity();
	virtual void runStep() = 0;
	virtual void commitSchedulingPoint() = 0;
	bool isIdle();
	bool isBusy() { return !isIdle(); }
	void prepareForSimulation();
};

#endif