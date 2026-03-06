cmake_minimum_required(VERSION 2.6)
project(mtkconfig)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -Wl,--no-undefined")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/config/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/log/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/property/")

#add source file
file(GLOB src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
message("SRC files:" ${src_files})

add_library(mtkconfig SHARED ${src_files})

set(link_libs mtkproperty mtkrillog prop)
target_link_libraries(mtkconfig ${link_libs})

#install
install(TARGETS mtkconfig
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
