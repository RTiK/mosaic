set(TestSources
        PieceTest.cpp
        PageTest.cpp
        IndividualTest.cpp)

add_executable(MosaicTest
        ${TestSources})

target_link_libraries(MosaicTest
        PUBLIC gtest_main
        PUBLIC Mosaic)
