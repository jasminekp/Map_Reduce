#include "Reduce.h"

/*
 * Reduce class sums the occurrences of the distinct word into one value and exports the word & occurrence to the output directory
 *
 */


//accept outputdir as parameter
Reduce::Reduce(std::string outputDir)
{
	this->outputDir = outputDir;
}


//reduce method. Iterators through the vector and sums up all of the occurences
bool Reduce::reduce(std::string word, std::vector<int> occurences) {

	int totalOccurences = 0; // variable to store the total amount of time a word was used

	//for loop used to iterate through each element of the vector
	for (auto& timesUsed : occurences) {
		totalOccurences += timesUsed; //each time through the loop, we are recording how many times the word was said
	}

	Export(word, totalOccurences);
	return true;
}



bool Reduce::Export(std::string key, int reducedValue)
{
	std::string stringRep = std::to_string(reducedValue);
	std::string keyPlusOccurences = key + " " + stringRep;

	fileWriter.write(keyPlusOccurences);

	return 0;
}


void Reduce::start()
{
	fileWriter.deleteFile(this->outputDir + "SUCCESS.txt");

	fileWriter.clear(this->outputDir + "out.txt");
	fileWriter.open(this->outputDir + "out.txt", std::ios::out | std::ios::app);


}


void Reduce::end()
{
	fileWriter.close();	//close out.txt

	fileWriter.open(this->outputDir + "SUCCESS.txt", std::ios::out | std::ios::app);

	fileWriter.close();	//close success.txt
}


