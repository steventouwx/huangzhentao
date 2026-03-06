cmake_minimum_required(VERSION 2.6)
project(ratconfig)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/property/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/log/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/ratconfig/")

#add source file
set(src_fils ratconfig.c)
add_library(ratconfig SHARED ${src_fils})

set(link_libs mtkrillog mtkproperty)
target_link_libraries(ratconfig ${link_libs})


#install
install(TARGETS ratconfig
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})



