#pragma once

//Libraries that will be used
#include <string>
#include <vector>
#include "FileManager.h"
#include <unordered_map>
#include "Map.h"



// Reduce.h - Contains declarations of Reduce functions
#pragma once

#ifdef REDUCE_EXPORTS
#define REDUCE_API __declspec(dllexport)
#else
#define REDUCE_API __declspec(dllimport)
#endif
#define WIN32_LEAN_AND_MEAN

extern "C" REDUCE_API bool reduce(std::string word, std::vector<int> occurences);
extern "C" REDUCE_API bool Export(std::string key, int reducedValue);
extern "C" REDUCE_API void startThread(std::vector<std::string> directories); //made this to start our thread
extern "C" REDUCE_API void end();

//creation of the sorterMap
extern "C" REDUCE_API void createSorter();


//creation of the sorterMap
extern "C" REDUCE_API void consolidate();

//one main master threader flow
extern "C" REDUCE_API void startMain(std::string outputdir); //made this to start our thread
extern "C" REDUCE_API void createSorterMain();

