#include <string>

class AKThread;
class AKThreadScheduler;
class AKSchedulingPoint;

class AKThreadProcessor {

protected:
	AKThreadScheduler* _scheduler;
	AKThread* _currentThread;
	AKSchedulingPoint* _schedulingPoint;
	std::string _history;
	std::string _activity;
	virtual void getJob() = 0;
public:
	AKThreadProcessor();
	~AKThreadProcessor();
	void setScheduler(AKThreadScheduler* scheduler) { _scheduler = scheduler; }
	std::string history() { return _history; }
	std::string activity() { return _activity; }
	std::string finalHistory();
	void runStep();
	virtual void commitSchedulingPoint() = 0;
	bool isIdle();
};