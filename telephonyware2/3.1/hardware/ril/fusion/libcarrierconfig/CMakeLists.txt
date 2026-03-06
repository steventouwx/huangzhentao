cmake_minimum_required(VERSION 2.6)
project(carrierconfig)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -std=gnu99")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../mtk-ril/framework/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/")

#add source file
set(src_fils carrierconfig.c)
add_library(carrierconfig SHARED ${src_fils})

#set(link_libs mtkcutils)
target_link_libraries(carrierconfig ${link_libs})


#install
install(TARGETS carrierconfig
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})




