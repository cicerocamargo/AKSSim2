class AKThread;

typedef enum {
	AKSchedulingEventThreadForked,
	AKSchedulingEventThreadJoined,
	AKSchedulingEventThreadCompleted
} AKSchedulingEvent;

class AKSchedulingPoint {
	AKSchedulingPoint() {}
	AKSchedulingPoint(AKThread* relatedThread, AKSchedulingEvent event);
public:
	unsigned _arrivalNumber;
	AKThread* relatedThread;
	AKSchedulingEvent event;

	const char* eventString();
	static AKSchedulingPoint* forkSchedulingPoint(AKThread* forkedThread);
	static AKSchedulingPoint* joinSchedulingPoint(AKThread* joinedThread);
	static AKSchedulingPoint* completionSchedulingPoint(AKThread* completedThread);
};