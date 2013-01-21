#include "AKTask.h"

class AKThread;
class AKSchedulingPoint;

class AKMTTask : public AKTask {
	static int instanceCounter;
	AKThread* _thread;
	AKThread* _forkedThread;
	AKThread* _joinedThread;

public:
	AKMTTask(int cost=0, AKThread* thread=0);
	//~AKMTTask();

	void setThread(AKThread* thread) { _thread = thread; }
	virtual AKThread* thread() { return _thread; }

	void setForkedThread(AKThread* thread);
	AKThread* forkedThread() { return _forkedThread; }

	void setJoinedThread(AKThread* thread);
	AKThread* joinedThread() { return _joinedThread; }

	bool isJoin() { return _joinedThread != 0; }
	bool isFork() { return _forkedThread != 0; }

	virtual AKTask* continuation();
	AKSchedulingPoint* runStep();

	void print();
};