enable_testing()

set(TestSources
        PageEvaluationTest.cpp
        IndividualTest.cpp
        piece/PieceTest.cpp
        piece/IconPieceTest.cpp
        piece/BgrIconPieceTest.cpp
)

add_executable(MosaicTests ${TestSources})

target_link_libraries(MosaicTests
        Mosaic
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(MosaicTests)
