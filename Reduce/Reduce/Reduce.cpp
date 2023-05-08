#include "framework.h"
#include "Reduce.h"

//Global Variable fileWriter
FileManager fileWriter;

REDUCE_API bool reduce(std::string word, std::vector<int> occurences) {

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


extern "C" REDUCE_API void start(std::string OutputDir) {
	fileWriter.deleteFile(OutputDir + "SUCCESS.txt");

	fileWriter.clear(OutputDir + "out.txt");
	fileWriter.open(OutputDir + "out.txt", std::ios::out | std::ios::app);
}


extern "C" REDUCE_API void end(std::string OutputDir) {
	fileWriter.close();	//closes out.txt

	fileWriter.open(OutputDir + "SUCCESS.txt", std::ios::out | std::ios::app);

	fileWriter.close();	//close success.txt
}