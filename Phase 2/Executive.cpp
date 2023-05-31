#include "Workflow.h"

using std::cout;
using std::cin;
using std::string;
using std::ofstream;


/*
 * The Executive class contains the main method for starting the Map_Reduce program. This class takes command line arguments from user and passes to the
 * Workflow class in order to start the mapping and reducing.
 */


bool compareDirectories(string directory1, string directory2) 
{
	int result = directory1.compare(directory2); // compares to see if the two directories are equal

	if (result == 0) 
	{ 
		// if the directories are equal, then we allow it as a valid input
		std::cout << "It seems you are trying to re-use a directory location. Please try selecting a new directory" << std::endl;
		return 0;
	}
	else {
		return 1;
	}
}

bool checkBackSlash(string directory) {

	char lastChar = directory.back();
	char backSlash = '\\';

	if (lastChar == backSlash) 
	{
		return 0;
	}
	return 1;
}


bool checkDirExists(std::string directory) {

	FileManager fm;

	if (fm.directoryExist(directory)) {
		return 1;
	}
	else {
		std::cout << "This directory does not exist. Please try again." << std::endl;
		return 0;
	}

}


void getUserInput(std::string msgPrompt, std::string &dir, bool compareDir, std::string &dirToCompare)
{
	bool checker = 0;

	do
	{
		std::cout << "\n" + msgPrompt << std::endl;
		std::getline(std::cin, dir); 

		checker = checkDirExists(dir); //checks to see if the directory exists

		if (checkBackSlash(dir)) {
			dir = dir + "\\";
		}

		if (compareDir)
		{
			if (checker == 0) {
				continue;
			}

			checker = compareDirectories(dir, dirToCompare); // checking to see if we have different directory locations
		}

	} 
	while (checker == 0);

}


int main(int argc, char *argv[])
{
	string inputDir, intermediateDir, outputDir, mapperDLL, reducerDLL;

	getUserInput("Please enter the input directory where you have stored the files: ", inputDir, false, inputDir);
	getUserInput("Please enter the intermediate directory where you would like to have the intermediate files stored: ", intermediateDir, true, inputDir);
	getUserInput("Please enter the output directory where you would like to have the output files stored: ", outputDir, true, inputDir);


	getUserInput("Please enter the path of the Map DLL: ", mapperDLL, false, mapperDLL);
	getUserInput("Please enter the path of the Reduce DLL: ", reducerDLL, false, reducerDLL);
	
	

	try
	{
		Workflow w(inputDir, outputDir, intermediateDir, mapperDLL, reducerDLL);
		w.execute();
	}
	catch (const std::exception &exc)
	{
		std::cerr << exc.what();
		return 1;
	}

	return 0;

}




