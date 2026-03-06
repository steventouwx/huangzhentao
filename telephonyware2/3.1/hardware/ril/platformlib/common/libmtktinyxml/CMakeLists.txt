cmake_minimum_required(VERSION 2.6)
project(mtktinyxml)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -O2 -Wall -frtti -save-temps -std=gnu++14")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/tinyxml")


#add source file
#file(GLOB_RECURSE src_files "${CMAKE_CURRENT_SOURCE_DIR}/" "*.cpp")

set(src_files
    tinyxml.cpp
    tinyxmlparser.cpp
    tinyxmlerror.cpp
    tinystr.cpp)

#message("SRC files:" ${src_files})
add_library(mtktinyxml SHARED ${src_files})

target_link_libraries(mtktinyxml pthread)

#install
install(TARGETS mtktinyxml
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})


