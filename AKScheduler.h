#include <list>
class AKSchedulingUnit;
class AKProcessor;

class AKScheduler {

	std::list<AKSchedulingUnit*> _readyJobs;
	std::list<AKProcessor*> _processors;
	std::list<AKProcessor*>::iterator procIt;

	int jobsPut;
	void printJobList(); // for debugging
public:
	AKSchedulingUnit* getJob();
	std::list<AKProcessor*> processors() { return _processors; }
	void putJob(AKSchedulingUnit* job);
	void prepareForSimulation(std::list<AKProcessor*> processorList, AKSchedulingUnit* rootJob);
};