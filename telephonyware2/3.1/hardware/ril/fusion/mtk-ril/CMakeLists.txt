cmake_minimum_required(VERSION 2.6)
project(mtkmipc-ril)

#add complie options
set(CMAKE_COMMON_FLAGS " -D__unused= -Wall -DRIL_SHLIB -D_GNU_SOURCE -DANDROID_MULTI_SIM \
    -DMTK_TELEPHONYWARE_SUPPORT -DMTK_MUX_CHANNEL_64 \
    -D__IMCF_MTK_VA__=1 -D__IMCF_NO_UA__=1 -DMTK_IMS_CHANNEL_SUPPORT -DTRM_SUPPORT -DTBOX -DTBOX_ADPTER")
    #-DTBOX -DTBOX_ADPTER

if(NOT DEFINED MTK_HYP_DUAL_TELE_SUPPORT)
    message(FATAL_ERROR "MTK_HYP_DUAL_TELE_SUPPORT is not defined")
endif()


if("${MTK_HYP_DUAL_TELE_SUPPORT}" STREQUAL "yes")
    set(CMAKE_COMMON_FLAGS "${CMAKE_COMMON_FLAGS} -DMTK_HYP_DUAL_TELE_SUPPORT")
    message(STATUS "MTK_HYP_DUAL_TELE_SUPPORT is set to yes")
endif()

if(TELEMATICS)
set(CMAKE_COMMON_FLAGS "${CMAKE_COMMON_FLAGS} -D__TELEMATICS__ ")
endif()
if(CMCC_NSIOT)
set(CMAKE_COMMON_FLAGS "${CMAKE_COMMON_FLAGS} -DCMCC_NSIOT_SUPPORT")
endif()

#if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../../../operator/hardware/ril/fusion/CMakeLists.txt")
#set(CMAKE_COMMON_FLAGS "${CMAKE_COMMON_FLAGS} -DMTK_OPERATOR_ADDON ")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../../operator/hardware/ril/fusion/mtk-ril/framework/include")
#endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_COMMON_FLAGS} -std=c99 -Wl,--no-undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_COMMON_FLAGS} -std=gnu++17 -Wl,--no-undefined")

#add header files (-I -L)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../libcarrierconfig")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/android/nanopb-c")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/ims/volte_header/volte_core/public_header")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/ims/volte_header/volte_imcb/sap")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../libratconfig/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/property/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/config/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/tinyxml/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/mtkrilutils/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/netutils")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../platformlib/include/log/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../libmipc/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../libmipc/include/api")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../libmipc/include/common")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../libmipc/include/msg")
include_directories("${FUSION_RIL_INCDIR}/mipcnetagent/")
include_directories("${FUSION_RIL_INCDIR}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/framework/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/framework/include/base")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/framework/include/core")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/framework/port/android/include")

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore_mipc")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore_mipc/client")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore_mipc/wp")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore_mipc/data")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore_mipc/nw")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore_mipc/cc")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore_mipc/ecall")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore_mipc/oem")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/telcore_mipc/capabilityswitch")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mdcomm_mipc/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mdcomm_mipc/capabilityswitch")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mdcomm_mipc/wp")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mdcomm_mipc/gwsd/include/gwsd")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mdcomm_mipc/ss")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mdcomm_mipc/gba")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mdcomm_mipc/sim")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mdcomm_mipc/dch")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mdcomm_mipc/ecall")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/adapter/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/adapter/common/")
if(TELEMATICS)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/adapter/telematics/")
endif()


#add source file
set(src_fils
    framework/port/android/rfx_gt_log.cpp
    framework/port/android/rfx_properties.cpp
    framework/core/RfxDataCloneManager.cpp
    framework/core/RfxReader.cpp
    framework/core/RfxTimer.cpp
    framework/core/RfxStatusManager.cpp
    framework/core/RfxAsyncSignal.cpp
    framework/core/RfxClassInfo.cpp
    framework/core/RfxController.cpp
    framework/core/Rfx.cpp
    framework/core/RfxChannelManager.cpp
    framework/core/RfxChannel.cpp
    framework/core/RfxRilAdapter.cpp
    framework/core/RfxFragmentEncoder.cpp
    framework/core/RfxControllerFactory.cpp
    framework/core/RfxSlotRootController.cpp
    framework/core/RfxMclDispatcherThread.cpp
    framework/core/RfxXmlParser.cpp
    framework/core/RfxMainThread.cpp
    framework/core/RfxAtResponse.cpp
    framework/core/RfxChannelContext.cpp
    framework/core/RfxMclStatusManager.cpp
    framework/core/RfxDispatchThread.cpp
    framework/core/RfxSender.cpp
    framework/core/RfxAction.cpp
    framework/core/RfxObject.cpp
    framework/core/RfxHandlerManager.cpp
    framework/core/RfxRootController.cpp
    framework/core/RfxBaseHandler.cpp
    framework/core/RfxSignal.cpp
    framework/core/RfxAtLine.cpp
    framework/base/RfxAuthMsgData.cpp
    framework/base/RfxSmsWriteData.cpp
    framework/base/RfxSimMeLockCatData.cpp
    framework/base/RfxSlicingConfigResponseData.cpp
    framework/base/RfxSetDataProfileData.cpp
    framework/base/RfxDchImsConfigData.cpp
    framework/base/RfxRsuResponseData.cpp
    framework/base/RfxDataRegStateData.cpp
    framework/base/RfxSimRefreshData.cpp
    framework/base/RfxVariant.cpp
    framework/base/RfxCellInfoData.cpp
    framework/base/RfxCrssNotificationData.cpp
    framework/base/RfxSuppServNotificationData.cpp
    framework/base/RfxMisc.cpp
    framework/base/RfxHardwareConfigData.cpp
    framework/base/RfxRuimSmsWriteData.cpp
    framework/base/RfxDchXmlConfigData.cpp
    framework/base/RfxPhoneCapabilityData.cpp
    framework/base/RfxDchConfigData.cpp
    framework/base/RfxSimAuthData.cpp
    framework/base/RfxIaApnData.cpp
    framework/base/RfxSmsParamsData.cpp
    framework/base/RfxLceStatusResponseData.cpp
    framework/base/RfxQueryCapData.cpp
    framework/base/RfxDchDataPathData.cpp
    framework/base/RfxPcoIaData.cpp
    framework/base/RfxSimOpenChannelData.cpp
    framework/base/RfxDialog.cpp
    framework/base/RfxDebugInfo.cpp
    framework/base/RfxEcallIndicationData.cpp
    framework/base/RfxImsDataInfoNotifyData.cpp
    framework/base/RfxStringsData.cpp
    framework/base/RfxSmsSimMemStatusCnfData.cpp
    framework/base/RfxImsConfData.cpp
    framework/base/RfxCertResponseData.cpp
    framework/base/RfxSimApduData.cpp
    framework/base/RfxSimIoRspData.cpp
    framework/base/RfxCdmaCbSmsCfgData.cpp
    framework/base/RfxPhbEntryData.cpp
    framework/base/RfxCidData.cpp
    framework/base/RfxCallForwardInfosExData.cpp
    framework/base/RfxPcoData.cpp
    framework/base/RfxDchConfigList.cpp
    framework/base/RfxEcallMsdData.cpp
    framework/base/RfxPhbEntriesExtData.cpp
    framework/base/RfxRilUtils.cpp
    framework/base/RfxDialogInfoData.cpp
    framework/base/RfxSimStatusData.cpp
    framework/base/RfxNetworkScanResultData.cpp
    framework/base/RfxEregrtData.cpp
    framework/base/RfxSimCarrRstData.cpp
    framework/base/RfxCallListData.cpp
    framework/base/RfxSignalStrengthData.cpp
    framework/base/RfxStringData.cpp
    framework/base/RfxImsBearerNotifyData.cpp
    framework/base/RfxSimSlotStatusData.cpp
    framework/base/RfxPlmnMvnoData.cpp
    framework/base/RfxDchConfigLocalList.cpp
    framework/base/RfxSetupDataCallData.cpp
    framework/base/RfxCdmaInfoRecData.cpp
    framework/base/RfxDialData.cpp
    framework/base/RfxAuthResponseData.cpp
    framework/base/RfxPhbEntryExtData.cpp
    framework/base/RfxVersionManager.cpp
    framework/base/RfxSimTypeInfoData.cpp
    framework/base/RfxCdmaWaitingCallData.cpp
    framework/base/RfxVsimUtils.cpp
    framework/base/RfxSimIoData.cpp
    framework/base/RfxLinkCapacityReportingCriteriaData.cpp
    framework/base/RfxPcoInfo.cpp
    framework/base/RfxViaUtils.cpp
    framework/base/RfxCapabilityData.cpp
    framework/base/RfxBarringCallInfosData.cpp
    framework/base/RfxCallState.cpp
    framework/base/RfxGsmCbSmsCfgData.cpp
    framework/base/RfxVtSendMsgData.cpp
    framework/base/RfxMipcData.cpp
    framework/base/RfxRadioCapabilityData.cpp
    framework/base/RfxFdModeData.cpp
    framework/base/RfxLceDataResponseData.cpp
    framework/base/RfxMessage.cpp
    framework/base/RfxEcallData.cpp
    framework/base/RfxBarringInfoData.cpp
    framework/base/RfxImsRegInfoData.cpp
    framework/base/RfxCdmaSmsAckData.cpp
    framework/base/RfxEmergencyNumberListData.cpp
    framework/base/RfxDefaultValueTable.cpp
    framework/base/RfxEtwsNotiData.cpp
    framework/base/RfxNetworkScanData.cpp
    framework/base/RfxIdToStringUtils.cpp
    framework/base/RfxDataCallResponseData.cpp
    framework/base/RfxMclMessage.cpp
    framework/base/RfxVoidData.cpp
    framework/base/RfxNwServiceState.cpp
    framework/base/RfxLinkCapacityEstimateData.cpp
    framework/base/RfxVoiceRegStateData.cpp
    framework/base/RfxEiregData.cpp
    framework/base/RfxEsvcData.cpp
    framework/base/RfxKeepaliveRequestData.cpp
    framework/base/RfxCallForwardInfoData.cpp
    framework/base/RfxKeepaliveStatusData.cpp
    framework/base/RfxNaCbInfoData.cpp
    framework/base/RfxSsrcData.cpp
    framework/base/RfxCallForwardInfoExData.cpp
    framework/base/RfxEmergencyDialData.cpp
    framework/base/RfxCallForwardInfosData.cpp
    framework/base/RfxActivityData.cpp
    framework/base/RfxRegistrationFailedData.cpp
    framework/base/RfxRcsSendMsgData.cpp
    framework/base/RfxPhbEntriesData.cpp
    framework/base/RfxCertMsgData.cpp
    framework/base/RfxRsuRequestData.cpp
    framework/base/RfxCdmaSmsMsgData.cpp
    framework/base/RfxEcholocateResponseData.cpp
    framework/base/RfxDchCloseSipData.cpp
    framework/base/RfxNetlinkIpv6InfoData.cpp
    framework/base/RfxVsimEventData.cpp
    framework/base/RfxCallFailCauseData.cpp
    framework/base/RfxRawData.cpp
    framework/base/RfxMipcUtils.cpp
    framework/base/RfxSscData.cpp
    framework/base/RfxVtCallStatusData.cpp
    framework/base/RfxImsCallInfo.cpp
    framework/base/RfxSimGenAuthData.cpp
    framework/base/RfxTokUtils.cpp
    framework/base/RfxVsimOpEventData.cpp
    framework/base/RfxQualifiedNetworkInfo.cpp
    framework/base/RfxSipRegInfoData.cpp
    framework/base/RfxVsimOpIndicationData.cpp
    framework/base/RfxRedialData.cpp
    framework/base/RfxPreciseCallState.cpp
    framework/base/RfxIntsData.cpp
    framework/base/RfxSmsRspData.cpp
    framework/base/RfxIdToMsgIdUtils.cpp
    framework/base/RfxImsSmsData.cpp
    framework/base/RfxOpUtils.cpp
    framework/base/RfxEcallSetNumData.cpp
    framework/base/RfxPhbMemStorageData.cpp
    telcore_mipc/vt/RtmVtController.cpp
    telcore_mipc/wp/RtmWpController.cpp
    telcore_mipc/cat/RtmCatController.cpp
    telcore_mipc/data/RtmDataController.cpp
    telcore_mipc/atci/RtmAtciController.cpp
    telcore_mipc/carrierconfig/RtmCarrierConfigController.cpp
    telcore_mipc/nw/RtmNwController.cpp
    telcore_mipc/mwi/RtmMobileWifiController.cpp
    telcore_mipc/ims/RtmImsDialogHandler.cpp
    telcore_mipc/ims/RtmImsConferenceController.cpp
    telcore_mipc/ims/RtmImsRttController.cpp
    telcore_mipc/ims/RtmImsController.cpp
    telcore_mipc/ims/config/ImsConfigUtils.cpp
    telcore_mipc/ims/config/ImsConfigDataHelper.cpp
    telcore_mipc/ims/config/RtmImsConfigController.cpp
    telcore_mipc/ss/RtmSuppServController.cpp
    telcore_mipc/ss/RtmSuppServUtil.cpp
    telcore_mipc/ss/RtmSuppServQueue.cpp
    telcore_mipc/phb/RtmPhbController.cpp
    telcore_mipc/phb/RtmPhbSimIoController.cpp
    telcore_mipc/phb/RtmPhbInModemController.cpp
    telcore_mipc/el/RtmElController.cpp
    telcore_mipc/sms/RtmGsmSmsController.cpp
    telcore_mipc/sms/RtmImsSmsController.cpp
    telcore_mipc/sms/RtmSmsMessage.cpp
    telcore_mipc/sms/RtmCdmaSmsController.cpp
    telcore_mipc/sms/RtmSmsUtil.cpp
    telcore_mipc/sms/RtmSmsHeader.cpp
    telcore_mipc/sms/RtmSmsNSlotController.cpp
    telcore_mipc/sms/RtmConcatSms.cpp
    telcore_mipc/sms/RtmSmsThread.cpp
    telcore_mipc/sms/RtmSCBMController.cpp
    telcore_mipc/sms/RtmSmsAddress.cpp
    telcore_mipc/sms/parser/SmsAddress.cpp
    telcore_mipc/sms/parser/SuplMsgDispatcher.cpp
    telcore_mipc/sms/parser/GsmSmsAddress.cpp
    telcore_mipc/sms/parser/PduParser.cpp
    telcore_mipc/sms/parser/SmsParserUtils.cpp
    telcore_mipc/sms/parser/SmsMessage.cpp
    telcore_mipc/sms/parser/ConcentratedSms.cpp
    telcore_mipc/sms/parser/SmsHeader.cpp
    telcore_mipc/rcs/RtmRcsController.cpp
    telcore_mipc/power/RtmModemController.cpp
    telcore_mipc/power/RtmRadioController.cpp
    telcore_mipc/sim/RtmCommSimRadioConfigController.cpp
    telcore_mipc/sim/RtmCommSimController.cpp
    telcore_mipc/capabilityswitch/RtmCapabilitySwitchChecker.cpp
    telcore_mipc/capabilityswitch/RtmCapabilityGetController.cpp
    telcore_mipc/capabilityswitch/RtmCapabilitySwitchController.cpp
    telcore_mipc/capabilityswitch/RtmCapabilitySwitchUtil.cpp
    telcore_mipc/cc/EccNumberPreference.cpp
    telcore_mipc/cc/EccNumberSource.cpp
    telcore_mipc/cc/RtmEccNumberController.cpp
    telcore_mipc/cc/RtmCallController.cpp
    telcore_mipc/ecall/RtmEcallController.cpp
    telcore_mipc/cc/RtmEccController.cpp
    telcore_mipc/vsim/RtmVsimController.cpp
    telcore_mipc/utils/RfxMipcControllerUtils.cpp
    telcore_mipc/gwsd/RtmGwsdController.cpp
    telcore_mipc/client/RilMttsClient.cpp
    telcore_mipc/client/RtcRilClientController.cpp
    telcore_mipc/client/RilClientQueue.cpp
    telcore_mipc/client/RilOemClient.cpp
    telcore_mipc/client/RilCommandsTable.cpp
    telcore_mipc/client/RilAtciClient.cpp
    telcore_mipc/client/RilParcelUtils.cpp
    telcore_mipc/client/RilClient.cpp
    telcore_mipc/sbp/RtmSbpController.cpp
    telcore_mipc/oem/RtmOemController.cpp
    mdcomm_mipc/oem/RmmOemRequestHandler.cpp
    mdcomm_mipc/vt/RmmVtMsgParser.cpp
    mdcomm_mipc/vt/RmmVtUrcHandler.cpp
    mdcomm_mipc/vt/RmmVtReqHandler.cpp
    mdcomm_mipc/vt/RmmVtCommonReqHandler.cpp
    mdcomm_mipc/vt/RmmVtDataThreadController.cpp
    mdcomm_mipc/wp/RmmWpRequestHandler.cpp
    mdcomm_mipc/wp/RmmWpModemRequestHandler.cpp
    mdcomm_mipc/cat/RmmCatCommonRequestHandler.cpp
    mdcomm_mipc/cat/RmmCatUrcHandler.cpp
    mdcomm_mipc/data/RmmDcUrcHandler.cpp
    mdcomm_mipc/data/RmmDcPdnManager.cpp
    mdcomm_mipc/data/RmmDcRequestHandler.cpp
    mdcomm_mipc/data/RmmDcUtility.cpp
    mdcomm_mipc/data/RmmDcEventHandler.cpp
    mdcomm_mipc/data/RmmDcImsURCHandler.cpp
    mdcomm_mipc/atci/RmmAtciRequestHandler.cpp
    mdcomm_mipc/atci/RmmAtciCommonRequestHandler.cpp
    mdcomm_mipc/atci/RmmAtciSpecialRequestHandler.cpp
    mdcomm_mipc/dch/RmmDchUrcHandler.cpp
    mdcomm_mipc/dch/RmmDchRequestHandler.cpp
    mdcomm_mipc/nw/RmmNwNrtRequestHandler.cpp
    mdcomm_mipc/nw/RmmNwRealTimeRequestHandler.cpp
    mdcomm_mipc/nw/RmmNwRatSwitchHandler.cpp
    mdcomm_mipc/nw/RmmNwAsyncRequestHandler.cpp
    mdcomm_mipc/nw/RmmNwHandler.cpp
    mdcomm_mipc/nw/RmmNwURCHandler.cpp
    mdcomm_mipc/nw/RmmNwRequestHandler.cpp
    mdcomm_mipc/mwi/RmmMobileWifiRequestHandler.cpp
    mdcomm_mipc/mwi/RmmMobileWifiUrcHandler.cpp
    mdcomm_mipc/ims/RmmImsControlUrcHandler.cpp
    mdcomm_mipc/ims/RmmImsVideoRingtoneRequestHandler.cpp
    mdcomm_mipc/ims/RmmImsVideoRingtoneUrcHandler.cpp
    mdcomm_mipc/ims/RmmImsUtil.cpp
    mdcomm_mipc/ims/RmmImsProvisioningRequestHandler.cpp
    mdcomm_mipc/ims/RmmImsRttControlUrcHandler.cpp
    mdcomm_mipc/ims/RmmImsControlRequestHandler.cpp
    mdcomm_mipc/ims/RmmImsCustomizedUtils.cpp
    mdcomm_mipc/ims/RmmImsRttControlRequestHandler.cpp
    mdcomm_mipc/ss/SSUtil.cpp
    mdcomm_mipc/ss/RmmSuppServRequestHandler.cpp
    mdcomm_mipc/ss/RmmSuppServUrcEntryHandler.cpp
    mdcomm_mipc/ss/RmmSuppServRequestBaseHandler.cpp
    mdcomm_mipc/ss/RmmSuppServRequestNRTHandler.cpp
    mdcomm_mipc/phb/RmmPhbURCHandler.cpp
    mdcomm_mipc/phb/RmmPhbSimIoRequestHandler.cpp
    mdcomm_mipc/phb/RmmPhbInModemHandler.cpp
    mdcomm_mipc/phb/RmmPhbRequestHandler.cpp
    mdcomm_mipc/common/RmmData.cpp
    mdcomm_mipc/common/RmmMessageHandler.cpp
    mdcomm_mipc/common/RmmChannelHandler.cpp
    mdcomm_mipc/el/RmmElRequestHandler.cpp
    mdcomm_mipc/sms/RmmSCBMRequestHandler.cpp
    mdcomm_mipc/sms/RmmCdmaSmsHandler.cpp
    mdcomm_mipc/sms/RmmCommSmsUrcHandler.cpp
    mdcomm_mipc/sms/RmmCommSmsRequestHandler.cpp
    mdcomm_mipc/sms/RmmBcRangeParser.cpp
    mdcomm_mipc/sms/RmmGsmSmsRequestHandler.cpp
    mdcomm_mipc/sms/RmmGsmSmsBaseHandler.cpp
    mdcomm_mipc/sms/RmmSCBMUrcHandler.cpp
    mdcomm_mipc/sms/RmmCdmaSmsConverter.cpp
    mdcomm_mipc/sms/RmmGsmSmsUrcHandler.cpp
    mdcomm_mipc/rcs/RcsStackChannel.cpp
    mdcomm_mipc/rcs/RcsCapabilityRule.cpp
    mdcomm_mipc/rcs/RmmRcsReqHandler.cpp
    mdcomm_mipc/power/RmmModemRequestHandler.cpp
    mdcomm_mipc/power/RmmRadioRequestHandler.cpp
    mdcomm_mipc/sim/base64.cpp
    mdcomm_mipc/sim/RmmSimCommUrcHandler.cpp
    mdcomm_mipc/sim/RmmSmlUrcHandler.cpp
    mdcomm_mipc/sim/RmmSmlRequestHandler.cpp
    mdcomm_mipc/sim/RmmSimBaseHandler.cpp
    mdcomm_mipc/sim/RmmSimCommRequestHandler.cpp
    mdcomm_mipc/capabilityswitch/RmmCapabilitySwitchUtil.cpp
    mdcomm_mipc/capabilityswitch/RmmCapabilitySwitchRequestHandler.cpp
    mdcomm_mipc/capabilityswitch/RmmCapabilitySwitchURCHandler.cpp
    mdcomm_mipc/cc/RmmCallControlCommonRequestHandler.cpp
    mdcomm_mipc/cc/RmmCallControlBaseHandler.cpp
    mdcomm_mipc/cc/RmmEccNumberURCHandler.cpp
    mdcomm_mipc/cc/RmmCallControlUrcHandler.cpp
    mdcomm_mipc/ecall/RmmEcallRequestHandler.cpp
    mdcomm_mipc/ecall/RmmEcallUrcHandler.cpp
    mdcomm_mipc/cc/RmmEccNumberRequestHandler.cpp
    mdcomm_mipc/vsim/RmmVsimUrcHandler.cpp
    mdcomm_mipc/vsim/RmmVsimRequestHandler.cpp
    mdcomm_mipc/gwsd/RfxGwsdUtils.cpp
    mdcomm_mipc/gwsd/RmmGwsdRequestHandler.cpp
    mdcomm_mipc/gba/RmmGbaRequestHandler.cpp
    mdcomm_mipc/oem/RmmOemUrcHandler.cpp
    mdcomm_mipc/oem/RmmHardwareConfigRequestHandler.cpp
    ril_callbacks.c
    mdcomm_mipc/sim/usim_fcp_parser.c)

#add adapter source file
set(src_files_adapter_base
        adapter/MtkRilAdapterFactory.cpp
        adapter/common/MtkRilAdapter.cpp
        adapter/telematics/TelematicsController.cpp
        adapter/telematics/TelematicsMtkRilAdapter.cpp)

set(src_fils ${src_fils} ${src_files_adapter_base})
#file(GLOB src_files_adapter_base "${CMAKE_CURRENT_SOURCE_DIR}/adapter/*.cpp")
#file(GLOB src_files_adapter_common "${CMAKE_CURRENT_SOURCE_DIR}/adapter/common/*.cpp")
#if(TELEMATICS)
#file(GLOB src_files_adapter_telematics "${CMAKE_CURRENT_SOURCE_DIR}/adapter/telematics/*.cpp")
#endif()

#set(src_fils ${src_fils} ${src_files_adapter_base} ${src_files_adapter_common})
#if(TELEMATICS)
#set(src_fils ${src_fils} ${src_files_adapter_telematics})
#endif()

# Collect all files in the directory
#file(GLOB_RECURSE FRAMEWORK_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*")

# Exclude specific files
#list(REMOVE_ITEM all_files 
#    ${CMAKE_CURRENT_SOURCE_DIR}/excluded_file1.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/excluded_folder/excluded_file2.cpp
#)

# Now all_files contains all files except the ones specified above

add_library(mtkmipc-ril SHARED ${src_fils})

if(MTK_MBRAIN_SUPPORT STREQUAL "yes")
add_definitions(-DMTK_MBRAIN_SUPPORT)
set(link_libs rilfusion mtknetutils ratconfig mtksysutils mtkhardwarelegacy mtkrilutils crypto mtkproperty mtkrillog
    mtkconfigutils mtktinyxml protobuf-c-nano-enable-malloc mipc mipcnetagent dl rt utils cutils binder log binder_ndk mbrainAIDL jsoncpp)
else()
set(link_libs rilfusion mtknetutils ratconfig mtksysutils mtkhardwarelegacy mtkrilutils crypto mtkproperty mtkrillog
    mtkconfigutils mtktinyxml protobuf-c-nano-enable-malloc mipc mipcnetagent dl rt utils cutils binder log)
endif()

target_link_libraries(mtkmipc-ril ${link_libs})
#add_dependencies(mtk-ril rilfusion)

#install
install(TARGETS mtkmipc-ril
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

