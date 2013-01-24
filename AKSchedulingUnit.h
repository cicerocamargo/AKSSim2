#ifndef AKSCHEDULINGUNIT_H
#define AKSCHEDULINGUNIT_H

typedef enum {
	AKSchedulingUnitStateDefault,
	AKSchedulingUnitStateReady,
	AKSchedulingUnitStateRunning,
	AKSchedulingUnitStateBlocked,
	AKSchedulingUnitStateFinished
} AKSchedulingUnitState;

class AKSchedulingPoint;

class AKSchedulingUnit {
protected:
	unsigned _arrivalNumber;
	int _id;
	AKSchedulingUnitState _state;
	bool _sentToScheduler;
	static const char* AKSchedulingUnitStateNames[];
public:
	AKSchedulingUnit();

	static const char* getNameFromState(AKSchedulingUnitState state);

	int id() { return _id; }
	bool sentToScheduler() { return _sentToScheduler; }
	void setSentToScheduler() { _sentToScheduler = true; }
	AKSchedulingUnitState state() { return _state; }
	unsigned arrivalNumber() { return _arrivalNumber; }
	void setArrivalNumber(unsigned number) { _arrivalNumber = number; }
	
	void setState(AKSchedulingUnitState state) { _state = state; }
	virtual int priority() = 0;
	virtual bool isRoot() = 0;
	virtual bool validateSchedule() = 0;
	virtual AKSchedulingPoint* runStep() = 0;
	virtual void prepareForSimulation() = 0;
};

#endif