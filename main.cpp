#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "AKSimulationParameters.h"
#include "AKSimulationController.h"

AKSimulationParameters parseParameters(int argc, char **argv);

int main(int argc, char** argv) {
	srand(time(NULL));

	AKSimulationParameters params = parseParameters(argc, argv);
	params.validate();
	
	AKSimulationController simulationController(params);
	simulationController.startSimulations();

	return 0;
}

void usage() {
	printf("print usage\n");
	exit(1);
}

AKSimulationParameters parseParameters(int argc, char **argv) {
	int c;
	AKSimulationParameters params;
	opterr = 0;

	while ( (c = getopt (argc, argv, "p:w:d:c:ivh")) != -1) {
		switch (c) {
			case 'c':
				params.cost = atoi(optarg);
				break;
			case 'p':
				params.processors = atoi(optarg);
				break;
			case 'w':
				params.width = atoi(optarg);
				break;
			case 'd':
				params.depth = atoi(optarg);
				break;
			case 'v':
				params.isVariableCost = true;
				break;
			case 'h':
				params.detailedHistory = true;
				break;
			case 'i':
				params.interactiveSimulation = true;
				break;
			case '?':
				if (optopt == 'p' || optopt == 'c' || optopt == 'w' || optopt == 'd'){
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				} else if (isprint(optopt)) {
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);	
				} else {
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				}
				usage();
			default:
				abort ();
		}
	}
#ifdef DEBUG
	params.print();
#endif
	return params;
}
