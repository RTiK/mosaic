set(TestSources
        PageEvaluationTest.cpp
        IndividualTest.cpp
        pieces/PieceTest.cpp
        pieces/IconPieceTest.cpp
        pieces/BgrIconPieceTest.cpp
        )

add_executable(MosaicTest
        ${TestSources})

target_link_libraries(MosaicTest
        PRIVATE gtest_main
        PRIVATE Mosaic)
