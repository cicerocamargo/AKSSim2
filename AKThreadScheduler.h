#include <list>
class AKThread;
class AKThreadProcessor;

class AKThreadScheduler {
	std::list<AKThread*> _readyThreads;
	std::list<AKThreadProcessor*> _processors;
public:
	AKThreadScheduler();
	~AKThreadScheduler();
	AKThread* getJob();
	std::list<AKThreadProcessor*> processors() { return _processors; }
	void putJob(AKThread* thread);
	void setRootThread(AKThread* rootThread);
	void addProcessor(AKThreadProcessor* processor);
};