cmake_minimum_required(VERSION 3.28)
project(Research_Cyclone)

set(CMAKE_CXX_STANDARD 20)
add_compile_options("-DMOCK_RPI")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra --coverage")
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra --coverage")
set(SOURCES main.cpp 
            Libraries
            Libraries)
add_executable(Research_Cyclone main.cpp ${SOURCES})