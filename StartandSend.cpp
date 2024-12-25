#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


class StartandSend{
    public:
    StartandSend(){
    }
std::string getCurrentDateTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);
    return std::string(buffer);
}


std::string getCurrentDate() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
    return std::string(buffer);
}



std::string start() {
    // Create a file named with the current date
    std::string date = getCurrentDate();
    std::string updateCommand = "start";
    std::string dataFile = "/DataSets" + date + ".txt";
    std::ofstream outFile(dataFile);

    if (!outFile) {
        std::cerr << "Error opening file for writing: " << dataFile << std::endl;
        return dataFile;
    }
    outFile << "Time,Depth(m),Temperature(C),Altitude,Pressure";
    return dataFile;

}



};