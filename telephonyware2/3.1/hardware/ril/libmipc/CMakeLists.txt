cmake_minimum_required(VERSION 2.6)
project(mipc)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall  -DTRM_SUPPORT -std=gnu99")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")

#add header files
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/api")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/common")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/msg")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../libtrm/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/log/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../platformlib/include/property/")

#link_directories()


#add source file
set(src_files
    src/common/platform_mtk.c
    src/msg/hashmap.c
    src/msg/list.c
    src/msg/msg.c
    src/msg/msg_host.c)

add_library(mipc SHARED ${src_files})

#add dependency to this module
set(link_libs mtkproperty mtkrillog trm pthread)
target_link_libraries(mipc ${link_libs})

#install
install(TARGETS mipc
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
