cmake_minimum_required(VERSION 2.6)
project(mtktelephonyservice)

add_definitions(
    -D_GNU_SOURCE
    -DUPDATE_APN
    -DTAGLOG_UTILS
    -DTAGLOG_SUPPORT
    -DTELE_FWK
    -DTELE_DATA_ATTACH
    -DTELE_DATA_FWK
    #-DTELE_AGPS
    -DTELEPHONYWARE
    -DANDROID_SIM_COUNT_2
    -DANDROID_MULTI_SIM
    -DNET_SOCKET_SUPPORT
)

if (RAT_CONFIG_C2K_SUPPORT STREQUAL "yes")
    add_definitions(-DC2K_SUPPORT)
endif()

if (MTK_MULTI_SIM_SUPPORT STREQUAL "dsds")
    add_definitions(-DMODE_DSDS)
elseif (MTK_MULTI_SIM_SUPPORT STREQUAL "dsss")
    add_definitions(-DMODE_DSSS)
elseif (MTK_MULTI_SIM_SUPPORT STREQUAL "ss")
    add_definitions(-DMODE_SSS)
endif()

if(MTK_MBRAIN_SUPPORT STREQUAL "yes")
    add_definitions(-DMTK_MBRAIN_SUPPORT)
endif()

#add complie options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -fPIC -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -O2 -Wall -std=gnu++17 -frtti -Wl,--no-undefined")

#-fno-rtti
message(STATUS " zwf FUSION_RIL_INCDIR=${FUSION_RIL_INCDIR}")
#add header files (-I -L)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../apn/inc")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../mtk-rilproxy")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/framework/base")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/framework/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/framework/include/base")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/framework/include/core")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/nw")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/client")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/apn")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/oem")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/sdk")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/suplmessageparser")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/data")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/call")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/agpsadapter")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/sms")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telephony/sms/gsm")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/radio-hal-adapter")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/")
#include_directories("${FUSION_RIL_INCDIR}/usr/include/u")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/mtkrilutils")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/log")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/property")
include_directories("${FUSION_RIL_INCDIR}/mtkfusionril/hardware_legacy")
#add source file
#platform/atcid_adaptation.cpp
#platform/atcid_cust_cmd_platform.c
set(src_fils
    ril_callbacks.c
    atchannel.c
    misc.c
    at_tok.c
    main.c
    # telephony/agpsadapter/protocol/Agps2FrameworkInterface.c
    # telephony/agpsadapter/protocol/Framework2AgpsInterface.c
    # telephony/agpsadapter/protocol/mtk_socket_data_coder.c
    # telephony/agpsadapter/protocol/mtk_socket_utils.c
    # telephony/agpsadapter/RpAgpsNwAdapter.cpp
    # telephony/agpsadapter/RpRilAgpsNwClient.cpp
    telephony/suplmessageparser/ConcentratedSms.cpp
    telephony/suplmessageparser/SuplMsgDispatcher.cpp
    telephony/sms/GsmSmsAddress.cpp
    telephony/sms/PduParser.cpp
    telephony/sms/SmsAddress.cpp
    telephony/sms/SmsHeader.cpp
    telephony/sms/SmsMessage.cpp
    telephony/sms/SmsParserUtils.cpp
    telephony/sms/gsm/sms_pdu.cpp
    telephony/sms/RpSmsController.cpp
    telephony/sms/WakelockUtils.cpp
    socket_channel.cpp
    socket_util.cpp
    framework/base/RfxVariant.cpp
    framework/base/RfxMessage.cpp
    framework/base/RfxSocketState.cpp
    framework/core/Rfx.cpp
    framework/core/RfxObject.cpp
    framework/core/RfxClassInfo.cpp
    framework/core/RfxAction.cpp
    framework/core/RfxAsyncSignal.cpp
    framework/core/RfxSignal.cpp
    framework/core/RfxTimer.cpp
    framework/core/RfxMainThread.cpp
    framework/core/RfxController.cpp
    framework/core/RfxRootController.cpp
    framework/core/RfxSlotRootController.cpp
    framework/core/RfxControllerFactory.cpp
    framework/core/RfxStatusManager.cpp
    framework/core/RfxRilAdapter.cpp
    framework/core/RfxDispatchThread.cpp
    framework/core/RfxSocketStateManager.cpp
    framework/base/RfxDebugInfo.cpp
    framework/base/RfxDefaultDestUtils.cpp
    framework/base/RfxCallState.cpp
    framework/base/RfxNwServiceState.cpp
    framework/core/RfxTestBasicController.cpp
    framework/core/RfxTestSuitController.cpp
    framework/base/RfxTransferUtils.cpp
    framework/base/RfxParcelTransfer.cpp
    framework/base/RfxParcelTransferUtils.cpp
    telephony/client/RilClient.cpp
    telephony/client/RilDefaultClient.cpp
    telephony/client/RilClientQueue.cpp
    telephony/client/RpRilClientController.cpp
    telephony/nw/RpNwStateController.cpp
    telephony/nw/RpNwPhoneTypeUpdater.cpp
    telephony/power/RpRadioController.cpp
    telephony/power/RadioMessage.cpp
    telephony/call/RpCallController.cpp
    telephony/sim/RpSimController.cpp
    telephony/sim/RpSimControllerBase.cpp
    telephony/sim/RpGsmSimController.cpp
    telephony/sim/RpC2kSimController.cpp
    telephony/apn/RpApnController.cpp
    telephony/data/RpDataController.cpp
    telephony/data/RpDataUtils.cpp
    telephony/data/ApnContext.cpp
    telephony/data/ApnSetting.cpp
    telephony/data/RetryManager.cpp
    telephony/data/Common.cpp
    telephony/data/NetworkConfig.cpp
    telephony/data/DcTracker.cpp
    telephony/data/DcFail.cpp
    telephony/data/DcController.cpp
    telephony/data/DataConnection.cpp
    telephony/data/PdnManager.cpp
    telephony/data/DataCallResponse.cpp
    #telephony/oem/RpOemController.cpp
    telephony/nw/SignalStrength.cpp
    telephony/sdk/Wrapper.cpp
    telephony/sdk/RilSdkClient.cpp
    telephony/sdk/TeleService.cpp
    telephony/sdk/VoiceService.cpp
    telephony/sdk/RpAidlAdapter.cpp
    telephony/sdk/TeleData.cpp
    telephony/sdk/MbrainClient.cpp
    telephony/sdk/MbrainMessageSender.cpp
    radio-hal-adapter/radio_hal_utils.cpp
    radio-hal-adapter/radio_service_proxy.cpp
    radio-hal-adapter/radio_service.cpp
    radio-hal-adapter/data/radio_data.cpp
    radio-hal-adapter/data/radio_data_indication.cpp
    radio-hal-adapter/data/radio_data_response.cpp
    radio-hal-adapter/data/mtkradioex_data.cpp
    radio-hal-adapter/data/mtkradioex_data_indication.cpp
    radio-hal-adapter/data/mtkradioex_data_response.cpp
    radio-hal-adapter/data/mtkradioex_assist_response.cpp
    radio-hal-adapter/ims/radio_ims.cpp
    radio-hal-adapter/ims/radio_ims_indication.cpp
    radio-hal-adapter/ims/radio_ims_response.cpp
    radio-hal-adapter/ims/mtkradioex_ims.cpp
    radio-hal-adapter/ims/mtkradioex_ims_indication.cpp
    radio-hal-adapter/ims/mtkradioex_ims_response.cpp
    radio-hal-adapter/modem/radio_modem.cpp
    radio-hal-adapter/modem/radio_modem_indication.cpp
    radio-hal-adapter/modem/radio_modem_response.cpp
    radio-hal-adapter/modem/mtkradioex_modem.cpp
    radio-hal-adapter/modem/mtkradioex_modem_indication.cpp
    radio-hal-adapter/modem/mtkradioex_modem_response.cpp
    radio-hal-adapter/network/radio_network.cpp
    radio-hal-adapter/network/radio_network_response.cpp
    radio-hal-adapter/network/radio_network_indication.cpp
    radio-hal-adapter/network/mtkradioex_network.cpp
    radio-hal-adapter/network/mtkradioex_network_response.cpp
    radio-hal-adapter/network/mtkradioex_network_indication.cpp
    radio-hal-adapter/sim/radio_sim.cpp
    radio-hal-adapter/sim/radio_sim_response.cpp
    radio-hal-adapter/sim/radio_sim_indication.cpp
    radio-hal-adapter/sim/mtkradioex_sim.cpp
    radio-hal-adapter/sim/mtkradioex_sim_response.cpp
    radio-hal-adapter/sim/mtkradioex_sim_indication.cpp
    radio-hal-adapter/sms/radio_messaging.cpp
    radio-hal-adapter/sms/radio_messaging_response.cpp
    radio-hal-adapter/sms/radio_messaging_indication.cpp
    radio-hal-adapter/sms/mtkradioex_messaging.cpp
    radio-hal-adapter/sms/mtkradioex_messaging_response.cpp
    radio-hal-adapter/sms/mtkradioex_messaging_indication.cpp
    radio-hal-adapter/voice/radio_voice.cpp
    radio-hal-adapter/voice/radio_voice_response.cpp
    radio-hal-adapter/voice/radio_voice_indication.cpp
    radio-hal-adapter/voice/mtkradioex_voice.cpp
    radio-hal-adapter/voice/mtkradioex_voice_response.cpp
    radio-hal-adapter/voice/mtkradioex_voice_indication.cpp
    radio-hal-adapter/ecall/mtkradioex_ecall.cpp
    radio-hal-adapter/ecall/mtkradioex_ecall_response.cpp
    radio-hal-adapter/ecall/mtkradioex_ecall_indication.cpp
    radio-hal-adapter/config/radio_config.cpp
    radio-hal-adapter/config/radio_config_response.cpp
    radio-hal-adapter/config/radio_config_indication.cpp)

add_executable(mtktelephonyservice ${src_fils})

if(MTK_MBRAIN_SUPPORT STREQUAL "yes")
target_link_libraries(mtktelephonyservice
    mtkproperty
    mtkrillog
    dl
    log
    pthread
    prop
    rt
    apn
    rilproxyutils_static
    binder
    utils
    cutils
    vendor.mediatek.hardware.tbox-V1-ndk
    vendor.mediatek.hardware.voice-V1-ndk
    binder_ndk
    mtkrilutils
    mtkradioaidl
    mtk_audio_mixer_ctrl
    asound
    mtkhardwarelegacy
    mbrainAIDL
    jsoncpp
)
else()

target_link_libraries(mtktelephonyservice
    mtkproperty
    mtkrillog
    dl
    log
    pthread
    prop
    rt
    apn
    rilproxyutils_static
    binder
    utils
    cutils
    vendor.mediatek.hardware.tbox-V1-ndk
    vendor.mediatek.hardware.voice-V1-ndk
    binder_ndk
    mtkrilutils
    mtkradioaidl
    mtk_audio_mixer_ctrl
    asound
    mtkhardwarelegacy
)
endif()


#link_directories(mtktelephonyservice PRIVATE ../librilproxyutils)

#install bin
install(TARGETS mtktelephonyservice
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})