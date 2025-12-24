cmake_minimum_required(VERSION 3.31)
project(Mosaic CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for IDE integration (IntelliSense, clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)
message("OpenCV version found: " ${OpenCV_VERSION})
include_directories( ${OpenCV_INCLUDE_DIRS} )
message("OpenCV include directories: " ${OpenCV_INCLUDE_DIRS})

set(Headers
        include/Mosaic/FileLogger.hpp
        include/Mosaic/HallOfFame.hpp
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
        src/HallOfFame.cpp
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
        include/
)

target_link_libraries(Mosaic PUBLIC ${OpenCV_LIBS})

# Enable testing support
enable_testing()

add_subdirectory(tests)
add_subdirectory(examples)
