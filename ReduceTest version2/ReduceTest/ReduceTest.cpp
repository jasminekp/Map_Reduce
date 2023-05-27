// MathTest.cpp : This file contains the 'main' function. Program execution begins 
// and ends there.

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include "FileManager.h"
#include <thread>


typedef double (*reducer)(std::string, std::vector<int>);
typedef double (*exporter)(std::string, int reducedValue);
typedef double (*starter)(std::vector<std::string>);
typedef double (*starter2)(std::string);

typedef double (*ender)();
void LoadReduceDLL();

HINSTANCE hDLL;
reducer reduce;
exporter Export;
starter start;
ender end;
starter2 start2;



int main()
{
	//thread vector
	std::vector<std::thread> ThreadVector;

	FileManager file;


	LoadReduceDLL();
	std::vector<std::string> vect{ "D:\\School Work\\CSE 687\\Inter\\","D:\\School Work\\CSE 687\\Output\\","0"}; //creating vector needed for test
	//start(vect);

	std::vector<std::string> vect2{ "D:\\School Work\\CSE 687\\Inter2\\","D:\\School Work\\CSE 687\\Output\\","1" }; //creating vector needed for test


	//proof of concept creating and executing two threads
	for (int i = 0; i < 2; i ++)
	{
		if (i == 1) {
			ThreadVector.emplace_back([&]() {start(vect); }); // Pass by reference here, make sure the object lifetime is correct
			ThreadVector[i].join();
		}
		else {
			ThreadVector.emplace_back([&]() {start(vect2); }); // Pass by reference here, make sure the object lifetime is correct
			ThreadVector[i].join();
		}

	}



	//std::thread thread_obj(start, vect);
	//thread_obj.join();
	//std::thread thread_obj2(start, vect2);
	//thread_obj2.join();

	start2("D:\\School Work\\CSE 687\\Output\\");
}

void LoadReduceDLL() 
{



	std::vector<int> vect{ 10, 20, 30 };

	const wchar_t* libName = L"Reduce";

	hDLL = LoadLibraryEx(libName, NULL, NULL);   // Handle to DLL

	if (hDLL != NULL) {
		start = (starter)GetProcAddress(hDLL, "startThread");
		start2 = (starter2)GetProcAddress(hDLL, "startMain");

		//end = (ender)GetProcAddress(hDLL, "end");
		//reduce = (reducer)GetProcAddress(hDLL, "reduce");
		//Export = (exporter)GetProcAddress(hDLL, "Export");

		/*
		if (start != NULL) {
			start("D:\\School Work\\CSE 687\\Output2\\");
			std::cout << "finished" << std::endl;
		}
		else
			std::cout << "Did not load Add correctly." << std::endl;

		if (reduce != NULL) {
			reduce("Hey", vect);
			std::cout << "finished" << std::endl;
		}
		else
			std::cout << "Did not load Add correctly." << std::endl;

		if (end != NULL) {
			end("D:\\School Work\\CSE 687\\Output2\\");
			std::cout << "finished" << std::endl;
		}
		else
			std::cout << "Did not load Add correctly." << std::endl;

		FreeLibrary(hDLL);
	}
	else {
		std::cout << "Library load failed!" << std::endl;
	}

	std::cin.get();
	*/
	}
}