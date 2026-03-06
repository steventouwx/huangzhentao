cmake_minimum_required(VERSION 2.6)
project(mtkrilutils)

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -O2 -Wall -DFUS-DMTK_TELEPHONYWARE_SUPPORT -DFUSION_RIL -DMTK_SHA_SUPPOR -D_GNU_SOURCE -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../libratconfig/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/log")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/mtkrilutils/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/config/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/property/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../android/nanopb-c/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/proto/")

#add source file
set(src_fils
    libmtkrilutils.c
    mtk_rs.c
    librilutils.c
    record_stream.c
    proto/sap-api.pb.c
    MtkPhoneNumberUtils.cpp
    PhonebookBitArray.cpp
    SimPhonebookUtils.cpp
    RfxRilDumpInfo.cpp)

add_library(mtkrilutils SHARED ${src_fils})

set(link_libs mtkrillog mtkproperty ratconfig mtkconfigutils crypto log)
target_link_libraries(mtkrilutils ${link_libs})

#install
install(TARGETS mtkrilutils
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
