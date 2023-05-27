#include "Map.h"

/*
 * The Map class takes the raw data line from the input file and tokenizes the
 * raw data line into distinct words. This class then buffers the distinct words into an
 * intermediate file.
 *
 */

void Map::TokenizeValue(std::string rawDataLine, std::vector<std::string>& tokens)
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
			temp.erase(std::remove_if(temp.begin(), temp.end(), [](const char c) { return !isalpha(c); }), temp.end());

			temp[i] = tolower(temp[i]);

		}

		tokens.push_back(temp);
	}

}

bool Map::removeNonAscii(std::string& str)
{
	str.erase(remove_if(str.begin(), str.end(), [](char c) {return !(c >= 0 && c < 128); }), str.end());
	return 0;
}


bool Map::Export(std::string word, int value, std::string fileName)
{
	if (buffer.size() >= Threshold)
	{
		for (auto data : buffer)
		{
			auto stringBufferData = data.first + ":" + std::to_string(data.second);
			fileWriter.write(stringBufferData);
		}
		buffer.clear();
	}
	buffer.push_back(make_pair(word, 1));

	return 0;
}


Map::Map(std::string intermediateDir)
{
	this->intermediateDirectory = intermediateDir;

}


bool Map::map(std::string key, std::string value)
{
	std::vector<std::string> vec{};

	//std::cout << value << std::endl;

	TokenizeValue(value, vec);

	for (auto word : vec)
	{
		Export(word, 1, key);
	}

	return 0;


}


void Map::start()
{
	fileWriter.clear(this->intermediateDirectory + "temp.txt");
	fileWriter.open(this->intermediateDirectory + "temp.txt", std::ios::out | std::ios::app);

}


void Map::end()
{
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


