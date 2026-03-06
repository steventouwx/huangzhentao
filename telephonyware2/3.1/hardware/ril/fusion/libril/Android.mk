# Copyright 2006 The Android Open Source Project

ifeq ($(MTK_RIL_MODE), c6m_1rild)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_VENDOR_MODULE := true

LOCAL_SRC_FILES:= \
    ril.cpp \
    ril_event.cpp\
    RilSapSocket.cpp \
    ril_service.cpp \
    RilOpProxy.cpp \
    rilAidlUtils.cpp \
    ril_context.cpp \
    radioConfig_service.cpp \
    radiodata_service.cpp \
    radioims_service.cpp \
    radiomessaging_service.cpp \
    radiomodem_service.cpp \
    radionetwork_service.cpp \
    radiosim_service.cpp \
    radiosap_service.cpp \
    radiovoice_service.cpp \
    radiorcs_service.cpp \
    radiodch_service.cpp \
    radioecall_service.cpp

LOCAL_SHARED_LIBRARIES := \
    libmtkrillog \
    libmtkhardware_legacy \
    libmtkrilutils \
    libmtkproperty \
    libmtkconfigutils \
    vendor.mediatek.hardware.mbrain-V6-ndk \
    libjsoncpp \


LOCAL_STATIC_LIBRARIES := \
    libprotobuf-c-nano-enable_malloc \

#LOCAL_CFLAGS := -DDSDA_RILD1

LOCAL_CFLAGS += -Wno-unused-parameter

ifeq ($(NATIVE_COVERAGE),true)
    LOCAL_CFLAGS += --coverage -g -O0 -fprofile-arcs -ftest-coverage
    LOCAL_LDFLAGS += --coverage -fprofile-arcs -ftest-coverage
endif

#use android binder, HIDL
LOCAL_CFLAGS += -DMTK_USE_AIDL

ifeq ($(strip $(MTK_BASIC_PACKAGE)), yes)
LOCAL_CFLAGS += -DMTK_BASIC_PACKAGE
endif

LOCAL_SHARED_LIBRARIES += libbinder libhidlbase libutils
LOCAL_SHARED_LIBRARIES += libbinder_ndk
LOCAL_SHARED_LIBRARIES += android.hardware.radio.data-V3-ndk
LOCAL_SHARED_LIBRARIES += android.hardware.radio.ims-V2-ndk
LOCAL_SHARED_LIBRARIES += android.hardware.radio.messaging-V3-ndk
LOCAL_SHARED_LIBRARIES += android.hardware.radio.modem-V3-ndk
LOCAL_SHARED_LIBRARIES += android.hardware.radio.network-V3-ndk
LOCAL_SHARED_LIBRARIES += android.hardware.radio.sim-V3-ndk
LOCAL_SHARED_LIBRARIES += android.hardware.radio.sap-V1-ndk
LOCAL_SHARED_LIBRARIES += android.hardware.radio.voice-V3-ndk
LOCAL_SHARED_LIBRARIES += android.hardware.radio.config-V3-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.modem-V3-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.data-V3-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.voice-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.sim-V2-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.network-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.messaging-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.ims-V2-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.assist-V2-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.atci-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.cap-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.em-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.mwi-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.rsu-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.se-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.smartratswitch-V1-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.rcs-V2-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.dch-V2-ndk
LOCAL_SHARED_LIBRARIES += vendor.mediatek.hardware.mtkradioex.ecall-V1-ndk

#used to check if support telephonyware
LOCAL_CFLAGS += -DMTK_TELEPHONYWARE_SUPPORT

#ifneq ($(MTK_NUM_MODEM_PROTOCOL),1)
    LOCAL_CFLAGS += -DANDROID_MULTI_SIM
#endif

LOCAL_CFLAGS += -DMTK_MUX_CHANNEL_64
LOCAL_CFLAGS += -DMTK_IMS_CHANNEL_SUPPORT

LOCAL_C_INCLUDES += \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/log \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/property \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/mtkrilutils \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/hardware_legacy \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/config \

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH) $(LOCAL_PATH)/../../include

LOCAL_MODULE:= librilfusion
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_CLANG := true
LOCAL_SANITIZE := integer

LOCAL_MULTILIB := first

include $(MTK_SHARED_LIBRARY)


endif
