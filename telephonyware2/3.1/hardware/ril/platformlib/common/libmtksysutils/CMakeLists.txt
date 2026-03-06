cmake_minimum_required(VERSION 2.6)
project(mtksysutils)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/log/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/property/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/sysutils/")

#add source file
set(src_fils
    FrameworkCommand.cpp
    FrameworkListener.cpp
    NetlinkEvent.cpp
    NetlinkListener.cpp
    ServiceManager.cpp
    SocketClient.cpp
    SocketListener.cpp)

add_library(mtksysutils SHARED ${src_fils})

set(link_libs mtkrillog mtkproperty)
target_link_libraries(mtksysutils ${link_libs})

#install
install(TARGETS mtksysutils
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
