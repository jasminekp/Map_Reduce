#include <fstream>
#include <string>
#include <experimental/filesystem>
//#include <filesystem>



#pragma once
class FileManager
{

public:
	FileManager();
	bool open(std::string filename, std::ios_base::openmode mode);
	void write(const std::string& data);
	bool getNextBlock(std::string& data);
	void close();
	void clear(std::string filename);
	void deleteFile(std::string filename);

	static bool directoryExist(const std::string& dirName);

private:
	std::fstream file;
};

