/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2016. All rights reserved.
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

#include "ril_callbacks.h"
#include "RfxSender.h"
#include "RfxHandlerManager.h"
#include "RfxChannelManager.h"
#include "RfxFragmentEncoder.h"
#include <libmtkrilutils.h>
#include <RfxRilDumpInfo.h>
#include <mtk_log.h>
#include <random>
#include "mipc_api.h"
#include "mipc_msg.h"
#include "mipc_msg_host.h"

using namespace std;

#define DEFAULT_AT_TIMEOUT_MIN "5" // 5 mins

typedef struct {
    char* command;
    int timeout;
} ATTimeout;

static ATTimeout sAtTimeout[] = {
    #include "RfxAtCommandTimeout.h"
};

#define RFX_LOG_TAG "AT"

void RfxSender::MclMessageHandler::handleMessage(const Message& message) {
    RFX_UNUSED(message);
    sender->processMessage(msg);
}

RfxSender::RfxSender(int fd, int channel_id, RfxChannelContext *context) :
        m_fd(fd), m_channel_id(channel_id), m_context(context) {
    mNeedWaitLooper = true;
    sem_init(&mWaitLooperSem, 0, 0);
    memset(&m_threadId, 0, sizeof(pthread_t));
    // read default timeout form property
    char property_timeout[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("vendor.ril.atsend.timeout", property_timeout, DEFAULT_AT_TIMEOUT_MIN);
    property_timeout[RFX_PROPERTY_VALUE_MAX - 1] = '\0';
    m_atTimeoutMsec = atoi(property_timeout) * 60 * 1000; // min -> msec
    rfx_property_get("persist.vendor.ril.fuzzy", property_timeout, "0");
    property_timeout[RFX_PROPERTY_VALUE_MAX - 1] = '\0';
    mIsFuzzyTesting = atoi(property_timeout);
    // default max timeout is 6s
    rfx_property_get("persist.vendor.ril.fuzzy.timeout", property_timeout, "6000000");
    property_timeout[RFX_PROPERTY_VALUE_MAX - 1] = '\0';
    mFuzzyTestingTimeout = atoi(property_timeout);
    mName = RfxChannelManager::channelIdToString(channel_id);
}

void RfxSender::enqueueMessage(const sp<RfxMclMessage>& msg) {
    waitLooper();
    printLog(DEBUG, String8::format("enqueueMessage: %s", msg->toString().string()));
    RfxSender* sender = this;
    sp<MessageHandler> handler = new MclMessageHandler(sender, msg);
    /** Request priority mechanism.
     * 1. Add a variable for RfxMessage & RfxMclMessage to store priority value.
     * 2. User should set priority value for request message at TCL.
     * 3. RfxSender will send request message to Loop queue by priority value.
     */
    m_looper->sendMessageAtTime(msg->getPriority(), handler, m_dummy_msg);
}

void RfxSender::enqueueMessageFront(const sp<RfxMclMessage>& msg) {
    waitLooper();
    printLog(DEBUG, String8::format("enqueueMessage: %s", msg->toString().string()));
    RfxSender* sender = this;
    sp<MessageHandler> handler = new MclMessageHandler(sender, msg);
    m_looper->sendMessageAtTime(MTK_RIL_REQUEST_PRIORITY::MTK_RIL_REQUEST_PRIORITY_HIGH, handler,
            m_dummy_msg);
}

void RfxSender::processMessage(const sp<RfxMclMessage>& msg) {
    if (mIsFuzzyTesting == 1) {
        std::random_device rd;
        std::default_random_engine gen = std::default_random_engine(rd());
        std::uniform_int_distribution<int> dis(0, mFuzzyTestingTimeout);
        int timer = dis(gen);
        printLog(ERROR, String8::format("processMessage, fuzzy testing, wait %d us", timer));
        usleep(timer);
        printLog(ERROR, String8::format("processMessage, fuzzy testing, timeout"));
    }
    // LibDumpRildInfo::AddDumpMessage(msg->toString().string());
    if ((m_channel_id % RIL_CHANNEL_OFFSET) == RIL_CMD_IMS) {
        RfxHandlerManager::processMessage(msg);
    } else {
        m_context->m_restartMutex.lock();
        RfxHandlerManager::processMessage(msg);
        if (m_context->getNeedWaitRestartCondition()) {
            // Wait Capability switch finish
            printLog(INFO, String8::format("processMessage, wait SIM switch done"));
            m_context->m_restartCondition.wait(m_context->m_restartMutex);
        }
        m_context->m_restartMutex.unlock();
    }
}

bool RfxSender::threadLoop() {
    m_looper = Looper::prepare(0);
    sem_post(&mWaitLooperSem);

    // read default timeout form property
    char property_timeout[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("vendor.ril.atsend.timeout", property_timeout, DEFAULT_AT_TIMEOUT_MIN);
    m_atTimeoutMsec = atoi(property_timeout) * 60 * 1000; // min -> msec
    m_threadId = pthread_self();
    mName = RfxChannelManager::channelIdToString(m_channel_id);

    printLog(DEBUG,
            String8::format("threadLoop. RfxSender %s init, channel id = %d", mName, m_channel_id));
    RfxHandlerManager::initHandler(m_channel_id);
    if ((m_channel_id % RIL_CHANNEL_OFFSET) == 0) {
        printLog(ERROR, String8::format("threadLoop. Urc init done"));
        RfxChannelManager::urcRegisterDone();
    }
    if ((RfxRilUtils::getRilRunMode() == RIL_RUN_MODE_MOCK) &&
            ((m_channel_id % RIL_CHANNEL_OFFSET ) != RIL_CMD_IMS)) {
        if (!RfxRilUtils::isMipcSupport()) {
            atSendCommand(String8::format("UT_%d", m_channel_id));
        } else {
            String8 cmd = String8::format("UT_%d", m_channel_id);
            mipc_msg_t* msg_req_ptr = mipc_msg_init(MIPC_SYS_AT_REQ, slotIdChangeToMipcSlotId(0));
            mipc_msg_add_tlv(msg_req_ptr, MIPC_SYS_AT_REQ_T_ATCMD, cmd.length() + 1, cmd.string());
            mipc_msg_t* msg_cnf_ptr = mipc_msg_sync(msg_req_ptr);
            mipc_msg_deinit(msg_req_ptr);
            mipc_msg_deinit(msg_cnf_ptr);
        }
    }

    int result;
    do {
        result = m_looper->pollAll(-1);
        printLog(DEBUG, String8::format("threadLoop, result = %d", result));
    } while (result == Looper::POLL_WAKE || result == Looper::POLL_CALLBACK);

    RFX_ASSERT(0); // Can't go here
    return false;
}

sp<Looper> RfxSender::waitLooper() {
    mWaitLooperMutex.lock();
    if (mNeedWaitLooper) {
        printLog(DEBUG, String8::format("waitLooper() begin"));
        sem_wait(&mWaitLooperSem);
        mNeedWaitLooper = false;
        sem_destroy(&mWaitLooperSem);
        printLog(DEBUG, String8::format("waitLooper() end"));
    }
    mWaitLooperMutex.unlock();
    return m_looper;
}

sp<RfxAtResponse> RfxSender::atSendCommandSinglelineAck (const char *command,
        const char *responsePrefix, RIL_Token ackToken) {
    sp<RfxAtResponse> outResponse = atSendCommandFullAck(command, SINGLELINE, responsePrefix,
            0, ackToken);
    if (outResponse->getError() == 0 &&
            outResponse->getSuccess() > 0 &&
            outResponse->getIntermediates() == NULL) {
        /* successful command must have an intermediate response */
        outResponse->setError(AT_ERROR_INVALID_RESPONSE);
    }
    return outResponse;
}

sp<RfxAtResponse> RfxSender::atSendCommandSingleline (const char *command,
        const char *responsePrefix) {
    return atSendCommandSinglelineAck(command, responsePrefix, NULL);
}

sp<RfxAtResponse> RfxSender::atSendCommandNumericAck (const char *command,
        RIL_Token ackToken) {
    sp<RfxAtResponse> outResponse = atSendCommandFullAck(command, NUMERIC,
            NULL, 0, ackToken);
    if (outResponse->getError() == 0 &&
            outResponse->getSuccess() > 0 &&
            outResponse->getIntermediates() == NULL) {
        /* successful command must have an intermediate response */
        outResponse->setError(AT_ERROR_INVALID_RESPONSE);
    }
    return outResponse;
}

sp<RfxAtResponse> RfxSender::atSendCommandNumeric (const char *command) {
    return atSendCommandNumericAck(command, NULL);
}

sp<RfxAtResponse> RfxSender::atSendCommandMultilineAck (const char *command,
        const char *responsePrefix, RIL_Token ackToken) {
    sp<RfxAtResponse> outResponse = atSendCommandFullAck(command, MULTILINE,
            responsePrefix, 0, ackToken);
    return outResponse;
}

sp<RfxAtResponse> RfxSender::atSendCommandMultiline (const char *command,
        const char *responsePrefix) {
    return atSendCommandMultilineAck(command, responsePrefix, NULL);
}

sp<RfxAtResponse> RfxSender::atSendCommandAck (const char *command,
        RIL_Token ackToken) {
    sp<RfxAtResponse> outResponse = atSendCommandFullAck(command, NO_RESULT,
            NULL, 0, ackToken);
    return outResponse;
}

sp<RfxAtResponse> RfxSender::atSendCommand (const char *command) {
    return atSendCommandAck(command, NULL);
}

sp<RfxAtResponse> RfxSender::atSendCommandRawAck (const char *command,
        RIL_Token ackToken) {
    sp<RfxAtResponse> outResponse = atSendCommandFullAck(command, RAW,
            NULL, 0, ackToken);
    return outResponse;
}

sp<RfxAtResponse> RfxSender::atSendCommandRaw (const char *command) {
    return atSendCommandRawAck(command, NULL);
}

sp<RfxAtResponse> RfxSender::atSendCommandFullAck(const char *command, AtCommandType type,
        const char *responsePrefix, long long timeoutMsec, RIL_Token ackToken) {
    // check modem state
    bool mdOff = RfxMclStatusManager::getNonSlotMclStatusManager()
            ->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    if (mdOff &&
            !strstr(command, "EPOF") &&
            !strstr(command, "EPON") &&
            !strstr(command, "ESIMAUTH") &&
            !strstr(command, "EAUTH") &&
            // External SIM [Start]
            !strstr(command, "ERSA") &&
            !strstr(command, "EIMSPDN") &&
            !strstr(command, "EAPNACT") &&
            !strstr(command, "EMDT")) {
        sp<RfxAtResponse> outResponse = new RfxAtResponse();
        outResponse->setSuccess(0);
        outResponse->setError(AT_ERROR_RADIO_UNAVAILABLE);
        return outResponse;
    }

    // lock sender thread
    m_context->m_commandMutex.lock();

    // for low power
    int mode = -1;
    const char* modeIndex = strstr(command, "EFUN=");
    if (modeIndex != NULL) {
        mode = atoi(modeIndex+5);
        if (mode != 0) {
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
            RfxRilUtils::triggerCCCIIoctlEx(CCCI_IOC_SET_EFUN, &mode);
#endif
            printLog(DEBUG, String8::format("Low Power: mdoe = %d", mode));
        }
    }

    timeoutMsec = getATCommandTimeout(command);
    sp<RfxAtResponse> outResponse = atSendCommandFullNolockAck(command, type, responsePrefix,
            timeoutMsec, ackToken);

    int index = needToHidenLog(command);
    const char *pHiddenPrefix = NULL;
    const char *pPrintCmd = NULL;
    if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
        char modemException[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get(PROPERTY_MODEM_EE, modemException, "0");
        if (outResponse->getError() == AT_ERROR_TIMEOUT) {
            char *pErrMsg = (char *) calloc(1, 201);
            if (pErrMsg != NULL) {
                // parse the AT CMD
                char key[20] = {0};
                if(index >= 0) {
                    pHiddenPrefix = getHidenLogPreFix(index);
                    pPrintCmd = pHiddenPrefix;
                    if (pHiddenPrefix != NULL) {
                        strncpy(key, pHiddenPrefix,
                                MIN(19, strlen(pHiddenPrefix)));
                    }
                } else {
                    pPrintCmd = command;
                    unsigned int cmdLen = strlen(command);
                    if (cmdLen > 0) {
                        unsigned int i = 0, start = 0, end = cmdLen - 1;
                        for (i = 0; i < cmdLen; i++) {
                            if (command[i] == '+') {
                                start = i + 1;
                            }
                            if (command[i] == '=' || command[i] == '?') {
                                end = i - 1;
                                break;
                            }
                        }
                        memset(key, 0x00, 20);
                        strncpy(key, (command + start), MIN(19, (end - start + 1)));
                    }
                }
                // check EE again to prevent false alarm
                /*unsigned*/ int  status = 0;
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
                RfxRilUtils::triggerCCCIIoctlEx(CCCI_IOC_GET_MD_STATE, &status);
#endif
                if (atoi(modemException) != 1 && status != 3) {
                    snprintf(pErrMsg, 200, "AT command pending too long, assert!!! AT cmd: %s,\
timer: %lldms\nCRDISPATCH_KEY:ATTO=%s", key, timeoutMsec, key);
                    printLog(ERROR, String8::format("AT command pending too long, assert!!!\
on channel %d, tid:%lu, AtCmd: %s, AT command timeout: %lldms", m_channel_id, m_threadId,
pPrintCmd, timeoutMsec));
                    if (outResponse->getIntermediates() == NULL) {
                        pid_t pid = getpid();
                        char* str = NULL;
                        int length = asprintf(&str, "%d", pid);
                        if ((length != -1) && (str != NULL)) {
                            rfx_property_set("vendor.ril.mux.report.case", str);
                            rfx_property_set("vendor.ril.muxreport", "1");
                            free(str);
                        }
                        mtkAssert(pErrMsg);
                    } else {
                        outResponse->setSuccess(1);
                        RfxAtLine *line = new RfxAtLine("OK", NULL);
                        outResponse->setError(0);
                        outResponse->setFinalResponse(line);
                    }
                } else {
                    snprintf(pErrMsg, 200, "Modem already exception, assert!!!  AT cmd: %s, timer:\
%lldms\nCRDISPATCH_KEY:ATTO=%s", key, timeoutMsec, key);
                    printLog(ERROR, String8::format("Modem already exception, assert!!! on channel %d, tid:%lu,\
last AT cmd: %s, AT command timeout: %lldms", m_channel_id, m_threadId, pPrintCmd, timeoutMsec));
                    // kill the current thread itself
                    pthread_exit(0);
                }
                free(pErrMsg);
            } else {
                pid_t pid = getpid();
                char* str = NULL;
                int length = asprintf(&str, "%d", pid);
                if ((length != -1) && (str != NULL)) {
                    rfx_property_set("vendor.ril.mux.report.case", str);
                    rfx_property_set("vendor.ril.muxreport", "1");
                    free(str);
                }
                if (atoi(modemException) != 1) {
                    mtkAssert((char *)"AT command pending too long, assert!!!");
                } else {
                    mtkAssert((char *)"Modem already exception, assert!!!");
                }
            }
        }
    } else {
        if (outResponse->getError() == AT_ERROR_TIMEOUT) {
            if(index >= 0) {
                pHiddenPrefix = getHidenLogPreFix(index);
                pPrintCmd = pHiddenPrefix;
            } else {
                pPrintCmd = command;
            }
            printLog(ERROR, String8::format("AT command pending too long!!! on channel %d, tid:%lu,\
last AT cmd: %s, AT command timeout: %lldms", m_channel_id, m_threadId, pPrintCmd, timeoutMsec));
            // reset MD
            rfx_property_set("vendor.ril.mux.report.case", "2");
            rfx_property_set("vendor.ril.muxreport", "1");
        }
    }

    /// for low power
    if ((strstr(command, "EFUN") != NULL) && (mode == 0)
            && (outResponse->getError() == 0) && (outResponse->getSuccess() > 0)) {
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
        RfxRilUtils::triggerCCCIIoctlEx(CCCI_IOC_SET_EFUN, &mode);
#endif
        printLog(DEBUG, String8::format("Low Power, mode = %d", mode));
    }

    m_context->m_commandMutex.unlock();
    // TODO: need at timeout procedure?

    return outResponse;
}

sp<RfxAtResponse> RfxSender::atSendCommandFull(const char *command, AtCommandType type,
        const char *responsePrefix, long long timeoutMsec) {
    return atSendCommandFullAck(command, type, responsePrefix, timeoutMsec, NULL);
}


mipc_msg_sim_ps_id_enum RfxSender::slotIdChangeToMipcSlotId(int slotId) {
    switch (slotId) {
        case 0: return MIPC_MSG_SIM0;
        case 1: return MIPC_MSG_SIM1;
        case 2: return MIPC_MSG_SIM2;
        case 3: return MIPC_MSG_SIM3;
        default:
            return MIPC_MSG_SIM0;
    }
}

int RfxSender:: mipcSlotIdChangeToSlotId(unsigned int slotId) {
    switch (slotId) {
        case MIPC_MSG_SIM0: return 0;
        case MIPC_MSG_SIM1: return 1;
        case MIPC_MSG_SIM2: return 2;
        case MIPC_MSG_SIM3: return 3;
        default:
            return 0;
    }
}

void RfxSender::handleFinalResponse(RfxAtLine* line) {
    // RFX_LOG_D(RFX_LOG_TAG, "handleFinalResponse data:%s", line->getLine());
    // printHiddenLog(line->getLine());
    sp<RfxAtResponse> outResponse = m_context->getResponse();
    outResponse->setFinalResponse(line);
}

void RfxSender::handleRequestAck() {
}

int RfxSender::findNextEOL(char *cur) {
    int number = 0;
    while (*cur != '\0' && *cur != '\r' && *cur != '\n') {
        cur++;
        number++;
    }
    return number;
}

RfxAtLine* RfxSender::setRequestResponseLine(char* line, int type, int isNumericSet) {
    sp<RfxAtResponse> response = m_context->getResponse();
    char *pread = line;
    while (*pread == '\r' || *pread == '\n') {
        pread++;
    }
    RfxAtLine *subAtLine = NULL;
    while (*pread != '\0') {
        int len = findNextEOL(pread);
        RFX_LOG_D(RFX_LOG_TAG, "setRequestResponseLine type:%d, len:%d\n", type, len);
        if (len > 0) {
            char *subline = (char*)malloc((len + 1)*sizeof(char));
            if (subline == NULL) {
                RFX_LOG_E(RFX_LOG_TAG, "setRequestResponseLine malloc memory fail\n");
                RFX_ASSERT(0);
            }
            strncpy(subline, pread, len);
            subline[len] = '\0';
            printHiddenLog(subline);
            subAtLine = new RfxAtLine(subline, NULL);
            if (isNumericSet) {
                if (!subAtLine->isFinalResponseSuccessInNumeric() &&
                        !subAtLine->isFinalResponseErrorInNumeric()) {
                    response->setIntermediates(subAtLine);
                }
            } else {
                if (!subAtLine->isFinalResponseSuccess() &&
                        !subAtLine->isFinalResponseErrorEx(m_channel_id) &&
                        !subAtLine->isIntermediatePattern()) {
                    response->setIntermediates(subAtLine);
                }
            }
            pread = pread + len;
            while (*pread == '\r' || *pread == '\n') {
                pread++;
            }
            free(subline);
            subline = NULL;
        }
    }
    return subAtLine;
}

void RfxSender::processLine(const char *line) {
    bool isNumericSet = false;
    int slotId = m_channel_id/RIL_PROXY_OFFSET;
    int isRequestAck = 0;
    sp<RfxAtResponse> response = m_context->getResponse();
    RfxAtLine* atLine = new (std::nothrow) RfxAtLine(line, NULL);

    if (atLine == NULL) {
        RFX_ASSERT(0);
    }

    if (atLine->isAckResponse()) {
        isRequestAck = 1;
        RFX_LOG_D(RFX_LOG_TAG, "receive ACK (%s)\n", line);
        delete(atLine);
    } else {
        switch (response->getCommandType()) {
        case NO_RESULT:
            {
                RFX_LOG_D(RFX_LOG_TAG, "NO_RESULT\n");
                char *pread = atLine->getLine();
                while (*pread == '\r' || *pread == '\n') {
                    pread++;
                }
                RfxAtLine *subAtLine = NULL;
                while (*pread != '\0') {
                    int len = findNextEOL(pread);
                    if (len > 0) {
                        char *subline = (char*)malloc((len + 1)*sizeof(char));
                        if (subline == NULL) {
                            RFX_LOG_E(RFX_LOG_TAG, "NO_RESULT malloc memory fail\n");
                            RFX_ASSERT(0);
                        }
                        strncpy(subline, pread, len);
                        subline[len] = '\0';
                        printHiddenLog(subline);
                        subAtLine = new RfxAtLine(subline, NULL);
                        if (subAtLine->isFinalResponseSuccess() ||
                                subAtLine->isFinalResponseErrorEx(m_channel_id)) {
                            // will set on the end
                        } else {
                            delete subAtLine;
                            subAtLine = NULL;
                        }
                        pread = pread + len;
                        while (*pread == '\r' || *pread == '\n') {
                            pread++;
                        }
                        free(subline);
                        subline = NULL;
                    }
                } // while
                delete atLine;
                atLine = subAtLine;
            }
            break;
        case NUMERIC:
            if (response->getIntermediates() == NULL) {
                char *pread = atLine->getLine();
                while (*pread == '\r' || *pread == '\n') {
                    pread++;
                }
                RfxAtLine *subAtLine = NULL;
                while (*pread != '\0') {
                    int len = findNextEOL(pread);
                    RFX_LOG_D(RFX_LOG_TAG, "NUMERIC len:%d\n", len);
                    if (len > 0) {
                        char *subline = (char*)malloc((len + 1)*sizeof(char));
                        if (subline == NULL) {
                            RFX_LOG_E(RFX_LOG_TAG, "NUMERIC malloc memory fail\n");
                            RFX_ASSERT(0);
                        }
                        strncpy(subline, pread, len);
                        subline[len] = '\0';
                        printHiddenLog(subline);
                        subAtLine = new RfxAtLine(subline, NULL);
                        if (response->getIntermediates() == NULL
                                && isdigit(subAtLine->getLine()[0])) {
                            response->setIntermediates(subAtLine);
                        } else if (subAtLine->isFinalResponseSuccess() ||
                                subAtLine->isFinalResponseErrorEx(m_channel_id)) {
                            // will set on the end
                        } else {
                            delete subAtLine;
                            subAtLine = NULL;
                        }
                        pread = pread + len;
                        while (*pread == '\r' || *pread == '\n') {
                            pread++;
                        }
                        free(subline);
                        subline = NULL;
                    }
                }
                delete atLine;
                atLine = subAtLine;
            } else {
            }
            break;
        case SINGLELINE:
            if (response->getIntermediates() == NULL) {
                char *pread = atLine->getLine();
                while (*pread == '\r' || *pread == '\n') {
                    pread++;
                }
                RfxAtLine *subAtLine = NULL;
                while (*pread != '\0') {
                    int len = findNextEOL(pread);
                    RFX_LOG_D(RFX_LOG_TAG, "SINGLELINE len:%d\n", len);
                    if (len > 0) {
                        char *subline = (char*)malloc((len + 1)*sizeof(char));
                        if (subline == NULL) {
                            RFX_LOG_D(RFX_LOG_TAG, "SINGLELINE malloc memory fail\n");
                            RFX_ASSERT(0);
                        }
                        strncpy(subline, pread, len);
                        subline[len] = '\0';
                        printHiddenLog(subline);
                        subAtLine = new RfxAtLine(subline, NULL);
                        if (response->getIntermediates() == NULL
                                && RfxMisc::strStartsWith(subAtLine->getLine(),
                                response->getResponsePrefix())) {
                            response->setIntermediates(subAtLine);
                        } else if (subAtLine->isFinalResponseSuccess() ||
                                subAtLine->isFinalResponseErrorEx(m_channel_id)) {
                            // will set on the end
                        } else {
                            delete subAtLine;
                            subAtLine = NULL;
                        }
                        pread = pread + len;
                        while (*pread == '\r' || *pread == '\n') {
                            pread++;
                        }
                        free(subline);
                        subline = NULL;
                    }
                }
                delete atLine;
                atLine = subAtLine;
            } else {
            }
            break;
        case MULTILINE: {
            // if (RfxMisc::strStartsWith(atLine->getLine(), response->getResponsePrefix())) {
                char *pread = atLine->getLine();
                while (*pread == '\r' || *pread == '\n') {
                    pread++;
                }
                RfxAtLine *subAtLine = NULL;
                while (*pread != '\0') {
                    int len = findNextEOL(pread);
                    // RFX_LOG_I(RFX_LOG_TAG, "MULTILINE len:%d\n", len);
                    if (len > 0) {
                        char *subline = (char*)malloc((len + 1)*sizeof(char));
                        if (subline == NULL) {
                            RFX_LOG_E(RFX_LOG_TAG, "MULTILINE malloc memory fail\n");
                            RFX_ASSERT(0);
                        }
                        strncpy(subline, pread, len);
                        subline[len] = '\0';
                        printHiddenLog(subline);
                        subAtLine = new RfxAtLine(subline, NULL);
                        if (RfxMisc::strStartsWith(subAtLine->getLine(),
                                response->getResponsePrefix())) {
                            response->setIntermediates(subAtLine);
                        }  else if (subAtLine->isFinalResponseSuccess() ||
                                subAtLine->isFinalResponseErrorEx(m_channel_id)) {
                            // will set on the end
                        } else {
                            delete subAtLine;
                            subAtLine = NULL;
                        }
                        pread = pread + len;
                        while (*pread == '\r' || *pread == '\n') {
                            pread++;
                        }
                        free(subline);
                        subline = NULL;
                    }
                }
                delete atLine;
                atLine = subAtLine;
            // } else {
            // }
            }
            break;
        /* atci start */
        case RAW:
            isNumericSet = RfxMclStatusManager::getMclStatusManager(slotId)->getBoolValue(
                    RFX_STATUS_KEY_ATCI_IS_NUMERIC, false);
            if (isNumericSet) {
                if (!atLine->isFinalResponseSuccessInNumeric() &&
                        !atLine->isFinalResponseErrorInNumeric()) {
                    RfxAtLine* subAtLine = setRequestResponseLine(atLine->getLine(), RAW, isNumericSet);
                    delete atLine;
                    atLine = subAtLine;
                }
            } else {    // isNumericSet is false
                if (!atLine->isFinalResponseSuccess() &&
                        !atLine->isFinalResponseErrorEx(m_channel_id) &&
                        !atLine->isIntermediatePattern()) {
                    RfxAtLine* subAtLine = setRequestResponseLine(atLine->getLine(), RAW, isNumericSet);
                    delete atLine;
                    atLine = subAtLine;
                }
            }
            break;
        /* atci end */
        default: /* this should never be reached */
            printLog(ERROR, String8::format("Unsupported AT command type %d\n",
                    response->getCommandType()));
            break;
        }
    }

    if (isRequestAck) {
        RFX_LOG_D(RFX_LOG_TAG, "isRequestAck\n");
        response->setAck(1);
        handleRequestAck();
    } else if (atLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "atLine == NULL\n");
        response->setSuccess(1);
        atLine = new (std::nothrow) RfxAtLine("OK", NULL);
        if (atLine == NULL) {
            RFX_ASSERT(0);
        }
        handleFinalResponse(atLine);
    } else {
       if (atLine->isFinalResponseSuccess()) {
            RFX_LOG_D(RFX_LOG_TAG, "isFinalResponseSuccess\n");
            response->setSuccess(1);
            handleFinalResponse(atLine);
        } else if (atLine->isFinalResponseErrorEx(m_channel_id)) {
            RFX_LOG_D(RFX_LOG_TAG, "isFinalResponseErrorEx\n");
            response->setSuccess(0);
            handleFinalResponse(atLine);
        } else if (atLine->isIntermediatePattern()) {
            RFX_LOG_D(RFX_LOG_TAG, "isIntermediatePattern\n");
            response->setSuccess(1);
            handleFinalResponse(atLine);
        } else if (isNumericSet == 1) {
            RFX_LOG_D(RFX_LOG_TAG, "isIntermediatePattern\n");
            if (atLine->isFinalResponseSuccessInNumeric()) {
                response->setSuccess(1);
                handleFinalResponse(atLine);
            } else if (atLine->isFinalResponseErrorInNumeric()) {
                response->setSuccess(0);
                handleFinalResponse(atLine);
            }
        }
    }
}

void RfxSender::printHiddenLog(const char *command) {
    int index = 0;
    if ((index = needToHidenLog(command)) >= 0) {
        printLog(INFO, String8::format("AT< %s=*** (%s, tid:%lu)\n",
                getHidenLogPreFix(index), mName, m_threadId));
    } else if (!strstr(command, ":") && isdigit(command[0])) {
        printLog(INFO, String8::format("AT< *** (%s, tid:%lu)\n", mName, m_threadId));
    } else {
        if (isLogReductionCmd(command)) {
            printLog(DEBUG, String8::format("AT< %s (%s, tid:%lu)\n", command, mName, m_threadId));
        } else {
            printLog(INFO, String8::format("AT< %s (%s, tid:%lu)\n", command, mName, m_threadId));
        }
    }
}

sp<RfxAtResponse> RfxSender::atSendCommandFullNolockAck(const char *command,
        AtCommandType type, const char *responsePrefix, long long timeoutMsec,
        RIL_Token ackToken) {

    int err = 0;
    nsecs_t ts = timeoutMsec * 1000000; // msec->nsec

    sp<RfxAtResponse> outResponse = new RfxAtResponse(type, responsePrefix);

    int config = m_channel_id / RIL_CHANNEL_OFFSET;
    if (RfxRilUtils::isMipcSupport() == true) {
        // printLog(INFO, String8::format("AT> %s (%s, tid:%lu)", command, mName, m_threadId));

        int index = 0;
        if ((index = needToHidenLog(command)) >= 0) {
            printLog(INFO, String8::format("AT> %s=*** (%s, tid:%lu)\n", getHidenLogPreFix(index),
                    mName, m_threadId));
        } else {
            if (isLogReductionCmd(command)) {
                printLog(DEBUG, String8::format("AT> %s (%s tid:%lu)\n", command, mName, m_threadId));
            } else {
                printLog(INFO, String8::format("AT> %s (%s tid:%lu)\n", command, mName, m_threadId));
            }
        }

        mipc_ret_cause_enum causeValue = ERR_NO_ERR;
        mipc_msg_t* msg_cnf_ptr;
        mipc_msg_t* msg_req_ptr = mipc_msg_init(MIPC_SYS_AT_REQ, slotIdChangeToMipcSlotId(config));
        mipc_msg_add_tlv(msg_req_ptr, MIPC_SYS_AT_REQ_T_ATCMD, strlen(command) + 1, command);
        mipc_msg_set_timeout_once(900000);  // 15 * 60 * 1000
        causeValue = mipc_msg_sync_timeout_with_cause(msg_req_ptr, &msg_cnf_ptr);
        mipc_msg_deinit(msg_req_ptr);
        if (causeValue != ERR_NO_ERR || msg_cnf_ptr == NULL) {
            printLog(INFO, String8::format("sync with cause fail causeValue = %d\n", causeValue));
            if (causeValue == ERR_TIME_OUT) {
                err = AT_ERROR_TIMEOUT;
            } else {
                err = AT_ERROR_GENERIC;
            }
            goto error;
        }
        uint16_t atcmdnlen = 0;
        char *atcmdcnf = (char*)(mipc_msg_get_val_ptr(msg_cnf_ptr, MIPC_SYS_AT_CNF_T_ATCMD, &atcmdnlen));
        // printLog(INFO, String8::format("AT< %s (%s, tid:%lu)", atcmdcnf,  mName, m_threadId));
        if (atcmdcnf == NULL || atcmdnlen == 0) {
            printLog(ERROR, String8::format("MIPC_SYS_AT_CNF_T_ATCMD cnf is null"));
            err = AT_ERROR_GENERIC;
            mipc_msg_deinit(msg_cnf_ptr);
            goto error;
        } else {
            m_context->setType(REQUEST);
            m_context->setResponse(outResponse);
            processLine(atcmdcnf);
            mipc_msg_deinit(msg_cnf_ptr);
        }
    } else {
        err = writeline(command);
        if (err < 0)
            goto error;

        // assign by constructor
        // outResponse->setCommandType(type);
        // outResponse->setResponsePrefix(responsePrefix);
        m_context->setType(REQUEST);
        m_context->setResponse(outResponse);
    }


    while (outResponse->getFinalResponse() == NULL /*&& p_channel->readerClosed == 0*/) {
        if (timeoutMsec != 0) {
            err = m_context->m_commandCondition.waitRelative(m_context->m_commandMutex, ts);
        } else {
            err = m_context->m_commandCondition.wait(m_context->m_commandMutex);
        }

        // m_commandCondition.waitRelative will return negative ETIMEDOUT if timeout
        if (err == -ETIMEDOUT) {
            err = AT_ERROR_TIMEOUT;
            goto error;
        }

        if ((outResponse->getFinalResponse() == NULL)
                && (outResponse->getIsAck() == 1)) {
            if (ackToken != NULL) {
                RFX_onRequestAck(ackToken);
            }
            outResponse->setAck(0);
        }
    }

    /*if (p_channel->readerClosed > 0) {
        err = AT_ERROR_CHANNEL_CLOSED;
        goto error;
    }*/

    /* line reader stores intermediate responses in reverse order */
    // outResponse->reverseIntermediates();

error:
    // clearPendingCommand(p_channel);

    m_context->setType(RFX_MSG_TYPE_NONE);
    m_context->setResponse(NULL);
    outResponse->setError(err);
    return outResponse;
}

sp<RfxAtResponse> RfxSender::atSendCommandFullNolock(const char *command,
        AtCommandType type, const char *responsePrefix, long long timeoutMsec) {
    return atSendCommandFullNolockAck(command, type, responsePrefix, timeoutMsec, NULL);
}

int RfxSender::sendUserData(int clientId, unsigned char* data, size_t length) {
    int ret;
    // config format : 00xx0000, xx : 00->slot1, 01->slot2, 10->slot3, 11->slot4
    int config = (m_channel_id/RIL_CHANNEL_OFFSET) << 4;
    printLog(DEBUG, String8::format("sendUserData clientId:%d, len:%zu", clientId, length));
    unsigned char* header = RfxFragmentEncoder::encodeHeader(
            RfxFragmentData(RfxFragmentEncoder::VERSION, clientId, config, length));
    if (header == NULL) {
        printLog(ERROR, String8::format("sendUserData error, header is NULL"));
        return 0;
    }
    ret = writelineUserData(header, RfxFragmentEncoder::HEADER_SIZE);
    free(header);
    if (data != NULL && length > 0) {
        ret = writelineUserData(data, length);
    }
    return ret;
}

int RfxSender::sendUserData(int clientId, int config, unsigned char* data, size_t length) {
    int ret;
    // config format : 00xx0000, xx : 00->slot1, 01->slot2, 10->slot3, 11->slot4
    int newConfig = (config & 0xCF) | ((m_channel_id/RIL_CHANNEL_OFFSET) << 4);
    printLog(DEBUG, String8::format("sendUserData clientId:%d, len:%zu", clientId, length));
    unsigned char* header = RfxFragmentEncoder::encodeHeader(
            RfxFragmentData(RfxFragmentEncoder::VERSION, clientId, newConfig, length));
    if (header == NULL) {
        printLog(ERROR, String8::format("sendUserData error, header is NULL"));
        return 0;
    }
    ret = writelineUserData(header, RfxFragmentEncoder::HEADER_SIZE);
    free(header);
    if (data != NULL && length > 0) {
        ret = writelineUserData(data, length);
    }
    return ret;
}

int RfxSender::getATCommandTimeout(const char *command) {
    RFX_UNUSED(command);
    for (size_t i=0; i<NUM_ELEMS(sAtTimeout); i++) {
        if (strstr(command, sAtTimeout[i].command) != NULL) {
            return (sAtTimeout[i].timeout * 60 * 1000);
        }
    }

    return m_atTimeoutMsec;
}

int RfxSender::writeline(const char *s) {
    size_t cur = 0;
    size_t postfixLength = 1;
    size_t len = strlen(s) + postfixLength;
    ssize_t written;

    if (m_fd < 0 /*|| p_channel->readerClosed > 0*/)
        return AT_ERROR_CHANNEL_CLOSED;

    int index = 0;
    if ((index = needToHidenLog(s)) >= 0) {
        printLog(INFO, String8::format("AT> %s=*** (%s, tid:%lu)\n", getHidenLogPreFix(index),
                mName, m_threadId));
    } else {
        if (isLogReductionCmd(s)) {
            printLog(DEBUG, String8::format("AT> %s (%s tid:%lu)\n", s, mName, m_threadId));
        } else {
            printLog(INFO, String8::format("AT> %s (%s tid:%lu)\n", s, mName, m_threadId));
        }
    }

    /* the main string */
    // send s + \r in a time
    char* wholeLine = NULL;
    // +1 for \0
    wholeLine = (char *) calloc(len + 1, sizeof(char));
    if (wholeLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "OOM");
        return AT_ERROR_GENERIC;
    }
    strncpy(wholeLine, s, len);
    strncat(wholeLine, "\r", 1);
    int writeErrno = 0;
    while (cur < len) {
        do {
            if (RfxRilUtils::isUserLoad() != 1) {
                printLog(DEBUG, String8::format("AT write start\n"));
            }
            written = write(m_fd, wholeLine + cur, len - cur);
            writeErrno = errno;
        } while (written < 0 && errno == EINTR);

        if (written < 0) {
            free(wholeLine);
            return AT_ERROR_GENERIC;
        }

        cur += written;
    }

    free(wholeLine);
    if (RfxRilUtils::isUserLoad() != 1) {
        printLog(DEBUG, String8::format("AT write end, errno = %d, length = %zu", errno, len));
    }
    return 0;
}

int RfxSender::writelineUserData(unsigned char* frame, size_t length) {
    int writeTimes = length/RfxFragmentEncoder::MAX_FRAME_SIZE;
    int remain = length % RfxFragmentEncoder::MAX_FRAME_SIZE;
    int writeP = 0;
    int count = 0;

    if (m_fd < 0) {
        printLog(ERROR, String8::format("m_fd is invalid"));
        return 0;
    }
    while (writeTimes) {
        printLog(DEBUG, String8::format("writelineUserData, writeTimes = %d, remain = %d, length:\
%zu", writeTimes, remain, length));
        count = write(m_fd, frame+writeP, RfxFragmentEncoder::MAX_FRAME_SIZE);
        writeP += RfxFragmentEncoder::MAX_FRAME_SIZE;
        if (count < 0 && errno == EINTR) {
            printLog(ERROR, String8::format("ViLTE write end: %d (err: %d - %s)\n", count,
                    errno, strerror(errno)));
            continue;
        } else if (count < 0) {
            printLog(ERROR, String8::format("ViLTE write end: %d (err: %d - %s)\n", count,
                    errno, strerror(errno)));
            return RAW_DATA_ERROR_GENERIC;
        }
        writeTimes--;
    }

    count = write(m_fd, frame+writeP, remain);
    if (count < 0 && errno == EINTR) {
        printLog(ERROR, String8::format("ViLTE write end: %d (err: %d - %s)\n", count,
                errno, strerror(errno)));
    } else if (count < 0) {
        printLog(ERROR, String8::format("ViLTE write end: %d (err: %d - %s)\n", count,
                errno, strerror(errno)));
        return RAW_DATA_ERROR_GENERIC;
    }

    if (RfxRilUtils::isUserLoad() != 1) {
        printLog(DEBUG, String8::format("User data write end, errno = %d, length = %zu", errno,
                length));
    }
    return 0;
}


void RfxSender::printLog(int level, String8 log) {
    RfxRilUtils::printLog(level, String8::format("%s", RFX_LOG_TAG), log,
            m_channel_id/RIL_CHANNEL_OFFSET);
}
