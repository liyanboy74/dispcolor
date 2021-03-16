set(GC9A01_Header_Dir "../gc9a01/include")

set(srcs "dispcolor.c")
list(APPEND srcs "include/fonts/f16f.c")
list(APPEND srcs "include/fonts/f24f.c")
list(APPEND srcs "include/fonts/f32f.c")
list(APPEND srcs "include/fonts/f6x8m.c")
list(APPEND srcs "include/fonts/font.c")

set(inc "include")
list(APPEND inc "include/fonts")
list(APPEND inc ${GC9A01_Header_Dir})

idf_component_register(SRCS  ${srcs} INCLUDE_DIRS ${inc})
