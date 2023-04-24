#include "Map.h"

/*
 * The Map class takes the raw data line from the input file and tokenizes the 
 * raw data line into distinct words. This class then buffers the distinct words into an
 * intermediate file.
 *
 */


/**
 * Checks if the buffer size is greater than the threshold and writes to the intermediate file
 */
bool Map::Export(std::string distinctWord, int value, std::string fileName, bool eof)
{

	if (eof)
	{
		std::string stringBufferData;
		stringBufferData += distinctWord + ":" + std::to_string(value);

		fm->write(intermediateDirectory + fileName, stringBufferData, std::ios::app);

		if (!buffer.empty())
		{
			std::string stringBufferData;
			for (auto data : buffer)
			{
				stringBufferData += data.first + ":" + std::to_string(data.second) + "\n";
			}

			fm->write(intermediateDirectory + fileName, stringBufferData, std::ios::app);
			buffer.clear();
		}
	}
	else if (buffer.size() >= Threshold)
	{
		std::string stringBufferData;
		std::string newLineChar = "\n";


		for (auto data : buffer)
		{
			stringBufferData += data.first + ":" + std::to_string(data.second) + newLineChar;

		}
		
		fm->write(intermediateDirectory + fileName, stringBufferData, std::ios::app);
		buffer.clear();
	}
	else
	{
		buffer.push_back(make_pair(distinctWord, 1));
	}

	return 0;
}


/**
 * Takes in the file name and the raw data string
 */
bool Map::map(std::string fileName, std::string value, bool eof)
{
	// tokenize the string into distinct words and remove punctuation, strings, etc
	std::vector<std::string> vec = TokenizeValue(value);

	for (auto word : vec)
	{
		// export the distinct word and the value
		Export(word, 1, fileName, eof);
	}

	return 0;
}


/**
 * Map Constructor
 */
Map::Map(FileManager *fm, std::string intermediateDir)
{
	fm = fm;
	this->intermediateDirectory = intermediateDir;

}


/**
 * TokenizeValue splits the raw data string from the text file into distinct words and separates by white space
 */
std::vector<std::string> Map::TokenizeValue(std::string rawDataLine)
{
	std::vector<std::string> distinctWords{};
	std::string temp;

	//replace all the new line characters
	std::replace(rawDataLine.begin(), rawDataLine.end(), '\n', ' ');

	std::istringstream stream(rawDataLine);


	// read the stream of raw data into temp and split by white space
	while (getline(stream, temp, ' '))
	{
		if (temp.empty())
			continue;

		for (int i = 0; i < temp.size(); i++)
		{
			// convert to lowercase
			temp[i] = tolower(temp[i]);

			//remove punctuation + special characters
			if (ispunct(temp[i]) || (temp[i] == ':') || (temp[i] == '-') || (temp[i] == '_') || (temp[i] >= '0' && temp[i] <= '9'))
			{
				temp.erase(i--, 1);
			}
		}

		distinctWords.push_back(temp);

	}

	return distinctWords;
}