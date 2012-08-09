//
//  Thread.cpp
//  AKSim
//
//  Created by Cícero Camargo on 09/08/12.
//  Copyright (c) 2012 Cícero Camargo. All rights reserved.
//


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "thread.h"
#include "task.h"

using namespace std;

int Thread::counter = 0;

/*
** funtion used to generate a cost in a definite range
*/
int costGen(int cost, int deviation){
	int _signal, _cost;
	deviation++;
	_signal = rand() % 2; // 0 ou 1
	
	if( _signal )
		_cost = cost + (rand() % deviation);
	else
		_cost = cost - (rand() % deviation);	

	if(_cost < 1) _cost = 1;
	return _cost;
}

// minimal class constructor
Thread::Thread(int id){
	if (id) this->id = id;
	else this->id = counter++;
	state = ThreadDefault;
	blockingTask = NULL;
}

/*
** Constructor for a thorough Anahy DCG
*/
Thread::Thread(int depth, int width, int cost, int deviation, Thread* parent, bool nested, bool randomWidth){
	list<Thread*> threads;
	list<Thread*>::iterator itThreads;
	list<Task*>::iterator itTasks, aux;
	int cont = 1, i, j;
	Task* t;
	
	blockingTask = NULL;
	state = ThreadDefault;
	this->parent = parent;
	this->id = ++counter;
	int _width; //GENERATES A NEW WIDTH IN THE RANGE 1 - width
	
 	if( randomWidth ) _width = rand() % width + 1;
	else _width = width;
	
	if(cost==0) cost = 1;
	
	if(depth > 0){
		for(i = 0; i < _width; ++i){ // creates
			threads.push_front(new Thread(depth-1, width, cost, deviation, this, nested, randomWidth)); // store created threads
			tasks.push_back( new Task(cont++, costGen(cost, deviation), this, threads.front(), NULL) );
		}
		// joins...
		if( nested ){
			for(itThreads = threads.begin(); itThreads != threads.end(); itThreads++) 
				tasks.push_back( new Task(cont++, costGen(cost, deviation), this, NULL, *itThreads) );
		}
		else{
			while( !threads.empty() ){
				i = rand() % threads.size();
				itThreads = threads.begin();
				for(j=0; j<i ; itThreads++, j++);
				if(itThreads != threads.end()){
					tasks.push_back( new Task(cont++, costGen(cost, deviation), this, NULL, *itThreads) );
					threads.remove(*itThreads);
				}
				// cout<< "Threads: ";
				// 				for(itThreads = threads.begin(); itThreads != threads.end(); itThreads++)
				// 					cout << (*itThreads)->getId() << " ";
				// 				cout << endl;
				// 				getchar();
			}
		}
	}
	tasks.push_back( new Task(cont++, costGen(cost, deviation), this) );
	
	// fix successors
	for(itTasks = tasks.begin(), aux = itTasks++; itTasks != tasks.end(); ++itTasks, ++aux){
		(*aux)->addSuccessor(*itTasks);
		(*itTasks)->addPredecessor(*aux);
		if( (*aux)->isCreate() ){
			t = ((*aux)->getCreated()).front()->getInputTask();
			(*aux)->addSuccessor(t);
			t->addPredecessor(*aux);
		}
		if( (*aux)->isJoin() ){
			t = ((*aux)->getJoined()).front()->getOutputTask();
			(*itTasks)->addPredecessor(t);
			t->addSuccessor(*itTasks);
		}
	}
	
//	display();
	
	fixDCG();
}

void Thread::addTask(Task* t){
	tasks.push_back(t);
	t->setThread(this);
}

/*
** Get internal dag from the root thread fixing ids
** all tasks are inserted in set<Task*>& dag
*/
void Thread::getDAG(set<Task*>& dag, int* startingId){
	list<Task*>::iterator itTasks;
	list<Thread*>::iterator itThreads;
	list<Thread*> created;
	
	for(itTasks = tasks.begin() ; itTasks != tasks.end() ; ++itTasks){
		(*itTasks)->setId((*startingId));
		*startingId = *startingId + 1;
		dag.insert(*itTasks);
	}

	for(itTasks = tasks.begin() ; itTasks != tasks.end() ; ++itTasks){
		if( (*itTasks)->isCreate() ){
			created = (*itTasks)->getCreated();
			for(itThreads = created.begin(); itThreads != created.end(); ++itThreads)
				(*itThreads)->getDAG(dag,startingId);
		}
	}
}

/*
** Display threads info
*/
void Thread::display(){
	list<Task*>::iterator it;
	cout << "Thread " << id << endl;
	for(it = tasks.begin(); it != tasks.end() ; it++){
		cout << "\t";
		(*it)->display();
		cout << endl;
	}
}

/*
** Display the entire DCG using Thread::display()
*/
void Thread::displayDCG(){
	display();
	list<Task*>::iterator itTasks;
	list<Thread*>::iterator itThreads;
	list<Thread*> created;
	for(itTasks=tasks.begin() ; itTasks != tasks.end() ; ++itTasks)
		if( (*itTasks)->isCreate() ){
			created = (*itTasks)->getCreated();
			for(itThreads=created.begin() ; itThreads != created.end() ; ++itThreads)
				(*itThreads)->displayDCG();
		}
}

/*
** Set every thread's state as DEFAULT
*/
void Thread::initDCG(){
	state = ThreadDefault;
	list<Task*>::iterator itTasks;
	list<Thread*>::iterator itThreads;
	list<Thread*> created;
	for(itTasks=tasks.begin() ; itTasks != tasks.end() ; ++itTasks)
		if( (*itTasks)->isCreate() ){
			created = (*itTasks)->getCreated();
			for(itThreads=created.begin() ; itThreads != created.end() ; ++itThreads)
				(*itThreads)->initDCG();
		}
}

// function used to sort tasks by the id
bool compareTasks(Task* t1, Task* t2){
	if (t1->getId() < t2->getId()) return true;
	return false;
}

void Thread::fixDCG( bool fixTasksId ){
	list<Task*>::iterator it;
	// for each task in this thread...
	for(it = tasks.begin(); it != tasks.end() ; it++){
		(*it)->setPredecessorInThread();
		(*it)->setSuccessorInThread();
		if (fixTasksId) (*it)->setThreadsFromTasks();
	}
		
	if( fixTasksId ){
		for(it = tasks.begin(); it != tasks.end(); ++it)
			if( (*it)->isInputOfThread() ){
				(*it)->fixId();
				break;
			}
		// sort tasks by id!
		tasks.sort(compareTasks);
	}
	else{
		int i;
		for(i=1, it = tasks.begin(); it != tasks.end(); ++it, ++i)
			(*it)->setId( i );
	}

	// fix threads' parents
	list<Thread*> created;
	list<Thread*>::iterator itThreads;
	for(it = tasks.begin(); it != tasks.end() ; it++)
		if( (*it)->isCreate() ){
			created = (*it)->getCreated();
			for(itThreads = created.begin(); itThreads != created.end() ; itThreads++){
				(*itThreads)->setParent(this);
				(*itThreads)->fixDCG( fixTasksId );
			}
		}			
}

Task* Thread::nextReadyTask(){
	list<Task*>::iterator itTasks;
	for(itTasks=tasks.begin() ; itTasks != tasks.end() ; ++itTasks)
		if( (*itTasks)->getState() == READY || (*itTasks)->getState() == FREE )
			return *itTasks;
		return NULL;
}

Task* Thread::getCurrentTask() {
    // FIXME: implement
    return this->nextReadyTask();
}

bool Thread::isCompleted() {
    return state == ThreadCompleted;
}

void Thread::addThreadWaiting(Thread *t) {
    threadsWaiting.push_back(t);
}

list<Thread*> Thread::allThreadsWaiting() {
    return threadsWaiting;
}


