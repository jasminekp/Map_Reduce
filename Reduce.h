/*

Humberto Dominguez
Professor Scott Roueche
CSE 687- Object Oriented Design
Syracuse University
Phase 1 - Reduce Header
4/20/2023

This class was created in part of creating the MapReduce code for Phase 1 for
CSE 687. The Reduce class's primary purpose is to consolidate all of the data that has been gather thus far.

Since this is the header portion of the class, this will declar all methods and members that the .cpp file
will use.
*/

#pragma once

//libraries that will be used in this class
#include <iostream>
#include "Reduce.h"
#include <vector>
#include <fstream>
#include "Filemanager.h"
#include <string>



//Declaring which member from a specific namespace is going to be used
using std::cout;
using std::cin;
using std::string;
using std::ofstream;



//This will declare what members and methods will be used by the class
class Reduce {

	//declaration of public members and methods that will be seen by the user
public:

	Reduce(); // Default constructor of the class

	Reduce(string outputDirectoryLocation); //secondary constructor in which the user supplies the output dir

	~Reduce(); //destructor

	bool reduce(string word, std::vector<int> occurences); //reduce method that takes a string and a vector of integers as an arguement

	void setoutputDir(); //allows us to change the value of the outputDir

	string getoutputDir(); //allows us to set the value of outputDir

	//declaration of private members and methods that will not be seen by the user
private:

	string outputDir = ""; //Output Directory which will be given a value when the class is created

	FileManager fm;

};
