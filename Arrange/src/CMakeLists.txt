set(Headers
        pieces/Piece.h
        Individual.h
        FileLogger.h
        pieces/LabPiece.h
        Page.h
        PageEvaluation.h
        IndividualGeneration.h
        PopulationUtil.h
        pieces/IconPiece.h
        pieces/BgrIconPiece.h
        )

set(Sources
        pieces/Piece.cpp
        Individual.cpp
        FileLogger.cpp
        pieces/LabPiece.cpp
        Page.cpp
        PageEvaluation.cpp
        IndividualGeneration.cpp
        PopulationUtil.cpp
        pieces/IconPiece.cpp
        pieces/BgrIconPiece.cpp
        )

add_library(Mosaic
        ${Sources}
        ${Headers})

target_link_libraries(Mosaic
        PUBLIC ${OpenCV_LIBS})
