#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "GetSensorData.cpp"

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
    outFile << "Time,Depth(m),Temperature(C),Altitude";
    return dataFile;

}
std::string readSensors(const std::string &dataFile) {
    std::string readLine;
    getline(std::cin, readLine);
    while ( readLine != "stop") {
        uploadDataSet(dataFile);
        return {"all"};
    }
    return"failed";
    // Close the file when done
}

void main() {
    std::cout<< readSensors(start())<< std::endl;
}
