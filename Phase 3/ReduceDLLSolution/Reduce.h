#pragma once
// Reduce.h - Contains declarations of Reduce functions

#ifdef REDUCE_EXPORTS
#define REDUCE_API __declspec(dllexport)
#else
#define REDUCE_API __declspec(dllimport)
#endif

//Libraries that will be used
#include <string>
#include <vector>
#include "FileManager.h"
#include <unordered_map>
#include <mutex>


//#define WIN32_LEAN_AND_MEAN

extern "C" REDUCE_API bool reduce(std::string word, std::vector<int> occurences, std::string fileName);
extern "C" REDUCE_API bool Export(std::string key, int reducedValue, std::string fileName);
extern "C" REDUCE_API void start(std::string OutputDir, std::string fileName);
extern "C" REDUCE_API void end(std::string OutputDir, std::string fileName);

extern "C" REDUCE_API void createSuccessFile(std::string OutputDir); 


std::unordered_map<std::string, FileManager> fileWriters;
std::mutex queue_mutex;
FileManager fileWriter; 
