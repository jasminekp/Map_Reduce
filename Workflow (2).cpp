#include "Workflow.h"
#include <unordered_map>
#include <comdef.h>
#include <thread>



/*
 * The Workflow class determines the flow of the entire Map_Reduce program. 
 *
 */

Workflow::Workflow(std::string inputD, std::string outputD, std::string intermediateD, std::string mapper, std::string reducer)
{
	this->inputDir = inputD;
	this->outputDir = outputD;
	this->intermediateDir = intermediateD;

	this->reducerDLL = reducer;
	this->mapperDLL = mapper;
}




bool Workflow::execute() 
{
	std::cout << "\n...Please wait..." << std::endl;
	int countFiles = 0;

	//---------------------------------------------------------------
	//------------------Load the Map DLL
	//---------------------------------------------------------------

	HINSTANCE hMapDLL;
	funcMapperStart mapStart;
	funcMapperMap map;
	funcMapperEnd mapEnd;

	LoadMapDLL(hMapDLL, mapStart, map, mapEnd);



	//---------------------------------------------------------------
	//------------------Mapper Phase
	//---------------------------------------------------------------
	mapStart(this->intermediateDir);

	for (const auto & inputfile : std::experimental::filesystem::directory_iterator(inputDir))
	{
		if (inputfile.path().extension().string() == ".txt")
		{
			countFiles++;
			FileManager reader;

			std::string fileName = inputfile.path().filename().string();


			reader.open(this->inputDir + fileName, std::ios::in);

			std::string blockData;

			while (reader.getNextBlock(blockData))
			{

				map(inputfile.path().filename().string(), blockData);

			}
			reader.close();

		}
	}

	mapEnd();

	
	if (countFiles == 0)
	{
		std::cout << "Error: could not find any text files in the input directory." << std::endl;

		FileManager deleter;
		deleter.deleteFile(this->outputDir + "SUCCESS.txt");
		return 1;
	}

	//---------------------------------------------------------------
	//------------------Load the Reduce DLL
	//---------------------------------------------------------------

	HINSTANCE hReduceDLL;
	funcReducerStartThread reduceStartThread; // use this for the threads
	funcReducerStartMain reduceStartMain; //use this for our main thread


	LoadReduceDLL(hReduceDLL, reduceStartThread, reduceStartMain); //assigning the functions



	//---------------------------------------------------------------
	//----------------Reducer phase
	//---------------------------------------------------------------

	std::vector<std::thread> ThreadVector; //this vector will hold our threads
	std::vector<std::string> inputArgs; // we need to fill up this intermediatedir, outputdir, and thread num
	std::vector<std::vector> inputArgHolder; // inputArgs will be put into here


	///////////////////////////////////////////////////////////////////
	//create sorter is still just looking for a temp.txt file. We'll need to modify this depnding on the output of mapper
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//We also need to populate the input args, but that depnds on how mappers output is. Again it is just looking for temp.txt
	// once we know how we're naming the mapper outputs, we can modify createsorter to look for those files
	///////////////////////////////////////////////////////////////////

	//we need to find out how many threads we are making. right now 2 is there as a placeholder
	for (int i = 0; i < 2; i++)
	{
			ThreadVector.emplace_back([&]() {start(vect[i]); }); //starts off thread
			ThreadVector[i].join(); //waits for thread to finish

	}

	reduceStartMain(outputD); //this will make everything into one file


	std::cout << "\nDone! Check Output Directory." << std::endl;


	//---------------------------------------------------------------
	//------------------End Workflow Execution & Unload the DLLs
	//---------------------------------------------------------------
	FreeLibrary(hMapDLL);
	FreeLibrary(hReduceDLL);

	return 0;


}


//check compile
bool Workflow::LoadReduceDLL(HINSTANCE &hReduceDLL, funcReducerStartThread&start, funcReducerStartMain&startMain)
{
	std::string dllName = reducerDLL + "Reduce";


	std::wstring wide_string = std::wstring(dllName.begin(), dllName.end());
	const wchar_t *libName = wide_string.c_str();


	//integrate and handle the DLL
	hReduceDLL = LoadLibraryEx(libName, NULL, NULL);   


	if (hReduceDLL != NULL)
	{
		start = (funcReducerStart)GetProcAddress(hReduceDLL, "startThread"); //check naming 
		startMain = (funcReducerStart)GetProcAddress(hReduceDLL, "startMain");//check naming 


		if (start == NULL)
		{
			std::cout << "Did not load reduce start correctly." << std::endl;
			return 1;
		}
		if (reduce == NULL)
		{
			std::cout << "Did not load reduce correctly." << std::endl;
			return 1;
		}
		if (end == NULL)
		{
			std::cout << "Did not load reduce end correctly." << std::endl;
			return 1;
		}
	}
	else {
		std::cout << "Reduce Library load failed!" << std::endl;
		return 1;
	}

	return 0;

}


bool Workflow::LoadMapDLL(HINSTANCE &hMapDLL, funcMapperStart &start, funcMapperMap &map, funcMapperEnd &end)
{
	std::string dllName = mapperDLL + "Map";


	std::wstring wide_string = std::wstring(dllName.begin(), dllName.end());
	const wchar_t *libName = wide_string.c_str();


	//integrate and handle the DLL
	hMapDLL = LoadLibraryEx(libName, NULL, NULL);  

	if (hMapDLL != NULL)
	{
		start = (funcMapperStart)GetProcAddress(hMapDLL, "start");
		map = (funcMapperMap)GetProcAddress(hMapDLL, "map");
		end = (funcMapperEnd)GetProcAddress(hMapDLL, "end");


		if (start == NULL)
		{
			std::cout << "Did not load map start correctly." << std::endl;
			return 1;
		}
		if (map == NULL)
		{
			std::cout << "Did not load map correctly." << std::endl;
			return 1;
		}
		if (end == NULL)
		{
			std::cout << "Did not load map end correctly." << std::endl;
			return 1;
		}
	}
	else {
		std::cout << "Map Library load failed!" << std::endl;
		return 1;
	}


	return 0;
}

