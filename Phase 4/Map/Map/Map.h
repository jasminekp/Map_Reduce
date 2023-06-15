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
#include <unordered_map>
#include "FileManager.h"
#include <mutex>



/*
 * Header File For Map Class
 */


bool Export(std::string distinctWord, int value, std::string fileName);
extern "C" MAP_API bool map(std::string key, std::string value);
extern "C" MAP_API bool start(std::string intermediateDirectory, std::string fileName);
extern "C" MAP_API void end(std::string fileName);
void TokenizeValue(std::string rawDataLine, std::vector<std::string>& tokens);
bool removeNonAscii(std::string& str);


FileManager fileWriter;
int Threshold = 1000;
std::unordered_map<std::string, std::list<std::pair<std::string, int>>> bufferMap;
std::unordered_map<std::string, FileManager> fileWriters;
std::mutex queue_mutex;

std::string intermediateDirectory;



