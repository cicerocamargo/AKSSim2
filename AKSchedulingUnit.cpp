#include "AKSchedulingUnit.h"

const char* AKSchedulingUnit::AKSchedulingUnitStateNames[] = {
	"Default",
	"Ready",
	"Running",
	"Blocked",
	"Finished"
};

AKSchedulingUnit::AKSchedulingUnit() {
	int _id = -1;
	_state = AKSchedulingUnitStateDefault;
}

const char* AKSchedulingUnit::getNameFromState(AKSchedulingUnitState state) {
	return AKSchedulingUnitStateNames[state];
}