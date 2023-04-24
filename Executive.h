/*

Humberto Dominguez
Professor Scott Roueche
CSE 687- Object Oriented Design
Syracuse University
Phase 1 - Executive Header
4/20/2023

This class was created in part of creating the MapReduce code for Phase 1 for
CSE 687. The Executive class's primary purpose is to gather all data related to directories.

Since this is the header portion of the class, this will declar all methods and members that the .cpp file
will use.
*/

#pragma once

//libraries that will be used in this class
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include "Workflow.h"



//Declaring which member from a specific namespace is going to be used
using std::cout;
using std::cin;
using std::string;
using std::ofstream;

//This will declare what members and methods will be used by the class
class Executive
{
	//declaration of public members and methods that will be seen by the user
public:
	Executive(); //default constructor

	~Executive(); //default destructor

	void obtainingDirectories(); //Method that iniates the user to enter the directories that will be used

	string getinputDir(); //returns the string for the input directory

	string getintermediateDir(); //returns the string for the intermediate dirctory 

	string getoutputDir(); // returns the string for the output directory


	//declaration of private members and methods that will not be seen by the user
private:

	bool checkInput(string directory);

	string inputDir = ""; //this directory will house all of the files that will need to be read in

	string intermediateDir = ""; //this directory will house all of the intermediate files

	string outputDir = ""; //this directory will house all of the output files
};

