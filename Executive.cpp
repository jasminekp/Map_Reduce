/*

Humberto Dominguez
Professor Scott Roueche
CSE 687- Object Oriented Design
Syracuse University
Phase 1 - Executive cpp
4/20/2023

This class was created in part of creating the MapReduce code for Phase 1 for
CSE 687. The Executive class's primary purpose is to gather all data related to directories.

Since this is the cpp portion of the class, this will define how and where the the member and methods will be
used.
*/

//libraries that will be used in this class
#include "Executive.h"

//default constructor 
Executive::Executive() {}

//default destructor 
Executive::~Executive() {}

//mathod that will obtain the directories and test to verify they exist
void Executive::obtainingDirectories() {
	
    bool checker = 0; //This is a bool variable to verify that the input is correct
    do {
        cout << "Please enter the input directory where you have stored the files: "; //asking for user input
        getline(cin, inputDir); // recording input
        checker = checkInput(inputDir); 
    } while (checker == 0);

    //since it is a do while loop, we don't have to manually change it to 0, since it will have to run through at leats once
    //checking intermediate file directory
    do {
        cout << "Please enter the intermediate directory where you would like to have the intermediate files stored: "; //asking for user input
        getline(cin, intermediateDir); // recording input
        checker = checkInput(intermediateDir);
    } while (checker == 0);

    //checking final result directory
    do {
        cout << "Please enter the output directory where you would like to have the finished product stored: "; //asking for user input
        getline(cin, outputDir); // recording input
        checker = checkInput(outputDir);
    } while (checker == 0);

    Workflow wk(Executive::getinputDir, Executive::getoutputDir, Executive::getintermediateDir); // will send to the via get functions

    wk.execute(); 
}

//this is our helper function to verify that a directory exists
bool Executive::checkInput(string directory) {

    const char* dir = directory.c_str(); //copying the string variable to a const char*

    struct stat holder; // will hold wether or not there is some type of data in the dirctory

    if (stat(dir, &holder) == 0) { // if statement that checks wether or no the directory is real
    cout << "The path you have entered is correct. Thank you.\n\n";
    return 1;
    }
    else {
        cout << "I'm sorry, but this path does not seem to be correct. Please try again.\n\n";
        return 0;
    }
}

//return the input directory folder
string Executive::getinputDir() {
    return inputDir;
}

//returns the intermediate folder
string Executive::getintermediateDir() {
    return intermediateDir;
}

//returns the output folder
string Executive::getoutputDir() {
    return outputDir;
}
