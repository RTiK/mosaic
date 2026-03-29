add_executable(GrayscaleIconsExample
        GrayscaleIconsExample.cpp)

add_executable(ColorIconsExample
        ColorIconsExample.cpp)

add_executable(RealIconExample
        RealIconExample.cpp)

target_link_libraries(GrayscaleIconsExample PRIVATE Mosaic)

target_link_libraries(ColorIconsExample PRIVATE Mosaic)

target_link_libraries(RealIconExample PRIVATE Mosaic)
