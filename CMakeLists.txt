cmake_minimum_required(VERSION 3.20)

project(RobotCar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Finn det installerte Drogon-biblioteket.
find_package(Drogon CONFIG REQUIRED)

# Lag programmet fra main.cpp.
add_executable(robot_car main.cpp)

# Koble Drogon til programmet vårt.
target_link_libraries(robot_car PRIVATE Drogon::Drogon)