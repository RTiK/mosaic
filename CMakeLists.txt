cmake_minimum_required(VERSION 3.31)
project(Mosaic CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(3RDPARTY_DIR 3rdparty)

# -- OpenCV
# first, we will attempt to find a local installation in the 3rdparty directory
find_package(OpenCV CONFIG 
        PATHS ${3RDPARTY_DIR}/opencv
        NO_DEFAULT_PATH)
# if a local installation has not been found we will search for a system-wide installation
find_package(OpenCV REQUIRED CONFIG)
message("OpenCV version found: " ${OpenCV_VERSION})

# -- GoogleTest
# first, we try to find GTest sources and compile it since that is the preferred way
if (WIN32)
    # MSVC default is to build shared libraries but GTest defaults to static unless we override this setting here
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()
add_subdirectory(${3RDPARTY_DIR}/googletest)
if (GOOGLETEST_VERSION)
    # if no sources are available, we will search for a local installation
    # and if that doesn't work finally fall back to a system-wide installation
    find_package(GTest
            PATHS ${3RDPARTY_DIR}/googletest
            NO_DEFAULT_PATH)
    find_package(GTest REQUIRED)
    message("GoogleTest version found: " ${GOOGLETEST_VERSION})
else ()
    message("GoogleTest compiled from source: " ${3RDPARTY_DIR}/googletest)
endif ()

enable_testing()

set(Headers
        include/Mosaic/FileLogger.hpp
        include/Mosaic/Individual.hpp
        include/Mosaic/IndividualGeneration.hpp
        include/Mosaic/Page.hpp
        include/Mosaic/PageEvaluation.hpp
        include/Mosaic/PopulationUtil.hpp
        include/Mosaic/piece/BgrIconPiece.hpp
        include/Mosaic/piece/ColorPiece.hpp
        include/Mosaic/piece/IconPiece.hpp
        include/Mosaic/piece/LabIconClusteringPiece.hpp
        include/Mosaic/piece/LabIconPiece.hpp
        include/Mosaic/piece/LabPiece.hpp
        include/Mosaic/piece/Piece.hpp
)

set(Sources 
        src/FileLogger.cpp
        src/Individual.cpp
        src/IndividualGeneration.cpp
        src/Page.cpp
        src/PageEvaluation.cpp
        src/PopulationUtil.cpp
        src/piece/BgrIconPiece.cpp
        src/piece/ColorPiece.cpp
        src/piece/IconPiece.cpp
        src/piece/LabIconClusteringPiece.cpp
        src/piece/LabIconPiece.cpp
        src/piece/LabPiece.cpp
)

add_library(Mosaic
        ${Headers}
        ${Sources}
)

target_include_directories(Mosaic PUBLIC 
        ${OpenCV_INCLUDE_DIRS}
        include
)

target_link_libraries(Mosaic PUBLIC ${OpenCV_LIBS})

add_subdirectory(tests)
add_subdirectory(examples)
