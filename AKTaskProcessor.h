#include "AKProcessor.h"

class AKTaskProcessor : public AKProcessor {
	virtual void getJob();
public:
	virtual void runStep();
	virtual void commitSchedulingPoint();
	AKTaskProcessor();
	~AKTaskProcessor();
};