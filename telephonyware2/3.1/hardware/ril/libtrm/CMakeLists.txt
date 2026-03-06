cmake_minimum_required(VERSION 2.6)
project(trm)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -std=gnu99")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")

#add header files
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/log/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/property/")

#link_directories()


#add source file
set(src_files src/libtrm.c)

add_library(trm SHARED ${src_files})

#add dependency to this module
set(link_libs mtkproperty mtkrillog pthread)
target_link_libraries(trm ${link_libs})

#install
install(TARGETS trm
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
