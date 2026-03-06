/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2022. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#pragma once

#include <signal.h>
#include <telephony/mtk_ril.h>
#include <ril_internal.h>
#include <mtk_log.h>
#include <libmtkrilutils.h>
#include <mtk_properties.h>
#include <aidl/android/hardware/radio/RadioIndicationType.h>
#include <aidl/android/hardware/radio/RadioResponseInfo.h>

template <typename ... Ts>
void memsetAndFreeStrings(int /* numPointers */, Ts... all) {
    char* values[] = { all... };
    for (auto v: values) {
        if (v != NULL) {
#ifdef MEMSET_FREED
#define MAX_STRING_LENGTH 4096
            memset(v, 0, strnlen(v, MAX_STRING_LENGTH));
#endif
            free(v);
        }
    }
}

extern "C" bool dispatchVoid(int serial, int slotId, int request);
extern "C" bool dispatchInts(int serial, int slotId, int request, int countInts, ...);
extern "C" android::ClientId getClientBySlot(unsigned int slotId);
extern void sendErrorResponse(android::RequestInfo *pRI, RIL_Errno err);
extern int plusCounterRadioNumOne(unsigned int slotId);
extern int getCounterRadioNum(unsigned int slotId);
extern int plusCounterMtkRadioNumOne(unsigned int slotId);
extern int getCounterMtkRadioNum(unsigned int slotId);
extern char* clientTypeToString(android::ClientType type);
extern RIL_RadioFunctions *s_vendorFunctions;
extern std::string convertCharPtrToStdString(const char* str);
extern uint8_t * hexStringToBytes(const void *response, size_t responseLen);
extern bool isMtkFwkAddonNotExisted(unsigned int slotId);
typedef void (*timer_function)(sigval_t val);

#undef BOOL_TO_INT
#define BOOL_TO_INT(x) (x ? 1 : 0)
#if defined(ANDROID_MULTI_SIM)
#undef CALL_ONREQUEST
#define CALL_ONREQUEST(a, b, c, d, e) \
        s_vendorFunctions->onRequest((a), (b), (c), (d), ((RIL_SOCKET_ID)(e)))
#undef CALL_ONSTATEREQUEST
#define CALL_ONSTATEREQUEST(a) s_vendorFunctions->onStateRequest((RIL_SOCKET_ID)(a))
#else
#undef CALL_ONREQUEST
#define CALL_ONREQUEST(a, b, c, d, e) s_vendorFunctions->onRequest((a), (b), (c), (d))
#undef CALL_ONSTATEREQUEST
#define CALL_ONSTATEREQUEST(a) s_vendorFunctions->onStateRequest()
#endif

namespace rilAidlUtils {
    static const unsigned int DATA_INDEX = 0;
    static const unsigned int MESSAGE_INDEX = 1;
    static const unsigned int MODEM_INDEX = 2;
    static const unsigned int NETWORK_INDEX = 3;
    static const unsigned int SIM_INDEX = 4;
    static const unsigned int VOICE_INDEX = 5;
    static const unsigned int IMS_INDEX = 6;
    static const unsigned int ECALL_INDEX = 7;
    static const unsigned int MAX_INDEX = 8;
    static unsigned int sSetAospResNum[MAX_INDEX][52] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    static unsigned int sSetMtkResNum[MAX_INDEX][52] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    static std::string binderDiedError = "binder died";
    ::aidl::android::hardware::radio::RadioIndicationType convertIntToAidlRadioIndicationType(int indicationType);
    void populateAidlResponseInfo(::aidl::android::hardware::radio::RadioResponseInfo& responseInfo,
            int serial, int responseType, RIL_Errno e);
    int responseIntOrEmpty(::aidl::android::hardware::radio::RadioResponseInfo& responseInfo,
            int serial, int responseType, RIL_Errno e, const void *response, size_t responseLen);
    bool copyAidlStringToRil(char **dest, const std::string &src,
            android::RequestInfo *pRI, bool allowEmpty = false);
    bool dispatchAidlRaw(int serial, int slotId, int request, const std::vector<uint8_t>& rawBytes);
    bool dispatchString(int serial, int slotId, int request,
            const std::string& str);
    bool dispatchStrings(int serial, int slotId, int request, bool allowEmpty,
            int countStrings, ...);
    bool dispatchStrings(int serial, int slotId, int request,
                const std::vector<std::string>& data);
    void checkAospAidlReturnStatus(unsigned int slotId, bool ret);
    void checkMtkAidlReturnStatus(unsigned int slotId, bool ret, android::ClientType type);
    unsigned int setAospResponseNumberToOne(unsigned int index, int slotId);
    unsigned int checkIfSetAllAospResponseDone(int slotId);
    unsigned int cleanSetAospResNumToZero(int slotId);
    unsigned int setMtkResponseNumberToOne(unsigned int index, int slotId);
    unsigned int checkIfSetAllMtkResponseDone(int slotId);
    unsigned int cleanSetMtkResNumToZero(int slotId);
    void ril_service_init_watch_dog(timer_t* timer, timer_function cb);
    void ril_service_start_watch_dog(timer_t timer, int milliseconds);
    void ril_service_stop_watch_dog(timer_t timer);

    class IAidlReturnChecker {
    public:
        virtual ~IAidlReturnChecker() {}
        virtual void aospAidlReturnStatusChecker(unsigned int slotId, ndk::ScopedAStatus& ret) = 0;
        virtual void mtkAidlReturnStatusChecker(unsigned int slotId, ndk::ScopedAStatus& ret,
                android::ClientType type) = 0;
        virtual void radioConfigAidlReturnStatusChecker(unsigned int slotId,
                ndk::ScopedAStatus& ret) = 0;
    };

    class AidlReturnChecker: public IAidlReturnChecker {
    public:
        virtual ~AidlReturnChecker() {}
        void aospAidlReturnStatusChecker(unsigned int slotId, ndk::ScopedAStatus& ret) {
        const char* message = ret.getMessage();
        if ((((ret.getStatus() == STATUS_DEAD_OBJECT) || (ret.getStatus() == STATUS_NO_INIT)) && ret.getExceptionCode() == EX_TRANSACTION_FAILED) ||
            (ret.getExceptionCode() == EX_ILLEGAL_STATE && message != nullptr && std::string(message) == rilAidlUtils::binderDiedError)) {
            checkAospAidlReturnStatus(slotId, false);
        } else {
            mtkLogE(LOG_TAG, "aospAidlReturnStatusChecker binder fail(%d)(%d)(%d)(%s)", slotId, ret.getStatus(), ret.getExceptionCode(),
                    ret.getMessage());
            if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
              pid_t pid = getpid();
              char* str = NULL;
              int length = asprintf(&str, "%d", pid);
              if ((length != -1) && (str != NULL)) {
                mtk_property_set("vendor.ril.mux.report.case", str);
                mtk_property_set("vendor.ril.muxreport", "1");
                free(str);
              } else {
                mtkLogE(LOG_TAG, "aosp AidlReturnStatusChecker binder fail, modem reset");
                mtk_property_set("vendor.ril.mux.report.case", "2");
                mtk_property_set("vendor.ril.muxreport", "1");
              }
              mtkAssert((char*)"aospAidlReturnStatusChecker binder fail");
            }
        }
        }

        void mtkAidlReturnStatusChecker(unsigned int slotId, ndk::ScopedAStatus& ret,
                android::ClientType type) {
        const char* message = ret.getMessage();
        if ((((ret.getStatus() == STATUS_DEAD_OBJECT) || (ret.getStatus() == STATUS_NO_INIT)) && ret.getExceptionCode() == EX_TRANSACTION_FAILED) ||
            (ret.getExceptionCode() == EX_ILLEGAL_STATE && message != nullptr && std::string(message) == rilAidlUtils::binderDiedError)) {
            checkMtkAidlReturnStatus(slotId, false, type);
        } else {
            mtkLogE(LOG_TAG, "mtkAidlReturnStatusChecker binder fail(%d)(%d)(%d)(%s)", slotId, ret.getStatus(), ret.getExceptionCode(),
                    ret.getMessage());
            if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
              pid_t pid = getpid();
              char* str = NULL;
              int length = asprintf(&str, "%d", pid);
              if ((length != -1) && (str != NULL)) {
                mtk_property_set("vendor.ril.mux.report.case", str);
                mtk_property_set("vendor.ril.muxreport", "1");
                free(str);
              } else {
                mtkLogE(LOG_TAG, "mtk mtkAidlReturnStatusChecker binder fail, modem reset");
                mtk_property_set("vendor.ril.mux.report.case", "2");
                mtk_property_set("vendor.ril.muxreport", "1");
              }
              mtkAssert((char*)"mtkAidlReturnStatusChecker binder fail");
            }
        }
        }

        void radioConfigAidlReturnStatusChecker(unsigned int slotId,
                ndk::ScopedAStatus& ret);
    };

    extern IAidlReturnChecker *retChecker;
}

