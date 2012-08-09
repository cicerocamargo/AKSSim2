//
//  Processor.h
//  AKSim
//
//  Created by Cícero Camargo on 09/08/12.
//  Copyright (c) 2012 Cícero Camargo. All rights reserved.
//


#include <string>
using namespace std;

class Task;
class Thread;
class Scheduler;
	
enum WorkingPolicy { WorkFirst, HelpFirst };

class Processor {
	static int instanceCounter;
	int id;
	WorkingPolicy workingPolicy;
	bool migration;

	Task* currentTask;
	Thread* currentThread;
	Scheduler* scheduler;

	string ganttChartRow;

	// to keep recent history
	Thread* threadCreated;
	Thread* threadJoined;
	Thread* threadCompleted;

	Processor(); // hidden to allow only pre-set processors
	void prepareForSchedulingPoint();

public:
	static Processor* processorWithWorkFirstPolicy();
	static Processor* processorWithHelpFirstPolicyAndMigrationEnabled();
	static Processor* processorWithHelpFirstPolicyAndMigrationDisabled();	
	
	void step();
	
	void setScheduler(Scheduler* s);
	void setCurrentThread(Thread* t);
	bool isIdle();
	bool reachedSchedulingPoint();
	void communicateWithScheduler();
	void clearRecentHistory();

	Thread* createdAThread();
	Thread* joinedAThread();
	Thread* completedAThread();
};	