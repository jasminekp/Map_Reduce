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
#include <thread>
#include <queue>
#include <mutex>
#include <unordered_map>

/*
 * Header File For Workflow Class
 */


class Workflow
{

public:
	typedef bool(*funcMapperStart)(std::string, std::string fileNumber);
	typedef bool(*funcMapperMap)(std::string key, std::string value);
	typedef void(*funcMapperEnd)(std::string intermediateFileName);

	typedef void(*funcReducerReduce)(std::string word, std::vector<int> occurences, std::string fileName);
	typedef void(*funcReducerStart)(std::string OutputDir, std::string fileName);
	typedef void(*funcReducerEnd)(std::string outputDir, std::string fileName);
	typedef void(*funcReducerSuccess)(std::string OutputDir);

	Workflow(std::string inputDir, std::string outputDir, std::string intermediateDir, std::string mapperDLL, std::string reducerDLL, int reducerProcesses);
	void mapThread(int numberOfReducers, int startingReduceNumber);
	void reducerThread(int reducerNumber);
	void sortData(std::string inputFile, std::unordered_map<std::string, std::vector<int>> &sorterMap, std::string directory);
	void reduceData(std::string inputFile, std::unordered_map<std::string, std::vector<int>> &sorterMap, std::string directory);

	bool MapperJobIsAvailable(std::string &inputFile);
	bool ReducerJobIsAvailable(std::string &inputFile, int reducerNumber);
	bool execute();

	bool LoadMapDLL(HINSTANCE &hMapDLL, funcMapperStart &start, funcMapperMap &map, funcMapperEnd &end);
	bool LoadReduceDLL(HINSTANCE &hReduceDLL, funcReducerStart &start, funcReducerReduce &reduce, funcReducerEnd &end, funcReducerSuccess &success);


private:
	int numberOfReducers;
	std::queue<std::string> mapperQueue;
	std::unordered_map<int, std::queue<std::string>> reducerQueue;
	std::mutex queue_mutex;
	std::string inputDir, outputDir, intermediateDir, mapperDLL, reducerDLL;

	HINSTANCE hMapDLL;
	funcMapperStart mapStart;
	funcMapperMap map;
	funcMapperEnd mapEnd;

	HINSTANCE hReduceDLL;
	funcReducerStart reduceStart;
	funcReducerReduce reduce;
	funcReducerEnd reduceEnd;
	funcReducerSuccess reduceSuccess;
};
