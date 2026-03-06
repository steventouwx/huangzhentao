LOCAL_SRC_FILES += \
    platform/atcid_adaptation.cpp \
    platform/atcid_cust_cmd_platform.c \
    platform/atcid_mipc.c

LOCAL_SHARED_LIBRARIES += \
    libnvram \
    libfile_op \
    libbinder_ndk \
    vendor.mediatek.hardware.atci-V1-ndk

ifeq ($(MTK_RIL_MODE), c6m_1rild)
    LOCAL_SHARED_LIBRARIES += libmipc libtrm
    LOCAL_CFLAGS += -DMTK_ATCI_MIPC
endif

ifeq ($(MTK_WLAN_SUPPORT),yes)
    LOCAL_SHARED_LIBRARIES += libwifitest
    LOCAL_CFLAGS += -DMTK_WLAN_FEATURE
endif

ifeq ($(MTK_GPS_SUPPORT),yes)
    LOCAL_CFLAGS += -DMTK_GPS_FEATURE
endif

LOCAL_CFLAGS += -DTRM_SUPPORT


ifdef MTK_THIN_MD_TELEPHONY_SUPPORT_MODE
    ifneq ($(MTK_THIN_MD_TELEPHONY_SUPPORT_MODE), 0)
        LOCAL_CFLAGS += -DMTK_ATCI_THIN_MD_SUPPORT
        LOCAL_CFLAGS += -DMTK_RIL_THIN_MD_SUPPORT
    endif
endif

LOCAL_C_INCLUDES += \
        $(MTK_PATH_SOURCE)/hardware/connectivity/wlan_rf/wifi_test_tool/libwifitest/inc \
        $(MTK_PATH_SOURCE)/external/nvram/libnvram \
        $(MTK_PATH_SOURCE)/external/nvram/libfile_op \
        $(MTK_PATH_SOURCE)/frameworks/opt/atcid/atci/platform
