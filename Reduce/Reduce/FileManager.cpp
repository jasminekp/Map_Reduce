#include "FileManager.h"


FileManager::FileManager()
{
}


bool FileManager::open(std::string filename, std::ios_base::openmode mode)
{
	file.open(filename, mode);
	return file.is_open();
}


void FileManager::write(const std::string& data)
{
	file << data << std::endl;
}

void FileManager::clear(std::string filename)
{
	file.open(filename, std::ios::out);
	file.close();
}

void FileManager::close()
{
	file.close();
}

void FileManager::deleteFile(std::string filename)
{
	std::experimental::filesystem::remove(filename);
}

bool FileManager::directoryExist(const std::string& dirName)
{
	std::experimental::filesystem::path path(dirName);  //static vs member methods
	return std::experimental::filesystem::is_directory(path);
}

bool FileManager::getNextBlock(std::string& data)
{

	if (std::getline(file, data))
		return true;
	return false;
}