# SPDX-License-Identifier: MediaTekProprietary
cmake_minimum_required(VERSION 2.6)
project(mipcnetagent)

#add complie options
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -WI --no-undefined")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -O2 -Wall -Werror -std=gnu++14 -lstdc++ -WI --no-undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++20 -Wno-address -Wno-unused-value -Wno-unused-parameter -Wno-ignored-qualifiers  -D__unused= -Wall -Wl,--no-undefined")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libmipcnetagent/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libmipcnetagent/ipmac/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/log/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/property/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionmipc/api/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionmipc/common/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionmipc/msg/")

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/property")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/mipc/api")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/mipc/common")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/mipc/msg")

#add source file
set(src_fils 
libmipcnetagent/ipmac/ip_mac.c
libmipcnetagent/netlink/libnetlink.c
libmipcnetagent/InterfaceManager.cpp
libmipcnetagent/MipcEventHandler.cpp
libmipcnetagent/NetAgentService.cpp
libmipcnetagent/NetlinkEventHandler.cpp)

add_library(mipcnetagent SHARED ${src_fils})

#add dependency mtkdns
set(link_libs dl rt pthread mtknetutils mtkrillog mtkrilutils mtksysutils mipc prop utils)
target_link_libraries(mipcnetagent ${link_libs})

#install
install(TARGETS mipcnetagent
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)


