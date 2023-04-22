#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <sstream>
#include <cctype>
#include "FileManager.h"
using namespace std;

class Map
{

private:
	FileManager fm;
	int Threshold = 100;
	list<pair<string, int>> buffer;
	vector<string> distinctWords;

	bool TokenizeValue(string rawDataLine)
	{
		// split string by word and count the number of times it appears in line
		if (rawDataLine.empty() || rawDataLine.length() <= 0)
		{
			return false;
		}

		distinctWords.clear();
		string temp;
		istringstream stream(rawDataLine);

		// read the stream of raw data into temp and split by white space
		while (getline(stream, temp, ' '))
		{
			// convert to lowercase and remove punctuation
			for (int i = 0; i < temp.size(); i++)
			{
				temp[i] = tolower(temp[i]);

				if (ispunct(temp[i]))
				{
					temp.erase(i--, 1);
				}
			}

			distinctWords.push_back(temp);
		}

		return true;
	}

public:
	bool Export(string distinctWord, int value)
	{
		if (buffer.size() > Threshold)
		{
			// writeIntermediateFile needed for the fm class
			string fileName;
			fm.writeIntermediateFile(fileName, distinctWord, value);
		}
		else
		{
			buffer.push_back(make_pair(distinctWord, 1));
		}
	}

	bool map(string fileName, std::string value)
	{
		// read in the string from the file through the FileManager class
		string rawData = fm.readFile();

		// tokenize the string into distinct words and remove punctuation, strings, etc
		if (TokenizeValue(value))
		{
			// loop through the distinctWords and add to buffer
			for (auto word : distinctWords)
			{
				// export the distinct word and the value
				Export(word, 1);
			}

			return 0;
		}
	}
};