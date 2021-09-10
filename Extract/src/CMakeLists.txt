set(Headers
        IconCenters.h
        config.h
        IconIO.h
        )

set(Sources
        extract.cpp
        config.cpp
        IconIO.cpp
        )

add_executable(Extract
        ${Sources}
        ${Headers})

target_link_libraries(Extract
        PRIVATE ${OpenCV_LIBS})