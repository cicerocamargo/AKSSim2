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

AKSimulationController::AKSimulationController(AKSimulationParameters params) : _simulationParameters(params) {
	_rootSchedulingUnit = 0; 
	_scheduler = new AKScheduler;	
	_rootThread = new AKThread;
	_rootThread->generateFullyStrictDCG(_simulationParameters.width,
										_simulationParameters.depth,
										_simulationParameters.cost,
										_simulationParameters.isVariableCost);

	_rootThread->calculatePriorityAttributes();

	_rootThread->print();
	if (_simulationParameters.interactiveSimulation) {
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
		if (_simulationParameters.chartType == AKGanttChartTypeExpanded) {
			std::cout << "   Processor " << i << ":\t" << (*proc)->history() << std::endl;
		} else {
			std::cout << "   Processor " << i << ":\t" << (*proc)->activity() << std::endl;
		}
		++i;
	}
	std::cout << "\n";	
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

		if (_simulationParameters.interactiveSimulation) {
			std::system("clear");
			_rootThread->print();
			printProcessorsHistory();
			getchar();
		}
	}
}

void AKSimulationController::runSimulationsAtTaskLevel() {
	_processors = AKArchitectureBuilder::buildTaskProcessors(_simulationParameters.processors);
	_rootSchedulingUnit = _rootThread->firstTask();

	AKTask::setPriorityAttribute(AKTaskPriorityAttributeLevelWithEstimatedTimes);
	_scheduler->prepareForSimulation(_processors, _rootSchedulingUnit);
	std::cout << "\nHLFET: " << runSimulation() << " u. t.\n";
	if (_simulationParameters.chartType != AKGanttChartTypeNone && _simulationParameters.interactiveSimulation == false) {
		printProcessorsHistory();
	}

	AKTask::setPriorityAttribute(AKTaskPriorityAttributeCoLevelWithEstimatedTimes);
	_scheduler->prepareForSimulation(_processors, _rootSchedulingUnit);
	std::cout << "\nSCFET: " << runSimulation() << " u. t.\n";
	if (_simulationParameters.chartType != AKGanttChartTypeNone && _simulationParameters.interactiveSimulation == false) {
		printProcessorsHistory();
	}

	AKTask::setPriorityAttribute(AKTaskPriorityAttributeLevelWithNonEstimatedTimes);
	_scheduler->prepareForSimulation(_processors, _rootSchedulingUnit);
	std::cout << "\nHLFNET: " << runSimulation() << " u. t.\n";
	if (_simulationParameters.chartType != AKGanttChartTypeNone && _simulationParameters.interactiveSimulation == false) {
		printProcessorsHistory();
	}

	AKTask::setPriorityAttribute(AKTaskPriorityAttributeCoLevelWithNonEstimatedTimes);
	_scheduler->prepareForSimulation(_processors, _rootSchedulingUnit);
	std::cout << "\nSCFNET: " << runSimulation() << " u. t.\n";
	if (_simulationParameters.chartType != AKGanttChartTypeNone && _simulationParameters.interactiveSimulation == false) {
		printProcessorsHistory();
	}

	AKTask::setPriorityAttribute(AKTaskPriorityAttributeRandom);
	_scheduler->prepareForSimulation(_processors, _rootSchedulingUnit);
	std::cout << "\nRANDOM: " << runSimulation() << " u. t.\n";
	if (_simulationParameters.chartType != AKGanttChartTypeNone && _simulationParameters.interactiveSimulation == false) {
		printProcessorsHistory();
	}
}

void AKSimulationController::runSimulationsAtThreadLevel() {

}

void AKSimulationController::startSimulations() {
	assert(_rootThread && _rootThread->isRoot());
	runSimulationsAtTaskLevel();
	runSimulationsAtThreadLevel();
}

