cmake_minimum_required(VERSION 3.10)
project(Mosaic CXX C)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

find_package(OpenCV REQUIRED CONFIG)

include_directories(src)
include_directories(${OpenCV_INCLUDE_LIBS})

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)

# we compile GTest from source downloaded manually
# feel free to use find_package if you installed it somewhere outside of the project
add_subdirectory(3rdparty/googletest)
