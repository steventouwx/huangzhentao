cmake_minimum_required(VERSION 2.6)

project(mtkhardwarelegacy)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall")

#add header files (-I -L)
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/hardware_legacy/")

#message(Cmake lib path: ${CMAKE_LIBRARY_PATH})

#add source file
file(GLOB src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
add_library(mtkhardwarelegacy SHARED ${src_files} )

#make this include folder visible to the modules who depends on mtkpower
target_include_directories(mtkhardwarelegacy PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../include/hardware_legacy/")

#install
install(TARGETS mtkhardwarelegacy
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

