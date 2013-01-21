#include <list>

class AKSchedulingPoint;
class AKMTTask;

typedef enum {
	AKThreadStateDefault,
	AKThreadStateReady,
	AKThreadStateRunning,
	AKThreadStateBlocked,
	AKThreadStateFinished
} AKThreadState;

class AKThread {
	static int instanceCounter;

	int _id;
	int _priority;
	AKThreadState _state;
	std::list<AKMTTask*> _tasks;
	AKMTTask* _currentTask;
	std::list<AKThread*> _threadsWaiting;
	std::list<AKMTTask*>::iterator taskIt;
public:
	AKThread();
	~AKThread();
	
	std::list<AKMTTask*>& tasks() { return _tasks; }
	AKThreadState state() { return _state; }
	void setState(AKThreadState state);
	int id() { return _id; }
	int priority();
	void setPriority(int priority) { _priority = priority; }
	AKMTTask* currentTask() { return _currentTask; }
	std::list<AKThread*>& threadsWaiting() { return _threadsWaiting; }

	int currentTaskId();
	void generateFullyStrictDCG(int width, int depth, int cost, bool isVariableCost);
	AKSchedulingPoint* runStep();
	bool isRoot();
	AKMTTask* firstTask();
	AKMTTask* lastTask();
	const char* stateString();
	void addThreadWaiting(AKThread* blockedThread);
	std::list<AKThread*> forkedThreads();
	bool validate();
	void print();
};