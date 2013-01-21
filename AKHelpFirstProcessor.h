#include "AKThreadProcessor.h"
#include <stack>

class AKHelpFirstProcessor : AKThreadProcessor {
	std::stack<AKThread*> _blockedThreads;
	bool _migrationEnabled;
protected:
	virtual void getJob();
public:
	AKHelpFirstProcessor(bool migrationEnabled=true);
	virtual void commitSchedulingPoint();

	/* data */
};