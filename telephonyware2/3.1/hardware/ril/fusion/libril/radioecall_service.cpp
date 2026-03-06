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
#include <telephony/mtk_ril_ivt.h>
#include <ril_internal.h>
#include <ril_service.h>
#include <rilAidlUtils.h>
#include <mtk_log.h>
#include <mtk_properties.h>
#include <radioecall_service.h>
#include <libmtkrilutils.h>
#include <limits.h>

#include <binder/ProcessState.h>
#include <android/binder_manager.h>

#undef LOG_TAG
#define LOG_TAG "RILC"

using rilAidlUtils::retChecker;
namespace aidl_radio = ::aidl::android::hardware::radio;

struct RadioEcallImpl {
    std::shared_ptr<aidl_mtk_radio_ecall::IMtkRadioExEcallResponse> mtkEcallResponse;
    std::shared_ptr<aidl_mtk_radio_ecall::IMtkRadioExEcallIndication> mtkEcallIndication;
};

RadioEcallImpl radioEcallServiceImpl[MAX_SIM_COUNT * (android::CLIENT_TBOX + 1)];

int mtkRadioExEcall::resetIvsResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioEcallServiceImpl[slotId].mtkEcallResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioEcallServiceImpl[%d].mtkEcallResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioEcallServiceImpl[slotId].mtkEcallResponse->resetIvsResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int mtkRadioExEcall::setMSDResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioEcallServiceImpl[slotId].mtkEcallResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioEcallServiceImpl[%d].mtkEcallResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioEcallServiceImpl[slotId].mtkEcallResponse->setMSDResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int mtkRadioExEcall::setTestNumResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioEcallServiceImpl[slotId].mtkEcallResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioEcallServiceImpl[%d].mtkEcallResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioEcallServiceImpl[slotId].mtkEcallResponse->setTestNumResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int mtkRadioExEcall::setReconfNumResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioEcallServiceImpl[slotId].mtkEcallResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioEcallServiceImpl[%d].mtkEcallResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioEcallServiceImpl[slotId].mtkEcallResponse->setReconfNumResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int mtkRadioExEcall::makeFastEcallResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioEcallServiceImpl[slotId].mtkEcallResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioEcallServiceImpl[%d].mtkEcallResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioEcallServiceImpl[slotId].mtkEcallResponse->makeFastEcallResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int mtkRadioExEcall::setEmsdPriResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioEcallServiceImpl[slotId].mtkEcallResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioEcallServiceImpl[%d].mtkEcallResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioEcallServiceImpl[slotId].mtkEcallResponse->setEmsdPriResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int mtkRadioExEcall::setNadDeregTimeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioEcallServiceImpl[slotId].mtkEcallResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioEcallServiceImpl[%d].mtkEcallResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioEcallServiceImpl[slotId].mtkEcallResponse->setNadDeregTimeResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int mtkRadioExEcall::setOprtModeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioEcallServiceImpl[slotId].mtkEcallResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioEcallServiceImpl[%d].mtkEcallResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioEcallServiceImpl[slotId].mtkEcallResponse->setOprtModeResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int mtkRadioExEcall::getOprtModeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioEcallServiceImpl[slotId].mtkEcallResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioEcallServiceImpl[%d].mtkEcallResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    int mode = 0;
    int numInts = responselen / sizeof(int);
    if (response == NULL || numInts != 1) {
            mtkLogE(LOG_TAG, "getOprtModeResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            mode = pInt[0];
        }

    ndk::ScopedAStatus ret = radioEcallServiceImpl[slotId].mtkEcallResponse->getOprtModeResponse(
            responseInfo, mode);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int mtkRadioExEcall::ecallIndicationInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s, slotId:%d, tboxSlotId:%d", __FUNCTION__, slotId, tboxSlotId);
    if (radioEcallServiceImpl[tboxSlotId].mtkEcallIndication != NULL) {
        mtkLogD(LOG_TAG, "ecallIndicationInd");

        if (response == NULL || responselen != sizeof(RIL_Ecall_Unsol_Indications)) {
            if (response == NULL) {
                mtkLogE(LOG_TAG, "handleEcallIndication invalid response: NULL");
            } else {
                mtkLogE(LOG_TAG, "handleEcallIndication: invalid response length %d, expecting %d",
                    sizeof(RIL_Ecall_Unsol_Indications), responselen);
            }
            return 0;
        }

        RIL_Ecall_Unsol_Indications *p_cur = (RIL_Ecall_Unsol_Indications *)response;
        ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::EcallMsg ecallMsgInd;
        ecallMsgInd.call_id = p_cur->call_id;
        ecallMsgInd.indType = (int)p_cur->ind;

        ndk::ScopedAStatus ret = radioEcallServiceImpl[tboxSlotId].mtkEcallIndication->ecallIndicationInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), ecallMsgInd);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-ecallIndicationInd: radioEcallServiceImpl[%d]->networkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}


bool dispatchSetMsd(int serial, int tboxSlotId, int request, RIL_ECallSetMSD& setMsd) {
    android::RequestInfo *pRI = android::addRequestToList(serial, tboxSlotId, request);
    mtkLogD(LOG_TAG, "dispatchSetMsd");
    if (pRI == NULL) {
        return false;
    }

    CALL_ONREQUEST(pRI->pCI->requestNumber, &setMsd, sizeof(setMsd), pRI, pRI->socket_id);
    return true;
}

bool dispatchSetNum(int serial, int tboxSlotId, int request, RIL_ECallSetNum& setNum) {
    android::RequestInfo *pRI = android::addRequestToList(serial, tboxSlotId, request);
    mtkLogD(LOG_TAG, "dispatchSetNum");
    if (pRI == NULL) {
        return false;
    }

    CALL_ONREQUEST(pRI->pCI->requestNumber, &setNum, sizeof(setNum), pRI, pRI->socket_id);
    return true;
}

bool dispatchFastEcall(int serial, int tboxSlotId, int request, RIL_ECallReqMsg& reqMsg) {
    mtkLogD(LOG_TAG, "dispatchFastEcall");
    android::RequestInfo *pRI = android::addRequestToList(serial, tboxSlotId, request);
    if (pRI == NULL) {
        return false;
    }

    CALL_ONREQUEST(pRI->pCI->requestNumber, &reqMsg, sizeof(reqMsg), pRI, pRI->socket_id);
    return true;
}

//mtk radio

namespace vendor::mediatek::hardware::mtkradioex::ecall {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::ecall;
constexpr auto ok = &ScopedAStatus::ok;

MtkRadioEcall::MtkRadioEcall(int slotid) {
    mSlotId = slotid;
}

ScopedAStatus MtkRadioEcall::resetIvs(int32_t in_serial, int32_t in_clientId) {
    mtkLogD(LOG_TAG, "resetIvs: serial:%d slotId:%d clientId:%d", in_serial, mSlotId, in_clientId);
    if (!checkRequestClientIdSupport(in_clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(in_clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(in_serial, mSlotId, RIL_REQUEST_ECALL_RESET_IVS);
    return ok();
}

ScopedAStatus MtkRadioEcall::setMSD(int32_t in_serial,
    const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetMSD& in_setMSD, int32_t in_clientId) {
    mtkLogD(LOG_TAG, "setMSD: serial:%d slotId:%d clientId:%d", in_serial, mSlotId, in_clientId);
    if (!checkRequestClientIdSupport(in_clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(in_clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    RIL_ECallSetMSD eCallSetMsd;
    memset(&eCallSetMsd, 0, sizeof(eCallSetMsd));
    eCallSetMsd.call_id = in_setMSD.call_id;
    eCallSetMsd.length = in_setMSD.length;

    const unsigned char *msd_data = reinterpret_cast<const unsigned char *>(in_setMSD.msd_data.c_str());
    int digitLimit= MIN((eCallSetMsd.length), MSD_MAX_LENGTH);
    eCallSetMsd.msd_data = (unsigned char *)alloca(digitLimit);

    for(int digitCount = 0 ; digitCount < digitLimit; digitCount++) {
        eCallSetMsd.msd_data[digitCount] = msd_data[digitCount];
    }

    dispatchSetMsd(in_serial, mSlotId, RIL_REQUEST_ECALL_SET_MSD, eCallSetMsd);
    return ok();
}

ScopedAStatus MtkRadioEcall::setTestNum(int32_t in_serial,
    const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum& in_testNum, int32_t in_clientId) {
    mtkLogD(LOG_TAG, "setTestNum: serial:%d slotId:%d clientId:%d", in_serial, mSlotId, in_clientId);
    if (!checkRequestClientIdSupport(in_clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(in_clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    mtkLogD(LOG_TAG, "setTestNum: in_testNum arg_num:%d type:%d address:%s length:%d",
        in_testNum.arg_num, in_testNum.type, in_testNum.address.c_str(), in_testNum.address.length());
    RIL_ECallSetNum setNum;
    memset (&setNum, 0, sizeof(setNum));
    setNum.arg_num = in_testNum.arg_num;
    setNum.type = in_testNum.type;
    setNum.address = (char *)alloca(128* sizeof(char));
    memcpy(setNum.address, in_testNum.address.c_str(), in_testNum.address.length());
    setNum.address[in_testNum.address.length()] = '\0';
    mtkLogD(LOG_TAG, "setTestNum: setNum arg_num:%d type:%d address:%s", setNum.arg_num, setNum.type, setNum.address);

    dispatchSetNum(in_serial, mSlotId, RIL_REQUEST_ECALL_SET_TEST_NUM, setNum);
    return ok();
}

ScopedAStatus MtkRadioEcall::setReconfNum(int32_t in_serial,
    const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum& in_reconfNum, int32_t in_clientId) {
    mtkLogD(LOG_TAG, "setReconfNum: serial:%d slotId:%d clientId:%d", in_serial, mSlotId, in_clientId);
    if (!checkRequestClientIdSupport(in_clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(in_clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    RIL_ECallSetNum setNum;
    memset(&setNum, 0, sizeof(setNum));
    setNum.arg_num = in_reconfNum.arg_num;
    setNum.type = in_reconfNum.type;
    setNum.address = (char *)alloca(128* sizeof(char));
    memcpy(setNum.address, in_reconfNum.address.c_str(), in_reconfNum.address.length());
    setNum.address[in_reconfNum.address.length()] = '\0';

    dispatchSetNum(in_serial, mSlotId, RIL_REQUEST_ECALL_SET_RECONF_NUM, setNum);
    return ok();
}

ScopedAStatus MtkRadioEcall::makeFastEcall(int32_t in_serial,
    const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallReqMsg& in_reqMsg, int32_t in_clientId) {
    mtkLogD(LOG_TAG, "makeFastEcall: serial:%d slotId:%d clientId:%d", in_serial, mSlotId, in_clientId);
    if (!checkRequestClientIdSupport(in_clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(in_clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    mtkLogD(LOG_TAG, "makefastecall in category=%d, varient=%d, address=%s, length=%d, domain=%d",
        (int)in_reqMsg.ecall_cat, (int)in_reqMsg.ecall_variant, in_reqMsg.address.c_str(), in_reqMsg.length, (int)in_reqMsg.domain);

    RIL_ECallReqMsg eCallReqMsg;
    memset(&eCallReqMsg, 0, sizeof(eCallReqMsg));
    eCallReqMsg.ecall_cat = (RIL_ECall_Category) (int)in_reqMsg.ecall_cat;
    eCallReqMsg.ecall_variant = (RIL_ECall_Variant) (int)in_reqMsg.ecall_variant;
    eCallReqMsg.address = (char *)alloca(128* sizeof(char));
    memcpy(eCallReqMsg.address, in_reqMsg.address.c_str(), in_reqMsg.address.length());
    eCallReqMsg.address[in_reqMsg.address.length()] = '\0';
    eCallReqMsg.length = in_reqMsg.length;

    mtkLogD(LOG_TAG, "makefastecall out category=%d, varient=%d, address=%s, length=%d",
        eCallReqMsg.ecall_cat, eCallReqMsg.ecall_variant, eCallReqMsg.address, eCallReqMsg.length);

    int digitLimit= MIN((eCallReqMsg.length), MSD_MAX_LENGTH);
    eCallReqMsg.msd_data = (unsigned char *)alloca(digitLimit);

    for(int digitCount = 0 ; digitCount < digitLimit; digitCount++) {
        eCallReqMsg.msd_data[digitCount] = in_reqMsg.msd_data[digitCount];
        mtkLogD(LOG_TAG, "makefastecall msd_data[%d]=%d", digitCount, eCallReqMsg.msd_data[digitCount]);
    }
    eCallReqMsg.msd_data[digitLimit] = '\0';

    eCallReqMsg.domain = (RIL_Dial_Domain) (int)in_reqMsg.domain;

    dispatchFastEcall(in_serial, mSlotId, RIL_REQUEST_ECALL_FAST_MAKE_ECALL, eCallReqMsg);
    return ok();
}

ScopedAStatus MtkRadioEcall::setEmsdPri(int32_t in_serial,
    const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallPRI& in_pri, int32_t in_clientId) {
    mtkLogD(LOG_TAG, "setEmsdPri: serial:%d slotId:%d clientId:%d", in_serial, mSlotId, in_clientId);
    if (!checkRequestClientIdSupport(in_clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(in_clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(in_serial, mSlotId, RIL_REQUEST_ECALL_SET_PRI,
        4, in_pri.data1, in_pri.data2, in_pri.data3, in_pri.data4);
    return ok();
}

ScopedAStatus MtkRadioEcall::setNadDeregTime(int32_t in_serial,
    const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallTime& in_time, int32_t in_clientId) {
    mtkLogD(LOG_TAG, "setNadDeregTime: serial:%d slotId:%d clientId:%d", in_serial, mSlotId, in_clientId);
    if (!checkRequestClientIdSupport(in_clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(in_clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(in_serial, mSlotId, RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME,
        4, in_time.purpose, in_time.mode, in_time.timer1, in_time.timer2);
    return ok();
}

ScopedAStatus MtkRadioEcall::setOprtMode(int32_t in_serial,
    int32_t in_mode, int32_t in_clientId) {
    mtkLogD(LOG_TAG, "setOprtMode: %d serial:%d slotId:%d clientId:%d", in_mode, in_serial, mSlotId, in_clientId);
    if (!checkRequestClientIdSupport(in_clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(in_clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(in_serial, mSlotId, RIL_REQUEST_ECALL_SET_OPRT_MODE,
        1, in_mode);
    return ok();
}

ScopedAStatus MtkRadioEcall::getOprtMode(int32_t in_serial, int32_t in_clientId) {
    mtkLogD(LOG_TAG, "getOprtMode: serial:%d slotId:%d clientId:%d", in_serial, mSlotId, in_clientId);
    if (!checkRequestClientIdSupport(in_clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(in_clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(in_serial, mSlotId, RIL_REQUEST_ECALL_GET_OPRT_MODE);
    return ok();
}

ScopedAStatus MtkRadioEcall::setResponseFunctionsMtk(const std::shared_ptr<aidl::IMtkRadioExEcallResponse>& radioResponse, const std::shared_ptr<aidl::IMtkRadioExEcallIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk ecall setResponseFunctions, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk ecall setResponseFunctions: got lock %d", mSlotId);

    radioEcallServiceImpl[mSlotId].mtkEcallResponse = radioResponse;
    radioEcallServiceImpl[mSlotId].mtkEcallIndication = radioIndication;
    plusCounterMtkRadioNumOne(mSlotId);

    rilAidlUtils::setMtkResponseNumberToOne(rilAidlUtils::ECALL_INDEX, mSlotId);
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk ecall setResponseFunctions: release lock %d", mSlotId);

    // client is connected. Send initial indications.
    if (rilAidlUtils::checkIfSetAllMtkResponseDone(mSlotId) == 1) {
        android::onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::RADIO_MTK);
    }

    return ok();
}

}

int mtkRadioExEcall::clearMtkEcallResponseAndIndications(unsigned int slotId) {
    radioEcallServiceImpl[slotId].mtkEcallResponse = NULL;
    radioEcallServiceImpl[slotId].mtkEcallIndication = NULL;
    return 0;
}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;

void publicRadioEcallAidlService(unsigned int sloid) {
    // add slotid for yocto client
    unsigned int tboxSlotid = sloid + android::CLIENT_TBOX * MAX_SIM_COUNT;

    if (sloid == 0) {

        // add service for yocto client
        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall> mtkTboxRadioEcallAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall>(tboxSlotid);
        gPublishedHals.push_back(mtkTboxRadioEcallAidl);
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall::descriptor + "/tboxSlot1";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioEcallAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox ecall addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 1) {

        // add service for yocto client
        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall> mtkTboxRadioEcallAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall>(tboxSlotid);
        gPublishedHals.push_back(mtkTboxRadioEcallAidl);
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall::descriptor + "/tboxSlot2";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioEcallAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox ecall addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 2) {

        // add service for yocto client
        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall> mtkTboxRadioEcallAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall>(tboxSlotid);
        gPublishedHals.push_back(mtkTboxRadioEcallAidl);
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall::descriptor + "/tboxSlot3";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioEcallAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox ecall addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 3) {

        // add service for yocto client
        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall> mtkTboxRadioEcallAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall>(tboxSlotid);
        gPublishedHals.push_back(mtkTboxRadioEcallAidl);
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::ecall::MtkRadioEcall::descriptor + "/tboxSlot4";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioEcallAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox ecall addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    }
}
