cmake_minimum_required(VERSION 2.6)
project(mtknetutils)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/netutils/")

#add source file
set(src_fils ifc_utils.c)
add_library(mtknetutils SHARED ${src_fils})

#set(link_libs mtklog mtkcutils)
#target_link_libraries(mtknetutils ${link_libs})

#install
install(TARGETS mtknetutils
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})




