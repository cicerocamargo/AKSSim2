#include <iostream>
#include <cassert>

typedef enum {
	AKGanttChartTypeContracted,
	AKGanttChartTypeExpanded,
	AKGanttChartTypeNone
} AKGanttChartType;

class AKSimulationParameters {

public:
	int processors;
	int width;
	int depth;
	int cost;
	bool isVariableCost;
	bool interactiveSimulation;
	AKGanttChartType chartType;

 	static const char* AKGanttChartTypeNames[];
 	static const char* nameFromChartType(AKGanttChartType value);

	AKSimulationParameters();

	void print();

	void validate();
};