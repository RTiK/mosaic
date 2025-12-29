add_executable(Example1
        Example1.cpp)

add_executable(Example2
        Example2.cpp)

add_executable(Example3
        Example3.cpp)

add_executable(Example3_JsonExport
        Example3_JsonExport.cpp)

add_executable(TestJsonExport
        TestJsonExport.cpp)

target_link_libraries(Example1 PRIVATE Mosaic)

target_link_libraries(Example2 PRIVATE Mosaic)

target_link_libraries(Example3 PRIVATE Mosaic)

target_link_libraries(Example3_JsonExport PRIVATE Mosaic)

target_link_libraries(TestJsonExport PRIVATE Mosaic)
