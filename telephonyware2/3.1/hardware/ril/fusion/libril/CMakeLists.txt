cmake_minimum_required(VERSION 2.6)
project(rilfusion)

#add complie options
set(CMAKE_COMMON_FLAGS "-DMTK_TELEPHONYWARE_SUPPORT -DANDROID_MULTI_SIM -DMTK_MUX_CHANNEL_64 -DMTK_IMS_CHANNEL_SUPPORT -DTBOX -DMTK_USE_AIDL -D__unused= -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall ${CMAKE_COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -O2 -Wall -std=gnu++17 ${CMAKE_COMMON_FLAGS} -Wl,--no-undefined")


#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/librilmtk/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/telephony/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/android/nanopb-c/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/config/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/hardware_legacy/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/mtkrilutils/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/property/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/log/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/common/libmtkrilutils/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../radio-aidl-gen/include")
#include_directories("${FUSION_RIL_INCDIR}/")

#add source filep
set(src_fils 
    ril.cpp
    ril_event.cpp
    RilSapSocket.cpp
    ril_service.cpp
    RilOpProxy.cpp
    rilAidlUtils.cpp
    ril_context.cpp
    radioConfig_service.cpp
    radiodata_service.cpp
    radioims_service.cpp
    radiomessaging_service.cpp
    radiomodem_service.cpp
    radionetwork_service.cpp
    radiosim_service.cpp
    radiosap_service.cpp
    radiovoice_service.cpp
    radiorcs_service.cpp
    ril_adapter/RadioDataAdapater.cpp
    radiodch_service.cpp
    radioecall_service.cpp)

if(NOT DEFINED MTK_HYP_DUAL_TELE_SUPPORT)
    message(FATAL_ERROR "MTK_HYP_DUAL_TELE_SUPPORT is not defined")
endif()


if("${MTK_HYP_DUAL_TELE_SUPPORT}" STREQUAL "yes")
    ADD_DEFINITIONS(-DMTK_HYP_DUAL_TELE_SUPPORT=1)
    message(STATUS "MTK_HYP_DUAL_TELE_SUPPORT is set to yes")
endif()

add_library(rilfusion SHARED ${src_fils})
if(MTK_MBRAIN_SUPPORT STREQUAL "yes")
add_definitions(-DMTK_MBRAIN_SUPPORT)
set(link_libs pthread stdc++ mtkrillog mtkrilutils protobuf-c-nano-enable-malloc mtkhardwarelegacy mtkproperty
    mtkconfigutils dl binder cutils utils binder_ndk mtkradioaidl mbrainAIDL jsoncpp)
else()
set(link_libs pthread stdc++ mtkrillog mtkrilutils protobuf-c-nano-enable-malloc mtkhardwarelegacy mtkproperty
    mtkconfigutils dl binder cutils utils binder_ndk mtkradioaidl)
endif()

target_link_libraries(rilfusion ${link_libs})

#install
install(TARGETS rilfusion
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

