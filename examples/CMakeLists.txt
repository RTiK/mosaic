add_executable(Example1
        Example1.cpp)

target_link_libraries(Example1
        PRIVATE Mosaic
        PRIVATE ${OpenCV_LIBS})