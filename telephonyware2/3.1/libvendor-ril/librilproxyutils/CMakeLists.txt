cmake_minimum_required(VERSION 2.6)
project(rilproxyutils_static)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall  -DTRM_SUPPORT -std=gnu99")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -DTAGLOG_SUPPORT")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")


#link_directories()


#add source file
set(src_files
    record_stream.c)

add_library(rilproxyutils_static STATIC ${src_files})

#add dependency to this module
#set(link_libs log xml2 sqlite3 pthread)
#target_link_libraries(apn ${link_libs})

#install
install(TARGETS rilproxyutils_static
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
