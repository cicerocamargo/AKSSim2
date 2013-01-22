#include <set>
#include "AKSchedulingUnit.h"

class AKThread;

typedef enum {
	AKTaskPriorityAttributeLevel,
	AKTaskPriorityAttributeCoLevel,
	AKTaskPriorityAttributeDynamicLevel
} AKTaskPriorityAttribute;

class AKTask : public AKSchedulingUnit {

protected:
	int _cost;
	int _level;
	int _coLevel;
	int _stepsRemaining;
	std::set<AKTask*> _predecessors;
	std::set<AKTask*> _successors;
	std::set<AKTask*>::iterator it;

	void tryGettingReady();
	static AKTaskPriorityAttribute priorityAttribute;
public:
	AKTask(int cost=0);
	~AKTask();
	static void setPriorityAttribute(AKTaskPriorityAttribute attr);

	int cost() { return _cost; }
	int stepsRemaining() { return _stepsRemaining; }
	std::set<AKTask*> predecessors() { return _predecessors; }
	std::set<AKTask*> successors() { return _successors; }
	virtual AKThread* thread() = 0;
	virtual AKTask* continuation() = 0;
	virtual void print();
	virtual void prepareForSimulation();

	int level();
	int coLevel();
	bool isInputTask();
	bool isOutputTask();
	virtual bool isRoot() { return isInputTask(); }
	virtual int priority();
	void calculateLevels();
	void addSuccessor(AKTask* task);
	void addPredecessor(AKTask* task);	
	void updateSuccessors(); // only called after '_state = AKTaskStateFinished'
	const char* stateString();
	void printSet(std::set<AKTask*>& aSet);
};