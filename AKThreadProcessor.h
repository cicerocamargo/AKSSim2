#include "AKProcessor.h"

class AKThread;

class AKThreadProcessor : public AKProcessor {
protected:
	virtual void getJob() = 0;
public:
	virtual void runStep();
	AKThread* currentThread();
	void setCurrentThread(AKThread* thread);
};