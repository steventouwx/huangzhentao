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

#include "RmmImsControlRequestHandler.h"
#include "rfx_properties.h"
#include "RfxMipcData.h"
#include "RfxRilUtils.h"
#include <telephony/mtk_ril.h>
#include <string>
#include <string.h>
#include "RmmImsUtil.h"
#include "RmmImsCustomizedUtils.h"
#include "utils/String8.h"
#include <sstream>

static const int requests[] = {
    RFX_MSG_REQUEST_SET_IMSCFG,                 //AT+EIMSCFG
    RFX_MSG_REQUEST_SET_IMSCFG_EX,              //AT+EIMSCFG, internal use only
    RFX_MSG_REQUEST_SET_VODATA_CFG,             //AT+EVODATA, internal use only
    RFX_MSG_REQUEST_SET_IMS_ADD_CAP,
    RFX_MSG_REQUEST_SET_IMS_UA,
    RFX_MSG_REQUEST_SET_MD_IMSCFG,              //AT+ECFGSET,AT+EWOCFGSET,AT+EIWLCFGSET
    RFX_MSG_REQUEST_SET_WFC_PROFILE,            //AT+EWFCP
    RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL,
    RFX_MSG_REQUEST_IMS_REGISTRATION_STATE,     //AT+CIREG
    RFX_MSG_REQUEST_SET_IMS_REGISTRATION_REPORT,  //AT+CIREG=2
    RFX_MSG_REQUEST_QUERY_VOPS_STATUS,          // AT+CIREP
    RFX_MSG_REQUEST_GET_IMSCFG,
};

#define RIL_WIFI_PREFERRED     1
#define RIL_CELLULAR_PREFERRED 2
#define RIL_WIFI_ONLY          3
#define RIL_4G_PREFERRED       4
#define PROPERTY_IMS_TEST_MODE      "persist.vendor.radio.imstestmode"
#define PROPERTY_IMS_DISABLE_SMS    "persist.vendor.radio.imsdisablesms"

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_IMSCFG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_IMSCFG_EX);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_VODATA_CFG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_IMS_ADD_CAP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_SET_IMS_UA);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxStringData, RFX_MSG_REQUEST_SET_MD_IMSCFG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_WFC_PROFILE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IMS_REGISTRATION_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_SET_IMS_REGISTRATION_REPORT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_VOPS_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData, RFX_MSG_REQUEST_GET_IMSCFG);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmImsControlRequestHandler, RIL_CMD_PROXY_1);

RmmImsControlRequestHandler::RmmImsControlRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmmImsControlRequestHandler constructor");
    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));
    //enableImsRegistrationReport(NULL); //mipc do it ?? TODO
    requestMultiImsSupportCount();

    //char atcmd[] = "AT+ECFGSET=\"report_eimsreginfo_urc\",\"1\"";
    //enableSipRegInfoInd(); //need mipc support

    //atSendCommand(String8::format("AT+CIREP=1"));
    //enableVopsStatusReport(); //need mipc support

    userAgentformat = String8("");

    /// M: L+L/SRLTE+L need know ct volte Support or not @{
    static bool initDone = false;
    if (initDone == false) {
        initDone = true;
        setCtVolteMode(); //move to mipc
    }
    /// @}
}

RmmImsControlRequestHandler::~RmmImsControlRequestHandler() {
}

void RmmImsControlRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    //logD(RFX_LOG_TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_SET_IMSCFG:
        case RFX_MSG_REQUEST_SET_IMSCFG_EX:
            requestSetImsCfg(msg);
            break;
        case RFX_MSG_REQUEST_SET_VODATA_CFG:
            requestSetVodataCfg(msg);
            break;
        case RFX_MSG_REQUEST_SET_IMS_ADD_CAP:
            requestSetImsAddCap(msg);
            break;
        case RFX_MSG_REQUEST_SET_IMS_UA:
            requestSetSipUserAgent();
            {
                sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(
                        msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
                // response to TeleCore
                responseToTelCore(rsp);
            }
            break;
        case RFX_MSG_REQUEST_SET_MD_IMSCFG:
            requestSetModemImsCfg(msg);
            break;
        case RFX_MSG_REQUEST_SET_WFC_PROFILE:
        case RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL:
            requestSendWfcProfile(msg);
            break;
        case RFX_MSG_REQUEST_IMS_REGISTRATION_STATE:
            requestImsRegState(msg);
            break;
        case RFX_MSG_REQUEST_SET_IMS_REGISTRATION_REPORT:
            enableImsRegistrationReport(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_VOPS_STATUS:
            queryVopsStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_IMSCFG:
            requestGetImsCfg(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "onHandleRequest, %d Should not be here", msg->getId());
            break;
    }
}

void RmmImsControlRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    //logD(RFX_LOG_TAG, "onHandleResponse: %d", msg->getId());
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_SET_IMSCFG:
        case RFX_MSG_REQUEST_SET_IMSCFG_EX:
            requestSetImsCfgResponse(msg);
            break;
        case RFX_MSG_REQUEST_SET_VODATA_CFG:
            requestSetVodataCfgResponse(msg);
            break;
        case RFX_MSG_REQUEST_SET_IMS_ADD_CAP:
            requestSetImsAddCapResponse(msg);
            break;
        case RFX_MSG_REQUEST_SET_WFC_PROFILE:
        case RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL:
            requestSendWfcProfileResponse(msg);
            break;
        case RFX_MSG_REQUEST_IMS_REGISTRATION_STATE:
            requestImsRegStateResponse(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_VOPS_STATUS:
            queryVopsStatusResponse(msg);
            break;
        case RFX_MSG_REQUEST_GET_IMSCFG:
            requestGetImsCfgResponse(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "onHandleResponse, %d Should not be here", msg->getId());
            break;
    }
}

void RmmImsControlRequestHandler::requestSetImsCfg(const sp<RfxMclMessage>& msg) {
    int *params = (int *)msg->getData()->getData();
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    if (isCurrentSlotSupportIms()) {
        uint8_t volte = (uint8_t)params[0];
        uint8_t vilte = (uint8_t)params[1];
        uint8_t vowifi = (uint8_t)params[2];
        uint8_t viwifi = (uint8_t)params[3];
        uint8_t sms = (uint8_t)params[4];
        uint8_t eims = (uint8_t)params[5];

        /**
         * Used to initialize RCS related feature tags.
         * invoke here because RCS feature initialize also
         * need to be done before IMS registration started,
         * thus appropriate feature tags can be carried in
         * IMS Register.
         */
        enableImsRcsStateFeature();

        /**
         * Check IMS test mode setting
         *    1 : AT+EIMS=0
         *    0 : Normal operation
         */
        char value[MTK_PROPERTY_VALUE_MAX] = {0};
        int imsTestMode = -1;
        rfx_property_get(PROPERTY_IMS_TEST_MODE, value, "0");
        imsTestMode = atoi(value);
        eims = (imsTestMode == 1) ? 0 : eims;

        char value1[MTK_PROPERTY_VALUE_MAX] = {0};
        rfx_property_get(PROPERTY_IMS_DISABLE_SMS, value1, "0");
        int smsDisable = atoi(value1);
        if (smsDisable == 1 && (volte | vilte | vowifi | viwifi) == 0) {
            sms = 0;
            eims = 0;
        }

        // Set user agent before ims config to modem
        requestSetSipUserAgent();

        // send request
        logI(RFX_LOG_TAG,
            "requestSetImsCfg, AT+EIMSCFG=%d,%d,%d,%d,%d,%d, Mode:%d, smsDisable:%d",
            volte, vilte, vowifi, viwifi, sms, eims, imsTestMode, smsDisable);

        unsigned char ims_value[] = {volte, vilte, vowifi, viwifi, sms, eims};
        sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_IMS_SET_CONFIG_REQ, m_slot_id);
        req_data->addMipcTlvUint32(
                MIPC_IMS_SET_CONFIG_REQ_T_CLASS,
                MIPC_IMS_CONFIG_CLASS_IMS_FEATURE);
        req_data->addMipcTlv(MIPC_IMS_SET_CONFIG_REQ_T_DATA, 6, ims_value);
        callToMipcMsgAsync(msg, req_data);
    } else {
        logD(RFX_LOG_TAG, "requestSetImsCfg, slotId[%d] don't support IMS", m_slot_id);
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    if (rilErrNo != RIL_E_SUCCESS) {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
    }
}

void RmmImsControlRequestHandler::requestSetImsCfgResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();

    logD(RFX_LOG_TAG, "requestSetImsCfgResponse, result:%d", result);
    if (MIPC_RESULT_SUCCESS != result) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmImsControlRequestHandler::requestSetVodataCfg(const sp<RfxMclMessage>& msg) {
    if (isCurrentSlotSupportIms()) {
        int *params = (int *)msg->getData()->getData();
        int vodata_en = params[0];

        // send AT command
        logD(RFX_LOG_TAG, "requestSetVodataCfg, vodata_en:%d", vodata_en);
        //p_response = atSendCommand(String8::format("AT+EVODATA=%d,0,0", vodata_en));
        sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_IMS_SET_EVODATA_REQ, m_slot_id);
        req_data->addMipcTlvUint8(MIPC_IMS_SET_EVODATA_REQ_T_EN, (uint8_t)vodata_en);
        req_data->addMipcTlvUint8(MIPC_IMS_SET_EVODATA_REQ_T_DATA_MODE, MIPC_IMS_EVODATA_MODE_ENUM_DEFAULT);
        req_data->addMipcTlvUint8(MIPC_IMS_SET_EVODATA_REQ_T_ALLOW_RAT, MIPC_IMS_EVODATA_ALLOWRAT_ENUM_DEFAULT);
        callToMipcMsgAsync(msg, req_data);

    } else {
        logD(RFX_LOG_TAG, "requestSetVodataCfg, slotId[%d] don't support IMS", m_slot_id);
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
    }
}

void RmmImsControlRequestHandler::requestSetVodataCfgResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();

    logD(RFX_LOG_TAG, "requestSetVodataCfgResponse, result:%d", result);
    if (MIPC_RESULT_SUCCESS != result) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmImsControlRequestHandler::requestSetImsAddCap(const sp<RfxMclMessage>& msg) {
    int *params = (int *)msg->getData()->getData();
    int capbility_en = params[0];

    logD(RFX_LOG_TAG, "requestSetImsAddCap, capbility_en:%d", capbility_en);
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_IMS_CFG_ADDITION_SERVICE_REQ, m_slot_id);
    uint32_t capbility[16] = { 0 };
    capbility[0] = (uint32_t)capbility_en;
    req_data->addMipcTlv(MIPC_IMS_CFG_ADDITION_SERVICE_REQ_T_SERVICE_CAP, sizeof(capbility), capbility);

    callToMipcMsgAsync(msg, req_data);
}

void RmmImsControlRequestHandler::requestSetImsAddCapResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();

    logD(RFX_LOG_TAG, "requestSetImsAddCapResponse, result:%d", result);
    if (MIPC_RESULT_SUCCESS != result) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmImsControlRequestHandler::requestSetModemImsCfg(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "requestSetModemImsCfg, enter");
    sp<RfxMclMessage> responseMsg;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;

    char **params = (char **)msg->getData()->getData();

    char *key_token = NULL;
    char *val_token = NULL;

    char *p_key_cur = (char *) calloc(strlen(params[0]) + 1, sizeof(char));
    char *p_val_cur = (char *) calloc(strlen(params[1]) + 1, sizeof(char));
    RFX_ASSERT(p_key_cur != NULL);
    RFX_ASSERT(p_val_cur != NULL);

    char *p_key_temp= p_key_cur;
    char *p_val_temp= p_val_cur;

    if ((p_key_cur == NULL) || (p_val_cur == NULL)) {
        logD(RFX_LOG_TAG, "requestSetModemImsCfg, calloc failed!");

        if (p_key_cur != NULL) {
            free(p_key_cur);
        }

        if (p_val_cur != NULL) {
            free(p_val_cur);
        }

        return;
    }

    strncpy(p_key_cur, params[0], strlen(params[0]));
    strncpy(p_val_cur, params[1], strlen(params[1]));

    std::string result = "";
    char *rsp_string;

    int type = atoi(params[2]);

    if (type >= 0 && type < MAX_MD_CFGTYPE_NUM) {
        while ((key_token = nextTok(&p_key_cur)) != NULL) {
            if ((val_token = nextTok(&p_val_cur)) != NULL) {

                // send AT command, default send AT+ECFGSET (type is 0)
                logD(RFX_LOG_TAG, "requestSetModemImsCfg, key:%s, value:%s", key_token, val_token);
                sp<RfxMipcData> rsp_data = NULL;

                if (strlen(key_token) > MAX_MD_CFG_KEY_LEN || strlen(val_token) > MAX_MD_CFG_VAL_LEN) {
                    logE(RFX_LOG_TAG, "requestSetModemImsCfg, key or value too long");
                } else if (type == ECFGSET) {
                    //atSendCommand(String8::format("%s=\"%s\",\"%s\"",
                    //        cfgAtCmd[ECFGSET], key_token, val_token));
                    sp<RfxMipcData> request_data =
                        RfxMipcData::obtainMipcData(MIPC_IMS_SET_CONFIG_REQ, m_slot_id);
                    request_data->addMipcTlvUint32(
                        MIPC_IMS_SET_CONFIG_REQ_T_CLASS, MIPC_IMS_CONFIG_CLASS_IMS_CONFIG);
                    request_data->addMipcTlvStr(MIPC_IMS_SET_CONFIG_REQ_T_TYPE, key_token);
                    request_data->addMipcTlv(
                        MIPC_IMS_SET_CONFIG_REQ_T_DATA, strlen(val_token), val_token);
                    rsp_data = callToMipcMsgSync(request_data);
                    logD(RFX_LOG_TAG, "requestSetModemImsCfg, ECFGSET mipc_result:%d",
                            rsp_data->getResult());
                } else if(type == EWOCFGSET) {
                    //atSendCommand(String8::format("%s=\"%s\",\"%s\"",
                    //        cfgAtCmd[EWOCFGSET], key_token, val_token));
                    sp<RfxMipcData> request_data =
                        RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, m_slot_id);
                    request_data->addMipcTlvUint32(
                        MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_N3X_CONFIG);
                    request_data->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE, key_token);
                    request_data->addMipcTlv(
                        MIPC_SYS_SET_CONFIG_REQ_T_DATA, strlen(val_token), val_token);
                    rsp_data = callToMipcMsgSync(request_data);
                    logD(RFX_LOG_TAG, "requestSetModemImsCfg, EWOCFGSET mipc_result:%d",
                            rsp_data->getResult());
                } else if(type == EIWLCFGSET) {
                    //atSendCommand(String8::format("%s=\"%s\",\"%s\"",
                    //        cfgAtCmd[EIWLCFGSET], key_token, val_token));
                    sp<RfxMipcData> request_data =
                        RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, m_slot_id);
                    request_data->addMipcTlvUint32(
                        MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_IWLAN_CONFIG);
                    request_data->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE, key_token);
                    request_data->addMipcTlv(
                        MIPC_SYS_SET_CONFIG_REQ_T_DATA, strlen(val_token), val_token);
                    rsp_data = callToMipcMsgSync(request_data);
                    logD(RFX_LOG_TAG, "requestSetModemImsCfg, EIWLCFGSET mipc_result:%d",
                            rsp_data->getResult());
                } else {
                    logE(RFX_LOG_TAG, "requestSetModemImsCfg, error type");
                    break;
                }

                // set result
                if (rsp_data == NULL || MIPC_RESULT_SUCCESS != rsp_data->getResult()) {
                    // default return fail, should return fail final if all fail
                    if (result.empty()) {
                        result += std::string("-1");
                    } else {
                        result += std::string(",-1");
                    }
                } else {
                    // if any of config have ever success, should return success final
                    if (result.empty()) {
                        result += std::string("0");
                    } else {
                        result += std::string(",0");
                    }
                    ril_errno = RIL_E_SUCCESS;
                }
            } else {
                logE(RFX_LOG_TAG,
                    "requestSetModemImsCfg, cannot parse value for current key:%s",
                    key_token);
                ril_errno = RIL_E_GENERIC_FAILURE;
                // key and value length not match
            }
        }
    } else {
        logD(RFX_LOG_TAG, "requestSetModemImsCfg, type:%d not match", type);
    }

    rsp_string = new char[result.size() + 1];
    std::copy(result.begin(), result.end(), rsp_string);
    rsp_string[result.size()] = '\0';

    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxStringData(rsp_string, strlen(rsp_string)), msg, false);
    // response to TeleCore
    responseToTelCore(responseMsg);

    free(p_key_temp);
    free(p_val_temp);
    delete[] rsp_string;
}

void RmmImsControlRequestHandler::requestSendWfcProfile(const sp<RfxMclMessage>& msg) {
    int *preference = (int*)msg->getData()->getData();
    int wfc_prefer = preference[0];

    logD(RFX_LOG_TAG, "requestSendWfcProfile, preference:%d", wfc_prefer);
    //atSendCommand(String8::format("AT+EWFCP=%d", wfc_prefer));
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_WFC_CFG_REQ, m_slot_id);
    req_data->addMipcTlvUint8(MIPC_WFC_CFG_REQ_T_WFC_PREFER, (uint8_t)wfc_prefer);
    callToMipcMsgAsync(msg, req_data);

}

void RmmImsControlRequestHandler::requestSendWfcProfileResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    logD(RFX_LOG_TAG, "requestSendWfcProfileResponse, result:%d", result);
    if (MIPC_RESULT_SUCCESS != result) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmmImsControlRequestHandler::requestImsRegState(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "requestImsRegState");

    //atSendCommandSingleline("AT+CIREG?", "+CIREG:");
    /* +CIREG: <n>,<reg_info>[,<ext_info>] */
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_IMS_GET_STATE_REQ, m_slot_id);
    req_data->addMipcTlvUint8(
        MIPC_IMS_GET_STATE_REQ_T_EVENT,
        MIPC_IMS_STATE_IND_EVENT_REG_STATE);
    callToMipcMsgAsync(msg, req_data);
}

void RmmImsControlRequestHandler::requestImsRegStateResponse(const sp<RfxMclMessage>& msg) {
    int response[2] = {0};
    sp<RfxMclMessage> responseMsg;

    RfxMipcData* rsp_data = msg->getMipcData();
    unsigned int  result = rsp_data->getResult();
    uint8_t eventType;

    if (MIPC_RESULT_SUCCESS != result) {
        logE(RFX_LOG_TAG, "requestImsRegStateResponse result=%d", result);
        goto error;
    }

    eventType = rsp_data->getMipcUint8Val(MIPC_IMS_GET_STATE_CNF_T_EVENT, 0);
    if (eventType != MIPC_IMS_STATE_IND_EVENT_REG_STATE) {
        logE(RFX_LOG_TAG, "requestImsRegStateResponse eventType wrong:%d", eventType);
        goto error;
    }

    response[0] = (uint8_t)(rsp_data->getMipcUint8Val(MIPC_IMS_GET_STATE_CNF_T_REG_STATE, 0));
    logD(RFX_LOG_TAG, "requestImsRegStateResponse, +CIREG regState = %d", response[0]);
    if (response[0] == 3 || response[0] == 4) { ////registering or reregistering
        response[0] = 0;
    } else if (response[0] == 2) { //unregistering
        response[0] = 1;
    }

    response[1] = 1; // RADIO_TECH_3GPP

    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(response, 2), msg, false);
    // response to TeleCore
    responseToTelCore(responseMsg);
    return;

error:
    logE(RFX_LOG_TAG, "requestImsRegStateResponse, shall not return error when radio is on");
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(response, 2), msg, false);
    // response to TeleCore
    responseToTelCore(responseMsg);
}

void RmmImsControlRequestHandler::enableImsRegistrationReport(const sp<RfxMclMessage>& msg) {
    //atSendCommand(String8::format("AT+CIREG=2"));
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;

    sendEvent(RFX_MSG_EVENT_IMS_REPORT_EIREG, RfxVoidData(), RIL_CMD_PROXY_URC, m_slot_id);

    const char *atcmd = "AT+CIREG=2";
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_SYS_AT_REQ, m_slot_id);
    req_data->addMipcTlvStr(MIPC_SYS_AT_REQ_T_ATCMD, atcmd);
    sp<RfxMipcData> rsp_data = callToMipcMsgSync(req_data);
    unsigned int result = rsp_data->getResult();
    if (MIPC_RESULT_SUCCESS != result) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    logD(RFX_LOG_TAG, "enableImsRegistrationReport, AT+CIREG=2, result:%d", result);
    if (msg != NULL) {
        response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
                RfxVoidData(), msg, false);
        // response to TeleCore
        responseToTelCore(response);
    }
}

void RmmImsControlRequestHandler::requestMultiImsSupportCount() {
    sp<RfxAtResponse> p_response = NULL;
    int multiImsCount;
    sp<RfxMclMessage> urc = NULL;

    // use AP side system property as default return value.
    char sApMultiImsCount[RFX_PROPERTY_VALUE_MAX] = { 0 };
    char sMdMultiImsCount[RFX_PROPERTY_VALUE_MAX] = { 0 };
    const char* PROP_AP_MULTI_IMS_SUPPORT = "persist.vendor.mims_support";
    const char* PROP_MD_MULTI_IMS_SUPPORT = "ro.vendor.md_mims_support";

    rfx_property_get(PROP_MD_MULTI_IMS_SUPPORT, sMdMultiImsCount, "-1");
    if (strcmp(sMdMultiImsCount, "-1") != 0) {
        return;
    }
    rfx_property_get(PROP_AP_MULTI_IMS_SUPPORT, sApMultiImsCount, "1");

    // send AT command to get intermediate response
    //p_response = atSendCommandSingleline("AT+EMULTIIMS?", "+EMULTIIMS:");

    /* ims count */
    multiImsCount = 2;

    logD(RFX_LOG_TAG, "requestMultiImsSupportCount, +EMULTIIMS: %d", multiImsCount);
    int n = sprintf(sMdMultiImsCount, "%d", multiImsCount);
    if (n <= 0) {
        logE(RFX_LOG_TAG, "requestMultiImsSupportCount, sprintf:%d", n);
    }
    rfx_property_set("ro.vendor.md_mims_support", sMdMultiImsCount);
}

void RmmImsControlRequestHandler::enableImsRcsStateFeature() {
    static bool initOnce[MAX_SIM_COUNT] = {false};

    if (initOnce[m_slot_id])
        return;

    // is rcs ua support?
    char property_value[MTK_PROPERTY_VALUE_MAX] = { 0 };
    int isRcsUaProxySupport = 0;
    rfx_property_get("persist.vendor.mtk_rcs_ua_support", property_value, "0");
    isRcsUaProxySupport = atoi(property_value);
    logD(RFX_LOG_TAG, "isRcsUaProxySupport: %d", isRcsUaProxySupport);

    if (isRcsUaProxySupport == 0) {
        /* MD will save RCS state once enabled, so always reset here
           for runtime switch RCS UA support case.
           Response of EIMSRCS not the issue here, just ignore it */
        atSendCommand("AT+EIMSRCS=0,0");
        initOnce[m_slot_id] = true;
        return;
    }

    // is rcs service enable?
    int isRcsServiceEnabled = 1;
    memset(property_value, 0, sizeof(property_value));
    if (m_slot_id == 1)
        rfx_property_get("persist.vendor.service.rcs.slot2", property_value, "1");
    else
        rfx_property_get("persist.vendor.service.rcs", property_value, "1");
    isRcsServiceEnabled = atoi(property_value);
    logD(RFX_LOG_TAG, "isRcsServiceEnabled: %d", isRcsServiceEnabled);

    // what are the rcs service tags?
    /**
     * Checks which RCS service tag was supported.
     * @return RCS service tag bit-map:
     *         0x01  session
     *         0x02  filetransfer
     *         0x04  msg
     *         0x08  largemsg
     *         0x10  geopush
     *         0x20  geopull
     *         0x40  geopullft
     *         0x80  imdn aggregation
     *        0x100  geosms
     *        0x200  fthttp
     *         0x00  RCS service tag was removed
     */
    int rcsSrvTags = 0, rcsCustomized = 0;
    memset(property_value, 0, sizeof(property_value));
    if (m_slot_id == 1)
        rfx_property_get("persist.vendor.service.tag.rcs.slot2", property_value, "0");
    else
        rfx_property_get("persist.vendor.service.tag.rcs", property_value, "0");
    rcsSrvTags = atoi(property_value);
    logD(RFX_LOG_TAG, "rcsCustomized: %d, rcsSrvTags: %d", rcsCustomized, rcsSrvTags);
    if (rcsSrvTags < 0x00 || rcsSrvTags > 0xFFFF) {
        rcsSrvTags = 0;
    }

    sp<RfxAtResponse> p_response;
    // send AT command
    if (isRcsServiceEnabled == 1 && rcsSrvTags > 0) {
        p_response = atSendCommand(String8::format("AT+EIMSRCS=1,%d", rcsSrvTags));
    } else {
        p_response = atSendCommand(String8::format("AT+EIMSRCS=0,%d", rcsSrvTags));
    }

    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        logE(RFX_LOG_TAG, "enableImsRcsStateFeature(): AT+EIMSRCS error: %d",
            p_response->getError());
    }
    initOnce[m_slot_id] = true;
}

void RmmImsControlRequestHandler::requestSetSipUserAgent() {
    sp<RfxAtResponse> p_response = NULL;
    RIL_Errno rilErrNo =  RIL_E_GENERIC_FAILURE;
    bool isReset = false;
    char feature[] = "ECFGRESET Supported";

    if (getFeatureVersion(feature) <= 0 || !RmmImsCustomizedUtils::isSupportCustomizedUserAgent()) {
        logD(RFX_LOG_TAG, "requestSetSipUserAgent, not support");
        return ;
    }

    String8 format = getMclStatusManager()->getString8Value(
            RFX_STATUS_KEY_CUSTOMIZED_USER_AGENT_FORMAT);

    logI(RFX_LOG_TAG, "requestSetSipUserAgent, format: old=%s, new = %s; reset version=%d",
            userAgentformat.string(), format.string(), getFeatureVersion(feature));

    while (rilErrNo == RIL_E_GENERIC_FAILURE) {
        // Check if the format already be set before

        if (userAgentformat == format) {
            logI(RFX_LOG_TAG, "requestSetSipUserAgent, format no changed.");
            break;
        } else {
            logI(RFX_LOG_TAG, "requestSetSipUserAgent, format changed.");
            if (String8("0") == format) {
                isReset = true;

                // Reset User-Agent Value to MD
                //p_response = atSendCommand(String8::format("AT+ECFGRESET=\"user_agent\""));

                const char *atcmd = "AT+ECFGRESET=\"user_agent\"";
                sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_AT_REQ, m_slot_id);
                initMipc->addMipcTlvStr(MIPC_SYS_AT_REQ_T_ATCMD, atcmd);
                sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);

                unsigned int  mipc_result = cnfMipc->getResult();
                logD(RFX_LOG_TAG, "requestSetSipUserAgent, reset mipc_result: %d", mipc_result);

                if (MIPC_RESULT_SUCCESS != mipc_result) {
                    logE(RFX_LOG_TAG, "requestSetSipUserAgent, Reset error: %d", mipc_result);
                    break;
                }

                rilErrNo = RIL_E_SUCCESS;
            } else {
                String8 newValue = RmmImsCustomizedUtils::getCustomizedUserAgent(format, m_slot_id);

                //atSendCommandSingleline(String8::format("AT+ECFGGET=\"user_agent\""),"+ECFGGET");
                const char *type = "user_agent";
                sp<RfxMipcData> get_data =
                    RfxMipcData::obtainMipcData(MIPC_IMS_GET_CONFIG_REQ, m_slot_id);
                get_data->addMipcTlvUint32(
                        MIPC_IMS_GET_CONFIG_REQ_T_CLASS,
                        MIPC_IMS_CONFIG_CLASS_IMS_CONFIG);
                get_data->addMipcTlvStr(MIPC_IMS_GET_CONFIG_REQ_T_TYPE, type);
                sp<RfxMipcData> get_rsp_data = callToMipcMsgSync(get_data);
                if (MIPC_RESULT_SUCCESS != get_rsp_data->getResult()) {
                    logE(RFX_LOG_TAG, "requestSetSipUserAgent, get error: %d",
                        get_rsp_data->getResult());
                    break;
                }
                uint16_t val_len = 0;
                char *temp_value =
                    (char*) get_rsp_data->getMipcVal(MIPC_IMS_GET_CONFIG_CNF_T_DATA , &val_len);

                char *oldValue = (char*)calloc(1, sizeof(char)*(val_len + 1));
                RFX_ASSERT(oldValue != NULL);
                memcpy(oldValue, temp_value, val_len);
                oldValue[val_len] = '\0';
                logD(RFX_LOG_TAG, "requestSetSipUserAgent, oldValue = %s, newValue = %s",
                        oldValue, newValue.string());
                // If Value is not changed
                if(strcmp(newValue.string(), oldValue) == 0) {
                    rilErrNo = RIL_E_SUCCESS;
                    free(oldValue);
                    break;
                } else {
                    free(oldValue);
                }

                // Send User-Agent Value to MD
                //atSendCommand(String8::format("AT+ECFGSET=\"user_agent\",\"%s\"", newValue.string()));
                char *cfgValue = NULL;
                int n = asprintf(&cfgValue, "%s",  newValue.string());
                if (n <= 0) {
                    logE(RFX_LOG_TAG, "requestSetSipUserAgent, set asprintf: %d", n);
                }
                sp<RfxMipcData> set_data =
                    RfxMipcData::obtainMipcData(MIPC_IMS_SET_CONFIG_REQ, m_slot_id);
                set_data->addMipcTlvUint32(
                    MIPC_IMS_SET_CONFIG_REQ_T_CLASS,
                    MIPC_IMS_CONFIG_CLASS_IMS_CONFIG);
                set_data->addMipcTlvStr(MIPC_IMS_SET_CONFIG_REQ_T_TYPE, type);
                if (cfgValue != NULL) {
                    set_data->addMipcTlv(MIPC_IMS_SET_CONFIG_REQ_T_DATA, strlen(cfgValue), cfgValue);
                }
                sp<RfxMipcData> set_rsp_data = callToMipcMsgSync(set_data);
                unsigned int  mipc_result = set_rsp_data->getResult();

                logD(RFX_LOG_TAG, "requestSetSipUserAgent, set mipc_result: %d", mipc_result);

                if (cfgValue != NULL) {
                    free(cfgValue);
                }
                if (MIPC_RESULT_SUCCESS != mipc_result) {
                    break;
                }
                rilErrNo = RIL_E_SUCCESS;
            }
        }
    }

    if (rilErrNo == RIL_E_SUCCESS) {
        userAgentformat = format;
        logD(RFX_LOG_TAG, "requestSetSipUserAgent, update userAgentformat to %s",
            userAgentformat.string());
    }
}

bool RmmImsControlRequestHandler::isCurrentSlotSupportIms() {
    bool result = false;

    char isImsSupport[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.ims_support", isImsSupport, "0");

    if (atoi(isImsSupport) == 0) {
        return result;
    }

    char imsCount[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.mims_support", imsCount, "1");

    if (atoi(imsCount) == 1) {
        if (RfxRilUtils::getMajorSim() == (m_slot_id + 1)) {
            result = true;
        }
    } else {
        int protocalStackId = RfxRilUtils::getProtocolStackId(m_slot_id);

        if (protocalStackId <= atoi(imsCount)) {
            result = true;
        }
    }
    return result;
}

void RmmImsControlRequestHandler::queryVopsStatus(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "queryVopsStatus " );

     // Query IMS network reporting
     // AT+CIREP?
     // +CIREP: <reporting>,<nwimsvops>
     //atSendCommandSingleline("AT+CIREP?", "+CIREP:");
     sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_IMS_GET_NW_RPT_REQ, m_slot_id);

     callToMipcMsgAsync(msg, req_data);
}

void RmmImsControlRequestHandler::queryVopsStatusResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_GENERIC_FAILURE;
    int responses[2] = {0};
    sp<RfxMclMessage> rsp = NULL;

    // +CIREP: <reporting>,<nwimsvops>
    //response = atSendCommandSingleline("AT+CIREP?", "+CIREP:");

    RfxMipcData* rsp_data = msg->getMipcData();
    unsigned int result = rsp_data->getResult();

    if (MIPC_RESULT_SUCCESS == result) {
        rilErrNo = RIL_E_SUCCESS;
        /**
         * <reporting> : integer
         * Enables or disables reporting of changes in the IMSVOPS supported
         * indication received from the network and reporting of PS to CS SRVCC,
         * PS to CS vSRVCC and CS to PS SRVCC handover information.
         *
         * 0   Disable reporting
         * 1   Enable reporting
         */
        responses[0] = (int)(rsp_data->getMipcUint8Val(MIPC_IMS_GET_NW_RPT_CNF_T_REPORT_STATUS, 0));

        /**
         * <nwimsvops> : integer
         * Gives the last IMS Voice Over PS sessions (IMSVOPS) supported
         * indication received from network.
         *
         * 0   IMSVOPS support indication is not received from network, or is negative
         * 1   IMSVOPS support indication as received from network is positive
         */
        responses[1] = (int)(rsp_data->getMipcUint8Val(MIPC_IMS_GET_NW_RPT_CNF_T_NW_IMS_VOPS, 0));
    }
    logD(RFX_LOG_TAG, "queryVopsStatusResponse, result:%d, +CIREP:%d,%d", result, responses[0], responses[1]);

    rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxIntsData(responses, 2), msg, false);
    responseToTelCore(rsp);
}

/*
void RmmImsControlRequestHandler::enableVopsStatusReport() {
    //atSendCommand(String8::format("AT+CIREP=1"));
    char atcmd[] = "AT+CIREP=1";
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_AT_REQ, m_slot_id);
    initMipc->addMipcTlvStr(MIPC_SYS_AT_REQ_T_ATCMD, atcmd);
    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);

    logD(RFX_LOG_TAG, "enableVopsStatusReport, AT+CIREP=1 result: %d",
            cnfMipc->getResult());
} */

/*
void RmmImsControlRequestHandler::enableSipRegInfoInd() {
    //atSendCommand(String8::format("AT+ECFGSET=\"report_eimsreginfo_urc\",\"1\""));

    //char atcmd[] = "AT+ECFGSET=\"report_eimsreginfo_urc\",\"1\"";
    //sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_AT_REQ, m_slot_id);
    //initMipc->addMipcTlvStr(MIPC_SYS_AT_REQ_T_ATCMD, atcmd);
    //sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);

    const char* type = "report_eimsreginfo_urc";
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_IMS_SET_CONFIG_REQ, m_slot_id);
    data->addMipcTlvUint32(MIPC_IMS_SET_CONFIG_REQ_T_CLASS, MIPC_IMS_CONFIG_CLASS_IMS_CONFIG);
    data->addMipcTlvStr(MIPC_IMS_SET_CONFIG_REQ_T_TYPE, type);
    char value[2] = {'1', '\0'};
    data->addMipcTlv(MIPC_IMS_SET_CONFIG_REQ_T_DATA, 1, value);
    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(data);

    logD(RFX_LOG_TAG, "enableSipRegInfoInd, result: %d", cnfMipc->getResult());
} */

void RmmImsControlRequestHandler::setCtVolteMode() {
    //atSendCommand(String8::format("AT+ECFGSET=\"report_eimsreginfo_urc\",\"1\""));

    const char* type = "mtk_ct_volte_support";
    char ctVolteSupport[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.mtk_ct_volte_support", ctVolteSupport, "0");

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_IMS_SET_CONFIG_REQ, m_slot_id);
    data->addMipcTlvUint32(MIPC_IMS_SET_CONFIG_REQ_T_CLASS, MIPC_IMS_CONFIG_CLASS_IMS_CONFIG);
    data->addMipcTlvStr(MIPC_IMS_SET_CONFIG_REQ_T_TYPE, type);
    if (atoi(ctVolteSupport) == 2 || atoi(ctVolteSupport) == 3) {
        ctVolteSupport[0] = '2';
    }
    data->addMipcTlv(MIPC_IMS_SET_CONFIG_REQ_T_DATA, 1, ctVolteSupport);
    ctVolteSupport[1] = '\0';

    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(data);

    logD(RFX_LOG_TAG, "setCtVolteMode:AT+ECFGSET=%s,%s error: %d",
            type, ctVolteSupport, cnfMipc->getResult());
}

void RmmImsControlRequestHandler::requestGetImsCfg(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "requestGetImsCfg");

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_IMS_GET_CONFIG_REQ, m_slot_id);
    req_data->addMipcTlvUint32(MIPC_IMS_GET_CONFIG_REQ_T_CLASS, MIPC_IMS_CONFIG_CLASS_IMS_FEATURE);
    callToMipcMsgAsync(msg, req_data);
}

void RmmImsControlRequestHandler::requestGetImsCfgResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    RfxMipcData* rsp_data = msg->getMipcData();
    int result = rsp_data->getResult();
    if (MIPC_RESULT_SUCCESS != result) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
        logW(RFX_LOG_TAG, "requestGetImsCfgResponse, result:%d", result);

        response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxStringData(), msg, false);
    } else {
        uint16_t val_len = 0;
        std::ostringstream convert;
        char* temp = (char*)rsp_data->getMipcVal(MIPC_IMS_GET_CONFIG_CNF_T_DATA, &val_len);
        for (int i = 0; i < val_len; i++) {
            convert << (int)temp[i];
        }
        std::string imscfg = convert.str();
        logD(RFX_LOG_TAG, "requestGetImsCfgResponse, result:%d, imscfg: %s, len: %d", result,
             imscfg.c_str(), val_len);

        response = RfxMclMessage::obtainResponse(
            msg->getId(), rilErrNo, RfxStringData(const_cast<char*>(imscfg.c_str()), val_len), msg,
            false);
    }
    responseToTelCore(response);
}
