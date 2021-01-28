set(Headers
        Piece.h Evaluator.cpp Evaluator.h PageTools.cpp PageTools.h)

set(Sources
        Piece.cpp Evaluator.cpp Evaluator.h PageTools.cpp PageTools.h)

add_library(Mosaic
        ${Sources}
        ${Headers})

target_link_libraries(Mosaic
        ${OpenCV_LIBS})