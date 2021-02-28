set(Headers
        Piece.h Evaluator.h PageTools.h Individual.h FileLogger.h)

set(Sources
        Piece.cpp Evaluator.cpp PageTools.cpp Mosaic.cpp Individual.cpp FileLogger.cpp)

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