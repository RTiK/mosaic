set(TestSources
        pieces/PieceTest.cpp
        PageEvaluationTest.cpp
        IndividualTest.cpp
        pieces/IconPieceTest.cpp
        )

add_executable(MosaicTest
        ${TestSources})

target_link_libraries(MosaicTest
        PRIVATE gtest_main
        PRIVATE Mosaic)
