cmake_minimum_required(VERSION 2.6)
project(atcid)
message(STATUS "test xxxxxxzwfxxxxx")
#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -fPIC -Wall -DMTK_ATCI_MIPC -DTRM_SUPPORT -D__ATCI_ENABLE__")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -O2 -Wall -std=gnu++17 -DMTK_ATCI_MIPC -DTRM_SUPPORT -D__ATCI_ENABLE__ -Wl,--no-undefined")

message(STATUS "FUSION_RIL_INCDIR=${FUSION_RIL_INCDIR}")
#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/platform/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${FUSION_RIL_INCDIR}")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/log/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/property/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionmipc/api/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionmipc/common/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionmipc/msg/")
include_directories("${FUSION_RIL_INCDIR}/mlclient/")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../hardware/ccci/include/")

#add source file
#platform/atcid_adaptation.cpp
#platform/atcid_cust_cmd_platform.c
set(src_fils
platform/atcid_mipc.c
src/at_tok.c
src/atcid_cmd_dispatch.c
src/atcid_cust_cmd.c
src/atcid_serial.c
src/atcid_util.c
src/atcid.c
src/atcid_mlclient_dispatch.c)
add_executable(atcid ${src_fils})

set(link_libs log pthread nvram file_op mipc trm mtkrillog mtkproperty mlclient cutils prop ubox)
target_link_libraries(atcid ${link_libs})

#install bin
install(TARGETS atcid DESTINATION bin)
