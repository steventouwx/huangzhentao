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

#include <telephony/mtk_ril.h>
#include <ril_internal.h>
#include <ril_service.h>
#include <rilAidlUtils.h>
#include <mtk_log.h>
#include <mtk_properties.h>
#include <radiorcs_service.h>

#include <binder/ProcessState.h>
#include <android/binder_manager.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rcs/BnMtkRadioExRcs.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RILC"

namespace aidl_radio = ::aidl::android::hardware::radio;
namespace aidl_rcs = ::aidl::vendor::mediatek::hardware::mtkradioex::rcs;

using rilAidlUtils::retChecker;

struct RadioRcsImpl {
    std::shared_ptr<aidl_rcs::IMtkRadioExRcsResponse> mtkRcsResponse;
    std::shared_ptr<aidl_rcs::IMtkRadioExRcsIndication> mtkRcsIndication;
};

RadioRcsImpl radioRcsServiceImpl[MAX_SIM_COUNT];

namespace vendor::mediatek::hardware::mtkradioex::rcs {
    using ::ndk::ScopedAStatus;
    namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::rcs;
    constexpr auto ok = &ScopedAStatus::ok;

    MtkRadioRcs::MtkRadioRcs(unsigned int slotId) {
        m_slot_id = slotId;
    }

    ScopedAStatus MtkRadioRcs::startRcsService(int32_t serial, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]startRcsService with serial[%d], client[%d]", m_slot_id, serial, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        dispatchVoid(serial, m_slot_id + clientId * MAX_SIM_COUNT, RIL_REQUEST_RCS_START_SERVICE);

        return ok();
    }

    ScopedAStatus MtkRadioRcs::stopRcsService(int32_t serial, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]stopRcsService with serial[%d], client[%d]", m_slot_id, serial, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        dispatchVoid(serial, m_slot_id + clientId * MAX_SIM_COUNT, RIL_REQUEST_RCS_STOP_SERVICE);

        return ok();
    }

    ScopedAStatus MtkRadioRcs::requestRegistrationInfo(int32_t serial, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]requestRegistrationInfo with serial[%d], client[%d]", m_slot_id, serial, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        dispatchVoid(serial, m_slot_id + clientId * MAX_SIM_COUNT, RIL_REQUEST_RCS_REGISTRATION_INFO);

        return ok();
    }

    ScopedAStatus MtkRadioRcs::setImsDeregistrationIndicationEnabled(int32_t serial,
        bool enabled, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]setImsDeregistrationIndicationEnabled with \
            serial[%d], enabled[%d], client[%d]", m_slot_id, serial, enabled, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        dispatchInts(serial, m_slot_id + clientId * MAX_SIM_COUNT,
            RIL_REQUEST_RCS_ENABLE_DEREG_IND, 1, BOOL_TO_INT(enabled));

        return ok();
    }

    ScopedAStatus MtkRadioRcs::resumeImsDeregistration(int32_t serial, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]resumeImsDeregistration with serial[%d], client[%d]", m_slot_id, serial, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        dispatchVoid(serial, m_slot_id + clientId * MAX_SIM_COUNT, RIL_REQUEST_RCS_RESUME_DEREGISTRATION);

        return ok();
    }

    ScopedAStatus MtkRadioRcs::triggerRestoration(int32_t serial, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]triggerRestoration with serial[%d], client[%d]", m_slot_id, serial, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        dispatchVoid(serial, m_slot_id + clientId * MAX_SIM_COUNT, RIL_REQUEST_RCS_TRIGGER_RESTORATION);

        return ok();
    }

    ScopedAStatus MtkRadioRcs::sendSipMessage(int32_t serial, int32_t requestId, int32_t connId,
        const std::vector<uint8_t>& msgData, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]sendSipMessage with serial[%d], client[%d]", m_slot_id, serial, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        // add null terminator before send SIP data to stack
        RIL_RcsSipMessage sip;
        sip.reqId = requestId;
        sip.connId = connId;
        sip.dataLength = msgData.size()+1;

        std::vector<uint8_t> rawData(sizeof(sip) + sip.dataLength);
        std::copy((uint8_t *)&sip, &((uint8_t *)&sip)[sizeof(sip)], &rawData[0]);
        std::copy(&msgData[0], &msgData[msgData.size()], &rawData[sizeof(sip)]);
        rawData[rawData.size()-1] = '\0';

        rilAidlUtils::dispatchAidlRaw(serial, m_slot_id + clientId * MAX_SIM_COUNT,
            RIL_REQUEST_RCS_SEND_SIP_MESSAGE, rawData);

        return ok();
    }

    ScopedAStatus MtkRadioRcs::switchRcsRoiStatus(int32_t serial, bool status, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]switchRcsRoiStatus with status[%d],client[%d]", m_slot_id, status, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        dispatchInts(serial, m_slot_id + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_RCS_SWITCH_ROI_STATUS, 1, BOOL_TO_INT(status));
        return ok();
    }

    ScopedAStatus MtkRadioRcs::updateRcsCapabilities(int32_t serial, int32_t mode,
            const std::string& featureTags, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]updateRcsCapabilities with mode[%d], clientId[%d]",
            m_slot_id, mode, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        rilAidlUtils::dispatchStrings(serial, m_slot_id + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_RCS_UPDATE_CAPABILITIES, false, 2,
                std::to_string(mode).c_str(), featureTags.c_str());
        return ok();
    }

    ScopedAStatus MtkRadioRcs::updateRcsSessionInfo(int32_t serial, int32_t count, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]updateRcsSessionInfo with count[%d], clientId[%d]",
                m_slot_id, count, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        dispatchInts(serial, m_slot_id + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_RCS_UPDATE_SESSION_INFO, 1, count);

        return ok();
    }

    ScopedAStatus MtkRadioRcs::getUserAgent(int32_t serial, int32_t clientId) {
        mtkLogD(LOG_TAG, "[%d][RCS]getUserAgent with clientId[%d]", m_slot_id, clientId);
        if (!checkRequestClientIdSupport(clientId, android::CLIENT_RCS)) {
            return ok();
        }

        dispatchVoid(serial, m_slot_id + clientId * MAX_SIM_COUNT, RIL_REQUEST_RCS_GET_USER_AGENT);
        return ok();
    }

    ScopedAStatus MtkRadioRcs::responseAcknowledgementMtk() {
        android::releaseWakeLock();
        return ok();
    }

    ScopedAStatus MtkRadioRcs::setResponseFunctionsRcs(
        const std::shared_ptr<aidl_rcs::IMtkRadioExRcsResponse>& radioResponse,
        const std::shared_ptr<aidl_rcs::IMtkRadioExRcsIndication>& radioIndication) {

        mtkLogD(LOG_TAG, "[%d][RCS]setResponseFunctionsRcs", m_slot_id);

        pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(m_slot_id);
        ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, m_slot_id);
        mtkLogV(LOG_TAG, "[%d]setResponseFunctionsRcs got lock", m_slot_id);
        radioRcsServiceImpl[m_slot_id].mtkRcsResponse = radioResponse;
        radioRcsServiceImpl[m_slot_id].mtkRcsIndication = radioIndication;
        plusCounterMtkRadioNumOne(m_slot_id);

        ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)m_slot_id);
        mtkLogV(LOG_TAG, "[%d]setResponseFunctionsRcs release lock", m_slot_id);

        return ok();
    }
}

int mtkRadioExRcs::startServiceResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    mtkLogD(LOG_TAG, "[%d][RCS]startServiceResponse with clientId[%d],serial[%d]", realSlotId, clientId, serial);

    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus status = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->startRcsServiceResponse(responseInfo);
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]startServiceResponse binder fail!", realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%d][RCS]startServiceResponse mtkRcsResponse is NULL", realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::stopServiceResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    mtkLogD(LOG_TAG, "[%d][RCS]stopServiceResponse with clientId[%d],serial[%d]", realSlotId, clientId, serial);

    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus status = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->stopRcsServiceResponse(responseInfo);
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]stopServiceResponse binder fail!", realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%d][RCS]stopServiceResponse mtkRcsResponse is NULL", realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::registrationInfoResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    mtkLogD(LOG_TAG, "[%d][RCS]registrationInfoResponse with clientId[%d],serial[%d]", realSlotId, clientId, serial);

    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus status = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->requestRegistrationInfoResponse(responseInfo);
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]registrationInfoResponse binder fail!", realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%d][RCS]registrationInfoResponse mtkRcsResponse is NULL", realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::enableDeregIndResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    mtkLogD(LOG_TAG, "[%d][RCS]enableDeregIndResponse with clientId[%d],serial[%d]", realSlotId, clientId, serial);

    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus status = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->setImsDeregistrationIndicationEnabledResponse(responseInfo);
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]enableDeregIndResponse binder fail!", realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%d][RCS]enableDeregIndResponse mtkRcsResponse is NULL", realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::resumeDeregistrationResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    mtkLogD(LOG_TAG, "[%d][RCS]resumeDeregistrationResponse with clientId[%d],serial[%d]", realSlotId, clientId, serial);

    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus status = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->resumeImsDeregistrationResponse(responseInfo);
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]resumeDeregistrationResponse binder fail!", realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%d][RCS]resumeDeregistrationResponse mtkRcsResponse is NULL", realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::triggerRestorationResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t respLen) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    mtkLogD(LOG_TAG, "[%d][RCS]triggerRestorationResponse with clientId[%d],serial[%d]",
            realSlotId, clientId, serial);

    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus status = radioRcsServiceImpl[realSlotId].mtkRcsResponse
                ->triggerRestorationResponse(responseInfo);
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]triggerRestorationResponse binder fail!", realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%d][RCS]triggerRestorationResponse mtkRcsResponse is NULL", realSlotId);
    }

    return 0;
}


int mtkRadioExRcs::sendSipMessageResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    mtkLogD(LOG_TAG, "[%d][RCS]sendSipMessageResponse with clientId[%d],serial[%d]", realSlotId, clientId, serial);

    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus status = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->sendSipMessageResponse(responseInfo);
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]sendSipMessageResponse binder fail!", realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%d][RCS]sendSipMessageResponse mtkRcsResponse is NULL", realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::switchRcsRoiStatusResponse(unsigned int slotId, android::ClientId clientId,
                                              int responseType, int serial, RIL_Errno e,
                                              const void *response, size_t responseen) {
    mtkLogD(LOG_TAG, "[%s][RCS] slotId:%d,clientId:%d,serial:%d", __FUNCTION__, slotId, clientId, serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->switchRcsRoiStatusResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "[%s][%d] binder fail!", __FUNCTION__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s][%d] mtkRcsResponse is NULL", __FUNCTION__, realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::updateRcsCapabilitiesResponse(unsigned int slotId, android::ClientId clientId,
                                                 int responseType, int serial, RIL_Errno e,
                                                 const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "[%s][RCS] slotId:%d,clientId:%d,serial:%d", __FUNCTION__, slotId, clientId, serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->updateRcsCapabilitiesResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "[%s][%d] binder fail!", __FUNCTION__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s][%d] mtkRcsResponse is NULL", __FUNCTION__, realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::updateRcsSessionInfoResponse(unsigned int slotId, android::ClientId clientId,
                                                int responseType, int serial, RIL_Errno e,
                                                const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "[%s][RCS] slotId:%d,clientId:%d,serial:%d", __FUNCTION__, slotId, clientId, serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->updateRcsSessionInfoResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "[%s][%d] binder fail!", __FUNCTION__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s][%d] mtkRcsResponse is NULL", __FUNCTION__, realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::getUserAgentResponse(unsigned int slotId, android::ClientId clientId,
                                        int responseType, int serial, RIL_Errno e,
                                        const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "[%s][RCS] slotId:%d,clientId:%d,serial:%d", __FUNCTION__, slotId, clientId, serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_RCS && radioRcsServiceImpl[realSlotId].mtkRcsResponse) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioRcsServiceImpl[realSlotId].mtkRcsResponse
            ->getUserAgentResponse(responseInfo, convertCharPtrToStdString((char*)response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "[%s][%d] binder fail!", __FUNCTION__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RCS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s][%d] mtkRcsResponse is NULL", __FUNCTION__, realSlotId);
    }

    return 0;
}

int mtkRadioExRcs::registrationInfoInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "[%d][RCS]registrationInfoInd", slotId);
    RIL_RcsRegistrationInfo *regInfo = 0;

    if (!response || responseLen != sizeof(*regInfo)) {
        mtkLogE(LOG_TAG, "[%d][RCS]registrationInfoInd response invalid", slotId);
        return 0;
    }
    regInfo = (RIL_RcsRegistrationInfo *)response;

    if (radioRcsServiceImpl[slotId].mtkRcsIndication) {
        aidl_rcs::RegistrationInfo aidlRegInfo;

        aidlRegInfo.regMode = aidl_rcs::RegMode::MODE_IMS;
        aidlRegInfo.regState = (aidl_rcs::RegState)regInfo->regState;
        aidlRegInfo.rcsMode = regInfo->rcsMode;
        aidlRegInfo.rcsFeatures = regInfo->rcsFeatures;
        aidlRegInfo.localAddress = convertCharPtrToStdString(regInfo->localAddress);
        aidlRegInfo.localPort = regInfo->localPort;
        aidlRegInfo.remoteAddress = convertCharPtrToStdString(regInfo->remoteAddress);
        aidlRegInfo.remotePort = regInfo->remotePort;
        aidlRegInfo.homeDomain = convertCharPtrToStdString(regInfo->homeDomain);
        aidlRegInfo.transport = (aidl_rcs::TransportProtocol)regInfo->transport;
        aidlRegInfo.ipVersion = (aidl_rcs::IpProtocol)regInfo->ipVersion;
        aidlRegInfo.IMPU = convertCharPtrToStdString(regInfo->IMPU);
        aidlRegInfo.IMPI = convertCharPtrToStdString(regInfo->IMPI);
        aidlRegInfo.userAgent = convertCharPtrToStdString(regInfo->userAgent);
        aidlRegInfo.ipSecLocalPort = regInfo->ipSecLocalPort;
        aidlRegInfo.ipSecRemotePort = regInfo->ipSecRemotePort;
        aidlRegInfo.securityVerifyHeader = convertCharPtrToStdString(regInfo->svHeader);
        aidlRegInfo.pAssociatedUri = convertCharPtrToStdString(regInfo->pAssociatedUri);
        aidlRegInfo.instanceId = convertCharPtrToStdString(regInfo->instanceId);
        aidlRegInfo.serviceRoute = convertCharPtrToStdString(regInfo->serviceRoute);
        aidlRegInfo.pAccessNetworkInfo = convertCharPtrToStdString(regInfo->pAccessNetworkInfo);
        aidlRegInfo.regTimeStamp = convertCharPtrToStdString(regInfo->regTimestamp);

        ndk::ScopedAStatus status = radioRcsServiceImpl[slotId].mtkRcsIndication->currentRegistrationInfo(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), aidlRegInfo);
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]registrationInfoInd binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogW(LOG_TAG, "[%d][RCS]registrationInfoInd mtkRcsIndication is null", slotId);
    }

    return 0;
}

int mtkRadioExRcs::sipMessageInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "[%d][RCS]sipMessageInd", slotId);
    RIL_RcsSipMessage *sipInfo = (RIL_RcsSipMessage *)response;

    if (!response || responseLen != sizeof(*sipInfo)+sipInfo->dataLength) {
        mtkLogE(LOG_TAG, "[%d][RCS]sipMessageInd response invalid", slotId);
        return 0;
    }

    if (radioRcsServiceImpl[slotId].mtkRcsIndication) {
        // remove null terminator before report to user
        std::vector<uint8_t> msgData = std::vector<uint8_t>(sipInfo->data, &sipInfo->data[sipInfo->dataLength-1]);

        ndk::ScopedAStatus status = radioRcsServiceImpl[slotId].mtkRcsIndication->sipMessageReceived(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                sipInfo->reqId, sipInfo->connId, msgData);
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]sipMessageInd binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogW(LOG_TAG, "[%d][RCS]sipMessageInd mtkRcsIndication is null", slotId);
    }

    return 0;
}

int mtkRadioExRcs::deregistraionInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "[%d][RCS]deregistraionInd", slotId);

    if (radioRcsServiceImpl[slotId].mtkRcsIndication) {
        ndk::ScopedAStatus status = radioRcsServiceImpl[slotId].mtkRcsIndication->deregistrationAttempt(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!status.isOk()) {
            mtkLogE(LOG_TAG, "[%d][RCS]deregistraionInd binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, status, android::RCS_MTK);
        }
    } else {
        mtkLogW(LOG_TAG, "[%d][RCS]deregistraionInd mtkRcsIndication is null", slotId);
    }

    return 0;
}

int mtkRadioExRcs::clearMtkRcsResponseAndIndications(unsigned int slotId) {
    mtkLogD(LOG_TAG, "[%s][%d]", __FUNCTION__, slotId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioRcsServiceImpl[realSlotId].mtkRcsResponse = NULL;
    radioRcsServiceImpl[realSlotId].mtkRcsIndication = NULL;
    return 0;
}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addRchLock(std::shared_ptr<vendor::mediatek::hardware::mtkradioex::rcs::MtkRadioRcs> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_RCS_PORT_BASE 57000
static pthread_t gThreadIds[MAX_SIM_COUNT];


void* addRcpVsockRadioRcsAidlService(void* arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_RCS_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", RADIO_RCS_PORT_BASE + sloid, sloid);

    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::rcs::MtkRadioRcs> radioRcsAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::rcs::MtkRadioRcs>(sloid);
    addRchLock(radioRcsAidl);
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioRcsAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}
#endif

void publicRadioRcsAidlService(unsigned int slotId) {
    /*std::shared_ptr<vendor::mediatek::hardware::mtkradioex::rcs::MtkRadioRcs> mtkRadioRcsAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::rcs::MtkRadioRcs>(slotId);
    gPublishedHals.push_back(mtkRadioRcsAidl);
    const char* descriptor = vendor::mediatek::hardware::mtkradioex::rcs::MtkRadioRcs::descriptor;
    const std::string instance = std::string(descriptor != NULL ? descriptor : "")
        + "/slot" + std::to_string(slotId + 1);
    binder_status_t status =
        AServiceManager_addService(mtkRadioRcsAidl->asBinder().get(), instance.c_str());
    mtkLogD(LOG_TAG, "[%s][%d] status:%d", __FUNCTION__, slotId, status);*/
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    // add service for android client
    if (pthread_create(&gThreadIds[slotId], NULL,
            addRcpVsockRadioRcsAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(slotId))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioRcs");
    }
#endif
}

