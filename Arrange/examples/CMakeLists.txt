add_executable(Example1
        Example1.cpp)

add_executable(Example2
        Example2.cpp)

target_link_libraries(Example1
        PRIVATE Mosaic
        PRIVATE ${OpenCV_LIBS})

target_link_libraries(Example2
        PRIVATE Mosaic
        PRIVATE ${OpenCV_LIBS})