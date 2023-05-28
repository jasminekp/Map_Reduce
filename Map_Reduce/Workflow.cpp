#include "Workflow.h"
#include <unordered_map>
#include <comdef.h>
#include <thread>



/*
 * The Workflow class determines the flow of the entire Map_Reduce program. 
 *
 */

HINSTANCE hMapDLL;

Workflow::Workflow(std::string inputD, std::string outputD, std::string intermediateD, std::string mapper, std::string reducer)
{
	this->inputDir = inputD;
	this->outputDir = outputD;
	this->intermediateDir = intermediateD;

	this->reducerDLL = reducer;
	this->mapperDLL = mapper;
}


void Workflow::mapperThread(int countFiles)
{

	funcMapperStart mapStart;
	funcMapperMap map;
	funcMapperEnd mapEnd;

	LoadMapDLL(hMapDLL, mapStart, map, mapEnd);

	std::vector<std::thread> threads;
	std::vector<std::string> fileNames;


	for (const auto & inputfile : std::experimental::filesystem::directory_iterator(inputDir))
	{
		if (inputfile.path().extension().string() == ".txt")
		{
			countFiles++;

			threads.push_back(std::thread(mapStart, this->intermediateDir, std::to_string(countFiles)));

			FileManager reader;

			std::string fileName = inputfile.path().filename().string();

			fileNames.push_back(fileName);

			reader.open(this->inputDir + fileName, std::ios::in);

			std::string blockData;

			while (reader.getNextBlock(blockData))
			{

				map(inputfile.path().filename().string(), blockData);

			}
			reader.close();

		}
	}

	for (auto &th : threads) {
		th.join();
	}

	mapEnd();


}


bool Workflow::execute() 
{
	std::cout << "\n...Please wait..." << std::endl;
	int countFiles = 0;

	mapperThread(countFiles);

	
	if (&countFiles == 0)
	{
		std::cout << "Error: could not find any text files in the input directory." << std::endl;

		FileManager deleter;
		deleter.deleteFile(this->outputDir + "SUCCESS.txt");
		return 1;
	}


	//---------------------------------------------------------------
	//-----------------Sorter Phase
	//---------------------------------------------------------------

	std::unordered_map<std::string, std::vector<int>> sorterMap;
	FileManager Ireader;
	std::string IblockData;

	Ireader.open(this->intermediateDir + "temp.txt", std::ios::in);

	while (Ireader.getNextBlock(IblockData))
	{
		size_t pos = IblockData.find(':');						// Find position of the delimiter ':'
		std::string key = IblockData.substr(0, pos);			// Extract first token before delimiter
		int value = std::stoi(IblockData.substr(pos + 1));

		if (sorterMap.find(key) != sorterMap.end())
		{
			sorterMap[key].push_back(value);
		}
		else
		{
			sorterMap[key] = { value };
		}
	}

	Ireader.close();



	//---------------------------------------------------------------
	//------------------Load the Reduce DLL
	//---------------------------------------------------------------

	HINSTANCE hReduceDLL;
	funcReducerStart reduceStart;
	funcReducerReduce reduce;
	funcReducerEnd reduceEnd;

	LoadReduceDLL(hReduceDLL, reduceStart, reduce, reduceEnd);



	//---------------------------------------------------------------
	//----------------Reducer phase
	//---------------------------------------------------------------

	reduceStart(this->outputDir);

	for (auto pair : sorterMap)
	{
		auto key = pair.first;
		auto value = pair.second;
	
		reduce(key, value);
	}
	
	reduceEnd(this->outputDir);

	std::cout << "\nDone! Check Output Directory." << std::endl;


	//---------------------------------------------------------------
	//------------------End Workflow Execution & Unload the DLLs
	//---------------------------------------------------------------
	FreeLibrary(hMapDLL);
	FreeLibrary(hReduceDLL);

	return 0;


}


bool Workflow::LoadReduceDLL(HINSTANCE &hReduceDLL, funcReducerStart &start, funcReducerReduce &reduce, funcReducerEnd &end)
{
	std::string dllName = reducerDLL + "Reduce";


	std::wstring wide_string = std::wstring(dllName.begin(), dllName.end());
	const wchar_t *libName = wide_string.c_str();


	//integrate and handle the DLL
	hReduceDLL = LoadLibraryEx(libName, NULL, NULL);   


	if (hReduceDLL != NULL)
	{
		start = (funcReducerStart)GetProcAddress(hReduceDLL, "start");
		reduce = (funcReducerReduce)GetProcAddress(hReduceDLL, "reduce");
		end = (funcReducerEnd)GetProcAddress(hReduceDLL, "end");


		if (start == NULL)
		{
			std::cout << "Did not load reduce start correctly." << std::endl;
			return 1;
		}
		if (reduce == NULL)
		{
			std::cout << "Did not load reduce correctly." << std::endl;
			return 1;
		}
		if (end == NULL)
		{
			std::cout << "Did not load reduce end correctly." << std::endl;
			return 1;
		}
	}
	else {
		std::cout << "Reduce Library load failed!" << std::endl;
		return 1;
	}

	return 0;

}


bool Workflow::LoadMapDLL(HINSTANCE &hMapDLL, funcMapperStart &start, funcMapperMap &map, funcMapperEnd &end)
{
	std::string dllName = mapperDLL + "Map";


	std::wstring wide_string = std::wstring(dllName.begin(), dllName.end());
	const wchar_t *libName = wide_string.c_str();


	//integrate and handle the DLL
	hMapDLL = LoadLibraryEx(libName, NULL, NULL);  

	if (hMapDLL != NULL)
	{
		start = (funcMapperStart)GetProcAddress(hMapDLL, "start");
		map = (funcMapperMap)GetProcAddress(hMapDLL, "map");
		end = (funcMapperEnd)GetProcAddress(hMapDLL, "end");


		if (start == NULL)
		{
			std::cout << "Did not load map start correctly." << std::endl;
			return 1;
		}
		if (map == NULL)
		{
			std::cout << "Did not load map correctly." << std::endl;
			return 1;
		}
		if (end == NULL)
		{
			std::cout << "Did not load map end correctly." << std::endl;
			return 1;
		}
	}
	else {
		std::cout << "Map Library load failed!" << std::endl;
		return 1;
	}


	return 0;
}

