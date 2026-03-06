cmake_minimum_required(VERSION 2.6)
project(mtkrillog)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -Wl,--no-undefined")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/log/")

#add source file
file(GLOB src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
message("SRC files:" ${src_files})

add_library(mtkrillog SHARED ${src_files})

#make this include folder visible to the modules who depends on mtkrillog
target_include_directories(mtkrillog PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../include/log/")

target_link_libraries(mtkrillog -llog)

#install
install(TARGETS mtkrillog
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
