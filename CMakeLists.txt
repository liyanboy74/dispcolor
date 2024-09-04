if(ESP_PLATFORM)

    set(LCD_Header_Dir "../gc9a01")

    set(srcs "dispcolor.c")
    list(APPEND srcs "fonts/f16f.c")
    list(APPEND srcs "fonts/f24f.c")
    list(APPEND srcs "fonts/f32f.c")
    list(APPEND srcs "fonts/f6x8m.c")
    list(APPEND srcs "fonts/font.c")

    set(inc ".")
    list(APPEND inc "fonts")
    list(APPEND inc ${LCD_Header_Dir})

    idf_component_register(SRCS  ${srcs} INCLUDE_DIRS ${inc})

else()

    cmake_minimum_required(VERSION 3.6)

    project(dispcolor)

    set(LCD_Header_Dir "../simlcd")

    set(src "dispcolor.c")
    list(APPEND src "fonts/f16f.c")
    list(APPEND src "fonts/f24f.c")
    list(APPEND src "fonts/f32f.c")
    list(APPEND src "fonts/f6x8m.c")
    list(APPEND src "fonts/font.c")

    set(inc ".")
    list(APPEND inc "fonts")
    list(APPEND inc ${LCD_Header_Dir})

    add_library(${PROJECT_NAME} ${src})

    target_link_libraries(${PROJECT_NAME} color simlcd)
    include_directories(../color ${inc})

endif()
