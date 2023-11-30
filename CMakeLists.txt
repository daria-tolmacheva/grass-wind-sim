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
find_package(NGL CONFIG REQUIRED)
add_executable(${ExecutableName})
# Add NGL include path
include_directories(include $ENV{HOME}/NGL/include)
target_include_directories(${ExecutableName} PRIVATE include)
target_sources(${ExecutableName} PRIVATE
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/Grass.cpp
        ${PROJECT_SOURCE_DIR}/src/Blade.cpp
        ${PROJECT_SOURCE_DIR}/include/Grass.h
        ${PROJECT_SOURCE_DIR}/include/Blade.h )

# Testing code
find_package(GTest CONFIG REQUIRED)
find_package(NGL CONFIG REQUIRED)
include(GoogleTest)
set(TestName GrassSimulationTests)
add_executable(${TestName})
# Add NGL include path
include_directories(include $ENV{HOME}/NGL/include)
target_include_directories(${TestName} PRIVATE include)
target_sources(${TestName} PRIVATE
        ${PROJECT_SOURCE_DIR}/tests/GrassTests.cpp
        ${PROJECT_SOURCE_DIR}/tests/BladeTests.cpp
        ${PROJECT_SOURCE_DIR}/src/Grass.cpp
        ${PROJECT_SOURCE_DIR}/src/Blade.cpp
        ${PROJECT_SOURCE_DIR}/include/Grass.h
        ${PROJECT_SOURCE_DIR}/include/Blade.h )
target_link_libraries(${TestName} PRIVATE GTest::gtest GTest::gtest_main)
gtest_discover_tests(${TestName})