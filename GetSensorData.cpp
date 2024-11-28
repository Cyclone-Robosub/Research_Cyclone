//
// Created by Tanishq Dwivedi on 11/28/24.
//
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <thread>

#include "StartandSend.cpp"

void uploadDataSet(std::string path) {
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