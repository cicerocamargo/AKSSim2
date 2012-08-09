//
//  Thread.h
//  AKSim
//
//  Created by Cícero Camargo on 09/08/12.
//  Copyright (c) 2012 Cícero Camargo. All rights reserved.
//

#include <list>
#include <set>
using namespace std;

class Task;

enum ThreadState { ThreadDefault, ThreadReady, ThreadRunning, ThreadCompleted, ThreadBlocked };


class Thread {
	static int counter; // the number of existing threads 
	int id;		
	int cost;	// tasks' processing cost
	list<Task*> tasks; 	// list of tasks encapsulated in this thread
	Thread* parent;		// poiter to the thread which created this
    
    list<Thread*> threadsWaiting; // threads that are waiting for me to continue
    
	// runtime information
	ThreadState state;
	Task* blockingTask;  
public:	
	// Class constructors
	Thread(int id = 0);
	Thread(int depth, int width, int cost, int deviation, Thread* parent = NULL, bool nested=true, bool randomWidth=false);
	
	void setParent(Thread* t) { parent = t; }
	int getId(){ return id; }
	int getCost(){ return cost; }
	int getCounter() { return counter; }
	
	void setState(ThreadState s){ state = s; }
	ThreadState getState() { return state; }
	
	void addTask(Task* t);
	list<Task*>& getTasks() { return tasks; }
	Task* nextReadyTask();
	Task* getBlockingTask(){ return blockingTask; }
	void setBlockingTask( Task* t ){ blockingTask = t; } 
		
	void getDAG(set<Task*>& dag, int* startingId);
	Task* getInputTask(){ return tasks.front(); }
	Task* getOutputTask(){ return tasks.back(); }
	
	// shows object state
	void display();
	void displayDCG();
	void initDCG();
	
	void fixDCG( bool called_from_gxl2dcg = false );

    
    Task* getCurrentTask();
    bool isCompleted();
    void addThreadWaiting(Thread* t);
    list<Thread*> allThreadsWaiting();
};
