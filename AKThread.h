#include "AKSchedulingUnit.h"
#include <list>

class AKMTTask;

typedef enum {
	AKThreadPriorityAttributeStaticCoLevelWithEstimatedTimes,
	AKThreadPriorityAttributeStaticCoLevelWithNonEstimatedTimes,
	AKThreadPriorityAttributeDynamicCoLevelWithEstimatedTimes,
	AKThreadPriorityAttributeDynamicCoLevelWithNonEstimatedTimes,
	AKThreadPriorityAttributeSequentialOrder,
	AKThreadPriorityAttributeRandom,
	AKThreadPriorityAttributeArrivalOrder
} AKThreadPriorityAttribute;

class AKThread : public AKSchedulingUnit {
	static int instanceCounter;
	static AKThreadPriorityAttribute priorityAttribute;

	std::list<AKMTTask*> _tasks;
	AKMTTask* _currentTask;
	std::list<AKThread*> _threadsWaiting;
	std::list<AKMTTask*>::iterator taskIt;
public:
	AKThread();
	~AKThread();
	
	static void setPriorityAttribute(AKThreadPriorityAttribute attr);
	static std::list<AKThreadPriorityAttribute> listWithPrioriyAttributes();
	static const char* algorithmNameFromPriorityAttribute(AKThreadPriorityAttribute attr);

	std::list<AKMTTask*>& tasks() { return _tasks; }
	virtual int priority();
	AKMTTask* currentTask() { return _currentTask; }
	std::list<AKThread*>& threadsWaiting() { return _threadsWaiting; }

	int currentTaskId();
	void generateFullyStrictDCG(int width, int depth, int cost, bool isVariableCost);
	void calculatePriorityAttributes();
	virtual AKSchedulingPoint* runStep();
	virtual void prepareForSimulation();
	bool isRoot();
	AKMTTask* firstTask();
	AKMTTask* lastTask();
	const char* stateString();
	void addThreadWaiting(AKThread* blockedThread);
	std::list<AKThread*> forkedThreads();
	bool validateSchedule();
	void print();
};