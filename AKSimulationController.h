#include <list>
#include "AKSimulationParameters.h"

class AKThread;
class AKProcessor;
class AKScheduler;
class AKSchedulingUnit;

typedef std::list<AKProcessor*> ListOfProcessors;

class AKSimulationController {
	AKSimulationParameters _params;
	AKThread* _rootThread;
	AKSchedulingUnit* _rootSchedulingUnit;
	AKScheduler* _scheduler;
	ListOfProcessors _processors;
	std::list<AKProcessor*>::iterator proc;

	bool endOfProgram();
	void printProcessorsHistory();
	void runSimulationsAtTaskLevel();
	void runSimulationsAtThreadLevel();
	void runMultithreadedSimulations(const char* architectureName);
	int runSimulation();
	AKSimulationController() {}
public:
	AKSimulationController(AKSimulationParameters parameters);
	~AKSimulationController();
	void startSimulations();
};