// Map.h - Contains declarations

#pragma once

#ifdef MAP_EXPORTS
#define MAP_API __declspec(dllexport)
#else
#define MAP_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <regex>
#include "FileManager.h"




/*
 * Header File For Map Class
 */


bool Export(std::string distinctWord, int value, std::string fileName);
extern "C" MAP_API bool map(std::string key, std::string value);
//extern "C" MAP_API Map(std::string intermediateDir);
extern "C" MAP_API void start(std::string intermediateDirectory, std::string fileNumber);
extern "C" MAP_API void end();
void TokenizeValue(std::string rawDataLine, std::vector<std::string>& tokens);


FileManager fileWriter;
int Threshold = 1000;
std::list<std::pair<std::string, int>> buffer;
std::string intermediateDirectory;
bool removeNonAscii(std::string& str);



