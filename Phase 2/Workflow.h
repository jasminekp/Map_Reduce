#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <regex>
#include <windows.h>
#include "FileManager.h"


/*
 * Header File For Workflow Class
 */


class Workflow
{

public:
	typedef void(*funcMapperStart)(std::string);
	typedef void(*funcMapperMap)(std::string key, std::string value);
	typedef void(*funcMapperEnd)();

	typedef void(*funcReducerReduce)(std::string word, std::vector<int> occurences);
	typedef void(*funcReducerStart)(std::string);
	typedef void(*funcReducerEnd)(std::string outputDir);

	Workflow(std::string inputDir, std::string outputDir, std::string intermediateDir, std::string mapperDLL, std::string reducerDLL);
	bool execute();

	bool LoadMapDLL(HINSTANCE &hMapDLL, funcMapperStart &start, funcMapperMap &map, funcMapperEnd &end);
	bool LoadReduceDLL(HINSTANCE &hReduceDLL, funcReducerStart &start, funcReducerReduce &reduce, funcReducerEnd &end);

private:
	
	std::string inputDir, outputDir, intermediateDir, mapperDLL, reducerDLL;

};
