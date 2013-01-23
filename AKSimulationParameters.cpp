#include "AKSimulationParameters.h"

const char* AKSimulationParameters::AKGanttChartTypeNames[] = {
	"Contracted",
	"Expanded",
	"None"
};

const char* AKSimulationParameters::nameFromChartType(AKGanttChartType value) {
	return AKGanttChartTypeNames[value];
}

AKSimulationParameters::AKSimulationParameters() {
	processors = 1;
	width = 1;
	depth = 0;
	cost = 1;
	isVariableCost = false;
	interactiveSimulation = false;
	chartType = AKGanttChartTypeNone;
}

void AKSimulationParameters::print() {
	std::cout << std::endl << processors << " processors";
	std::cout << "\nwidth = " << width;
	std::cout << "\ndepth = " << depth;
	std::cout << "\naverage task cost = " << cost;
	std::cout << "\nvariable cost? " << (isVariableCost ? "Yes" : "No");
	std::cout << "\nstep by step? " << (interactiveSimulation ? "Yes" : "No");
	std::cout << "\nchart type: " << AKSimulationParameters::nameFromChartType(chartType);
	std::cout << std::endl << std::endl;
}

void AKSimulationParameters::validate() {
	assert(width > 0);
	assert(processors > 0);
	assert(cost > 0);
}