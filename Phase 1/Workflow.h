#include <string>
#include "Map.h"


/*
 * Header File For Workflow Class
 */


class Workflow
{
public:
	Workflow(std::string inputDir, std::string outputDir, std::string intermediateDir);
	bool execute();

private:
	std::string inputDir, outputDir, intermediateDir;
};
