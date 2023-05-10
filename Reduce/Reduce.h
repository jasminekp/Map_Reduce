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



//#define WIN32_LEAN_AND_MEAN

extern "C" REDUCE_API bool reduce(std::string word, std::vector<int> occurences);
extern "C" REDUCE_API bool Export(std::string key, int reducedValue);
extern "C" REDUCE_API void start(std::string OutputDir);
extern "C" REDUCE_API void end(std::string OutputDir);


FileManager fileWriter; 
