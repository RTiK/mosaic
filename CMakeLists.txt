cmake_minimum_required(VERSION 3.17)
project(Mosaic)

set(CMAKE_CXX_STANDARD 14)

add_executable(Mosaic main.cpp)

include_directories("/usr/local/Cellar/opencv/4.4.0/include/opencv4")

find_package( OpenCV REQUIRED )

target_link_libraries(Mosaic LINK_PUBLIC ${OpenCV_LIBS})