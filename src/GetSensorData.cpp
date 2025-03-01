//
// Created by Tanishq Dwivedi on 11/28/24.
//
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

// By inheriting the Node we create a subscription Node to the topic
// "research_node" In main.cpp, the ROS node will make sure that the 
// subscrition node is not created every single time as the spin method
// has data stating that the subscription has already been created. In
// addition, we bind the subscription object to the uploadDataSet with the 
// ROS messages.
class GetSensorData : public rclcpp::Node {
 private:
  std::ofstream outFile;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  // auto topic_callback;

 public:
  GetSensorData(std::string givenPath) : Node("researchcnode") {
    outFile.open(givenPath, std::ofstream::app);
    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "researchSensorsData", 10,
        std::bind(&GetSensorData::uploadDataSet, this, std::placeholders::_1));
  }
  void uploadDataSet(const std_msgs::msg::String::SharedPtr msg) {
    std::string currentTime = getCurrentDateTime();
    std::string ReadDataLine =
        msg->data;  // Directly extract data from the message
    /*
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds
          = std::chrono::duration_cast<std::chrono::milliseconds>(
                duration)
                .count();
  */
    // Display the date and time represented by the timestamp
    // std::string yay =  std::ctime(&timestamp);
    // getline(std::cin, ReadDataLine);
    // Write data to the file;
    //std::cout << "HERE HERE " << ReadDataLine << std::endl;
    outFile << currentTime << "," << ReadDataLine << std::endl;
    // e.g., 5-second delay
  }
  void Close() { outFile.close(); }

  // Get the time in Hour Minute and Second Time
  // This Assumes that the file name has the date. 
  std::string getCurrentDateTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);
    return std::string(buffer);
  }
};