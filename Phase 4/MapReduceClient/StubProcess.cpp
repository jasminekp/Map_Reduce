#include "ClientConnection.h"

typedef bool(*funcMapperStart)(std::string, std::string fileNumber);
typedef bool(*funcMapperMap)(std::string key, std::string value);
typedef void(*funcMapperEnd)(std::string intermediateFileName);

typedef void(*funcReducerReduce)(std::string word, std::vector<int> occurences, std::string fileName);
typedef void(*funcReducerStart)(std::string OutputDir, std::string fileName);
typedef void(*funcReducerEnd)(std::string outputDir, std::string fileName);
typedef void(*funcReducerSuccess)(std::string OutputDir);

int numberOfReducers;
std::queue<std::string> mapperQueue;
std::unordered_map<int, std::queue<std::string>> reducerQueue;
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


bool MapperJobIsAvailable(std::string &inputFile)
{

	if (!mapperQueue.empty())
	{
		inputFile = mapperQueue.front();
		mapperQueue.pop();

		return true;
	}
	else
	{
		return false;
	}

}


bool ReducerJobIsAvailable(std::string &inputFile, int reducerNumber)
{
	auto &rt = reducerQueue[reducerNumber];

	//check that there is a reducer queue with the specified reducer number
	if (!rt.empty())
	{
		inputFile = rt.front();
		rt.pop();

		return true;
	}
	else
	{
		return false;
	}
}


void sortData(std::string inputFile, std::unordered_map<std::string, std::vector<int>> &sorterMap, std::string directory)
{

	FileManager Ireader;
	std::string IblockData;

	//sorter phase
	Ireader.open(directory + inputFile, std::ios::in);

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
}


void reduceData(std::string inputFile, std::unordered_map<std::string, std::vector<int>> &sorterMap, std::string directory)
{
	//reducer phase
	reduceStart(directory, inputFile);

	for (auto pair : sorterMap)
	{
		auto key = pair.first;
		auto value = pair.second;

		reduce(key, value, inputFile);
	}

	reduceEnd(directory, inputFile);
}

void mapThread(int numberOfReducers, int startingReducerNumber)
{
	int reducerNumber = (startingReducerNumber % numberOfReducers) + 1;

	std::string inputFile = "";

	while (MapperJobIsAvailable(inputFile))
	{
		std::string intermediateFileName = std::to_string(reducerNumber) + inputFile;

		if (!mapStart(intermediateDir, intermediateFileName))
		{
			continue;
		}

		FileManager reader;
		std::string blockData;

		reader.open(inputDir + inputFile, std::ios::in);

		while (reader.getNextBlock(blockData))
		{
			map(intermediateFileName, blockData);
		}

		reader.close();

		mapEnd(intermediateFileName);

		reducerNumber = (reducerNumber % numberOfReducers) + 1;
	}
}

void reducerThread(int reducerNumber)
{
	std::unordered_map<std::string, std::vector<int>> sorterMap;
	FileManager Ireader;
	std::string IblockData;
	std::string inputFile = "";


	while (ReducerJobIsAvailable(inputFile, reducerNumber))
	{
		//sorter phase
		sortData(inputFile, sorterMap, intermediateDir);


		//reducer phase
		reduceData(inputFile, sorterMap, outputDir);

	}

}


bool LoadReduceDLL(HINSTANCE &hReduceDLL, funcReducerStart &start, funcReducerReduce &reduce, funcReducerEnd &end, funcReducerSuccess &success)
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
		success = (funcReducerSuccess)GetProcAddress(hReduceDLL, "createSuccessFile");


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
		if (success == NULL)
		{
			std::cout << "Did not load reduce 'create success file' correctly." << std::endl;
			return 1;
		}
	}
	else {
		std::cout << "Reduce Library load failed!" << std::endl;
		return 1;
	}

	return 0;

}


bool LoadMapDLL(HINSTANCE &hMapDLL, funcMapperStart &start, funcMapperMap &map, funcMapperEnd &end)
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




int main(int argc, char *argv[])
{

	//---------------------------------------------------------------
	//------------------Create Socket Connection for the Stub Client
	//---------------------------------------------------------------

	ClientConnection MClient;
	MClient.createStubClientSocket();



	//---------------------------------------------------------------
	//------------------Initialize Variables
	//---------------------------------------------------------------
	int countFiles = 0;
	
	inputDir = argv[1];
	intermediateDir = argv[2];
	outputDir = argv[3];
	mapperDLL = argv[4];
	reducerDLL = argv[5];
	numberOfReducers = std::stoi(argv[6]);



	//---------------------------------------------------------------
	//------------------Load the Map DLL
	//---------------------------------------------------------------
	LoadMapDLL(hMapDLL, mapStart, map, mapEnd);


	//---------------------------------------------------------------
	//------------------Create the Mapper Queue WorkerJob
	//---------------------------------------------------------------
	for (const auto & inputfile : std::experimental::filesystem::directory_iterator(inputDir))
	{
		if (inputfile.path().extension().string() == ".txt")
		{
			countFiles++;
			mapperQueue.push(inputfile.path().filename().string());
		}
	}

	//---------------------------------------------------------------
	//------------------Error Handling for Text Files
	//---------------------------------------------------------------
	if (countFiles == 0)
	{
		std::cout << "Error: could not find any text files in the input directory." << std::endl;

		FileManager deleter;
		deleter.deleteFile(outputDir + "SUCCESS.txt");
		return 1;
	}


	//---------------------------------------------------------------
	//------------------Create the Required Number of Mapper Threads
	//---------------------------------------------------------------
	std::vector<std::thread> threads;


	uint32_t num_threads = ceil((double)mapperQueue.size() / numberOfReducers);

	//if number of threads is greater than max allowed by system, just set num threads to max allowed
	num_threads = min(num_threads, std::thread::hardware_concurrency());

	for (uint32_t i = 0; i < num_threads; i++)
	{
		threads.push_back(std::thread(&mapThread, numberOfReducers, i));
	}


	//---------------------------------------------------------------
	//------------------Wait for All Mapper Threads to Finish
	//---------------------------------------------------------------
	for (auto &th : threads)
	{
		th.join();
	}



	////---------------------------------------------------------------
	////------------------Load the Reduce DLL
	////---------------------------------------------------------------
	LoadReduceDLL(hReduceDLL, reduceStart, reduce, reduceEnd, reduceSuccess);


	//---------------------------------------------------------------
	//------------------Create the Reduce Queue WorkerJob
	//---------------------------------------------------------------
	for (const auto & inputfile : std::experimental::filesystem::directory_iterator(intermediateDir))
	{
		if (inputfile.path().extension().string() == ".txt")
		{
			int reducerNumber = stoi(inputfile.path().filename().string().substr(0, 1));

			if (reducerQueue.find(reducerNumber) == reducerQueue.end())
			{
				reducerQueue[reducerNumber] = {};
			}

			reducerQueue[reducerNumber].push(inputfile.path().filename().string());
		}
	}


	//---------------------------------------------------------------
	//------------------Create the Required Number of Reducer Threads
	//---------------------------------------------------------------
	std::vector<std::thread> reduceThreads;

	for (int i = 0; i < numberOfReducers; i++)
	{
		reduceThreads.push_back(std::thread(&reducerThread, i + 1));
	}


	//---------------------------------------------------------------
	//------------------Wait for All Reducer Threads to Finish
	//---------------------------------------------------------------
	for (auto &th : reduceThreads)
	{
		th.join();
	}


	//--------------------------------------------------------------------------
	//------------------Combine All Output Files Into One Sorted/Reduced File
	//--------------------------------------------------------------------------
	std::unordered_map<std::string, std::vector<int>> sorterMap;
	FileManager Ireader;
	std::string IblockData;

	for (const auto & inputfile : std::experimental::filesystem::directory_iterator(outputDir))
	{
		if (inputfile.path().extension().string() == ".txt")
		{
			//sorter phase
			sortData(inputfile.path().filename().string(), sorterMap, outputDir);
		}
	}

	reduceData("COMBINED_OUTPUT.txt", sorterMap, outputDir);


	reduceSuccess(outputDir);


	////---------------------------------------------------------------
	////------------------End Workflow Execution & Unload the DLLs
	////---------------------------------------------------------------
	FreeLibrary(hMapDLL);
	FreeLibrary(hReduceDLL);

	std::cout << "\nMap & Reduce Processes Complete. Check Output Directory." << std::endl;


	return 0;

}