set(Headers
        Individual.h
        FileLogger.h
        Page.h
        PageEvaluation.h
        IndividualGeneration.h
        PopulationUtil.h
        pieces/Piece.h
        pieces/LabPiece.h
        pieces/IconPiece.h
        pieces/BgrIconPiece.h
        pieces/ColorPiece.h
        pieces/LabIconPiece.h
        pieces/LabIconClusteringPiece.h
        )

set(Sources
        Individual.cpp
        FileLogger.cpp
        Page.cpp
        PageEvaluation.cpp
        IndividualGeneration.cpp
        PopulationUtil.cpp
        pieces/LabPiece.cpp
        pieces/IconPiece.cpp
        pieces/BgrIconPiece.cpp
        pieces/ColorPiece.cpp
        pieces/LabIconPiece.cpp
        pieces/LabIconClusteringPiece.cpp
        )

add_library(Mosaic
        ${Sources}
        ${Headers})

target_link_libraries(Mosaic
        PUBLIC ${OpenCV_LIBS})
