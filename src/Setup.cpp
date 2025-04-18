//Created by Tanishq Dwivedi
#include "Setup.hpp"

#include <signal.h>
#include <unistd.h>

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
// Create the file based on the current date
namespace fs = std::filesystem;
Setup::Setup() {}


std::string Setup::getCurrentDate() {
  time_t now = time(0);
  tm* localTime = localtime(&now);
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
  return std::string(buffer);
}
// Create a file named with the current date and return
  //the path towards the new file if it does not exist.
std::string Setup::start() {
  fs::path currentPath = fs::current_path();
  fs::path parentPath = currentPath.parent_path();
  std::string dataFilePathString = std::string(parentPath) + "/DataSets/";
  std::string date = getCurrentDate();
  std::string updateCommand = "start";
  std::string dataFile = date + ".csv";
  std::fstream outFile;
  if (!std::filesystem::exists(dataFilePathString
 + dataFile))
  {
    outFile.open(dataFilePathString
   + dataFile, std::ofstream::app);

    //Append this for every new file.
    outFile << "Time,Depth(m),Pressure,Temperature(C),Altitude\n";
    outFile.close();
  }
  return dataFilePathString + dataFile;
}
