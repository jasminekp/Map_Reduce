#include "framework.h"
#include "Reduce.h"
#include <unordered_map>


//Global Variable fileWriter
FileManager fileWriter;

// included the sorterMap
std::unordered_map<std::string, std::vector<int>> sorterMap;

//creating global variables to store everything
std::string intermediateDir;

//creating global variables to store everything
std::string OutputDir;

//creating global variables to store everything
std::string threadNum;


extern "C" REDUCE_API void startThread(std::vector<std::string> directories) {

	intermediateDir = directories[0];
	OutputDir = directories[1];
	threadNum = directories[2];


	createSorter();

	fileWriter.deleteFile(OutputDir + "SUCCESS.txt");

	fileWriter.open(OutputDir + "out" + threadNum +".txt", std::ios::out | std::ios::app);

	consolidate();

	end();
}



//added this to the dll
extern "C" REDUCE_API void createSorter() {

	std::string IblockData;

	fileWriter.open(intermediateDir + "temp.txt", std::ios::in);

	while (fileWriter.getNextBlock(IblockData))
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

	fileWriter.close();
}




//added this to the dll
extern "C" REDUCE_API void consolidate() {

	for (auto pair : sorterMap)
	{
		auto key = pair.first;
		auto value = pair.second;

		reduce(key, value);
	}

}


extern "C" REDUCE_API bool reduce(std::string word, std::vector<int> occurences) {

	int totalOccurences = 0; // variable to store the total amount of time a word was used

	//for loop used to iterate through each element of the vector
	for (auto& timesUsed : occurences) {
		totalOccurences += timesUsed; //each time through the loop, we are recording how many times the word was said
	}

	Export(word, totalOccurences);
	return true;
}




extern "C" REDUCE_API bool Export(std::string key, int reducedValue) {
	std::string stringRep = std::to_string(reducedValue);
	std::string keyPlusOccurences = key + " " + stringRep;

	fileWriter.write(keyPlusOccurences);

	return 0;
}




extern "C" REDUCE_API void end() {
	fileWriter.close();	//closes out.txt

	fileWriter.open(OutputDir + "SUCCESS.txt", std::ios::out | std::ios::app);

	fileWriter.close();	//close success.txt
}






extern "C" REDUCE_API void startMain(std::string outputdir) {
	OutputDir = outputdir;
	
	Map mapper(OutputDir);


	for (const auto& inputfile : std::experimental::filesystem::directory_iterator(OutputDir))
	{

		std::string fileName = inputfile.path().filename().string();

		if (fileName != "SUCCESS.txt")
		{


			fileWriter.open(OutputDir + fileName, std::ios::in);

			std::string blockData;

			while (fileWriter.getNextBlock(blockData))
			{
				mapper.map(inputfile.path().filename().string(), blockData);

			}

			fileWriter.close();
		}

	}

	mapper.end();

	createSorterMain();


	fileWriter.deleteFile(OutputDir + "SUCCESS.txt");

	fileWriter.open(OutputDir + "MainSolution.txt", std::ios::out | std::ios::app);

	consolidate();

	end();


	fileWriter.deleteFile(OutputDir + "MainSolutiontemp.txt");

}


//added this to the dll
extern "C" REDUCE_API void createSorterMain() {

	std::string IblockData;

	fileWriter.open(OutputDir + "MainSolutiontemp.txt", std::ios::in);

	while (fileWriter.getNextBlock(IblockData))
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

	fileWriter.close();
}