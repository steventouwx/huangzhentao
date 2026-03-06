cmake_minimum_required(VERSION 2.6)
project(apn)

#add complie options
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall  -DTRM_SUPPORT -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -DTAGLOG_SUPPORT")

#CXXFLAGS += -g -Wall -Wextra -pthread -DTAGLOG_SUPPORT -Wl,--hash-style=gnu
#LOCAL_LIBS += -lpthread -lstdc++ -llog -lxml2 -lsqlite3

#CPPFLAGS += -isystem ../inc/ -I$(ROOT)/usr/include/logger/ -I$(ROOT)/usr/include/libxml2/ -I$(ROOT)/usr/include/
#add header files
#include_directories("${FUSION_RIL_INCDIR}/")
include_directories("${FUSION_RIL_INCDIR}/")
include_directories("${FUSION_RIL_INCDIR}/logger/")
include_directories("${FUSION_RIL_INCDIR}/libxml2/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/inc")


#link_directories()


#add source file
set(src_files
    src/apn_interface.cpp
    src/apn_manager.cpp
    src/LogUtils.cpp)

add_library(apn SHARED ${src_files})

#add dependency to this module
set(link_libs log xml2 sqlite3 pthread)
target_link_libraries(apn ${link_libs})

#install
install(TARGETS apn
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
