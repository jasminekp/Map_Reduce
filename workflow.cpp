#include "Workflow.h"
#include <unordered_map>
#include "Reduce.h"

Workflow::Workflow(std::string inputD, std::string outputD, std::string intermediateD)
{
  inputDir = inputD;
  outputDir = outputD;
  intermediateDir = intermediateD;
}


bool Workflow::execute()
{

    //---------------------------------------------------------------
    //------------------Mapper phase
    //---------------------------------------------------------------
    Map mapper;
    mapper.start();
    
    //TODO -> read through inputfile from input directory

     for (const auto &inputfile : std::filesystem::directory_iterator(inputDir)) 
     {
        if (inputfile.path().extension() == ".txt") 
        {
            // Perform some action on each file
                 FileManager reader;
                 reader.open(inputfile,std::ios::in);

                std::string blockData;
                while(reader.getNextBlock(blockData))
                  {
                    mapper.map(inputfile,blockData);
                  }
                  reader.close();
                  mapper.end();
        }
     }

   /* std::string inputfile = "temp.txt";
    FileManager reader;
    reader.open(inputfile,std::ios::in);
    
    std::string blockData;
    while(reader.getNextBlock(blockData))
    {
      mapper.map(inputfile,blockData);
    }
    reader.close();
    mapper.end();

    */

    //---------------------------------------------------------------
    //-----------------sorter phase
    //---------------------------------------------------------------
    std::unordered_map<std::string, std::vector<int>> sorterMap;
    FileManager Ireader;
    std::string IblockData;

    Ireader.open(intermediateDir+"/temp.txt",std::ios::in);
    while(Ireader.getNextBlock(IblockData))
    {
      size_t pos = IblockData.find(':'); // Find position of the delimiter ':'
      std::string key = IblockData.substr(0, pos); // Extract first token before delimiter
      int value = std::stoi(IblockData.substr(pos+1)); 
      if(sorterMap.find(key)!=sorterMap.end())
      {
        sorterMap[key].push_back(value);
      }
      else
      {
        sorterMap[key] = {value};
      }
    }
    Ireader.close();
    
    //print the sorterMap values 
    //for(auto pair: sorterMap)
    //{
      //auto key = pair.first;
      //auto value = pair.second;
      
      //std::cout << key << ":";
      //for(auto v: value)
      //{
        //std::cout << v << " ";
      //}
      //std::cout << std::endl;
    //}

    //---------------------------------------------------------------
    //----------------Reducer phase
    //---------------------------------------------------------------
    FileManager writer;
    Reduce reducer;
    reducer.start();
    for(auto pair:sorterMap)
    {
      auto key  = pair.first;
      auto value = pair.second;
      
      reducer.reduce(key,value);
    }
    reducer.end();
}

