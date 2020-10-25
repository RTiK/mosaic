set(Headers
        Individual.h
        Piece.h)

set(Sources
        Individual.cpp
        Piece.cpp)

add_library(Mosaic
        ${Sources}
        ${Headers})

target_link_libraries(Mosaic
        ${OpenCV_LIBS})