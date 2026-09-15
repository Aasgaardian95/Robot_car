cmake_minimum_required(VERSION 3.20)

project(RobotCar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Drogon CONFIG REQUIRED)

# Finn OpenCV-delene for bilder, kamera og JPEG.
find_package(OpenCV CONFIG REQUIRED COMPONENTS core videoio imgcodecs)

add_executable(robot_car main.cpp)

# Fortell kompilatoren hvor OpenCVs headerfiler ligger.
target_include_directories(robot_car PRIVATE ${OpenCV_INCLUDE_DIRS})

# Koble begge bibliotekene til programmet.
target_link_libraries(robot_car PRIVATE Drogon::Drogon ${OpenCV_LIBS})