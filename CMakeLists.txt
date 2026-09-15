cmake_minimum_required(VERSION 3.20)

project(RobotCar LANGUAGES CXX)

# Bruk C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Bygg programmet fra main.cpp.
add_executable(robot_car main.cpp)