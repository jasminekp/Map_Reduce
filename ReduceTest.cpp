// MathTest.cpp : This file contains the 'main' function. Program execution begins 
// and ends there.

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

typedef double (*reducer)(std::string, std::vector<int>);
typedef double (*exporter)(std::string, int reducedValue);
typedef double (*starter)(std::string);
typedef double (*ender)(std::string);


int main()
{
	HINSTANCE hDLL;
	reducer reduce;
	exporter Export;
	starter start;
	ender end;

	std::vector<int> vect{ 10, 20, 30 };

	const wchar_t* libName = L"Reduce";

	hDLL = LoadLibraryEx(libName, NULL, NULL);   // Handle to DLL

	if (hDLL != NULL) {
		start = (starter)GetProcAddress(hDLL, "start");
		end = (ender)GetProcAddress(hDLL, "end");
		reduce = (reducer)GetProcAddress(hDLL, "reduce");
		Export = (exporter)GetProcAddress(hDLL, "Export");


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
			std::cout << "finished"  << std::endl;
		}
		else
			std::cout << "Did not load Add correctly." << std::endl;

		FreeLibrary(hDLL);
	}
	else {
		std::cout << "Library load failed!" << std::endl;
	}

	std::cin.get();
	return 0;
}
