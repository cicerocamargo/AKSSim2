#include <set>

class AKThread;

typedef enum {
	AKTaskStateDefault,
	AKTaskStateReady,
	AKTaskStateRunning,
	AKTaskStateFinished
} AKTaskState;

class AKTask {

protected:
	int _id;
	int _cost;
	int _priority;
	int _coLevel;
	int _stepsRemaining;
	AKTaskState _state;
	std::set<AKTask*> _predecessors;
	std::set<AKTask*> _successors;
	std::set<AKTask*>::iterator it;

	void tryGettingReady();
public:
	AKTask(int cost=0);
	~AKTask();

	int id() { return _id; }
	int cost() { return _cost; }
	int priority() { return _priority; }
	void setPriority(int priority) { _priority = priority; }
	AKTaskState state() { return _state; }
	void setState(AKTaskState state) { _state = state; }
	std::set<AKTask*>& predecessors() { return _predecessors; }
	std::set<AKTask*>& successors() { return _successors; }
	virtual AKThread* thread() { return 0; }
	virtual AKTask* continuation() { return 0; }

	void addSuccessor(AKTask* task);
	void addPredecessor(AKTask* task);	
	void updateSuccessors(); // only called after '_state = AKTaskStateFinished'
	const char* stateString();
	void printSet(std::set<AKTask*>& aSet);
};