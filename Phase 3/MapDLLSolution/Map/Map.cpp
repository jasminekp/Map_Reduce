
#pragma once
#include "framework.h"
#include "Map.h"

/*
 * The Map class takes the raw data line from the input file and tokenizes the
 * raw data line into distinct words. This class then buffers the distinct words into an
 * intermediate file.
 *
 */




 //TokenizeValue reads in the raw data line from the file and stores the data into distinct words
void TokenizeValue(std::string rawDataLine, std::vector<std::string>& tokens)
{
	if (!rawDataLine.empty() && rawDataLine.back() == '\n')
		rawDataLine.pop_back();

	std::string temp;
	std::istringstream stream(rawDataLine);


	while (getline(stream, temp, ' '))
	{
		//check and remove any non-ascii characters in file
		removeNonAscii(temp);

		// convert to lowercase and remove punctuation
		for (int i = 0; i < temp.size(); i++)
		{
			temp.erase(std::remove_if(temp.begin(), temp.end(), [](const char c) { return !isalpha(c) || c == '$'; }), temp.end());

			temp[i] = tolower(temp[i]);

		}

		tokens.push_back(temp);
	}

}

//removeNonAscii method removes any non-ascii characters in the string
bool removeNonAscii(std::string& str)
{
	str.erase(remove_if(str.begin(), str.end(), [](char c) {return !(c >= 0 && c < 128); }), str.end());
	return 0;
}


//Export method takes the distinct word, value as 1, and the name of the file to store to a buffer list (keyvaluepair)
bool Export(std::string word, int value, std::string fileName)
{
	queue_mutex.lock();

	auto &buffer = bufferMap[fileName];
	auto &fileWriter = fileWriters[fileName];

	if (buffer.size() >= 1000)
	{
		
		for (auto data : buffer)
		{
			auto stringBufferData = data.first + ":" + std::to_string(data.second);
			fileWriter.write(stringBufferData);
		}

		buffer.clear();
	}


	buffer.push_back(make_pair(word, 1));

	queue_mutex.unlock();

	return 0;
}





//map method - takes the key as the (intermediate) file name and value as the raw data line in order to tokenize & export to file
extern "C" MAP_API bool map(std::string key, std::string value)
{
	std::vector<std::string> vec{};

	TokenizeValue(value, vec);

	for (auto word : vec)
	{
		Export(word, 1, key);
	}

	return 0;


}



//start method starts the mapping process by clearing any existing temp files within intermediate directory and opening the file to write to
extern "C" MAP_API bool start(std::string intermediateDirectory, std::string fileName)
{
	//std::lock_guard<std::mutex> lock(queue_mutex);
	queue_mutex.lock();

	if (bufferMap.find(fileName) != bufferMap.end() || fileWriters.find(fileName) != fileWriters.end())
	{
		return false;
	}


	fileWriters[fileName] = FileManager();

	bufferMap[fileName] = {};	//create an empty buffer for the file

	auto &fileWriter = fileWriters[fileName];


	fileWriter.open(intermediateDirectory + fileName, std::ios::out | std::ios::app);

	queue_mutex.unlock();
	return true;

}


//end method outputs any remaining data within the buffer that was not taken care of in the Export method
//and writes to the intermediate directory file
extern "C" MAP_API void end(std::string fileName)
{
	auto &buffer = bufferMap[fileName];
	auto &fileWriter = fileWriters[fileName];

	if (buffer.size() > 0)
	{
		for (auto data : buffer)
		{
			auto stringBufferData = data.first + ":" + std::to_string(data.second);
			fileWriter.write(stringBufferData);

		}
	}
	

	fileWriter.close();
}


