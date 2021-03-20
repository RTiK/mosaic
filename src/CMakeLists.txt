set(Headers
        Piece.h
        Individual.h
        FileLogger.h
        LabPiece.h
        Page.h)

set(Sources
        Piece.cpp
        Mosaic.cpp
        Individual.cpp
        FileLogger.cpp
        LabPiece.cpp
        Page.cpp)

add_library(Mosaic
        ${Sources}
        ${Headers})

target_link_libraries(Mosaic
        ${OpenCV_LIBS})

add_executable(Mosaic_main
        ${Sources}
        ${Headers})

target_link_libraries(Mosaic_main
        ${OpenCV_LIBS})