cmake_minimum_required(VERSION 3.28)
project(Research_Cyclone)

set(CMAKE_CXX_STANDARD 14)
add_compile_options("-DMOCK_RPI")

include_directories(.
        lib)

add_executable(Research_Cyclone
    main.cpp
    Depth.cpp
    phSensor.cpp
    Tempearture.cpp
    Libraries
)
find_package(nlohmann_json 3.11.3 REQUIRED)

target_link_libraries(Research_Cyclone PRIVATE nlohmann_json::nlohmann_json)

