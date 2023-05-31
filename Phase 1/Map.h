#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <regex>
#include "FileManager.h"

/*
 * Header File For Map Class
 */

class Map
{
public:
	bool Export(std::string distinctWord, int value, std::string fileName);
	bool map(std::string key, std::string value);
	Map(std::string intermediateDir);
	void start();
	void end();
	void TokenizeValue(std::string rawDataLine, std::vector<std::string>& tokens);

private:
	FileManager fileWriter;
	int Threshold = 1000;
	std::list<std::pair<std::string, int>> buffer;
	std::string intermediateDirectory;
	bool removeNonAscii(std::string& str);

};

