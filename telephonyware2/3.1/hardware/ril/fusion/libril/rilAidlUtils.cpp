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

#include "rilAidlUtils.h"
#include <mtk_log.h>
#include <ril_service.h>
#include <radiomodem_service.h>
#include <radiodata_service.h>
#include <radiomessaging_service.h>
#include <radionetwork_service.h>
#include <radiovoice_service.h>
#include <radiosim_service.h>
#include <radioims_service.h>
#include <radiorcs_service.h>
#include <radiodch_service.h>
#include <radioConfig_service.h>
#include <radioecall_service.h>
#include <cutils/memory.h>

#undef LOG_TAG
#define LOG_TAG "RILC"


std::string convertCharPtrToStdString(const char* str) {
    return str == NULL ? std::string("") : std::string(str);
}

#define INVALID_HEX_CHAR 16

uint8_t convertHexCharToInt(uint8_t c) {
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);

    return INVALID_HEX_CHAR;
}

uint8_t * hexStringToBytes(const void *response, size_t responseLen) {
    if (responseLen % 2 != 0) {
        return NULL;
    }

    uint8_t *bytes = (uint8_t *)calloc(responseLen/2, sizeof(uint8_t));
    if (bytes == NULL) {
        mtkLogE(LOG_TAG, "hexStringToBytes: cannot allocate memory for bytes string");
        return NULL;
    }
    uint8_t *hexString = (uint8_t *)response;

    for (size_t i = 0; i < responseLen; i += 2) {
        uint8_t hexChar1 = convertHexCharToInt(hexString[i]);
        uint8_t hexChar2 = convertHexCharToInt(hexString[i + 1]);

        if (hexChar1 == INVALID_HEX_CHAR || hexChar2 == INVALID_HEX_CHAR) {
            mtkLogE(LOG_TAG, "hexStringToBytes: invalid hex char %d %d",
                    hexString[i], hexString[i + 1]);
            free(bytes);
            return NULL;
        }
        bytes[i/2] = ((hexChar1 << 4) | hexChar2);
    }

    return bytes;
}

namespace rilAidlUtils {
    static AidlReturnChecker globalChecker;
    IAidlReturnChecker *retChecker = &globalChecker;
    namespace aidl = ::aidl::android::hardware::radio;

    void AidlReturnChecker::radioConfigAidlReturnStatusChecker(unsigned int slotId,
                ndk::ScopedAStatus& ret) {
        aidlRadioConfig::checkAidlReturnStatus(slotId, ret);
    }

    aidl::RadioIndicationType convertIntToAidlRadioIndicationType(int indicationType) {
        return indicationType == RESPONSE_UNSOLICITED ? (aidl::RadioIndicationType::UNSOLICITED) :
                (aidl::RadioIndicationType::UNSOLICITED_ACK_EXP);
    }

    void populateAidlResponseInfo(aidl::RadioResponseInfo& responseInfo, int serial, int responseType, RIL_Errno e) {
        responseInfo.serial = serial;
        switch (responseType) {
            case RESPONSE_SOLICITED:
                responseInfo.type = aidl::RadioResponseType::SOLICITED;
            break;
            case RESPONSE_SOLICITED_ACK_EXP:
                responseInfo.type = aidl::RadioResponseType::SOLICITED_ACK_EXP;
            break;
        }
        responseInfo.error = (aidl::RadioError) e;
    }

    /**
     * Copies over src to dest. If memory allocation fails, responseFunction() is called for the
     * request with error RIL_E_NO_MEMORY. The size() method is used to determine the size of the
     * destination buffer into which the HIDL string is copied. If there is a discrepancy between
     * the string length reported by the size() method, and the length of the string returned by
     * the c_str() method, the function will return false indicating a failure.
     *
     * Returns true on success, and false on failure.
     */
    bool copyAidlStringToRil(char **dest, const std::string &src, android::RequestInfo *pRI,
            bool allowEmpty) {
        size_t len = src.size();
        if (len == 0 && !allowEmpty) {
            *dest = NULL;
            return true;
        }
        *dest = (char *) calloc(len + 1, sizeof(char));
        if (*dest == NULL) {
            mtkLogE(LOG_TAG, "Memory allocation failed for request %s",
                    android::requestToString(pRI->pCI->requestNumber));
            sendErrorResponse(pRI, RIL_E_NO_MEMORY);
            return false;
        }
        if (strlcpy(*dest, src.c_str(), len + 1) >= (len + 1)) {
            mtkLogE(LOG_TAG, "Copy of the HIDL string has been truncated, as "
                  "the string length reported by size() does not "
                  "match the length of string returned by c_str().");
            free(*dest);
            *dest = NULL;
            sendErrorResponse(pRI, RIL_E_INTERNAL_ERR);
            return false;
        }
        return true;
    }

    bool dispatchAidlRaw(int serial, int slotId, int request, const std::vector<uint8_t>& rawBytes) {
        android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
        if (pRI == NULL) {
            return false;
        }
        const uint8_t *uData = rawBytes.data();
        CALL_ONREQUEST(request, (void *) uData, rawBytes.size(), pRI, slotId);
        return true;
    }

    bool dispatchString(int serial, int slotId, int request, const std::string& str) {
        android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
        if (pRI == NULL) {
            return false;
        }

        char *pString;
        if (!copyAidlStringToRil(&pString, str, pRI)) {
            return false;
        }

        CALL_ONREQUEST(request, pString, sizeof(char *), pRI, slotId);

        memsetAndFreeStrings(1, pString);
        return true;
    }

    bool dispatchStrings(int serial, int slotId, int request, bool allowEmpty,
            int countStrings, ...) {
        android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
        if (pRI == NULL) {
            return false;
        }

        char **pStrings;
        pStrings = (char **)calloc(countStrings, sizeof(char *));
        if (pStrings == NULL) {
            mtkLogE(LOG_TAG, "Memory allocation failed for request %s", android::requestToString(request));
            sendErrorResponse(pRI, RIL_E_NO_MEMORY);
            return false;
        }

        va_list ap;
        va_start(ap, countStrings);
        for (int i = 0; i < countStrings; i++) {
            const char* str = va_arg(ap, const char *);
            if (!copyAidlStringToRil(&pStrings[i], std::string(str), pRI, allowEmpty)) {
                va_end(ap);
                for (int j = 0; j < i; j++) {
                    memsetAndFreeStrings(1, pStrings[j]);
                }
                free(pStrings);
                return false;
            }
        }
        va_end(ap);

        CALL_ONREQUEST(request, pStrings, countStrings * sizeof(char *), pRI, slotId);

        if (pStrings != NULL) {
            for (int i = 0 ; i < countStrings ; i++) {
                memsetAndFreeStrings(1, pStrings[i]);
            }
#ifdef MEMSET_FREED
            memset(pStrings, 0, countStrings * sizeof(char *));
#endif
            free(pStrings);
        }
        return true;
    }

    bool dispatchStrings(int serial, int slotId, int request,
                const std::vector<std::string>& data) {
        android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
        if (pRI == NULL) {
            return false;
        }

        int countStrings = data.size();
        char **pStrings;
        pStrings = (char **)calloc(countStrings, sizeof(char *));
        if (pStrings == NULL) {
            mtkLogE(LOG_TAG, "Memory allocation failed for request %s", android::requestToString(request));
            sendErrorResponse(pRI, RIL_E_NO_MEMORY);
            return false;
        }

        for (int i = 0; i < countStrings; i++) {
            if (!copyAidlStringToRil(&pStrings[i], data[i], pRI)) {
                for (int j = 0; j < i; j++) {
                    memsetAndFreeStrings(1, pStrings[j]);
                }
                free(pStrings);
                return false;
            }
        }

        CALL_ONREQUEST(request, pStrings, countStrings * sizeof(char *), pRI, slotId);

        if (pStrings != NULL) {
            for (int i = 0 ; i < countStrings ; i++) {
                memsetAndFreeStrings(1, pStrings[i]);
            }
#ifdef MEMSET_FREED
            memset(pStrings, 0, countStrings * sizeof(char *));
#endif
            free(pStrings);
        }
        return true;
    }

    int responseIntOrEmpty(::aidl::android::hardware::radio::RadioResponseInfo& responseInfo,
            int serial, int responseType, RIL_Errno e, const void *response, size_t responseLen) {
        int ret = -1;

        if (response == NULL && responseLen == 0) {
            // Earlier RILs did not send a response for some cases although the interface
            // expected an integer as response. Do not return error if response is empty. Instead
            // Return -1 in those cases to maintain backward compatibility.
        } else if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "responseIntOrEmpty: Invalid response");
            if (e == RIL_E_SUCCESS) e = RIL_E_INVALID_RESPONSE;
        } else {
            int *p_int = (int *) response;
            ret = p_int[0];
        }
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        return ret;
    }

    void checkMtkAidlReturnStatus(unsigned int slotId, bool ret, android::ClientType type) {
        if (ret == false) {
            mtkLogE(LOG_TAG, "checkMtkAidlReturnStatus: unable to call response/indication, %d", slotId);
            // Remote process hosting the callbacks must be dead. Reset the callback objects;
            // there's no other recovery to be done here. When the client process is back up, it will
            // call setResponseFunctions()

            // Caller should already hold rdlock, release that first
            // note the current counter to avoid overwriting updates made by another thread before
            // write lock is acquired.
            int counter = getCounterMtkRadioNum(slotId);
            pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(slotId);
            int gotRLock = 0;
            int gotWLock = 0;
            if (radio::isTidInRLock((int) slotId, pthread_self()) != -1) {
            // had got the lock
                gotRLock = 1;
                radio::unlockRadioServiceRlock(radioServiceRwlockPtr, (int) slotId);
                mtkLogV(LOG_TAG, "checkMtkAidlReturnStatus: release r lock %d", slotId);
            } else {
                mtkLogE(LOG_TAG, "checkMtkAidlReturnStatus: didn't get r lock %d", slotId);
            }

            if (radio::isTidInWLock((int) slotId, pthread_self()) == -1) {
                // acquire wrlock
                gotWLock = 1;
                radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(slotId);
                ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int) slotId);
                mtkLogV(LOG_TAG, "checkMtkAidlReturnStatus: got w lock %d", slotId);
            } else {
                // has already acquire wrlock and no need release and got again
                mtkLogE(LOG_TAG, "checkMtkAidlReturnStatus: didn't get w lock %d", slotId);
            }

            android::ClientId client = getClientBySlot(slotId);

            // make sure the counter value has not changed
            if (counter == getCounterMtkRadioNum(slotId)) {
                if (client == android::CLIENT_IMS) {
                    mtkLogE(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: reset responseIms/indicationIms callback for %s",
                            slotId, clientTypeToString(type));
                    if (type == android::IMS_MTK) {
                        mtkLogI(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: clearMtkImsResponseAndIndications", slotId);
                        mtkRadioExIms::clearMtkImsResponseAndIndications(slotId);
                    } else if (type == android::IMS_WFC) {
                        mtkLogI(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: clearMwiResponseAndIndications", slotId);
                        mtkRadioExIms::clearMwiResponseAndIndications(slotId);
                    } else {
                        mtkLogI(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: clearMtkVoiceImsResponseAndIndications", slotId);
                        mtkRadioExVoice::clearMtkVoiceImsResponseAndIndications(slotId);
                    }
                } else if (client == android::CLIENT_SE) {
                    if (type == android::SE_MTK) {
                        mtkRadioExSim::clearMtkSeResponseAndIndications(slotId);
                    }
                } else if (client == android::CLIENT_EM) {
                    if (type == android::EM_MTK) {
                        mtkRadioExModem::clearMtkEmResponseAndIndications(slotId);
                    }
                } else if (client == android::CLIENT_ASSIST) {
                    if (type == android::ASSIST_MTK) {
                        mtkRadioExData::clearAssistResponseAndIndications(slotId);
                        mtkRadioExModem::clearAssistResponseAndIndications(slotId);
                    }
                } else if (client == android::CLIENT_RCS) {
                    if (type == android::RCS_MTK) {
                        mtkRadioExRcs::clearMtkRcsResponseAndIndications(slotId);
                    }
                } else if (client == android::CLIENT_CAP) {
                    if (type == android::CAP_MTK) {
                        mtkRadioExModem::clearMtkCapResponse(slotId);
                    }
                } else if (client == android::CLIENT_SMART_RAT_SWITCH) {
                    if (type == android::SMART_RAT_SWITCH_MTK) {
                        mtkRadioExNetwork::clearMtkSmartRatResponseAndIndications(slotId);
                    }
                } else if (client == android::CLIENT_RSU) {
                    if (type == android::RSU_MTK) {
                        mtkRadioExSim::clearMtkRsuResponseAndIndications(slotId);
                    }
                } else if (client == android::CLIENT_DCH) {
                    if (type == android::DCH_EXT) {
                        mtkRadioExDch::clearMtkDchResponseAndIndications(slotId);
                    } else if (type == android::DCH_MTK) {
                        mtkRadioExDch::clearMtkDchInternalResponseAndIndications(slotId);
                    }
                } else {
                    mtkLogI(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: reset responseMtk/indicationMtk callback", slotId);
                    if (type == android::IMS_MTK) {
                        mtkLogI(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: error reset IMS response/indication callback", slotId);
                        mtkRadioExIms::clearMtkImsResponseAndIndications(slotId);
                        mtkRadioExVoice::clearMtkVoiceImsResponseAndIndications(slotId);
                    }  else if (type == android::IMS_WFC) {
                        mtkLogI(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: error reset WFC response/indication callback", slotId);
                         mtkRadioExIms::clearMwiResponseAndIndications(slotId);
                    } else if (type == android::EM_MTK) {
                        mtkLogI(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: error reset EM response/indication callback", slotId);
                        mtkRadioExModem::clearMtkEmResponseAndIndications(slotId);
                    } else if (type == android::SMART_RAT_SWITCH_MTK) {
                        mtkLogI(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: error reset SMART RAT SWITCH response/indication callback", slotId);
                        mtkRadioExNetwork::clearMtkSmartRatResponseAndIndications(slotId);
                    } else if (type == android::GBA_MTK) {
                        mtkLogI(LOG_TAG,
                            "checkMtkAidlReturnStatus[%d]: error reset Gba response callback", slotId);
                        mtkRadioExModem::clearMtkGbaResponse(slotId);
                    } else {
                        if (sSetMtkResNum[DATA_INDEX][slotId] == 0) {
                            mtkRadioExData::clearMtkDataResponseAndIndications(slotId);
                        }
                        if (sSetMtkResNum[MESSAGE_INDEX][slotId] == 0) {
                            mtkRadioExMessaging::clearMtkMessagingResponseAndIndications(slotId);
                        }
                        if (sSetMtkResNum[MODEM_INDEX][slotId] == 0) {
                            mtkRadioExModem::clearMtkModemResponseAndIndications(slotId);
                        }
                        if (sSetMtkResNum[NETWORK_INDEX][slotId] == 0) {
                            mtkRadioExNetwork::clearMtkNetworkResponseAndIndications(slotId);
                        }
                        if (sSetMtkResNum[SIM_INDEX][slotId] == 0) {
                            mtkRadioExSim::clearMtkSimResponseAndIndications(slotId);
                        }
                        if (sSetMtkResNum[VOICE_INDEX][slotId] == 0) {
                            mtkRadioExVoice::clearMtkVoiceResponseAndIndications(slotId);
                        }
                        if (sSetMtkResNum[ECALL_INDEX][slotId] == 0) {
                            mtkRadioExEcall::clearMtkEcallResponseAndIndications(slotId);
                        }
                    }
                }

                plusCounterMtkRadioNumOne(slotId);  // mCounterMtkRadioEx[slotId]++;
                android::onCommandDisconnect((RIL_SOCKET_ID) slotId, type);
            } else {
                mtkLogE(LOG_TAG, "checkMtkAidlReturnStatus: not resetting responseFunctions as they likely "
                        "got updated on another thread");
            }

            // release wrlock
            if (gotWLock == 1) {
                ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)slotId);
            }
            mtkLogV(LOG_TAG, "checkMtkAidlReturnStatus: release lock %d", slotId);
            if (gotRLock == 1) {
                // Reacquire rdlock
                radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(slotId);
                ::radio::lockRadioServiceRlock(radioServiceRwlockPtr, (int)slotId);
                mtkLogV(LOG_TAG, "checkMtkAidlReturnStatus: got r lock %d", slotId);
            } else {
                mtkLogE(LOG_TAG, "checkMtkAidlReturnStatus: don't need to re-get r lock %d", slotId);
            }
        }
    }

    void checkAospAidlReturnStatus(unsigned int slotId, bool ret) {
        if (ret == false) {
            mtkLogE(LOG_TAG, "checkAospAidlReturnStatus: unable to call response/indication, %d", slotId);
            // Remote process hosting the callbacks must be dead. Reset the callback objects;
            // there's no other recovery to be done here. When the client process is back up, it will
            // call setResponseFunctions()

            // Caller should already hold rdlock, release that first
            // note the current counter to avoid overwriting updates made by another thread before
            // write lock is acquired.
            int counter = getCounterRadioNum(slotId);

            pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(slotId);
            int gotRLock = 0;
            int gotWLock = 0;
            if (radio::isTidInRLock((int) slotId, pthread_self()) != -1) {
                // had got the lock
                gotRLock = 1;
                ::radio::unlockRadioServiceRlock(radioServiceRwlockPtr, (int) slotId);
                mtkLogV(LOG_TAG, "checkAospAidlReturnStatus: release r lock %d", slotId);
            } else {
                mtkLogE(LOG_TAG, "checkAospAidlReturnStatus: didn't get r lock %d", slotId);
            }

            if (radio::isTidInWLock((int) slotId, pthread_self()) == -1) {
                // acquire wrlock
                gotWLock = 1;
                radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(slotId);
                ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int) slotId);
                mtkLogV(LOG_TAG, "checkAospAidlReturnStatus: got w lock %d", slotId);
            } else {
                // has already acquire wrlock and no need release and got again
                mtkLogE(LOG_TAG, "checkAospAidlReturnStatus: didn't get w lock %d", slotId);
            }
            // make sure the counter value has not changed
            if (counter == getCounterRadioNum(slotId)) {
                mtkLogI(LOG_TAG,
                        "checkAospAidlReturnStatus[%d]: reset response/indication callback", slotId);
                plusCounterRadioNumOne(slotId);  // mCounterRadio[mSlotId]++;
                android::ClientId client = getClientBySlot(slotId);
                if (client == android::CLIENT_IMS) {
                    radioModem::clearModemResponseAndIndications(slotId);
                    radioNetwork::clearNetworkResponseAndIndications(slotId);
                    radioVoice::clearVoiceResponseAndIndications(slotId);
                    radioSim::clearSimResponseAndIndications(slotId);
                    android::onCommandDisconnect((RIL_SOCKET_ID) slotId, android::IMS_AOSP);
                } else if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
                    if (sSetAospResNum[DATA_INDEX][slotId] == 0) {
                        radioData::clearDataResponseAndIndications(slotId);
                    }
                    if (sSetAospResNum[MESSAGE_INDEX][slotId] == 0) {
                        radioMessaging::clearMessagingResponseAndIndications(slotId);
                    }
                    if (sSetAospResNum[MODEM_INDEX][slotId] == 0) {
                        radioModem::clearModemResponseAndIndications(slotId);
                    }
                    if (sSetAospResNum[NETWORK_INDEX][slotId] == 0) {
                        radioNetwork::clearNetworkResponseAndIndications(slotId);
                    }
                    if (sSetAospResNum[SIM_INDEX][slotId] == 0) {
                        radioSim::clearSimResponseAndIndications(slotId);
                    }
                    if (sSetAospResNum[VOICE_INDEX][slotId] == 0) {
                        radioVoice::clearVoiceResponseAndIndications(slotId);
                    }
                    if (sSetAospResNum[IMS_INDEX][slotId] == 0) {
                        radioIms::clearImsResponseAndIndications(slotId);
                    }
                    android::onCommandDisconnect((RIL_SOCKET_ID) slotId, android::RADIO_AOSP);
                } else if (client == android::CLIENT_SE) {
                    radioModem::clearModemResponseAndIndications(slotId);
                    radioSim::clearSimResponseAndIndications(slotId);
                    android::onCommandDisconnect((RIL_SOCKET_ID) slotId, android::SE_MTK);
                }
            } else {
                mtkLogE(LOG_TAG, "checkAospAidlReturnStatus: not resetting responseFunctions as they likely "
                    "    got updated on another thread");
            }

            // release wrlock
            if (gotWLock == 1) {
                ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)slotId);
            }
            mtkLogV(LOG_TAG, "checkAospAidlReturnStatus: release lock %d", slotId);
            if (gotRLock == 1) {
                // Reacquire rdlock
                radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(slotId);
                ::radio::lockRadioServiceRlock(radioServiceRwlockPtr, (int)slotId);
                mtkLogV(LOG_TAG, "checkAospAidlReturnStatus: got r lock %d", slotId);
            } else {
                mtkLogE(LOG_TAG, "checkAospAidlReturnStatus: don't need to re-get r lock %d", slotId);
            }
        }
    }

    unsigned int setAospResponseNumberToOne(unsigned int index, int slotId) {
        if (slotId >= 0 && slotId < 4 * android::CLIENT_COUNT) {
            sSetAospResNum[index][slotId] = 1;
        }
        return 0;
    }

    unsigned int checkIfSetAllAospResponseDone(int slotId) {
        // 6 is the number radio aidl service
        if (slotId >= 0 && slotId < 4 * android::CLIENT_COUNT) {
            if ((sSetAospResNum[DATA_INDEX][slotId] == 1) &&
                    (sSetAospResNum[MESSAGE_INDEX][slotId] == 1) &&
                    (sSetAospResNum[MODEM_INDEX][slotId] == 1) &&
                    (sSetAospResNum[NETWORK_INDEX][slotId] == 1) &&
                    (sSetAospResNum[SIM_INDEX][slotId] == 1) &&
                    (sSetAospResNum[VOICE_INDEX][slotId] == 1)) {
                mtkLogI(LOG_TAG, "set all aosp response done: slotId:%d", slotId);
                cleanSetAospResNumToZero(slotId);
                return 1;
            }
        }
        return 0;
    }

    unsigned int cleanSetAospResNumToZero(int slotId) {
        if (slotId >= 0 && slotId < 4 * android::CLIENT_COUNT) {
            for (unsigned int i = 0; i < MAX_INDEX; i++) {
                sSetAospResNum[i][slotId] = 0;
            }
        }
        return 0;
    }

    unsigned int setMtkResponseNumberToOne(unsigned int index, int slotId) {
        if (slotId >= 0 && slotId < 4 * android::CLIENT_COUNT) {
            sSetMtkResNum[index][slotId] = 1;
            mtkLogE(LOG_TAG, "setMtkResponseNumberToOne  index%d slotId %d",index, slotId);
        }
        return 0;
    }

    unsigned int checkIfSetAllMtkResponseDone(int slotId) {
        // 6 is the number radio aidl service
        if (slotId >= 0 && slotId < 4 * android::CLIENT_COUNT) {
            if ((sSetMtkResNum[DATA_INDEX][slotId] == 1) &&
                    (sSetMtkResNum[MESSAGE_INDEX][slotId] == 1) &&
                    (sSetMtkResNum[MODEM_INDEX][slotId] == 1) &&
                    (sSetMtkResNum[NETWORK_INDEX][slotId] == 1) &&
                    (sSetMtkResNum[SIM_INDEX][slotId] == 1) &&
                    (sSetMtkResNum[VOICE_INDEX][slotId] == 1)) {
                mtkLogI(LOG_TAG, "set all MTK response done: slotId:%d", slotId);
                cleanSetMtkResNumToZero(slotId);
                return 1;
            }
        }
        return 0;
    }

    unsigned int cleanSetMtkResNumToZero(int slotId) {
        if (slotId >= 0 && slotId < 4 * android::CLIENT_COUNT) {
            for (unsigned int i = 0; i < MAX_INDEX; i++) {
                sSetMtkResNum[i][slotId] = 0;
            }
        }
        return 0;
    }

    void ril_service_init_watch_dog(timer_t* timer, timer_function cb) {
        struct sigevent sevp;
        memset(&sevp, 0, sizeof(sevp));
        sevp.sigev_value.sival_int = 0;
        sevp.sigev_notify = SIGEV_THREAD;
        sevp.sigev_notify_function = cb;
        if (timer_create(CLOCK_MONOTONIC, &sevp, timer) == -1) {
            mtkLogE(LOG_TAG, "ril service timer create failed reason[%s]", strerror(errno));
        }
    }

    void ril_service_start_watch_dog(timer_t timer, int milliseconds) {
        struct itimerspec expire;
        expire.it_interval.tv_sec = 0;
        expire.it_interval.tv_nsec = 0;
        expire.it_value.tv_sec = milliseconds / 1000;
        expire.it_value.tv_nsec = (milliseconds % 1000) * 1000000;
        int ret = timer_settime(timer, 0, &expire, NULL);
        if (ret != 0) {
            mtkLogE(LOG_TAG, "ril service set timer failed reason[%s]", strerror(errno));
        }
    }

    void ril_service_stop_watch_dog(timer_t timer) {
        ril_service_start_watch_dog(timer, 0);
    }

}
