#include "AKSimulationController.h"
#include "AKThread.h"
#include "AKMTTask.h"
#include "AKProcessor.h"
#include "AKArchitectureBuilder.h"
#include "AKScheduler.h"
#include "Defines.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstdlib>

AKSimulationController::AKSimulationController(AKSimulationParameters params) : _params(params) {
	_rootSchedulingUnit = 0; 
	_scheduler = new AKScheduler;	
	_rootThread = new AKThread;
	_rootThread->generateFullyStrictDCG(_params.width,
										_params.depth,
										_params.cost,
										_params.isVariableCost);

	_rootThread->calculatePriorityAttributes();

	_rootThread->print();
	if (_params.interactiveSimulation) {
		getchar();
	}
}

AKSimulationController::~AKSimulationController() {}

bool AKSimulationController::endOfProgram() {
	FOR_EACH (proc, _processors) {
		if ( (*proc)->isBusy() ) {
			return false;
		}
	}	
	return true;
}

void AKSimulationController::printProcessorsHistory() {
	std::cout << "\n GanntChart:\n";
	int i = 0;
	FOR_EACH (proc, _processors) {
		char str[5];
		sprintf(str, "%02d", i);
		if (_params.chartType == AKGanttChartTypeExpanded) {
			std::cout << "   Processor " << str << ": " << (*proc)->history() << std::endl;
		} else {
			std::cout << "   Processor " << str << ": " << (*proc)->activity() << std::endl;
		}
		++i;
	}
	std::cout << "\n\n";	
}

int AKSimulationController::runSimulation() {
	int makespan = 0;

	FOR_EVER {

		FOR_EACH (proc, _processors) {
			(*proc)->runStep();
		}

		if ( endOfProgram() ) {
			if (_rootSchedulingUnit->validateSchedule() == false) {
				std::cerr << "Error: some tasks weren't executed!\n";
				return -1;
			} else {
				return makespan;
			}
		}
		makespan++;

		FOR_EACH (proc, _processors) {
			(*proc)->commitSchedulingPoint();
		}

		if (_params.interactiveSimulation) {
			int rc = std::system("clear");
			_rootThread->print();
			printProcessorsHistory();
			getchar();
		}
	}
}

void AKSimulationController::runSimulationsAtTaskLevel() {
	std::cout << "\nCurrent architecture: Basic Task  Processors\n\n";

	_processors = AKArchitectureBuilder::buildTaskProcessors(_params.processors);
	_rootSchedulingUnit = _rootThread->firstTask();
	std::list<AKTaskPriorityAttribute> attributes = AKTask::listWithPrioriyAttributes();
	std::list<AKTaskPriorityAttribute>::iterator attr;

	FOR_EACH(attr, attributes) {
		AKTask::setPriorityAttribute(*attr);
		_scheduler->prepareForSimulation(_processors, _rootSchedulingUnit);
		std::cout << AKTask::algorithmNameFromPriorityAttribute(*attr)
				  << ": " << runSimulation() << " u. t.\n";
		if (_params.chartType != AKGanttChartTypeNone && _params.interactiveSimulation == false) {
			printProcessorsHistory();
		}	
	}
}

void AKSimulationController::runSimulationsAtThreadLevel() {
	_rootSchedulingUnit = _rootThread;
	
	_processors = AKArchitectureBuilder::buildWorkFirstProcessors(_params.processors);
	runMultithreadedSimulations("Work First Processors");
	_processors = AKArchitectureBuilder::buildHelpFirstProcessorsWithMigration(_params.processors);
	runMultithreadedSimulations("Help First Processors (migration enabled)");
	_processors = AKArchitectureBuilder::buildHelpFirstProcessorsWithoutMigration(_params.processors);
	runMultithreadedSimulations("Help First Processors (no migration)");
}

void AKSimulationController::startSimulations() {
	assert(_rootThread && _rootThread->isRoot());
	runSimulationsAtTaskLevel();
	runSimulationsAtThreadLevel();
}

void AKSimulationController::runMultithreadedSimulations(const char* architectureName) {
	std::cout << "\nCurrent architecture: " << architectureName << "\n\n";

	std::list<AKThreadPriorityAttribute> attributes = AKThread::listWithPrioriyAttributes();
	std::list<AKThreadPriorityAttribute>::iterator attr;

	FOR_EACH(attr, attributes) {
		AKThread::setPriorityAttribute(*attr);
		_scheduler->prepareForSimulation(_processors, _rootSchedulingUnit);
		std::cout << AKThread::algorithmNameFromPriorityAttribute(*attr)
				  << ": " << runSimulation() << " u. t.\n";
		if (_params.chartType != AKGanttChartTypeNone && _params.interactiveSimulation == false) {
			printProcessorsHistory();
		}	
	}	

}
