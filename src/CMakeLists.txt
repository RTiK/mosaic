set(Headers
        Piece.h
        Individual.h
        FileLogger.h
        LabPiece.h
        Page.h
        PageEvaluation.h
        IndividualGeneration.h
        PopulationUtil.h
        )

set(Sources
        Piece.cpp
        Individual.cpp
        FileLogger.cpp
        LabPiece.cpp
        Page.cpp
        PageEvaluation.cpp
        IndividualGeneration.cpp
        PopulationUtil.cpp
        )

add_library(Mosaic
        ${Sources}
        ${Headers})

target_link_libraries(Mosaic
        PUBLIC ${OpenCV_LIBS})
