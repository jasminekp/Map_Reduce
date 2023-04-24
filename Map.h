
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <iostream>
#include "FileManager.h"

/*
 * Header File For Map Class
 */
class Map
{
public:
	bool Export(std::string distinctWord, int value, std::string fileName, bool eof);
	bool map(std::string fileName, std::string value, bool eof);
	Map(FileManager *fm, std::string intermediateDir);

private:

	FileManager *fm;
	int Threshold = 30;
	std::list<std::pair<std::string, int>> buffer;
	std::string intermediateDirectory;

	std::vector<std::string> TokenizeValue(std::string rawDataLine);
	
};