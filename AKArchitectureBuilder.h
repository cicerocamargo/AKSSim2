#include <list>

class AKProcessor;

class AKArchitectureBuilder {
	AKArchitectureBuilder() {}
	~AKArchitectureBuilder() {}
	static std::list<AKProcessor*> buildHelpFirstProcessors(unsigned n, bool migrationEnabled);
public:
	static std::list<AKProcessor*> buildTaskProcessors(unsigned n);
	static std::list<AKProcessor*> buildWorkFirstProcessors(unsigned n);
	static std::list<AKProcessor*> buildHelpFirstProcessorsWithMigration(unsigned n);
	static std::list<AKProcessor*> buildHelpFirstProcessorsWithoutMigration(unsigned n);
};