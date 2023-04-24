#include <iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <gtest/gtest.h>

class FileManager 
{ 
    //Declaring the private attributes of the FileManager Class
    private:
    std::fstream fs; 
    const int BlOCK_SIZE = 100; 

    public:
    //Default Constructor
    FileManager()
    {

    }   

    bool open(std::string filename)
    {
        fs.open(filename);
        if (fs.is_open())
        {
            return true; 
        }
        return false; 
    }

    bool close()
    {
        fs.close();

        return true; 
    }

    bool write(std::string filename, const std::string& data)
    {
        std::ofstream outfile = std::ofstream(filename);
        outfile << data << std::endl; 
        outfile.close();
        return 0; 
    }

    std::string& getNextBlock(std::string filename)
    {
        std::string line;
        std::string result = " "; 

        for(int i = 0 ; i < BlOCK_SIZE; i++)
        {

        if (!std::getline(fs, line));
            break; 

            result = result + line + "\n"; 

        }

        return result; 
    }

    bool directoryExist(const std::string& directoryName)
    {
        std::filesystem::path path(directoryName);
        return std::experimental::filesystem::is_directory(path);

    }


  TEST(,)
    {
        cout << "TEST CALLED>>>>" << endl; 

    }


    
    int main (int argc, const char * argv[])
    {
    
    //testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();

    //Creating an object of the type FileManager

    FileManager fm = FileManager(); 

    return 0; 

    }

};
