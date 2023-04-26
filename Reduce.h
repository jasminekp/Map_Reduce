#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "FileManager.h"

#pragma once

//This will declare what members and methods will be used by the class
class Reduce {

public:

	Reduce(std::string outputDir); // Default constructor of the class

	bool reduce(std::string word, std::vector<int> occurences); //reduce method that takes a string and a vector of integers as an arguement
	bool Export(std::string key, int reducedValue);
	void start();
	void end();


private:
	FileManager fileWriter;
	std::string outputDir = "";

};

