//
//  Task.h
//  AKSim
//
//  Created by Cícero Camargo on 09/08/12.
//  Copyright (c) 2012 Cícero Camargo. All rights reserved.
//


#include <list>
#include <string>
#include <map>

using namespace std;

enum taskState{ DEFAULT, READY, EXECUTING, EXECUTED, FREE };

class Thread;

class Task {
	static int counter; // counts the amount of tasks created
	
	int id;		// task ID
	int cost;	// processing cost of the task
	list<Task*> successors; 	// list of successors int the DAG
	list<Task*> predecessors;	// list of predecessors int the DAG
	Task* pred; // predecessor in the thread
	Task* suc;  // successor in the thread
	Thread* thread;  // thread in which this task is
	list<Thread*> created; // list of threads created by this task 
	list<Thread*> joined;  // list of threads joined by this task
		
//gxl files estra info
	string gxlId;
	
//scheduling extra info
	int tLevel, bLevel;
	taskState state; // task's state during scheduling
	bool critical; // true if this task is in the critical path
public:	
	Task(int id=0, int cost=0, Thread* t=NULL, Thread* c=NULL, Thread* j=NULL); // Class constructor
	
	int getCounter(){ return counter; }
	int getId(){ return id; }
	void setId(int id) { this->id = id; }
	void fixId();
	void setGxlId(string id){ gxlId = id; }
	string getGxlId(){ return gxlId; }
	
	Task* getPred(){ return pred; }
	Task* getSuc(){ return suc; }
	
	int getCost(){ return cost; }
	void setCost(int cost){ this->cost = cost; }
	
	void addSuccessor(Task* n){	successors.push_back(n); }
	list<Task*>& getSuccessors() { return successors; }
	void setSuccessorInThread();
	
	void addPredecessor(Task* n){ predecessors.push_back(n); }
	list<Task*>& getPredecessors() { return predecessors; }
	void setPredecessorInThread();
	
	void addCreated(Thread* t) { created.push_back(t); }
	list<Thread*>& getCreated() { return created; }

	void addJoined(Thread* t) { joined.push_back(t); }
	list<Thread*>& getJoined() { return joined; }
	
	void setThread(Thread* t){ thread = t; }
	Thread* getThread() { return thread; }
	
	bool isInputTask() { return predecessors.empty(); }
	bool isOutputTask() { return successors.empty(); }
	bool isCreate() { return !created.empty(); }
	bool isJoin() { return !joined.empty(); }
	bool isInputOfThread();
	bool isOutputOfThread();
	
	void setThreadsFromTasks();
	
	// shows object internal state
	void display();
	
	void setTLevel();
	int getTLevel(){ return tLevel; }
	void setBLevel();
	int getBLevel(){ return bLevel; }
	void setState(taskState s);
	taskState getState(){ return state; }
	void updateSuccessors();
	void tryGettingReady();
	bool getCritical() { return critical; }
	void setCritical() { critical = true; }
    
    Thread* getAssociatedThread();
    void step();
    bool isCompleted();
};


