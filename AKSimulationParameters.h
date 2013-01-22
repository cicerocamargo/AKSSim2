#ifndef AKSIMULATIONPARAMETERS_H
#define AKSIMULATIONPARAMETERS_H

#include <iostream>
#include <cassert>

struct AKSimulationParameters {

	int processors;
	int width;
	int depth;
	int cost;
	bool isVariableCost;
	bool interactiveSimulation;
	bool detailedHistory;

	AKSimulationParameters() {
		processors = 1;
		width = 1;
		depth = 0;
		cost = 1;
		isVariableCost = false;
		interactiveSimulation = false;
		detailedHistory = false;
	}

	void print() {
		std::cout << std::endl << processors << " processors";
		std::cout << "\nwidth = " << width;
		std::cout << "\ndepth = " << depth;
		std::cout << "\naverage task cost = " << cost;
		std::cout << "\nvariable cost? " << (isVariableCost ? "Yes" : "No");
		std::cout << "\ninteractive? " << (interactiveSimulation ? "Yes" : "No");
		std::cout << "\nshow detailed history? " << (detailedHistory ? "Yes" : "No");
		std::cout << std::endl << std::endl;
	}

	void validate() {
		assert(width > 0);
		assert(processors > 0);
		assert(cost > 0);
	}
};

#endif