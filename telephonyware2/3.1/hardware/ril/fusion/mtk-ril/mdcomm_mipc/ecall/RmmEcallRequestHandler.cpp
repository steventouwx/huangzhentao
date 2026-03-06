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

#include "RmmEcallRequestHandler.h"

#define RFX_LOG_TAG "RmmEcallReqHandler"

#define MAX_ECC_NUM          16
#define MAX_ECC_BUF_SIZE     (MAX_ECC_NUM * 8 + 1)
#define ECC_VDBG          0

static const char PROPERTY_NW_ECC_LIST[MAX_SIM_COUNT][MAX_PROP_CHARS] = {
    "vendor.ril.ecc.service.category.list",
    "vendor.ril.ecc.service.category.list.1",
    "vendor.ril.ecc.service.category.list.2",
    "vendor.ril.ecc.service.category.list.3",
};

using std::string;
using android::NO_ERROR;

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmEcallRequestHandler, RIL_CMD_PROXY_1);

RmmEcallRequestHandler::RmmEcallRequestHandler(int slot_id,
        int channel_id): RfxBaseHandler(slot_id, channel_id) {
    const int request_id_list[] = {
        RFX_MSG_REQUEST_ECALL_SET_IVS,
        RFX_MSG_REQUEST_ECALL_SET_TEST_NUM,
        RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM,
        RFX_MSG_REQUEST_ECALL_SET_MSD,
        RFX_MSG_REQUEST_ECALL_SET_PSAP,
        RFX_MSG_REQUEST_ECALL_MAKE_ECALL,
        RFX_MSG_REQUEST_ECALL_IVS_PUSH_MSD,
        RFX_MSG_REQUEST_ECALL_PSAP_PULL_MSD,
        RFX_MSG_REQUEST_ECALL_SET_MSD_MODE,
        RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL,
        RFX_MSG_REQUEST_ECALL_CTRL_SEQUENCE,
        RFX_MSG_REQUEST_ECALL_RESET_IVS,
        RFX_MSG_REQUEST_ECALL_QUERY_ECALL_NUM,
        RFX_MSG_REQUEST_ECALL_SET_PRI,
        RFX_MSG_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME,
        RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE,
        RFX_MSG_REQUEST_ECALL_GET_SIM_INFO,
        RFX_MSG_REQUEST_ECALL_SET_OPRT_MODE,
        RFX_MSG_REQUEST_ECALL_GET_OPRT_MODE
    };

    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(int));

    //init member variables
    mEccListWithCard = "";
    mEccListNoCard = "";
    mIsSimInsert = false;
    if (ECC_VDBG == 1) {
        mEccListWithCard = String8("+111,2+222,+333,4+444");
        mEccListNoCard = String8("+555,6+666");
    }
}

RmmEcallRequestHandler::~RmmEcallRequestHandler() {
}

void RmmEcallRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s", RFX_ID_TO_STR(request));

    int ret = 0;
    switch(request) {
        case RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL:
            ret = handleEcallFastMakeEcall(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_MSD:
            ret = handleEcallUpdateMsd(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_RESET_IVS:
            ret = handleEcallResetIvs(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE:
            ret = handleEcallSetRegState(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_OPRT_MODE:
            handleEcallSetOprtMode(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_GET_OPRT_MODE:
            handleEcallGetOprtMode(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_TEST_NUM:
            ret = handleEcallSetTestAddr(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM:
            ret = handleEcallSetReconfAddr(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_PRI:
            ret = handleEcallSetAddrPri(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }

    if (ret != 0) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
    }
}

void RmmEcallRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    if (RfxRilUtils::isUserLoad() != 1) {
        logD(RFX_LOG_TAG, "onHandleResponse: %s", RFX_ID_TO_STR(msg->getId()));
    }

    RfxMipcData* data = msg->getMipcData();
    if (data == NULL) {
        logE(RFX_LOG_TAG, "onHandleResponse: getMipcData() return null");
        responseVoidDataToTcl(msg, RIL_E_GENERIC_FAILURE);
        return;
    }

    logD(RFX_LOG_TAG, "onHandleResponse MipcMsgId:%d, MipcSlotId:%d result:%d",
             data->getMipcMsgId(), data->getMipcSlotId(), data->getResult());
    int ret = 0;
    switch (msg->getId()) {
        case RFX_MSG_REQUEST_ECALL_GET_OPRT_MODE:
            requestGetOprtModeResponse(msg);
        default:
            responseVoidDataToTcl(msg);
            break;
    }
    if (ret != 0) {
        responseVoidDataToTcl(msg, RIL_E_GENERIC_FAILURE);
    }
}

void RmmEcallRequestHandler::requestGetOprtModeResponse(const sp<RfxMclMessage>& msg) {
    int response[1] = {0};
    sp<RfxMclMessage> responseMsg;

    RfxMipcData* rsp_data = msg->getMipcData();
    unsigned int  result = rsp_data->getResult();

    if (MIPC_RESULT_SUCCESS != result) {
        logE(RFX_LOG_TAG, "requestGetOprtModeResponse result=%d", result);
        responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
        RfxIntsData(response, 1), msg, false);
        responseToTelCore(responseMsg);
    }

    response[0] = (uint8_t)(rsp_data->getMipcUint8Val(MIPC_ECALL_IVS_GET_ECALL_MODE_CNF_T_ECALL_MODE, 0));
    logD(RFX_LOG_TAG, "requestGetOprtModeResponse, oprtMode = %d", response[0]);

    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(response, 1), msg, false);
    responseToTelCore(responseMsg);
}

void RmmEcallRequestHandler::responseVoidDataToTcl(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    RIL_Errno err = (data != NULL && data->getResult() == MIPC_RESULT_SUCCESS) ?
            RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> mclResponse =
            RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
}

void RmmEcallRequestHandler::responseVoidDataToTcl(const sp<RfxMclMessage>& msg, RIL_Errno err) {
    sp<RfxMclMessage> mclResponse =
        RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
}

int RmmEcallRequestHandler::handleEcallFastMakeEcall(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleEcallFastMakeEcall with clientId: %d, with token: %d",
        msg->getClientId(), msg->getToken());

    int rVal = 0;
    mipc_ecall_type_const_enum ecall_type = mipc_ecall_type_const_enum::mipc_ecall_type_const_NONE;
    mipc_ecall_msd_format_const_enum ecall_format = mipc_ecall_msd_format_const_enum::MIPC_ECALL_MSD_FORMAT_BINARY;
    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;
    RIL_ECallReqMsg *eCallReqMsg = NULL; //middle struct for data from parcel to md
    eCallReqMsg = (RIL_ECallReqMsg*)(msg->getData()->getData());

    if (msg->getRilToken() == NULL || eCallReqMsg->address == NULL) {
        logD(RFX_LOG_TAG, "handleEcallFastMakeEcall check RilToken or address error!");
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    logD(RFX_LOG_TAG, "eCallReqMsg category: %d, variant: %d,  address: [%s], msd len: %d, domain = %d",
        (int) eCallReqMsg->ecall_cat, (int) eCallReqMsg->ecall_variant, (eCallReqMsg->address == NULL ? "" : eCallReqMsg->address),
        eCallReqMsg->length, (int) eCallReqMsg->domain);

    for (int i = 0; i < eCallReqMsg->length; i++) {
        logD(RFX_LOG_TAG, "msd_data[%d] = %d", i, eCallReqMsg->msd_data[i]);
    }

    //********************** prepare transfer data *************************
    switch(eCallReqMsg->ecall_variant) {
        case ECALL_TEST:
            ecall_type = MIPC_ECALL_TYPE_TEST;
            break;
        case ECALL_EMERGENCY:
            if(eCallReqMsg->ecall_cat == EMER_CAT_MANUAL_ECALL) {
                ecall_type = MIPC_ECALL_TYPE_MANUAL;
                break;
            } else if (eCallReqMsg->ecall_cat == EMER_CAT_AUTO_ECALL) {
                ecall_type = MIPC_ECALL_TYPE_AUTO;
                break;
            } else {
                break;
            }
        case ECALL_RECONFIG:
            ecall_type = MIPC_ECALL_TYPE_RECOFIG;
            break;
        default:
            break;
    }

    logD(RFX_LOG_TAG, "ecall_type: %d", (int) ecall_type);
    /*check call address, query ecall num*/
    if ((eCallReqMsg->address != NULL) && (strcmp(eCallReqMsg->address, "") != 0) && (eCallReqMsg->ecall_variant != ECALL_EMERGENCY)) {
        logD(RFX_LOG_TAG, "user has set eCall address(%s), query the number first!",eCallReqMsg->address);
        if (strspn(eCallReqMsg->address, "+*#0123456789") != strlen(eCallReqMsg->address)) {
            logD(RFX_LOG_TAG, "Start to check eCall address (sip).");
            rVal = checkEccNumberAndServiceCategory(eCallReqMsg->address, msg, eCallReqMsg->ecall_variant, 0);
            if (rVal == -1) {
                logD(RFX_LOG_TAG, "handleEcallFastMakeEcall checkEccNumberAndServiceCategory error!");
                responseVoidDataToTcl(msg, RIL_E_CANCELLED);
                return 0;
            }
        } else {
            logD(RFX_LOG_TAG, "Start to check eCall address (num).");
            rVal = checkEccNumberAndServiceCategory(eCallReqMsg->address, msg, eCallReqMsg->ecall_variant, 1);
            if (rVal == -1) {
                logD(RFX_LOG_TAG, "handleEcallFastMakeEcall checkEccNumberAndServiceCategory error!");
                responseVoidDataToTcl(msg, RIL_E_CANCELLED);
                return 0;
            }
        }
    }

    //************************* send data with mipc *************************
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_IVS_ONEKEY_ECALL_REQ, msg->getSlotId());
    data->addMipcTlvUint8(MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_TYPE, ecall_type);
    if (eCallReqMsg->length > 0) {
        data->addMipcTlvUint8(MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_MSD_FORMAT, ecall_format);
        if (eCallReqMsg->msd_data) {
            data->addMipcTlv(MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_MSD,
                (eCallReqMsg->length < MIPC_MAX_ECALL_MSD_DATA_LEN ? eCallReqMsg->length : MIPC_MAX_ECALL_MSD_DATA_LEN), eCallReqMsg->msd_data);
        } else {
            logD(RFX_LOG_TAG, "handleEcallFastMakeEcall msd null and return!");
            responseVoidDataToTcl(msg, RIL_E_CANCELLED);
            return 0;
        }
    }
    data->addMipcTlvUint32(MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_DOMAIN, (mipc_call_dial_domain_const_enum)eCallReqMsg->domain);

    logD(RFX_LOG_TAG, "MIPC_CALL_IVS_ONEKEY_ECALL_REQ");
    return callToMipcMsgAsync(msg, data);
}

int RmmEcallRequestHandler::handleEcallUpdateMsd(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleEcallUpdateMsd with clientId: %d, with token: %d",
        msg->getClientId(), msg->getToken());

    mipc_ecall_msd_format_const_enum msd_format = mipc_ecall_msd_format_const_enum::MIPC_ECALL_MSD_FORMAT_BINARY;
    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;
    RIL_ECallSetMSD *msd = NULL;
    msd = (RIL_ECallSetMSD*)(msg->getData()->getData());
    logD(RFX_LOG_TAG, "handleEcallUpdateMsd call id: %d,msd data: [%s], length: %d",
        msd->call_id, msd->msd_data, msd->length);

    if (msg->getRilToken() == NULL) {
        logD(RFX_LOG_TAG, "handleEcallUpdateMsd check RilToken error!");
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    //************************* send data with mipc *************************
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_UPDATE_MSD_REQ, msg->getSlotId());
    data->addMipcTlvUint8(MIPC_ECALL_IVS_UPDATE_MSD_REQ_T_MSD_FORMAT, msd_format);
    if (msd->msd_data) {
        data->addMipcTlv(MIPC_ECALL_IVS_UPDATE_MSD_REQ_T_MSD_DATA,
            (msd->length < MIPC_MAX_ECALL_MSD_DATA_LEN ? msd->length : MIPC_MAX_ECALL_MSD_DATA_LEN), msd->msd_data);
    }

    logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_UPDATE_MSD_REQ");
    return callToMipcMsgAsync(msg, data);
}

int RmmEcallRequestHandler::handleEcallResetIvs(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleEcallResetIvs with clientId: %d, with token: %d",
        msg->getClientId(), msg->getToken());

    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;

    if (msg->getRilToken() == NULL) {
        logD(RFX_LOG_TAG, "handleEcallResetIvs check RilToken error!");
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    //************************* send data with mipc *************************
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_RESET_REQ, msg->getSlotId());
    logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_RESET_REQ");
    return callToMipcMsgAsync(msg, data);
}

int RmmEcallRequestHandler::handleEcallSetRegState(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleEcallSetRegState with clientId: %d, with token: %d",
        msg->getClientId(), msg->getToken());

    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;
    int simType = handleEcallGetSimInfo(msg->getSlotId());
    mipc_nw_register_mode_const_enum reg_state = mipc_nw_register_mode_const_enum::MIPC_NW_REGISTER_MODE_DEREGISTER;
    int *pInt = (int*)msg->getData()->getData();  // pInt[0]: arg_num; pInt[1]: state;
    sp<RfxMclMessage> response;
    int rInt[1] = {0};

    if (simType == -1 || msg->getRilToken() == NULL) {
        logD(RFX_LOG_TAG, "handleEcallSetRegState check RilToken error!");
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    if (pInt[1] == 1) {
        reg_state = mipc_nw_register_mode_const_enum::MIPC_NW_REGISTER_MODE_AUTOMATIC;
    }
    logD(RFX_LOG_TAG, "simType:%d, reg_state:%d", simType, reg_state);

    //************************* send data with mipc *************************
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_SET_REGISTER_STATE_REQ, msg->getSlotId());
    reqData->addMipcTlvUint8(MIPC_NW_SET_REGISTER_STATE_REQ_T_MODE, reg_state);
    if (pInt[1] == 2) {
        reqData->addMipcTlvUint8(MIPC_NW_SET_REGISTER_STATE_REQ_T_CTRL_MODE, pInt[1]);
    }
    logD(RFX_LOG_TAG, "MIPC_NW_SET_REGISTER_STATE_REQ");
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData, 15*60*1000/*timeout 15m*/);

    if (cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(RFX_LOG_TAG, "handleEcallSetRegState: result[%d]", cnfData->getResult());
        rInt[0] = -1;
        response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_GENERIC_FAILURE, RfxIntsData(pInt, 1), msg);
        responseToTelCore(response);
    } else {
        if (simType == 1) {
            rInt[0] = 0;
            response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_SUCCESS, RfxIntsData(pInt, 1), msg);
            responseToTelCore(response);
        } else {
            rInt[0] = 1;
            response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_SUCCESS, RfxIntsData(pInt, 1), msg);
            responseToTelCore(response);
        }
    }

    return 0;
}

int RmmEcallRequestHandler::handleEcallSetOprtMode(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleEcallSetOprtMode with clientId: %d, with token: %d",
        msg->getClientId(), msg->getToken());

    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;
    mipc_ecall_mode_type_const_enum eCall_mode = mipc_ecall_mode_type_const_enum::MIPC_ECALL_MODE_TYPE_ECALL_AND_NORMAL;
    int *pInt = (int*)msg->getData()->getData();  // pInt[0]: arg_num; pInt[1]: mode;
    sp<RfxMclMessage> response;

    if (msg->getRilToken() == NULL) {
        logD(RFX_LOG_TAG, "handleEcallSetOprtMode check RilToken error!");
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    if (pInt[0] == 1) {
        eCall_mode = mipc_ecall_mode_type_const_enum::MIPC_ECALL_MODE_TYPE_ECALL_ONLY;
    }
    logD(RFX_LOG_TAG, "mode:%d %d", pInt[0], pInt[1]);

    //************************* send data with mipc *************************
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_SET_ECALL_MODE_REQ, msg->getSlotId());
    data->addMipcTlvUint8(MIPC_ECALL_IVS_SET_ECALL_MODE_REQ_T_ECALL_MODE, eCall_mode);
    logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_SET_ECALL_MODE_REQ");
    return callToMipcMsgAsync(msg, data);
}

int RmmEcallRequestHandler::handleEcallGetOprtMode(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleEcallGetOprtMode with clientId: %d, with token: %d",
        msg->getClientId(), msg->getToken());

    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;

    if (msg->getRilToken() == NULL) {
        logD(RFX_LOG_TAG, "handleEcallGetOprtMode check RilToken error!");
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    //************************* send data with mipc *************************
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_GET_ECALL_MODE_REQ, msg->getSlotId());
    logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_GET_ECALL_MODE_REQ");
    return callToMipcMsgAsync(msg, data);
}

int RmmEcallRequestHandler::handleEcallGetSimInfo(int slotId) {
    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;
    //mipc_ecall_ivs_get_sim_info_struct result_ptr;
    //MEMSET(&result_ptr, 0, sizeof(mipc_ecall_ivs_get_sim_info_struct));
    int simType = 0;

    //************************* send data with mipc *************************
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_GET_SIM_INFO_REQ, slotId);
    logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_GET_SIM_INFO_REQ");
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData, 15*60*1000/*timeout 15m*/);

    if (cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(RFX_LOG_TAG, "handleEcallGetSimInfo: result[%d]", cnfData->getResult());
        return -1;
    } else {
        // sim_type: 0: normal; 1: ecall only; 2: ecall and normal;
        uint32_t simType = cnfData->getMipcUint32Val(MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_SIM_TYPE, 0);
        logD(RFX_LOG_TAG, "handleEcallGetSimInfo simType=%d", simType);
        return simType;
    }
}

int RmmEcallRequestHandler::handleEcallSetTestAddr(const sp<RfxMclMessage>& msg){
    logD(RFX_LOG_TAG, "handleEcallSetTestAddr with clientId: %d, with token: %d",
        msg->getClientId(), msg->getToken());

    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;
    mipc_call_dial_address_type_const_enum address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NONE;
    RIL_ECallSetNum *eCallSetNum = NULL; //middle struct for data from parcel to md
    eCallSetNum = (RIL_ECallSetNum*)(msg->getData()->getData());

    if (msg->getRilToken() == NULL) {
        logD(RFX_LOG_TAG, "handleEcallSetTestAddr check RilToken error!");
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    logD(RFX_LOG_TAG, "handleEcallSetTestAddr arg_num:%d, type:%d, address:%s\n",
        eCallSetNum->arg_num, eCallSetNum->type, eCallSetNum->address);
    int arg_num = eCallSetNum->arg_num;
    int type_t = eCallSetNum->type;
    char test_address[MIPC_MAX_DIAL_ADDRESS_LEN] = {0};
    memcpy(test_address, eCallSetNum->address, 
        strlen(eCallSetNum->address) < MIPC_MAX_DIAL_ADDRESS_LEN ? strlen(eCallSetNum->address) + 1 : MIPC_MAX_DIAL_ADDRESS_LEN);
    if (arg_num == 0) {
        logD(RFX_LOG_TAG, "clear both test num and uri\n");
        address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NONE;
    } else if (arg_num == 1 || arg_num == 2) {
        switch (type_t) {
            case 1: {
                address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_SIP_URI;
                break;
            }
            case 2: {
                address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NUMBER;
                break;
            }
            default:
                break;
        }
        logD(RFX_LOG_TAG, "set test num or uri,type(%d),data(%s)\n", address_type, eCallSetNum->address);
    }

    //************************* send data with mipc *************************
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_SET_TEST_ADDR_REQ, msg->getSlotId());
    if (eCallSetNum->address) {
        data->addMipcTlv(MIPC_ECALL_IVS_SET_TEST_ADDR_REQ_T_ADDRESS, strlen(test_address), test_address);
    }
    data->addMipcTlvUint32(MIPC_ECALL_IVS_SET_TEST_ADDR_REQ_T_ADDR_TYPE, address_type);
    logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_SET_TEST_ADDR_REQ");
    return callToMipcMsgAsync(msg, data);
}

int RmmEcallRequestHandler::handleEcallSetReconfAddr(const sp<RfxMclMessage>& msg){
    logD(RFX_LOG_TAG, "handleEcallSetReconfAddr with clientId: %d, with token: %d",
        msg->getClientId(), msg->getToken());

    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;
    mipc_call_dial_address_type_const_enum address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NONE;
    RIL_ECallSetNum *eCallSetNum = NULL; //middle struct for data from parcel to md
    eCallSetNum = (RIL_ECallSetNum*)(msg->getData()->getData());

    if (msg->getRilToken() == NULL) {
        logD(RFX_LOG_TAG, "handleEcallSetReconfAddr check RilToken error!");
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    logD(RFX_LOG_TAG, "handleEcallSetReconfAddr arg_num:%d, type:%d, address:%s\n",
        eCallSetNum->arg_num, eCallSetNum->type, eCallSetNum->address);
    int arg_num = eCallSetNum->arg_num;
    int type_t = eCallSetNum->type;
    char test_address[MIPC_MAX_DIAL_ADDRESS_LEN] = {0};
    memcpy(test_address, eCallSetNum->address, 
        strlen(eCallSetNum->address) < MIPC_MAX_DIAL_ADDRESS_LEN ? strlen(eCallSetNum->address) + 1 : MIPC_MAX_DIAL_ADDRESS_LEN);
    if (arg_num == 0) {
        logD(RFX_LOG_TAG, "clear both test num and uri\n");
        address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NONE;
    } else if (arg_num == 1 || arg_num == 2) {
        switch (type_t) {
            case 1: {
                address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_SIP_URI;
                break;
            }
            case 2: {
                address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NUMBER;
                break;
            }
            default:
                break;
        }
        logD(RFX_LOG_TAG, "set test num or uri,type(%d),data(%s)\n", address_type, eCallSetNum->address);
    }

    //************************* send data with mipc *************************
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ, msg->getSlotId());
    if (eCallSetNum->address) {
        data->addMipcTlv(MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ_T_ADDRESS, strlen(test_address), test_address);
    }
    data->addMipcTlvUint32(MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ_T_ADDR_TYPE, address_type);
    logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ");
    return callToMipcMsgAsync(msg, data);
}

int RmmEcallRequestHandler::handleEcallSetAddrPri(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleEcallSetAddrPri with clientId: %d, with token: %d",
        msg->getClientId(), msg->getToken());

    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;
    mipc_ecall_address_priority_class_const_enum first_pri = mipc_ecall_address_priority_class_const_enum::mipc_ecall_address_priority_class_const_NONE;
    mipc_ecall_address_priority_class_const_enum second_pri = mipc_ecall_address_priority_class_const_enum::mipc_ecall_address_priority_class_const_NONE;
    mipc_ecall_address_priority_class_const_enum third_pri = mipc_ecall_address_priority_class_const_enum::mipc_ecall_address_priority_class_const_NONE;
    mipc_ecall_address_priority_class_const_enum fourth_pri = mipc_ecall_address_priority_class_const_enum::mipc_ecall_address_priority_class_const_NONE;
    int *pInt = (int*)msg->getData()->getData();  // pInt[0]: arg_num; pInt[1]: state;
    sp<RfxMclMessage> response;
    int rInt[5] = {0};

    if (msg->getRilToken() == NULL) {
        logD(RFX_LOG_TAG, "handleEcallSetAddrPri check RilToken error!");
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    for (int i = 0 ; i < pInt[0] ; i++) {
        if (i == 0) {
            switch (pInt[i+1]) {
            case 1:
                first_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_URI;
                break;
            case 2:
                first_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_URI;
                break;
            case 3:
                first_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_NUM;
                break;
            case 4:
                first_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_NUM;
                break;
            default:
                break;
            }
        } else if (i == 1) {
            switch (pInt[i+1]) {
            case 1:
                second_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_URI;
                break;
            case 2:
                second_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_URI;
                break;
            case 3:
                second_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_NUM;
                break;
            case 4:
                second_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_NUM;
                break;
            default:
                break;
            }
        } else if (i == 2) {
            switch (pInt[i+1]) {
            case 1:
                third_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_URI;
                break;
            case 2:
                third_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_URI;
                break;
            case 3:
                third_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_NUM;
                break;
            case 4:
                third_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_NUM;
                break;
            default:
                break;
            }
        } else if (i == 3) {
            switch (pInt[i+1]) {
            case 1:
                fourth_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_URI;
                break;
            case 2:
                fourth_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_URI;
                break;
            case 3:
                fourth_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_NUM;
                break;
            case 4:
                fourth_pri = MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_NUM;
                break;
            default:
                break;
            }
        }
    }
    logD(RFX_LOG_TAG, "pri:%d %d %d %d", first_pri, second_pri, third_pri, fourth_pri);

    //************************* send data with mipc *************************
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_SET_ADDR_PRI_REQ, msg->getSlotId());
    data->addMipcTlvUint8(MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_FIRST_PRI, first_pri);
    data->addMipcTlvUint8(MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_SECOND_PRI, second_pri);
    data->addMipcTlvUint8(MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_THIRD_PRI, third_pri);
    data->addMipcTlvUint8(MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_FOURTH_PRI, fourth_pri);
    logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_SET_ADDR_PRI_REQ");
    return callToMipcMsgAsync(msg, data);
}

int RmmEcallRequestHandler::checkEccNumberAndServiceCategory(char* number, const sp<RfxMclMessage>& msg, RIL_ECall_Variant ecall_variant, int is_number)
{
    String8 dialNumber = String8::format("%s", number);
    bool isEcc = 0;//isEmergencyNumber(dialNumber);
    int sc = 0;
    mipc_api_result_enum ret = MIPC_API_RESULT_FAIL;
    //Parcel* p = new Parcel();
    int rVal = 0;

    if (isEcc) {
        sc = getServiceCategory(dialNumber);
    }

    if(isEcc == 0) {
        logD(RFX_LOG_TAG, "address(%s), config test/reconf number before make ecall!!\n", number);
        if (ecall_variant == ECALL_TEST) {
            mipc_call_dial_address_type_const_enum address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NUMBER;

            if ((is_number == 0) || (is_number == 1)) {
                if (is_number == 0) {
                    address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_SIP_URI;
                } else if (is_number == 1) {
                    address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NUMBER;
                }

                //************************* send data with mipc *************************
                sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_SET_TEST_ADDR_REQ, msg->getSlotId());
                if (number) {
                    reqData->addMipcTlv(MIPC_ECALL_IVS_SET_TEST_ADDR_REQ_T_ADDRESS, strlen(number), number);
                }
                reqData->addMipcTlvUint32(MIPC_ECALL_IVS_SET_TEST_ADDR_REQ_T_ADDR_TYPE, address_type);
                logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_SET_TEST_ADDR_REQ");
                sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData, 15*60*1000/*timeout 15m*/);

                if (cnfData->getResult() != MIPC_RESULT_SUCCESS) {
                    logD(RFX_LOG_TAG, "checkEccNumberAndServiceCategory MIPC_ECALL_IVS_SET_TEST_ADDR_REQ response error!");
                    responseVoidDataToTcl(msg, RIL_E_GENERIC_FAILURE);
                } else {
                    rVal = 0;
                    logD(RFX_LOG_TAG, "Set test_address:%s", (number==NULL? "": number));
                }
            } else {
                rVal = -1;
            }
        } else if (ecall_variant == ECALL_RECONFIG) {
            mipc_call_dial_address_type_const_enum address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NUMBER;

            if ((is_number == 0) || (is_number == 1)) {
                if (is_number == 0) {
                    address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_SIP_URI;
                } else if (is_number == 1) {
                    address_type = mipc_call_dial_address_type_const_enum::MIPC_CALL_DIAL_ADDRESS_TYPE_NUMBER;
                }

                //************************* send data with mipc *************************
                sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ, msg->getSlotId());
                if (number) {
                    reqData->addMipcTlv(MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ_T_ADDRESS, strlen(number), number);
                }
                reqData->addMipcTlvUint32(MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ_T_ADDR_TYPE, address_type);
                logD(RFX_LOG_TAG, "MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ");
                sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData, 15*60*1000/*timeout 15m*/);

                if (cnfData->getResult() != MIPC_RESULT_SUCCESS) {
                    logD(RFX_LOG_TAG, "checkEccNumberAndServiceCategory MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ response error!");
                    responseVoidDataToTcl(msg, RIL_E_GENERIC_FAILURE);
                } else {
                    rVal = 0;
                    logD(RFX_LOG_TAG, "Set reconf_address:%s", (number==NULL? "": number));
                }
            } else {
                rVal = -1;
            }
        } else {
            rVal = -1;
            logE(RFX_LOG_TAG, "Should not be here, not support config this eCall type\n");
        }
    } else if(isEcc == 1 && (sc == 32 || sc == 64)) {
        logD(RFX_LOG_TAG, "address is ecall emergency number, sc is %d\n", sc);

        //if(sc == 6) FastEcallType = 2;
        //else if(sc == 7) FastEcallType = 3;
        rVal = 0;
     } else {
        rVal = -1;
        logD(RFX_LOG_TAG, "other ECC number shall not be used in ecall!!\n");
    }
    return rVal;
}

bool RmmEcallRequestHandler::isEmergencyNumber(String8 number) {
    char eccListProp[MAX_ECC_BUF_SIZE] = {0};
    char* ecc = NULL;
    char* eccCategory = NULL;

    if (number.isEmpty()) {
        logD(RFX_LOG_TAG, "[%s] no match empty number", __FUNCTION__);
        return false;
    }

    // 1. Check ECC sync from AP
    String8 eccList = mIsSimInsert ? mEccListWithCard : mEccListNoCard;
    logD(RFX_LOG_TAG, "[%s] AP eccList: %s", __FUNCTION__, eccList.string());
    if (isEccMatchInList(number, eccList)) {
        logD(RFX_LOG_TAG, "[%s] match AP list ECC", __FUNCTION__);
        return true;
    }

    // 2. Check GSM ECC from SIM
    strncpy(eccListProp, (const char *)mGsmEcc.string(), MAX_ECC_BUF_SIZE - 1);
    if (strlen(eccListProp) > 0) {
        ecc = strtok(eccListProp, ",;");
        while (ecc != NULL) {
            if (strcmp(ecc, number.string()) == 0) {
                logD(RFX_LOG_TAG, "[%s] match GSM SIM ECC", __FUNCTION__);
                return true;
            }
            eccCategory = strtok(NULL, ",;");
            if (eccCategory == NULL)
                break;
            ecc = strtok(NULL, ",;");
        }
    }

    // 3. Check CDMA ECC from SIM, no C2K now
    // 4. Check ECC from Network
    memset(eccListProp, 0, sizeof(eccListProp));
    rfx_property_get(PROPERTY_NW_ECC_LIST[m_slot_id], eccListProp, "");
    if (strlen(eccListProp) > 0) {
        ecc = strtok(eccListProp, ",;");
        while (ecc != NULL) {
            if (strcmp(ecc, number.string()) == 0) {
                logD(RFX_LOG_TAG, "[%s] match network ECC", __FUNCTION__);
                return true;
            }
            eccCategory = strtok(NULL, ",;");
            if (eccCategory == NULL)
                break;
            ecc = strtok(NULL, ",;");
        }
    }

    // 5. Check default ECC from 3GPP spec
    String8 defaultEccList = mIsSimInsert ? String8("112,911") :
            String8("112,911,000,08,110,118,119,999");
    if (isEccMatchInList(number, defaultEccList)) {
        logD(RFX_LOG_TAG, "[%s] match default ECC", __FUNCTION__);
        return true;
    }

    logD(RFX_LOG_TAG, "[%s] no match ECC", __FUNCTION__);
    return false;
}

int RmmEcallRequestHandler::getServiceCategory(String8 number) {
    char eccListProp[MAX_ECC_BUF_SIZE] = {0};
    char* ecc = NULL;
    char* eccCategory = NULL;

    // get from Network ECC
    rfx_property_get(PROPERTY_NW_ECC_LIST[m_slot_id], eccListProp, "");
    if (strlen(eccListProp) > 0) {
        ecc = strtok(eccListProp, ",;");
        while (ecc != NULL) {
            eccCategory = strtok(NULL, ",;");
            if (eccCategory != NULL && strcmp(ecc, number.string()) == 0) {
                return atoi(eccCategory);
            }
            ecc = strtok(NULL, ",;");
        }
    }

    // get from SIM ECC
    strncpy(eccListProp, (const char *)mGsmEcc.string(), MAX_ECC_BUF_SIZE - 1);
    if (strlen(eccListProp) > 0) {
        ecc = strtok(eccListProp, ",;");
        while (ecc != NULL) {
            eccCategory = strtok(NULL, ",;");
            if (eccCategory != NULL && strcmp(ecc, number.string()) == 0 &&
                    atoi(eccCategory) > 0) {
                return atoi(eccCategory);
            }
            ecc = strtok(NULL, ",;");
        }
    }

    // get from AP configured ECC (format +112,+911,1+110,+119...)
    String8 eccListString = mIsSimInsert ? mEccListWithCard : mEccListNoCard;
    char* eccList = (char *)strdup(eccListString.string());
    if (eccList != NULL) {
        if (strlen(eccList) > 0) {
            ecc = strtok(eccList, ",");
            while (ecc != NULL) {
                char *plusChar = strchr(ecc, '+');
                if (plusChar != NULL) {
                    eccCategory = ecc;
                    *plusChar = '\0';
                    ecc = plusChar + 1;
                    if (strcmp(ecc, number.string()) == 0 && atoi(eccCategory) > 0) {
                        int ret = atoi(eccCategory);
                        free(eccList);
                        return ret;
                    }
                }
                ecc = strtok(NULL, ",");
            }
        }
        free(eccList);
    }

    return 0;
}

bool RmmEcallRequestHandler::isEccMatchInList(String8 number, String8 eccList) {
    // Add match boundary char ',' for easy match
    String8 searchEccList = String8(",") + eccList + String8(",");
    if (searchEccList.find(String8(",") + number + String8(",")) != -1) {
        return true;
    }
    if (searchEccList.find(String8("+") + number + String8(",")) != -1) {
        return true;
    }

    return false;
}

string RmmEcallRequestHandler::IdToString(int request) {
    switch(request) {
    case RIL_REQUEST_ECALL_FAST_MAKE_ECALL: return "RIL_REQUEST_ECALL_FAST_MAKE_ECALL";
    case RIL_REQUEST_ECALL_SET_MSD: return "RIL_REQUEST_ECALL_SET_MSD";
    case RIL_REQUEST_ECALL_RESET_IVS: return "RIL_REQUEST_ECALL_RESET_IVS";
    case RIL_REQUEST_ECALL_SET_REGISTRATION_STATE: return "RIL_REQUEST_ECALL_SET_REGISTRATION_STATE";
    case RIL_REQUEST_ECALL_SET_OPRT_MODE: return "RIL_REQUEST_ECALL_SET_OPRT_MODE";
    case RIL_REQUEST_ECALL_GET_OPRT_MODE: return "RIL_REQUEST_ECALL_GET_OPRT_MODE";
    case RIL_REQUEST_ECALL_SET_TEST_NUM: return "RIL_REQUEST_ECALL_SET_TEST_NUM";
    case RIL_REQUEST_ECALL_SET_RECONF_NUM: return "RIL_REQUEST_ECALL_SET_RECONF_NUM";
    case RIL_REQUEST_ECALL_SET_PRI: return "RIL_REQUEST_ECALL_SET_PRI";
    case RIL_UNSOL_ECALL_INDICATIONS: return "RIL_UNSOL_ECALL_INDICATIONS";
    default: return "<unknown request>";
    }
}

