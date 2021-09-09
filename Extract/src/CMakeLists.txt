add_executable(Extract extract.cpp IconCenters.h config.h config.cpp)

target_link_libraries(Extract
        PRIVATE ${OpenCV_LIBS})