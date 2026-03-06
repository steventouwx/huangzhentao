cmake_minimum_required(VERSION 2.6)
project(mtkfusionrild)
#-DTBOX
#add complie options
set(CMAKE_COMMON_FLAGS " -D__unused= -Wall -DRIL_SHLIB -D_GNU_SOURCE \
    -DMTK_TELEPHONYWARE_SUPPORT -DMTK_MUX_CHANNEL_64 \
    -DMTK_IMS_CHANNEL_SUPPORT -DTBOX")


if(NOT DEFINED MTK_HYP_DUAL_TELE_SUPPORT)
    message(FATAL_ERROR "MTK_HYP_DUAL_TELE_SUPPORT is not defined")
endif()


if("${MTK_HYP_DUAL_TELE_SUPPORT}" STREQUAL "yes")
    set(CMAKE_COMMON_FLAGS "${CMAKE_COMMON_FLAGS} -DMTK_HYP_DUAL_TELE_SUPPORT")
    message(STATUS "MTK_HYP_DUAL_TELE_SUPPORT is set to yes")
endif()



set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_COMMON_FLAGS} -std=c99 -Wl,--no-undefined")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_COMMON_FLAGS} -std=gnu++14")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../libmipc/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../libmipc/include/api")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../libmipc/include/common")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../libmipc/include/msg")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../fusion/libril_spm/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/log")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/property")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/mtkrilutils")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/config")



#add source file
set(src_fils rild.c)

add_executable(mtkfusionrild ${src_fils})
set(link_libs mtkrillog mtkproperty rilfusion mtkrilutils mtkconfigutils mipc dl prop)
target_link_libraries(mtkfusionrild ${link_libs})


install(TARGETS mtkfusionrild
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
