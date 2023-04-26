
#include "Workflow.h"

using std::cout;
using std::cin;
using std::string;
using std::ofstream;


bool compareDirectories(string directory1, string directory2) {
	int result = directory1.compare(directory2); // compares to see if the two directories are equal

	if (result == 0) { // if the directories are equal, then we allow it as a valid input
		cout << "It seems you are trying to re-use a directory location. Please try selecting a new directory\n\n";
		return 0;
	}
	else {
		return 1;
	}
}

bool checkBackSlash(string directory) {

	char lastChar = directory.back();
	char backSlash = '\\';
	if (lastChar == backSlash) {
		cout << "There is a backslash present";
		return 0;
	}
	return 1;
}


bool checkInput(std::string directory) {

	FileManager fm;

	if (fm.directoryExist(directory)) {
		return 1;
	}
	else {
		cout << "I'm sorry, but this path does not seem to be correct. Please try again.\n\n";
		return 0;
	}


	/*
	const char* dir = directory.c_str(); //copying the string variable to a const char*

	struct stat holder; // will hold wether or not there is some type of data in the dirctory

	if (stat(dir, &holder) == 0) { // if statement that checks wether or no the directory is real
		std::cout << "The path you have entered is correct. Thank you.\n\n";
		return 1;
	}
	else {
		std::cout << "I'm sorry, but this path does not seem to be correct. Please try again.\n\n";
		return 0;
	}
	*/
}



int main(int argc, char *argv[])
{
	string inputDir = ""; //this directory will house all of the files that will need to be read in

	string intermediateDir = ""; //this directory will house all of the intermediate files

	string outputDir = ""; //This checks the output Directory


	bool checker = 0; //This is a bool variable to verify that the input is correct


	do 
	{
		cout << "Please enter the input directory where you have stored the files: "; //asking for user input
		std::getline(std::cin, inputDir); // recording input

		checker = checkInput(inputDir); //checks to see if the directory exists

		if (checkBackSlash(inputDir)) {
			inputDir = inputDir + "\\";
		}

	} 
	while (checker == 0);

	cout << "The path you have entered is correct. Thank you.\n\n";




	////since it is a do while loop, we don't have to manually change it to 0, since it will have to run through at leats once
	////checking intermediate file directory
	do 
	{
		cout << "Please enter the intermediate directory where you would like to have the intermediate files stored: "; //asking for user input
		getline(std::cin, intermediateDir); // recording input

		checker = checkInput(intermediateDir); //checks to see if the directory exists

		if (checkBackSlash(intermediateDir)) {
			intermediateDir = intermediateDir + "\\";
		}

		//Since we know the file directory doesn't exist, we just start the next loop through
		if (checker == 0) {
			continue;
		}

		checker = compareDirectories(inputDir, intermediateDir); // checking to see if we have different directory locations

	} 
	while (checker == 0);

	cout << "The path you have entered is correct. Thank you.\n\n";



	////checking final result directory
	do
	{
		cout << "Please enter the output directory where you would like to have the finished product stored: "; //asking for user input
		getline(std::cin, outputDir); // recording input

		checker = checkInput(outputDir); //checks to see if the directory exists

		if (checkBackSlash(outputDir)) {
			outputDir = outputDir + "\\";
		}

		//Since we know the file directory doesn't exist, we just start the next loop through
		if (checker == 0) {
			continue;
		}

		checker = compareDirectories(inputDir, outputDir); // checking to see if we have different directory locations
		
		//Since we know the file directory doesn't exist, we just start the next loop through
		if (checker == 0) {
			continue;
		}
		checker = compareDirectories(intermediateDir, outputDir); // checking to see if we have different directory locations


	} 
	while (checker == 0);
	cout << "The path you have entered is correct. Thank you.\n\n";

	try
	{
		Workflow w(inputDir, outputDir, intermediateDir);
		w.execute();
	}
	catch (const std::exception &exc)
	{
		std::cerr << exc.what();
		return 1;
	}
	
	return 0;

}




