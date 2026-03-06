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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <memory>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "NetAgentService.h"

#include "RmcDataDefs.h"
#include "RmcDcUtility.h"
#include "RfxRawData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxDataCallResponseData.h"
#include "RfxPcoData.h"
#include "RfxPcoIaData.h"
#include "RfxMessageId.h"
#include "RmcDcCommonReqHandler.h"
#include "RfxRilUtils.h"
#include "tinyxml.h"
#include <libmtkrilutils.h>
#include "RfxKeepaliveStatusData.h"
#include "RfxNetlinkIpv6InfoData.h"
#ifdef CMCC_NSIOT_SUPPORT
#include "hidl/HidlSupport.h"
//#include <hidl/HidlTransportSupport.h>
//#include <hidl/Status.h>
#include <vendor/mediatek/hardware/netdagent/1.0/INetdagent.h>

#include <aidl/vendor/mediatek/hardware/netdagent/INetdagents.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#endif
#define RFX_LOG_TAG "RmcDcCommon"
#define MAX_AT_CMD_LENGTH 512
#define CMD_INDEX_OFFSET 100

using android::hardware::Return;
using android::hardware::hidl_string;
using vendor::mediatek::hardware::netdagent::V1_0::INetdagent;

using ::aidl::vendor::mediatek::hardware::netdagent::INetdagents;
using ::ndk::SpAIBinder;
using ::ndk::ScopedAStatus;

/*****************************************************************************
 * Class RmcDcCommonReqHandler
 *****************************************************************************/
RFX_REGISTER_DATA_TO_URC_ID(RfxPcoData, RFX_MSG_UNSOL_PCO_DATA);
RFX_REGISTER_DATA_TO_URC_ID(RfxPcoIaData, RFX_MSG_UNSOL_PCO_DATA_AFTER_ATTACHED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_QUALIFIED_NETWORK_TYPES_CHANGED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_QUERY_PCO_WITH_AID);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_REQ_DATA_CONTEXT_IDS);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_SEND_QUALIFIED_NETWORK_TYPES_CHANGED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_QUERY_SELF_IA);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_MOBILE_DATA_USAGE);
RFX_REGISTER_DATA_TO_URC_ID(RfxKeepaliveStatusData, RFX_MSG_URC_KEEPALIVE_STATUS);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NW_LIMIT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxNetlinkIpv6InfoData, RFX_MSG_EVENT_KERNEL_ADDRESS_UPDATED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_RF_OCCUPY_TIMEOUT);

RIL_MtkDataProfileInfo* RmcDcCommonReqHandler::s_LastApnTable[MAX_SIM_COUNT] = {NULL};
int RmcDcCommonReqHandler::s_nLastReqNum[MAX_SIM_COUNT] = {0};
int* RmcDcCommonReqHandler::s_ApnCmdIndex[MAX_SIM_COUNT] = {NULL};

int RmcDcCommonReqHandler::s_dataSetting[MAX_SIM_COUNT][DATA_SETTING_NUMBERS] = {{0}};
int RmcDcCommonReqHandler::s_dataSetting_resend[MAX_SIM_COUNT][DATA_SETTING_NUMBERS] = {{SKIP_DATA_SETTINGS}};
volatile int RmcDcCommonReqHandler::s_mtuV6[MAX_CCMNI_INTERFACE_COUNT] = {0};

RmcDcCommonReqHandler::RmcDcCommonReqHandler(int slot_id, int channel_id, RmcDcPdnManager* pdnManager)
: RfxBaseHandler(slot_id, channel_id), m_pPdnManager(pdnManager), m_nGprsFailureCause(PDP_FAIL_ERROR_UNSPECIFIED) {
    mIsUserBuild = RmcDcUtility::isUserBuild();
    mRfOccupy = false;
    mRfOccupyTag = 0;
    char mdrsraSupport[] = "MD_RSRA";
    char mdrsraLegacy[] = "MD_RSRAV3";
    mMdrsraSupport = RmcDcUtility::isMdrsraSupport(getFeatureVersion(mdrsraSupport),
            getFeatureVersion(mdrsraLegacy));
}

RmcDcCommonReqHandler::~RmcDcCommonReqHandler() {
}

int RmcDcCommonReqHandler::activatePdn(const char *reqApn, const char *profileType,
        Vector<int> *vAidList, int isEmergency, MTK_RIL_Data_Call_Response_v11* response,
        int cmdIndex, RIL_TrafficDescriptor *td, bool matchAllRuleAllowed) {
    String8 cmd("");
    int err = 0;
    sp<RfxAtResponse> p_response;
    RfxAtLine *p_cur = NULL;
    int rid = m_slot_id;
    int cmeError = CME_ERROR_NON_CME;
    char apn[MAX_APN_NAME_LENGTH] = {0};

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] isEmergency: %d",
            rid, __FUNCTION__, isEmergency);

    if (response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] response is null!",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    char* tdStr = NULL;
    char appId[MAX_APN_NAME_LENGTH + 1] = {0};
    if (td != NULL) {
        if (td->dnn != NULL && strlen(td->dnn) > 0 &&
                td->osAppIdLen > 0 && td->osAppId != NULL) {
            if (td->osAppIdLen > OS_ID_UUID_LEN + 1) {
                strncpy(appId, (char *)&td->osAppId[APP_ID_INDEX], MAX_APN_NAME_LENGTH);
                asprintf(&tdStr,
                        "dnn=%s/os_id=\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x/app_id=%s",
                        td->dnn,
                        td->osAppId[0], td->osAppId[1], td->osAppId[2], td->osAppId[3],
                        td->osAppId[4], td->osAppId[5], td->osAppId[6], td->osAppId[7],
                        td->osAppId[8], td->osAppId[9], td->osAppId[10], td->osAppId[11],
                        td->osAppId[12], td->osAppId[13], td->osAppId[14], td->osAppId[15],
                        (char *)&td->osAppId[APP_ID_INDEX]);
            } else {
                asprintf(&tdStr, "dnn=%s", td->dnn);
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Invalid OS APP ID!", rid, __FUNCTION__);
            }
        } else if (td->dnn != NULL && strlen(td->dnn) > 0) {
            asprintf(&tdStr, "dnn=%s", td->dnn);
        } else if (td->osAppIdLen > 0 && td->osAppId != NULL) {
            // 16 bytes for UUID, 1 byte for length of osAppId, and up to 255 bytes for osAppId
            if (td->osAppIdLen > OS_ID_UUID_LEN + 1) {
                strncpy(appId, (char *)&td->osAppId[APP_ID_INDEX], MAX_APN_NAME_LENGTH);
                asprintf(&tdStr,
                        "os_id=\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x\\%02x/app_id=%s",
                        td->osAppId[0], td->osAppId[1], td->osAppId[2], td->osAppId[3],
                        td->osAppId[4], td->osAppId[5], td->osAppId[6], td->osAppId[7],
                        td->osAppId[8], td->osAppId[9], td->osAppId[10], td->osAppId[11],
                        td->osAppId[12], td->osAppId[13], td->osAppId[14], td->osAppId[15],
                        (char *)&td->osAppId[APP_ID_INDEX]);
            } else {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Invalid OS APP ID!", rid, __FUNCTION__);
            }
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Invalid TD!", rid, __FUNCTION__);
            // Still fallback to legacy way
        }
    }

    // AT+EAPNACT=<state>,<apn_name>,<apn_type>;
    // <state>: 0-deactivate; 1-activate
    if (reqApn != NULL && strlen(reqApn) > 0) {
        if (strlen(appId) > 0) {
            // Use apn\1Fapp_id (ASCII 0x1F/31) as URSP apn context
            // 1. Modem will insert apn for URSP even AP didn't sync this APN before.
            // 2. AP/MD will use this APN to communicate when pdn not setup successfully
            // (cid is not available), like retry timer query
            // To align with AT command test format from UI
            // Ex: AT+EAPNACT=1,"cmwap\1FENTERPRISE",...
            char *urspApn = NULL;
            if (strcmp(appId, "CBS") == 0) {
                // Android T support CBS slice type and it is also valid APN for 2/3/4G
                // Here we should still support CBS on 4G based on it will not share
                // APN setting with other slice type.
                asprintf(&urspApn, "%s", reqApn);
            } else {
                asprintf(&urspApn, "%s\\1F%s", reqApn, appId);
            }
            RFX_ASSERT(urspApn != NULL);
            strncpy(apn, urspApn, MAX_APN_NAME_LENGTH - 1);
            apn[MAX_APN_NAME_LENGTH - 1] = '\0';
            FREEIF(urspApn);
            // Don't convert escape char \1F
        } else {
            strncpy(apn, reqApn, MAX_APN_NAME_LENGTH - 1);
            apn[MAX_APN_NAME_LENGTH - 1] = '\0';
            addEscapeSequence(apn);
        }
    }

    if (tdStr != NULL) {
        // AT> AT+EAPNACT=1,"<apn_name>","<apn_type>",<apn_idx>,
        //                <if_id>[,"<traffic_desc>","<ue_local_conf>",<match_all_disallowed>]
        cmd.append(String8::format("AT+EAPNACT=1,\"%s\",\"%s\",%d,,\"%s\",,%d",
                apn, profileType, cmdIndex, tdStr, matchAllRuleAllowed ? 0 : 1));
    } else {
        cmd.append(String8::format("AT+EAPNACT=1,\"%s\",\"%s\",%d", apn, profileType, cmdIndex));
    }

    // +CGEV: ME PDN ACT <aid>[,<reason>[,<aid_other>]]
    p_response = atSendCommandMultiline(cmd, "+CGEV: ME PDN ACT ");

    if (tdStr != NULL) {
        free(tdStr);
    }

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        goto error;
    }

    err = p_response->getError();
    if (err == AT_ERROR_CHANNEL_CLOSED) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error when channel closed", rid, __FUNCTION__);
        goto error_channel_closed;
    }

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        p_cur->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error when token start",
                    rid, __FUNCTION__);
            goto error;
        }

        int aid = INVALID_AID;
        int reason = NO_CAUSE;
        int otherAid = INVALID_AID;
        char *tempParam = NULL;

        tempParam = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error when parsing aid",
                    rid, __FUNCTION__);
            goto error;
        }
        char *firstParam = tempParam + strlen(ME_PDN_ACT);
        aid = RmcDcUtility::stringToBinaryBase(firstParam, 10, &err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting aid to binary"
                    ", err = %d", rid, __FUNCTION__, err);
            goto error;
        }

        setIsDedicateBearer(aid, 0);
        updatePdnActiveStatus(aid, DATA_STATE_LINKDOWN);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] default bearer activated [aid=%d]",
                rid, __FUNCTION__, aid);

        if (p_cur->atTokHasmore()) {
            reason = p_cur->atTokNextint(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error when parsing reason for aid%d",
                        rid, __FUNCTION__, aid);
                goto error;
            }

            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] default bearer activated [aid=%d, reason=%d]",
                    rid, __FUNCTION__, aid, reason);

            if (p_cur->atTokHasmore()) {
                otherAid = p_cur->atTokNextint(&err);
                if (err < 0) {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error when parsing otherAid for aid%d",
                            rid, __FUNCTION__, aid);
                    goto error;
                }

                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] default bearer activated [aid=%d, reason=%d,"
                        " otherAid=%d]", rid, __FUNCTION__, aid, reason, otherAid);
            }
        }

        setAidAndPrimaryAid(aid, aid, aid);
        setReason(aid, reason);
        setIsEmergency(aid, isEmergency);
        vAidList->push_back(aid);
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] %zu PDN activated",
            rid, __FUNCTION__, vAidList->size());

    if (p_response->isAtResponseFail() || (vAidList->size() == 0)) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", rid, __FUNCTION__);
            goto error;
        }
        cmeError = p_response->atGetCmeError();
        if (cmeError == CME_UNKNOWN || cmeError == CME_ERROR_NON_CME) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EAPNACT CME UNKNOWN(100) or NON CME ERROR(-1): %d",
                    rid, __FUNCTION__, cmeError);
            m_nGprsFailureCause = PDP_FAIL_ERROR_UNSPECIFIED;
            if (cmeError == CME_ERROR_NON_CME) {
                goto error;
            }
        } else {
            updateLastFailCause(cmeError);
            // Setup Data Failure Info for customer requirement
            if (DMF_support == 0 && isApShouldReportDataFailCause()){
               if (isLocalFailCase(cmeError)){
                   causeType = DSI_CE_TYPE_INTERNAL;
               } else {
                   causeType = DSI_CE_TYPE_3GPP_SPEC_DEFINED;
               }
            }
            if (m_nGprsFailureCause == PDP_FAIL_NONE) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] NO CAUSE(0) but AT+EAPNACT failed",
                        rid, __FUNCTION__);
                m_nGprsFailureCause = PDP_FAIL_ERROR_UNSPECIFIED;
            } else {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EAPNACT CAUSE: %X",
                        rid, __FUNCTION__, m_nGprsFailureCause);
            }
        }
        goto cme_error;
    }

    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] finished X", rid, __FUNCTION__);
    return PDN_SUCCESS;

error:
    response->status = m_nGprsFailureCause = PDP_FAIL_ERROR_UNSPECIFIED;
    return PDN_FAILED;

cme_error:
    response->status = m_nGprsFailureCause;
    response->suggestedRetryTime = getModemSuggestedRetryTime(apn);
    if ((PDP_FAIL_MISSING_UKNOWN_APN == response->status ||
                PDP_FAIL_SERVICE_OPTION_NOT_SUBSCRIBED == response->status)
            && strcasecmp(profileType,"mms") == 0
            && response->suggestedRetryTime != -1) {
        char simMccMnc[RFX_PROPERTY_VALUE_MAX] = {0};
        getMccmnc(simMccMnc);
        if (0 == strcmp(simMccMnc, "42003")) {
            // Mobily(42003) has two APNs for MMS based on type of SIM card,
            // prepaid(apn="mms2") , postpaid(apn="mms1).
            // AOSP won't check SIM type(prepaid or postpaid) when select APN.
            // If wrong APN was selected, NW will return #27 and valid back-off timer.
            // AOSP has no chance to use right APN due to the back-off timer.
            // To fix it, reset back-off timer to -1(no suggest).
            RFX_LOG_I(RFX_LOG_TAG,
                    "error #27 or #33 for special oprator when set up mms");
            response->suggestedRetryTime = -1;
        }
    }
    if (PDP_FAIL_REJECTED_BY_IWLAN == response->status
            && response->suggestedRetryTime == -1
            && strcasecmp(profileType,"default") == 0) {
        response->suggestedRetryTime = SUGGEST_TIME_FOR_PDP_FAIL_REJECTED_BY_IWLAN;
    }
    return PDN_FAILED;

error_channel_closed:
    response->status = m_nGprsFailureCause = PDP_FAIL_ERROR_UNSPECIFIED;
    return CHANNEL_CLOSED;
}

int RmcDcCommonReqHandler::getModemSuggestedRetryTime(const char *apnName) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RfxAtLine *line = NULL;
    int err = 0;
    String8 cmd("");
    int suggestType = RETRY_TYPE_NO_SUGGEST;
    int suggestTime = -1;
    int rid = m_slot_id;

    if (m_nGprsFailureCause == PDP_FAIL_DATA_NOT_ALLOW) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] m_nGprsFailureCause is PDP_FAIL_DATA_NOT_ALLOW",
                rid, __FUNCTION__);
        return DATA_NOT_ALLOW_RETRY_TIME;
    }

    // AT+EDRETRY=<mode><APN name>
    // <mode>:indicate it's query mode or reset mode. 0:query mode, 1:reset mode
    // <APN name>: APN (string type) which is use to query or reset bar timer and retry count.
    cmd = String8::format("AT+EDRETRY=0,\"%s\"", apnName);
    p_response = atSendCommandSingleline(cmd, "+EDRETRY:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return -1;
    }

    if (p_response->isATCmdRspErr()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EDRETRY query command returns ERROR", rid, __FUNCTION__);
        return -1; //NO suggested retry time
    } else {
        err = p_response->getError();
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get error!",
                    rid, __FUNCTION__);
            return -1;
        }

        line = p_response->getIntermediates();

        if (line == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get intermediate results!",
                    rid, __FUNCTION__);
            return -1;
        }

        line->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start", rid, __FUNCTION__);
            return -1;
        }
        suggestType = line->atTokNextint(&err);
        if (err < 0) return -1;

        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Modem suggest type:[%d]", rid, __FUNCTION__, suggestType);
        if (suggestType == RETRY_TYPE_NO_SUGGEST) {
            return -1;
        } else if (suggestType == RETRY_TYPE_NO_RETRY) {
            return 0x7fffffff;
        } else if (suggestType == RETRY_TYPE_WITH_SUGGEST) {
            if (line->atTokHasmore()) {
                suggestTime = line->atTokNextint(&err);
                if (err < 0) return -1;
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Modem suggest time:[%d]", rid, __FUNCTION__, suggestTime);
                if (suggestTime >= 0 && suggestTime != 0x7fffffff) {
                    // The unit of suggested retry time from modem is second.
                    // We need to return the value in msec.
                    return suggestTime*1000;
                } else {
                    return -1;
                }
            } else {
                return -1;
            }
        }
    }
    return suggestTime;
}

int RmcDcCommonReqHandler::updatePdnInformation(const int activatedAid, int protocol,
        NETAGENT_IFST_STATUS ifst) {
    int rid = m_slot_id;
    int err = 0;
    int tempAid = INVALID_AID;
    int transIntfId = INVALID_TRANS_INTF_ID;
    char *cmdFormat = NULL;
    int cause = 0;
    int rat = 1;  // 1: cellular
    int mtu = 0;
    int pdnType = INVALID_PDN_TYPE;
    char *addr1 = NULL;
    char *addr2 = NULL;
    String8 cmd("");
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response;
    int protocolClassBitmap = RmcDcUtility::getProtocolClassBitmap(protocol);

    if (protocolClassBitmap == NETAGENT_ADDR_TYPE_UNKNOWN) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] invalid protocolClassBitmap", rid, __FUNCTION__);
        return PDN_FAILED;
    }

    if (ifst != NETAGENT_IFST_NONE && ifst != NETAGENT_IFST_UP) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] invalid ifst status %d", rid, __FUNCTION__, ifst);
        return PDN_FAILED;
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] activated aid%d", rid, __FUNCTION__, activatedAid);

    // AT+EPDN=<aid>, "ifst", <ifst_status + protocol_class_bitmap>
    // <ifst_status> :
    //       0     -> update interface status without waiting
    //       16    -> wait for interface up
    // <protocol_class_bitmap> :
    //       0     -> unknown
    //       1     -> wait for ipv4 address
    //       2     -> wait for ipv6 address
    //       3     -> wait for ipv4 and ipv6 address
    //       4     -> wait for any address
    //
    // Response:
    //     +EPDN:<aid>,"new",<rat type>,<interface id>,<mtu>,<address type>,<address1>[,<address2>]
    //     +EPDN:<aid>,"update",<interface id>,<address type>,<address1>[,<address2>]
    //     +EPDN:<aid>,"err",<err>
    cmd.append(String8::format("AT+EPDN=%d,\"ifst\",%d", activatedAid, ifst + protocolClassBitmap));
    p_response = atSendCommandSingleline(cmd, "+EPDN:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", rid, __FUNCTION__);
        } else {
            cause = p_response->atGetCmeError();
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EPDN(ifst) for activatedAid%d returns cme cause : %d",
                    rid, __FUNCTION__, activatedAid, cause);
        }
        return PDN_FAILED;
    }

    line = p_response->getIntermediates();

    if (line == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get intermediate results!",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    line->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    tempAid = line->atTokNextint(&err);
    if (err < 0 || (tempAid != activatedAid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get aid or it's different to MD assigned",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    cmdFormat = line->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cmd",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    if (strncmp(DC_ERROR, cmdFormat, strlen(DC_ERROR)) == 0) {
        if (line->atTokHasmore()) {
            cause = line->atTokNextint(&err);  // Not use currently.
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing err cause",
                        rid, __FUNCTION__);
            } else {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EPDN(ifst) for activatedAid%d returns err cause : %d",
                        rid, __FUNCTION__, activatedAid, cause);
            }
        }
        return PDN_FAILED;
    } else if (strncmp(DC_NEW, cmdFormat, strlen(DC_NEW)) == 0) {
        rat = line->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing rat type",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }
        // Value from 3~6 will be view as IWLAN, which is related to VoData feature
        if (rat >= ACCESS_NW_TYPE_VODATA1 && rat <= ACCESS_NW_TYPE_VODATA4) {
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] rat=%d change to IWLAN(2)", m_slot_id, __FUNCTION__, rat);
            rat = ACCESS_NW_TYPE_IWLAN;
        }
        updateRat(activatedAid, rat);

        transIntfId = line->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing transIntfId",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] push transIntfId %d to pdn table [%d]",
                rid, __FUNCTION__, transIntfId, activatedAid);
        updateTransIntfId(activatedAid, transIntfId);

        mtu = line->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing mtu",
                    rid, __FUNCTION__);
            updateMtu(activatedAid, 0);
            return PDN_FAILED;
        }
        updateMtu(activatedAid, mtu);

        pdnType = line->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing addrType",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }

        addr1 = line->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing addr1",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }

        if (line->atTokHasmore()) {
            addr2 = line->atTokNextstr(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing addr2",
                        rid, __FUNCTION__);
                return PDN_FAILED;
            }
        }

        if (PDN_SUCCESS != updateIpAddress(pdnType, activatedAid, addr1, addr2)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateIpAddress failed for activatedAid%d",
                    rid, __FUNCTION__, activatedAid);
            return PDN_FAILED;
        }
    } else if (strncmp(DC_UPDATE, cmdFormat, strlen(DC_UPDATE)) == 0) {
        transIntfId = line->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing transIntfId",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }

        if (getTransIntfId(activatedAid) != transIntfId) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] transIntfId%d is different to MD assigned %d",
                    rid, __FUNCTION__, getTransIntfId(activatedAid), transIntfId);
            return PDN_FAILED;
        }

        pdnType = line->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing addrType",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }

        addr1 = line->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing addr1", rid, __FUNCTION__);
            return PDN_FAILED;
        }

        if (line->atTokHasmore()) {
            addr2 = line->atTokNextstr(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing addr2",
                        rid, __FUNCTION__);
                return PDN_FAILED;
            }
        }

        if (PDN_SUCCESS != updateIpAddress(pdnType, activatedAid, addr1, addr2)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateIpAddress failed for activatedAid%d",
                    rid, __FUNCTION__, activatedAid);
            return PDN_FAILED;
        }
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no matched command format (%s)",
                rid, __FUNCTION__, cmdFormat);
        return PDN_FAILED;
    }

    // As AT+EPDN="ifst" return mtuV4, so only to query mtu value if IPv6 is valid.
    if (mMdrsraSupport
            && (pdnType == NETAGENT_ADDR_TYPE_IPV6 || pdnType == NETAGENT_ADDR_TYPE_IPV4V6)) {
        queryAndUpdateMtu(activatedAid);
    }

    return PDN_SUCCESS;
}

int RmcDcCommonReqHandler::updateIpAddress(int pdnType, int aid, char* addr1, char* addr2) {
    char addressV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
    switch (pdnType) {
        case NETAGENT_ADDR_TYPE_IPV4: {
            updateIpv4Address(aid, addr1);
            break;
        }
        case NETAGENT_ADDR_TYPE_IPV6: {
            convertIpv6Address(addressV6, addr1, 0);
            updateIpv6Address(aid, addressV6);
            break;
        }
        case NETAGENT_ADDR_TYPE_IPV4V6: {
            updateIpv4Address(aid, addr1);
            convertIpv6Address(addressV6, addr2, 0);
            updateIpv6Address(aid, addressV6);
            break;
        }
        default:
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no matched pdn type (%d)",
                    m_slot_id, __FUNCTION__, pdnType);
            return PDN_FAILED;
    }
    return PDN_SUCCESS;
}

void RmcDcCommonReqHandler::requestSetupDataCall(const sp<RfxMclMessage>& msg) {
    RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)msg->getData()->getData();
    sp<RfxAtResponse> p_response;
    const int nPdnInfoSize = getPdnTableSize();
    int isEmergency = 0;
    int rid = m_slot_id;
    int transIntfId = INVALID_TRANS_INTF_ID;
    Vector<int> vAidList;
    Vector<int>::iterator it;
    MTK_RIL_Data_Call_Response_v11* response = NULL;
    sp<RfxMclMessage> responseMsg;
    int num = 1;
    char *pReqApn = pReq->dataProfileInfo.apn == NULL ? const_cast<char *>("") : pReq->dataProfileInfo.apn;
    int profileId = pReq->dataProfileInfo.profileId;
    int reason = ACT_REASON_AOSP_NORMAL;
    if (pReq->reason == 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] pReqData[15] = NULL, suggest to check why ril version"
                " is lower than V1.2", rid, __FUNCTION__);
    } else {
        reason = pReq->reason;
    }
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] profileId=%d, reason=%d",
            rid, __FUNCTION__, profileId, reason);
    // replace default apn string for "On Site LTE UICC"
    if ((profileId == RIL_DATA_PROFILE_DEFAULT) && needReplaceDefaultApnToAttachApn()) {
        pReqApn = mAttachApn;
    }

    int preferModem = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_PREFERRED_DATA_SIM, -1);
    bool rfOccupySupported = isNddsRfOccupySupported();

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] preferModem=%d, rfOccupySupported=%d,"
            " profileId=%d, reason=%d", rid, __FUNCTION__, preferModem,
            rfOccupySupported, profileId, reason);

    const char *pProfileType = RmcDcUtility::getProfileType(profileId);
    int protocol = RmcDcUtility::getProtocolType(pReq->dataProfileInfo.protocol);

    ApnTableReq apnTableReq;
    int cmdIndex = 0;
    apnTableReq.apn = const_cast<char *>(pReqApn);
    apnTableReq.username = pReq->dataProfileInfo.user;
    apnTableReq.password = pReq->dataProfileInfo.password;
    apnTableReq.apnTypeId = RmcDcUtility::getApnTypeId(profileId);
    apnTableReq.protocol = pReq->dataProfileInfo.protocol;
    apnTableReq.authtype = pReq->dataProfileInfo.authType;
    cmdIndex = RmcDcCommonReqHandler::getCmdIndexFromApnTable(rid, &apnTableReq);

    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] apn=%s, profile=%s, protocol=%d",
            rid, __FUNCTION__, pReqApn, pProfileType, protocol);

    // Initialize data call response.
    response = (MTK_RIL_Data_Call_Response_v11*)calloc(1, sizeof(MTK_RIL_Data_Call_Response_v11));
    RFX_ASSERT(response != NULL);
    initDataCallResponse(response, num);

    if (!isDataAllowed(pReqApn)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] isDataAllowed() return false!",
                rid, __FUNCTION__);
        response->status = m_nGprsFailureCause = PDP_FAIL_DATA_NOT_ALLOW;
        goto error;
    }

    if (isHandoverRequest(reason, profileId, &transIntfId)) {
        createDataResponse(transIntfId, IPV4V6, response);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] %s", rid, __FUNCTION__,
                responsesToString(response, num).string());
        goto finish;
    }

    // Send command to temporarily OCCUPY RF
    if (rfOccupySupported && profileId == RIL_DATA_PROFILE_DEFAULT && preferModem != rid) {
        mRfOccupyTag++;
        if (mRfOccupyTag > 10000) mRfOccupyTag = 1;
        int data[1] = {mRfOccupyTag};
        sendRfOccupyCommandIfNeed(true);
        sendEvent(RFX_MSG_EVENT_RF_OCCUPY_TIMEOUT, RfxIntsData(data, 1), RIL_CMD_PROXY_5,
                m_slot_id, -1, -1, RF_OCCUPY_EXPIRATION_TIME);
    }

    // Reserve size of AID list.
    vAidList.reserve(nPdnInfoSize);

    /*Fix timing issue of IMS/EIMS PDN*/
    //IMS/EIMS PDN was already deactivated in MD,
    //while setup data call for IMS/EIMS PDN is still ongoing in AP fwk/ril.
    //Refer to ALPS04066280 and ALPS03861864 for detail scenario.
    if ((strncmp(pProfileType, "ims", 3) == 0) &&
        (getMclStatusManager(rid)->getIntValue(RFX_STATUS_KEY_IMS_PDN_STATE) == IMS_OR_EIMS_PDN_DEACTIVATED)) {
        RFX_LOG_I(RFX_LOG_TAG, "[%d] %s pdn is already deactivated at MD, shouldn't be activated", rid, pProfileType);
        goto error;
    } else if ((strncmp(pProfileType, "emergency", 9) == 0) &&
        (getMclStatusManager(rid)->getIntValue(RFX_STATUS_KEY_EIMS_PDN_STATE) == IMS_OR_EIMS_PDN_DEACTIVATED)) {
        RFX_LOG_I(RFX_LOG_TAG, "[%d] %s pdn is already deactivated at MD, shouldn't be activated", rid, pProfileType);
        goto error;
    }
    /*Fix timing issue of IMS/EIMS PDN*/

    // Activate PDN.
    if (PDN_SUCCESS != activatePdn(pReqApn, pProfileType, &vAidList, isEmergency,
            response, cmdIndex, pReq->tdValid > 0 ? &pReq->TD : NULL, pReq->matchAllRuleAllowed)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] activatePdn failed for apn [name=\"%s\",type=\"%s\",cmdIndex=%d]",
                rid, __FUNCTION__, pReqApn, pProfileType, cmdIndex);
        goto error;
    }

    for (it = vAidList.begin(); it != vAidList.end(); it++) {
        int activatedAid = *it;
        // Wait for MD binding NW interface id and updating rat, interfaceId, mtu and address if needed.
        if (PDN_SUCCESS != updatePdnInformation(activatedAid, IPV4V6, NETAGENT_IFST_UP)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updatePdnInformation failed for activatedAid%d",
                    rid, __FUNCTION__, activatedAid);
            response->status = m_nGprsFailureCause = PDP_FAIL_ERROR_UNSPECIFIED;
            goto error;
        } else if (transIntfId == INVALID_TRANS_INTF_ID) {
            transIntfId = getTransIntfId(activatedAid);
        } else if (transIntfId != getTransIntfId(activatedAid)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error occurs when same apn has two different transIntfIds!",
                    rid, __FUNCTION__);
            response->status = m_nGprsFailureCause = PDP_FAIL_ERROR_UNSPECIFIED;
            goto error;
        }
        setProfileId(activatedAid, profileId);
        updateApnName(activatedAid, pReqApn);
        m_pPdnManager->setAccessNetwork(activatedAid,
                RmcDcUtility::radioTechToAccessNw((RIL_RadioTechnology)getCurrentRat()));

        if (PDN_SUCCESS != updateDefaultBearerInfo(activatedAid)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error occurs when updateDefaultBearerInfo aid = %d!",
                    rid, __FUNCTION__, activatedAid);
            response->status = m_nGprsFailureCause = PDP_FAIL_ERROR_UNSPECIFIED;
            goto error;
        }
        updateQosInformation(activatedAid);
        updateTdInfo(activatedAid);
    }

    if (!isAllAidActive(&vAidList)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error occurs when isAllAidActive!",
                rid, __FUNCTION__);
        response->status = m_nGprsFailureCause = PDP_FAIL_LOST_CONNECTION;
        goto error;
    }

    createDataResponse(transIntfId, IPV4V6, response);
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] %s", rid, __FUNCTION__, responsesToString(response, num).string());

    sendEvent(RFX_MSG_EVENT_QUERY_PCO_WITH_AID, RfxIntsData(vAidList.begin(), 1),
            RIL_CMD_PROXY_5, m_slot_id, -1, -1, 100e6);
    sendEvent(RFX_MSG_EVENT_REQ_DATA_CONTEXT_IDS, RfxVoidData(),
            RIL_CMD_PROXY_5, m_slot_id, -1, -1, 100e6);
    setPdnInfoProperty();
#ifdef CMCC_NSIOT_SUPPORT
    /*VOLTE-NSIOT-blocking feature*/
    //Refer to ALPS02029630 for detail scenario.
    if ((strncmp(pProfileType, "ims", 3) == 0) &&
        (getMclStatusManager(rid)->getIntValue(RFX_STATUS_KEY_IMS_PDN_STATE) == IMS_OR_EIMS_PDN_ACTIVATED)) {
        enableVolteIotFirewall(true,transIntfId);
    }
#endif
    goto finish;

error:
    if (profileId == RIL_DATA_PROFILE_DEFAULT) {
        sendRfOccupyCommandIfNeed(false);
    }

    for (it = vAidList.begin(); it != vAidList.end(); it++) {
        int activatedAid = *it;
        deactivateDataCall(activatedAid);
        clearPdnInfo(activatedAid);
    }

    // notify Setup Data Failure Info for customer requirement
    if (DMF_support == 0 && isApShouldReportDataFailCause()){
        notifyDataFailureCause(true);
    }

finish:
    responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxDataCallResponseData(response, num), msg);
    responseToTelCore(responseMsg);

    if (response != NULL) {
        if (profileId == RIL_DATA_PROFILE_VENDOR_MMS && response->status == PDP_FAIL_NONE
                && !vAidList.empty()) {
            int data[2];
            data[0] = RmcDcUtility::getApnTypeId(profileId);
            data[1] = getRat(*(vAidList.begin()));
            sendEvent(RFX_MSG_EVENT_SEND_QUALIFIED_NETWORK_TYPES_CHANGED, RfxIntsData(data, 2),
                    RIL_CMD_PROXY_5, m_slot_id, -1, -1, 500e6);
        }
        freeDataResponse(response);
        FREEIF(response);
    }
    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] finish", rid, __FUNCTION__);
}

void RmcDcCommonReqHandler::enableVolteIotFirewall(bool enable, int transIntfId) {
#ifdef CMCC_NSIOT_SUPPORT
    int rid = m_slot_id;
    int configured_sbp = atoi(getMclStatusManager(rid)->getString8Value(
            RFX_STATUS_KEY_SBP_ID).string());
    if (configured_sbp != 1) {
        RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] enableVolteIotFirewall, not OP01 project", rid, __FUNCTION__);
        return;
    }

    if (RmcDcUtility::isTestSim(m_slot_id) != 1) {
        RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] enableVolteIotFirewall, not TEST SIM", rid, __FUNCTION__);
        return;
    }

    //get Netdagent HIDL service
    hidl_string hidl_cmd;
    bool result;
    char* cmd = NULL;

    NetAgentService *pNetAgentService = NetAgentService::getInstance();
    if (NULL == pNetAgentService) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] NetAgentService is NULL", rid, __FUNCTION__);
        return;
    }

    //execute  netdagent
    if (enable) {
        //cmd = String.format("netdagent firewall set_volte_nsiot_firewall %s", ifc);
        asprintf(&cmd, "netdagent firewall set_volte_nsiot_firewall  %s%d",
                    pNetAgentService->getInterfaceName(), getInterfaceId(transIntfId));
    } else {
        //cmd = String.format("netdagent firewall clear_volte_nsiot_firewall %s", ifc);
        asprintf(&cmd, "netdagent firewall clear_volte_nsiot_firewall  %s%d",
                    pNetAgentService->getInterfaceName(), getInterfaceId(transIntfId));
    }
    RFX_ASSERT(cmd != NULL);
    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] cmd:%s",rid, __FUNCTION__, cmd);

    // switch to adil flow
    std::string ins = std::string() + INetdagents::descriptor + "/default";
    std::shared_ptr<INetdagents> netdagent;
    netdagent = INetdagents::fromBinder(SpAIBinder(AServiceManager_getService(ins.c_str())));
    if (netdagent != nullptr) {
        ndk::ScopedAStatus status = netdagent->dispatchNetdagentCmd(cmd, &result);
        if (!status.isOk()) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aidl:netdagent->dispatchNetdagentCmd() with %s fail",rid, __FUNCTION__, cmd);
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aidl:netdagent->dispatchNetdagentCmd() with %s ok",rid, __FUNCTION__, cmd);
        }
        FREEIF(cmd);
        return;
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] get %s aidl service failed",rid, __FUNCTION__, INetdagents::descriptor);
    }

    android::sp<INetdagent> gNetdagentService = INetdagent::tryGetService();
    if (gNetdagentService == nullptr) {
        FREEIF(cmd);
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] get %s service failed",rid, __FUNCTION__, INetdagent::descriptor);
        return;
    }
    hidl_cmd = cmd;
    Return<bool> ret = gNetdagentService->dispatchNetdagentCmd(hidl_cmd);
    FREEIF(cmd);
    if(!ret.isOk()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] dispatchNetdagentCmd is not ok",
                rid, __FUNCTION__);
        return;
    }
    if(ret == false) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] dispatchNetdagentCmd failed",
                rid, __FUNCTION__);
        return;
    }
#endif
}

void RmcDcCommonReqHandler::notifyDataFailureCause(bool reqDataCall) {
    sp<RfxMclMessage> urcMsg;
    char failureCause[46] = {0};
    RfxNwServiceState defaultServiceState(0, 0, 0, 0);
    RfxNwServiceState ss;
    int dataRadioTech;
    String8 signalStr8;

    RFX_LOG_E(RFX_LOG_TAG, "[%d] urc [%s]", m_slot_id, __FUNCTION__, "notifyDataFailureCause");

    // Header
    // OEM Type
    failureCause[0] = 0x00;
    // Message ID
    failureCause[1] = 0x0B;
    failureCause[2] = 0x00;
    failureCause[3] = 0x03;
    // Payload Data
    failureCause[4] = 0x00;
    failureCause[5] = 0x00;
    failureCause[6] = 0x00;
    failureCause[7] = 0x26;
    // PayLoad Data
    // Modem Event ID
    if (reqDataCall) {
        failureCause[8] = 0x00; // Data Setup Call Failure
    } else {
        failureCause[8] = 0x01; // Data Call Disconnect Cause
    }
    // Tag of technology
    failureCause[9] = 0x00;
    // Length of technology
    failureCause[10] = 0x01;
    // Radio technology
    ss = getMclStatusManager()
            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    dataRadioTech = ss.getRilDataRadioTech();
    failureCause [11] = dataRadioTech & 0xFF;
    // Tag of Network Cause
    failureCause[12] = 0x01;
    // Length of Network Cause
    failureCause[13] = 0x04;
    // Network Cause
    if (causeType == DSI_CE_TYPE_INTERNAL){
        failureCause[14] = 0x00;
        failureCause[15] = 0x00;
        failureCause[16] = 0x00;
        failureCause[17] = 0x00;
    } else {
        failureCause[14] = (m_nGprsFailureCause >> 24) & 0xFF;
        failureCause[15] = (m_nGprsFailureCause >> 16) & 0xFF;
        failureCause[16] = (m_nGprsFailureCause >> 8) & 0xFF;
        failureCause[17] = m_nGprsFailureCause & 0xFF;
    }
    RFX_LOG_D(RFX_LOG_TAG, "notifyDataFailureCause, Network Cause = [%d][%d][%d][%d]\n",
        failureCause[14],failureCause[15],failureCause[16],failureCause[17]);
    // Tag of profile ID
    failureCause[18] = 0x02;
    // Length of profile ID
    failureCause[19] = 0x01;
    // Profile ID
    failureCause[20] = 0x00;
    // Tag of local cause
    failureCause[21] = 0x03;
    // Length of local cause
    failureCause[22] = 0x04;
    // Local Cause
    if (causeType == DSI_CE_TYPE_3GPP_SPEC_DEFINED){
        failureCause[23] = 0x00;
        failureCause[24] = 0x00;
        failureCause[25] = 0x00;
        failureCause[26] = 0x00;
    } else {
        failureCause[23] = (m_nGprsFailureCause >> 24) & 0xFF;
        failureCause[24] = (m_nGprsFailureCause >> 16) & 0xFF;
        failureCause[25] = (m_nGprsFailureCause >> 8) & 0xFF;
        failureCause[26] = m_nGprsFailureCause & 0xFF;
    }
    RFX_LOG_D(RFX_LOG_TAG, "notifyDataFailureCause, Local Cause = [%d][%d][%d][%d]\n",
    failureCause[23],failureCause[24],failureCause[25],failureCause[26]);
    // Tag of handover attach
    failureCause[27] = 0x04;
    // Length of handover attach
    failureCause[28] = 0x01;
    // Handover attach
    failureCause[29] = 0x00;
    // Tag of cause type
    failureCause[30] = 0x05;
    // Length of cause type
    failureCause[31] = 0x02;
    // Cause type
    failureCause[32] = 0x00;
    failureCause[33] = causeType;
    // Tag of Signal strength
    failureCause[34] = 0x07;
    // Length of Signal strength
    failureCause[35] = 0x04;
    // Tag of Signal quality
    failureCause[40] = 0x08;
    // Length of Signal quality
    failureCause[41] = 0x04;
    // Signal strength and Signal quality
    signalStr8 = getMclStatusManager()->getString8Value(
        RFX_STATUS_KEY_SIGNAL_STRENGTH, String8(""));
    if (signalStr8.size() > 0) {
        const char *siStr = signalStr8.string();
        char tmpSiStr[32] = {0};
        strncpy(tmpSiStr, siStr, 31);
        const char *delim = ",";
        char *pch;
        char* savePtr = NULL;
        RFX_LOG_D(RFX_LOG_TAG, "Splitting string [%s] into tokens:", siStr);
        pch = strtok_r(tmpSiStr,delim, &savePtr);
        char *splitStr[3];
        int idx = 0;
        while (pch != NULL && idx < 3) {
            RFX_LOG_D(RFX_LOG_TAG, "%s\n",pch);
            splitStr[idx++] = pch;
            pch = strtok_r(NULL, delim, &savePtr);
        }
        char *rsrp = splitStr[1];
        char *rsrq = splitStr[2];
        // Signal strength
        if (rsrp != NULL && strlen(rsrp) > 0) {
            int signalStrength = atoi(rsrp);
            failureCause[36] = (signalStrength >> 24) & 0xFF;
            failureCause[37] = (signalStrength >> 16) & 0xFF;
            failureCause[38] = (signalStrength >> 8) & 0xFF;
            failureCause[39] = signalStrength & 0xFF;
        }
        RFX_LOG_D(RFX_LOG_TAG, "notifyDataFailureCause, Signal strength = [%d][%d][%d][%d]\n",
            failureCause[36],failureCause[37],failureCause[38],failureCause[39]);
        // Signal quality
        if (rsrq != NULL && strlen(rsrq) > 0) {
            int signalQuality = atoi(rsrq);
            failureCause[42] = (signalQuality >> 24) & 0xFF;
            failureCause[43] = (signalQuality >> 16) & 0xFF;
            failureCause[44] = (signalQuality >> 8) & 0xFF;
            failureCause[45] = signalQuality & 0xFF;
        }
        RFX_LOG_D(RFX_LOG_TAG, "notifyDataFailureCause, Signal quality = [%d][%d][%d][%d]\n",
            failureCause[42],failureCause[43],failureCause[44],failureCause[45]);
    }

    RFX_LOG_E(RFX_LOG_TAG, "notifyDataFailureCause urc parsing");

    urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_OEM_HOOK_RAW,
            m_slot_id, RfxRawData(failureCause, 46));
    responseToTelCore(urcMsg);
}

// Default Bearer
void RmcDcCommonReqHandler::onNwPdnAct(const sp<RfxMclMessage>& msg) {
    // +CGEV: NW PDN ACT <aid>
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int err = 0;
    int aid = INVALID_AID;
    RfxAtLine *pLine = NULL;
    char *tempParam = NULL;
    sp<RfxMclMessage> urcMsg;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", rid, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                rid, __FUNCTION__);
        return;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        AT_LINE_FREE(pLine);
        return;
    }

    tempParam = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                rid, __FUNCTION__);
        AT_LINE_FREE(pLine);
        return;
    }

    tempParam += strlen(NW_PDN_ACT);
    aid = RmcDcUtility::stringToBinaryBase(tempParam, 10, &err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting aid to binary, err = %d",
                rid, __FUNCTION__, err);
        AT_LINE_FREE(pLine);
        return;
    }

    // For the current design, we don't expect to use any default bearers which are established by NW.
    // However, if operator's requirements appear in the future, then the design should be rethinking.
    if (confirmPdnUsage(aid, false) != CME_SUCCESS) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] confirmPdnUsage failed for aid%d",
                rid, __FUNCTION__, aid);
    }

    AT_LINE_FREE(pLine);
}

// Default Bearer
void RmcDcCommonReqHandler::onNwPdnDeact(const sp<RfxMclMessage>& msg) {
    // +CGEV: NW PDN DEACT <aid>
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int err = 0;
    int aid = INVALID_AID;
    RfxAtLine *pLine = NULL;
    char *tempParam = NULL;
    const int nPdnInfoSize = getPdnTableSize();

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", rid, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                rid, __FUNCTION__);
        return;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        AT_LINE_FREE(pLine);
        return;
    }

    tempParam = pLine->atTokNextstr(&err);
    if (err < 0){
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                rid, __FUNCTION__);
        AT_LINE_FREE(pLine);
        return;
    }

    tempParam += strlen(NW_PDN_DEACT);
    aid = RmcDcUtility::stringToBinaryBase(tempParam, 10, &err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting aid to binary, err = %d",
                rid, __FUNCTION__, err);
        AT_LINE_FREE(pLine);
        return;
    }

    if (PDN_SUCCESS != updatePdnDeactInfo(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updatePdnDeactInfo failed for aid%d",
                rid, __FUNCTION__, aid);
    }

    // Just telling DDM that AP will not use this aid to let DDM release it.
    if (confirmPdnUsage(aid, false) != CME_SUCCESS) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] confirmPdnUsage failed for aid%d",
                rid, __FUNCTION__, aid);
    }

    for (int i = 0; i < nPdnInfoSize; i++) {
        if (isDedicateBearer(i) && getPrimaryAid(i) == aid) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] clean dedicate pdn aid%d info "
                    "due to default pdn aid%d deactivated", rid, __FUNCTION__, getAid(i), aid);
            clearPdnInfo(i);
        }
    }

    // notify Nw Pdn Deact info for customer requirement
    if (DMF_support == 0 && isApShouldReportDataFailCause()){
        updateLastFailCause(getDeactReason(aid));
        if (isLocalFailCase(getDeactReason(aid))){
            causeType = DSI_CE_TYPE_INTERNAL;
        } else {
            causeType = DSI_CE_TYPE_3GPP_SPEC_DEFINED;
        }
        notifyDataFailureCause(false);
    }

    if (notifyDeactReasonIfNeeded(aid)) {
        sendDataCallListResponse(msg, aid);
        clearPdnInfo(aid);
        AT_LINE_FREE(pLine);
        return;
    }

    clearPdnInfo(aid);
    sendDataCallListResponse(msg);
    requestGetDataContextIds(msg);
    setPdnInfoProperty();

    AT_LINE_FREE(pLine);
}

void RmcDcCommonReqHandler::onNwAct(const sp<RfxMclMessage>& msg) {
    // +CGEV: NW ACT <p_aid> <aid> <event_type>
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int err = 0;
    int aid = INVALID_AID;
    int defaultAid = INVALID_AID;
    RfxAtLine *pLine = NULL;
    char *tempParam = NULL;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", rid, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);
    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!", rid, __FUNCTION__);
        return;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when toke start", rid, __FUNCTION__);
        goto error;
    }

    tempParam = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid", rid, __FUNCTION__);
        goto error;
    }

    tempParam += strlen(NW_ACT);
    defaultAid = RmcDcUtility::stringToBinaryBase(tempParam, 10, &err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting aid to binary, err = %d",
                rid, __FUNCTION__, err);
        goto error;
    }

    aid = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid", rid, __FUNCTION__);
        goto error;
    }

    if (confirmPdnUsage(aid, true) != CME_SUCCESS) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] confirmPdnUsage failed, aid:%d",
                rid, __FUNCTION__, aid);
    }

    setAidAndPrimaryAid(aid, aid, defaultAid);

    if (defaultAid == aid) {
        setIsDedicateBearer(aid, false);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] not to update active status [aid=%d, active=%d]",
                rid, __FUNCTION__, aid, getPdnActiveStatus(aid));
    } else if (INVALID_AID != defaultAid) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] a dedicate bearer is activated [aid=%d]"
                ",[primary id=%d, active=%d]",
                rid, __FUNCTION__, aid, defaultAid, getPdnActiveStatus(defaultAid));
        updatePdnActiveStatus(aid, getPdnActiveStatus(defaultAid));
        setIsDedicateBearer(aid, true);
        updateTransIntfId(aid, getTransIntfId(defaultAid));
        requestOrSendDataCallList(msg, defaultAid);
    }

error:
    AT_LINE_FREE(pLine);
}

void RmcDcCommonReqHandler::onNwDeact(const sp<RfxMclMessage>& msg) {
    // +CGEV: NW DEACT <p_aid> <aid> <event_type>
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int err = 0;
    int aid = INVALID_AID;
    int defaultAid = INVALID_AID;
    RfxAtLine *pLine = NULL;
    char *tempParam = NULL;
    const int nPdnInfoSize = getPdnTableSize();

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", rid, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);
    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!", rid, __FUNCTION__);
        return;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when toke start", rid, __FUNCTION__);
        goto error;
    }

    tempParam = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid", rid, __FUNCTION__);
        goto error;
    }

    tempParam += strlen(NW_DEACT);
    defaultAid = RmcDcUtility::stringToBinaryBase(tempParam, 10, &err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting aid to binary, err = %d",
                rid, __FUNCTION__, err);
        goto error;
    }

    aid = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid", rid, __FUNCTION__);
        goto error;
    }

    if (PDN_SUCCESS != updatePdnDeactInfo(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updatePdnDeactInfo failed for aid%d",
                rid, __FUNCTION__, aid);
    }

    // tell DDM that AP will not use this aid and let DDM release it.
    if (confirmPdnUsage(aid, false) != CME_SUCCESS) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] confirmPdnUsage failed for aid%d",
                rid, __FUNCTION__, aid);
    }

    for (int i = 0; i < nPdnInfoSize; i++) {
        if (i==aid && isDedicateBearer(i) && getPrimaryAid(i) == defaultAid) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] clean dedicate pdn aid%d info "
                    "due to default pdn aid%d deactivated", rid, __FUNCTION__, getAid(i), aid);
            clearPdnInfo(aid);
            requestOrSendDataCallList(msg, defaultAid);
        }
    }

error:
    AT_LINE_FREE(pLine);
}

void RmcDcCommonReqHandler::onNwModify(const sp<RfxMclMessage>& msg) {
    // +CGEV: NW MODIFY <aid>, <change_reason>, <event_type>
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int err = 0;
    int aid = INVALID_AID;
    Vector<int> vAid;
    int defaultAid = INVALID_AID;
    int reason = 0;
    RfxAtLine *pLine = NULL;
    char *tempParam = NULL;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", rid, __FUNCTION__, urc);

    // Reserve size of AID.
    vAid.reserve(1);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                rid, __FUNCTION__);
        goto error;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        goto error;
    }

    tempParam = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                rid, __FUNCTION__);
        goto error;
    }
    tempParam += strlen(NW_MODIFY);
    aid = RmcDcUtility::stringToBinaryBase(tempParam, 10, &err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting aid to binary"
                ", err = %d", rid, __FUNCTION__, err);
        goto error;
    }
    vAid.push_back(aid);

    reason = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing reason",
                rid, __FUNCTION__);
        goto error;
    }

    setAid(aid, aid);
    defaultAid = getPrimaryAid(aid);

    if (!m_pPdnManager->isDedicateBearer(aid)) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] not to update active status [aid=%d, active=%d]",
                rid, __FUNCTION__, aid, getPdnActiveStatus(aid));
    } else if (m_pPdnManager->isDedicateBearer(aid) && INVALID_AID != defaultAid) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] a dedicate bearer is modified [aid=%d]"
                ",[primary id=%d, active=%d]",
                rid, __FUNCTION__, aid, defaultAid, getPdnActiveStatus(defaultAid));
        updateTransIntfId(aid, getTransIntfId(defaultAid));
        updatePdnActiveStatus(aid, getPdnActiveStatus(defaultAid));
    }

    if (DATA_STATE_INACTIVE == isAllAidActive(&vAid)) {
        goto error;
    }

    if ((INVALID_AID != defaultAid) &&
            (DATA_STATE_ACTIVE == getPdnActiveStatus(defaultAid))) {
        requestOrSendDataCallList(msg, defaultAid);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] udpate aid %d thru DataCallListChanged",
                rid, __FUNCTION__, defaultAid);
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] default bearer %d is not activated, "
                "not to send URC of dedicate bearer activation", rid, __FUNCTION__, defaultAid);
    }

    requestQueryPco(aid, 0, NULL, NULL);

error:
    AT_LINE_FREE(pLine);
}

void RmcDcCommonReqHandler::onNwReact(const sp<RfxMclMessage>& msg) {
    // +CGEV: NW REACT <PDP_type>, <PDP_addr>, [<aid>]
    int err = 0;
    int aid = INVALID_AID;
    char *out = NULL;
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    const int nPdnInfoSize = getPdnTableSize();
    RfxAtLine *pLine = NULL;
    char *tempParam = NULL;
    bool isDedicatedBearer = false;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", rid, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                rid, __FUNCTION__);
        goto error;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        goto error;
    }

    tempParam = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing defaultAid",
                rid, __FUNCTION__);
        goto error;
    }

    if (tempParam != NULL && strlen(tempParam) > 0) {
        // Skip PDP type

        if (pLine->atTokHasmore()) {
            // Skip PDP address
            out = pLine->atTokNextstr(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing pdp address",
                        rid, __FUNCTION__);
                goto error;
            }
        }

        if (pLine->atTokHasmore()) {
            aid = pLine->atTokNextint(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                        rid, __FUNCTION__);
                goto error;
            }

            if (aid < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs due to invalid aid=%d",
                        rid, __FUNCTION__, aid);
                goto error;
            }

            for (int i = 0; i < nPdnInfoSize; i++) {
                if (isDedicateBearer(i) && getPrimaryAid(i) == aid) {
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] clean dedicate pdn aid%d info "
                            "due to default pdn aid%d deactivated", rid, __FUNCTION__, i, aid);
                    clearPdnInfo(i);
                }
            }

            isDedicatedBearer = isDedicateBearer(aid);
            clearPdnInfo(aid);
            if (isDedicatedBearer) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] aid%d is a dedicate bearer",
                        rid, __FUNCTION__, aid);
            } else {
                sendDataCallListResponse(msg);
            }
        }
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs on null/empty token string",
                rid, __FUNCTION__);
    }

error:
    AT_LINE_FREE(pLine);
}

// Default Bearer
void RmcDcCommonReqHandler::onMePdnAct(const sp<RfxMclMessage>& msg) {
    // +CGEV: ME PDN ACT <aid>[,<reason>[,<aid_other>[,<WLAN_Offload>[,<SSC>,<old_aid>]]]]
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int activatedAid = INVALID_AID;
    int reason = NO_CAUSE;
    int otherAid = INVALID_AID;
    int wlanOffload = INVALID_VALUE;
    int sscMode = SSC_UNKNOWN;
    int oldAid = INVALID_AID;
    int olderAid = INVALID_AID;
    int newerAid = INVALID_AID;
    int err = 0;
    char *tempParam = NULL;
    RfxAtLine *pLine = NULL;
    sp<RfxAtResponse> p_response;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", rid, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                rid, __FUNCTION__);
        goto error;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        goto error;
    }

    // aid
    tempParam = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                rid, __FUNCTION__);
        goto error;
    }
    tempParam += strlen(ME_PDN_ACT);
    activatedAid = RmcDcUtility::stringToBinaryBase(tempParam, 10, &err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting activatedAid to binary"
                ", err = %d", rid, __FUNCTION__, err);
        goto error;
    }

    // reason
    if (pLine->atTokHasmore()) {
        reason = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing reason for aid%d",
                    rid, __FUNCTION__, activatedAid);
            reason = NO_CAUSE;
        }
    }

    // other aid
    if (pLine->atTokHasmore()) {
        otherAid = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing otherAid for aid%d",
                    rid, __FUNCTION__, activatedAid);
            otherAid = INVALID_AID;
        }
    }

    // WLAN offload
    if (pLine->atTokHasmore()) {
        wlanOffload = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing wlanOffload for aid%d",
                    rid, __FUNCTION__, activatedAid);
            wlanOffload = INVALID_VALUE;
        }
    }

    // SSC mode
    if (pLine->atTokHasmore()) {
        sscMode = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing sscType for aid%d",
                    rid, __FUNCTION__, activatedAid);
            sscMode = SSC_UNKNOWN;
        }
    }

    // old aid
    if (pLine->atTokHasmore()) {
        oldAid = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing oldAid for aid%d",
                    rid, __FUNCTION__, activatedAid);
            oldAid = INVALID_AID;
        }
    }

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] aid%d activated result %d and current state is %d",
            rid, __FUNCTION__, activatedAid, reason, getPdnActiveStatus(activatedAid));

    // As spec, single address bearers only allowed and MT initiated context activation
    // for a second address type was not successful.
    // SINGLE_BEARER_ONLY_ALLOWED_ACTIVATE_FAILED(3) represents this scenario and no need
    // to handle this failed aid.
    if (reason == SINGLE_BEARER_ONLY_ALLOWED_ACTIVATE_FAILED) goto error;

    if (sscMode == SSC_MODE2) {
        // Currently, SSC mode 2 is already supported by AOSP, we don't need to handle it.
        // The flow is as follows:
        // 1. Receive network pdn deact urc.
        // 2. Unsolicited data call list change.
        // 3. Android framework retry data connection.
        goto error;
    } else if (sscMode == SSC_MODE3) {
        olderAid = oldAid;
        newerAid = activatedAid;
    } else {
        olderAid = activatedAid;
        newerAid = otherAid;
    }

    if (olderAid != INVALID_AID && getPdnActiveStatus(olderAid) == DATA_STATE_INACTIVE) {
        updatePdnActiveStatus(olderAid, DATA_STATE_LINKDOWN);  // Update with link down state.
        setAidAndPrimaryAid(olderAid, olderAid, olderAid);
        setReason(olderAid, reason);
    }

    // For IPv4v6 fallback, ex. +CGEV: ME PDN ACT 1, 2, 4
    // For SSC mode3 new PDU, ex. +CGEV: ME PDN ACT 4,,,,2,1
    if (newerAid != INVALID_AID) {
        m_pPdnManager->setAccessNetwork(newerAid,
                RmcDcUtility::radioTechToAccessNw((RIL_RadioTechnology)getCurrentRat()));
        if (getPdnActiveStatus(olderAid) != DATA_STATE_ACTIVE) {
            // confirm with DDM that AP will not use this otherAid.
            if (confirmPdnUsage(newerAid, false, sscMode) != CME_SUCCESS) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] confirmPdnUsage failed for newerAid%d",
                        rid, __FUNCTION__, newerAid);
            }
        } else {
            // confirm with DDM that AP will use this otherAid.
            if (confirmPdnUsage(newerAid, true, sscMode) != CME_SUCCESS) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] confirmPdnUsage failed for newerAid%d",
                        rid, __FUNCTION__, newerAid);
            }

            // update the TransIntfId of otherAid
            if (otherAid != INVALID_AID && newerAid == otherAid) {
                setAidAndPrimaryAid(newerAid, newerAid, newerAid);
                updateTransIntfId(newerAid, getTransIntfId(olderAid));
            }

            // Update rat, interfaceId, mtu and address if needed.
            if (PDN_SUCCESS != updatePdnInformation(newerAid, IPV4V6, NETAGENT_IFST_NONE)) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updatePdnInformation failed for newerAid%d",
                        rid, __FUNCTION__, newerAid);
                goto error;
            }

            if (PDN_SUCCESS != updateDefaultBearerInfo(newerAid)) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateDefaultBearerInfo failed for newerAid%d",
                        rid, __FUNCTION__, newerAid);
                goto error;
            }
            updateActiveStatus();
            updateQosInformation(newerAid);
            updateTdInfo(newerAid);
            if (DATA_STATE_INACTIVE == getPdnActiveStatus(newerAid)) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] newerAid%d is inactive",
                        rid, __FUNCTION__, newerAid);
                goto error;
            }

            if (sscMode == SSC_MODE3) {
                setSscMode(newerAid, sscMode);
                setProfileId(newerAid, getProfileId(olderAid));
                updateApnName(newerAid, getApnName(olderAid));
                updatePdnActiveStatus(newerAid, DATA_STATE_LINKDOWN);
                sendDataCallListResponse(msg, INVALID_AID, sscMode);
            } else {
                sendDataCallListResponse(msg);
            }

            requestQueryPco(olderAid, 0, NULL, NULL);
        }
    }

error:
    AT_LINE_FREE(pLine);
}

// Default Bearer
void RmcDcCommonReqHandler::onMePdnDeact(const sp<RfxMclMessage>& msg) {
    // R12: +CGEV: ME PDN DEACT <aid>
    int err = 0;
    int aid = INVALID_AID;
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    const int nPdnInfoSize = getPdnTableSize();
    RfxAtLine *pLine = NULL;
    char *tempParam = NULL;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", rid, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                rid, __FUNCTION__);
        return;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        AT_LINE_FREE(pLine);
        return;
    }

    tempParam = pLine->atTokNextstr(&err);
    if (err < 0){
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing defaultAid",
                rid, __FUNCTION__);
        AT_LINE_FREE(pLine);
        return;
    }

    tempParam += strlen(ME_PDN_DEACT);
    aid = RmcDcUtility::stringToBinaryBase(tempParam, 10, &err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting aid "
                "to binary, err = %d", rid, __FUNCTION__, err);
        AT_LINE_FREE(pLine);
        return;
    }

    // Just telling DDM that AP will not use this aid to let DDM release it.
    if (confirmPdnUsage(aid, false) != CME_SUCCESS) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] confirmPdnUsage failed for aid%d",
                rid, __FUNCTION__, aid);
    }

    if (getProfileId(aid) == RIL_DATA_PROFILE_DEFAULT) {
        sendRfOccupyCommandIfNeed(false);
    }

    for (int i = 0; i < nPdnInfoSize; i++) {
        if (isDedicateBearer(i) && getPrimaryAid(i) == aid) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] clean dedicate pdn aid%d info "
                    "due to default pdn aid%d deactivated", rid, __FUNCTION__, getAid(i), aid);
            clearPdnInfo(i);
        }
    }

    clearPdnInfo(aid);
    sendDataCallListResponse(msg);
    requestGetDataContextIds(msg);
    setPdnInfoProperty();

    AT_LINE_FREE(pLine);
}

void RmcDcCommonReqHandler::onPdnChange(const sp<RfxMclMessage>& msg) {
    //+EPDN: <aid>,"dcchg",<event>
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int err = 0;
    int aid = INVALID_AID;
    char *cmdFormat = NULL;
    int event = DC_EVENT_UNKNOWN;
    RfxAtLine *pLine = NULL;
    bool needCheckmore = false;
    sp<RfxMclMessage> urcMsg;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", rid, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                rid, __FUNCTION__);
        goto error;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        goto error;
    }

    aid = pLine->atTokNextint(&err);
    if (err < 0) {
        if (pLine->atTokHasmore()) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Has command data, need parsing more",
                    rid, __FUNCTION__);
            // For 93 data retry, if aid is null and has command data
            // We need to check if it is "+EPDN:,"dcchg",129"
            needCheckmore = true;
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                    rid, __FUNCTION__);
            goto error;
        }
    }

    if (!needCheckmore && getTransIntfId(aid) == INVALID_TRANS_INTF_ID) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs due to original"
                " transIntfId removed", rid, __FUNCTION__);
        goto error;
    }

    cmdFormat = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cmd",
                rid, __FUNCTION__);
        goto error;
    }

    if (strncmp(DC_CHANGE, cmdFormat, strlen(DC_CHANGE)) == 0) {
        event = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing event",
                    rid, __FUNCTION__);
        }

        if(needCheckmore && event != DC_EVENT_DATA_RESET_NOTIFY) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Have checked more but event is NOT 129",
                rid, __FUNCTION__);
            goto error;
        }

        switch (event) {
            case DC_EVENT_NO_RA:
                /* Clear IPv6 addresse by aid due to RA fail */
                for (int i = 0; i < MAX_NUM_IPV6_ADDRESS_NUMBER; i++) {
                    updateIpv6Address(aid, i, "");
                }
                sendDataCallListResponse(msg);
                break;
            case DC_EVENT_DATA_RESET_NOTIFY:
                // M: Data Retry, MD notify data count reset
                // +EPDN:,"dcchg",129
                urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_MD_DATA_RETRY_COUNT_RESET, m_slot_id,
                        RfxVoidData());
                responseToTelCore(urcMsg);
                break;
            case DC_EVENT_IP_NORMAL_CHANGE:
                onMePdnPropertyChange(msg, aid);
                break;
            case DC_EVENT_HANDOVER_NORMAL_CHANGE:
                onMePdnPropertyChange(msg, aid, PDN_CHANGE_REASON_HANDOVER);
                sendQualifiedNetworkTypesChanged(aid);
                break;
            default:
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no matched event (%d)",
                        rid, __FUNCTION__, event);
                break;
        }
    }

error:
    AT_LINE_FREE(pLine);
}

void RmcDcCommonReqHandler::onMePdnPropertyChange(const sp<RfxMclMessage>& msg,
        const int aid, PDN_CHANGE_REASON reason) {
    // Handle any PDN link property changing.
    // First use AT+EPDN to confirm that new IP address (if IP changed) already been configured,
    // then query PDN information and sync data call list.
    int rid = m_slot_id;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] aid%d", rid, __FUNCTION__, aid);

    // Because this event was sent from URC channel to Data channel asynchronously, need to
    // confirm active status before do other things.
    updateActiveStatus();
    if (DATA_STATE_INACTIVE == getPdnActiveStatus(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid%d is inactive", rid, __FUNCTION__, aid);
        return;
    }

    if (PDN_SUCCESS != updatePdnAddress(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updatePdnAddress failed for aid%d",
                rid, __FUNCTION__, aid);
        return;
    }

    if (PDN_SUCCESS != updateDefaultBearerInfo(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateDefaultBearerInfo failed for aid%d",
                rid, __FUNCTION__, aid);
        return;
    }

    if (mMdrsraSupport) queryAndUpdateMtu(aid);
    updateQosInformation(aid, m_pPdnManager->isDedicateBearer(aid));
    updateTdInfo(aid);
    if (!(RmcDcUtility::isXfrmSupport() && reason == PDN_CHANGE_REASON_HANDOVER)) {
        sendDataCallListResponse(msg);
    }
}

// Support IWLAN AP-ASSISTED mode
void RmcDcCommonReqHandler::onQualifiedNetworkTypeChanged(const sp<RfxMclMessage>& msg) {
    char feature[] = "IWLPL_VERNO";
    int supportVersion = getFeatureVersion(feature, 1);

    if (supportVersion == 1) {
        handleQualifiedNetworkTypeChanged_V1(msg);
    } else if (supportVersion == 2){
        handleQualifiedNetworkTypeChanged_V2(msg);
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] unknown version for QualifiedNetworkTypeChanged!",
                m_slot_id, __FUNCTION__);
    }
}

void RmcDcCommonReqHandler::handleQualifiedNetworkTypeChanged_V1(const sp<RfxMclMessage>& msg) {
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int err = 0;
    std::unique_ptr<RfxAtLine> pLine(new RfxAtLine(urc, NULL));
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc=%s", rid, __FUNCTION__, urc);
    pLine->atTokStart(&err);
    if (err < 0) return;

    char *cmdFormat = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cmd", rid, __FUNCTION__);
        return;
    }
    sp<RfxMclMessage> urcMsg;
    bool isUrcForActiveApnType = false;
    int mPdnTableSize = getPdnTableSize();
    PdnInfo pdnInfo;

    // Update Inter Wireless LAN Priority List
    // [Legacy format]
    // +EIWLPL:<cmd>,<type>,<priority for setup>,<priority for cellular>,<priority for wifi>,<description>
    //  <cmd> : 'notify'
    //  <type>: 'ims' or 'emergency' or 'mms
    //  <priority for setup> : 1 ~ 5
    //  <priority for cellular> : 1 ~ 5
    //  <priority for wifi> : 1 ~ 5
    //  <descrption> : just for log print, e.g. wifi > cellular

    //  value of priority
    //       0: start
    //       1: null
    //       2: cellular
    //       3: wifi
    //       4: cellular > wifi
    //       5: wifi > cellular
    //       6: end

    if (strncmp("notify", cmdFormat, strlen("notify")) == 0) {
        int priorityListForSetup = 0;
        int priorityListForCellular = 0;
        int priorityListForWifi = 0;
        char *type = NULL;
        char *descrption = NULL;

        int matchApnType = -1;
        int supportNetworkTypes = 0;
        char tempApnTypeStr[MAX_AT_CMD_LENGTH] = {0};

        type = pLine->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing type",
                    rid, __FUNCTION__);
            return;
        }
        priorityListForSetup = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing priorityListForSetup",
                    rid, __FUNCTION__);
            return;
        }
        priorityListForCellular = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing priorityListForCellular",
                    rid, __FUNCTION__);
            return;
        }
        priorityListForWifi = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing priorityListForWifi",
                    rid, __FUNCTION__);
            return;
        }
        descrption = pLine->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing descrption",
                    rid, __FUNCTION__);
            return;
        }

        // get str of type to compare
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] type=%s", rid, __FUNCTION__, type);
        for (int i = 0; i < MAX_COUNT_QUALIFIED_NETWORK_UPDATE_APNS; i++) {
            memset(tempApnTypeStr, 0, MAX_AT_CMD_LENGTH);
            strncpy(tempApnTypeStr,
                    RmcDcUtility::getApnType(SUPPORT_QUALIFIED_NETWORK_UPDATE_APNS[i]),
                    MAX_AT_CMD_LENGTH - 1);
            tempApnTypeStr[MAX_AT_CMD_LENGTH - 1] = '\0';
            if (strncmp(tempApnTypeStr, type, strlen(tempApnTypeStr)) == 0){
                matchApnType = SUPPORT_QUALIFIED_NETWORK_UPDATE_APNS[i];
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] matchApnType=%d", rid, __FUNCTION__, matchApnType);
                break;
            }
        }

        if (matchApnType < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] apn=%s not support qualified network update!",
                        rid, __FUNCTION__, type);
            return;
        }

        // matchApnType > 0
        bool urcWith1Type = true;
        int urc1Type[3] = {1, matchApnType, ACCESS_NW_TYPE_UNKNOWN};
        int urc2Types[4] = {1, matchApnType, ACCESS_NW_TYPE_UNKNOWN, ACCESS_NW_TYPE_UNKNOWN};
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] priorityListForSetup=%d", rid, __FUNCTION__,
                priorityListForSetup);
        switch(priorityListForSetup) {
            case PRIORITY_CELLULAR_ONLY:
                urc1Type[2] = ACCESS_NW_TYPE_CELLULAR;
                break;

            case PRIORITY_WIFI_ONLY:
                urc1Type[2] = ACCESS_NW_TYPE_IWLAN;
                break;

            case PRIORITY_CELLULAR_PREFER:
            case PRIORITY_WIFI_PREFER:
                supportNetworkTypes = getSupportNetworkTypes(matchApnType);
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] supportNetworkTypes=%d",
                        rid, __FUNCTION__, supportNetworkTypes);
                if (supportNetworkTypes == SUPPORT_TRANSPORT_MODE_CELLULAR_WIFI) {
                    if (priorityListForSetup == PRIORITY_CELLULAR_PREFER) {
                        urc2Types[2] = ACCESS_NW_TYPE_CELLULAR;
                        urc2Types[3] = ACCESS_NW_TYPE_IWLAN;
                    } else {
                        urc2Types[2] = ACCESS_NW_TYPE_IWLAN;
                        urc2Types[3] = ACCESS_NW_TYPE_CELLULAR;
                    }
                    urcWith1Type = false;
                } else if (supportNetworkTypes > SUPPORT_TRANSPORT_MODE_UNKNOW) {
                    if (supportNetworkTypes == SUPPORT_TRANSPORT_MODE_CELLULAR_ONLY) {
                        urc1Type[2] = ACCESS_NW_TYPE_CELLULAR;
                    } else {
                        urc1Type[2] = ACCESS_NW_TYPE_IWLAN;
                    }
                } else {
                     RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] supportNetworkTypes unknown: %d",
                             rid, __FUNCTION__, supportNetworkTypes);
                }
                break;

            default:
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] priority list for setup unknown: %d",
                            rid, __FUNCTION__, priorityListForSetup);
                return;
        }

        // check if APN type(ims or emergency or mms) of URC is active state
        for (int i = 0; i < mPdnTableSize; i++) {
            int currentAid = getAid(i);
            if (currentAid != INVALID_AID) {
                pdnInfo = getPdnInfo(currentAid);
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] pdnInfo.profileId=%d, pdn_info.active=%d",
                        rid, __FUNCTION__, pdnInfo.profileId, pdnInfo.active);
                if ((matchApnType == RmcDcUtility::getApnTypeId(pdnInfo.profileId))
                        && (pdnInfo.active == DATA_STATE_ACTIVE)) {
                    isUrcForActiveApnType = true;
                    break;
                }
            }
        }

        if (isUrcForActiveApnType) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] apn state is active skip to notify",
                    rid, __FUNCTION__);
        } else {
            if (urcWith1Type) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urcWith1Type: %d",
                            rid, __FUNCTION__, urc1Type[2]);
                urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_QUALIFIED_NETWORK_TYPES_CHANGED,
                        m_slot_id, RfxIntsData(urc1Type, 3));
                responseToTelCore(urcMsg);
            } else {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urcWith2Types: %d",
                            rid, __FUNCTION__, urc2Types[2]);
                urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_QUALIFIED_NETWORK_TYPES_CHANGED,
                        m_slot_id, RfxIntsData(urc2Types, 4));
                responseToTelCore(urcMsg);
            }
        }
    } else {
        RFX_LOG_E(RFX_LOG_TAG,
                "[%d][%s] unknown <cmd> for handleQualifiedNetworkTypeChanged_V1",
                rid, __FUNCTION__);
    }
}

void RmcDcCommonReqHandler::handleQualifiedNetworkTypeChanged_V2(const sp<RfxMclMessage>& msg) {
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int err = 0;
    std::unique_ptr<RfxAtLine> pLine(new RfxAtLine(urc, NULL));
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] urc=%s", rid, __FUNCTION__, urc);
    pLine->atTokStart(&err);
    if (err < 0) return;

    char *cmdFormat = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cmd", rid, __FUNCTION__);
        return;
    }
    sp<RfxMclMessage> urcMsg;
    bool isUrcForActiveApnType = false;
    int mPdnTableSize = getPdnTableSize();
    PdnInfo pdnInfo;

    // [New Format]
    // AT+EIWLPL=2
    // +EIWLPL: "<cmd>", <PDN type>, <number of RAT list>[, <rat1>[, ...]]
    //
    // cmd "setup" report timing is same as legacy format's "notify"
    //
    // Rat value
    // UNSPEC = 0,
    // CELL = 1,
    // WLAN = 2,
    // DATA_1 = 3,
    // DATA_2 = 4,
    // DATA_3 = 5,
    // DATA_4 = 6,
    // NONE = 254
    //
    // Max number of RAT list is defined to 6, current scenario will be 3
    //
    // e.g. IMS PDN priority for setup
    // +EIWLPL: "setup", "ims", 2, 1, 2    // priority for IMS PDN setup (Cellular(1) > WIFI(2))
    // +EIWLPL: "setup", "ims", 0          // No suggested RAT
    if (strncmp("setup", cmdFormat, strlen("setup")) == 0) {
        char *type = NULL;
        int matchApnType = -1;
        char tempApnTypeStr[MAX_AT_CMD_LENGTH] = {0};

        type = pLine->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing type",
                    rid, __FUNCTION__);
            return;
        }

        // get str of type to campare
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] type=%s", rid, __FUNCTION__, type);
        for (int i = 0; i < MAX_COUNT_QUALIFIED_NETWORK_UPDATE_APNS; i++) {
            strncpy(tempApnTypeStr,
                    RmcDcUtility::getApnType(SUPPORT_QUALIFIED_NETWORK_UPDATE_APNS[i]),
                    MAX_AT_CMD_LENGTH - 1);
            tempApnTypeStr[MAX_AT_CMD_LENGTH - 1] = '\0';
            if (strncmp(tempApnTypeStr, type, strlen(tempApnTypeStr)) == 0){
                matchApnType = SUPPORT_QUALIFIED_NETWORK_UPDATE_APNS[i];
                RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] matchApnType=%d", rid, __FUNCTION__, matchApnType);
                break;
            }
        }
        if (matchApnType < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no match APN type.", rid, __FUNCTION__);
            return;
        }

        // get number of RAT in list
        int num = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing priorityListForSetup",
                    rid, __FUNCTION__);
            return;
        }

        // get RAT
        int rat = ACCESS_NW_TYPE_UNKNOWN;
        int ratList[6] = {ACCESS_NW_TYPE_UNKNOWN};// [0] for mode, [1] for APN type
        int ratIndex = 2;
        int skipIwlan = false;
        for (int i = 0; i < num; i++) {
            rat = pLine->atTokNextint(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing priorityListForSetup",
                        rid, __FUNCTION__);
                return;
            }

            // If prefer is VoData, transfer type to IWLAN
            if (rat >= ACCESS_NW_TYPE_VODATA1 && rat <= ACCESS_NW_TYPE_VODATA4) {
                rat = ACCESS_NW_TYPE_IWLAN;
            }

            if (rat == ACCESS_NW_TYPE_IWLAN) {
                if (!skipIwlan) {
                    skipIwlan = true;
                    ratList[ratIndex] = rat;
                    ratIndex++;
                }
            } else {
                ratList[ratIndex] = rat;
                ratIndex++;
            }
        }

        // check if APN type(ims or emergency or mms) of URC is active state
        for (int i = 0; i < mPdnTableSize; i++) {
            int currentAid = getAid(i);
            if (currentAid != INVALID_AID) {
                pdnInfo = getPdnInfo(currentAid);
                RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] pdnInfo.profileId=%d, pdn_info.active=%d",
                        rid, __FUNCTION__, pdnInfo.profileId, pdnInfo.active);
                if ((matchApnType == RmcDcUtility::getApnTypeId(pdnInfo.profileId))
                        && (pdnInfo.active == DATA_STATE_ACTIVE)) {
                    isUrcForActiveApnType = true;
                    break;
                }
            }
        }

        if (isUrcForActiveApnType) {
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] apn state is active skip to notify",
                    rid, __FUNCTION__);
        } else {
            ratList[0] = 1; //Mode
            ratList[1] = matchApnType; //APN type
            urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_QUALIFIED_NETWORK_TYPES_CHANGED,
                    m_slot_id, RfxIntsData(ratList, (num + 2)));
            responseToTelCore(urcMsg);
        }
    } else {
        RFX_LOG_E(RFX_LOG_TAG,
                "[%d][%s] unknown <cmd> for handleQualifiedNetworkTypeChanged_V2!",
                rid, __FUNCTION__);
    }
}

void RmcDcCommonReqHandler::requestDeactivateDataCall(const sp<RfxMclMessage>& msg) {
    const char **pReqData = (const char **)msg->getData()->getData();
    int transIntfId = atoi(pReqData[0]);
    int reason = atoi(pReqData[1]);
    int rid = m_slot_id;
    int err = 0;
    int count = 0;
    const int nPdnInfoSize = getPdnTableSize();
    sp<RfxMclMessage> responseMsg;
    sp<RfxMclMessage> urcMsg;

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] transIntfId=%d, reason: %d",
            rid, __FUNCTION__, transIntfId, reason);

    if (reason != DEACT_REASON_AOSP_HANDOVER && transIntfId > INVALID_TRANS_INTF_ID) {
        for (int i = 0; i < nPdnInfoSize; i++) {
            if ((getTransIntfId(i) == transIntfId) && !isDedicateBearer(i)) {
                // Error handling if needed.
                err = deactivateDataCall(getAid(i), reason);
#ifdef CMCC_NSIOT_SUPPORT
                /*VOLTE-NSIOT-blocking feature*/
                //Refer to ALPS02029630 for detail scenario.
                PdnInfo pdn_info = getPdnInfo(i);
                if (pdn_info.profileId == RIL_DATA_PROFILE_IMS && pdn_info.active == DATA_STATE_ACTIVE) {
                    enableVolteIotFirewall(false,transIntfId);
                }
#endif
                // Reset pdn info with current aid.
                clearPdnInfo(getAid(i));
                count++;
            }
        }
    }

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] transIntfId=%d, reason: %d count: %d",
            rid, __FUNCTION__, transIntfId, reason, count);
    sendEvent(RFX_MSG_EVENT_REQ_DATA_CONTEXT_IDS, RfxVoidData(),
            RIL_CMD_PROXY_5, m_slot_id, -1, -1, 100e6);
    setPdnInfoProperty();

    responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(responseMsg);

    // As AOSP design framework layer not handle deactivate data call done event,
    // need rild notify data call list change event to framework.
    if (count > 0) {
        urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED,
                m_slot_id, RfxVoidData());
        requestOrSendDataCallList(urcMsg);
    }
}

int RmcDcCommonReqHandler::deactivateDataCall(const int aid) {
    return deactivateDataCall(aid, DEACT_REASON_UNKNOWN);
}

int RmcDcCommonReqHandler::deactivateDataCall(const int aid, const int reason) {
    int rid = m_slot_id;
    int err = 0;
    int respAid = INVALID_AID;
    char *tmpStr = NULL;
    char *tmpAid = NULL;
    String8 cmd("");
    RfxAtLine *pLine = NULL;
    sp<RfxAtResponse> p_response;

    // Check if AID is out of bound.
    if (!validateAid(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid=%d is out of bound",
                rid, __FUNCTION__, aid);
        return CME_ERROR_NON_CME;
    }

    if (getProfileId(aid) == RIL_DATA_PROFILE_DEFAULT) {
        sendRfOccupyCommandIfNeed(false);
    }

    // AT+EAPNACT=<state>,<aid>[,<reason>];
    // <state>: 0-deactivate; 1-activate
    // <reason>: see PDN_DEACT_REASON in RmcDataDefs.h for more details
    cmd.append(String8::format("AT+EAPNACT=0,%d,%d", aid, convertDeactCauseToMdStandard(reason)));
    p_response = atSendCommandSingleline(cmd, "+CGEV: ME PDN DEACT ");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return CME_ERROR_NON_CME;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() == 0) {
            int cause = p_response->atGetCmeError();
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] CME CAUSE : %d",
                    rid, __FUNCTION__, cause);
            return cause;
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] CME CAUSE : CME_UNKNOWN",
                    rid, __FUNCTION__);
            return CME_UNKNOWN;
        }
    }

    pLine = p_response->getIntermediates();

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get intermediate results!",
                rid, __FUNCTION__);
        return CME_ERROR_NON_CME;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        return CME_ERROR_NON_CME;
    }

    tmpStr = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                rid, __FUNCTION__);
        return CME_ERROR_NON_CME;
    }
    tmpAid = tmpStr + strlen(ME_PDN_DEACT);
    respAid = RmcDcUtility::stringToBinaryBase(tmpAid, 10, &err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting aid to binary"
                ", err = %d", rid, __FUNCTION__, err);
        return CME_ERROR_NON_CME;
    }

    if (respAid != aid) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid%d is different to MD assigned aid%d",
                rid, __FUNCTION__, aid, respAid);
        return CME_ERROR_NON_CME;
    }

    return CME_SUCCESS;
}

bool RmcDcCommonReqHandler::lockAndClearApn() {
    sp<RfxAtResponse> p_response;
    // lock the apn table
    p_response = atSendCommand("AT+EAPNLOCK=1");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] 1. fail to get p_response!",
                m_slot_id, __FUNCTION__);
        return false;
    }

    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EAPNLOCK=1 returns ERROR",
                m_slot_id, __FUNCTION__);
        return false;
    }

    // clear the apn table
    p_response = atSendCommand("AT+EAPNSET");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] 2. fail to get p_response!",
                m_slot_id, __FUNCTION__);
        return false;
    }

    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EAPNSET clear apn table ERROR",
                m_slot_id, __FUNCTION__);
        return false;
    }
    return true;
}

bool RmcDcCommonReqHandler::unlockApn() {
    sp<RfxAtResponse> p_response;
    // unlock the apn table
    p_response = atSendCommand("AT+EAPNLOCK=0");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] 5. fail to get p_response!",
                m_slot_id, __FUNCTION__);
        return false;
    }else if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EAPNLOCK=0 returns ERROR",
                m_slot_id, __FUNCTION__);
        return false;
    }
    return true;
}

bool RmcDcCommonReqHandler::syncApnItem(int apnIndex) {
    sp<RfxAtResponse> p_response;
    String8 cmd("");
    int supportedTypesBitmask = s_LastApnTable[m_slot_id][apnIndex].supportedTypesBitmask;
    char strParam[MAX_AT_CMD_LENGTH] = {0};
    char tmpStrParam[MAX_AT_CMD_LENGTH] = {0};
    char ApnTypeStr[MAX_AT_CMD_LENGTH] = {0};
    char tempApnTypeStr[MAX_AT_CMD_LENGTH] = {0};
    int countOfApnType = 0;
    char apn[MAX_APN_NAME_LENGTH] = {0};
    char user[MAX_APN_NAME_LENGTH] = {0};
    char password[MAX_APN_NAME_LENGTH] = {0};
    int n = 0;

    // After adjustment, there might be supportBitmask 0 case, just discard whole record
    if (!supportedTypesBitmask)
        return true;

    // To sync IMS/Eemergency PDN default mtu config to MD -- START
    if ((supportedTypesBitmask & RIL_APN_TYPE_IMS) == RIL_APN_TYPE_IMS) {
        cmd.clear();
        cmd.append(String8::format("AT+EPDNCFG=\"ims_mtu\",%d",s_LastApnTable[m_slot_id][apnIndex].mtu));
        p_response = atSendCommand(cmd);
    }

    if ((supportedTypesBitmask & RIL_APN_TYPE_EMERGENCY) == RIL_APN_TYPE_EMERGENCY) {
        cmd.clear();
        cmd.append(String8::format("AT+EPDNCFG=\"eims_mtu\",%d",s_LastApnTable[m_slot_id][apnIndex].mtu));
        p_response = atSendCommand(cmd);
    }
    // To sync IMS/Eemergency PDN default mtu config to MD -- END

    // update the user name and password
    // AT+EAPNSET = "apn",cmdIndex,"user","pwd"
    strncpy(apn, s_LastApnTable[m_slot_id][apnIndex].apn, MAX_APN_NAME_LENGTH-1);
    apn[MAX_APN_NAME_LENGTH-1] = '\0';
    addEscapeSequence(apn);
    strncpy(user, s_LastApnTable[m_slot_id][apnIndex].user, MAX_APN_NAME_LENGTH-1);
    user[MAX_APN_NAME_LENGTH-1] = '\0';
    addEscapeSequence(user);
    strncpy(password, s_LastApnTable[m_slot_id][apnIndex].password, MAX_APN_NAME_LENGTH-1);
    password[MAX_APN_NAME_LENGTH-1] = '\0';
    addEscapeSequence(password);
    cmd.clear();
    cmd.append(String8::format("AT+EAPNSET=\"%s\",%d,\"%s\",\"%s\"",
            apn, s_ApnCmdIndex[m_slot_id][apnIndex], user, password));
    p_response = atSendCommand(cmd);

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] 3. fail to get p_response!",
                m_slot_id, __FUNCTION__);
        return false;
    }

    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EAPNSET update user name and password ERROR",
                m_slot_id, __FUNCTION__);
        return false;
    }

    // conver supportedTypesBitmask to apn type stting.
    // the format of apn type string is "type, type, type, ..."
    memset(ApnTypeStr, 0, MAX_AT_CMD_LENGTH);
    countOfApnType = 0;
    if (supportedTypesBitmask == RIL_APN_TYPE_ALL ||
            supportedTypesBitmask == RIL_APN_TYPE_MTKALL) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] Unexpected to configure apn to support all types",
                m_slot_id, __FUNCTION__);
        strncpy(ApnTypeStr, RmcDcUtility::getApnType(supportedTypesBitmask),
                MAX_AT_CMD_LENGTH-1);
        ApnTypeStr[MAX_AT_CMD_LENGTH-1] = '\0';
    } else {
        for (int j = 0; j < RIL_APN_TYPE_COUNT; j++) {
            int typeBitmask = 1 << j;
            if (supportedTypesBitmask & typeBitmask) {
                memset(tempApnTypeStr, 0, MAX_AT_CMD_LENGTH);
                strncpy(tempApnTypeStr, RmcDcUtility::getApnType(typeBitmask),
                        MAX_AT_CMD_LENGTH - 1);
                tempApnTypeStr[MAX_AT_CMD_LENGTH - 1] = '\0';
                if (countOfApnType > 0) {
                    strncat(ApnTypeStr + strlen(ApnTypeStr), ",",
                            MAX_AT_CMD_LENGTH - strlen(ApnTypeStr) - 1);
                }
                strncat(ApnTypeStr + strlen(ApnTypeStr), tempApnTypeStr,
                        MAX_AT_CMD_LENGTH - strlen(ApnTypeStr) - 1);
                countOfApnType++;
            }
        }
    }

    // update other parameters
    // AT+EAPNSET = "apn",cmdIndex+1,"param1=value1;param2=value2;param3=value3;..."
    memset(strParam, 0, MAX_AT_CMD_LENGTH);
    if (strlen(ApnTypeStr) > 0) {
        n = snprintf(strParam, MAX_AT_CMD_LENGTH, "type=%s;", ApnTypeStr);
        if (n < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to print Apn Type Str",
                    m_slot_id, __FUNCTION__);
            return false;
        }
    }
    memset(tmpStrParam, 0, MAX_AT_CMD_LENGTH);
    n = snprintf(tmpStrParam, MAX_AT_CMD_LENGTH, "protocol=%s;roaming_protocol=%s;"
            "authtype=%d;carrier_enabled=%d;max_conns=%d;max_conns_time=%d;wait_time=%d;"
            "bearer_bitmask=%u;inactive_timer=%d",
            strlen(s_LastApnTable[m_slot_id][apnIndex].protocol) == 0 ?
                            "IP" : s_LastApnTable[m_slot_id][apnIndex].protocol,
            strlen(s_LastApnTable[m_slot_id][apnIndex].roamingProtocol) == 0 ?
                            "IP" : s_LastApnTable[m_slot_id][apnIndex].roamingProtocol,
            RmcDcUtility::getAuthType(s_LastApnTable[m_slot_id][apnIndex].authType),
            s_LastApnTable[m_slot_id][apnIndex].enabled,
            s_LastApnTable[m_slot_id][apnIndex].maxConns,
            s_LastApnTable[m_slot_id][apnIndex].maxConnsTime,
            s_LastApnTable[m_slot_id][apnIndex].waitTime,
            0 == s_LastApnTable[m_slot_id][apnIndex].bearerBitmask ?
                            ALL_BEARER_EXCLUDE_IWLAN_BITMASK : s_LastApnTable[m_slot_id][apnIndex].bearerBitmask,
            s_LastApnTable[m_slot_id][apnIndex].inactiveTimer);
    if (n < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to print Apn params", m_slot_id, __FUNCTION__);
        return false;
    }
    strncat(strParam + strlen(strParam), tmpStrParam, MAX_AT_CMD_LENGTH-strlen(strParam)-1);

    cmd.clear();
    cmd.append(String8::format("AT+EAPNSET=\"%s\",%d,\"%s\"", apn, s_ApnCmdIndex[m_slot_id][apnIndex]+1, strParam));
    p_response = atSendCommandSingleline(cmd, "+EAPNSET:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] 4. fail to get p_response!",
                m_slot_id, __FUNCTION__);
        return false;
    }

    if (p_response->getIntermediates() != NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EAPNSET update other parameters ERROR",
                m_slot_id, __FUNCTION__);
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] %s",
                m_slot_id, __FUNCTION__, p_response->getIntermediates()->getLine());
        return false;
    }
    return true;
}

bool RmcDcCommonReqHandler::isImsTypeApnExist() {
    int supportedTypesBitmask = 0;
    // reverse traversal maybe better because most cases ims apn will be the last or but one.
    for (int i = s_nLastReqNum[m_slot_id] - 1; i >= 0; i--) {
        supportedTypesBitmask = s_LastApnTable[m_slot_id][i].supportedTypesBitmask;
        if ((supportedTypesBitmask != RIL_APN_TYPE_ALL) &&
            (supportedTypesBitmask != RIL_APN_TYPE_MTKALL) &&
            ((supportedTypesBitmask & RIL_APN_TYPE_IMS) == RIL_APN_TYPE_IMS)) {
            return true;
        }
    }
    return false;
}

bool RmcDcCommonReqHandler::syncDefaultImsApn() {
    sp<RfxAtResponse> p_response;
    String8 cmd("");
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] add default IMS apn settings ", m_slot_id, __FUNCTION__);
    // update the user name and password
    // AT+EAPNSET = "apn",cmdIndex,"user","pwd"
    cmd.clear();
    cmd.append(String8::format("AT+EAPNSET=\"ims\",1,\"\",\"\""));
    p_response = atSendCommand(cmd);

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] 1. Set default IMS apn fail to get p_response!",
                m_slot_id, __FUNCTION__);
        return false;
    }

    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EAPNSET update user name and password ERROR",
                m_slot_id, __FUNCTION__);
        return false;
    }

    // update other parameters
    // AT+EAPNSET = "apn",cmdIndex+1,"param1=value1;param2=value2;param3=value3;..."
    cmd.clear();
    cmd.append(String8::format("AT+EAPNSET=\"ims\",2,\"type=ims;protocol=IPV4V6;" \
            "roaming_protocol=IPV4V6;authtype=0;carrier_enabled=1;max_conns=0;" \
            "max_conns_time=0;wait_time=0;bearer_bitmask=%u;inactive_timer=0\"",
            ALL_BEARER_BITMASK));
    p_response = atSendCommandSingleline(cmd, "+EAPNSET:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] 2. Set default IMS apn fail to get p_response!",
                m_slot_id, __FUNCTION__);
        return false;
    }

    if (p_response->getIntermediates() != NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EAPNSET update other parameters ERROR",
                m_slot_id, __FUNCTION__);
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] %s",
                m_slot_id, __FUNCTION__, p_response->getIntermediates()->getLine());
        return false;
    }
    return true;
}

bool RmcDcCommonReqHandler::needReplaceDefaultApnToAttachApn() {
    if (RmcDcUtility::isVerizonOnSiteLteSimCard(m_slot_id) && (mAttachApn != NULL)) {
        return true;
    }
    return false;
}

void RmcDcCommonReqHandler::replaceDefaultApnIfNeeded(RIL_MtkDataProfileInfo &apn) {
    // check default apn need replaced by attach apn
    bool needReplace = (apn.supportedTypesBitmask & RIL_APN_TYPE_DEFAULT) && needReplaceDefaultApnToAttachApn();
    if (needReplace) {
        FREEIF(apn.apn);
        asprintf(&apn.apn, "%s", mAttachApn);
    }
}

void RmcDcCommonReqHandler::onAttachApnChanged(const char* apnName) {
    if (mAttachApn != NULL) {
        free(mAttachApn);
        mAttachApn = NULL;
    }
    if (apnName != NULL) {
        asprintf(&mAttachApn, "%s", apnName);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] attach apn recorded:%s", m_slot_id, __FUNCTION__, apnName);
    }
    if (needReplaceDefaultApnToAttachApn() && s_nLastReqNum[m_slot_id] > 0) {
        // resync apn list with replaced default apn string
        if (!lockAndClearApn()) {
            goto error;
        }
        for (int i = 0; i < s_nLastReqNum[m_slot_id]; i++) {
            // replace default apn string for "On Site LTE UICC"
            replaceDefaultApnIfNeeded(s_LastApnTable[m_slot_id][i]);
            if (!syncApnItem(i)) {
                goto error;
            }
        }
        // Customization requirement, update protocol&roaming protocol of modem's default emergency apn.
        if (RmcDcUtility::isSupportOverrideModemDefaultApn()) {
            updateMdDefaultEmergencyApn();
        }
        //if IMS apn settings does not exists, add one and sync to MD
        if(RmcDcUtility::isSupportDefaultImsApnSettings() && !isImsTypeApnExist()) {
            syncDefaultImsApn();
        }
error:
        unlockApn();
    }
}

void RmcDcCommonReqHandler::requestSyncApnTable(const sp<RfxMclMessage>& msg) {
    int n = 0;
    int rid = m_slot_id;
    sp<RfxMclMessage> response;
    RIL_MtkDataProfileInfo **oldDataProfilePtrs =
                (RIL_MtkDataProfileInfo **)msg->getData()->getData();
    int nOldNum = msg->getData()->getDataLength() / sizeof(RIL_MtkDataProfileInfo*);

    if (nOldNum <= 0) {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    RIL_MtkDataProfileInfo **dataProfilePtrs = (RIL_MtkDataProfileInfo**) calloc(
            nOldNum + MAX_COUNT_EXTRA_APN, sizeof(RIL_MtkDataProfileInfo*));
    RFX_ASSERT(dataProfilePtrs != NULL);
    int nReqNum = createDataProfiles(oldDataProfilePtrs, dataProfilePtrs, nOldNum);

    adjustSupportedBitmaskForRcs(dataProfilePtrs, nReqNum);

    // DataProfile info
    int profileId = 0;
    int authType = 0;
    int type = 0;
    int maxConnsTime = 0;
    int maxConns = 0;
    int waitTime = 0;
    int enabled = 0;
    int supportedTypesBitmask = 0;
    long int bearerBitmask = 0;
    int mtu = 0;
    int inactiveTimer = 0;
    // local ver.
    int cmdIndex = 1;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] num of pararmeters = %d", rid, __FUNCTION__, nReqNum);

    // avoid syncing the same apn table, e.g. sub id ready then record loaded event
    if (s_LastApnTable[m_slot_id] != NULL) {
        bool bSameApnTable = true;
        if (s_nLastReqNum[m_slot_id] == nReqNum) {
            for (int i = 0; i < s_nLastReqNum[m_slot_id]; i++) {
                RIL_MtkDataProfileInfo *preProfile = &s_LastApnTable[m_slot_id][i];
                RIL_MtkDataProfileInfo *curProfile = dataProfilePtrs[i];
                if (preProfile->profileId != curProfile->profileId
                        || 0 != strcmp(preProfile->apn, curProfile->apn)
                        || 0 != strcmp(preProfile->protocol, curProfile->protocol)
                        || 0 != strcmp(preProfile->roamingProtocol, curProfile->roamingProtocol)
                        || preProfile->authType != curProfile->authType
                        || 0 != strcmp(preProfile->user, curProfile->user)
                        || 0 != strcmp(preProfile->password, curProfile->password)
                        || preProfile->maxConnsTime != curProfile->maxConnsTime
                        || preProfile->maxConns != curProfile->maxConns
                        || preProfile->waitTime != curProfile->waitTime
                        || preProfile->enabled != curProfile->enabled
                        || preProfile->supportedTypesBitmask != curProfile->supportedTypesBitmask
                        || preProfile->bearerBitmask != curProfile->bearerBitmask
                        || preProfile->inactiveTimer != curProfile->inactiveTimer) {
                    bSameApnTable = false;
                    break;
                }
            }
        } else {
            bSameApnTable = false;
        }
        if (bSameApnTable) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] ignore same apn table sync",
                    rid, __FUNCTION__);
            response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxVoidData(), msg, false);

            // Make IA the same as used to be.
            // Once the Set_Data_Profile triggered, send the EIAAPN even if EAPNSET is ignored.
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Sending event: RFX_MSG_EVENT_QUERY_SELF_IA",
                    rid, __FUNCTION__);
            sendEvent(RFX_MSG_EVENT_QUERY_SELF_IA, RfxVoidData(),
                    RIL_CMD_PROXY_5, m_slot_id);
            responseToTelCore(response);

            deleteDataProfiles(dataProfilePtrs, nOldNum + MAX_COUNT_EXTRA_APN);
            return;
        } else {
            // reset Last APN table
            resetLastApnTable();
        }
    }

    // init s_nLastReqNum, s_ApnCmdIndex, and s_LastApnTable
    s_nLastReqNum[m_slot_id] = nReqNum;
    s_ApnCmdIndex[m_slot_id] = (int*)calloc(s_nLastReqNum[m_slot_id], sizeof(int));
    RFX_ASSERT(s_ApnCmdIndex[m_slot_id] != NULL);
    s_LastApnTable[m_slot_id] = (RIL_MtkDataProfileInfo*)calloc(s_nLastReqNum[m_slot_id],
            sizeof(RIL_MtkDataProfileInfo));
    RFX_ASSERT(s_LastApnTable[m_slot_id] != NULL);

    if (!lockAndClearApn()) {
        goto error;
    }

    for (int i = 0; i < nReqNum; i++) {
        profileId = dataProfilePtrs[i]->profileId;
        authType = RmcDcUtility::getAuthType(dataProfilePtrs[i]->authType);
        type = dataProfilePtrs[i]->type;
        maxConnsTime = dataProfilePtrs[i]->maxConnsTime;
        maxConns = dataProfilePtrs[i]->maxConns;
        waitTime = dataProfilePtrs[i]->waitTime;
        enabled = dataProfilePtrs[i]->enabled;
        supportedTypesBitmask = dataProfilePtrs[i]->supportedTypesBitmask;
        bearerBitmask = dataProfilePtrs[i]->bearerBitmask;
        mtu = dataProfilePtrs[i]->mtu;
        inactiveTimer = dataProfilePtrs[i]->inactiveTimer;

        // update Last APN table
        s_LastApnTable[m_slot_id][i].profileId = profileId;
        asprintf(&s_LastApnTable[m_slot_id][i].apn, "%s", dataProfilePtrs[i]->apn);
        asprintf(&s_LastApnTable[m_slot_id][i].protocol, "%s", dataProfilePtrs[i]->protocol);
        asprintf(&s_LastApnTable[m_slot_id][i].roamingProtocol, "%s",
                dataProfilePtrs[i]->roamingProtocol);
        s_LastApnTable[m_slot_id][i].authType = dataProfilePtrs[i]->authType;  //backup raw data
        asprintf(&s_LastApnTable[m_slot_id][i].user, "%s", dataProfilePtrs[i]->user);
        asprintf(&s_LastApnTable[m_slot_id][i].password, "%s", dataProfilePtrs[i]->password);
        s_LastApnTable[m_slot_id][i].type = type;
        s_LastApnTable[m_slot_id][i].maxConnsTime = maxConnsTime;
        s_LastApnTable[m_slot_id][i].maxConns = maxConns;
        s_LastApnTable[m_slot_id][i].waitTime = waitTime;
        s_LastApnTable[m_slot_id][i].enabled = enabled;
        s_LastApnTable[m_slot_id][i].supportedTypesBitmask = supportedTypesBitmask;
        s_LastApnTable[m_slot_id][i].bearerBitmask = bearerBitmask;
        s_LastApnTable[m_slot_id][i].mtu = mtu;
        asprintf(&s_LastApnTable[m_slot_id][i].mvnoType, "%s", dataProfilePtrs[i]->mvnoType);
        asprintf(&s_LastApnTable[m_slot_id][i].mvnoMatchData, "%s",
                dataProfilePtrs[i]->mvnoMatchData);
        s_LastApnTable[m_slot_id][i].inactiveTimer = inactiveTimer;

        cmdIndex = 1;

        for (int j = 0; j < i; j++) {
            if (0 == strcasecmp(dataProfilePtrs[i]->apn, dataProfilePtrs[j]->apn)) {
                cmdIndex += CMD_INDEX_OFFSET;
            }
        }
        s_ApnCmdIndex[m_slot_id][i] = cmdIndex;

        // replace default apn string for "On Site LTE UICC"
        replaceDefaultApnIfNeeded(s_LastApnTable[m_slot_id][i]);
        if (!syncApnItem(i)) {
            goto error;
        }
    }

    // Customization requirement, update protocol&roaming protocol of modem's default emergency apn.
    if (RmcDcUtility::isSupportOverrideModemDefaultApn()) {
        updateMdDefaultEmergencyApn();
    }
    //if IMS apn settings does not exists, add one and sync to MD
    if(RmcDcUtility::isSupportDefaultImsApnSettings() && !isImsTypeApnExist()) {
        if (!syncDefaultImsApn()) {
            goto error;
        }
    }

    if (!unlockApn()) {
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] Sending event: RFX_MSG_EVENT_QUERY_SELF_IA",
            rid, __FUNCTION__);
    sendEvent(RFX_MSG_EVENT_QUERY_SELF_IA, RfxVoidData(),
            RIL_CMD_PROXY_5, m_slot_id);

    responseToTelCore(response);

    deleteDataProfiles(dataProfilePtrs, nOldNum + MAX_COUNT_EXTRA_APN);
    return;

error:
    // reset Last APN table
    resetLastApnTable();

    // unlock the apn table
    unlockApn();
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);

    deleteDataProfiles(dataProfilePtrs, nOldNum + MAX_COUNT_EXTRA_APN);
}

void RmcDcCommonReqHandler::adjustSupportedBitmaskForRcs(
        RIL_MtkDataProfileInfo **dataProfiles, int dpCount) {
    if (!dataProfiles || dpCount <= 0)
        return;

    int idxRcs = -1;
    int idxDefault = -1;

    for (int i = 0; i < dpCount; i++) {
        if (dataProfiles[i]->supportedTypesBitmask == RIL_APN_TYPE_ALL
                || dataProfiles[i]->supportedTypesBitmask == RIL_APN_TYPE_MTKALL)
            continue;
        if ((dataProfiles[i]->supportedTypesBitmask & RIL_APN_TYPE_RCS)
                && !(dataProfiles[i]->supportedTypesBitmask & RIL_APN_TYPE_DEFAULT)
                && idxRcs < 0)
            idxRcs = i;
        if (!(dataProfiles[i]->supportedTypesBitmask & RIL_APN_TYPE_RCS)
                && (dataProfiles[i]->supportedTypesBitmask & RIL_APN_TYPE_DEFAULT)
                && idxDefault < 0)
            idxDefault = i;

        if (idxRcs >= 0 && idxDefault >= 0) {
            RFX_LOG_I(RFX_LOG_TAG, "Adjust for rcs APN needed:rcs[%d],default[%d]",
                idxRcs, idxDefault);

            dataProfiles[idxRcs]->supportedTypesBitmask &= ~RIL_APN_TYPE_RCS;
            dataProfiles[idxDefault]->supportedTypesBitmask |= RIL_APN_TYPE_RCS;
            break;
        }

    }
}

void RmcDcCommonReqHandler::requestSyncDataSettingsToMd(const sp<RfxMclMessage>& msg) {
    int rid = m_slot_id;
    int *pReqData = (int *)msg->getData()->getData();
    int reqDataNum = msg->getData()->getDataLength() / sizeof(int);
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_Errno ret = RIL_E_SUCCESS;

    // s_dataSetting[m_slot_id][0 (MOBILE_DATA)]:      data setting on/off.
    // s_dataSetting[m_slot_id][1 (ROAMING_DATA)]:     data roaming setting on/off.
    // s_dataSetting[m_slot_id][2 (DEFAULT_DATA_SIM)]: default data SIM setting.
    // s_dataSetting[m_slot_id][3 (DOMESTIC_ROAMING_DATA)]: domestic roaming setting.
    // s_dataSetting[m_slot_id][4 (INTERNATIONAL_ROAMING_DATA)]: international roaming setting.
    for (int i = 0; i < reqDataNum; i++) {
        s_dataSetting[m_slot_id][i] = pReqData[i];
        if (pReqData[i] != SKIP_DATA_SETTINGS) {
            s_dataSetting_resend[m_slot_id][i] = pReqData[i];
        }
    }

    // 0: clear
    // 1: slot 0
    // 2: slot 1
    // Modem only receive the slot ID which start from 1.
    // And 0 is treated as "UNSET" to reset.
    int setDefaultSim = s_dataSetting[m_slot_id][DEFAULT_DATA_SIM] + 1;

    RFX_LOG_D(RFX_LOG_TAG,
            "[%d][%s] Start, dataOn:%d, dataRoaming:%d, defaultDataSim: %d (original: %d),"
            "domestic roaming: %d, international raoming: %d",
            rid,
            __FUNCTION__,
            s_dataSetting[m_slot_id][MOBILE_DATA],
            s_dataSetting[m_slot_id][ROAMING_DATA],
            setDefaultSim,
            s_dataSetting[m_slot_id][DEFAULT_DATA_SIM],
            s_dataSetting[m_slot_id][DOMESTIC_ROAMING_DATA],
            s_dataSetting[m_slot_id][INTERNATIONAL_ROAMING_DATA]);

    if (s_dataSetting[m_slot_id][DEFAULT_DATA_SIM] != SKIP_DATA_SETTINGS
        && (s_dataSetting[m_slot_id][DEFAULT_DATA_SIM] <= MAX_RFX_SLOT_ID
            || s_dataSetting[m_slot_id][DEFAULT_DATA_SIM] >= RFX_SLOT_ID_0)) {

        p_response = atSendCommand(String8::format("AT+EDALLOW=2,%d",
            setDefaultSim));

        if (p_response == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] 1. fail to get p_response!",
                    rid, __FUNCTION__);
            ret = RIL_E_GENERIC_FAILURE;
        } else if (p_response->isATCmdRspErr()) {
            // If sync default data SIM to MD meet error, then keep sync data settings to MD.
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EDALLOW command returns ERROR",
                    rid, __FUNCTION__);
            ret = RIL_E_GENERIC_FAILURE;
        }
    }

    if (s_dataSetting[m_slot_id][MOBILE_DATA] == SKIP_DATA_SETTINGS) {
        response = RfxMclMessage::obtainResponse(msg->getId(), ret, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    /**
     * To get IMS test mode setting and sync to modem for OP12
     *    Enable : Set Attach PDN to VZWINTERNET
     *    Disable: Set Attach PDN to VZWIMS (Default)
     */
    char value[MTK_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.imstestmode", value, "0");
    int imsTestMode = atoi(value);

    // AT+ECNCFG=<mobile_data>[,[<data_roaming>],[<volte>],<ims_test_mode>
    // [,<domestic_data_roaming>,<international_data_roaming>]]
    if (s_dataSetting[m_slot_id][DOMESTIC_ROAMING_DATA] == SKIP_DATA_SETTINGS &&
            s_dataSetting[m_slot_id][INTERNATIONAL_ROAMING_DATA] == SKIP_DATA_SETTINGS) {
        if (s_dataSetting[m_slot_id][ROAMING_DATA] == SKIP_DATA_SETTINGS) {
            p_response = atSendCommand(String8::format("AT+ECNCFG=%d,,,%d",
                    s_dataSetting[m_slot_id][MOBILE_DATA],
                    imsTestMode));
        } else {
            p_response = atSendCommand(String8::format("AT+ECNCFG=%d,%d,,%d",
                    s_dataSetting[m_slot_id][MOBILE_DATA],
                    s_dataSetting[m_slot_id][ROAMING_DATA],
                    imsTestMode));
        }
    } else if (s_dataSetting[m_slot_id][DOMESTIC_ROAMING_DATA] != SKIP_DATA_SETTINGS &&
            s_dataSetting[m_slot_id][INTERNATIONAL_ROAMING_DATA] != SKIP_DATA_SETTINGS) {
        p_response = atSendCommand(String8::format("AT+ECNCFG=%d,,,%d,%d,%d",
                s_dataSetting[m_slot_id][MOBILE_DATA],
                imsTestMode,
                s_dataSetting[m_slot_id][DOMESTIC_ROAMING_DATA],
                s_dataSetting[m_slot_id][INTERNATIONAL_ROAMING_DATA]));
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] invalid domestic/international roaming value",
                rid, __FUNCTION__);
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    if ((p_response == NULL) || (p_response != NULL && p_response->isATCmdRspErr())) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+ECNCFG command returns ERROR or p_response is null",
                rid, __FUNCTION__);
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxVoidData(), msg, false);
    }

    responseToTelCore(response);
}

void RmcDcCommonReqHandler::requestResendSyncDataSettingsToMd(const sp<RfxMclMessage>& msg) {
    int rid = m_slot_id;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Start, dataOn:%d",
            rid,
            __FUNCTION__,
            s_dataSetting_resend[m_slot_id][MOBILE_DATA]);

    if (s_dataSetting_resend[m_slot_id][MOBILE_DATA] == SKIP_DATA_SETTINGS) {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    char value[MTK_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.imstestmode", value, "0");
    int imsTestMode = atoi(value);

    if (s_dataSetting_resend[m_slot_id][DOMESTIC_ROAMING_DATA] == SKIP_DATA_SETTINGS &&
            s_dataSetting_resend[m_slot_id][INTERNATIONAL_ROAMING_DATA] == SKIP_DATA_SETTINGS) {
        if (s_dataSetting_resend[m_slot_id][ROAMING_DATA] == SKIP_DATA_SETTINGS) {
            p_response = atSendCommand(String8::format("AT+ECNCFG=%d,,,%d",
                    s_dataSetting_resend[m_slot_id][MOBILE_DATA],
                    imsTestMode));
        } else {
            p_response = atSendCommand(String8::format("AT+ECNCFG=%d,%d,,%d",
                    s_dataSetting_resend[m_slot_id][MOBILE_DATA],
                    s_dataSetting_resend[m_slot_id][ROAMING_DATA],
                    imsTestMode));
        }
    } else if (s_dataSetting_resend[m_slot_id][DOMESTIC_ROAMING_DATA] != SKIP_DATA_SETTINGS &&
            s_dataSetting_resend[m_slot_id][INTERNATIONAL_ROAMING_DATA] != SKIP_DATA_SETTINGS) {
        p_response = atSendCommand(String8::format("AT+ECNCFG=%d,,,%d,%d,%d",
                s_dataSetting_resend[m_slot_id][MOBILE_DATA],
                imsTestMode,
                s_dataSetting_resend[m_slot_id][DOMESTIC_ROAMING_DATA],
                s_dataSetting_resend[m_slot_id][INTERNATIONAL_ROAMING_DATA]));
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] invalid domestic/international roaming value",
                rid, __FUNCTION__);
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    if ((p_response == NULL) || (p_response != NULL && p_response->isATCmdRspErr())) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+ECNCFG command returns ERROR or p_response is null",
                rid, __FUNCTION__);
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxVoidData(), msg, false);
    }

    responseToTelCore(response);
}

void RmcDcCommonReqHandler::requestResetMdDataRetryCount(const sp<RfxMclMessage>& msg) {
    int rid = m_slot_id;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    const char *apnName = (const char *)msg->getData()->getData();
    char apn[MAX_APN_NAME_LENGTH] = {0};

    // Debug only
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Reset MD data count for APN: %s", rid, __FUNCTION__, apnName);

    // AT+EDRETRY=<mode><APN name>
    // <mode>:indicate it's query mode or reset mode. 0:query mode, 1:reset mode
    // <APN name>: APN (string type) which is use to query or reset bar timer and retry count.
    if (apnName != NULL && strlen(apnName) > 0) {
        strncpy(apn, apnName, MAX_APN_NAME_LENGTH-1);
        apn[MAX_APN_NAME_LENGTH-1] = '\0';
        addEscapeSequence(apn);
    }
    p_response = atSendCommand(String8::format("AT+EDRETRY=1,\"%s\"", apn));

    if ((p_response == NULL) || (p_response != NULL && p_response->isATCmdRspErr())) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EDRETRY command returns ERROR or p_response is null",
                rid, __FUNCTION__);
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxVoidData(), msg, false);
    }

    responseToTelCore(response);
}

int RmcDcCommonReqHandler::convertDeactCauseToMdStandard(int reason) {
    switch (reason) {
        case DEACT_REASON_AOSP_SHUTDOWN: return ATCMD_DEACT_CAUSE_EGACT_CAUSE_AOSP_SHUTDOWN;
        case DEACT_REASON_AOSP_HANDOVER: return ATCMD_DEACT_CAUSE_EGACT_CAUSE_AOSP_HANDOVER;
        case DEACT_REASON_APN_CHANGED: return ATCMD_DEACT_CAUSE_EGACT_CAUSE_APN_CHANGED;
        case DEACT_REASON_TEMP_DATA_SWITCHING:
            return ATCMD_DEACT_CAUSE_EGACT_CAUSE_TEMP_DATA_SWITCHING;
        default: return ATCMD_DEACT_CAUSE_EGACT_CAUSE_DEACT_NORMAL;
    }
}

bool RmcDcCommonReqHandler::isLocalFailCase(int cause) {
    if ((cause > SM_CAUSE_BASE && cause < SM_CAUSE_END)
        || (cause > ESM_CAUSE_BASE && cause < ESM_CAUSE_END)
        || (cause > PAM_CAUSE_BASE && cause < PAM_CAUSE_END)
        || (cause > CME_CAUSE_BASE_V14 && cause < CME_CAUSE_END_V14)) {
        return false;
    } else {
        return true;
    }
}

void RmcDcCommonReqHandler::updateLastFailCause(int cause) {
    m_nGprsFailureCause = RmcDcUtility::convertFailCauseToRilStandard(cause, m_slot_id);
}

int RmcDcCommonReqHandler::getLastFailCause() {
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] CAUSE: %X", m_slot_id, __FUNCTION__, m_nGprsFailureCause);
    return m_nGprsFailureCause;
}

int RmcDcCommonReqHandler::isAllAidActive(Vector<int> *vAidList) {
    int rid = m_slot_id;

    updateActiveStatus();

    for (Vector<int>::iterator it = vAidList->begin(); it != vAidList->end(); it++) {
        int aid = *it;
        RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] pdn_info[%d], state: %d",
                rid, __FUNCTION__, aid, getPdnActiveStatus(aid));
        if (DATA_STATE_INACTIVE == getPdnActiveStatus(aid)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid%d is inactive", rid, __FUNCTION__, aid);
            return 0;
        }
    }

    return 1;
}

void RmcDcCommonReqHandler::requestOrSendDataCallList(const sp<RfxMclMessage>& msg) {
    int rid = m_slot_id;
    MTK_RIL_Data_Call_Response_v11* responsesOfActive = NULL;
    Vector<int> vTransIntfIdList;
    Vector<int>::iterator it;
    sp<RfxMclMessage> responseMsg;
    const int nPdnInfoSize = getPdnTableSize();

    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] PdnTableSize=%d", rid, __FUNCTION__, nPdnInfoSize);

    // Reserve size of TransIntfId list.
    vTransIntfIdList.reserve(nPdnInfoSize);

    // Update all PDN active status.
    updateActiveStatus();

    // Update all PDN ip address.
    updatePdnAddress();

    // Update all PDN default bearer information.
    if (PDN_SUCCESS != updateDefaultBearerInfo()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateDefaultBearerInfo failed", rid, __FUNCTION__);
        goto error;
    }

    // update Traffic descriptor info for 5G only
    updateTdInfo();

    for (int i = 0; i < nPdnInfoSize; i++) {
        if ((getPdnActiveStatus(i) == DATA_STATE_ACTIVE) &&
                (getTransIntfId(i) != INVALID_TRANS_INTF_ID) &&
                (std::find(vTransIntfIdList.begin(), vTransIntfIdList.end(),
                getTransIntfId(i)) == vTransIntfIdList.end())) {
                vTransIntfIdList.push_back(getTransIntfId(i));
        }
    }

    if (vTransIntfIdList.size() > 0) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] active trans-interface number is %zu",
                rid, __FUNCTION__, vTransIntfIdList.size());

        responsesOfActive = (MTK_RIL_Data_Call_Response_v11*)
                calloc(1, vTransIntfIdList.size() * sizeof(MTK_RIL_Data_Call_Response_v11));

        if (responsesOfActive == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to allocate responsesOfActive!",
                    rid, __FUNCTION__);
            RFX_ASSERT(false);
        }

        initDataCallResponse(responsesOfActive, vTransIntfIdList.size());
        int index = 0;
        for (it = vTransIntfIdList.begin(); it != vTransIntfIdList.end(); it++) {
            int transIntfId = *it;
            createDataResponse(transIntfId, IPV4V6, &responsesOfActive[index]);
            ++index;
        }
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] %s", rid, __FUNCTION__,
                responsesToString(responsesOfActive, vTransIntfIdList.size()).string());

        if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                    RfxDataCallResponseData(responsesOfActive, vTransIntfIdList.size()), msg);
        } else {
            responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid,
                    RfxDataCallResponseData(responsesOfActive, vTransIntfIdList.size()));
        }
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no active response", rid, __FUNCTION__);

        if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
        } else {
            responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid,
                    RfxVoidData());
        }
    }

    responseToTelCore(responseMsg);

    if (responsesOfActive != NULL) {
        int size = vTransIntfIdList.size();
        for (int i = 0; i < size; i++) {
            freeDataResponse(&responsesOfActive[i]);
        }
        FREEIF(responsesOfActive);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] free responsesOfActive", rid, __FUNCTION__);
    }

    return;

error:
    if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE, RfxVoidData(), msg);
    } else {
        responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid, RfxVoidData());
    }
    responseToTelCore(responseMsg);
}

void RmcDcCommonReqHandler::requestOrSendDataCallList(const sp<RfxMclMessage>& msg, Vector<int> *vAidList) {
    int rid = m_slot_id;
    MTK_RIL_Data_Call_Response_v11* responsesOfActive = NULL;
    Vector<int> vTransIntfIdList;
    Vector<int>::iterator it;
    sp<RfxMclMessage> responseMsg;
    const int nPdnInfoSize = getPdnTableSize();

    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] PdnTableSize=%d", rid, __FUNCTION__, nPdnInfoSize);

    // Reserve size of TransIntfId list.
    vTransIntfIdList.reserve(nPdnInfoSize);

    updateActiveStatus();

    for (it = vAidList->begin(); it != vAidList->end(); it++) {
        int aid = *it;
        if (PDN_SUCCESS != updatePdnAddress(aid)) {
            // Not to check error since we expect to query all PDN connections even if error occurred
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] updatePdnAddress failed for aid%d",
                    rid, __FUNCTION__, aid);
        }

        if (PDN_SUCCESS != updateDefaultBearerInfo(aid)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateDefaultBearerInfo failed for aid%d",
                    rid, __FUNCTION__, aid);
            goto error;
        }
        updateQosInformation(aid, m_pPdnManager->isDedicateBearer(aid));
        updateTdInfo(aid);
    }

    for (int i = 0; i < nPdnInfoSize; i++) {
        if ((getPdnActiveStatus(i) == DATA_STATE_ACTIVE) &&
                (getTransIntfId(i) != INVALID_TRANS_INTF_ID) &&
                (std::find(vTransIntfIdList.begin(), vTransIntfIdList.end(),
                getTransIntfId(i)) == vTransIntfIdList.end())) {
                vTransIntfIdList.push_back(getTransIntfId(i));
        }
    }

    if (vTransIntfIdList.size() > 0) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] active trans-interface number is %zu",
                rid, __FUNCTION__, vTransIntfIdList.size());

        responsesOfActive = (MTK_RIL_Data_Call_Response_v11*)
                calloc(1, vTransIntfIdList.size() * sizeof(MTK_RIL_Data_Call_Response_v11));

        if (responsesOfActive == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to allocate responsesOfActive!",
                    rid, __FUNCTION__);
            RFX_ASSERT(false);
        }

        initDataCallResponse(responsesOfActive, vTransIntfIdList.size());
        int index = 0;
        for (it = vTransIntfIdList.begin(); it != vTransIntfIdList.end(); it++) {
            int transIntfId = *it;
            createDataResponse(transIntfId, IPV4V6, &responsesOfActive[index]);
            ++index;
        }
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] %s", rid, __FUNCTION__,
                responsesToString(responsesOfActive, vTransIntfIdList.size()).string());

        if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                    RfxDataCallResponseData(responsesOfActive, vTransIntfIdList.size()), msg);
        } else {
            responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid,
                    RfxDataCallResponseData(responsesOfActive, vTransIntfIdList.size()));
        }
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no active response", rid, __FUNCTION__);

        if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
        } else {
            responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid,
                    RfxVoidData());
        }
    }

    responseToTelCore(responseMsg);

    if (responsesOfActive != NULL) {
        int size = vTransIntfIdList.size();
        for (int i = 0; i < size; i++) {
            freeDataResponse(&responsesOfActive[i]);
        }
        FREEIF(responsesOfActive);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] free responsesOfActive", rid, __FUNCTION__);
    }

    return;

error:
    if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE, RfxVoidData(), msg);
    } else {
        responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid, RfxVoidData());
    }
    responseToTelCore(responseMsg);
}

void RmcDcCommonReqHandler::requestOrSendDataCallList(const sp<RfxMclMessage>& msg, int aid) {
    int rid = m_slot_id;
    MTK_RIL_Data_Call_Response_v11* responsesOfActive = NULL;
    Vector<int> vTransIntfIdList;
    Vector<int>::iterator it;
    sp<RfxMclMessage> responseMsg;
    const int nPdnInfoSize = getPdnTableSize();

    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] PdnTableSize=%d", rid, __FUNCTION__, nPdnInfoSize);

    if (PDN_SUCCESS != updatePdnAddress(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updatePdnAddress failed for aid%d",
                rid, __FUNCTION__, aid);
        return;
    }

    if (PDN_SUCCESS != updateDefaultBearerInfo(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateDefaultBearerInfo failed for aid%d",
                rid, __FUNCTION__, aid);
        return;
    }

    updateActiveStatus();
    updateQosInformation(aid, m_pPdnManager->isDedicateBearer(aid));
    updateTdInfo(aid);

    // Reserve size of TransIntfId list.
    vTransIntfIdList.reserve(nPdnInfoSize);

    for (int i = 0; i < nPdnInfoSize; i++) {
        if ((getPdnActiveStatus(i) == DATA_STATE_ACTIVE) &&
                (getTransIntfId(i) != INVALID_TRANS_INTF_ID) &&
                (std::find(vTransIntfIdList.begin(), vTransIntfIdList.end(),
                getTransIntfId(i)) == vTransIntfIdList.end())) {
                vTransIntfIdList.push_back(getTransIntfId(i));
        }
        if (isDedicateBearer(i) && getPrimaryAid(i) == aid) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] dedicate bearer,[aid=%d, active=%d]",
                rid, __FUNCTION__, i, getPdnActiveStatus(i));
            updateQosInformation(i, true);
        }
    }

    if (vTransIntfIdList.size() > 0) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] active trans-interface number is %zu",
                rid, __FUNCTION__, vTransIntfIdList.size());

        responsesOfActive = (MTK_RIL_Data_Call_Response_v11*)
                calloc(1, vTransIntfIdList.size() * sizeof(MTK_RIL_Data_Call_Response_v11));

        if (responsesOfActive == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to allocate responsesOfActive!",
                    rid, __FUNCTION__);
            RFX_ASSERT(false);
        }

        initDataCallResponse(responsesOfActive, vTransIntfIdList.size());
        int index = 0;
        for (it = vTransIntfIdList.begin(); it != vTransIntfIdList.end(); it++) {
            int transIntfId = *it;
            createDataResponse(transIntfId, IPV4V6, &responsesOfActive[index]);
            ++index;
        }
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] %s", rid, __FUNCTION__,
                responsesToString(responsesOfActive, vTransIntfIdList.size()).string());

        if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                    RfxDataCallResponseData(responsesOfActive, vTransIntfIdList.size()), msg);
        } else {
            responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid,
                    RfxDataCallResponseData(responsesOfActive, vTransIntfIdList.size()));
        }
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no active response", rid, __FUNCTION__);

        if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
        } else {
            responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid,
                    RfxVoidData());
        }
    }

    responseToTelCore(responseMsg);

    if (responsesOfActive != NULL) {
        int size = vTransIntfIdList.size();
        for (int i = 0; i < size; i++) {
            freeDataResponse(&responsesOfActive[i]);
        }
        FREEIF(responsesOfActive);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] free responsesOfActive", rid, __FUNCTION__);
    }
}

void RmcDcCommonReqHandler::sendDataCallListResponse(const sp<RfxMclMessage>& msg, int deactivatedAid,
        int sscMode, bool deprecated) {
    RFX_UNUSED(sscMode);
    int rid = m_slot_id;
    MTK_RIL_Data_Call_Response_v11* responsesOfActive = NULL;
    Vector<int> vTransIntfIdList;
    Vector<int>::iterator it;
    sp<RfxMclMessage> responseMsg;
    const int nPdnInfoSize = getPdnTableSize();
    MTK_RIL_NetlinkIpv6Info *netInfo = deprecated ? (MTK_RIL_NetlinkIpv6Info*)msg->getData()->getData() : NULL;

    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] PdnTableSize=%d", rid, __FUNCTION__, nPdnInfoSize);

    // Reserve size of TransIntfId list.
    vTransIntfIdList.reserve(nPdnInfoSize);

    for (int i = 0; i < nPdnInfoSize; i++) {
        if ((getPdnActiveStatus(i) == DATA_STATE_ACTIVE) &&
                (getTransIntfId(i) != INVALID_TRANS_INTF_ID) &&
                (std::find(vTransIntfIdList.begin(), vTransIntfIdList.end(),
                getTransIntfId(i)) == vTransIntfIdList.end())) {
            vTransIntfIdList.push_back(getTransIntfId(i));
        }
    }

    if (vTransIntfIdList.size() > 0) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] active trans-interface number is %zu",
                rid, __FUNCTION__, vTransIntfIdList.size());

        responsesOfActive = (MTK_RIL_Data_Call_Response_v11*)
                calloc(1, vTransIntfIdList.size() * sizeof(MTK_RIL_Data_Call_Response_v11));

        if (responsesOfActive == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to allocate responsesOfActive!",
                    rid, __FUNCTION__);
            RFX_ASSERT(false);
        }

        initDataCallResponse(responsesOfActive, vTransIntfIdList.size());
        int index = 0;
        for (it = vTransIntfIdList.begin(); it != vTransIntfIdList.end(); it++) {
            int transIntfId = *it;
            createDataResponse(transIntfId, IPV4V6, &responsesOfActive[index], netInfo);

            if (deactivatedAid != INVALID_AID &&
                    notifyDeactReasonIfNeeded(deactivatedAid) &&
                    transIntfId == getTransIntfId(deactivatedAid)) {
                responsesOfActive[index].active = DATA_STATE_INACTIVE;
                responsesOfActive[index].status = getDeactReason(deactivatedAid);
            }

            ++index;
        }
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] %s", rid, __FUNCTION__,
                responsesToString(responsesOfActive, vTransIntfIdList.size()).string());

        if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                    RfxDataCallResponseData(responsesOfActive, vTransIntfIdList.size()), msg);
        } else {
            responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid,
                    RfxDataCallResponseData(responsesOfActive, vTransIntfIdList.size()));
        }
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no active response", rid, __FUNCTION__);

        if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
        } else {
            responseMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED, rid,
                    RfxVoidData());
        }
    }

    responseToTelCore(responseMsg);

    if (responsesOfActive != NULL) {
        int size = vTransIntfIdList.size();
        for (int i = 0; i < size; i++) {
            freeDataResponse(&responsesOfActive[i]);
        }
        FREEIF(responsesOfActive);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] free responsesOfActive", rid, __FUNCTION__);
    }
}

void RmcDcCommonReqHandler::updateActiveStatus() {
    sp<RfxAtResponse> p_response;
    RfxAtLine *p_cur;
    int maxAidSupported = 0;
    int rid = m_slot_id;
    int err = 0;
    std::stringstream activeAidString;
    std::stringstream inactiveAidString;

    p_response = atSendCommandMultiline("AT+CGACT?", "+CGACT:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return;
    }

    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+CGACT? response ERROR", rid, __FUNCTION__);
        return;
    }

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        maxAidSupported++;
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] PDP capability [%d], PdnTableSize [%d]",
            rid, __FUNCTION__, maxAidSupported, getPdnTableSize());

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        int responseAid = 0;
        int active = 0;

        p_cur->atTokStart(&err);
        if (err < 0)
            break;

        responseAid = p_cur->atTokNextint(&err);
        if (err < 0)
            break;

        active = p_cur->atTokNextint(&err);
        if (err < 0) break;

        if (active > 0) {
            // 0=inactive, 1=active/physical link down, 2=active/physical link up
            if (getTransIntfId(responseAid) == INVALID_TRANS_INTF_ID) {
                updatePdnActiveStatus(responseAid, DATA_STATE_LINKDOWN);
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] aid%d is linkdown", rid, __FUNCTION__, responseAid);
            } else {
                updatePdnActiveStatus(responseAid, DATA_STATE_ACTIVE);
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] aid%d is active", rid, __FUNCTION__, responseAid);
            }

            if (getAid(responseAid) == INVALID_AID) {
                setAidAndPrimaryAid(responseAid, responseAid, responseAid);
            }
            activeAidString << responseAid << ":" << getPrimaryAid(responseAid) << ", ";
        } else {
            updatePdnActiveStatus(responseAid, DATA_STATE_INACTIVE);
            inactiveAidString << responseAid << ":" << getPrimaryAid(responseAid) << ", ";
        }
    }
    if (activeAidString.str().length() > 0) {
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] active list [aid:primaryAid] = [%s]",
                rid, __FUNCTION__, activeAidString.str().c_str());
    }
    if (inactiveAidString.str().length() > 0) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] inactive list [aid:primaryAid] = [%s]",
                rid, __FUNCTION__, inactiveAidString.str().c_str());
    }
}

int RmcDcCommonReqHandler::updateDefaultBearerInfo() {
    //Be aware that this method is used only for retrieving P-CSCF address and signaling flag
    int nRet = PDN_FAILED;
    sp<RfxAtResponse> p_response;
    RfxAtLine *p_cur;
    int count = 0;
    int *v4Count = NULL;
    int *v6Count = NULL;
    int *pcscfCount = NULL;
    int rid = m_slot_id;
    PdnInfo pdn_info;
    memset(&pdn_info, 0, sizeof(pdn_info));
    int err = 0;
    char* out = NULL;
    const int nPdnInfoSize = getPdnTableSize();
    String8 cmd("");

    for (int i = 0; i < nPdnInfoSize; i++) {
        //clear default bearer information in AID table since we would query it again
        pdn_info = getPdnInfo(i);
        memset(pdn_info.pcscf, 0, sizeof(pdn_info.pcscf));
        memset(pdn_info.dnsV4, 0, sizeof(pdn_info.dnsV4));
        memset(pdn_info.dnsV6, 0, sizeof(pdn_info.dnsV6));
        pdn_info.signalingFlag = 0;
        pdn_info.bearerId = INVALID_AID;
        setPdnInfo(i, &pdn_info);
    }

    /*+CGCONTRDP response a list of:
      [+CGCONTRDP: <cid>,<bearer_id>,<apn>[,<local_addr and subnet_mask>[,<gw_addr>[,<DNS_prim_addr>
      [,<DNS_sec_addr>[,<P-CSCF_prim_addr>[,<P-CSCF_sec_addr>[,<IM_CN_Signalling_Flag>
      [,<LIPA_indication>[,<IPv4_MTU>[,<WLAN_Offload>[,<Local_Addr_Ind>[,<Non-IP_MTU>
      [,<Serving_PLMN_rate_control_value>[,<Reliable_Data_Service>[,<PS_Data_Off_Support>
      [,<PDU_session_id>,<QFI>[,<SSC_mode>[,<S-NSSAI>[,<Access_type>
      [,<RQ_timer>[,<Always-on_ind>]]]]]]]]]]]]]]]]]]]]]]
    */
    p_response = atSendCommandMultiline("AT+CGCONTRDP", "+CGCONTRDP:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", rid, __FUNCTION__);
        } else {
            int cause = p_response->atGetCmeError();
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+CGCONTRDP returns cme cause : %d",
                    rid, __FUNCTION__, cause);
        }
        return PDN_FAILED;
    }

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        ++count;
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] response count [%d]", rid, __FUNCTION__, count);

    if (count == 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] response success but no intermediate", rid, __FUNCTION__);
        return PDN_SUCCESS;
    }

    v4Count = (int*)calloc(1, nPdnInfoSize * sizeof(int));
    RFX_ASSERT(v4Count != NULL);
    v6Count = (int*)calloc(1, nPdnInfoSize * sizeof(int));
    RFX_ASSERT(v6Count != NULL);
    pcscfCount = (int*)calloc(1, nPdnInfoSize * sizeof(int));
    RFX_ASSERT(pcscfCount != NULL);

    memset(v4Count, 0, nPdnInfoSize * sizeof(int));
    memset(v6Count, 0, nPdnInfoSize * sizeof(int));
    memset(pcscfCount, 0, nPdnInfoSize * sizeof(int));

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        cmd.clear();
        p_cur->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                    rid, __FUNCTION__);
            goto error;
        }

        int responseAid = 0;
        int bearerId = 0;
        char* apn = NULL;

        responseAid = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                    rid, __FUNCTION__);
            goto error;
        }

        bearerId = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing bearer id",
                    rid, __FUNCTION__);
        } else {
            updateBearerId(responseAid, bearerId);
        }

        apn = p_cur->atTokNextstr(&err); //apn
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing apn",
                    rid, __FUNCTION__);
            goto error;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //local_addr and subnet_mask
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing local addr and subnet mask",
                        rid, __FUNCTION__);
                goto error;
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //gw_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing gw addr",
                        rid, __FUNCTION__);
                goto error;
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //DNS_prim_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing DNS prim addr",
                        rid, __FUNCTION__);
                goto error;
            }

            if (out != NULL && strlen(out) > 0 && strcmp(NULL_IPV4_ADDRESS, out) != 0) {
                int index = 0;
                if (RmcDcUtility::getAddressType(out) == IPV6) {
                    char dnsV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
                    convertIpv6Address(dnsV6, out, 0);
                    index = v6Count[responseAid]++;
                    updateIpv6Dns(responseAid, index, dnsV6);
                    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] DNS%d V6 for aid%d is %s",
                            rid, __FUNCTION__, v6Count[responseAid], responseAid,
                            getIpv6Dns(responseAid, index));
                } else if (RmcDcUtility::getAddressType(out) == IPV4) {
                    index = v4Count[responseAid]++;
                    updateIpv4Dns(responseAid, index, out);
                    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] DNS%d V4 for aid%d is %s",
                            rid, __FUNCTION__, v4Count[responseAid], responseAid,
                            getIpv4Dns(responseAid, index));
                } else {
                   RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] DNS for aid%d is NOT V4 or V6 type",
                           rid, __FUNCTION__, responseAid);
                   goto error;
                }
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //DNS_sec_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing DNS sec addr",
                        rid, __FUNCTION__);
                goto error;
            }

            if (out != NULL && strlen(out) > 0 && strcmp(NULL_IPV4_ADDRESS, out) != 0) {
                int index = 0;
                if (RmcDcUtility::getAddressType(out) == IPV6) {
                    char dnsV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
                    convertIpv6Address(dnsV6, out, 0);
                    index = v6Count[responseAid]++;
                    updateIpv6Dns(responseAid, index, dnsV6);
                    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] DNS%d V6 for aid%d is %s",
                            rid, __FUNCTION__, v6Count[responseAid], responseAid,
                            getIpv6Dns(responseAid, index));
                } else if (RmcDcUtility::getAddressType(out) == IPV4) {
                    index = v4Count[responseAid]++;
                    updateIpv4Dns(responseAid, index, out);
                    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] DNS%d V4 for aid%d is %s",
                            rid, __FUNCTION__, v4Count[responseAid], responseAid,
                            getIpv4Dns(responseAid, index));
                } else {
                   RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] DNS for aid%d is NOT V4 or V6 type",
                           rid, __FUNCTION__, responseAid);
                   goto error;
                }
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //P-CSCF_prim_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing P-CSCF prim addr",
                        rid, __FUNCTION__);
                goto error;
            }
            if (out != NULL && strlen(out) > 0 && strcmp(NULL_IPV4_ADDRESS, out) != 0) {
                if (RmcDcUtility::getAddressType(out) == IPV6) {
                    char pcscfV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
                    convertIpv6Address(pcscfV6, out, 0);
                    updatePcscfAddress(responseAid, pcscfCount[responseAid]++, pcscfV6);
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] P-CSCF is %s",
                            rid, __FUNCTION__, pcscfV6);
                } else if (RmcDcUtility::getAddressType(out) == IPV4) {
                    updatePcscfAddress(responseAid, pcscfCount[responseAid]++, out);
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] P-CSCF is %s",
                            rid, __FUNCTION__, out);
                } else {
                   RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] P-CSCF for aid%d is NOT V4 or V6 type",
                           rid, __FUNCTION__, responseAid);
                   goto error;
                }
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //P-CSCF_sec_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing P-CSCF sec addr",
                        rid, __FUNCTION__);
                goto error;
            }
            if (out != NULL && strlen(out) > 0 && strcmp(NULL_IPV4_ADDRESS, out) != 0) {
                if (RmcDcUtility::getAddressType(out) == IPV6) {
                    char pcscfV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
                    convertIpv6Address(pcscfV6, out, 0);
                    updatePcscfAddress(responseAid, pcscfCount[responseAid]++, pcscfV6);
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] P-CSCF is %s",
                            rid, __FUNCTION__, pcscfV6);
                } else if (RmcDcUtility::getAddressType(out) == IPV4) {
                    updatePcscfAddress(responseAid, pcscfCount[responseAid]++, out);
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] P-CSCF is %s",
                            rid, __FUNCTION__, out);
                } else {
                   RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] P-CSCF for aid%d is NOT V4 or V6 type",
                           rid, __FUNCTION__, responseAid);
                   goto error;
                }
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            int signalingFlag = p_cur->atTokNextint(&err); //IM_CN_Signalling_Flag
            if (err < 0) {
                setSignalingFlag(responseAid, 0);
                cmd.append(String8::format(" invalid signalingFlag"));
            } else {
                setSignalingFlag(responseAid, signalingFlag);
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] signalingFlag = %d for aid%d",
                        rid, __FUNCTION__, getSignalingFlag(responseAid), responseAid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int lipaIndication = p_cur->atTokNextint(&err); //LIPA_indication
            RFX_UNUSED(lipaIndication);
        }

        if (p_cur->atTokHasmore()) {
            // Use MTU from AT+EPDN, ignore this one.
            int mtu = p_cur->atTokNextint(&err); //IPv4_MTU
            if (err < 0) {
                cmd.append(String8::format(" invalid mtu"));
            } else {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] ignore mtu = %d for aid%d",
                        rid, __FUNCTION__, mtu, responseAid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int wlan_offload = p_cur->atTokNextint(&err); //WLAN_Offload
            if (err < 0) {
                cmd.append(String8::format(" invalid WLAN_Offload"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] WLAN_Offload = %d for aid%d",
                        rid, __FUNCTION__, wlan_offload, responseAid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int local_Addr_Ind = p_cur->atTokNextint(&err); //Local_Addr_Ind
            if (err < 0) {
                cmd.append(String8::format(" invalid Local_Addr_Ind"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] Local_Addr_Ind = %d for aid%d",
                        rid, __FUNCTION__, local_Addr_Ind, responseAid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int non_IP_MTU = p_cur->atTokNextint(&err); //Non-IP_MTU
            if (err < 0) {
                cmd.append(String8::format(" invalid Non-IP_MTU"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] Non-IP_MTU = %d for aid%d",
                        rid, __FUNCTION__, non_IP_MTU, responseAid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int sprcv = p_cur->atTokNextint(&err); //Serving_PLMN_rate_control_value
            if (err < 0) {
                cmd.append(String8::format(" invalid Serving_PLMN_rate_control_value"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG,
                        "[%d][%s] Serving_PLMN_rate_control_value = %d for aid%d",
                        rid, __FUNCTION__, sprcv, responseAid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int rds = p_cur->atTokNextint(&err); //Reliable_Data_Service
            if (err < 0) {
                cmd.append(String8::format(" invalid Reliable_Data_Service"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] Reliable_Data_Service = %d for aid%d",
                        rid, __FUNCTION__, rds, responseAid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int psdos = p_cur->atTokNextint(&err); //PS_Data_Off_Support
            if (err < 0) {
                cmd.append(String8::format(" invalid PS_Data_Off_Support"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] PS_Data_Off_Support = %d for aid%d",
                        rid, __FUNCTION__, psdos, responseAid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int pduSessionId = p_cur->atTokNextint(&err); //PDU_session_id
            if (err < 0) {
                cmd.append(String8::format(" invalid PDU_session_id"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] PDU_session_id = %d for aid%d",
                        rid, __FUNCTION__, pduSessionId, responseAid);
                if (!m_pPdnManager->isDedicateBearer(responseAid)) {
                    m_pPdnManager->setPduSessionId(responseAid, pduSessionId);
                }
            }
        }

        if (p_cur->atTokHasmore()) {
            int qfi = p_cur->atTokNextint(&err); //QFI
            if (err < 0) {
                RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] invalid QFI for aid%d",
                        rid, __FUNCTION__, responseAid);
                cmd.append(String8::format(" invalid QFI"));
            } else {
                RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] QFI = %d for aid%d",
                        rid, __FUNCTION__, qfi, responseAid);
                if (!m_pPdnManager->isDedicateBearer(responseAid)
                        && getCurrentRat() == RADIO_TECH_NR) {
                    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] set 5G Qos qfi", rid, __FUNCTION__);
                    RIL_Qos *qos = m_pPdnManager->getDefaultQos(responseAid);
                    if (qos == NULL) {
                        qos = (RIL_Qos*) calloc(1, sizeof(RIL_Qos));
                        RFX_ASSERT(qos != NULL);
                    }

                    qos->nr.qfi = qfi;
                    if (m_pPdnManager->getDefaultQos(responseAid) == NULL) {
                        m_pPdnManager->setDefaultQos(responseAid, qos);
                        FREEIF(qos);
                    }
                }
            }
        }

        if (p_cur->atTokHasmore()) {
            int ssc_mode = p_cur->atTokNextint(&err); //SSC_mode
            if (err < 0) {
                cmd.append(String8::format(" invalid SSC_mode"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] SSC_mode = %d for aid%d",
                        rid, __FUNCTION__, ssc_mode, responseAid);
            }
        }

        if (!cmd.isEmpty()) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]%s for aid%d",
                    rid, __FUNCTION__, cmd.string(), responseAid);
        }
    }

    nRet = PDN_SUCCESS;
    goto finish;

error:
    nRet = PDN_FAILED;

finish:
    FREEIF(pcscfCount);
    FREEIF(v4Count);
    FREEIF(v6Count);
    return nRet;
}

int RmcDcCommonReqHandler::updateDefaultBearerInfo(int aid) {
    //Be aware that this method is used only for retrieving P-CSCF address and signaling flag
    sp<RfxAtResponse> p_response;
    RfxAtLine *p_cur = NULL;
    int v4Count = 0;
    int v6Count = 0;
    int pcscfCount = 0;
    int rid = m_slot_id;
    int err = 0;
    int responseAid = 0;
    int bearerId = 0;
    char* apn = NULL;
    char* out = NULL;
    String8 cmd("");

    //clear default bearer information in AID table since we would query it again
    PdnInfo pdn_info = getPdnInfo(aid);
    memset(pdn_info.pcscf, 0, sizeof(pdn_info.pcscf));
    memset(pdn_info.dnsV4, 0, sizeof(pdn_info.dnsV4));
    memset(pdn_info.dnsV6, 0, sizeof(pdn_info.dnsV6));
    pdn_info.signalingFlag = 0;
    pdn_info.bearerId = INVALID_AID;
    setPdnInfo(aid, &pdn_info);

    /*+CGCONTRDP response a list of:
      [+CGCONTRDP: <cid>,<bearer_id>,<apn>[,<local_addr and subnet_mask>[,<gw_addr>[,<DNS_prim_addr>
      [,<DNS_sec_addr>[,<P-CSCF_prim_addr>[,<P-CSCF_sec_addr>[,<IM_CN_Signalling_Flag>
      [,<LIPA_indication>[,<IPv4_MTU>[,<WLAN_Offload>[,<Local_Addr_Ind>[,<Non-IP_MTU>
      [,<Serving_PLMN_rate_control_value>[,<Reliable_Data_Service>[,<PS_Data_Off_Support>
      [,<PDU_session_id>,<QFI>[,<SSC_mode>[,<S-NSSAI>[,<Access_type>
      [,<RQ_timer>[,<Always-on_ind>]]]]]]]]]]]]]]]]]]]]]]
    */
    p_response = atSendCommandMultiline(String8::format("AT+CGCONTRDP=%d", aid), "+CGCONTRDP:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", rid, __FUNCTION__);
        } else {
            int cause = p_response->atGetCmeError();
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+CGCONTRDP for activatedAid%d returns cme cause : %d",
                    rid, __FUNCTION__, aid, cause);
        }
        return PDN_FAILED;
    }

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        cmd.clear();
        p_cur->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }

        responseAid = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }

        if (responseAid != aid) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid%d is different to MD assigned aid%d",
                    rid, __FUNCTION__, aid, responseAid);
             return PDN_FAILED;
        }

        bearerId = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing bearer id fail",
                    rid, __FUNCTION__);

            // <bearer id> mapped to 4G EPS bearer ID and it might be empty when 5G PDU established
            //return PDN_FAILED;
        } else {
            updateBearerId(aid, bearerId);
        }

        apn = p_cur->atTokNextstr(&err); //apn
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing apn",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //local_addr and subnet_mask
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing local addr and subnet mask",
                        rid, __FUNCTION__);
                return PDN_FAILED;
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //gw_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing gw addr",
                        rid, __FUNCTION__);
                return PDN_FAILED;
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //DNS_prim_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing DNS prim addr",
                        rid, __FUNCTION__);
                return PDN_FAILED;
            }

            if (out != NULL && strlen(out) > 0 && strcmp(NULL_IPV4_ADDRESS, out) != 0) {
                int index = 0;
                if (RmcDcUtility::getAddressType(out) == IPV6) {
                    char dnsV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
                    convertIpv6Address(dnsV6, out, 0);
                    index = v6Count++;
                    updateIpv6Dns(aid, index, dnsV6);
                    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] DNS%d V6 for aid%d is %s",
                            rid, __FUNCTION__, v6Count, aid, getIpv6Dns(aid, index));
                } else if (RmcDcUtility::getAddressType(out) == IPV4) {
                    index = v4Count++;
                    updateIpv4Dns(aid, index, out);
                    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] DNS%d V4 for aid%d is %s",
                            rid, __FUNCTION__, v4Count, aid, getIpv4Dns(aid, index));
                } else {
                   RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] DNS for aid%d is NOT V4 or V6 type",
                           rid, __FUNCTION__, aid);
                   return PDN_FAILED;
                }
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //DNS_sec_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing DNS sec addr",
                        rid, __FUNCTION__);
                return PDN_FAILED;
            }

            if (out != NULL && strlen(out) > 0 && strcmp(NULL_IPV4_ADDRESS, out) != 0) {
                int index = 0;
                if (RmcDcUtility::getAddressType(out) == IPV6) {
                    char dnsV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
                    convertIpv6Address(dnsV6, out, 0);
                    index = v6Count++;
                    updateIpv6Dns(aid, index, dnsV6);
                    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] DNS%d V6 for aid%d is %s",
                            rid, __FUNCTION__, v6Count, aid, getIpv6Dns(aid, index));
                } else if (RmcDcUtility::getAddressType(out) == IPV4) {
                    index = v4Count++;
                    updateIpv4Dns(aid, index, out);
                    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] DNS%d V4 for aid%d is %s",
                            rid, __FUNCTION__, v4Count, aid, getIpv4Dns(aid, index));
                } else {
                   RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] DNS for aid%d is NOT V4 or V6 type",
                           rid, __FUNCTION__, aid);
                   return PDN_FAILED;
                }
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //P-CSCF_prim_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing P-CSCF prim addr",
                        rid, __FUNCTION__);
                return PDN_FAILED;
            }
            if (out != NULL && strlen(out) > 0 && strcmp(NULL_IPV4_ADDRESS, out) != 0) {
                if (RmcDcUtility::getAddressType(out) == IPV6) {
                    char pcscfV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
                    convertIpv6Address(pcscfV6, out, 0);
                    updatePcscfAddress(aid, pcscfCount++, pcscfV6);
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] P-CSCF is %s",
                            rid, __FUNCTION__, pcscfV6);
                } else if (RmcDcUtility::getAddressType(out) == IPV4) {
                    updatePcscfAddress(aid, pcscfCount++, out);
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] P-CSCF is %s",
                            rid, __FUNCTION__, out);
                } else {
                   RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] P-CSCF for aid%d is NOT V4 or V6 type",
                           rid, __FUNCTION__, responseAid);
                   return PDN_FAILED;
                }
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            out = p_cur->atTokNextstr(&err); //P-CSCF_sec_addr
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing P-CSCF sec addr",
                        rid, __FUNCTION__);
                return PDN_FAILED;
            }
            if (out != NULL && strlen(out) > 0 && strcmp(NULL_IPV4_ADDRESS, out) != 0) {
                if (RmcDcUtility::getAddressType(out) == IPV6) {
                    char pcscfV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
                    convertIpv6Address(pcscfV6, out, 0);
                    updatePcscfAddress(aid, pcscfCount++, pcscfV6);
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] P-CSCF is %s",
                            rid, __FUNCTION__, pcscfV6);
                } else if (RmcDcUtility::getAddressType(out) == IPV4) {
                    updatePcscfAddress(aid, pcscfCount++, out);
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] P-CSCF is %s",
                            rid, __FUNCTION__, out);
                } else {
                   RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] P-CSCF for aid%d is NOT V4 or V6 type",
                           rid, __FUNCTION__, aid);
                   return PDN_FAILED;
                }
            }
            out = NULL;
        }

        if (p_cur->atTokHasmore()) {
            int signalingFlag = p_cur->atTokNextint(&err); //IM_CN_Signalling_Flag
            if (err < 0) {
                setSignalingFlag(aid, 0);
                cmd.append(String8::format(" invalid signalingFlag"));
            } else {
                setSignalingFlag(aid, signalingFlag);
            }
        }

        if (p_cur->atTokHasmore()) {
            int lipaIndication = p_cur->atTokNextint(&err); //LIPA_indication
            if (err < 0) {
                cmd.append(String8::format(" invalid LIPA_indication"));
            } else {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] LIPA_indication = %d for aid%d",
                        rid, __FUNCTION__, lipaIndication, responseAid);
            }
        }

        if (p_cur->atTokHasmore()) {
            // Use MTU from AT+EPDN, ignore this one.
            int mtu = p_cur->atTokNextint(&err); //IPv4_MTU
            if (err < 0) {
                cmd.append(String8::format(" invalid mtu"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] ignore mtu = %d for aid%d",
                        rid, __FUNCTION__, mtu, aid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int wlan_offload = p_cur->atTokNextint(&err); //WLAN_Offload
            if (err < 0) {
                cmd.append(String8::format(" invalid WLAN_Offload"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] WLAN_Offload = %d for aid%d",
                        rid, __FUNCTION__, wlan_offload, aid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int local_Addr_Ind = p_cur->atTokNextint(&err); //Local_Addr_Ind
            if (err < 0) {
                cmd.append(String8::format(" invalid Local_Addr_Ind"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] Local_Addr_Ind = %d for aid%d",
                        rid, __FUNCTION__, local_Addr_Ind, aid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int non_IP_MTU = p_cur->atTokNextint(&err); //Non-IP_MTU
            if (err < 0) {
                cmd.append(String8::format(" invalid Non-IP_MTU"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] Non-IP_MTU = %d for aid%d",
                        rid, __FUNCTION__, non_IP_MTU, aid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int sprcv = p_cur->atTokNextint(&err); //Serving_PLMN_rate_control_value
            if (err < 0) {
                cmd.append(String8::format(" invalid Serving_PLMN_rate_control_value"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG,
                        "[%d][%s] Serving_PLMN_rate_control_value = %d for aid%d",
                        rid, __FUNCTION__, sprcv, aid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int rds = p_cur->atTokNextint(&err); //Reliable_Data_Service
            if (err < 0) {
                cmd.append(String8::format(" invalid Reliable_Data_Service"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] Reliable_Data_Service = %d for aid%d",
                        rid, __FUNCTION__, rds, aid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int psdos = p_cur->atTokNextint(&err); //PS_Data_Off_Support
            if (err < 0) {
                cmd.append(String8::format(" invalid PS_Data_Off_Support"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] PS_Data_Off_Support = %d for aid%d",
                        rid, __FUNCTION__, psdos, aid);
            }
        }

        if (p_cur->atTokHasmore()) {
            int pduSessionId = p_cur->atTokNextint(&err); //PDU_session_id
            if (err < 0) {
                cmd.append(String8::format(" invalid PDU_session_id"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] PDU_session_id = %d for aid%d",
                        rid, __FUNCTION__, pduSessionId, aid);
                if (!m_pPdnManager->isDedicateBearer(aid)) {
                    m_pPdnManager->setPduSessionId(aid, pduSessionId);
                }
            }
        }

        if (p_cur->atTokHasmore()) {
            int qfi = p_cur->atTokNextint(&err); //QFI
            if (err < 0) {
                RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] invalid QFI for aid%d",
                        rid, __FUNCTION__, aid);
                cmd.append(String8::format(" invalid QFI"));
            } else {
                RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] QFI = %d for aid%d",
                        rid, __FUNCTION__, qfi, aid);
                if (!m_pPdnManager->isDedicateBearer(aid) && getCurrentRat() == RADIO_TECH_NR) {
                    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] set 5G Qos qfi", rid, __FUNCTION__);
                    RIL_Qos *qos = m_pPdnManager->getDefaultQos(aid);
                    if (qos == NULL) {
                        qos = (RIL_Qos*) calloc(1, sizeof(RIL_Qos));
                        RFX_ASSERT(qos != NULL);
                    }

                    qos->nr.qfi = qfi;
                    if (m_pPdnManager->getDefaultQos(aid) == NULL) {
                        m_pPdnManager->setDefaultQos(aid, qos);
                        FREEIF(qos);
                    }
                }
            }
        }

        if (p_cur->atTokHasmore()) {
            int ssc_mode = p_cur->atTokNextint(&err); //SSC_mode
            if (err < 0) {
                cmd.append(String8::format(" invalid SSC_mode"));
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] SSC_mode = %d for aid%d",
                        rid, __FUNCTION__, ssc_mode, aid);
            }
        }

        if (!cmd.isEmpty()) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]%s for aid%d",
                    rid, __FUNCTION__, cmd.string(), responseAid);
        }
    }

    return PDN_SUCCESS;
}

int RmcDcCommonReqHandler::getCurrentRat() {
    RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
    RfxNwServiceState ss = getMclStatusManager(m_slot_id)
            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    int dataRadioTech = ss.getRilDataRadioTech();
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] data radiotech:%d",
            m_slot_id, __FUNCTION__, dataRadioTech);
    return dataRadioTech;
}

bool RmcDcCommonReqHandler::updateSecondaryPdpDynamicParameters(int aid) {
    if (aid < 0 || aid >= getPdnTableSize()) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] invalid aid%d", m_slot_id, __FUNCTION__, aid);
        return false;
    }
    if (!m_pPdnManager->isDedicateBearer(aid)) {
        RFX_LOG_E(RFX_LOG_TAG,
            "[%d][%s] not dedicate bearer, aid:%d", m_slot_id, __FUNCTION__, aid);
        return false;
    }

    int p_cid = 0;
    int bearerId = 0;
    int imCnSF = 0;
    int wlanOffload = 0;
    int pduSessionId = 0;
    int qfi = 0;
    int responseAid = 0;
    int err = 0;
    RfxAtLine *p_cur = NULL;

    /* +CGSCONTRDP response a list of:
      [+CGSCONTRDP: <cid>,<p_cid>,<bearer_id>[,<IM_CN_Signalling_Flag>[,<WLAN_Offload>
      [,<PDU_session_id>,<QFI>]]]]
    */
    sp<RfxAtResponse> p_response = atSendCommandMultiline(String8::format("AT+CGSCONTRDP=%d",
            aid), "+CGSCONTRDP:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!", m_slot_id, __FUNCTION__);
        return false;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", m_slot_id, __FUNCTION__);
        } else {
            int cause = p_response->atGetCmeError();
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+CGSCONTRDP for aid%d returns cme cause : %d",
                    m_slot_id, __FUNCTION__, aid, cause);
        }
        return false;
    }

    p_cur = p_response->getIntermediates();
    if (p_cur == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] p_cur is NULL", m_slot_id, __FUNCTION__);
        return false;
    }

    p_cur->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        return false;
    }

    responseAid = p_cur->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing response aid",
                m_slot_id, __FUNCTION__);
        return false;
    }

    if (responseAid != aid) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid%d is different from MD assigned aid%d",
                m_slot_id, __FUNCTION__, aid, responseAid);
        return false;
    }

    p_cid = p_cur->atTokNextint(&err); // p_cid
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing p_cid fail", m_slot_id, __FUNCTION__);
        return false;
    }

    bearerId = p_cur->atTokNextint(&err); // bearer_id
    if (err < 0) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] parsing bearer_id fail",
                m_slot_id, __FUNCTION__);
        // bearer id might be empty for NR
    }  else {
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] bearer_id = %d for aid%d",
                m_slot_id, __FUNCTION__, bearerId, aid);
        if (getCurrentRat() == RADIO_TECH_LTE || getCurrentRat() == RADIO_TECH_LTE_CA) {
            // For 4G, treat bearer id as qosSessionId
            RIL_QosSession *qosSession = m_pPdnManager->getQosSession(aid);
            if (qosSession == NULL) {
                m_pPdnManager->setQosSessionCount(aid, 1);
                RIL_QosSession tmpQosSession;
                tmpQosSession.qosSessionId = bearerId;
                m_pPdnManager->setQosSession(aid, &tmpQosSession);
            } else {
                qosSession->qosSessionId = bearerId;
            }
        }
    }

    if (p_cur->atTokHasmore()) {
        imCnSF = p_cur->atTokNextint(&err); // IM_CN_Signalling_Flag
        if (err < 0) {
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] parsing IM_CN_Signalling_Flag fail",
                    m_slot_id, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        wlanOffload = p_cur->atTokNextint(&err); // WLAN_Offload
        if (err < 0) {
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] parsing WLAN_Offload fail",
                    m_slot_id, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        pduSessionId = p_cur->atTokNextint(&err); // PDU_session_id
        if (err < 0) {
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] parsing PDU_session_id fail",
                    m_slot_id, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        qfi = p_cur->atTokNextint(&err); // QFI
        if (err < 0) {
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] parsing QFI fail",
                    m_slot_id, __FUNCTION__);
        } else {
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] QFI = %d for aid%d",
                    m_slot_id, __FUNCTION__, qfi, aid);
            if (getCurrentRat() == RADIO_TECH_NR) {
                // For 5G, treat QFI as qosSessionId and NrQos.qfi.
                RIL_QosSession *qosSession = m_pPdnManager->getQosSession(aid);
                if (qosSession == NULL) {
                    qosSession = (RIL_QosSession*) calloc(1, sizeof(RIL_QosSession));
                }
                if (qosSession == NULL) {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] calloc failed!",
                    m_slot_id, __FUNCTION__);
                    return false;
                }
                qosSession->qosSessionId = qfi;
                qosSession->qos.nr.qfi = qfi;
                if (m_pPdnManager->getQosSession(aid) == NULL) {
                    m_pPdnManager->setQosSessionCount(aid, 1);
                    m_pPdnManager->setQosSession(aid, qosSession);
                    FREEIF(qosSession);
                }
            }
        }
    }
    return true;
}

bool RmcDcCommonReqHandler::updateEpsQos(int aid) {
    if (aid < 0 || aid >= getPdnTableSize()) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] invalid aid%d", m_slot_id, __FUNCTION__, aid);
        return false;
    }
    int qci = 0;
    int dl_mbr = 0;
    int ul_mbr = 0;
    int dl_gbr = 0;
    int ul_gbr = 0;
    int dl_ambr = 0;
    int ul_ambr = 0;
    int rid = m_slot_id;
    int err = 0;
    int responseAid = 0;
    RfxAtLine *p_cur = NULL;

    /*
    [+CGEQOSRDP: <cid>,<QCI>,[<DL_GBR>,<UL_GBR>],[<DL_MBR>,<UL_MBR>][,<DL_AMBR>,<UL_AMBR>]]
    [<CR><LF>+CGEQOSRDP: <cid>,<QCI>,[<DL_GBR>,<UL_GBR>],[<DL_MBR>,<UL_MBR>][,<DL_AMBR>,<UL_AMBR>]
    [...]]
    If the parameter <cid> is omitted, the Quality of Service parameters for all secondary
    and non secondary active PDP contexts are returned.
    */
    sp<RfxAtResponse> p_response = atSendCommandMultiline(String8::format("AT+CGEQOSRDP=%d", aid),
            "+CGEQOSRDP:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!", rid, __FUNCTION__);
        return false;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", rid, __FUNCTION__);
        } else {
            int cause = p_response->atGetCmeError();
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+CGEQOSRDP for aid%d returns cme cause : %d",
                    rid, __FUNCTION__, aid, cause);
        }
        return false;
    }

    p_cur = p_response->getIntermediates();
    if (p_cur == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] p_cur is NULL", rid, __FUNCTION__);
        return false;
    }

    p_cur->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        return false;
    }

    responseAid = p_cur->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing response aid",
                rid, __FUNCTION__);
        return false;
    }

    if (responseAid != aid) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid%d is different from MD assigned aid%d",
                rid, __FUNCTION__, aid, responseAid);
         return false;
    }

    qci = p_cur->atTokNextint(&err); // qci
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing qci fail", rid, __FUNCTION__);
        return false;
    }

    if (p_cur->atTokHasmore()) {
        dl_gbr = p_cur->atTokNextint(&err); // optional dl_gbr
        if (err < 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no dl_gbr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        ul_gbr = p_cur->atTokNextint(&err); // optional ul_gbr
        if (err < 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no ul_gbr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        dl_mbr = p_cur->atTokNextint(&err); // optional dl_mbr
        if (err < 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no dl_mbr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        ul_mbr = p_cur->atTokNextint(&err); // optional ul_mbr
        if (err < 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no ul_mbr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        dl_ambr = p_cur->atTokNextint(&err); // optional dl_ambr
        if (err < 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no dl_ambr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        ul_ambr = p_cur->atTokNextint(&err); // optional ul_ambr
        if (err < 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no ul_ambr", rid, __FUNCTION__);
        }
    }

    if (!m_pPdnManager->isDedicateBearer(aid)) {
        RIL_Qos defaultQos;
        defaultQos.eps.qci = qci;
        defaultQos.eps.downlink.guaranteedBitrateKbps = dl_gbr;
        defaultQos.eps.downlink.maxBitrateKbps = dl_mbr;
        defaultQos.eps.uplink.guaranteedBitrateKbps = ul_gbr;
        defaultQos.eps.uplink.maxBitrateKbps  = ul_mbr;
        m_pPdnManager->setDefaultQos(aid, &defaultQos);
        return true;
    } else {
        RIL_QosSession *pQosSession = m_pPdnManager->getQosSession(aid);
        RIL_QosSession tmpQosSession;
        RIL_QosSession *pTmpQosSession = pQosSession != NULL ? pQosSession : &tmpQosSession;

        pTmpQosSession->qos.eps.qci = qci;
        pTmpQosSession->qos.eps.downlink.guaranteedBitrateKbps = dl_gbr;
        pTmpQosSession->qos.eps.downlink.maxBitrateKbps = dl_mbr;
        pTmpQosSession->qos.eps.uplink.guaranteedBitrateKbps = ul_gbr;
        pTmpQosSession->qos.eps.uplink.maxBitrateKbps  = ul_mbr;

        if (pQosSession == NULL) {
            m_pPdnManager->setQosSession(aid, pTmpQosSession);
        }
        return true;
    }
}

bool RmcDcCommonReqHandler::updateNrQos(int aid) {
    if (aid < 0 || aid >= getPdnTableSize()) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] invalid aid%d", m_slot_id, __FUNCTION__, aid);
        return false;
    }
    int fiveQi = 0;
    int dl_gfbr = 0;
    int ul_gfbr = 0;
    int dl_mfbr = 0;
    int ul_mfbr = 0;
    int dl_sambr = 0;
    int ul_sambr = 0;
    int averagingWindow = 0;
    int rid = m_slot_id;
    int err = 0;
    int responseAid = 0;
    RfxAtLine *p_cur = NULL;

    /*
    [+C5GQOSRDP: <cid>,<5QI>[,<DL_GFBR>,<UL_GFBR>[,<DL_MFBR>,<UL_MFBR>
    [,<DL_SAMBR>,<UL_SAMBR>[,<Averaging_window>]]]]]

    [<CR><LF>+C5GQOSRDP: <cid>,<5QI>[,<DL_GFBR>,<UL_GBR>[,<DL_MFBR>,<UL_MFBR>
    [,<DL_SAMBR>,<UL_SAMBR>[,<Averaging_window>]]]]

    [...]]

    If the parameter <cid> is omitted, the Quality of Service parameters for
    all QoS flows are returned.
    */

    sp<RfxAtResponse> p_response = atSendCommandMultiline(String8::format("AT+C5GQOSRDP=%d", aid),
            "+C5GQOSRDP:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!", rid, __FUNCTION__);
        return false;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", rid, __FUNCTION__);
        } else {
            int cause = p_response->atGetCmeError();
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+C5GQOSRDP for aid%d returns cme cause : %d",
                    rid, __FUNCTION__, aid, cause);
        }
        return false;
    }

    p_cur = p_response->getIntermediates();
    if (p_cur == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] p_cur is NULL", rid, __FUNCTION__);
        return false;
    }

    p_cur->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        return false;
    }

    responseAid = p_cur->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing response aid",
                rid, __FUNCTION__);
        return false;
    }

    if (responseAid != aid) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid%d is different from MD assigned aid%d",
                rid, __FUNCTION__, aid, responseAid);
         return false;
    }

    fiveQi = p_cur->atTokNextint(&err); // 5QI
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing 5QI fail",
                rid, __FUNCTION__);
        return false;
    }

    if (p_cur->atTokHasmore()) {
        dl_gfbr = p_cur->atTokNextint(&err); // dl_gfbr
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no dl_gfbr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        ul_gfbr = p_cur->atTokNextint(&err); // ul_gfbr
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no ul_gfbr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        dl_mfbr = p_cur->atTokNextint(&err); // dl_mfbr
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no dl_mfbr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        ul_mfbr = p_cur->atTokNextint(&err); // ul_mfbr
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no ul_mfbr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        dl_sambr = p_cur->atTokNextint(&err); // dl_sambr
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no dl_sambr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        ul_sambr = p_cur->atTokNextint(&err); // ul_sambr
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no ul_sambr", rid, __FUNCTION__);
        }
    }

    if (p_cur->atTokHasmore()) {
        averagingWindow = p_cur->atTokNextint(&err); // averagingWindow
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no averagingWindow", rid, __FUNCTION__);
        }
    }

    /* From 24.501, "The network shall not set the QFI value to 0".
     * In current design flow, qfi should already be set when
     * update default bearer by sending CGCONTRDP for default bearer
     * or CGSCONTRDP for dedicate bearer
     */
    if (!m_pPdnManager->isDedicateBearer(aid)) {
        RIL_Qos *defaultQos = m_pPdnManager->getDefaultQos(aid);
        if (defaultQos == NULL) {
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] defaultQos is NULL", rid, __FUNCTION__);
            return false;
        }
        defaultQos->nr.fiveQi = fiveQi;
        defaultQos->nr.averagingWindowMs = averagingWindow;
        defaultQos->nr.downlink.guaranteedBitrateKbps = dl_gfbr;
        defaultQos->nr.downlink.maxBitrateKbps = dl_mfbr;
        defaultQos->nr.uplink.guaranteedBitrateKbps = ul_gfbr;
        defaultQos->nr.uplink.maxBitrateKbps = ul_mfbr;
        return true;
    } else {
        RIL_QosSession *qosSession = m_pPdnManager->getQosSession(aid);
        if (qosSession == NULL) {
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] qosSession is NULL", rid, __FUNCTION__);
            return false;
        }
        qosSession->qos.nr.fiveQi = fiveQi;
        qosSession->qos.nr.averagingWindowMs = averagingWindow;
        qosSession->qos.nr.downlink.guaranteedBitrateKbps = dl_gfbr;
        qosSession->qos.nr.downlink.maxBitrateKbps = dl_mfbr;
        qosSession->qos.nr.uplink.guaranteedBitrateKbps = ul_gfbr;
        qosSession->qos.nr.uplink.maxBitrateKbps = ul_mfbr;
        qosSession->qos.nr.qfi = qosSession->qosSessionId;
        return true;
    }
}

bool RmcDcCommonReqHandler::updateQosFilters(int aid) {
    if (aid < 0 || aid >= getPdnTableSize()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] invalid aid%d", m_slot_id, __FUNCTION__, aid);
        return false;
    }
    if (!m_pPdnManager->isDedicateBearer(aid)) {
        RFX_LOG_E(RFX_LOG_TAG,
            "[%d][%s] donot update QosFilters for non-dedicatebearer, aid:%d",
            m_slot_id, __FUNCTION__, aid);
        return false;
    }

    int outInt = 0;
    int responseAid = 0;
    int err = 0;
    char* out = NULL;
    RfxAtLine *p_cur = NULL;
    RIL_QosSession *qosSession = NULL;
    RIL_QosFilter *qosFilters = NULL;
    int qosFilterCount = 0;
    int i = 0;
    int nSize = 0;
    char *token = NULL;
    char *tmp = NULL;
    char *saveptr = NULL;
    const char *dotDelim = ".";
    int portStart = -1;
    int portEnd = -1;
    /* +CGTFTRDP response a list of:
      [+CGTFTRDP: <cid>,<packet filter identifier>,<evaluation precedence index>,
      <remote address and subnet mask>,<protocol number (ipv4) / next header (ipv6)>,
      <local port range>,<remote port range>,<ipsec security parameter index (spi)>,
      <type of service (tos) (ipv4) and mask / traffic class (ipv6) and mask>,
      <flow label (ipv6)>,<direction>,<NW packet filter Identifier>,
      <local address and subnet mask>,<QRI>]
    */
    sp<RfxAtResponse> p_response = atSendCommandMultiline(String8::format("AT+CGTFTRDP=%d", aid),
            "+CGTFTRDP:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!", m_slot_id, __FUNCTION__);
        return false;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", m_slot_id, __FUNCTION__);
        } else {
            int cause = p_response->atGetCmeError();
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+CGTFTRDP for aid%d returns cme cause : %d",
                    m_slot_id, __FUNCTION__, aid, cause);
        }
        return false;
    }
    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        qosFilterCount++;
    }
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] qosFilterCount=%d",
            m_slot_id, __FUNCTION__, qosFilterCount);

    qosFilters = (RIL_QosFilter*) calloc(1, sizeof(RIL_QosFilter)*qosFilterCount);
    if (qosFilters == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] qosFilters calloc failed",
                m_slot_id, __FUNCTION__);
    }

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        p_cur->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                    m_slot_id, __FUNCTION__);
            goto error;
        }

        responseAid = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing response aid",
                    m_slot_id, __FUNCTION__);
            goto error;
        }

        if (responseAid != aid) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid%d is different from MD assigned aid%d",
                    m_slot_id, __FUNCTION__, aid, responseAid);
             goto error;
        }

        outInt = p_cur->atTokNextint(&err); // packet filter identifier
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing packet filter identifier fail",
                    m_slot_id, __FUNCTION__);
            goto error;
        }

        outInt = p_cur->atTokNextint(&err); // evaluation precedence index
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing evaluation precedence index fail",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        qosFilters[i].precedence = outInt;

        /* remote address and subnet mask,
           The string is given as dot-separated numeric (0-255), parameters on the form:
           "a1.a2.a3.a4.m1.m2.m3.m4" for IPv4 or
           "a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.m1.m2.m3.m4.m5.m6.m7
           .m8.m9.m10.m11.m12.m13.m14.m15.m16" for IPv6.
        */
        out = p_cur->atTokNextstr(&err); // remote address and subnet mask
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG,
                    "[%d][%s] ERROR occurs when parsing remote address and subnet mask",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        if (out != NULL && strlen(out) > 0) {
            RFX_LOG_D(RFX_LOG_TAG,
                    "[%d][%s] remote address and subnet mask: %s",
                    m_slot_id, __FUNCTION__, out);
            asprintf(&tmp, "%s", out);
            nSize = 0;
            token = strstr(tmp, dotDelim);
            while (token != NULL) {
                nSize++;
                token = strstr(token+1, dotDelim);
            }
            nSize++;
            if (nSize == DOT_SEP_IPV6_ADDR_AND_SUBNETMASK_PART_NUM) {
                qosFilters[i].remoteAddresses = (char*) calloc(1,
                        sizeof(char)*MAX_IPV6_ADDR_AND_MASK_LENGTH);
            } else if (nSize == DOT_SEP_IPV4_ADDR_AND_SUBNETMASK_PART_NUM) {
                qosFilters[i].remoteAddresses = (char*) calloc(1,
                        sizeof(char)*MAX_IPV4_ADDR_AND_MASK_LENGTH);
            } else {
               RFX_LOG_I(RFX_LOG_TAG,
                       "[%d][%s] remote address and subnet mask for aid%d is NOT V4 or V6 type"
                       ", count=%d",
                       m_slot_id, __FUNCTION__, aid, nSize);
               FREEIF(tmp);
               return PDN_FAILED;
            }
            RFX_ASSERT(qosFilters[i].remoteAddresses != NULL);
            convertAddrAndSubnetMask(qosFilters[i].remoteAddresses, out);

            FREEIF(tmp);
            token = NULL;
            out = NULL;
        }

        // protocol number (ipv4) / next header (ipv6), The value range is from 0 to 255.
        outInt = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG,
                    "[%d][%s] parsing evaluation protocol number index fail",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        if (outInt == TCP) {
            qosFilters[i].protocol = TCP;
        } else if (outInt == UDP) {
            qosFilters[i].protocol = UDP;
        } else if (outInt == ESP) {
            qosFilters[i].protocol = ESP;
        } else if (outInt == AH) {
            qosFilters[i].protocol = AH;
        } else {
            qosFilters[i].protocol = NONE_PROTOCOL;
        }

        // <local port range>: The string is given as dot-separated numeric (0-65535)
        // parameters on the form "f.t".
        out = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG,
                    "[%d][%s] ERROR occurs when parsing local port range",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        if (out != NULL && strlen(out) > 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] local port range: %s",
                    m_slot_id, __FUNCTION__, out);
            if (strstr(out, dotDelim) != NULL) {
                asprintf(&tmp, "%s", out);

                token = strtok_r(tmp, dotDelim, &saveptr);
                if (token != NULL && strlen(token) > 0) {
                    portStart = atoi(token);
                }
                token = strtok_r(NULL, dotDelim, &saveptr);
                if (token != NULL && strlen(token) > 0) {
                    portEnd = atoi(token);
                }
                FREEIF(tmp);
                saveptr = NULL;
                token = NULL;
                qosFilters[i].localPort.start = portStart;
                qosFilters[i].localPort.end = portEnd;
            } else {
                RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] local port range format is illegal",
                    m_slot_id, __FUNCTION__);
                qosFilters[i].localPort.start = atoi(out);
                qosFilters[i].localPort.end = atoi(out);
            }
            out = NULL;
        }

        // <remote port range>: The string is given as dot-separated numeric (0-65535)
        // parameters on the form "f.t".
        out = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG,
                    "[%d][%s] ERROR occurs when parsing remote port range",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        if (out != NULL && strlen(out) > 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] remote port range: %s",
                    m_slot_id, __FUNCTION__, out);
            if (strstr(out, dotDelim) != NULL) {
                asprintf(&tmp, "%s", out);

                token = strtok_r(tmp, dotDelim, &saveptr);
                if (token != NULL && strlen(token) > 0) {
                    portStart = atoi(token);
                }
                token = strtok_r(NULL, dotDelim, &saveptr);
                if (token != NULL && strlen(token) > 0) {
                    portEnd = atoi(token);
                }
                FREEIF(tmp);
                saveptr = NULL;
                token = NULL;
                qosFilters[i].remotePort.start = portStart;
                qosFilters[i].remotePort.end = portEnd;
            } else {
                RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] remote port range format is illegal",
                    m_slot_id, __FUNCTION__);
                qosFilters[i].remotePort.start = atoi(out);
                qosFilters[i].remotePort.end = atoi(out);
            }
            out = NULL;
        }

        // <ipsec security parameter index (spi)>: numeric value in hexadecimal format.
        // The value range is from 00000000 to FFFFFFFF.
        out = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing ipsec spi",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        if (out != NULL && strlen(out) > 0) {
            qosFilters[i].ipSecSpi = RmcDcUtility::hexStrToDec(out);
            RFX_LOG_I(RFX_LOG_TAG,
                    "[%d][%s] ipsec spi hex: %s, dec:%ld",
                    m_slot_id, __FUNCTION__, out, qosFilters[i].ipSecSpi);
            out = NULL;
        } else {
            qosFilters[i].ipSecSpi = -1;
        }

        // <type of service (tos) (ipv4) and mask / traffic class (ipv6) and mask>:
        // string type. The string is given as dot-separated numeric (0-255)
        // parameters on the form "t.m".
        out = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG,
                    "[%d][%s] ERROR occurs when parsing tos(ipv4)/ traffic clsss(ipv6)",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        if (out != NULL && strlen(out) > 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] tos: %s", m_slot_id, __FUNCTION__, out);
            asprintf(&tmp, "%s", out);
            token = strtok_r(tmp, dotDelim, &saveptr);
            if (token != NULL && strlen(token) > 0) {
                qosFilters[i].typeOfService = atoi(token);
            }
            FREEIF(tmp);
            saveptr = NULL;
            token = NULL;
            out = NULL;
        } else {
            qosFilters[i].typeOfService = -1;
        }

        // <flow label (ipv6)>: numeric value in hexadecimal format.
        // The value range is from 00000 to FFFFF. Valid for IPv6 only.
        out = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG,
                    "[%d][%s] ERROR occurs when parsing flow label (ipv6)",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        if (out != NULL && strlen(out) > 0) {
            qosFilters[i].ipv6FlowLabel = RmcDcUtility::hexStrToDec(out);
            RFX_LOG_D(RFX_LOG_TAG,
                    "[%d][%s] flow label (ipv6) hex: %s, dec:%ld",
                    m_slot_id, __FUNCTION__, out, qosFilters[i].ipv6FlowLabel);
            out = NULL;
        } else {
            qosFilters[i].ipv6FlowLabel = -1;
        }

        // <direction>: integer type. Specifies the transmission direction
        // in which the packet filter shall be applied.
        // 0   Pre-Release 7 TFT filter (see 3GPP TS 24.008 [8], table 10.5.162)
        // 1   Uplink
        // 2   Downlink
        // 3   Bidirectional (Up & Downlink)
        outInt = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing direction fail",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        // since AOSP defination are different from 3gpp spec, convert it
        if (outInt == 0) {
            qosFilters[i].direction = BIDIRECTIONAL;
        } else if (outInt == 1) {
            qosFilters[i].direction = UPLINK;
        } else if (outInt == 2) {
            qosFilters[i].direction = DOWNLINK;
        } else if (outInt == 3) {
            qosFilters[i].direction = BIDIRECTIONAL;
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Surprise! direction:%d",
                    m_slot_id, __FUNCTION__, outInt);
        }

        // <NW packet filter Identifier>
        outInt = p_cur->atTokNextint(&err); // packet filter identifier
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing nw packet filter identifier fail",
                    m_slot_id, __FUNCTION__);
            goto error;
        }

        // <local address and subnet mask>: string type.
        // The string is given as dot-separated numeric (0-255) parameters on the form:
        // "a1.a2.a3.a4.m1.m2.m3.m4" for IPv4 or
        // "a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.m1.m2.m3.m4.m5.m6.m7
        // .m8.m9.m10.m11.m12.m13.m14.m15.m16", for IPv6.
        out = p_cur->atTokNextstr(&err); // local address and subnet mask
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG,
                    "[%d][%s] ERROR occurs when parsing local address and subnet mask",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        if (out != NULL && strlen(out) > 0) {
            RFX_LOG_D(RFX_LOG_TAG,
                    "[%d][%s] local address and subnet mask: %s",
                    m_slot_id, __FUNCTION__, out);
            asprintf(&tmp, "%s", out);
            nSize = 0;
            token = strstr(tmp, dotDelim);
            while (token != NULL) {
                nSize++;
                token = strstr(token+1, dotDelim);
            }
            nSize++;
            if (nSize == DOT_SEP_IPV6_ADDR_AND_SUBNETMASK_PART_NUM) {
                qosFilters[i].localAddresses = (char*) calloc(1,
                        sizeof(char)*MAX_IPV6_ADDR_AND_MASK_LENGTH);
            } else if (nSize == DOT_SEP_IPV4_ADDR_AND_SUBNETMASK_PART_NUM) {
                qosFilters[i].localAddresses = (char*) calloc(1,
                        sizeof(char)*MAX_IPV4_ADDR_AND_MASK_LENGTH);
            } else {
               RFX_LOG_I(RFX_LOG_TAG,
                       "[%d][%s] local address and subnet mask for aid%d is NOT V4 or V6 type"
                       ", count=%d",
                       m_slot_id, __FUNCTION__, aid, nSize);
               return PDN_FAILED;
            }
            RFX_ASSERT(qosFilters[i].localAddresses != NULL);
            convertAddrAndSubnetMask(qosFilters[i].localAddresses, out);

            FREEIF(tmp);
            token = NULL;
            out = NULL;
        }

        /* not use.
        // <QRI>: integer type. Identifies the QoS rule,
        // see 3GPP TS 23.501 [165] and 3GPP TS 24.501 [161].
        outInt = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing QRI fail",
                    m_slot_id, __FUNCTION__);
            goto error;
        }

        // <traffic_segregation>: integer type; indicates to the network whether
        // traffic segregation is requested or not, see 3GPP TS 24.501 [161].
        // 0   traffic segregation is not requested
        // 1   traffic segregation is requested
        outInt = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parsing traffic_segregation fail",
                    m_slot_id, __FUNCTION__);
            goto error;
        }
        */

        i++;
    }

    m_pPdnManager->setQosFilters(aid, qosFilterCount, qosFilters);

    goto finish;

error:
    FREEIF(qosFilters);
    FREEIF(qosSession);
    return false;

finish:
    FREEIF(qosFilters);
    FREEIF(qosSession);
    return true;
}

/*
 * For default bearer, update Qos based on RAT
 * For dedicate bearer, update Qos Session
 */
bool RmcDcCommonReqHandler::updateQosInformation(int aid, bool isDedicateBearer) {
    if (aid < 0 || aid >= getPdnTableSize()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] invalid aid%d", m_slot_id, __FUNCTION__, aid);
        return false;
    }

    int rat = m_pPdnManager->getAccessNetwork(aid);
    // update dedicate bearer QosFilters
    // From perspective of 3GPP spec, Qos Session(qosSessionId,Qos, Qos Filter)
    // is exist for 2G/3G/4G/5G. But, currently AOSP only design Qos struct for
    // 4G and 5G, so we only need update Qos session info for 4G and 5G.
    if (isDedicateBearer && (rat == E_UTRAN || rat == NG_RAN)) {
        // update dedicate bearer QosSessionId
        if (!updateSecondaryPdpDynamicParameters(aid)) {
            RFX_LOG_E(RFX_LOG_TAG,
                    "[%d][%s] updateSecondaryPdpDynamicParameters failed, aid%d",
                    m_slot_id, __FUNCTION__, aid);
            return false;
        }

        // update dedicate bearer QosFilters
        if (!updateQosFilters(aid)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateQosFilters failed, aid%d",
                    m_slot_id, __FUNCTION__, aid);
            return false;
        }
    }

    // update Qos
    if (rat == E_UTRAN) {
        if (!updateEpsQos(aid)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateEpsQos failed, aid%d",
                    m_slot_id, __FUNCTION__, aid);
            return false;
        }
    } else if (rat == NG_RAN) {
        if (!updateNrQos(aid)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] updateNrQos failed, aid%d",
                    m_slot_id, __FUNCTION__, aid);
            return false;
        }
    } else {
        RFX_LOG_W(RFX_LOG_TAG,
               "[%d][%s] Rat is neither 4G nor 5G, donot set QosInfo",
               m_slot_id, __FUNCTION__);
        return false;
    }
    return true;
}

void RmcDcCommonReqHandler::updatePdnAddress() {
    const int pdnInfoSize = getPdnTableSize();
    int rid = m_slot_id;

    for (int i = 0; i < pdnInfoSize; i++) {
        if (DATA_STATE_INACTIVE != getPdnActiveStatus(i)) {
            if (PDN_SUCCESS != updatePdnAddress(i)) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Fail to updatePdnAddress for aid%d",
                        rid, __FUNCTION__, i);
            }
        }
    }
}

int RmcDcCommonReqHandler::updatePdnAddress(int aid) {
    String8 cmd("");
    RfxAtLine *p_cur = NULL;
    sp<RfxAtResponse> p_response;
    int err = -1;
    int rid = m_slot_id;
    int cause = 0;
    int row = 1;
    int ipv6Index = 0;

    if (isDedicateBearer(aid)) {
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] NO need to update dedicate bearer address [aid%d]",
                rid, __FUNCTION__, aid);
        return PDN_SUCCESS;
    }

    /* Firstly clear all ip addresses with aid before update it */
    updateIpv4Address(aid, "");
    for (int i = 0; i < MAX_NUM_IPV6_ADDRESS_NUMBER; i++) {
        updateIpv6Address(aid, i, "");
    }

    /* Use AT+EPDN=<aid>,"addr" to query the ip address assigned to this PDP context indicated via this aid */
    cmd.append(String8::format("AT+EPDN=%d,\"addr\"", aid));

    // Response:
    //    +EPDN:<aid>,"addr",<address type>[,<address>]
    //    +EPDN:<aid>,"err",<err>
    p_response = atSendCommandMultiline(cmd, "+EPDN:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response for aid%d",
                rid, __FUNCTION__, aid);
        return PDN_FAILED;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause for aid% "
                    "due to final response indicates success", rid, __FUNCTION__, aid);
        }
        return PDN_FAILED;
    }

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        ADDRESS_TYPE addrType = ADDRESS_NULL;
        int responseAid = INVALID_AID;
        char *cmdFormat = NULL;
        char *address = NULL;

        p_cur->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start with aid%d for %d's row",
                    rid, __FUNCTION__, aid, row);
            return PDN_FAILED;
        }

        /* Get 1st parameter: AID */
        responseAid = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid%d for %d's row",
                    rid, __FUNCTION__, aid, row);
            return PDN_FAILED;
        }

        if (responseAid != aid) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR aid%d is different to MD assigned aid%d for %d's row",
                    rid, __FUNCTION__, aid, responseAid, row);
            return PDN_FAILED;
        }

        /* Get 2nd parameter: Cmd */
        cmdFormat = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR parsing cmd with aid%d for %d's row",
                    rid, __FUNCTION__, aid, row);
            return PDN_FAILED;
        }

        if (strncmp(DC_ERROR, cmdFormat, strlen(DC_ERROR)) == 0) {
            /* Get 3rd paramter: Error*/
            if (p_cur->atTokHasmore()) {
                cause = p_cur->atTokNextint(&err);  // Not use currently.
                if (err < 0) {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing err cause for aid%d for %d's row",
                            rid, __FUNCTION__, aid, row);
                } else {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EPDN(addr) for aid%d returns err cause : %d for %d's row",
                            rid, __FUNCTION__, aid, cause, row);
                }
            }
            return PDN_FAILED;
        } else if (strncmp(DC_ADDRESS, cmdFormat, strlen(DC_ADDRESS)) == 0) {
            /* Get 3rd paramter: Address type*/
            if (p_cur->atTokHasmore()) {
                addrType = (ADDRESS_TYPE)p_cur->atTokNextint(&err);
                if (err < 0) {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR parsing address type with aid%d for %d's row",
                            rid, __FUNCTION__, aid, row);
                    return PDN_FAILED;
                }
            }

            /* Get 4th paramter: Address*/
            if (p_cur->atTokHasmore()) {
                address = p_cur->atTokNextstr(&err);
                if (err < 0) {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR parsing address with aid%d for %d's row",
                            rid, __FUNCTION__, aid, row);
                    return PDN_FAILED;
                }

                if (addrType == ADDRESS_IPV4) {
                    updateIpv4Address(aid, address);
                } else if (addrType == ADDRESS_IPV6_GLOBAL) {
                    char addressV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
                    convertIpv6Address(addressV6, address, 0);
                    updateIpv6Address(aid, ipv6Index, addressV6);
                    ipv6Index++;
                } else {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no matched address type (%s) with aid%d for %d's row",
                            rid, __FUNCTION__, RmcDcUtility::addrTypeToString(addrType), aid, row);
                }
            }
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no matched command format (%s) with aid%d for %d's row",
                    rid, __FUNCTION__, cmdFormat, aid, row);
            return PDN_FAILED;
        }
        row++;
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Response IP address [aid%d, IPv4Addr=%s, IPv6Addr=%s]",
            rid, __FUNCTION__, aid, mIsUserBuild ? "xxx" : getIpv4Address(aid),
            mIsUserBuild ? "xxx" : ipv6AddressToString(aid).string());

    return PDN_SUCCESS;
}

void RmcDcCommonReqHandler::initDataCallResponse(MTK_RIL_Data_Call_Response_v11* responses, int length) {
    for (int i = 0; i < length; i++) {
        memset(&responses[i], 0, sizeof(MTK_RIL_Data_Call_Response_v11));
        responses[i].status = PDP_FAIL_ERROR_UNSPECIFIED;
        responses[i].active = DATA_STATE_INACTIVE;
        responses[i].cid = INVALID_AID;
        responses[i].suggestedRetryTime = -1;
    }
}

void RmcDcCommonReqHandler::initAidList(int* list) {
    const int nPdnInfoSize = getPdnTableSize();
    for (int i = 0; i < nPdnInfoSize; i++) {
        list[i] = INVALID_AID;
    }
}

void RmcDcCommonReqHandler::createDataResponse(int transIntfId, int protocol,
        MTK_RIL_Data_Call_Response_v11* response, MTK_RIL_NetlinkIpv6Info *netInfo) {
    int rid = m_slot_id;
    char addressV4[MAX_IPV4_ADDRESS_LENGTH] = {0};
    char addressV6[MAX_NUM_IPV6_ADDRESS_NUMBER][MAX_IPV6_ADDRESS_LENGTH] = {{0}};
    char dnsV4[MAX_NUM_DNS_ADDRESS_NUMBER][MAX_IPV4_ADDRESS_LENGTH] = {{0}};
    char dnsV6[MAX_NUM_DNS_ADDRESS_NUMBER][MAX_IPV6_ADDRESS_LENGTH] = {{0}};
    char pcscf[MAX_PCSCF_NUMBER][MAX_IPV6_ADDRESS_LENGTH] = {{0}};
    int addressV4Length = 0;
    int addressV6Length = 0;
    int addressV6LinkLength = 0;
    int nAddressLength = 0;
    int v4DnsLength = 0;
    int v6DnsLength = 0;
    int nDnsesLen = 0;
    int pcscfLength = 0;
    PdnInfo *pdnInfo = NULL;
    PdnInfo pdn_info;
    memset(&pdn_info, 0, sizeof(pdn_info));
    const int nPdnInfoSize = getPdnTableSize();
    char *addressV6Link = NULL;
    int mtuV6 = 0;
    int useXfrm = 0;
    Vector<int> vDedicateBearerIdList;
    vDedicateBearerIdList.reserve(nPdnInfoSize);
    Vector<int>::iterator it;

    if (response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] response is null, return!",
                rid, __FUNCTION__);
        return;
    }

    NetAgentService *pNetAgentService = NetAgentService::getInstance();
    if (NULL == pNetAgentService && RfxRilUtils::getRilRunMode() != RIL_RUN_MODE_MOCK) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] NetAgentService is NULL", rid, __FUNCTION__);
        return;
    }

    for (int i = 0; i < nPdnInfoSize; i++) {
        pdn_info = getPdnInfo(i);
        int interfaceId = getInterfaceId(transIntfId);
        if ((interfaceId != INVALID_INTERFACE_ID) &&
                (pdn_info.transIntfId == transIntfId) &&
                !pdn_info.isDedicateBearer &&
                (pdn_info.active == DATA_STATE_ACTIVE)) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] copy pdn_info[%d] to data response for transIntfId=%d",
                    rid, __FUNCTION__, i, transIntfId);

            pdnInfo = &pdn_info;
            response->active = pdnInfo->active;

            if (RmcDcUtility::isXfrmSupport() && pdnInfo->rat == ACCESS_NW_TYPE_IWLAN
                    && !(pdn_info.profileId == RIL_DATA_PROFILE_IMS
                    || pdn_info.profileId == RIL_DATA_PROFILE_VENDOR_EMERGENCY)) {
                useXfrm = 1;
            }
            asprintf(&response->ifname, "%s%d", (useXfrm == 1 ? NETWORK_INTERFACE_XFRM :
                    (RfxRilUtils::getRilRunMode() == RIL_RUN_MODE_MOCK ? "ccmni" :
                            (pNetAgentService->getInterfaceName() != NULL ?
                            pNetAgentService->getInterfaceName() : "ccmni"))),
                    interfaceId);

            if (strlen(pdnInfo->addressV4) > 0
                    && strlen(pdnInfo->addressV4) < MAX_IPV4_ADDRESS_LENGTH
                    && (protocol == IPV4 || protocol == IPV4V6)) {
                strncpy(addressV4, pdnInfo->addressV4, MAX_IPV4_ADDRESS_LENGTH-1);
                addressV4Length += strlen(addressV4);
            }

            for (int j = 0; j < MAX_NUM_IPV6_ADDRESS_NUMBER; j++) {
                if (strlen(pdnInfo->addressV6[j]) > 0
                        && strlen(pdnInfo->addressV6[j]) < MAX_IPV6_ADDRESS_LENGTH
                        && (protocol == IPV6 || protocol == IPV4V6)) {
                    strncpy(addressV6[j], pdnInfo->addressV6[j], MAX_IPV6_ADDRESS_LENGTH-1);
                    addressV6Length += strlen(addressV6[j]);
                    if (j != 0 || addressV4Length != 0)
                        ++addressV6Length;  // add one space
                }
                if (netInfo != NULL && netInfo->interfaceId == interfaceId
                        && strncasecmp(netInfo->address, pdnInfo->addressV6[j],
                                strlen(netInfo->address)) == 0) {
                    asprintf(&addressV6Link, "&%d&%ld&%ld",
                            netInfo->flags, netInfo->deprecationTime, netInfo->expirationTime);
                    if (addressV6Link == NULL) {
                        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to allocate addressV6Link!",
                                rid, __FUNCTION__);
                    } else {
                        addressV6LinkLength += strlen(addressV6Link);
                    }
                }
            }

            for (int j = 0; j < MAX_NUM_DNS_ADDRESS_NUMBER; j++) {
                // If there is no IP address but with corresponding
                // DNS server, it may result in TCP/IP break down due
                // to DNS server algorithm from N, so need check if
                // have IP address when report DNS
                if (strlen(pdnInfo->dnsV4[j]) > 0
                        && strlen(pdnInfo->dnsV4[j]) < MAX_IPV4_ADDRESS_LENGTH
                        && addressV4Length > 0) {
                    strncpy(dnsV4[j], pdnInfo->dnsV4[j], MAX_IPV4_ADDRESS_LENGTH-1);
                    v4DnsLength += strlen(dnsV4[j]);
                    if (j != 0 || v6DnsLength != 0)
                        ++v4DnsLength;  // add one space
                }
                if (strlen(pdnInfo->dnsV6[j]) > 0
                        && strlen(pdnInfo->dnsV6[j]) < MAX_IPV6_ADDRESS_LENGTH
                        && addressV6Length > 0) {
                    strncpy(dnsV6[j], pdnInfo->dnsV6[j], MAX_IPV6_ADDRESS_LENGTH-1);
                    v6DnsLength += strlen(dnsV6[j]);
                    if (j != 0 || v4DnsLength != 0)
                        ++v6DnsLength;  // add one space
                }
            }

            for (int j = 0; j < MAX_PCSCF_NUMBER; j++) {
                if (strlen(pdnInfo->pcscf[j]) > 0
                        && strlen(pdnInfo->pcscf[j]) < MAX_IPV6_ADDRESS_LENGTH) {
                    strncpy(pcscf[j], pdnInfo->pcscf[j], MAX_IPV6_ADDRESS_LENGTH-1);
                    pcscfLength += strlen(pcscf[j]);
                    if (j != 0)
                        ++pcscfLength;  // add one space
                }
            }

            if (pdnInfo->mtuV6 == 0) {
                if (interfaceId >= 0 && interfaceId < MAX_CCMNI_INTERFACE_COUNT
                        && s_mtuV6[interfaceId] != 0) {
                    mtuV6 = s_mtuV6[interfaceId];
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] mtuv6=%d already set",
                            rid, __FUNCTION__, mtuV6);
                }
            } else {
                mtuV6 = pdnInfo->mtuV6;
            }

            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] mtu=%d, mtuV6=%d, accessNetwork=%d",
                    rid, __FUNCTION__, pdnInfo->mtu, mtuV6, pdnInfo->accessNetwork);
            response->mtu = pdnInfo->mtu;
            response->mtuV6 = mtuV6;
            response->accessNetwork = pdnInfo->accessNetwork;

            // default Qos
            if (pdnInfo->defaultQos != NULL && response->accessNetwork == E_UTRAN) {
                response->defaultQos = (RIL_Qos*)calloc(1, sizeof(RIL_Qos));
                RFX_ASSERT(response->defaultQos != NULL);
                response->defaultQos->eps.qci = pdnInfo->defaultQos->eps.qci;
                response->defaultQos->eps.uplink.maxBitrateKbps
                        = pdnInfo->defaultQos->eps.uplink.maxBitrateKbps;
                response->defaultQos->eps.uplink.guaranteedBitrateKbps
                        = pdnInfo->defaultQos->eps.uplink.guaranteedBitrateKbps;
                response->defaultQos->eps.downlink.maxBitrateKbps
                        = pdnInfo->defaultQos->eps.downlink.maxBitrateKbps;
                response->defaultQos->eps.downlink.guaranteedBitrateKbps
                        = pdnInfo->defaultQos->eps.downlink.guaranteedBitrateKbps;
            } else if (pdnInfo->defaultQos != NULL && response->accessNetwork == NG_RAN) {
                response->defaultQos = (RIL_Qos*)calloc(1, sizeof(RIL_Qos));
                RFX_ASSERT(response->defaultQos != NULL);
                response->defaultQos->nr.fiveQi = pdnInfo->defaultQos->nr.fiveQi;
                response->defaultQos->nr.uplink.maxBitrateKbps
                        = pdnInfo->defaultQos->nr.uplink.maxBitrateKbps;
                response->defaultQos->nr.uplink.guaranteedBitrateKbps
                        = pdnInfo->defaultQos->nr.uplink.guaranteedBitrateKbps;
                response->defaultQos->nr.downlink.maxBitrateKbps
                        = pdnInfo->defaultQos->nr.downlink.maxBitrateKbps;
                response->defaultQos->nr.downlink.guaranteedBitrateKbps
                        = pdnInfo->defaultQos->nr.downlink.guaranteedBitrateKbps;
                response->defaultQos->nr.qfi = pdnInfo->defaultQos->nr.qfi;
                response->defaultQos->nr.averagingWindowMs
                        = pdnInfo->defaultQos->nr.averagingWindowMs;
            }

            // QosSessions
            // collect dedicated bearers of this transinterface
            for (int j = 0; j < nPdnInfoSize; j++) {
                if (transIntfId == m_pPdnManager->getTransIntfId(j)
                        && m_pPdnManager->isDedicateBearer(j)
                        && getPdnInfo(j).qosSession != NULL
                        && (response->accessNetwork == E_UTRAN
                                || response->accessNetwork == NG_RAN)) {
                    vDedicateBearerIdList.push_back(j);
                }
            }
            response->qosSessionCount = vDedicateBearerIdList.size();

            if (vDedicateBearerIdList.size() > 0) {
                response->qosSessions = (RIL_QosSession*)calloc(1,
                        sizeof(RIL_QosSession) * vDedicateBearerIdList.size());
                RFX_ASSERT(response->qosSessions != NULL);

                int n = 0;
                PdnInfo tmpPdnInfo;
                for (it = vDedicateBearerIdList.begin();
                        it != vDedicateBearerIdList.end(); it++, n++) {
                    int aid = *it;
                    tmpPdnInfo = getPdnInfo(aid);

                    response->qosSessions[n].qosSessionId = tmpPdnInfo.qosSession->qosSessionId;
                    memcpy(&(response->qosSessions[n].qos),
                            &(tmpPdnInfo.qosSession->qos), sizeof(RIL_Qos));

                    response->qosSessions[n].qosFilterCount = tmpPdnInfo.qosSession->qosFilterCount;
                    response->qosSessions[n].qosFilters = (RIL_QosFilter*)calloc(1,
                            sizeof(RIL_QosFilter) * response->qosSessions[n].qosFilterCount);
                    RFX_ASSERT(response->qosSessions[n].qosFilters != NULL);
                    for (int k = 0; k < response->qosSessions[n].qosFilterCount; k++) {
                        const RIL_QosFilter* tmpFilter = &tmpPdnInfo.qosSession->qosFilters[k];
                        if (tmpFilter->localAddresses != NULL) {
                            int addLen = strlen(tmpFilter->localAddresses);
                            response->qosSessions[n].qosFilters[k].localAddresses =
                                    (char*) calloc(1, sizeof(char)*(addLen + 1));
                            RFX_ASSERT(response->qosSessions[n].qosFilters[k].localAddresses != NULL);
                            memcpy(response->qosSessions[n].qosFilters[k].localAddresses,
                                    tmpFilter->localAddresses, addLen);
                        }

                        if (tmpFilter->remoteAddresses != NULL) {
                            int addLen = strlen(tmpFilter->remoteAddresses);
                            response->qosSessions[n].qosFilters[k].remoteAddresses =
                                    (char*) calloc(1, sizeof(char)*(addLen + 1));
                            RFX_ASSERT(response->qosSessions[n].qosFilters[k].remoteAddresses != NULL);
                            memcpy(response->qosSessions[n].qosFilters[k].remoteAddresses,
                                    tmpFilter->remoteAddresses, addLen);
                        }

                        response->qosSessions[n].qosFilters[k].localPort.start
                                = tmpFilter->localPort.start;
                        response->qosSessions[n].qosFilters[k].localPort.end
                                = tmpFilter->localPort.end;
                        response->qosSessions[n].qosFilters[k].remotePort.start
                                = tmpFilter->remotePort.start;
                        response->qosSessions[n].qosFilters[k].remotePort.end
                                = tmpFilter->remotePort.end;
                        response->qosSessions[n].qosFilters[k].protocol
                                = tmpFilter->protocol;
                        response->qosSessions[n].qosFilters[k].typeOfService
                                = tmpFilter->typeOfService;
                        response->qosSessions[n].qosFilters[k].ipv6FlowLabel
                                = tmpFilter->ipv6FlowLabel;
                        response->qosSessions[n].qosFilters[k].ipSecSpi
                                = tmpFilter->ipSecSpi;
                        response->qosSessions[n].qosFilters[k].direction
                                = tmpFilter->direction;
                        response->qosSessions[n].qosFilters[k].precedence
                                = tmpFilter->precedence;
                    }
                }
            }

            response->handoverFailureMode = pdnInfo->handoverFailureMode;
            // Modem solution for WLAN handover so pduSessionId is no need.
            response->pduSessionId = 0;
            // slice info
            if (pdnInfo->optionalSliceInfo != NULL) {
                response->optionalSliceInfo = (RIL_SliceInfo*)calloc(1, sizeof(RIL_SliceInfo));
                RFX_ASSERT(response->optionalSliceInfo != NULL);
                memcpy(response->optionalSliceInfo, pdnInfo->optionalSliceInfo,
                        sizeof(RIL_SliceInfo));
            }

            // traffic descriptors
            response->tdCount = pdnInfo->tdCount;
            if (response->tdCount > 0 && pdnInfo->TDs != NULL) {
                response->TDs = (RIL_TrafficDescriptor*)calloc(1,
                        sizeof(RIL_TrafficDescriptor) * response->tdCount);
                RFX_ASSERT(response->TDs != NULL);
            }
            for (int j = 0; j < response->tdCount; j++) {
                if (pdnInfo->TDs == NULL || response->TDs == NULL) {
                    // should not happen
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] pdnInfo->TDs or response->TDs is NULL",
                            rid, __FUNCTION__);
                    break;
                }
                if (pdnInfo->TDs[j].dnn != NULL && strlen(pdnInfo->TDs[j].dnn) > 0) {
                    int len = strlen(pdnInfo->TDs[j].dnn);
                    response->TDs[j].dnn = (char*)calloc(1, sizeof(char) * (len + 1));
                    RFX_ASSERT(response->TDs[j].dnn != NULL);
                    memcpy(response->TDs[j].dnn, pdnInfo->TDs[j].dnn, len);
                }
                response->TDs[j].osAppIdLen = pdnInfo->TDs[j].osAppIdLen;
                if (response->TDs[j].osAppIdLen > 0) {
                    response->TDs[j].osAppId = (uint8_t*)calloc(1,
                            sizeof(uint8_t) * response->TDs[j].osAppIdLen);
                    RFX_ASSERT(response->TDs[j].osAppId != NULL);
                    memcpy(response->TDs[j].osAppId, pdnInfo->TDs[j].osAppId,
                            response->TDs[j].osAppIdLen);
                }
            }
        } else if (pdn_info.transIntfId == transIntfId) {
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] not need to create pdn_info[%d] for transIntfId=%d"
                    " because pdn_info{isDedicateBearer=%d, active=%d} is not satisfied",
                    rid, __FUNCTION__, i, transIntfId, pdn_info.isDedicateBearer,
                    pdn_info.active);
        }
    }

    response->status = PDP_FAIL_NONE;
    response->cid = transIntfId;

    nAddressLength = addressV4Length + addressV6Length + addressV6LinkLength;
    response->addresses = (char*)calloc(1, (nAddressLength * sizeof(char)) + 1);
    RFX_ASSERT(response->addresses != NULL);
    memset(response->addresses, 0, (nAddressLength * sizeof(char)) + 1);
    response->gateways = (char*)calloc(1, (nAddressLength * sizeof(char)) + 1);
    RFX_ASSERT(response->gateways != NULL);
    memset(response->gateways, 0, (nAddressLength * sizeof(char)) + 1);

    if (addressV4Length > 0) {
        strncat(response->addresses + strlen(response->addresses), addressV4,
            nAddressLength - strlen(response->addresses));
        strncat(response->gateways + strlen(response->gateways), addressV4,
            nAddressLength - strlen(response->gateways));
    }
    for (int i = 0; i < MAX_NUM_IPV6_ADDRESS_NUMBER; i++) {
        if (addressV6Length > 0 && strlen(addressV6[i]) > 0) {
            int currentLength = strlen(response->addresses);
            if (currentLength > 0) {
                strncat(response->addresses + currentLength, " ", nAddressLength - currentLength);
            }
            strncat(response->addresses + strlen(response->addresses), addressV6[i],
                    nAddressLength - strlen(response->addresses));

            if (addressV6LinkLength > 0 && addressV6Link != NULL && netInfo != NULL &&
                    strncasecmp(netInfo->address, addressV6[i], strlen(netInfo->address)) == 0) {
                strncat(response->addresses + strlen(response->addresses), addressV6Link,
                    nAddressLength - strlen(response->addresses));
            }

            currentLength = strlen(response->gateways);
            if (currentLength > 0) {
                strncat(response->gateways + currentLength, " ", nAddressLength - currentLength);
            }
            if (isIpv6Global(addressV6[i])) {
                strncat(response->gateways + strlen(response->gateways), UNSPECIFIED_IPV6_GATEWAY,
                        nAddressLength - strlen(response->gateways));
            } else {
                strncat(response->gateways + strlen(response->gateways), addressV6[i],
                    nAddressLength - strlen(response->gateways));
            }
        }
    }

    if (addressV4Length > 0 && addressV6Length > 0) {
        asprintf(&response->type, SETUP_DATA_PROTOCOL_IPV4V6);
    } else if (addressV6Length > 0) {
        asprintf(&response->type, SETUP_DATA_PROTOCOL_IPV6);
    } else {
        asprintf(&response->type, SETUP_DATA_PROTOCOL_IP);
    }

    nDnsesLen = v4DnsLength + v6DnsLength;
    response->dnses = (char*)calloc(1, (nDnsesLen * sizeof(char)) + 1);
    RFX_ASSERT(response->dnses != NULL);
    memset(response->dnses, 0, (nDnsesLen * sizeof(char)) + 1);

    for (int i = 0; i < MAX_NUM_DNS_ADDRESS_NUMBER; i++) {
        if (v6DnsLength > 0 && strlen(dnsV6[i]) > 0) {
            int currentLength = strlen(response->dnses);
            if (currentLength > 0) {
                strncat(response->dnses + currentLength, " ", nDnsesLen - currentLength);
            }
            strncat(response->dnses + strlen(response->dnses), dnsV6[i],
                    nDnsesLen - strlen(response->dnses));
        }
    }
    for (int i = 0; i < MAX_NUM_DNS_ADDRESS_NUMBER; i++) {
        if (v4DnsLength > 0 && strlen(dnsV4[i]) > 0) {
            int currentLength = strlen(response->dnses);
            if (currentLength > 0) {
                strncat(response->dnses + currentLength, " ", nDnsesLen - currentLength);
            }
            strncat(response->dnses + strlen(response->dnses), dnsV4[i],
                    nDnsesLen - strlen(response->dnses));
        }
    }

    response->pcscf= (char*)calloc(1, pcscfLength * sizeof(char) + 1);
    RFX_ASSERT(response->pcscf != NULL);
    memset(response->pcscf, 0, pcscfLength * sizeof(char) + 1);
    for (int i = 0; i < MAX_PCSCF_NUMBER; i++) {
        if (pcscfLength > 0 && strlen(pcscf[i]) > 0) {
            int currentLength = strlen(response->pcscf);
            if (currentLength > 0) {
                strncat(response->pcscf + currentLength, " ", pcscfLength - currentLength);
                currentLength = strlen(response->pcscf);
            }
            strncat(response->pcscf + currentLength, pcscf[i], pcscfLength - currentLength);
        }
    }

    FREEIF(addressV6Link);
}

String8 RmcDcCommonReqHandler::responsesToString(MTK_RIL_Data_Call_Response_v11* responses, int num) {
    int rid = m_slot_id;
    String8 cmd("");

    if (num == 0 || responses == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] the number of data call responses is %d "
                "(if number is not 0, that means data call responses is null)",
                rid, __FUNCTION__, num);
        return String8::format("");
    }

    cmd.append(String8::format("DataCallResponse <-"));

    for (int i = 0; i < num; i++) {
        cmd.append(String8::format(" {status=%d, suggestedRetryTime=%d, transIntfId=%d, active=%d,"
                " type=%s, ifname=%s, addresses=%s, dnses=%s, gateways=%s, pcscf=%s, mtu=%d}",
                (&responses[i])->status,
                (&responses[i])->suggestedRetryTime,
                (&responses[i])->cid,
                (&responses[i])->active,
                (&responses[i])->type,
                (&responses[i])->ifname,
                mIsUserBuild ? "xxx" : (&responses[i])->addresses,
                mIsUserBuild ? "xxx" : (&responses[i])->dnses,
                mIsUserBuild ? "xxx" : (&responses[i])->gateways,
                mIsUserBuild ? "xxx" : (&responses[i])->pcscf,
                (&responses[i])->mtu));
    }
    return cmd;
}

String8 RmcDcCommonReqHandler::ipv6AddressToString(int aid) {
    int rid = m_slot_id;
    String8 cmd("");

    // Check if AID is out of bound.
    if (!validateAid(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid=%d is out of bound",
                rid, __FUNCTION__, aid);
        return String8::format("");
    }

    for (int i = 0; i < MAX_NUM_IPV6_ADDRESS_NUMBER; i++) {
        if (i != 0) {
            cmd.append(String8::format("%s", ", "));
        }
        cmd.append(String8::format("%s", getIpv6Address(aid, i)));
    }

    return cmd;
}

void RmcDcCommonReqHandler::freeDataResponse(MTK_RIL_Data_Call_Response_v11* response) {
    if (response->ifname != NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] type=%s, ifname=%s, addresses=%s, dnses=%s, gateways=%s, pcscf=%s",
                m_slot_id,
                __FUNCTION__,
                response->type,
                response->ifname,
                mIsUserBuild ? "xxx" : response->addresses,
                mIsUserBuild ? "xxx" : response->dnses,
                mIsUserBuild ? "xxx" : response->gateways,
                mIsUserBuild ? "xxx" : response->pcscf);
    }

    FREEIF(response->type);
    FREEIF(response->ifname);
    FREEIF(response->addresses);
    FREEIF(response->gateways);
    FREEIF(response->dnses);
    FREEIF(response->pcscf);
    FREEIF(response->defaultQos);
    FREEIF(response->optionalSliceInfo);

    if (response->qosSessionCount > 0) {
        for (int i = 0; i < response->qosSessionCount; i++) {
            if (response->qosSessions[i].qosFilterCount > 0) {
                FREEIF(response->qosSessions[i].qosFilters);
            }
        }
        FREEIF(response->qosSessions);
    }

    if (response->tdCount > 0) {
        for (int i = 0; i < response->tdCount; i++) {
            FREEIF(response->TDs[i].dnn);
            FREEIF(response->TDs[i].osAppId);
        }
        FREEIF(response->TDs);
    }
}

int RmcDcCommonReqHandler::convertIpv6Address(char* output, char* input, int isLinkLocal) {
    // input : 32.1.13.184.0.0.0.3.61.48.97.182.50.254.113.251
    // output : 2001:0DB8:0000:0003:3D30:61B6:32FE:71FB
    int rid = m_slot_id;
    char *p = NULL;
    char *address = NULL;
    int value = 0;
    int len = 0;
    int count = 8;
    char tmpOutput[MAX_IPV6_ADDRESS_LENGTH] = {0};
    int n = 0;
    char *saveptr = NULL;

    if (output == NULL || input == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] output or input is NULL", rid, __FUNCTION__);
        return -1;
    }

    if (isLinkLocal == 1) {
        strncpy(output, IPV6_PREFIX, MAX_IPV6_ADDRESS_LENGTH - 1);
    } else {
        strncpy(output, NULL_ADDR, MAX_IPV6_ADDRESS_LENGTH - 1);
    }

    address = strsep(&input, "/");
    p = strtok_r(address, ".", &saveptr);

    //Skip the 64 bit of this PDP address if this pdp adress is a local link address
    if (isLinkLocal == 1) {
        while (count && p) {
            p = strtok_r(NULL, ".", &saveptr);
            count--;
        }
    }

    while (p) {
        memset(tmpOutput, 0, MAX_IPV6_ADDRESS_LENGTH);
        value = atoi(p);
        n = snprintf(tmpOutput, MAX_IPV6_ADDRESS_LENGTH, "%02X", value);
        if (n < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] snprintf errorl", rid, __FUNCTION__);
            return -1;
        }
        strncat(output + strlen(output), tmpOutput, MAX_IPV6_ADDRESS_LENGTH-strlen(output)-1);
        p = strtok_r(NULL, ".", &saveptr);
        if (p == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] The format of IP address is illegal", rid, __FUNCTION__);
            return -1;
        }
        memset(tmpOutput, 0, MAX_IPV6_ADDRESS_LENGTH);
        value = atoi(p);
        n = snprintf(tmpOutput, MAX_IPV6_ADDRESS_LENGTH, "%02X:", value);
        if (n < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] snprintf errorl", rid, __FUNCTION__);
            return -1;
        }
        strncat(output + strlen(output), tmpOutput, MAX_IPV6_ADDRESS_LENGTH-strlen(output)-1);
        p = strtok_r(NULL, ".", &saveptr);
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] before convertIpv6Address: %s", rid, __FUNCTION__,
            mIsUserBuild ? "xxx" : output);
    len = strlen(output);
    output[len-1] = '\0';

    if (input != NULL) {
        memset(tmpOutput, 0, MAX_IPV6_ADDRESS_LENGTH);
        n = snprintf(tmpOutput, MAX_IPV6_ADDRESS_LENGTH, "/%s", input);
        if (n < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] snprintf errorl", rid, __FUNCTION__);
            return -1;
        }
        strncat(output + strlen(output), tmpOutput, MAX_IPV6_ADDRESS_LENGTH-strlen(output)-1);
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] convertIpv6Address: %s", rid, __FUNCTION__,
            mIsUserBuild ? "xxx" : output);

    return 0;
}

int RmcDcCommonReqHandler::convertAddrAndSubnetMask(char* output, const char* input) {
    /* Input(:
           "a1.a2.a3.a4.m1.m2.m3.m4" for IPv4
           or
           "a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.m1.m2.m3.m4.m5.m6.m7
           .m8.m9.m10.m11.m12.m13.m14.m15.m16", for IPv6.
       Output:
           192.0.1.11/16
           2001:db8::1/64
       note: ax and mx are 0-255 numeric, defined in 27.007
    */
    if (output == NULL || input == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] output or input is NULL", m_slot_id, __FUNCTION__);
        return -1;
    }
    int n = 0;
    int i = 0;
    int inputLen = strlen(input);
    int delimCount = 0;
    int midDelimPos = 0;
    int IpType = -1;
    int addrPartLen = 0;
    int subnetMaskCount = 0;
    char *tempIn = (char*) calloc(1, sizeof(char) * (inputLen+1));
    RFX_ASSERT(tempIn != NULL);
    memcpy(tempIn, input, inputLen);
    char *subNetMask = NULL;
    char *addrV6 = NULL;
    const char dotDelim = '.';
    for (i = 0; tempIn[i] != '\0'; i++) {
        if (dotDelim == tempIn[i]) {
            delimCount++;
        }
    }
    IpType = delimCount == 7 ? IPV4 : (delimCount == 31 ? IPV6 : -1);
    if (IpType == -1) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] illegal format, delimCount:%d",
                m_slot_id, __FUNCTION__, delimCount);
        FREEIF(tempIn);
        return -1;
    }

    midDelimPos = (delimCount +1)/2;
    delimCount = 0;
    for (i = 0; tempIn[i] != '\0'; i++) {
        if (dotDelim == tempIn[i] && ++delimCount == midDelimPos) {
            break;
        }
        addrPartLen++;
    }
    subNetMask = (char*) calloc(1, sizeof(char) * (inputLen-addrPartLen) + 1);
    RFX_ASSERT(subNetMask != NULL);
    memcpy(subNetMask, input+addrPartLen+1, inputLen-addrPartLen-1);

    subnetMaskCount  = calculateSubnetMaskNumber(subNetMask);

    memset(tempIn+addrPartLen, 0, inputLen-addrPartLen);
    if (IpType == IPV6) {
        addrV6 = (char*) calloc(1, sizeof(char) * MAX_IPV6_ADDRESS_LENGTH);
        RFX_ASSERT(addrV6 != NULL);
        int isLocal = strncasecmp("254.128", tempIn, strlen("254.128")) == 0 ? 1 : 0;
        convertIpv6Address(addrV6, tempIn, isLocal);
        FREEIF(tempIn);
        tempIn = addrV6;
    }
    if (subnetMaskCount != -1) {
        n = snprintf(output,
                IpType == IPV4 ? MAX_IPV4_ADDR_AND_MASK_LENGTH : MAX_IPV6_ADDR_AND_MASK_LENGTH,
                "%s%s%d", tempIn, "/", subnetMaskCount);
    } else {
        n = snprintf(output,
                IpType == IPV4 ? MAX_IPV4_ADDR_AND_MASK_LENGTH : MAX_IPV6_ADDR_AND_MASK_LENGTH,
                "%s", tempIn);
    }

    if (n < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] snprintf error", m_slot_id, __FUNCTION__);
    }

    FREEIF(tempIn);
    FREEIF(subNetMask);
    return 0;
}

int RmcDcCommonReqHandler::calculateSubnetMaskNumber(char* input) {
    if (input == NULL || strlen(input) == 0) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] illegal param", m_slot_id, __FUNCTION__);
        return -1;
    }
    int inputLen = strlen(input);
    char* tmpInput = (char*) calloc(1, sizeof(char) * (inputLen + 1));
    RFX_ASSERT(tmpInput != NULL);
    memcpy(tmpInput, input, inputLen);
    int count = 0;
    int i = 0;
    int bitNum = 7;
    int ipDec = 0;
    long maxValue = 255;
    char* ipStr = NULL;
    char* saveptr = NULL;
    const char* delim = ".";
    ipStr = strtok_r(tmpInput, delim, &saveptr);
    while(ipStr != NULL && strlen(ipStr) > 0){
        ipDec = atoi(ipStr);
        if (ipDec > maxValue) {
            FREEIF(tmpInput);
            return -1;
        }
        for (i = bitNum; i >= 0 && ipDec/(1 << i) >= 1; ipDec -= (1<<i), count++, i--) {}
        ipStr = strtok_r(NULL, delim, &saveptr);
    }
    FREEIF(tmpInput);
    return (count > 0) ? count : -1;
}

bool RmcDcCommonReqHandler::isIpv6Global(char *ipv6Addr) {
    int rid = m_slot_id;
    char *address = NULL;
    String8 cmd("");

    address = strsep(&ipv6Addr, "/");
    cmd.append(String8::format("{IPv6 address:%s, IPv6 prefix length:%s}", address, ipv6Addr));

    if (address != NULL) {
        struct sockaddr_in6 sa;
        int ret = 0;

        if (strncasecmp("FE80", address, strlen("FE80")) == 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] not global -> %s", rid, __FUNCTION__, cmd.string());
            return false;
        }

        // ret: -1, error occurs, ret: 0, invalid address, ret: 1, success;
        ret = inet_pton(AF_INET6, address, &(sa.sin6_addr));
        if (ret <= 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] inet_pton ret: %d -> %s",
                    rid, __FUNCTION__, ret, cmd.string());
            return false;
        }

        if (IN6_IS_ADDR_MULTICAST(&sa.sin6_addr)) {
            if (IN6_IS_ADDR_MC_GLOBAL(&sa.sin6_addr)) {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] global -> %s", rid, __FUNCTION__, cmd.string());
                return true;
            } else {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] not global -> %s", rid, __FUNCTION__, cmd.string());
            }
        } else {
            if (IN6_IS_ADDR_LINKLOCAL(&sa.sin6_addr)) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] link-local -> %s", rid, __FUNCTION__, cmd.string());
            } else if (IN6_IS_ADDR_SITELOCAL(&sa.sin6_addr)) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] site-local -> %s", rid, __FUNCTION__, cmd.string());
            } else if (IN6_IS_ADDR_V4MAPPED(&sa.sin6_addr)) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] v4mapped -> %s", rid, __FUNCTION__, cmd.string());
            } else if (IN6_IS_ADDR_V4COMPAT(&sa.sin6_addr)) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] v4compat -> %s", rid, __FUNCTION__, cmd.string());
            } else if (IN6_IS_ADDR_LOOPBACK(&sa.sin6_addr)) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] host -> %s", rid, __FUNCTION__, cmd.string());
            } else if (IN6_IS_ADDR_UNSPECIFIED(&sa.sin6_addr)) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] unspecified -> %s", rid, __FUNCTION__, cmd.string());
            } else if (_IN6_IS_ULA(&sa.sin6_addr)) {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] take uni-local as global -> %s",
                        rid, __FUNCTION__, cmd.string());
                return true;
            } else {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] global -> %s", rid, __FUNCTION__,
                        mIsUserBuild ? "xxx" : cmd.string());
                return true;
            }
        }
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] input ipv6 address is null!!", rid, __FUNCTION__);
    }
    return false;
}

void RmcDcCommonReqHandler::resetPco(int aid) {
    String8 cmd("");
    sp<RfxAtResponse> p_response;
    int rid = m_slot_id;

    cmd.append(String8::format("AT+EGPCO=%d", aid));
    p_response = atSendCommand(cmd);

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return;
    }

    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d] %s returns ERROR", rid, cmd.string());
    }
}

void RmcDcCommonReqHandler::setPco(int aid, const char *option, const char *content) {
    String8 cmd("");
    sp<RfxAtResponse> p_response;
    int rid = m_slot_id;

    cmd.append(String8::format("AT+EGPCO=%d,\"%s\",\"%s\"", aid, option, content));
    p_response = atSendCommand(cmd);

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return;
    }

    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d] %s returns ERROR", rid, cmd.string());
    }
}

void RmcDcCommonReqHandler::setPco(int aid, const char *buf) {
    String8 cmd("");
    sp<RfxAtResponse> p_response;
    int rid = m_slot_id;

    cmd.append(String8::format("AT+EGPCO=%d,%s", aid, buf));
    p_response = atSendCommand(cmd);

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return;
    }

    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d] %s returns ERROR", rid, cmd.string());
    }
}

void RmcDcCommonReqHandler::getPco(int aid, const char *option, Vector<String8>& vContent) {
    // for PDN_AP_INITIATED case, apn and its index are not needed to get PCO, and are NULL
    getPco(PDN_AP_INITIATED, aid, NULL, option, vContent);
}

void RmcDcCommonReqHandler::getPco(int apnidx, const char* apn,
        const char *option, Vector<String8>& vContent) {
    // for PDN_MD_INITIATED case, apnidx/apn/proto in previous URC are used to get PCO
    getPco(PDN_MD_INITIATED, apnidx, apn, option, vContent);
}

void RmcDcCommonReqHandler::getPco(PDN_INITIATOR who, int idx, const char* apn,
        const char *option, Vector<String8>& vContent) {
    String8 cmd("");
    sp<RfxAtResponse> p_response;
    RfxAtLine *p_cur = NULL;
    int err = 0;
    int mdRspIdx = INVALID_AID;
    int rid = m_slot_id;
    char *mdOption = NULL;
    char *mdContent = NULL;

    if (option == NULL) {
        // obtain all PCO values from modem
        if (who == PDN_AP_INITIATED) {
            cmd.append(String8::format("AT+EGPCORDP=%d", idx));
        } else if (who == PDN_MD_INITIATED) {
            cmd.append(String8::format("AT+BGPCORDP=\"%s\",%d", apn, idx));
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] requested PDN format error!",
                    rid, __FUNCTION__);
            return;
        }
    } else {
        // obtain specific PCO value from modem
        if (who == PDN_AP_INITIATED) {
            cmd.append(String8::format("AT+EGPCORDP=%d,\"%s\"", idx, option));
        } else if (who == PDN_MD_INITIATED) {
            cmd.append(String8::format("AT+BGPCORDP=\"%s\",%d,\"%s\"", apn, idx, option));
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] requested PDN format error!",
                    rid, __FUNCTION__);
            return;
        }
    }

    if (who == PDN_AP_INITIATED) {
        p_response = atSendCommandMultiline(cmd, "+EGPCORDP:");
    } else if (who == PDN_MD_INITIATED) {
        p_response = atSendCommandMultiline(cmd, "+BGPCORDP:");
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] requested PDN format error!",
            rid, __FUNCTION__);
    }

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return;
    }

    if (p_response->isAtResponseFail()) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d] %s returns ERROR", rid, cmd.string());
        return;
    }

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        p_cur->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                    rid, __FUNCTION__);
            return;
        }

        mdRspIdx = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get idx",
                    rid, __FUNCTION__);
            return;
        }

        mdOption = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get option or it's different to MD assigned",
                    rid, __FUNCTION__);
            return;
        }

        mdContent = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get content",
                    rid, __FUNCTION__);
            return;
        }
        vContent.push_back(String8::format("%s,%s", mdOption, mdContent));
    }
}

bool RmcDcCommonReqHandler::isDataAllowed(const char* pReqApn) {
    int bearer_bitmask = 0;
    bool bSupportWifiBearer = false;
    // 0 - Not registered
    // 1 - Registered
    int nWfcState = getMclStatusManager(m_slot_id)->getIntValue(RFX_STATUS_KEY_WFC_STATE, 0);
    // 0 - Not registered
    // 1 - Registered, home network
    // 2 - Not registered, but MT is currently searching a new operator to register
    // 3 - Registration denied
    // 4 - Unknown
    // 5 - Registered, roaming
    RfxNwServiceState ss = getMclStatusManager(m_slot_id)->getServiceStateValue(
            RFX_STATUS_KEY_SERVICE_STATE, RfxNwServiceState(0, 0, 0, 0));
    int nPsState = ss.getRilDataRegState();
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] nWfcState = %d, nPsState = %d, roaming data = %d",
            m_slot_id, __FUNCTION__, nWfcState, nPsState,
            s_dataSetting_resend[m_slot_id][ROAMING_DATA]);

    if (1 == nWfcState) {
        for (int i = 0; i < s_nLastReqNum[m_slot_id]; i++) {
            if (0 == strcmp(pReqApn, s_LastApnTable[m_slot_id][i].apn)) {
                bearer_bitmask = s_LastApnTable[m_slot_id][i].bearerBitmask;
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] bearer_bitmask = %d",
                        m_slot_id, __FUNCTION__, bearer_bitmask);
                if (0 == bearer_bitmask
                        ||((bearer_bitmask & (1 << (RADIO_TECH_IWLAN - 1))) != 0)) {
                    bSupportWifiBearer = true;
                }
                break;
            }
        }
        if (!bSupportWifiBearer && 5 == nPsState &&
                0 == s_dataSetting_resend[m_slot_id][ROAMING_DATA]) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] the PS state is roaming but data roaming is off",
                    m_slot_id, __FUNCTION__);
            return false;
        }
    }
    return true;
}

int RmcDcCommonReqHandler::getInterfaceId(int transIntfId) {
    if (0 <= transIntfId) {
        return transIntfId % TRANSACTION_ID_OFFSET;
    }
    return INVALID_INTERFACE_ID;
}

void RmcDcCommonReqHandler::requestClearAllPdnInfo(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s]", m_slot_id, __FUNCTION__);
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->clearAllPdnInfo();
}

void RmcDcCommonReqHandler::requestQueryPco(int idx, int ia, const char* apn, const char* iptype) {
    String8 support = getMclStatusManager()->getString8Value(
            RFX_STATUS_KEY_PCO_DATA_FEATURE_SUPPORTED);
    if (strcmp(support.string(), "1") != 0) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] PCO data not supported", m_slot_id, __FUNCTION__);
        return;
    }

    // if it is ia, idx is not aid but apn index, must not check it!
    if (ia == 0 && getPdnActiveStatus(idx) == DATA_STATE_INACTIVE) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no need to check inactive aid %d",
                m_slot_id, __FUNCTION__, idx);
        return;
    }

    Vector<String8> contents;
    if (ia) {
        getPco(idx, apn, NULL, contents);
    } else {
        getPco(idx, NULL, contents);
    }

    if (contents.size() == 0) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] query PCO return: no contents",
                m_slot_id, __FUNCTION__);
        return;
    }

    for (Vector<String8>::iterator it = contents.begin(); it != contents.end(); it++) {
        if (it == NULL) return;
        std::string pcoContent((*it).string());
        std::string pcoIe("");
        std::string pcoValue("");
        int nContentLength = 1;
        sp<RfxMclMessage> urcMsg = NULL;

        int delimiterA = pcoContent.find(",");
        if (delimiterA < 0) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] query PCO return: content's format missing ','",
                    m_slot_id, __FUNCTION__);
            continue;
        }

        int delimiterB = pcoContent.find(":");
        if (delimiterB < 0) {
            pcoValue = pcoContent.substr(delimiterA + 1, std::string::npos);
        } else {
            pcoValue = pcoContent.substr(delimiterB + 1, std::string::npos);
        }

        pcoIe = pcoContent.substr(0, delimiterA);
        if (pcoIe.find("FF00") != std::string::npos || pcoIe.find("FF03") != std::string::npos) {
            nContentLength = 1;
        } else {
            nContentLength = pcoValue.size();
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] not FF00 or FF03", m_slot_id, __FUNCTION__);
        }

        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] idx:%d has PCO, content: %s, val: %s",
                m_slot_id, __FUNCTION__, idx, pcoContent.c_str(), pcoValue.c_str());

        if (ia) {
            RIL_PCO_Data_attached pcoDataIa;
            pcoDataIa.cid = 0; // ia apn not yet known by framework/rild, just fill 0.
            pcoDataIa.apn_name = (char*) apn;
            pcoDataIa.bearer_proto = (char*) iptype;
            pcoDataIa.pco_id = std::stoi(pcoIe.c_str(), NULL, 16);
            pcoDataIa.contents_length = nContentLength;
            pcoDataIa.contents = (char*) pcoValue.c_str();
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] PCO cid %d, apn %s, proto %s, pco_id %d",
                    m_slot_id, __FUNCTION__, pcoDataIa.cid, pcoDataIa.apn_name,
                    pcoDataIa.bearer_proto, pcoDataIa.pco_id);

            urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_PCO_DATA_AFTER_ATTACHED,
                    m_slot_id, RfxPcoIaData(&pcoDataIa, sizeof(pcoDataIa)));
        } else {
            int tid = getTransIntfId(idx);
            int protoBitMask = 0; // 1:v4, 2:v6, 4:v4v6
            for (int i = 0; i < getPdnTableSize(); i++) {
                if (tid == getTransIntfId(i)) {
                    std::string ipv4 = std::string(getIpv4Address(i));
                    std::string ipv6 = std::string(getIpv6Address(i));
                    ipv4.erase(std::remove_if(begin(ipv4), end(ipv4), ::isspace), end(ipv4));
                    ipv6.erase(std::remove_if(begin(ipv6), end(ipv6), ::isspace), end(ipv6));
                    protoBitMask |= (ipv4.size() > 0 && ipv6.size() == 0) ? 1 << IPV4 :
                            (ipv4.size() == 0 && ipv6.size() > 0) ? 1 << IPV6 :
                            (ipv4.size() > 0 && ipv6.size() > 0) ? 1 << IPV4V6 : 1 << IPV4;
                }
            }
            std::string proto;
            switch (protoBitMask) {
                case 1 << IPV4:
                    proto = SETUP_DATA_PROTOCOL_IP;
                    break;
                case 1 << IPV6:
                    proto = SETUP_DATA_PROTOCOL_IPV6;
                    break;
                case (1 << IPV4 | 1 << IPV6):
                case 1 << IPV4V6:
                    proto = SETUP_DATA_PROTOCOL_IPV4V6;
                    break;
                default:
                    proto = SETUP_DATA_PROTOCOL_IP;
                    break;
            }

            RIL_PCO_Data pcoData;
            pcoData.cid = getTransIntfId(idx);
            pcoData.bearer_proto = (char*)proto.c_str();
            pcoData.pco_id = std::stoi(pcoIe.c_str(), NULL, 16);
            pcoData.contents_length = nContentLength;
            pcoData.contents = (char*)pcoValue.c_str();
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] PCO cid %d, proto %s, pcoid %d, contents %s",
                    m_slot_id, __FUNCTION__, pcoData.cid, pcoData.bearer_proto, pcoData.pco_id,
                    pcoData.contents);

            urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_PCO_DATA, m_slot_id,
                    RfxPcoData(&pcoData, sizeof(pcoData)));
        }

        if (urcMsg == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to obtainUrc!", m_slot_id, __FUNCTION__);
            continue;
        }

        String8 pcoStatusKey = String8(pcoIe.c_str());
        pcoStatusKey.append(":");
        pcoStatusKey.append(pcoValue.c_str());
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_PCO_STATUS, pcoStatusKey);
        responseToTelCore(urcMsg);
    }
}

int RmcDcCommonReqHandler::confirmPdnUsage(const int aid, const bool bUsed, int sscMode) {
    int rid = m_slot_id;
    String8 cmd("");
    sp<RfxAtResponse> p_response;

    // Check if AID is out of bound.
    if (!validateAid(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid=%d is out of bound",
                rid, __FUNCTION__, aid);
        return CME_ERROR_NON_CME;
    }

    // AT+EAPNACT=<state>,<aid>;
    // <state>: 2 - NOT to use this PDN and NOT to receive "+CGEV" URCs for the corresponding bearers;
    // <state>: 3 - use this PDN and to receive "+CGEV" URCs for the corresponding bearers;
    // <state>: 4 - NOT to use this SSC mode3 PDU and NOT to receive "+CGEV" URCs for the corresponding bearers;
    // <state>: 5 - use this SSC mode3 PDU and to receive "+CGEV" URCs for the corresponding bearers;
    if (bUsed) {
        if (sscMode == SSC_MODE3) {
            cmd.append(String8::format("AT+EAPNACT=5,%d", aid));
        } else {
            cmd.append(String8::format("AT+EAPNACT=3,%d", aid));
        }
    } else {
        if (sscMode == SSC_MODE3) {
            cmd.append(String8::format("AT+EAPNACT=4,%d", aid));
        } else {
            cmd.append(String8::format("AT+EAPNACT=2,%d", aid));
        }
    }
    p_response = atSendCommand(cmd);

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return CME_ERROR_NON_CME;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() == 0) {
            int cause = p_response->atGetCmeError();
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] CME CAUSE : %d",
                    rid, __FUNCTION__, cause);
            return cause;
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] CME CAUSE : CME_UNKNOWN",
                    rid, __FUNCTION__);
            return CME_UNKNOWN;
        }
    }

    return CME_SUCCESS;
}

int RmcDcCommonReqHandler::updatePdnDeactInfo(const int aid) {
    int rid = m_slot_id;
    int err = 0;
    RfxAtLine *pLine = NULL;
    char *cmdFormat = NULL;
    int rspAid = INVALID_AID;
    int reason = NO_REASON;
    int rat = 2454;  // default value.
    int cause = 0;
    sp<RfxAtResponse> p_response;

    // Check if AID is out of bound.
    if (!validateAid(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid=%d is out of bound",
                rid, __FUNCTION__, aid);
        return PDN_FAILED;
    }

    // AT+EPDN=<aid>,"deact_info"
    // Response:
    //    +EPDN:<aid>,"deact_info",<reason>,<rat type>
    //    +EPDN:<aid>,"err",<err>
    p_response = atSendCommandSingleline(String8::format("AT+EPDN=%d,\"deact_info\"", aid), "+EPDN:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", rid, __FUNCTION__);
        } else {
            cause = p_response->atGetCmeError();
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EPDN(deact_info) for aid%d returns cme cause : %d",
                    rid, __FUNCTION__, aid, cause);
        }
        return PDN_FAILED;
    }

    pLine = p_response->getIntermediates();

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get intermediate results!",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    rspAid = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    if (rspAid != aid) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid%d is different to MD assigned aid%d",
                rid, __FUNCTION__, aid, rspAid);
        return PDN_FAILED;
    }

    cmdFormat = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cmd",
                rid, __FUNCTION__);
        return PDN_FAILED;
    }

    if (strncmp(DC_ERROR, cmdFormat, strlen(DC_ERROR)) == 0) {
        if (pLine->atTokHasmore()) {
            cause = pLine->atTokNextint(&err);  // Not use currently.
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing err cause",
                        rid, __FUNCTION__);
            } else {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EPDN(deact_info) for aid%d returns err cause : %d",
                        rid, __FUNCTION__, aid, cause);
            }
        }
        return PDN_FAILED;
    } else if (strncmp(DC_DEACT_INFO, cmdFormat, strlen(DC_DEACT_INFO)) == 0) {
        reason = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing reason",
                    rid, __FUNCTION__);
            return PDN_FAILED;
        }

        if (reason == NO_REASON) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no reason for pdn deact aid%d",
                    rid, __FUNCTION__, aid);
        } else {
            setDeactReason(aid, RmcDcUtility::convertFailCauseToRilStandard(reason, m_slot_id));
        }

        if (pLine->atTokHasmore()) {
            rat = pLine->atTokNextint(&err);  // Not use currently.
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing rat type",
                        rid, __FUNCTION__);
                return PDN_FAILED;
            }
            // Value from 3~6 will be view as IWLAN, which is related to VoData feature
            if (rat >= ACCESS_NW_TYPE_VODATA1 && rat <= ACCESS_NW_TYPE_VODATA4) {
                RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] rat=%d change to IWLAN(2)", m_slot_id, __FUNCTION__, rat);
                rat = ACCESS_NW_TYPE_IWLAN;
            }
        }
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no matched command format (%s)",
                rid, __FUNCTION__, cmdFormat);
        return PDN_FAILED;
    }

    return PDN_SUCCESS;
}

bool RmcDcCommonReqHandler::isFallbackPdn(const int aid) {
    int rid = m_slot_id;
    int pdnsOfActive = 0;
    PdnInfo pdn_info;
    memset(&pdn_info, 0, sizeof(pdn_info));
    const int nPdnInfoSize = getPdnTableSize();
    String8 cmd("");
    bool fallbackPdnActive = true;

    // Check if AID is out of bound.
    if (!validateAid(aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid=%d is out of bound",
                rid, __FUNCTION__, aid);
        return false;
    }

    if (getPdnActiveStatus(aid) != DATA_STATE_ACTIVE) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] aid=%d is not active",
                rid, __FUNCTION__, aid);
        fallbackPdnActive = false;
    }

    for (int i = 0; i < nPdnInfoSize; i++) {
        pdn_info = getPdnInfo(i);
        if ((pdn_info.active == DATA_STATE_ACTIVE) &&
                (getTransIntfId(pdn_info.aid) == getTransIntfId(aid))) {
            if (0 == pdnsOfActive) {
                cmd.append(String8::format("%d", pdn_info.aid));
            } else {
                cmd.append(String8::format(",%d", pdn_info.aid));
            }
            pdnsOfActive++;
        }
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] %s for aid=%d (all Of active aid <%s> for transIntfId=%d)",
            rid, __FUNCTION__, BTOS(pdnsOfActive > (fallbackPdnActive ? 1 : 0)), aid, cmd.string(), getTransIntfId(aid));
    return (pdnsOfActive > (fallbackPdnActive ? 1 : 0)) ? true : false;
}

bool RmcDcCommonReqHandler::notifyDeactReasonIfNeeded(const int deactivatedAid) {
    int rid = m_slot_id;

    // Check if AID is out of bound.
    if (!validateAid(deactivatedAid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] deactivatedAid=%d is out of bound",
                rid, __FUNCTION__, deactivatedAid);
        return false;
    }

    // Update the reason of why pdn was deactivated to FWK for checking if needs to do pdn retrying
    // except that there has no existed reason or another of fallback pdn of the same apn is still alive.
    if (getDeactReason(deactivatedAid) != NO_REASON && !isFallbackPdn(deactivatedAid)) {
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] notify deact reason %d for {deactAid=%d, transIntfId=%d}",
                rid, __FUNCTION__, getDeactReason(deactivatedAid), deactivatedAid,
                getTransIntfId(deactivatedAid));
        return true;
    }
    return false;
}

bool RmcDcCommonReqHandler::validateAid(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->validateAid(aid);
}

PdnInfo RmcDcCommonReqHandler::getPdnInfo(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getPdnInfo(aid);
}

void RmcDcCommonReqHandler::setPdnInfo(int aid, PdnInfo* pdnInfo) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setPdnInfo(aid, pdnInfo);
}

void RmcDcCommonReqHandler::clearPdnInfo(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    if (validateAid(aid)) {
        int interfaceId = getInterfaceId(getTransIntfId(aid));
        if (interfaceId >= 0 && interfaceId < MAX_CCMNI_INTERFACE_COUNT) {
            s_mtuV6[interfaceId] = 0;
        }
    }
    m_pPdnManager->clearPdnInfo(aid);
}

bool RmcDcCommonReqHandler::isDedicateBearer(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->isDedicateBearer(aid);
}

void RmcDcCommonReqHandler::setSignalingFlag(int aid, int flag) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setSignalingFlag(aid, flag);
}

void RmcDcCommonReqHandler::setAid(int index, int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setAid(index, aid);
}

void RmcDcCommonReqHandler::setAidAndPrimaryAid(int index, int aid, int primaryAid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setAidAndPrimaryAid(index, aid, primaryAid);
}

void RmcDcCommonReqHandler::setIsEmergency(int aid, bool isEmergency) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setIsEmergency(aid, isEmergency);
}

void RmcDcCommonReqHandler::setIsDedicateBearer(int aid, bool isDedicateBearer) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setIsDedicateBearer(aid, isDedicateBearer);
}

void RmcDcCommonReqHandler::setReason(int aid, int reason) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setReason(aid, reason);
}

void RmcDcCommonReqHandler::setDeactReason(int aid, int deactReason) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setDeactReason(aid, deactReason);
}

void RmcDcCommonReqHandler::setSscMode(int aid, int sscMode) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setSscMode(aid, sscMode);
}

void RmcDcCommonReqHandler::setPdnType(int aid, int pdnType) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setPdnType(aid, pdnType);
}

void RmcDcCommonReqHandler::setProfileId(int aid, int profileId) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->setProfileId(aid, profileId);
}

int RmcDcCommonReqHandler::getPdnTableSize() {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getPdnTableSize();
}

int RmcDcCommonReqHandler::getPdnActiveStatus(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getPdnActiveStatus(aid);
}

int RmcDcCommonReqHandler::getTransIntfId(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getTransIntfId(aid);
}

char* RmcDcCommonReqHandler::getApnName(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getApnName(aid);
}

int RmcDcCommonReqHandler::getAid(int index) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getAid(index);
}

int RmcDcCommonReqHandler::getPrimaryAid(int index) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getPrimaryAid(index);
}

char* RmcDcCommonReqHandler::getIpv4Dns(int aid, int index) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getIpv4Dns(aid, index);
}

char* RmcDcCommonReqHandler::getIpv6Dns(int aid, int index) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getIpv6Dns(aid, index);
}

int RmcDcCommonReqHandler::getMtu(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getMtu(aid);
}

int RmcDcCommonReqHandler::getMtuV6(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getMtuV6(aid);
}

int RmcDcCommonReqHandler::getRat(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getRat(aid);
}

int RmcDcCommonReqHandler::getSignalingFlag(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getSignalingFlag(aid);
}

char* RmcDcCommonReqHandler::getIpv4Address(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getIpv4Address(aid);
}

char* RmcDcCommonReqHandler::getIpv6Address(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getIpv6Address(aid, 0);
}

char* RmcDcCommonReqHandler::getIpv6Address(int aid, int index) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getIpv6Address(aid, index);
}

int RmcDcCommonReqHandler::getReason(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getReason(aid);
}

int RmcDcCommonReqHandler::getDeactReason(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getDeactReason(aid);
}

int RmcDcCommonReqHandler::getSscMode(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getSscMode(aid);
}

int RmcDcCommonReqHandler::getPdnType(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getPdnType(aid);
}

int RmcDcCommonReqHandler::getProfileId(int aid) {
    RFX_ASSERT(m_pPdnManager != NULL);
    return m_pPdnManager->getProfileId(aid);
}

int RmcDcCommonReqHandler::getSupportNetworkTypes(int apnTypeId) {
    int retValue = SUPPORT_TRANSPORT_MODE_UNKNOW;
    int supportedTypesBitmask = 0;
    int bearerBitmask = 0;

    bool bSupportWifiBearer = false;
    bool bSupportCellularBearer = false;

    for (int i = 0; i < s_nLastReqNum[m_slot_id]; i++){
        supportedTypesBitmask = s_LastApnTable[m_slot_id][i].supportedTypesBitmask;
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s]apnTypeId=%d supportedTypesBitmask=%d", m_slot_id,
                __FUNCTION__, apnTypeId, supportedTypesBitmask);
        // get bearerBitmast of apn type in APN Table
        if (supportedTypesBitmask & apnTypeId) {
            bearerBitmask = s_LastApnTable[m_slot_id][i].bearerBitmask;
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] bearerBitmask=%d", m_slot_id, __FUNCTION__,
                bearerBitmask);
            if (bearerBitmask == 0) {  // support all types
                retValue = SUPPORT_TRANSPORT_MODE_CELLULAR_WIFI;
                return retValue;
            } else {
                if ((bearerBitmask & (1 << (RADIO_TECH_IWLAN - 1))) != 0) {
                    bSupportWifiBearer = true;
                }
                if ((bearerBitmask & (1 << (RADIO_TECH_LTE - 1))) != 0) {
                    bSupportCellularBearer = true;
                }
            }
        }
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] bSupportWifiBearer=%d bSupportCellularBearer=%d",
            m_slot_id, __FUNCTION__, bSupportWifiBearer, bSupportCellularBearer);
    if (bSupportWifiBearer && bSupportCellularBearer) {
        retValue = SUPPORT_TRANSPORT_MODE_CELLULAR_WIFI;
    } else {
        if (bSupportWifiBearer) {
            retValue = SUPPORT_TRANSPORT_MODE_WIFI_ONLY;
        } else if (bSupportCellularBearer) {
            retValue = SUPPORT_TRANSPORT_MODE_CELLULAR_ONLY;
        } else {
            return SUPPORT_TRANSPORT_MODE_UNKNOW;
        }
    }
    return retValue;
}

void RmcDcCommonReqHandler::updatePdnActiveStatus(int aid, int pdnActiveStatus) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updatePdnActiveStatus(aid, pdnActiveStatus);
}

void RmcDcCommonReqHandler::updateApnName(int aid, const char* apnName) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateApnName(aid, apnName);
}

void RmcDcCommonReqHandler::updateTransIntfId(int aid, int transIntfId) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateTransIntfId(aid, transIntfId);
}

void RmcDcCommonReqHandler::updateMtu(int aid, int mtu) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateMtu(aid, mtu);
}

void RmcDcCommonReqHandler::updateMtuV6(int aid, int mtuV6) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateMtuV6(aid, mtuV6);
}

void RmcDcCommonReqHandler::updateRat(int aid, int rat) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateRat(aid, rat);
}

void RmcDcCommonReqHandler::updateIpAddress(int aid, const char* ipv4Addr, const char* ipv6Addr) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateIpAddress(aid, ipv4Addr, 0, ipv6Addr);
}

void RmcDcCommonReqHandler::updateIpv4Address(int aid, const char* ipv4Addr) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateIpv4Address(aid, ipv4Addr);
}

void RmcDcCommonReqHandler::updateIpv6Address(int aid, const char* ipv6Addr) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateIpv6Address(aid, 0, ipv6Addr);
}

void RmcDcCommonReqHandler::updateIpv6Address(int aid, int index, const char* ipv6Addr) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateIpv6Address(aid, index, ipv6Addr);
}

void RmcDcCommonReqHandler::updateBearerId(int aid, int bearerId) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateBearerId(aid, bearerId);
}

void RmcDcCommonReqHandler::updatePcscfAddress(int aid, int index, const char* pcscfAddr) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updatePcscfAddress(aid, index, pcscfAddr);
}

void RmcDcCommonReqHandler::updateIpv4Dns(int aid, int index, const char* v4Dns) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateIpv4Dns(aid, index, v4Dns);
}

void RmcDcCommonReqHandler::updateIpv6Dns(int aid, int index, const char* v6Dns) {
    RFX_ASSERT(m_pPdnManager != NULL);
    m_pPdnManager->updateIpv6Dns(aid, index, v6Dns);
}

bool RmcDcCommonReqHandler::isSupportWifiBearer(int bearerBitmask) {
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] bearerBitmask = %d", m_slot_id,
            __FUNCTION__, bearerBitmask);
    return ((bearerBitmask & (1 << (RADIO_TECH_IWLAN - 1))) != 0);
}

int RmcDcCommonReqHandler::getCmdIndexFromApnTable(unsigned int slot_id, ApnTableReq *pApnTableReq) {
    int cmdIndex = 1;

    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] pApnTableReq apn=%s, user=%s, pwd=%s, apnTypeId=%d, protocol=%s, authtype=%d",
            slot_id, __FUNCTION__, pApnTableReq->apn, pApnTableReq->username, pApnTableReq->password,
            pApnTableReq->apnTypeId, pApnTableReq->protocol, pApnTableReq->authtype);

    // apn;user;password;type(default,mms,...);protocol;roaming_protocol;
    // authtype;carrier_enabled;max_conns;max_conns_time;wait_time;bearer_bitmask;inactive_timer;cmd_index
    for (int i = 0; i < s_nLastReqNum[slot_id]; i++) {

        RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] s_nLastReqNum[%d][%d] apn=%s, user=%s, pwd=%s, supportedTypesBitmask=%d, protocol=%s, authtype=%d",
                slot_id, __FUNCTION__, slot_id, i, s_LastApnTable[slot_id][i].apn, s_LastApnTable[slot_id][i].user,
                s_LastApnTable[slot_id][i].password, s_LastApnTable[slot_id][i].supportedTypesBitmask,
                s_LastApnTable[slot_id][i].protocol, s_LastApnTable[slot_id][i].authType);

        if (pApnTableReq->apn && 0 != strcasecmp(pApnTableReq->apn, s_LastApnTable[slot_id][i].apn)) {
            RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] apn is not the same: %s",
                    slot_id, __FUNCTION__, s_LastApnTable[slot_id][i].apn);
            continue;
        }

        if (pApnTableReq->username && 0 != strcasecmp(pApnTableReq->username, s_LastApnTable[slot_id][i].user)) {
            RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] user is not the same: %s",
                    slot_id, __FUNCTION__, s_LastApnTable[slot_id][i].user);
            continue;
        }

        if (pApnTableReq->password && 0 != strcasecmp(pApnTableReq->password, s_LastApnTable[slot_id][i].password)) {
            RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] pwd is not the same: %s",
                    slot_id, __FUNCTION__, s_LastApnTable[slot_id][i].password);
            continue;
        }

        if (pApnTableReq->apnTypeId != APN_TYPE_INVALID) {
            if ((s_LastApnTable[slot_id][i].supportedTypesBitmask & pApnTableReq->apnTypeId) != pApnTableReq->apnTypeId) {
                RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] supportedTypesBitmask is not supported: %d",
                        slot_id, __FUNCTION__, s_LastApnTable[slot_id][i].supportedTypesBitmask);
                continue;
            }
        }

        if (pApnTableReq->protocol&& 0 != strcasecmp(pApnTableReq->protocol, s_LastApnTable[slot_id][i].protocol)) {
            RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] protocol is not the same: %s",
                    slot_id, __FUNCTION__, s_LastApnTable[slot_id][i].protocol);
            continue;
        }
        if (pApnTableReq->authtype != s_LastApnTable[slot_id][i].authType) {
            RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] authtype is not the same: %d",
                    slot_id, __FUNCTION__, s_LastApnTable[slot_id][i].authType);
            continue;
        }

        cmdIndex = s_ApnCmdIndex[slot_id][i];
        break;
    }

    return (cmdIndex-1);
}

void RmcDcCommonReqHandler::addEscapeSequence(char *buffer) {
    char tmp[MAX_APN_NAME_LENGTH] = {0};
    char tmp2[2] = {0};

    if (strlen(buffer) == 0) {
        return;
    }

    strncpy(tmp, buffer, MAX_APN_NAME_LENGTH-1);
    tmp[MAX_APN_NAME_LENGTH-1] = '\0';
    memset(buffer, 0, MAX_APN_NAME_LENGTH);
    for (unsigned int i = 0; i < strlen(tmp) && strlen(buffer) < (MAX_APN_NAME_LENGTH-3); i++) {
        if (tmp[i] == '\"') {
            strncat(buffer + strlen(buffer), "\\22", MAX_APN_NAME_LENGTH-strlen(buffer)-1);
        } else if (tmp[i] == '\\') {
            strncat(buffer + strlen(buffer), "\\5C", MAX_APN_NAME_LENGTH-strlen(buffer)-1);
        } else if (tmp[i] == '\n') {
            strncat(buffer + strlen(buffer), "\\0D", MAX_APN_NAME_LENGTH-strlen(buffer)-1);
        } else {
            memset(tmp2, 0, 2);
            strncpy(tmp2, &tmp[i], 1);
            tmp2[1] = '\0';
            strncat(buffer + strlen(buffer), tmp2, MAX_APN_NAME_LENGTH-strlen(buffer)-1);
        }
    }
}

// Support requirements to cleanup all data connections.
// This function is used for sync data connections status between Java Framework
// and RILD when phone process reinitiate.
void RmcDcCommonReqHandler::handleResetAllConnections(const sp<RfxMclMessage>& msg) {
    RFX_LOG_D(RFX_LOG_TAG, "[%s]", __FUNCTION__);
    sp<RfxMclMessage> responseMsg;
    sp<RfxMclMessage> urcMsg;
    const int nPdnInfoSize = getPdnTableSize();
    int count = 0;

    for (int i = 0; i < nPdnInfoSize; i++) {
        int aid = getAid(i);
        if (aid != INVALID_AID && !isDedicateBearer(i)) {
            deactivateDataCall(aid);
            clearPdnInfo(aid);
            count++;
        }
    }

    responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(responseMsg);

    if (count > 0) {
        urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_DATA_CALL_LIST_CHANGED,
                m_slot_id, RfxVoidData());
        requestOrSendDataCallList(urcMsg);
    }
}

void RmcDcCommonReqHandler::sendRfOccupyCommandIfNeed(bool enabled) {
    if (mRfOccupy == enabled) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no need", m_slot_id, __FUNCTION__);
        return;
    }

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] occupy %d", m_slot_id, __FUNCTION__, enabled);
    mRfOccupy = enabled;
    String8 cmd("");
    if (enabled) {
        cmd.append(String8::format("AT+EMSPM=1,\"MSPM_NDDS_DATA_VALIDATION_SESSION\",0,0"));
    } else {
        cmd.append(String8::format("AT+EMSPM=2,\"MSPM_NDDS_DATA_VALIDATION_SESSION\""));
    }

    sp<RfxAtResponse> p_response = atSendCommand(cmd);

    if (p_response == NULL || p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] get p_response fail!", m_slot_id, __FUNCTION__);
    }
}

void RmcDcCommonReqHandler::onReleaseRfOccupy(const sp<RfxMclMessage>& msg) {
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] release occupy", m_slot_id, __FUNCTION__);
    sendRfOccupyCommandIfNeed(false);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcDcCommonReqHandler::releaseRfOccupyTimeout(const sp<RfxMclMessage>& msg) {
    int *data = (int *)msg->getData()->getData();
    if (data == NULL) return;
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] release rf occupy %d tag %d %d",
            m_slot_id, __FUNCTION__, mRfOccupy, data[0], mRfOccupyTag);
    if (data[0] == mRfOccupyTag) {
        sendRfOccupyCommandIfNeed(false);
    }
}

void RmcDcCommonReqHandler::requestSetPreferredDataModem(const sp<RfxMclMessage>& msg) {
    int *pReqData = (int *)msg->getData()->getData();
    // 0: slot 0
    // 1: slot 1
    int preferredMd = pReqData[0];
    int rid = m_slot_id;
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> responseMsg;

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] requestSetPreferredDataModem, preferredMd=%d",
            rid, __FUNCTION__, preferredMd);

    if (preferredMd >= RFX_SLOT_ID_0 && preferredMd < MAX_RFX_SLOT_ID) {
        char feature[] = "EDATASIM Supported";
        if (getFeatureVersion(feature) == 1) {
            // Always release RF occupy because prefer data swithed.
            sendRfOccupyCommandIfNeed(false);

            // AT+EDATASIM=<sim_slot>  sim_slot = 0(clear) or 1/2/3/4(slot)
            p_response = atSendCommand(String8::format("AT+EDATASIM=%d", preferredMd + 1));
            if (p_response == NULL) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!", rid, __FUNCTION__);
                ret = RIL_E_INTERNAL_ERR;
            } else if (p_response->isATCmdRspErr()) {
                // If sync default data SIM to MD meet error, then keep sync data settings to MD.
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT command got ERROR", rid, __FUNCTION__);
                ret = RIL_E_INTERNAL_ERR;
            }
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] EDATASIM not supported.", rid, __FUNCTION__);
        }
    }

    responseMsg = RfxMclMessage::obtainResponse(ret, RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

int RmcDcCommonReqHandler::createDataProfiles(RIL_MtkDataProfileInfo **oldDataProfilePtrs,
        RIL_MtkDataProfileInfo **dataProfilePtrs, int reqNum) {
    for (int i = 0; i < reqNum + MAX_COUNT_EXTRA_APN; i++) {
        dataProfilePtrs[i] = (RIL_MtkDataProfileInfo*) calloc(1, sizeof(RIL_MtkDataProfileInfo));
        RFX_ASSERT(dataProfilePtrs[i] != NULL);

        if (i >= reqNum) {
            initialDataProfile(dataProfilePtrs[i], NULL);
        } else {
            initialDataProfile(dataProfilePtrs[i], oldDataProfilePtrs[i]);
        }
    }

    reqNum += insertExtraApns(dataProfilePtrs, reqNum);
    adjustBearerBitmask(dataProfilePtrs, reqNum);
    return reqNum;
}

void RmcDcCommonReqHandler::initialDataProfile(RIL_MtkDataProfileInfo *dest,
        RIL_MtkDataProfileInfo *src) {
    if (src == NULL) {
        FREEIF(dest->apn);
        FREEIF(dest->protocol);
        FREEIF(dest->roamingProtocol);
        FREEIF(dest->user);
        FREEIF(dest->password);
        FREEIF(dest->mvnoType);
        FREEIF(dest->mvnoMatchData);

        dest->type = 1;
        dest->enabled = 1;
        asprintf(&dest->apn, "%s", "");
        asprintf(&dest->protocol, "%s", "IP");
        asprintf(&dest->roamingProtocol, "%s", "IP");
        asprintf(&dest->user, "%s", "");
        asprintf(&dest->password, "%s", "");
        asprintf(&dest->mvnoType, "%s", "");
        asprintf(&dest->mvnoMatchData, "%s", "");
    } else {
        memcpy(dest, src, sizeof(RIL_MtkDataProfileInfo));
        asprintf(&dest->apn, "%s", src->apn);
        asprintf(&dest->protocol, "%s", src->protocol);
        asprintf(&dest->roamingProtocol, "%s", src->roamingProtocol);
        asprintf(&dest->user, "%s", src->user);
        asprintf(&dest->password, "%s", src->password);
        asprintf(&dest->mvnoType, "%s", src->mvnoType);
        asprintf(&dest->mvnoMatchData, "%s", src->mvnoMatchData);
    }
}

void RmcDcCommonReqHandler::deleteDataProfiles(RIL_MtkDataProfileInfo **dest, int count) {
    for (int i = 0; i < count; i++) {
        if (dest[i] != NULL) {
            FREEIF(dest[i]->apn);
            FREEIF(dest[i]->protocol);
            FREEIF(dest[i]->roamingProtocol);
            FREEIF(dest[i]->user);
            FREEIF(dest[i]->password);
            FREEIF(dest[i]->mvnoType);
            FREEIF(dest[i]->mvnoMatchData);
            FREEIF(dest[i]);
        }
    }
    FREEIF(dest);
}

/**
 * As OEM has no emergency type in apns-conf.xml, need add emergency type
 * or add a new apn including emergency type that from vendor-apns-conf.xml.
 * 1. If check the apns are the same, add emergency type to existed apn.
 * 2. If the apns are not the same, add a new apn with emergency type.
 */
int RmcDcCommonReqHandler::insertExtraApns(RIL_MtkDataProfileInfo **dataProfilePtrs, int count) {
    TiXmlDocument *apnXml = new TiXmlDocument();
    if (!apnXml->LoadFile("vendor/etc/vendor-apns-conf.xml", TIXML_ENCODING_UTF8)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Load apn xml error", m_slot_id, __FUNCTION__);
        delete apnXml;
        return 0;
    }

    TiXmlElement* apns = apnXml->FirstChildElement("apns");
    if (apns == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] apns element is NULL", m_slot_id, __FUNCTION__);
        delete apnXml;
        return 0;
    }

    char mccmnc[RFX_PROPERTY_VALUE_MAX] = {0};
    if (!getMccmnc(mccmnc)) {
        delete apnXml;
        return 0;
    }

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] mccmnc = %s", m_slot_id, __FUNCTION__, mccmnc);
    std::string mcc = std::string(mccmnc).substr(0, 3);
    std::string mnc = std::string(mccmnc).substr(3, std::string::npos);

    int index = 0;
    TiXmlElement* elem = apns->FirstChildElement("apn");
    while (elem) {
        TiXmlAttribute* attr = elem->FirstAttribute();
        if (attr == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] attr is NULL", m_slot_id, __FUNCTION__);
            continue;
        }

        bool match = false;
        std::string attrName = "";
        std::string attrValue = "";
        while (attr) {
            attrName = attr->Name();
            attrValue = attr->Value();

            // Leave directly if mccmnc is not matched.
            if ((attrName.compare("mcc") == 0 && attrValue.compare(mcc) != 0)
                    || (attrName.compare("mnc") == 0 && attrValue.compare(mnc) != 0)
                    || (attrName.compare("type") == 0
                    && attrValue.find("emergency") == std::string::npos)) {
                match = false;
                break;
            }
            match = true;

            setApnParameter(dataProfilePtrs[index + count], attrName, attrValue);
            attr = attr->Next();
        }

        if (match) {
            bool insert = true;
            for (int i = 0; i < count; i++) {
                if (compareApn(dataProfilePtrs[i], dataProfilePtrs[index + count])) {
                    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] find same apn index = %d",
                            m_slot_id, __FUNCTION__, i);
                    insertExtraTypes(dataProfilePtrs[i], dataProfilePtrs[index + count]);
                    updateBearerBitmak(dataProfilePtrs[i], dataProfilePtrs[index + count]);
                    insert = false;
                    break;
                }
            }
            if (insert) {
                index++;
                RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] insert count = %d",
                       m_slot_id, __FUNCTION__, index);
                if (index == MAX_COUNT_EXTRA_APN) {
                    delete apnXml;
                    return index;
                }
            } else {
                initialDataProfile(dataProfilePtrs[index + count] , NULL);
            }
        } else {
            initialDataProfile(dataProfilePtrs[index + count] , NULL);
        }

        elem = elem->NextSiblingElement("apn");
    }

    delete apnXml;
    return index;
}

bool RmcDcCommonReqHandler::compareApn(RIL_MtkDataProfileInfo *profile1,
        RIL_MtkDataProfileInfo *profile2) {
    // Currently only check apn/mvnoType/mvnoMatchData, if equal, treat as the same apn.
    // other paramenters will compliance with OEM's apn configuration.
    if (strcasecmp(profile1->apn, profile2->apn) != 0) {
        return false;
    }
    if (strcasecmp(profile1->mvnoType, profile2->mvnoType) != 0) {
        return false;
    }
    if (strcasecmp(profile1->mvnoMatchData, profile2->mvnoMatchData) != 0) {
        return false;
    }

    return true;
}

void RmcDcCommonReqHandler::insertExtraTypes(RIL_MtkDataProfileInfo *dest,
        RIL_MtkDataProfileInfo *src) {
    // Currently only check emergency, add other types if need.
    if (src->supportedTypesBitmask & RIL_APN_TYPE_EMERGENCY) {
        dest->supportedTypesBitmask = (dest->supportedTypesBitmask | RIL_APN_TYPE_EMERGENCY);
    }
}

void RmcDcCommonReqHandler::updateBearerBitmak(RIL_MtkDataProfileInfo *dest,
        RIL_MtkDataProfileInfo *src) {
    if (isSupportWifiBearer(src->bearerBitmask)) {
        if (dest->bearerBitmask == 0) {
            dest->bearerBitmask = ALL_BEARER_BITMASK;
        } else {
            dest->bearerBitmask |= (1 << (RADIO_TECH_IWLAN - 1));
        }
    }
}

void RmcDcCommonReqHandler::setApnParameter(RIL_MtkDataProfileInfo *profile,
        std::string name, std::string value) {
    if (name.compare("carrier") == 0) {
        return;
    }
    if (name.compare("apn") == 0) {
        asprintf(&profile->apn, "%s", value.c_str());
        return;
    }
    if (name.compare("protocol") == 0) {
        asprintf(&profile->protocol, "%s", value.c_str());
        return;
    }
    if (name.compare("roaming_protocol") == 0) {
        asprintf(&profile->roamingProtocol, "%s", value.c_str());
        return;
    }
    if (name.compare("authtype") == 0) {
        profile->authType = atoi(value.c_str());
        return;
    }
    if (name.compare("user") == 0) {
        asprintf(&profile->user, "%s", value.c_str());
        return;
    }
    if (name.compare("password") == 0) {
        asprintf(&profile->password, "%s", value.c_str());
        return;
    }
    if (name.compare("wait_time") == 0) {
        profile->waitTime = atoi(value.c_str());
        return;
    }
    if (name.compare("max_conns") == 0) {
        profile->maxConns = atoi(value.c_str());
        return;
    }
    if (name.compare("max_conns_time") == 0) {
        profile->maxConnsTime = atoi(value.c_str());
        return;
    }
    if (name.compare("type") == 0) {
        // Currently only check emergency, add other types if need.
        if (value.find("emergency") != std::string::npos) {
            profile->supportedTypesBitmask |= RIL_APN_TYPE_EMERGENCY;
        }
        return;
    }
    if (name.compare("enabled") == 0) {
        profile->enabled = atoi(value.c_str());
        return;
    }

    if (name.compare("network_type_bitmask") == 0) {
        int bitmask = 0;
        int singleBitmask = -1;
        std::string::size_type p1, p2;
        p1 = 0;
        p2 = value.find("|");
        if (p2 == std::string::npos) {
            singleBitmask = atoi(value.c_str()) - 1;
            if (singleBitmask >= 0) {
                bitmask |= (1 << singleBitmask);
            }
        } else {
            std::string b = "";
            while(std::string::npos != p2) {
                b = value.substr(p1, p2 - p1);
                singleBitmask = atoi(b.c_str()) - 1;
                if (singleBitmask >= 0) {
                    bitmask |= (1 << singleBitmask);
                }
                p1 = p2 + std::string("|").size();
                p2 = value.find("|", p1);
            }
            b = value.substr(p1, std::string::npos);
            singleBitmask = atoi(b.c_str()) - 1;
            if (singleBitmask >= 0) {
                bitmask |= (1 << singleBitmask);
            }
        }
        profile->bearerBitmask = bitmask;
        return;
    }
    if (name.compare("mtu") == 0) {
        profile->mtu = atoi(value.c_str());
        return;
    }
    if (name.compare("mvno_type") == 0) {
        asprintf(&profile->mvnoType, "%s", value.c_str());
        return;
    }
    if (name.compare("mvno_match_data") == 0) {
        asprintf(&profile->mvnoMatchData, "%s", value.c_str());
        return;
    }
}

bool RmcDcCommonReqHandler::getMccmnc(char* mccmnc) {
    char key[MAX_LENGTH_PROPERTY_NAME] = {0};
    int n = 0;
    if (m_slot_id == 0) {
        n = snprintf(key, MAX_LENGTH_PROPERTY_NAME - 1, "%s", PROPERTY_MCC_MNC);
    } else {
        n = snprintf(key, MAX_LENGTH_PROPERTY_NAME - 1, "%s%s%d", PROPERTY_MCC_MNC, ".", m_slot_id);
    }
    if (n < 0) {
        return false;
    }
    rfx_property_get(key, mccmnc, "0");
    if (strlen(mccmnc) > 3) {
        return true;
    }

    memset(mccmnc, 0, MAX_LENGTH_MCCMNC);
    memset(key, 0, MAX_LENGTH_PROPERTY_NAME);
    if (m_slot_id == 0) {
        n = snprintf(key, MAX_LENGTH_PROPERTY_NAME - 1, "%s", PROPERTY_MCC_MNC_CDMA);
    } else {
        n = snprintf(key, MAX_LENGTH_PROPERTY_NAME - 1, "%s%s%d",
                PROPERTY_MCC_MNC_CDMA, ".", m_slot_id);
    }
    if (n < 0) {
        return false;
    }
    rfx_property_get(key, mccmnc, "0");
    if (strlen(mccmnc) > 3) {
        return true;
    }

    return false;
}

// Handle emergency apn as customization requirement.
// Update protocol&roaming protocol of modem's default emergency apn.
void RmcDcCommonReqHandler::updateMdDefaultEmergencyApn() {
    int count = s_nLastReqNum[m_slot_id];
    int n = 0;
    int nonEmptyApn = -1;
    int emptyApn = -1;
    char strParam[MAX_AT_CMD_LENGTH] = {0};
    sp<RfxAtResponse> p_response;
    String8 cmd("");

    // Find emergency apns that has only emergency type.
    for (int i = 0; i < count; i++) {
        if (s_LastApnTable[m_slot_id][i].supportedTypesBitmask == RIL_APN_TYPE_EMERGENCY) {
            if (strcmp("", s_LastApnTable[m_slot_id][i].apn) == 0) {
                emptyApn = i;
            } else {
                nonEmptyApn = i;
            }
        }
    }

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] emergency apn index %d, %d",
            m_slot_id, __FUNCTION__, nonEmptyApn, emptyApn);

    // OEM never sync any emergency apn from Java Framework.
    // 1. nonEmptyApn == -1 means there is no emergency apn whose name is not empty
    // in vendor-apns-conf.xml, no need to handle.
    // 2. nonEmptyApn != -1 && emptyApn != -1 means there are 2 emergency apn, one support wifi,
    // one support cellular, will sync to modem directly, no need to handle here.
    // 3. nonEmptyApn != -1 && emptyApn == -1 means vendor-apns-xml has emergency apn whose name
    // is not empty, sync protocol and roaming protocol to modem for this case.
    if (nonEmptyApn != -1 && emptyApn == -1
            && s_LastApnTable[m_slot_id][nonEmptyApn].bearerBitmask != (1 << (RADIO_TECH_IWLAN - 1))) {
        // Sync proptocol and roaming protocol to modem.
        cmd.append(String8::format("AT+EAPNSET=\"\",1,\"\",\"\""));
        p_response = atSendCommand(cmd);
        if (p_response == NULL || p_response->isAtResponseFail()) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] user/pwd error", m_slot_id, __FUNCTION__);
            return;
        }

        memset(strParam, 0, MAX_AT_CMD_LENGTH);
        n = snprintf(strParam, MAX_AT_CMD_LENGTH, "type=emergency;protocol=%s;"
                "roaming_protocol=%s;authtype=2;carrier_enabled=1;max_conns=0;"
                "max_conns_time=0;wait_time=0;bearer_bitmask=%d;inactive_timer=0",
                s_LastApnTable[m_slot_id][nonEmptyApn].protocol,
                s_LastApnTable[m_slot_id][nonEmptyApn].roamingProtocol,
                ALL_BEARER_EXCLUDE_IWLAN_BITMASK);
        if (n < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to print apn info", m_slot_id, __FUNCTION__);
        }

        cmd.clear();
        cmd.append(String8::format("AT+EAPNSET=\"\",2,\"%s\"", strParam));
        p_response = atSendCommand(cmd);
        if (p_response == NULL || p_response->isAtResponseFail()) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parameter error", m_slot_id, __FUNCTION__);
        }
    }
}

void RmcDcCommonReqHandler::requestGetDataContextIds(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    Vector<int> vTransIntfIdList;
    const int nPdnInfoSize = getPdnTableSize();

    vTransIntfIdList.reserve(nPdnInfoSize);

    for (int i = 0; i < nPdnInfoSize; i++) {
        if ((getPdnActiveStatus(i) == DATA_STATE_ACTIVE) &&
                (getTransIntfId(i) != INVALID_TRANS_INTF_ID) &&
                (std::find(vTransIntfIdList.begin(), vTransIntfIdList.end(),
                getTransIntfId(i)) == vTransIntfIdList.end())) {
                vTransIntfIdList.push_back(getTransIntfId(i));
        }
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] the number of context ids = %d",
            m_slot_id, __FUNCTION__, (int)vTransIntfIdList.size());

    sendEvent(RFX_MSG_EVENT_RSP_DATA_CONTEXT_IDS,
            RfxIntsData((int*)vTransIntfIdList.array(), vTransIntfIdList.size()),
            RIL_CMD_PROXY_3, m_slot_id);
}

bool RmcDcCommonReqHandler::isHandoverRequest(int reason, int profileId, int *pTransIntfId) {
    if (reason != ACT_REASON_AOSP_HANDOVER) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] profileId %d reason %d is not for handover",
                m_slot_id, __FUNCTION__, profileId, reason);
        return false;
    }

    const int nPdnInfoSize = getPdnTableSize();
    PdnInfo pdn_info;

    for (int i = 0; i < nPdnInfoSize; i++) {
        pdn_info = getPdnInfo(i);
        if (pdn_info.profileId == profileId && !pdn_info.isDedicateBearer &&
                pdn_info.active == DATA_STATE_ACTIVE) {
            *pTransIntfId = pdn_info.transIntfId;
            return true;
        }
    }
    return false;
}

void RmcDcCommonReqHandler::requestSendQualifiedNetworkTypesChanged(const sp<RfxMclMessage>& msg) {
    int *pReqData = (int *)msg->getData()->getData();
    int apnType = pReqData[0];
    int rat = pReqData[1];
    int data[3] = {0};
    sp<RfxMclMessage> urcMsg;

    data[0] = 1; // this element is 'mode' to indicate the parsing rule of the remain array elements.
    data[1] = apnType;
    data[2] = rat;

    urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_QUALIFIED_NETWORK_TYPES_CHANGED,
            m_slot_id, RfxIntsData(data, 3));
    responseToTelCore(urcMsg);

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] mode=%d, apnType=%d, rat=%d",
            m_slot_id, __FUNCTION__, data[0], data[1], data[2]);
}

void RmcDcCommonReqHandler::sendQualifiedNetworkTypesChanged(const int aid) {
    String8 cmd("");
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response;
    int cause = 0;
    int err = 0;
    int tempAid = 0;
    char *cmdFormat = NULL;
    int rat = 0;
    PdnInfo pdn_info;
    int data[3] = {0};
    sp<RfxMclMessage> urcMsg;

    cmd.append(String8::format("AT+EPDN=%d,\"ran\"", aid));
    p_response = atSendCommandSingleline(cmd, "+EPDN:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                m_slot_id, __FUNCTION__);
        return;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", m_slot_id, __FUNCTION__);
        } else {
            cause = p_response->atGetCmeError();
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EPDN=%d,\"ran\" returns cme cause : %d",
                    m_slot_id, __FUNCTION__, aid, cause);
        }
        return;
    }

    line = p_response->getIntermediates();

    if (line == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get intermediate results!",
                m_slot_id, __FUNCTION__);
        return;
    }

    line->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        return;
    }

    tempAid = line->atTokNextint(&err);
    if (err < 0 || (tempAid != aid)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get aid or it's different to MD assigned",
                m_slot_id, __FUNCTION__);
        return;
    }

    cmdFormat = line->atTokNextstr(&err);
    if (err < 0 || strncmp(DC_RAN, cmdFormat, strlen(DC_RAN)) != 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cmd",
                m_slot_id, __FUNCTION__);
        return;
    }

    /** Rat value
     *  0: Unknown
     *  1: Cellular
     *  2: IWLAN
     *  3: DATA_1
     *  4: DATA_2
     *  5: DATA_3
     *  6: DATA_4
     *  Notes. value from 3~6 will be view as IWLAN, which is related to VoData feature
     */
    rat = line->atTokNextint(&err);
    if (err < 0 || rat < 1) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get rat or it's value not greater or equal to 1",
                m_slot_id, __FUNCTION__);
        return;
    }

    pdn_info = getPdnInfo(aid);
    if (!pdn_info.isDedicateBearer && pdn_info.active == DATA_STATE_ACTIVE) {
        data[0] = 1; // this element is 'mode' to indicate the parsing rule of the remain array elements.
        data[1] = RmcDcUtility::getApnTypeId(pdn_info.profileId);

        // Value from 3~6 will be view as IWLAN, which is related to VoData feature
        if (rat >= ACCESS_NW_TYPE_VODATA1 && rat <= ACCESS_NW_TYPE_VODATA4) {
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] rat=%d change to IWLAN(2)", m_slot_id, __FUNCTION__, rat);
            rat = ACCESS_NW_TYPE_IWLAN;
        }
        data[2] = rat;

        urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_QUALIFIED_NETWORK_TYPES_CHANGED,
                m_slot_id, RfxIntsData(data, 3));
        responseToTelCore(urcMsg);
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] mode=%d, apnType=%d, rat=%d", m_slot_id,
                __FUNCTION__, data[0], data[1], data[2]);
    }
}

void RmcDcCommonReqHandler::setPdnInfoProperty() {
    Vector<int> vTransIntfIdList;
    const int nPdnInfoSize = getPdnTableSize();
    String8 keyPdnInfo("");
    String8 valuePdnInfo("");

    keyPdnInfo.append(String8::format("vendor.ril.data.pdn_info%d", m_slot_id));

    vTransIntfIdList.reserve(nPdnInfoSize);
    for (int i = 0; i < nPdnInfoSize; i++) {
        if ((getPdnActiveStatus(i) == DATA_STATE_ACTIVE) &&
                (getTransIntfId(i) != INVALID_TRANS_INTF_ID) &&
                (std::find(vTransIntfIdList.begin(), vTransIntfIdList.end(),
                getTransIntfId(i)) == vTransIntfIdList.end())) {
            PdnInfo pdn_info = getPdnInfo(i);
            if (pdn_info.profileId != RIL_DATA_PROFILE_IMS
                    && pdn_info.profileId != RIL_DATA_PROFILE_VENDOR_EMERGENCY) {
                valuePdnInfo.append(String8::format("%s_%d,",
                        RmcDcUtility::getProfileType(pdn_info.profileId),
                        getInterfaceId(getTransIntfId(i))));
                vTransIntfIdList.push_back(getTransIntfId(i));
            }
        }
    }

    rfx_property_set(keyPdnInfo, valuePdnInfo.string());

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] %s = %s",
            m_slot_id, __FUNCTION__, keyPdnInfo.string(), valuePdnInfo.string());
}

void RmcDcCommonReqHandler::mobileDataUsageNotify(const sp<RfxMclMessage>& msg) {
    // +EGCMD: <tx_bytes>,<tx_packets>,<rx_bytes>,<rx_packets>
    char *urc = (char*)msg->getData()->getData();
    int err = 0;
    RfxAtLine *pLine = NULL;
    sp<RfxMclMessage> urcMsg;
    unsigned int response[4] = {0};

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", m_slot_id, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    response[0] = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing tx_bytes",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    response[1] = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing tx_packets",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    response[2] = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing rx_bytes",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    response[3] = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing rx_packets",
                m_slot_id, __FUNCTION__);
        goto error;
    }
    RFX_LOG_E(RFX_LOG_TAG, "urc parsing");
    urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_MOBILE_DATA_USAGE,
            m_slot_id, RfxIntsData(response, 4*sizeof(int)));
    responseToTelCore(urcMsg);

error:
    AT_LINE_FREE(pLine);
}

// Handle apns that support ims/mms/xcap/emergency/cbs, but have no bearerbitmask configured.
// e.g. There is an apn(A) support mms type and has no bearerbitmask configured,
// If there is another apn also has mms type but the BearerBitmask is 18(wifi only),
// then configure apn A with bearerbitmask cellular only, else configure apn A
// cellular and WiFi.
void RmcDcCommonReqHandler::adjustBearerBitmask(RIL_MtkDataProfileInfo **dataProfilePtrs,
        int count) {
    if (isWifiApnExplicitlyConfigured()) {
        return;
    }

    for (int i = 0; i < count; i++) {
        if (dataProfilePtrs[i]->bearerBitmask == 0
                && isSupportWifiType(dataProfilePtrs[i]->supportedTypesBitmask)) {
            bool find = false;
            for (int j = 0; j < count; j++) {
                if (isSupportWifiType(dataProfilePtrs[i]->supportedTypesBitmask
                        & dataProfilePtrs[j]->supportedTypesBitmask)
                        && dataProfilePtrs[j]->bearerBitmask == (1 << (RADIO_TECH_IWLAN - 1))) {
                    find = true;
                    break;
                }
            }

            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] %s find %d apn that support wifi only",
                    m_slot_id, __FUNCTION__, dataProfilePtrs[i]->apn, find);

            if (find) {
                dataProfilePtrs[i]->bearerBitmask = ALL_BEARER_EXCLUDE_IWLAN_BITMASK;
            } else {
                dataProfilePtrs[i]->bearerBitmask = ALL_BEARER_BITMASK;
            }
        }
    }
}

bool RmcDcCommonReqHandler::isSupportWifiType(int typesBitmask) {
    if (typesBitmask & RIL_APN_TYPE_IMS
            || typesBitmask & RIL_APN_TYPE_MMS
            || typesBitmask & RIL_APN_TYPE_XCAP
            || typesBitmask & RIL_APN_TYPE_EMERGENCY
            || typesBitmask & RIL_APN_TYPE_CBS) {
        return true;
    }
    return false;
}

void RmcDcCommonReqHandler::requestStartKeepalive(const sp<RfxMclMessage>& msg) {
    String8 cmd("");
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response;
    int err = 0;
    sp<RfxMclMessage> responseMsg;
    int ret = 0;
    char sourceAddress[MAX_IPV6_ADDRESS_LENGTH] = {0};
    char destinationAddress[MAX_IPV6_ADDRESS_LENGTH] = {0};
    char srcPort[6] = {0}, dstPort[6] = {0};
    int type = IPV4_UDP;
    RIL_Errno status = RIL_E_INVALID_ARGUMENTS;
    RIL_KeepaliveStatus keepaliveStatus = {0, KEEPALIVE_INACTIVE};
    int n = 0;

    RIL_KeepaliveRequest *pReq = (RIL_KeepaliveRequest*)msg->getData()->getData();

    if (pReq->type == NATT_IPV4) {
        n = snprintf(sourceAddress, MAX_IPV4_ADDRESS_LENGTH, "%d.%d.%d.%d",
                pReq->sourceAddress[0] & 0xff, pReq->sourceAddress[1] & 0xff,
                pReq->sourceAddress[2] & 0xff, pReq->sourceAddress[3] & 0xff);
        if (n < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to snprintf", m_slot_id, __FUNCTION__);
            goto finish;
        }
        n = snprintf(destinationAddress, MAX_IPV4_ADDRESS_LENGTH, "%d.%d.%d.%d",
                pReq->destinationAddress[0] & 0xff, pReq->destinationAddress[1] & 0xff,
                pReq->destinationAddress[2] & 0xff, pReq->destinationAddress[3] & 0xff);
        if (n < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to snprintf", m_slot_id, __FUNCTION__);
            goto finish;
        }
    } else {
        n = snprintf(sourceAddress, MAX_IPV6_ADDRESS_LENGTH, "%02X%02X:%02X%02X:"
                "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
                pReq->sourceAddress[0] & 0xff, pReq->sourceAddress[1] & 0xff,
                pReq->sourceAddress[2] & 0xff, pReq->sourceAddress[3] & 0xff,
                pReq->sourceAddress[4] & 0xff, pReq->sourceAddress[5] & 0xff,
                pReq->sourceAddress[6] & 0xff, pReq->sourceAddress[7] & 0xff,
                pReq->sourceAddress[8] & 0xff, pReq->sourceAddress[9] & 0xff,
                pReq->sourceAddress[10] & 0xff, pReq->sourceAddress[11] & 0xff,
                pReq->sourceAddress[12] & 0xff, pReq->sourceAddress[13] & 0xff,
                pReq->sourceAddress[14] & 0xff, pReq->sourceAddress[15] & 0xff);
        if (n < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to snprintf", m_slot_id, __FUNCTION__);
            goto finish;
        }
        n = snprintf(destinationAddress, MAX_IPV6_ADDRESS_LENGTH, "%02X%02X:%02X%02X:"
                "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
                pReq->destinationAddress[0] & 0xff, pReq->destinationAddress[1] & 0xff,
                pReq->destinationAddress[2] & 0xff, pReq->destinationAddress[3] & 0xff,
                pReq->destinationAddress[4] & 0xff, pReq->destinationAddress[5] & 0xff,
                pReq->destinationAddress[6] & 0xff, pReq->destinationAddress[7] & 0xff,
                pReq->destinationAddress[8] & 0xff, pReq->destinationAddress[9] & 0xff,
                pReq->destinationAddress[10] & 0xff, pReq->destinationAddress[11] & 0xff,
                pReq->destinationAddress[12] & 0xff, pReq->destinationAddress[13] & 0xff,
                pReq->destinationAddress[14] & 0xff, pReq->destinationAddress[15] & 0xff);
        if (n < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to snprintf", m_slot_id, __FUNCTION__);
            goto finish;
        }
        type = IPV6_UDP;
    }

    n = snprintf(srcPort, sizeof(srcPort), "%d", pReq->sourcePort);
    if (n < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to snprintf", m_slot_id, __FUNCTION__);
        goto finish;
    }
    n = snprintf(dstPort, sizeof(dstPort), "%d", pReq->destinationPort);
    if (n < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to snprintf", m_slot_id, __FUNCTION__);
        goto finish;
    }

    cmd.append(String8::format("AT+EKALIVE=1,%d,%d,\"%s\",\"%s\",\"%s\",\"%s\",%d",
            type, getInterfaceId(pReq->cid), sourceAddress, destinationAddress, srcPort, dstPort,
            pReq->maxKeepaliveIntervalMillis/1000));

    p_response = atSendCommandSingleline(cmd, "+EKALIVE:");

    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!",
                m_slot_id, __FUNCTION__);
        goto finish;
    }

    if (p_response->isAtResponseFail()) {
        if (p_response->getSuccess() > 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] weird case, can not get fail cause due to final"
                    " response indicates success", m_slot_id, __FUNCTION__);
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EKALIVE=1,x returns cme cause : %d",
                    m_slot_id, __FUNCTION__, p_response->atGetCmeError());
        }
        goto finish;
    }

    line = p_response->getIntermediates();

    if (line == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get intermediate results!",
                m_slot_id, __FUNCTION__);
        goto finish;
    }

    line->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        goto finish;
    }

    ret = line->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get conn_id", m_slot_id, __FUNCTION__);
        goto finish;
    }
    keepaliveStatus.sessionHandle = (uint32_t)ret;

    ret = line->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get md_status", m_slot_id, __FUNCTION__);
        goto finish;
    }
    keepaliveStatus.code = (RIL_KeepaliveStatusCode)ret;

    status = RIL_E_SUCCESS;

finish:
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] sessionHandle=%d, code=%d", m_slot_id, __FUNCTION__,
            keepaliveStatus.sessionHandle, keepaliveStatus.code);
    responseMsg = RfxMclMessage::obtainResponse(status,
            RfxKeepaliveStatusData(&keepaliveStatus, sizeof(keepaliveStatus)), msg);
    responseToTelCore(responseMsg);
}

void RmcDcCommonReqHandler::requestStopKeepalive(const sp<RfxMclMessage>& msg) {
    String8 cmd("");
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> responseMsg;

    int* param = (int*)(msg->getData()->getData());
    int conn_id = param[0];

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] conn_id=%d", m_slot_id, __FUNCTION__, conn_id);

    cmd.append(String8::format("AT+EKALIVE=0,%d", conn_id));

    p_response = atSendCommand(cmd);

    if (p_response == NULL || p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] AT+EKALIVE=0,x returns ERROR",
                m_slot_id, __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg);
    } else {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    }
    responseToTelCore(responseMsg);
}

void RmcDcCommonReqHandler::updateKeepaliveStatus(const sp<RfxMclMessage>& msg) {
    //+EKALIVE: <conn_id>,<md_status>
    char *urc = (char*)msg->getData()->getData();
    int err = 0;
    RfxAtLine *pLine = NULL;
    int ret = 0;
    sp<RfxMclMessage> urcMsg;
    RIL_KeepaliveStatus keepaliveStatus = {0, KEEPALIVE_INACTIVE};

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", m_slot_id, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    ret = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing conn_id",
                m_slot_id, __FUNCTION__);
        goto error;
    }
    keepaliveStatus.sessionHandle = (uint32_t)ret;

    ret = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing md_satus",
                m_slot_id, __FUNCTION__);
        goto error;
    }
    keepaliveStatus.code = (RIL_KeepaliveStatusCode)ret;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] sessionHandle=%d, code=%d", m_slot_id, __FUNCTION__,
            keepaliveStatus.sessionHandle, keepaliveStatus.code);
    urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_KEEPALIVE_STATUS, m_slot_id,
            RfxKeepaliveStatusData(&keepaliveStatus, sizeof(keepaliveStatus)));
    responseToTelCore(urcMsg);

error:
    AT_LINE_FREE(pLine);
}

void RmcDcCommonReqHandler::onNwLimitNotify(const sp<RfxMclMessage>& msg) {
    //+ENWLIMIT: <state>
    char *urc = (char*)msg->getData()->getData();
    int err = 0;
    RfxAtLine *pLine = NULL;
    sp<RfxMclMessage> urcMsg;
    int response[1] = {0};

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", m_slot_id, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    response[0] = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing aid",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    urcMsg = RfxMclMessage::obtainUrc(RFX_MSG_URC_NW_LIMIT, m_slot_id, RfxIntsData(response, 1));
    responseToTelCore(urcMsg);

error:
    AT_LINE_FREE(pLine);
}

void RmcDcCommonReqHandler::onKernelAddressUpdated(const sp<RfxMclMessage>& msg) {
    MTK_RIL_NetlinkIpv6Info *netInfo = (MTK_RIL_NetlinkIpv6Info*)msg->getData()->getData();
    int aid = INVALID_AID;
    const int nPdnInfoSize = getPdnTableSize();

    for (int i = 0; i < nPdnInfoSize; i++) {
        if ((getPdnActiveStatus(i) == DATA_STATE_ACTIVE) &&
                (getInterfaceId(getTransIntfId(i)) == netInfo->interfaceId)) {
            aid = i;
            break;
        }
    }

    if (aid == INVALID_AID) return;

    if ((netInfo->flags & IFA_F_DEPRECATED) != 0) {
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] deprecated (aid=%d, interfaceId=%d, address=%s)",
                m_slot_id, __FUNCTION__, aid, netInfo->interfaceId, netInfo->address);
        sendDataCallListResponse(msg, INVALID_AID, SSC_UNKNOWN, true);
        return;
    }
    if (netInfo->notify == 1) {
        sendDataCallListResponse(msg, INVALID_AID, SSC_UNKNOWN, false);
    }
}

void RmcDcCommonReqHandler::updateMtuV6FromKernel(int interfaceId, int mtuV6) {
    int aid = INVALID_AID;
    const int nPdnInfoSize = getPdnTableSize();
    for (int i = 0; i < nPdnInfoSize; i++) {
        if ((getPdnActiveStatus(i) == DATA_STATE_ACTIVE) &&
                (getInterfaceId(getTransIntfId(i)) == interfaceId)) {
            aid = i;
            break;
        }
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] interfaceId=%d aid=%d",
            m_slot_id, __FUNCTION__, interfaceId, aid);
    if (aid == INVALID_AID) {
        if (interfaceId >= 0 && interfaceId < MAX_CCMNI_INTERFACE_COUNT) {
            s_mtuV6[interfaceId] = mtuV6;
        }
    } else {
        updateMtuV6(aid, mtuV6);
    }
}

void RmcDcCommonReqHandler::resetLastApnTable() {
    if (s_LastApnTable[m_slot_id] != NULL) {
        for (int i = 0; i < s_nLastReqNum[m_slot_id]; i++) {
            FREEIF(s_LastApnTable[m_slot_id][i].apn);
            FREEIF(s_LastApnTable[m_slot_id][i].protocol);
            FREEIF(s_LastApnTable[m_slot_id][i].roamingProtocol);
            FREEIF(s_LastApnTable[m_slot_id][i].user);
            FREEIF(s_LastApnTable[m_slot_id][i].password);
            FREEIF(s_LastApnTable[m_slot_id][i].mvnoType);
            FREEIF(s_LastApnTable[m_slot_id][i].mvnoMatchData);
        }
        FREEIF(s_LastApnTable[m_slot_id]);
    }
    FREEIF(s_ApnCmdIndex[m_slot_id]);
    s_nLastReqNum[m_slot_id] = 0;
}

void RmcDcCommonReqHandler::updateTdInfo() {
    int currentRat = getCurrentRat();
    // Only support URSP on 4/5G RAT
    if (currentRat != RADIO_TECH_NR && currentRat != RADIO_TECH_LTE &&
            currentRat != RADIO_TECH_LTE_CA) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Don't support URSP on RAT: %d", m_slot_id, __FUNCTION__,
                currentRat);
        return;
    }

    const int pdnInfoSize = getPdnTableSize();
    for (int i = 0; i < pdnInfoSize; i++) {
        if (DATA_STATE_INACTIVE != getPdnActiveStatus(i)) {
            updateTdInfo(i);
        }
    }
}

void RmcDcCommonReqHandler::updateTdInfo(int aid) {
    int currentRat = getCurrentRat();
    // Only support URSP on 4/5G RAT
    if (currentRat != RADIO_TECH_NR && currentRat != RADIO_TECH_LTE &&
            currentRat != RADIO_TECH_LTE_CA) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Don't support URSP on RAT: %d", m_slot_id, __FUNCTION__,
                currentRat);
        return;
    }

    if (aid < 0 || aid >= getPdnTableSize()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] invalid aid%d", m_slot_id, __FUNCTION__, aid);
        return;
    }

    /* Query TD list for this cid
        AT> AT+EGURSPRDP[=<cid>]
        AT< +EGURSPRDP: <cid>,"<param>",<attr>
        AT< +EGURSPRDP: <cid>,"<traffic_desc>","< ue_local_conf >",<match_all_disallowed>
        AT< ...
        AT< OK
    */
    sp<RfxAtResponse> p_response = atSendCommandMultiline(
            String8::format("AT+EGURSPRDP=%d", aid),
            "+EGURSPRDP:");
    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!", m_slot_id, __FUNCTION__);
        return;
    }
    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] At Response Fail!", m_slot_id, __FUNCTION__);
        return;
    }

    int tdCount = 0;
    RfxAtLine *p_cur = NULL;
    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        tdCount++;
    }
    tdCount = tdCount - 1;
    if (tdCount < 1) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] invalid intermediates", m_slot_id, __FUNCTION__);
        return;
    }

    RIL_TrafficDescriptor *pTds = (RIL_TrafficDescriptor*)calloc(tdCount,
            sizeof(RIL_TrafficDescriptor));
    RFX_ASSERT(pTds != NULL);

    p_cur = p_response->getIntermediates();
    int err = 0;
    char *outStr = NULL;
    int outInt = 0;
    int tdIndex = 0;
    if (p_cur == NULL) {
        goto ERROR;
    }
    // Skip first +EGURSPRDP which indicate current pdu setup parameter
    p_cur = p_cur->getNext();
    for (; p_cur != NULL; p_cur = p_cur->getNext()) {
        p_cur->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                    m_slot_id, __FUNCTION__);
            goto ERROR;
        }

        // <cid>
        outInt = p_cur->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cid",
                    m_slot_id, __FUNCTION__);
            goto ERROR;
        }
        // "<traffic_desc>"
        outStr = p_cur->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing traffic_desc",
                    m_slot_id, __FUNCTION__);
            goto ERROR;
        }
        RmcDcUtility::parseTdFromString(&pTds[tdIndex], outStr);

        tdIndex++;
    }

    m_pPdnManager->clearTds(aid);
    m_pPdnManager->setTds(aid, tdCount, pTds);

ERROR:
    // Free dnn and os APP id
    for (int i = 0; i < tdCount; i++) {
        FREEIF(pTds[i].dnn);
        FREEIF(pTds[i].osAppId);
    }
    // Free TD info
    FREEIF(pTds);
}

void RmcDcCommonReqHandler::onUrspReevaluate(const sp<RfxMclMessage>& msg) {
    //AT< +EGURSPREEVALU: <cid>, <event>
    char *urc = (char*)msg->getData()->getData();
    int err = 0;
    RfxAtLine *pLine = NULL;
    sp<RfxMclMessage> urcMsg;
    int cid = -1;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", m_slot_id, __FUNCTION__, urc);

    pLine = new RfxAtLine(urc, NULL);

    if (pLine == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    cid = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cid",
                m_slot_id, __FUNCTION__);
        goto error;
    }

    // Send to framework to update TD list
    requestOrSendDataCallList(msg, cid);

error:
    AT_LINE_FREE(pLine);
}

void RmcDcCommonReqHandler::onDataRatUpdated(const sp<RfxMclMessage>& msg) {
    int needUpdateQos = 0;
    int newRat = RmcDcUtility::radioTechToAccessNw((RIL_RadioTechnology)getCurrentRat());

    int tableSize = m_pPdnManager->getPdnTableSize();
    for (int i = 0; i < tableSize; i++) {
        if (m_pPdnManager->getPdnActiveStatus(i) != DATA_STATE_ACTIVE) continue;

        int oldRat = m_pPdnManager->getAccessNetwork(getAid(i));
        m_pPdnManager->setAccessNetwork(getAid(i), newRat);
        if (oldRat != newRat && (newRat == E_UTRAN || newRat == NG_RAN)) {
            if (!m_pPdnManager->isDedicateBearer(i)) {
                if (PDN_SUCCESS != updateDefaultBearerInfo(i)) {
                    RFX_LOG_E(RFX_LOG_TAG,
                            "[%d][%s] updateDefaultBearerInfo failed for aid%d",
                            m_slot_id, __FUNCTION__, i);
                    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                            RfxVoidData(), msg);
                    responseToTelCore(responseMsg);
                    return;
                }
            }
            updateQosInformation(i, m_pPdnManager->isDedicateBearer(i));
            needUpdateQos = 1;
        }
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] newRat=%d, needUpdateQos=%d",
            m_slot_id, __FUNCTION__, newRat, needUpdateQos);
    if (needUpdateQos == 1) {
        // Here we need a non-request message, so that we can response data call list
        // as a urc
        sp<RfxMclMessage> urcMsg = RfxMclMessage::obtainUrc(
                RFX_MSG_URC_DATA_CALL_LIST_CHANGED, m_slot_id, RfxVoidData());
        requestOrSendDataCallList(urcMsg);
    }

    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

int RmcDcCommonReqHandler::queryAndUpdateMtu(unsigned int aid) {
    int err = 0;
    int tempAid = -1;
    int mtu = 0;
    int type = 0;
    char *cmdFormat = NULL;
    String8 cmd("");
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response;

    // AT+EPDN=<aid>, "mtu"
    // +EPDN:<aid>,"mtu",<type>,<mtu>
    // OK
    cmd.append(String8::format("AT+EPDN=%d,\"mtu\"", aid));
    p_response = atSendCommandMultiline(cmd, "+EPDN:");

    if (p_response == NULL || p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] get p_response fail!",
                m_slot_id, __FUNCTION__);
        return PDN_FAILED;
    }

    for (line = p_response->getIntermediates(); line != NULL; line = line->getNext()) {
        line->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error when token start",
                    m_slot_id, __FUNCTION__);
            return PDN_FAILED;
        }

        tempAid = line->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error when parsing aid",
                    m_slot_id, __FUNCTION__);
            return PDN_FAILED;
        }

        if (tempAid != aid) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] aid=%d and tempAid=%d is not matched",
                m_slot_id, __FUNCTION__, aid, tempAid);
            return PDN_FAILED;
        }

        cmdFormat = line->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cmd",
                    m_slot_id, __FUNCTION__);
            return PDN_FAILED;
        }

        if (strncmp(DC_MTU, cmdFormat, strlen(DC_MTU)) == 0) {
            type = line->atTokNextint(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error when parsing type",
                        m_slot_id, __FUNCTION__);
                return PDN_FAILED;
            }

            mtu = line->atTokNextint(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Error when parsing mtu",
                        m_slot_id, __FUNCTION__);
                return PDN_FAILED;
            }

            if (type == NETAGENT_ADDR_TYPE_IPV4) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] set mtuV4=%d", m_slot_id, __FUNCTION__, mtu);
                updateMtu(aid, mtu);
            }
            if (type == NETAGENT_ADDR_TYPE_IPV6) {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] set mtuV6=%d", m_slot_id, __FUNCTION__, mtu);
                updateMtuV6(aid, mtu);
            }
        }
    }
    return PDN_SUCCESS;
}
