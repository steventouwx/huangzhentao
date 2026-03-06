/*
 * Copyright (c) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "RILC"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <ril_service.h>
#include <utils/SystemClock.h>
#include <inttypes.h>
#include <libmtkrilutils.h>
#include <mtk_log.h>
#include <mtk_properties.h>
#include <mtkconfigutils.h>
#include <ril_internal.h>
#include <RfxRilDumpInfo.h>
#include <cstdint>

#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include "utils/Vector.h"

#define INVALID_HEX_CHAR 16

using android::CommandInfo;
using android::RequestInfo;
using android::requestToString;
using android::sp;
using ::android::Vector;


#define BOOL_TO_INT(x) (x ? 1 : 0)
#define ATOI_NULL_HANDLED(x) (x ? atoi(x) : -1)
#define ATOI_NULL_HANDLED_DEF(x, defaultVal) (x ? atoi(x) : defaultVal)

#if defined(ANDROID_MULTI_SIM)
#define CALL_ONREQUEST(a, b, c, d, e) \
        s_vendorFunctions->onRequest((a), (b), (c), (d), ((RIL_SOCKET_ID)(e)))
#define CALL_ONSTATEREQUEST(a) s_vendorFunctions->onStateRequest((RIL_SOCKET_ID)(a))
#else
#define CALL_ONREQUEST(a, b, c, d, e) s_vendorFunctions->onRequest((a), (b), (c), (d))
#define CALL_ONSTATEREQUEST(a) s_vendorFunctions->onStateRequest()
#endif

RIL_RadioFunctions *s_vendorFunctions = NULL;
static CommandInfo *s_commands;


void appendRequestIntoPendingInd(unsigned int slot, int request, int responseType, const void* data,
        int size);
int64_t nitzTimeReceived[MAX_SIM_COUNT * android::CLIENT_COUNT];
// counter used for synchronization. It is incremented every time response callbacks are updated.
volatile int32_t mCounterRadio[MAX_SIM_COUNT * android::CLIENT_COUNT];
volatile int32_t mCounterMtkRadioEx[MAX_SIM_COUNT * android::CLIENT_COUNT];

// To Compute IMS Slot Id
extern "C" unsigned int toRealSlot(unsigned int slotId);
extern "C" unsigned int toClientSlot(unsigned int slotId, android::ClientId clientId);
extern "C" android::ClientId getClientBySlot(unsigned int slotId);

static pthread_rwlock_t radioServiceRwlocks[] = { PTHREAD_RWLOCK_INITIALIZER,
                                                  PTHREAD_RWLOCK_INITIALIZER,
                                                  PTHREAD_RWLOCK_INITIALIZER,
                                                  PTHREAD_RWLOCK_INITIALIZER};

static Vector<pthread_t> readLockOwnerList[MAX_SIM_COUNT];
static pthread_mutex_t readLockOwnerListMutex[MAX_SIM_COUNT] = {PTHREAD_MUTEX_INITIALIZER,
                                                                PTHREAD_MUTEX_INITIALIZER,
                                                                PTHREAD_MUTEX_INITIALIZER,
                                                                PTHREAD_MUTEX_INITIALIZER};
static Vector<pthread_t> writeLockOwnerList[MAX_SIM_COUNT];
static pthread_mutex_t writeLockOwnerListMutex[MAX_SIM_COUNT] = {PTHREAD_MUTEX_INITIALIZER,
                                                                PTHREAD_MUTEX_INITIALIZER,
                                                                PTHREAD_MUTEX_INITIALIZER,
                                                                PTHREAD_MUTEX_INITIALIZER};


// M: [Inactive Timer] start
int decodeInactiveTimer(unsigned int authType);
// M: [Inactive Timer] end

bool isMtkFwkAddonNotExisted(unsigned int slotId);

long stringToLong(char *str, int base, int *err);

int plusCounterRadioNumOne(unsigned int slotId) {
    mCounterRadio[slotId]++;
    return mCounterRadio[slotId];
}

int getCounterRadioNum(unsigned int slotId) {
    return mCounterRadio[slotId];
}

int plusCounterMtkRadioNumOne(unsigned int slotId) {
    mCounterMtkRadioEx[slotId]++;
    return mCounterMtkRadioEx[slotId];
}

int getCounterMtkRadioNum(unsigned int slotId) {
    return mCounterMtkRadioEx[slotId];
}

uint8_t hexCharToInt(uint8_t c) {
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);

    return INVALID_HEX_CHAR;
}

uint8_t * convertHexStringToBytes(const void *response, size_t responseLen) {
    if (responseLen % 2 != 0) {
        return NULL;
    }

    uint8_t *bytes = (uint8_t *)calloc(responseLen/2, sizeof(uint8_t));
    if (bytes == NULL) {
        mtkLogE(LOG_TAG, "convertHexStringToBytes: cannot allocate memory for bytes string");
        return NULL;
    }
    uint8_t *hexString = (uint8_t *)response;

    for (size_t i = 0; i < responseLen; i += 2) {
        uint8_t hexChar1 = hexCharToInt(hexString[i]);
        uint8_t hexChar2 = hexCharToInt(hexString[i + 1]);

        if (hexChar1 == INVALID_HEX_CHAR || hexChar2 == INVALID_HEX_CHAR) {
            mtkLogE(LOG_TAG, "convertHexStringToBytes: invalid hex char %d %d",
                    hexString[i], hexString[i + 1]);
            free(bytes);
            return NULL;
        }
        bytes[i/2] = ((hexChar1 << 4) | hexChar2);
    }

    return bytes;
}

long stringToLong(char *str, int base, int *err) {
    long out;
    unsigned long ul;
    *err = 0;

    if (str == NULL) {
        *err = -1;
        return 0;
    }

    ul = strtoul(str, NULL, base);
    if (ul == ULONG_MAX) {
        mtkLogE(LOG_TAG, "stringToLong: error on strtoul");
        *err = -2;
        return 0;
    }
    out = (long)ul;

    return out;
}

extern "C" bool dispatchVoid(int serial, int slotId, int request) {
    RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }
    CALL_ONREQUEST(request, NULL, 0, pRI, slotId);
    return true;
}

void sendErrorResponse(RequestInfo *pRI, RIL_Errno err) {
    android::checkAndDequeueRequestInfoIfAck(pRI, false);
    pRI->pCI->responseFunction((unsigned int) pRI->socket_id, getClientBySlot((int) pRI->socket_id),
            RESPONSE_SOLICITED, pRI->token, err, NULL, 0);
    free(pRI);
}

extern "C" bool dispatchInts(int serial, int slotId, int request, int countInts, ...) {
    RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    int *pInts = (int *)calloc(countInts, sizeof(int));

    if (pInts == NULL) {
        mtkLogE(LOG_TAG, "Memory allocation failed for request %s", requestToString(request));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }
    va_list ap;
    va_start(ap, countInts);
    for (int i = 0; i < countInts; i++) {
        pInts[i] = va_arg(ap, int);
    }
    va_end(ap);

    CALL_ONREQUEST(request, pInts, countInts * sizeof(int), pRI, slotId);

    if (pInts != NULL) {
#ifdef MEMSET_FREED
        memset(pInts, 0, countInts * sizeof(int));
#endif
        free(pInts);
    }
    return true;
}

char* clientTypeToString(android::ClientType type) {
    switch (type) {
        case android::RADIO_AOSP:
            return (char *)"RADIO_AOSP";
        case android::RADIO_MTK:
            return (char *)"RADIO_MTK";
        case android::IMS_AOSP:
            return (char *)"IMS_AOSP";
        case android::IMS_MTK:
            return (char *)"IMS_MTK";
        case android::IMS_WFC:
            return (char *)"IMS_WFC";
        case android::SE_MTK:
            return (char *)"SE_MTK";
        case android::EM_AOSP:
            return (char *)"EM_AOSP";
        case android::EM_MTK:
            return (char *)"EM_MTK";
        case android::ASSIST_MTK:
            return (char *)"ASSIST_MTK";
        case android::RCS_MTK:
            return (char*)"RCS_MTK";
        case android::RSU_MTK:
            return (char*)"RSU_MTK";
        default:
            return (char *)"<UNKNOWN_TYPE>";
    }
}

int radio::isTidInWLock(unsigned int slotId, pthread_t tid) {
    int i, ret = -1;
    int err = 0;
    unsigned int slot = toRealSlot(slotId);
    err = pthread_mutex_lock(&(writeLockOwnerListMutex[slot]));
    if (err != 0) {
        mtkLogE(LOG_TAG, "isTidInWLock lock slot(%u) fail(%d)", slot, err);
        mtkAssert((char*)"isTidInWLock lock fail");
    }
    for (i = 0; i < writeLockOwnerList[slot].size(); i++) {
        if (0 != pthread_equal(tid, writeLockOwnerList[slot][i])) {
            ret = i;
        }
    }
    err = pthread_mutex_unlock(&(writeLockOwnerListMutex[slot]));
    if (err != 0) {
        mtkLogE(LOG_TAG, "isTidInWLock unlock slot(%u) fail(%d)", slot, err);
        mtkAssert((char*)"isTidInWLock unlock fail");
    }
    return ret;
}

void addTidToWLock(unsigned int slotId, pthread_t tid) {
    unsigned int slot = toRealSlot(slotId);
    int ret = 0;
    ret = pthread_mutex_lock(&(writeLockOwnerListMutex[slot]));
    if (ret != 0) {
        mtkLogE(LOG_TAG, "addTidToWLock lock slot(%u) fail(%d)", slot, ret);
        mtkAssert((char*)"addTidToWLock lock fail");
    }
    if (radio::isTidInWLock(slot, tid) == -1) {
        // tid is not in the list, just add it
        writeLockOwnerList[slot].add(tid);
    }
    ret = pthread_mutex_unlock(&(writeLockOwnerListMutex[slot]));
    if (ret != 0) {
        mtkLogE(LOG_TAG, "addTidToWLock unlock slot(%u) fail(%d)", slot, ret);
        mtkAssert((char*)"addTidToWLock unlock fail");
    }
}

void removeTidFromWLock(unsigned int slotId, pthread_t tid) {
    unsigned int slot = toRealSlot(slotId);
    int ret = 0;
    ret = pthread_mutex_lock(&(writeLockOwnerListMutex[slot]));
    if (ret != 0) {
        mtkLogE(LOG_TAG, "removeTidFromWLock lock slot(%u) fail(%d)", slot, ret);
        mtkAssert((char*)"removeTidFromWLock lock fail");
    }
    int i = radio::isTidInWLock(slot, tid);
    if (i != -1) {
        writeLockOwnerList[slot].removeAt(i);
    }
    ret = pthread_mutex_unlock(&(writeLockOwnerListMutex[slot]));
    if (ret != 0) {
        mtkLogE(LOG_TAG, "removeTidFromWLock unlock slot(%u) fail(%d)", slot, ret);
        mtkAssert((char*)"removeTidFromWLock unlock fail");
    }
}

int radio::isTidInRLock(unsigned int slotId, pthread_t tid) {
    int i, ret = -1;
    int err = 0;
    unsigned int slot = toRealSlot(slotId);
    err = pthread_mutex_lock(&(readLockOwnerListMutex[slot]));
    if (err != 0) {
        mtkLogE(LOG_TAG, "isTidInRLock lock slot(%u) fail(%d)", slot, err);
        mtkAssert((char*)"isTidInRLock lock fail");
    }
    for (i = 0; i < readLockOwnerList[slot].size(); i++) {
        if (0 != pthread_equal(tid, readLockOwnerList[slot][i])) {
            ret = i;
        }
    }
    err = pthread_mutex_unlock(&(readLockOwnerListMutex[slot]));
    if (err != 0) {
        mtkLogE(LOG_TAG, "isTidInRLock unlock slot(%u) fail(%d)", slot, err);
        mtkAssert((char*)"isTidInRLock unlock fail");
    }
    return ret;
}

void addTidToRLock(unsigned int slotId, pthread_t tid) {
    unsigned int slot = toRealSlot(slotId);
    int ret = 0;
    ret = pthread_mutex_lock(&(readLockOwnerListMutex[slot]));
    if (ret != 0) {
        mtkLogE(LOG_TAG, "addTidToRLock lock slot(%u) fail(%d)", slot, ret);
        mtkAssert((char*)"addTidToRLock lock fail");
    }
    if (radio::isTidInRLock(slot, tid) == -1) {
        // tid is not in the list, just add it
        readLockOwnerList[slot].add(tid);
    }
    ret = pthread_mutex_unlock(&(readLockOwnerListMutex[slot]));
    if (ret != 0) {
        mtkLogE(LOG_TAG, "addTidToRLock unlock slot(%u) fail(%d)", slot, ret);
        mtkAssert((char*)"addTidToRLock unlock fail");
    }
}

void removeTidFromRLock(unsigned int slotId, pthread_t tid) {
    unsigned int slot = toRealSlot(slotId);
    int ret = 0;
    ret = pthread_mutex_lock(&(readLockOwnerListMutex[slot]));
    if (ret != 0) {
        mtkLogE(LOG_TAG, "removeTidFromRLock lock slot(%u) fail(%d)", slot, ret);
        mtkAssert((char*)"removeTidFromRLock lock fail");
    }
    int i = radio::isTidInRLock(slot, tid);
    if (i != -1) {
        readLockOwnerList[slot].removeAt(i);
    }
    ret = pthread_mutex_unlock(&(readLockOwnerListMutex[slot]));
    if (ret != 0) {
        mtkLogE(LOG_TAG, "removeTidFromRLock unlock slot(%u) fail(%d)", slot, ret);
        mtkAssert((char*)"removeTidFromRLock unlock fail");
    }
}

pthread_rwlock_t * radio::getRadioServiceRwlock(int slotId) {
    pthread_rwlock_t *radioServiceRwlockPtr = &(radioServiceRwlocks[toRealSlot(slotId)]);
    return radioServiceRwlockPtr;
}

void radio::lockRadioServiceRlock(pthread_rwlock_t *radioServiceRwlockPtr, int slotId) {
    int ret = pthread_rwlock_rdlock(radioServiceRwlockPtr);
    if (ret != 0) {
        mtkLogE(LOG_TAG, "lockRadioServiceRlock mutex slot(%d) fail(%d)", slotId, ret);
        mtkAssert((char*)"lockRadioServiceRlock mutex fail");
    }
    addTidToRLock((int) slotId, pthread_self());
}

void radio::unlockRadioServiceRlock(pthread_rwlock_t *radioServiceRwlockPtr, int slotId) {
    int ret = pthread_rwlock_unlock(radioServiceRwlockPtr);
    if (ret != 0) {
        mtkLogE(LOG_TAG, "unlockRadioServiceRlock mutex slot(%d) fail(%d)", slotId, ret);
        mtkAssert((char*)"unlockRadioServiceRlock mutex fail");
    }
    removeTidFromRLock((int) slotId, pthread_self());
}

void radio::lockRadioServiceWlock(pthread_rwlock_t *radioServiceRwlockPtr, int slotId) {
    int ret = pthread_rwlock_wrlock(radioServiceRwlockPtr);
    if (ret != 0) {
        mtkLogE(LOG_TAG, "lockRadioServiceWlock mutex slot(%d) fail(%d)", slotId, ret);
        mtkAssert((char*)"lockRadioServiceWlock mutex fail");
    }
    addTidToWLock((int) slotId, pthread_self());
}

void radio::unlockRadioServiceWlock(pthread_rwlock_t *radioServiceRwlockPtr, int slotId) {
    int ret = pthread_rwlock_unlock(radioServiceRwlockPtr);
    if (ret != 0) {
        mtkLogE(LOG_TAG, "unlockRadioServiceWlock unlock slot(%d) fail(%d)", slotId, ret);
        mtkAssert((char*)"unlockRadioServiceWlock unlock fail");
    }
    removeTidFromWLock((int) slotId, pthread_self());
}

void radio::initReadLockOwnerListMutex(void) {
    int ret = 0;
    for (int i = 0; i < MAX_SIM_COUNT; i++) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
        ret = pthread_mutex_init(&(readLockOwnerListMutex[i]), &attr);
        if (ret != 0) {
            mtkLogE(LOG_TAG, "initReadLockOwnerListMutex mutex init(%d) fail(%d)", i, ret);
            mtkAssert((char*)"initReadLockOwnerListMutex mutex init fail");
        }
    }
}

void radio::initWriteLockOwnerListMutex(void) {
    int ret = 0;
    for (int i = 0; i < MAX_SIM_COUNT; i++) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
        ret = pthread_mutex_init(&(writeLockOwnerListMutex[i]), &attr);
        if (ret != 0) {
            mtkLogE(LOG_TAG, "initWriteLockOwnerListMutex mutex init(%d) fail(%d)", i, ret);
            mtkAssert((char*)"initWriteLockOwnerListMutex mutex init fail");
        }
    }
}

#ifdef MTK_USE_AIDL
extern void radioModemAcknowledgeRequest(unsigned int slotId, int serial);
extern void publicRadioModemAidlService(unsigned int sloid);
extern void publicRadioMessagingAidlService(unsigned int sloid);
extern void publicRadioDataAidlService(unsigned int sloid);
extern void publicRadioImsAidlService(unsigned int sloid);
extern void publicRadioNetworkAidlService(unsigned int sloid);
extern void publicRadioSimAidlService(unsigned int sloid);
extern void publicRadioVoiceAidlService(unsigned int sloid);
extern void publicRadioRcsAidlService(unsigned int slotId);
extern void publicRadioDchAidlService(unsigned int slotId);
extern void publicRadioEcallAidlService(unsigned int slotId);
extern void publicRadioConfigAidlService(unsigned int sloid);

void radio::acknowledgeRequest(unsigned int slotId, int serial) {
    radioModemAcknowledgeRequest(slotId, serial);
}

bool isMtkFwkAddonNotExisted(unsigned int slotId) {
    static unsigned int sMtkFwkAddonNotExisted = -1;
    if (sMtkFwkAddonNotExisted == -1) {
        char tempstr[MTK_PROPERTY_VALUE_MAX] = {0};
        mtk_property_get("ro.vendor.mtk_telephony_add_on_policy", tempstr, "-1");
        mtkLogI(LOG_TAG, "isMtkFwkAddonNotExisted(%d) %s", slotId, tempstr);
        if (strncmp(tempstr, "0", 1) == 0) {
            sMtkFwkAddonNotExisted = 0;
        } else {
            sMtkFwkAddonNotExisted = 1;
        }
    }
    return sMtkFwkAddonNotExisted == 1;
}
#endif

void radio::registerService(RIL_RadioFunctions *callbacks, CommandInfo *commands) {
    //using namespace android::hardware;
    unsigned int simCount = getSimCount();
    /* [ALPS03590595]Set s_vendorFunctions and s_commands before registering service to
        null exception timing issue. */
    s_vendorFunctions = callbacks;
    s_commands = commands;

    if (simCount > MAX_SIM_COUNT) {
        simCount = 2;
    }

#ifndef MTK_HYP_DUAL_TELE_SUPPORT
    #ifdef MTK_USE_AIDL
        if(!ABinderProcess_isThreadPoolStarted()) {
            ABinderProcess_setThreadPoolMaxThreadCount(0);
        } else {
            mtkLogE(LOG_TAG, "**binder pool already started, please chekch reason**");
        }
        //ABinderProcess_startThreadPool();
    #endif
#endif

#if defined(MTK_MTTS_TEST_ENABLE)
    char tempstr[MTK_PROPERTY_VALUE_MAX] = {0};
    mtk_property_get("persist.vendor.ril.test_mode", tempstr, "0");
    if (atoi(tempstr) != 0) {
        mtkLogW(LOG_TAG, "registerService: MTTS mode, don't register HIDL service!!");
        return;
    }
#endif

    mtkLogI(LOG_TAG, "register service start");

#ifdef MTK_USE_AIDL
    for (unsigned int i = 0; i < simCount; i++) {
        pthread_rwlock_t *radioServiceRwlockPtr = getRadioServiceRwlock(i);
        lockRadioServiceWlock(radioServiceRwlockPtr, (int)i);
        publicRadioDataAidlService(i);
        publicRadioMessagingAidlService(i);
        publicRadioModemAidlService(i);
        publicRadioNetworkAidlService(i);
        publicRadioSimAidlService(i);
        publicRadioVoiceAidlService(i);
        publicRadioImsAidlService(i);
        publicRadioRcsAidlService(i);
        publicRadioDchAidlService(i);
        publicRadioEcallAidlService(i);
        publicRadioConfigAidlService(i);
        unlockRadioServiceWlock(radioServiceRwlockPtr, (int)i);
        mtkLogV(LOG_TAG, "register service: release lock %d", i);
    }
    return;
#endif
}

void rilc_thread_pool() {
#ifdef MTK_USE_AIDL
    ABinderProcess_joinThreadPool();
#endif
}
