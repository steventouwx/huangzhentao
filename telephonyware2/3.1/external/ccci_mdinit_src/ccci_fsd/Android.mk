LOCAL_EXPORT_C_INCLUDE_DIRS := \
	$(MTK_PATH_SOURCE)/hardware/ccci/include \
	$(MTK_PATH_SOURCE)/external/libstorage_otp

###liblog for andorid log, libcutils for property_get, opt for opt
LOCAL_SHARED_LIBRARIES += liblog libnvram libcutils libstorage_otp

LOCAL_SRC_FILES += ccci_fsd/ccci_fsd.c ccci_fsd/platform/fsd_platform.c ccci_fsd/platform/power.c