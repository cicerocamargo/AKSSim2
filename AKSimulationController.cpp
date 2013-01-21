#include "AKSimulationParameters.h"
#include "AKSimulationController.h"
#include "AKThread.h"
//#include "AKThreadProcessor.h"
#include "AKHelpFirstProcessor.h"
//#include "AKWorkFirstProcessor.h"
#include "AKThreadScheduler.h"
#include <iostream>
#include <cstdio>
#include <cassert>

AKSimulationController::AKSimulationController(AKSimulationParameters params) {
	_isInteractive = params.interactiveSimulation;
	_detailedHistory = params.detailedHistory;
	_scheduler = new AKThreadScheduler;
	for (int i = 0; i < params.processors; ++i) {
		_scheduler->addProcessor((AKThreadProcessor*)new AKHelpFirstProcessor(false));
	}
	_processors = _scheduler->processors();
	_rootThread = new AKThread;
	_rootThread->generateFullyStrictDCG(params.width, params.depth, params.cost, params.isVariableCost);
	_scheduler->setRootThread(_rootThread);
	_rootThread->print();
	if (_isInteractive) {
		getchar();
	}
}

AKSimulationController::~AKSimulationController() {}

bool AKSimulationController::endOfProgram() {
	for (proc = _processors.begin(); proc != _processors.end(); proc++) {
		if (!(*proc)->isIdle()) {
			return false;
		}
	}	
	return true;
}

void AKSimulationController::printProcessorsHistory() {
	std::cout << "History:\n";
	int i = 0;
	for (proc = _processors.begin(); proc != _processors.end(); proc++, i++) {
		if (_detailedHistory) {
			std::cout << "  Processor " << i << ":\t"
					  << (_isInteractive ? (*proc)->history() : (*proc)->finalHistory()) << std::endl;
		} else {
			std::cout << "  Processor " << i << ":\t" << (*proc)->activity() << std::endl;
		}
	}	
}

int AKSimulationController::runSimulation() {
	int makespan = 0;

	while (true) {

		for (proc = _processors.begin(); proc != _processors.end(); proc++) {
			(*proc)->runStep();
		}

		if ( endOfProgram() ) {
			if (_rootThread->validate() == false) {
				std::cerr << "Error: some tasks weren't executed!\n";
				return -1;
			} else {
				if (!_isInteractive) { printProcessorsHistory(); }
				return makespan;
			}
		}
		makespan++;

		for (proc = _processors.begin(); proc != _processors.end(); proc++) {
			(*proc)->commitSchedulingPoint();
		}

		if (_isInteractive) {
			std::system("clear");
			_rootThread->print();
			printProcessorsHistory();
			getchar();
		}
	}
}