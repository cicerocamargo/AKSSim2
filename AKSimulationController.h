#include <list>
using namespace std;

struct AKSimulationParameters;
class AKThread;
class AKThreadProcessor;
class AKThreadScheduler;

class AKSimulationController {
	bool _isInteractive;
	bool _detailedHistory;
	AKThread* _rootThread;
	AKThreadScheduler* _scheduler;
	list<AKThreadProcessor*> _processors;
	list<AKThreadProcessor*>::iterator proc;

	bool endOfProgram();
	void printProcessorsHistory();
	AKSimulationController() {}
public:
	AKSimulationController(AKSimulationParameters parameters);
	~AKSimulationController();
	int runSimulation();

	/* data */
};