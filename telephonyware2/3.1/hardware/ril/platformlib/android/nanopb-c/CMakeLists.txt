cmake_minimum_required(VERSION 2.6)
project(protobuf-c-nano-enable-malloc)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -DPB_ENABLE_MALLOC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -DPB_ENABLE_MALLOC")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")

#add source file
set(src_fils pb_decode.c pb_encode.c pb_common.c)
add_library(protobuf-c-nano-enable-malloc SHARED ${src_fils})

#install
install(TARGETS protobuf-c-nano-enable-malloc
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})



