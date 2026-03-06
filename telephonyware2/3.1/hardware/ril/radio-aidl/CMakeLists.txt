cmake_minimum_required(VERSION 2.6)
project(mtkradioaidl)

#add complie options
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -std=gnu99")
#-fno-rtti
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17  -fPIC -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -O2 -Wall -std=gnu++17 ${CMAKE_COMMON_FLAGS} -Wl,--no-undefined")

#add header files
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/interface_cpp/include")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/src")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/log/")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/property/")

#link_directories()


#add source file
file(GLOB_RECURSE SOURCES "*.cpp")

add_library(mtkradioaidl SHARED ${SOURCES})

#add dependency to this module
set(link_libs binder utils)
target_link_libraries(mtkradioaidl ${link_libs})

#install
install(TARGETS mtkradioaidl
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
