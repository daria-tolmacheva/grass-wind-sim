# We always try to use a version > 3.1 if available
cmake_minimum_required(VERSION 3.1)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
  set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# Name of the project, it is best to use something different from the exe name
project(GrassSimulation_build)

# Here we set the C++ standard to use
set(CMAKE_CXX_STANDARD 17)

# Now we add our target executable and the file it is built from
set(ExecutableName GrassSimulation)
add_executable(${ExecutableName})
target_sources(${ExecutableName} PRIVATE
        src/main.cpp
        src/Grass.cpp
        include/Grass.h)

# Testing code
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)
set(TestName GrassSimulationTests)
add_executable(${TestName})
target_include_directories(${TestName} PRIVATE include)
target_sources(${TestName} PRIVATE
        tests/GrassTests.cpp
        src/Grass.cpp
        include/Grass.h)
target_link_libraries(${TestName} PRIVATE GTest::gtest GTest::gtest_main)
gtest_discover_tests(${TestName})