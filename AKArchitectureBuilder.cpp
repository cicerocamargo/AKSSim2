#include "AKArchitectureBuilder.h"
#include "AKTaskProcessor.h"
#include "AKWorkFirstProcessor.h"
#include "AKHelpFirstProcessor.h"
#include "Defines.h"

std::list<AKProcessor*> AKArchitectureBuilder::buildHelpFirstProcessors(unsigned n, bool migrationEnabled) {
	std::list<AKProcessor*> list;
	for (int i = 0; i < n; ++i)	{
		list.push_back((AKProcessor*)new AKHelpFirstProcessor(migrationEnabled));
	}
	return list;		
}

std::list<AKProcessor*> AKArchitectureBuilder::buildTaskProcessors(unsigned n) {
	std::list<AKProcessor*> list;
	for (int i = 0; i < n; ++i)	{
		list.push_back(new AKTaskProcessor);
	}
	return list;
}

std::list<AKProcessor*> AKArchitectureBuilder::buildWorkFirstProcessors(unsigned n) {
	std::list<AKProcessor*> list;
	for (int i = 0; i < n; ++i)	{
		list.push_back((AKProcessor*)new AKWorkFirstProcessor);
	}
	return list;
}

std::list<AKProcessor*> AKArchitectureBuilder::buildHelpFirstProcessorsWithMigration(unsigned n) {
	return AKArchitectureBuilder::buildHelpFirstProcessors(n, true);
}

std::list<AKProcessor*> AKArchitectureBuilder::buildHelpFirstProcessorsWithoutMigration(unsigned n) {
	return AKArchitectureBuilder::buildHelpFirstProcessors(n, false);
}