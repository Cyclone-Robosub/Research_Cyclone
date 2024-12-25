//
// Created by Tanishq Dwivedi on 11/28/24.
//
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <thread>

#include "StartandSend.cpp"

class GetSensorData {
        private:
        std::string path;
        public:
        GetSensorData(std::string givenPath) {
                path = givenPath;
        }
        std::string getCurrentDateTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);
    return std::string(buffer);
}
void uploadDataSet() {
        std::string currentTime = getCurrentDateTime();
        std::string ReadDataLine;
        getline(std::cin, ReadDataLine);
        // Write data to the file
        std::ofstream outFile(path);
        outFile << currentTime << "," << ReadDataLine << std::endl;
        // need multi-thread management
        std::this_thread::sleep_for(std::chrono::seconds(4));
        // e.g., 5-second delay
}
};