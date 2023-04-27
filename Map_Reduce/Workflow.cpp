#include "Workflow.h"
#include <unordered_map>
#include "Reduce.h"


/*
 * The Workflow class determines the flow of the entire Map_Reduce program. This class checks the 
 *
 */

Workflow::Workflow(std::string inputD, std::string outputD, std::string intermediateD)
{
	this->inputDir = inputD;
	this->outputDir = outputD;
	this->intermediateDir = intermediateD;
}


bool Workflow::execute() 
{

	std::cout << "\n...Please wait..." << std::endl;
	int countFiles = 0;

	//---------------------------------------------------------------
	//------------------Mapper phase
	//---------------------------------------------------------------


	Map mapper(this->intermediateDir);
	mapper.start();

	for (const auto & inputfile : std::experimental::filesystem::directory_iterator(inputDir))
	{
		if (inputfile.path().extension().string() == ".txt")
		{
			countFiles++;
			FileManager reader;

			std::string fileName = inputfile.path().filename().string();

		  
			reader.open(this->inputDir + fileName, std::ios::in);

			std::string blockData;

			while (reader.getNextBlock(blockData))
			{
				mapper.map(inputfile.path().filename().string(), blockData);
				
			}

			reader.close();
			
		}

	}

	mapper.end();


	if (countFiles == 0)
	{
		std::cout << "Error: could not find any text files in the input directory." << std::endl;

		FileManager deleter;
		deleter.deleteFile(this->outputDir + "SUCCESS.txt");
		return 1;
	}


	//---------------------------------------------------------------
	//-----------------sorter phase
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
	//----------------Reducer phase
	//---------------------------------------------------------------

	Reduce reducer(this->outputDir);
	reducer.start();

	for (auto pair : sorterMap)
	{
		auto key = pair.first;
		auto value = pair.second;

		reducer.reduce(key, value);
	}

	reducer.end();

	std::cout << "\nDone! Check Output Directory." << std::endl;

	return 0;
}

