#pragma once

#include "framework.h"
#include "Reduce.h"



REDUCE_API bool reduce(std::string word, std::vector<int> occurences, std::string fileName) {

	int totalOccurences = 0; // variable to store the total amount of time a word was used


	//for loop used to iterate through each element of the vector
	for (auto timesUsed : occurences)
	{
		totalOccurences += timesUsed; //each time through the loop, we are recording how many times the word was said
	}

	Export(word, totalOccurences, fileName);
	return true;
}




extern "C" REDUCE_API bool Export(std::string key, int reducedValue, std::string fileName) {
	
	std::string stringRep = std::to_string(reducedValue);
	std::string keyPlusOccurences = key + ":" + stringRep;

	auto &fileWriter = fileWriters[fileName];

	fileWriter.write(keyPlusOccurences);

	return 0;
}


extern "C" REDUCE_API void start(std::string OutputDir, std::string fileName) 
{
	fileWriters[fileName] = FileManager();

	auto &fileWriter = fileWriters[fileName];
	std::string fileNumber = fileName.substr(0, 1);

	fileWriter.open(OutputDir + "out_" + fileName, std::ios::out | std::ios::app);

}


extern "C" REDUCE_API void end(std::string OutputDir, std::string fileName) 
{
	auto &fileWriter = fileWriters[fileName];

	fileWriter.close();	
}



extern "C" REDUCE_API void createSuccessFile(std::string OutputDir)
{
	 FileManager fileWriter;

	//then create success file
	fileWriter.open(OutputDir + "SUCCESS.txt", std::ios::out | std::ios::app);

	fileWriter.close();	
}
