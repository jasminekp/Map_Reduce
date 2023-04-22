/*

Humberto Dominguez
Professor Scott Roueche
CSE 687- Object Oriented Design
Syracuse University
Phase 1 - Reduce Header
4/20/2023

This class was created in part of creating the MapReduce code for Phase 1 for
CSE 687. The Reduce class's primary purpose is to consolidate all of the data that has been gather thus far.

Since this is the cpp portion of the class, this will define how and where the the member and methods will be
used.
*/

//libraries that will be used in this class
#include "Reduce.h"



//Constructor. Creation of the class
Reduce::Reduce() {

}

//Constructor with one arguement. Creation of the class
Reduce::Reduce(string outputDirectoryLocation) : outputDir{ outputDirectoryLocation }
{}

//Constructor. Creation of the class
Reduce::~Reduce() {

}


//reduce method. Iterators through the vector and sums up all of the occurences
bool Reduce::reduce(string word, std::vector<int> occurences) {

    int totalOccurences = 0; // variable to store the total amount of time a word was used

    //for loop used to iterate through each element of the vector
    for (auto& timesUsed : occurences) {
        totalOccurences += timesUsed; //each time through the loop, we are recording how many times the word was said
    }

    //exporting the data to the file 
    string consolidatedKeyAndValue = outputDir + word + ".txt"; //creates a txt file with the 
    fm.open(consolidatedKeyAndValue); // creates a txt file with the name of the key
    string stringRep = std::to_string(totalOccurences); //converts the total number of Occurences to a string
    string keyPlusOccurences = word + " " + stringRep; // creates a string of the key and total occurences that will be written to the file
    fm.write(consolidatedKeyAndValue, keyPlusOccurences); // writes the data to the correct file
    fm.close(consolidatedKeyAndValue); //closes the correct file

    //creation of the success file
    string successFile = outputDir + word + "_SUCCESS.txt"; // creates the success file
    fm.open(successFile); //opens the file
    fm.write(successFile, ""); //writing to the empty file
    fm.close(successFile); //closes the file

    //WHAT IS THE FORMAT OF THE OUTPUT DIRECTORY FILE


    return true;
}


void Reduce::setoutputDir() {
    cout << "Please type in the Directory that you would like the files to be save in.: "; //asking the user which directory they would like their file to be saved in
    cin >> outputDir;
}

string Reduce::getoutputDir() {
    return outputDir;
}