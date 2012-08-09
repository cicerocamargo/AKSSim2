//
//  Task.cpp
//  AKSim
//
//  Created by Cícero Camargo on 09/08/12.
//  Copyright (c) 2012 Cícero Camargo. All rights reserved.
//


#include "task.h"
#include "thread.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int Task::counter = 0;
// class constructor
Task::Task(int id, int cost, Thread* t, Thread* c, Thread* j){
	++counter;
	this->id = id;
	this->cost = cost;
	thread = t;
	if(c) created.push_back(c);
	if(j) joined.push_back(j);

	tLevel = 0;
	bLevel = 0;
	state = DEFAULT;
	critical = false;
}

// methods to set up suc and pred
void Task::setSuccessorInThread(){
	list<Task*>::iterator it;
	for(it = successors.begin(); it != successors.end() ; it++)
		if ( ((*it)->getThread())->getId() == thread->getId() ){
			suc = *it;
			return;
		}
	suc = NULL;
}

void Task::setPredecessorInThread(){
	list<Task*>::iterator it;
	for(it = predecessors.begin(); it != predecessors.end() ; it++)
		if ( ((*it)->getThread())->getId() == thread->getId() ){
			pred = *it;
			return;
		}
	pred = NULL;
}


/*
** return true if this task is INPUT of its thread
** otherwise, return false.
*/
bool Task::isInputOfThread(){
	if( pred == NULL )
		setPredecessorInThread();
	if( pred == NULL )
		return true;
	return false;
}

/*
** return true if this task is OUTPUT of its thread
** otherwise, return false.
*/
bool Task::isOutputOfThread(){
	if( suc == NULL )
		setSuccessorInThread();
	if( suc == NULL )
		return true;
	return false;
}

/*
** Display task info
*/
void Task::display(){
	list<Task*>::iterator it;
	
	// node info
	cout << "task: " << id << "  (c = " << cost << ")";
	if( isInputTask() ) cout << "  Input";
	if( isOutputTask() ) cout << "  Output";
	if( tLevel ) cout << "  t-level = " << tLevel;
	if( bLevel ) cout << "  b-level = " << bLevel;
	if( critical ) cout << "   CRITICAL!";
	// if( pred ) cout << endl << "\t\t pred: " << (pred->getThread())->getId() << ":" << pred->getId();
	// if( suc ) cout << endl << "\t\t suc: " << (suc->getThread())->getId() << ":" << suc->getId();
	cout << endl;
	

	// related dependences info	
	if( !isInputTask() ){
		cout << "\t\tpredecessors: ";
		for( it = predecessors.begin() ; it != predecessors.end() ; it++ ){
			cout << "  ";
			if((*it)->getThread())
				cout << (*it)->getThread()->getId() << ":";
			cout <<	(*it)->getId();
		}
		cout << endl;
	}

	if( !isOutputTask() ){
		cout << "\t\tsuccessors: ";
		for( it = successors.begin() ; it != successors.end() ; it++ ){
			cout << "  ";
			if((*it)->getThread())
				cout << (*it)->getThread()->getId() << ":";
			cout << (*it)->getId();
		}
		cout << endl;
	}
	if( isCreate() ){
		cout << "\t\tcreates threads ";
		for(list<Thread*>::iterator it = created.begin(); it!=created.end(); it++)
			cout << "  " << (*it)->getId();
		cout << endl;
	}
	if( isJoin() ){
		cout << "\t\tjoins threads ";
		for(list<Thread*>::iterator it = joined.begin(); it!=joined.end(); it++)
			cout << "  " << (*it)->getId();
		cout << endl;
	}
}

/*
** Fix the IDs resulting of gxl2Thread so that
** each thread has task 1 to n.
*/
void Task::fixId(){
	if( isInputOfThread() )
		id = 1;
	else
		id = pred->getId() + 1;
	if( suc )
		suc->fixId();
}

// setSuccessorInThread() and void Task::setPredecessorInThread()
// must have been executed before!!
void Task::setThreadsFromTasks(){
	list<Task*>::iterator it;
	
	if( successors.size() > 1 && suc ){ // is a create
		for(it = successors.begin(); it != successors.end(); ++it)
			if( (*it)->getId() != suc->getId() )
				addCreated( (*it)->getThread() );	
	}
	if( predecessors.size() > 1 && pred ){ // PRED is a create
		for(it = predecessors.begin(); it != predecessors.end(); ++it)
			if( (*it)->getId() != pred->getId() )
				pred->addJoined( (*it)->getThread() );	
	}
}

void Task::setTLevel(){
	list<Task*>::iterator it;
	
	if ( this->isInputTask() )
		tLevel = cost;
	else {
		int max = 0;
		for(it=predecessors.begin() ; it!=predecessors.end() ; it++)
			if( (*it)->getTLevel() > max )
				max = (*it)->getTLevel();
		tLevel = cost + max;
	}
	if ( !this->isOutputTask() )
		for(it=successors.begin() ; it!=successors.end() ; it++)
			(*it)->setTLevel();
}

void Task::setBLevel(){
	list<Task*>::iterator it;
	
	if ( this->isOutputTask() )
		bLevel = cost;
	else {
		int max = 0;
		for(it=successors.begin() ; it!=successors.end() ; it++)
			if( (*it)->getBLevel() > max )
				max = (*it)->getBLevel();
		bLevel = cost + max;
	} 
	if ( !this->isInputTask() )
		for(it=predecessors.begin() ; it!=predecessors.end() ; it++)
			(*it)->setBLevel();
}

/*
** Modifies task's state during the scheduling
*/
void Task::setState(taskState s){
	state = s;
	if( s == EXECUTED && thread && isOutputOfThread() ) {
		thread->setState( ThreadCompleted );
    }
}

/****** For DCG scheduling only!! *****/
void Task::updateSuccessors(){
	// cout << "Atualizando sucessores da tarefa " << getId() << " do thread " << thread->getId() << "..." <<endl;
	// cout << "\tEsta tarefa termina criando os threads ";
	// for(list<Thread*>::iterator it = created.begin(); it!=created.end(); it++)
	// 	cout << "  " << (*it)->getId();
	// cout << endl;
	// cout << "\te fazendo join nos threads ";
	// for(list<Thread*>::iterator it = joined.begin(); it!=joined.end(); it++)
	// 	cout << "  " << (*it)->getId();
	// cout << endl;
	
	if( isCreate() ){
		for(list<Task*>::iterator it = successors.begin(); it != successors.end(); it++)
			(*it)->setState( READY );
		for(list<Thread*>::iterator _it = created.begin(); _it != created.end(); _it++)
			(*_it)->setState( ThreadReady );
	}
	if( isJoin() ){
		if(!suc){
			cerr << "ERROR: Task " << getId() << " is a Join and has no successor in thread "<< thread->getId() << endl;
			exit( EXIT_FAILURE );
		}
		bool block = false;
		//cout <<  "\tseu sucessor no thread é a tarefa " << suc->getId() << endl;
		for(list<Thread*>::iterator it = joined.begin(); it != joined.end(); it++)
			if( (*it)->getState() != ThreadCompleted )
				block = true;
		if( block ){
			//cout << "entrou aqui! Deve bloquear thread " << thread->getId() << endl;
			thread->setState( ThreadBlocked );
			thread->setBlockingTask( this );
		}
		else
			suc->setState( READY );		
	}
	else if( suc ) suc->setState( READY );
}

/*
** check if all predecessors are EXECUTED
*/
void Task::tryGettingReady(){
	list<Task*>::iterator it;
	for(it = predecessors.begin(); it != predecessors.end(); ++it)
		if ( (*it)->getState() != EXECUTED )
			return;
	state = FREE;
}

Thread* Task::getAssociatedThread() {
    
    // TODO: refactoring
    
    if (this->isCreate()) {
        return created.front();
    }
    else if (this->isJoin()) {
        return joined.front();
    }
    else {
        return NULL;
    }
}

void Task::step() {
    // TODO: IMPLEMENT
}

bool Task::isCompleted() {
    return state == EXECUTED;
}
