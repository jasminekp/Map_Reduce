#undef UNICODE

#define WIN32_LEAN_AND_MEAN

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
		Workflow(std::string inputDir, std::string outputDir, std::string intermediateDir, std::string mapperDLL,
					std::string reducerDLL, std::string reducerProcesses, std::string stubClientDir);

		int createControllerServerSocket();
		void callStubClient();

		bool execute();


	private:
		std::string numberOfReducers;
		std::string inputDir, outputDir, intermediateDir, mapperDLL, reducerDLL, stubClient;

};
