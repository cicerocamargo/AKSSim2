#include "AKThreadProcessor.h"

class AKWorkFirstProcessor : AKThreadProcessor {
protected:
	virtual void getJob();
public:
	AKWorkFirstProcessor();
	virtual void commitSchedulingPoint();
};