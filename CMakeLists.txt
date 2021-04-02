cmake_minimum_required(VERSION 3.10)
project(Mosaic CXX C)

set(CMAKE_CXX_STANDARD 17)

set(3RDPARTY_DIR 3rdparty)

enable_testing()

# -- OpenCV
# first, we will attempt to find a local installation in the 3rdparty directory
find_package(OpenCV CONFIG 
        PATHS ${3RDPARTY_DIR}/opencv/build
        NO_DEFAULT_PATH)
# if a local installation has not been found we will search for a system-wide installation
find_package(OpenCV REQUIRED CONFIG)

# -- GoogleTest
# first, we try to find GTest sources and compile it since that is the preferred way
add_subdirectory(${3RDPARTY_DIR}/googletest)
if (GOOGLETEST_VERSION)
    # if no sources are available, we will search for a local installation
    # and if that doesn't work finally fall back to a system-wide installation
    find_package(GTest
            PATHS ${3RDPARTY_DIR}/googletest
            NO_DEFAULT_PATH)
    find_package(GTest REQUIRED)
endif (GOOGLETEST_VERSION)

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)
