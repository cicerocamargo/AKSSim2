#include <list>
#include "AKSimulationParameters.h"
using namespace std;

class AKThread;
class AKProcessor;
class AKScheduler;

class AKSimulationController {
	AKSimulationParameters _simulationParameters;
	AKThread* _rootThread;
	AKScheduler* _scheduler;
	list<AKProcessor*> _processors;
	list<AKProcessor*>::iterator proc;

	bool endOfProgram();
	void printProcessorsHistory();
	void runSimulationsAtTaskLevel();
	void runSimulationsAtThreadLevel();
	int runSimulation();
	AKSimulationController() {}
public:
	AKSimulationController(AKSimulationParameters parameters);
	~AKSimulationController();
	void startSimulations();
};