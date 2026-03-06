cmake_minimum_required(VERSION 2.6)
project(muxreport)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -fPIC -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -O2 -Wall -Werror -std=gnu++14 -lstdc++")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${FUSION_RIL_INCDIR}")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/log/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/property/")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../hardware/ccci/include/")

#add source file
set(src_fils src/muxreport.c)
add_executable(muxreport ${src_fils})

set(link_libs log pthread prop mtkrillog mtkproperty)
target_link_libraries(muxreport ${link_libs})

#install bin
install(TARGETS muxreport DESTINATION bin)