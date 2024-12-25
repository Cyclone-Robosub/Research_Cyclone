#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <thread>
#include "GetSensorData.cpp"
#include "StartandSend.cpp"

int main() {
    std::string command;
    std::string dataFile;
    while(true)
    {
        std::getline(std::cin, command);
        if(command == "start.")
        {
            break;
        }
    }
    std::shared_ptr<StartandSend> startandSend = std::make_shared<StartandSend>();
    std::string selectedFile = startandSend->start();
    std::shared_ptr<GetSensorData> getSensorData = std::make_shared<GetSensorData>(selectedFile);
    while()
    getSensorData->uploadDataSet();
    return 0;

}
