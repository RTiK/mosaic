set(TestSources
        PieceTest.cpp
        IndividialTest.cpp)

add_executable(MosaicTest
        ${TestSources})

target_link_libraries(MosaicTest
        PUBLIC gtest_main
        PUBLIC Mosaic)
