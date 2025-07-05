# -- GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
        PRIVATE
        Mosaic
        GTest::gtest
        GTest::gtest_main
)

target_include_directories(MosaicTests
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(MosaicTests)
