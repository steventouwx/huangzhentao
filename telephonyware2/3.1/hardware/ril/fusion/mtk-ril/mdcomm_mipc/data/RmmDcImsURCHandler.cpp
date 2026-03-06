/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2018. All rights reserved.
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
#include <regex>
#include <string>
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include "RfxImsBearerNotifyData.h"
#include "RfxImsDataInfoNotifyData.h"
#include "RmmDcUtility.h"
#include "RmmDcImsURCHandler.h"

#include "RfxMipcData.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"

#define RFX_LOG_TAG "RmmDcImsURCHandler"
#define MAX_PROFILEID_LENGTH 10   // max profileID length
#define MAX_AT_CMD_LENGTH 512

/*****************************************************************************
 * Class RmmDcImsURCHandler
 * Handle RIL request on DATA2 (RIL_CMD_PROXY_10) channel
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmmDcImsURCHandler, RIL_CMD_PROXY_10);

RFX_REGISTER_DATA_TO_URC_ID(RfxImsBearerNotifyData, RFX_MSG_URC_IMS_BEARER_STATE_NOTIFY);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_IMS_BEARER_INIT);
RFX_REGISTER_DATA_TO_URC_ID(RfxImsDataInfoNotifyData, RFX_MSG_URC_IMS_DATA_INFO_NOTIFY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_IMS_BEARER_NOTIFICATION);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_IMS_REQUEST_HANDLER_REGISTER_DONE);


RmmDcImsURCHandler::RmmDcImsURCHandler(int slot_id, int channel_id) :
    RfxBaseHandler(slot_id, channel_id) {
    rfx_property_set(IMS_EIMS_PDN_INFO,"");

    const int requestList[] = {
        RFX_MSG_REQUEST_SET_IMS_BEARER_NOTIFICATION,
        RFX_MSG_REQUEST_RESET_IMS_CONNECTION,
    };
    registerToHandleRequest(requestList, sizeof(requestList) / sizeof(int));

    const int urc[] = {
        MIPC_IMS_PDN_IND,
    };
    registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));

    // ims onoff switch
    setImsNotifySwitch();
}

RmmDcImsURCHandler::~RmmDcImsURCHandler() {
}

void RmmDcImsURCHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
         case RFX_MSG_REQUEST_SET_IMS_BEARER_NOTIFICATION:
            handleImsBearerNotification(msg);
            break;
         case RFX_MSG_REQUEST_RESET_IMS_CONNECTION:
            handleResetImsConnection(msg);
            break;
        default:
            RFX_LOG_D(RFX_LOG_TAG, "unknown request, ignore!");
            break;
    }
}

void RmmDcImsURCHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    uint16_t msgId = msg->getMipcData()->getMipcMsgId();
    switch (msgId) {
        case MIPC_IMS_PDN_IND:
            handleImsBearerNotify(msg);
            break;
        default:
            RFX_LOG_E(RFX_LOG_TAG, "Should not be here");
        break;
    }
}


// URC +EIMSPDN "notify" URC, msg is RfxStringData format  //char *cmdFormat, int interfaceId, RIL_IMS_BearerNotification* notification
void RmmDcImsURCHandler::handleImsBearerNotify(const sp<RfxMclMessage>& msg) {

        //+EIMSPDN: <cmd>, <aid>, <state>, <type>
        //+EIMSPDN: "notify", <aid>, <state>, <type>, <if_id>[, <dnsv4_num>, <dnsv4_addr> [,<dnsv6_num>, <dnsv6_addr>]]
        //<cmd> :   //       "notify" -> MD notify AP to construct IMS PDN

        int rid = m_slot_id;
        char str[MAX_AT_CMD_LENGTH];
        int indType = msg->getMipcData()->getMipcInt8Val(MIPC_IMS_PDN_IND_T_IND_TYPE, 0);
        int aid = msg->getMipcData()->getMipcUint32Val(MIPC_IMS_PDN_IND_T_ID, 0);
        int action = msg->getMipcData()->getMipcInt8Val(MIPC_IMS_PDN_IND_T_PDN_STATE, 0);
        int interfaceId = msg->getMipcData()->getMipcUint32Val(MIPC_IMS_PDN_IND_T_INTERFACE_ID, 0);
        uint32_t type = (uint32_t) msg->getMipcData()->getMipcUint32Val(MIPC_IMS_PDN_IND_T_APN_TYPE, 0);
        if (type == MIPC_APN_TYPE_IMS) {
            strcpy(str,"ims");
        } else {
            strcpy(str,"emergency");
        }
        char *apnType = str;
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] IMS notification indType=%d, aid=%d, action = %d, type = %d,apn type=%s ",
                    rid, __FUNCTION__, indType, aid, action,type, apnType);
        std::unique_ptr<RIL_IMS_BearerNotification> notification(new RIL_IMS_BearerNotification());
        notification->phone = rid;
        notification->aid = aid;
        notification->type = apnType;
        notification->action = action;

        // set LinkMaster
        setLinkMaster(action, interfaceId, apnType);

        if (indType == MIPC_IMS_IND_TYPE_NOTIFY) {
            //RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] IMS notification phone=%d, aid=%d, apn type=%s, action = %d",
            //            rid, __FUNCTION__, notification->phone, notification->aid, notification->type, action);

            if (action == MIPC_MIPC_CID_ACT_STATE_ACTIVATED) {
                notifyImsBearerRequest(RFX_MSG_URC_IMS_BEARER_STATE_NOTIFY, notification.get(), true);
            } else if (action == MIPC_MIPC_CID_ACT_STATE_DEACTIVATED) {
                notifyImsBearerRequest(RFX_MSG_URC_IMS_BEARER_STATE_NOTIFY, notification.get(), true);
            }
        } else if(indType == MIPC_IMS_IND_TYPE_INIT) {

            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] IMS notification phone=%d bearer initial...",
                    rid, __FUNCTION__, m_slot_id);
            sp<RfxMclMessage> urc_to_tel_core = RfxMclMessage::obtainUrc(
                    RFX_MSG_URC_IMS_BEARER_INIT, m_slot_id,
                    RfxIntsData((void*)&m_slot_id, sizeof(int)));
            responseToTelCore(urc_to_tel_core);
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_PDN_ACT_AID,RFX_NO_ONGOING_IMS_PDN_AID);
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_PDN_DEACT_AID,RFX_NO_ONGOING_IMS_PDN_AID);
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_EIMS_PDN_ACT_AID,RFX_NO_ONGOING_IMS_PDN_AID);
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_EIMS_PDN_DEACT_AID,RFX_NO_ONGOING_IMS_PDN_AID);
            getMclStatusManager(rid)->setIntValue(RFX_STATUS_KEY_IMS_PDN_STATE,IMS_OR_EIMS_PDN_INIT);
            getMclStatusManager(rid)->setIntValue(RFX_STATUS_KEY_EIMS_PDN_STATE,IMS_OR_EIMS_PDN_INIT);
        }else if(indType == MIPC_IMS_IND_TYPE_REJECT) {
            //int aid = -1;
            int cause = -1;
            char *type = NULL;
            char *strEvent = NULL;
            char *strCause = NULL;

            std::unique_ptr<RIL_IMS_DataInfoNotify> noti(new RIL_IMS_DataInfoNotify());

            // Notify Clear Codes 33/29 event for IMS PDN activation
            //+EIMSPDN: "reject", <cause> , <type>
            //<cause> : 29 or 33
            //<type> : "ims" or "emergency"

/*
            cause = pLine->atTokNextint(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing cause",
                        rid, __FUNCTION__);
                return;
            }

            type = pLine->atTokNextstr(&err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing type",
                        rid, __FUNCTION__);
                return;
            }
*/
            asprintf(&strEvent, "ClearCodes");
            asprintf(&strCause, "%d", cause);
            noti->phone = m_slot_id;
            noti->type = type;
            noti->event = strEvent;
            noti->extra = strCause;
            //notifyImsDataInfo(noti.get());
            if(strEvent != NULL) free(strEvent);
            if(strCause != NULL) free(strCause);
        }
}

/*****************************************************************************
 * REQUEST HANDLER
 *****************************************************************************/

void RmmDcImsURCHandler::notifyImsBearerRequest(int requestId, RIL_IMS_BearerNotification* notification, bool isUpdateState) {
    RFX_LOG_D(RFX_LOG_TAG, "%s,isEnableBearerNotification=%d", __FUNCTION__,isEnableBearerNotification);
    /* Customized IMS and Emergency(EIMS) PDN notify/process rule support
       0 : DO NOT notify both IMS/EIMS PDN and do NOT request/releaseNetwork in FWK
       1 or Unset : Notify both IMS/EIMS PDN and do request/releaseNetwork in FWK (Default rule)
       2 : Notify both IMS/EIMS PDN but do NOT request/releaseNetwork in FWK
       3 : DO notify IMS PDN and DO request/releaseNetwork in FWK
           DO NOT notify EIMS PDN and DO NOT request/releaseNetwork in FWK*/
    int action = notification->action;
    int pdnState = (action == 1) ? IMS_OR_EIMS_PDN_ACTIVATED : IMS_OR_EIMS_PDN_DEACTIVATED;

    if(isEnableBearerNotification == 1 || isEnableBearerNotification == 2) {
        sp<RfxMclMessage> urc_to_tel_core = RfxMclMessage::obtainUrc(requestId,
            m_slot_id, RfxImsBearerNotifyData((void*)notification, sizeof(RIL_IMS_BearerNotification)));
        responseToTelCore(urc_to_tel_core);
        if (isUpdateState) {
            if (action == 1) {
                if(strncmp(notification->type, "ims", 3) == 0) {
                    getMclStatusManager(m_slot_id)->setIntValue(RFX_STATUS_KEY_IMS_PDN_STATE,pdnState);
                    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_PDN_ACT_AID,notification->aid);
                } else if (strncmp(notification->type, "emergency", 9) == 0) {
                    getMclStatusManager(m_slot_id)->setIntValue(RFX_STATUS_KEY_EIMS_PDN_STATE,pdnState);
                    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_EIMS_PDN_ACT_AID,notification->aid);
                }
            } else if (action == 0) {
                if(strncmp(notification->type, "ims", 3) == 0) {
                    getMclStatusManager(m_slot_id)->setIntValue(RFX_STATUS_KEY_IMS_PDN_STATE,pdnState);
                    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_PDN_DEACT_AID,notification->aid);
                } else if (strncmp(notification->type, "emergency", 9) == 0) {
                    getMclStatusManager(m_slot_id)->setIntValue(RFX_STATUS_KEY_EIMS_PDN_STATE,pdnState);
                    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_EIMS_PDN_DEACT_AID,notification->aid);
                }
            }
        }
    }

    if( isEnableBearerNotification == 3) {
        if(action == 1) {
            if(strncmp(notification->type, "ims", 3) == 0) {
                sp<RfxMclMessage> urc_to_tel_core = RfxMclMessage::obtainUrc(requestId,
                        m_slot_id, RfxImsBearerNotifyData((void*)notification, sizeof(RIL_IMS_BearerNotification)));
                responseToTelCore(urc_to_tel_core);
                if (isUpdateState) {
                    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_PDN_ACT_AID,notification->aid);
                    getMclStatusManager(m_slot_id)->setIntValue(RFX_STATUS_KEY_IMS_PDN_STATE,pdnState);
                }
            } else if (strncmp(notification->type, "emergency", 9) == 0) {
            }
        } else if(action == 0){
            if(strncmp(notification->type, "ims", 3) == 0) {
                sp<RfxMclMessage> urc_to_tel_core = RfxMclMessage::obtainUrc(requestId,
                        m_slot_id, RfxImsBearerNotifyData((void*)notification, sizeof(RIL_IMS_BearerNotification)));
                responseToTelCore(urc_to_tel_core);
                if (isUpdateState) {
                    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_PDN_DEACT_AID,notification->aid);
                    getMclStatusManager(m_slot_id)->setIntValue(RFX_STATUS_KEY_IMS_PDN_STATE,pdnState);
                }
            } else if (strncmp(notification->type, "emergency", 9) == 0) {
            }
        }
    }
}




/*****************************************************************************
 * EVENT HANDLER
 *****************************************************************************/

// IMSM module notify urc or not of EIMSPDN.
void RmmDcImsURCHandler::setImsNotifySwitch() {
    RFX_LOG_I(RFX_LOG_TAG, "setImsNotifySwitch");
    int enable = (RmmDcUtility::isImsSupport()) ? 1 : 0;
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_AT_REQ, m_slot_id);
    initMipc->addMipcTlvStr(MIPC_SYS_AT_REQ_T_ATCMD,
            String8::format("AT+EIMSPDN= \"onoff\",%d", enable));
    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);
    if (cnfMipc->getResult() != MIPC_RESULT_SUCCESS) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] MIPC_SYS_AT_REQ onoff error", m_slot_id, __FUNCTION__);
    }
}

void RmmDcImsURCHandler::handleImsBearerNotification(const sp<RfxMclMessage>& msg) {
    const int *pReqData = (const int*)msg->getData()->getData();
    int enable = pReqData[0];
    int rid = m_slot_id;
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] handleImsBearerNotification phone=%d enable=%d",
            rid, __FUNCTION__, m_slot_id, enable);
    isEnableBearerNotification = enable;

    checkImsServiceRestartAfterSystemCrash();

    notifyUnsyncImsBearerRequest();

    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

// handle unsynchronized scenario after phone/ims service has been killed
void RmmDcImsURCHandler::checkImsServiceRestartAfterSystemCrash() {
    char zygote_value[RFX_PROPERTY_VALUE_MAX] = {0};

    // get property to check if zygote restarted
    rfx_property_get("vendor.ril.zygote.state", zygote_value, "");
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] vendor.ril.zygote.state=%s", m_slot_id, __FUNCTION__, zygote_value);

    if (strcmp(zygote_value, "stopped") == 0 || strcmp(zygote_value, "restarting") == 0) {
        // trigger TRM to handle unsynchronized scenario
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] trigger TRM after zygote restarting",
                m_slot_id, __FUNCTION__);

        rfx_property_set("vendor.ril.zygote.state", "");

        rfx_property_set("vendor.ril.mux.report.case", "2");
        rfx_property_set("vendor.ril.muxreport", "1");

        return;
    }
}

void RmmDcImsURCHandler::notifyUnsyncImsBearerRequest() {
    static int ACTION_IMS_BEARER_DEACTIVATION = 0;
    static int ACTION_IMS_BEARER_ACTIVATION = 1;
    char type[] = "ims";

    int imsPdnState  = IMS_OR_EIMS_PDN_INIT;
    int aid = -1;

    std::unique_ptr<RIL_IMS_BearerNotification> notification(new RIL_IMS_BearerNotification());

    imsPdnState = getMclStatusManager()->getIntValue(
        RFX_STATUS_KEY_IMS_PDN_STATE,IMS_OR_EIMS_PDN_INIT);

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] imsPdnState=%d", m_slot_id, __FUNCTION__, imsPdnState);

    notification->phone = m_slot_id;
    notification->type = type;

    if (imsPdnState == IMS_OR_EIMS_PDN_ACTIVATED) {
        aid = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_PDN_ACT_AID, -1);
        if (aid != RFX_NO_ONGOING_IMS_PDN_AID) {
            notification->aid = aid;
            notification->action = ACTION_IMS_BEARER_ACTIVATION;

            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] notifyUnsyncImsBearerRequest phone=%d, aid=%d, type=%s, action = %d",
                        m_slot_id, __FUNCTION__, notification->phone, notification->aid, notification->type, notification->action);

            notifyImsBearerRequest(RFX_MSG_URC_IMS_BEARER_STATE_NOTIFY, notification.get(), false);
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no ongoing IMS PDN ACT aid",
                    m_slot_id, __FUNCTION__);
        }
    } else if (imsPdnState == IMS_OR_EIMS_PDN_DEACTIVATED) {
        aid = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_PDN_DEACT_AID, -1);
        if (aid != RFX_NO_ONGOING_IMS_PDN_AID) {
            notification->aid = aid;
            notification->action = ACTION_IMS_BEARER_DEACTIVATION;

            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] notifyUnsyncImsBearerRequest phone=%d, aid=%d, type=%s, action = %d",
                        m_slot_id, __FUNCTION__, notification->phone, notification->aid, notification->type, notification->action);

            notifyImsBearerRequest(RFX_MSG_URC_IMS_BEARER_STATE_NOTIFY, notification.get(), false);
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no ongoing IMS PDN DEACT aid",
                    m_slot_id, __FUNCTION__);
        }
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] no ongoing ACTIVATED or DEACTIVATED event",
                m_slot_id, __FUNCTION__);
    }
}

void RmmDcImsURCHandler::setLinkMaster(int action, int interfaceId, char *type) {
    //LinkMaster
    //"<PDN_TYPE>_<Slot ID>_<Interface_ID>,<PDN_TYPE>_<Slot ID>_<Interface_ID>, ..."
    //ims_0_0,emergency_1_2,ims_1_2,
    char currProp[MTK_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get(IMS_EIMS_PDN_INFO,currProp,"");
    char tempProp[MTK_PROPERTY_VALUE_MAX] = {0};
    //LinkMaster
    static int ACTION_IMS_BEARER_DEACTIVATION = 0;
    static int ACTION_IMS_BEARER_ACTIVATION = 1;
    int rid = m_slot_id;

    if (action == ACTION_IMS_BEARER_ACTIVATION) {
        //LinkMaster
        if (interfaceId >= 0) {
            snprintf(tempProp,sizeof(tempProp), "%s_%d_%d,", type, rid, interfaceId);
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] activate pdn type is %s, slotid is %d, interfaceID is %d",
                    rid, __FUNCTION__, type, rid, interfaceId);
            if (strcmp(currProp,"") == 0) { //first property
                strncpy(currProp, tempProp, MTK_PROPERTY_VALUE_MAX-1);
            } else { //second property or more
                //Clean previous info at same SIM
                string tempPropStr;
                tempPropStr = currProp;
                char begin_Index[MAX_PROFILEID_LENGTH+2]={0};
                int n = snprintf(begin_Index,sizeof(begin_Index),"%s_%d",type,rid);
                if (n < 0) {
                   RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to snprintf", rid, __FUNCTION__);
                   return;
                }
                string::size_type begin_ = tempPropStr.find(begin_Index);
                if (begin_!= std::string::npos) {
                    begin_=begin_+strlen(begin_Index)+1;
                    string::size_type end_ = tempPropStr.rfind(type,begin_);
                    if(end_ != std::string::npos) {
                        if(end_ == 0) {
                            tempPropStr.erase(end_,begin_-end_+4);
                        } else {
                            tempPropStr.erase(end_-1,begin_-end_+4);
                        }
                        strncpy(currProp, tempPropStr.c_str(), MTK_PROPERTY_VALUE_MAX-1);
                        rfx_property_set(IMS_EIMS_PDN_INFO,currProp);
                    } else {
                        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] End index is not found, no need to clean property", rid, __FUNCTION__);
                    }
                } else {
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Begin index is not found, no need to clean property", rid, __FUNCTION__);
                }
                RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] current vendor.ims.eims.pdn.info= %s", rid, __FUNCTION__, currProp);

                if (strlen(tempProp) < MTK_PROPERTY_VALUE_MAX - strlen(currProp) - 1) {
                    strncat(currProp+strlen(currProp), tempProp, MTK_PROPERTY_VALUE_MAX-strlen(currProp)-1);
                } else {
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] over max string length", rid, __FUNCTION__);
                }
            }
            rfx_property_set(IMS_EIMS_PDN_INFO,currProp);
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] updated vendor.ims.eims.pdn.info= %s", rid, __FUNCTION__, currProp);
        } else {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Cannot get interfaceID", rid, __FUNCTION__);
        }//LinkMaster
    } else if (action == ACTION_IMS_BEARER_DEACTIVATION) {
        //LinkMaster
        char begin_Index[MAX_PROFILEID_LENGTH+2]={0};
        int ret = snprintf(begin_Index,sizeof(begin_Index),"%s_%d",type,rid);

        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] deactivate pdn type is %s, slotid is %d",
                rid, __FUNCTION__, type, rid);
        if (ret > 0 && strcmp(currProp,"") != 0) {
            string tempPropStr;
            tempPropStr = currProp;
            string::size_type begin_ = tempPropStr.find(begin_Index);
            if (begin_!= std::string::npos) {
                begin_=begin_+strlen(begin_Index)+1;
                string::size_type end_ = tempPropStr.rfind(type,begin_);
                if(end_ != std::string::npos) {
                    if(end_ == 0) {
                        tempPropStr.erase(end_,begin_-end_+4);
                    } else {
                        tempPropStr.erase(end_-1,begin_-end_+4);
                    }
                    if (strlen(tempPropStr.c_str()) < MTK_PROPERTY_VALUE_MAX - strlen(currProp) - 1) {
                        strncpy(currProp, tempPropStr.c_str(), (MTK_PROPERTY_VALUE_MAX - strlen(currProp)- 1));
                    } else {
                        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] DEACTIVATION over max string length", rid, __FUNCTION__);
                    }
                    rfx_property_set(IMS_EIMS_PDN_INFO,currProp);
                } else {
                    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] End index is not found, no need to clean property", rid, __FUNCTION__);
                }
            } else {
                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Begin index is not found, no need to clean property", rid, __FUNCTION__);
            }
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] updated vendor.ims.eims.pdn.info= %s", rid, __FUNCTION__, currProp);
        } else {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] Property is null, no need to clean property, ret = %d", rid, __FUNCTION__, ret);
        }//LinkMaster
    }
}

void RmmDcImsURCHandler::handleResetImsConnection(const sp<RfxMclMessage>& msg) {
    static int ACTION_BEARER_ACTIVATION = 1;
    sp<RfxMclMessage> responseMsg;
    std::unique_ptr<RIL_IMS_BearerNotification> notification(new RIL_IMS_BearerNotification());
    notification->phone = m_slot_id;

    int imsPdnState  = getMclStatusManager()->getIntValue(
        RFX_STATUS_KEY_IMS_PDN_STATE,IMS_OR_EIMS_PDN_INIT);
    int eimsPdnState  = getMclStatusManager()->getIntValue(
        RFX_STATUS_KEY_EIMS_PDN_STATE,IMS_OR_EIMS_PDN_INIT);

    if (imsPdnState == IMS_OR_EIMS_PDN_ACTIVATED) {
        int imsAid = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_PDN_ACT_AID, -1);
        char imsType[] = "ims";
        if (imsAid != RFX_NO_ONGOING_IMS_PDN_AID) {
            notification->aid = imsAid;
            notification->action = ACTION_BEARER_ACTIVATION;
            notification->type = imsType;
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] handleResetImsConnection imsAid=%d, imsPdnState=%d,phone=%d, aid=%d, type=%s, action = %d",
               m_slot_id, __FUNCTION__, imsAid,imsPdnState,notification->phone, notification->aid, notification->type, notification->action);

            notifyImsBearerRequest(RFX_MSG_URC_IMS_BEARER_STATE_NOTIFY, notification.get(), false);
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no ongoing IMS PDN ACT aid",
                    m_slot_id, __FUNCTION__);
        }
    }
    if (eimsPdnState == IMS_OR_EIMS_PDN_ACTIVATED) {
        int eimsAid = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_EIMS_PDN_ACT_AID, -1);
        char eimsType[] = "emergency";
        if (eimsAid != RFX_NO_ONGOING_IMS_PDN_AID) {
            notification->aid = eimsAid;
            notification->action = ACTION_BEARER_ACTIVATION;
            notification->type = eimsType;
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] handleResetImsConnection eimsAid=%d, imsPdnState=%d,phone=%d, aid=%d, type=%s, action = %d",
                m_slot_id, __FUNCTION__, eimsAid,eimsPdnState,notification->phone, notification->aid, notification->type, notification->action);

            notifyImsBearerRequest(RFX_MSG_URC_IMS_BEARER_STATE_NOTIFY, notification.get(), false);
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] no ongoing EIMS PDN ACT aid",
                    m_slot_id, __FUNCTION__);
        }
    }
    responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}
