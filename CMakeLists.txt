cmake_minimum_required(VERSION 3.28)
project(Research_Cyclone)

set(CMAKE_CXX_STANDARD 14)
add_compile_options("-DMOCK_RPI")

include_directories(.
        lib)

add_executable(Research_Cyclone
    main.cpp
    Depth.hpp
    phSensor.hpp
    Tempearture.hpp
    /Libraries
)
find_package(nlohmann_json 3.11.3 REQUIRED)

target_link_libraries(Research_Cyclone PRIVATE nlohmann_json::nlohmann_json)
# GTest start
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/d144031940543e15423a25ae5a8a74141044862f.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        run_tests
        
)
target_link_libraries(
        run_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
#Gtest end