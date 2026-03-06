# Copyright 2006 The Android Open Source Project

# XXX using libutils for simulator build only...
#

ifeq ($(MTK_RIL_MODE), c6m_1rild)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

FRAMEWORK_CPP := $(patsubst ./%,framework/%, $(shell cd $(LOCAL_PATH)/framework ; find $(1) -name "*.cpp" \
    -and -not -name "RfxFooData.cpp" \
))

TELCORE_CPP := $(patsubst ./%,telcore/%, $(shell cd $(LOCAL_PATH)/telcore ; find $(1) -name "*.cpp" \
))

MIPC_TELCORE_CPP := $(patsubst ./%,telcore_mipc/%, $(shell cd $(LOCAL_PATH)/telcore_mipc ; find $(1) -name "*.cpp" \
))

ifdef MTK_THIN_MD_TELEPHONY_SUPPORT_MODE
    ifneq ($(MTK_THIN_MD_TELEPHONY_SUPPORT_MODE), 0)
        ifeq ($(MTK_IMS_SUPPORT), no)
            MTK_THIN_MD_TELEPHONY_WITHOUT_IMS = 1
        endif
    endif
endif

ifdef MTK_THIN_MD_TELEPHONY_WITHOUT_IMS
    MDCOMM_CPP := $(patsubst ./%,mdcomm/%, $(shell cd $(LOCAL_PATH)/mdcomm ; find $(1) -name "*.cpp" \
        -and -not -name "RmcOemRequestHandler.cpp" \
        -and -not -name "RmcVtCommonReqHandler.cpp" \
        -and -not -name "RmcVtDataThreadController.cpp" \
        -and -not -name "RmcVtMsgParser.cpp" \
        -and -not -name "RmcVtReqHandler.cpp" \
        -and -not -name "RmcVtUrcHandler.cpp" \
        -and -not -name "RmcRcsReqHandler.cpp" \
    ))

    MIPC_MDCOMM_CPP := $(patsubst ./%,mdcomm_mipc/%, $(shell cd $(LOCAL_PATH)/mdcomm_mipc ; find $(1) -name "*.cpp" \
        -and -not -name "RmmOemRequestHandler.cpp" \
        -and -not -name "RmmVtCommonReqHandler.cpp" \
        -and -not -name "RmmVtDataThreadController.cpp" \
        -and -not -name "RmmVtMsgParser.cpp" \
        -and -not -name "RmmVtReqHandler.cpp" \
        -and -not -name "RmmVtUrcHandler.cpp" \
        -and -not -name "RmmRcsReqHandler.cpp" \
    ))
else
    MDCOMM_CPP := $(patsubst ./%,mdcomm/%, $(shell cd $(LOCAL_PATH)/mdcomm ; find $(1) -name "*.cpp" \
        -and -not -name "RmcOemRequestHandler.cpp" \
    ))

    MIPC_MDCOMM_CPP := $(patsubst ./%,mdcomm_mipc/%, $(shell cd $(LOCAL_PATH)/mdcomm_mipc ; find $(1) -name "*.cpp" \
        -and -not -name "RmmOemRequestHandler.cpp" \
    ))
endif

ifeq ($(MTK_SPM_TELEPHONY_NATIVE),yes)
SPM_ADAPTER_SRC_FILES := \
    adapter/MtkRilAdapterFactory.cpp \
    adapter/common/MtkRilAdapter.cpp \
    adapter/telematics/TelematicsMtkRilAdapter.cpp \
    adapter/telematics/TelematicsController.cpp \

SPM_ADAPTER_INC_FILES := \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/adapter/include \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/adapter/common \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/adapter/telematics \

else
SPM_ADAPTER_SRC_FILES :=

SPM_ADAPTER_INC_FILES :=

endif

AT_SRC_FILES:= \
    $(FRAMEWORK_CPP) \
    $(TELCORE_CPP) \
    mdcomm/oem/RmcOemRequestHandler.cpp \
    $(MDCOMM_CPP) \
    ril_callbacks.c \
    mdcomm/sim/usim_fcp_parser.c \
    $(SPM_ADAPTER_SRC_FILES) \


MIPC_SRC_FILES:= \
    $(FRAMEWORK_CPP) \
    $(MIPC_TELCORE_CPP) \
    mdcomm_mipc/oem/RmmOemRequestHandler.cpp \
    $(MIPC_MDCOMM_CPP) \
    ril_callbacks.c \
    mdcomm_mipc/sim/usim_fcp_parser.c \
    $(SPM_ADAPTER_SRC_FILES) \


RIL_LOCAL_SHARED_LIBRARIES := \
    libmtkutils librilfusion libmtknetutils libratconfig libmtksysutils libmtkhardware_legacy \
    libmtkrilutils libcrypto libmtkcutils libmtkproperty libmtkrillog \
    libmtkconfigutils libmtktinyxml \
    vendor.mediatek.hardware.netdagent@1.0 \
    libhidlbase \
    vendor.mediatek.hardware.netdagent-V1-ndk \
    libbinder_ndk \
    vendor.mediatek.hardware.mbrain-V6-ndk \
    libjsoncpp \

#when enable RFX_OBJ_DEBUG in RfxDefs.h
#RIL_LOCAL_SHARED_LIBRARIES += libladder

RIL_LOCAL_STATIC_LIBRARIES := \
    libprotobuf-c-nano-enable_malloc \

RIL_LOCAL_HEADER_LIBRARIES := netagent_headers libMtkLogHeaders \
    libUtilsHeaders mtk_ril_include_headers ims_headers libandroid_sensor_headers \
    libcarrierconfig_includes

# for asprinf
RIL_LOCAL_CFLAGS := -D_GNU_SOURCE

ifndef MTK_THIN_MD_TELEPHONY_SUPPORT_MODE
    RIL_LOCAL_HEADER_LIBRARIES += ccci_headers
else
    ifneq ($(MTK_THIN_MD_TELEPHONY_SUPPORT_MODE), 0)
        RIL_LOCAL_CFLAGS += -DMTK_RIL_THIN_MD_SUPPORT
    else
        RIL_LOCAL_HEADER_LIBRARIES += ccci_headers
    endif
endif

RIL_LOCAL_CFLAGS += -DMTK_SYSENV_SUPPORT

RIL_LOCAL_CFLAGS += -DMTK_MCIF_WIFI_SUPPORT

#used to check if support telephonyware
RIL_LOCAL_CFLAGS += -DMTK_TELEPHONYWARE_SUPPORT

#ifneq ($(MTK_NUM_MODEM_PROTOCOL),1)
    RIL_LOCAL_CFLAGS += -DANDROID_MULTI_SIM
#endif

#only for local retest
#RIL_LOCAL_CFLAGS += -DMTK_ONY_FOR_RILD_DEBUG

RIL_LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/framework/include \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/framework/include/base \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/framework/include/core \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/framework/port/android/include \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/ \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore/client \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore/oem/ \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore/capabilityswitch/ \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm/data/ \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm/ \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm/capabilityswitch \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore_mipc \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore_mipc/client \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/ \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/capabilityswitch \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore/wp \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm/wp \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm/ss \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm/gba \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore_mipc/wp \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/wp \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/ss \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/gba \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm/sim \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm/gwsd/include/gwsd \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/sim \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/gwsd/include/gwsd \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore_mipc/data \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore_mipc/nw \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore_mipc/cc \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore_mipc/ecall \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/data \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/nw \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/utils \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/property \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/log \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/mtkrilutils \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/config \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/platformlib/include/tinyxml \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore_mipc/oem/ \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/telcore_mipc/capabilityswitch/ \
    $(TOP)/$(MTK_PATH_SOURCE)/hardware/ril/fusion/mtk-ril/mdcomm_mipc/dch \
    $(TOP)/vendor/mediatek/proprietary/hardware/audio/common/include/ \
    $(SPM_ADAPTER_INC_FILES)

#when enable RFX_OBJ_DEBUG in RfxDefs.h
#RIL_LOCAL_C_INCLUDES += $(MTK_ROOT)/external/libudf/libladder/ \

ifneq ($(wildcard vendor/mediatek/proprietary/operator/hardware/ril/fusion/Android.mk),)
ifneq ($(MTK_SPM_TELEPHONY_NATIVE),yes)
    RIL_LOCAL_HEADER_LIBRARIES += libopril_includes
    RIL_LOCAL_CFLAGS += -DMTK_OPERATOR_ADDON
endif
endif

RIL_LOCAL_CFLAGS += -DMTK_MUX_CHANNEL_64
RIL_LOCAL_CFLAGS += -DMTK_IMS_CHANNEL_SUPPORT

ifeq ($(MTK_SPM_TELEPHONY_NATIVE),yes)
RIL_LOCAL_CFLAGS += -DTBOX
RIL_LOCAL_CFLAGS += -DTBOX_ADPTER
#RIL_LOCAL_CFLAGS += -DTBOX_ECALL
ifdef TBOX_ECALL
RIL_LOCAL_SHARED_LIBRARIES += libspeechdrv
endif
endif

RIL_LOCAL_SHARED_LIBRARIES += libbinder

#LOCAL_LDFLAGS := -Wl,--unresolved-symbols=ignore-all

RIL_LOCAL_STATIC_LIBRARIES := \
    libprotobuf-c-nano-enable_malloc \

# for vilte & imcb needed
RIL_LOCAL_CFLAGS += -D __IMCF_MTK_VA__=1 -D __IMCF_NO_UA__=1

# for ss & xcap needed
RIL_LOCAL_REQUIRED_MODULES := libutinterface_md

# External SIM
#ifeq ($(strip $(MTK_EXTERNAL_SIM_SUPPORT)), yes)
#    RIL_LOCAL_STATIC_LIBRARIES += libmtk-fusion-ril-prop-vsim
#    RIL_LOCAL_CFLAGS += -DMTK_EXTERNAL_SIM_SUPPORT
#else ifeq ($(strip $(RELEASE_BRM)), yes)
#    RIL_LOCAL_STATIC_LIBRARIES += libmtk-fusion-ril-prop-vsim
#endif

ifneq ($(filter address, $(strip $(SANITIZE_TARGET))),)
RIL_LOCAL_CFLAGS += -fno-sanitize-address-use-after-scope
endif

LOCAL_SRC_FILES := $(MIPC_SRC_FILES)

LOCAL_SHARED_LIBRARIES := $(RIL_LOCAL_SHARED_LIBRARIES)
LOCAL_SHARED_LIBRARIES += libmipc
LOCAL_STATIC_LIBRARIES := $(RIL_LOCAL_STATIC_LIBRARIES)
LOCAL_STATIC_LIBRARIES += libmipcnetagent
LOCAL_HEADER_LIBRARIES := $(RIL_LOCAL_HEADER_LIBRARIES)
LOCAL_CFLAGS:= $(RIL_LOCAL_CFLAGS)
LOCAL_C_INCLUDES := $(RIL_LOCAL_C_INCLUDES)
LOCAL_REQUIRED_MODULES := $(RIL_LOCAL_REQUIRED_MODULES)

ifeq (foo,foo)
  #build shared library
  LOCAL_CFLAGS += -DRIL_SHLIB
  LOCAL_CFLAGS += -DTRM_SUPPORT
  LOCAL_MODULE:= libmtkmipc-ril
  LOCAL_PROPRIETARY_MODULE := true
  LOCAL_MODULE_OWNER := mtk
  LOCAL_MULTILIB := first
ifeq ($(NATIVE_COVERAGE),true)
    LOCAL_CFLAGS += --coverage -g -O0 -fprofile-arcs -ftest-coverage
    LOCAL_LDFLAGS += --coverage -fprofile-arcs -ftest-coverage
endif
  include $(MTK_SHARED_LIBRARY)
else
#build executable
  LOCAL_SHARED_LIBRARIES += \
      libril
  LOCAL_MODULE:= libmtkmipc-ril
  LOCAL_PROPRIETARY_MODULE := true
  LOCAL_MODULE_OWNER := mtk
  LOCAL_MULTILIB := first
  include $(MTK_EXECUTABLE)
endif

#LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(AT_SRC_FILES)

LOCAL_SHARED_LIBRARIES := $(RIL_LOCAL_SHARED_LIBRARIES)
LOCAL_SHARED_LIBRARIES += libmipc
LOCAL_STATIC_LIBRARIES := $(RIL_LOCAL_STATIC_LIBRARIES)
LOCAL_STATIC_LIBRARIES += libnetagent
LOCAL_HEADER_LIBRARIES := $(RIL_LOCAL_HEADER_LIBRARIES)
LOCAL_CFLAGS:= $(RIL_LOCAL_CFLAGS)
LOCAL_C_INCLUDES := $(RIL_LOCAL_C_INCLUDES)
LOCAL_REQUIRED_MODULES := $(RIL_LOCAL_REQUIRED_MODULES)
RIL_LOCAL_DISABLE_NATIVE_COVERAGE := false

ifeq ($(NATIVE_COVERAGE),true)
    RIL_LOCAL_DISABLE_NATIVE_COVERAGE := true
    NATIVE_COVERAGE := false
endif

ifeq (foo,foo)
  #build shared library
  LOCAL_CFLAGS += -DRIL_SHLIB
  LOCAL_MODULE:= libmtk-ril
  LOCAL_PROPRIETARY_MODULE := true
  LOCAL_MODULE_OWNER := mtk
  LOCAL_MULTILIB := first
  include $(MTK_SHARED_LIBRARY)
else
#build executable
  LOCAL_SHARED_LIBRARIES += \
      libril
  LOCAL_MODULE:= libmtk-ril
  LOCAL_PROPRIETARY_MODULE := true
  LOCAL_MODULE_OWNER := mtk
  LOCAL_MULTILIB := first
  include $(MTK_EXECUTABLE)
endif

ifeq ($(RIL_LOCAL_DISABLE_NATIVE_COVERAGE),true)
    NATIVE_COVERAGE := true
endif


include $(CLEAR_VARS)
LOCAL_MODULE:=libmtkril_includes
LOCAL_EXPORT_C_INCLUDE_DIRS := $(RIL_LOCAL_C_INCLUDES)
include $(BUILD_HEADER_LIBRARY)

endif
