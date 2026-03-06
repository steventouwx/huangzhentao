cmake_minimum_required(VERSION 2.6)
project(mtkconfigutils)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/config/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/property/")

#add source file
set(src_fils mtkconfigutils.c)
add_library(mtkconfigutils SHARED ${src_fils})

set(link_libs mtkconfig)
target_link_libraries(mtkconfigutils ${link_libs})

#install
install(TARGETS mtkconfigutils
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})



