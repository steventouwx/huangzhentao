cmake_minimum_required(VERSION 2.6)
project(mtkproperty)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -Wl,--no-undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")

if(NOT DEFINED MTK_HYP_DUAL_TELE_SUPPORT)
    message(FATAL_ERROR "MTK_HYP_DUAL_TELE_SUPPORT is not defined")
endif()


if("${MTK_HYP_DUAL_TELE_SUPPORT}" STREQUAL "yes")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMTK_HYP_DUAL_TELE_SUPPORT")
    message(STATUS "MTK_HYP_DUAL_TELE_SUPPORT is set to yes")
endif()

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/config")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/property/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/log/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")

#add source file
#file(GLOB src_fils "${CMAKE_CURRENT_SOURCE_DIR}/mtk_properties.c")
set(src_fils
    mtk_properties.c
    property_map.cpp)

add_library(mtkproperty SHARED ${src_fils})

set(link_libs prop mtkrillog)

target_link_libraries(mtkproperty ${link_libs})

#install
install(TARGETS mtkproperty
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

