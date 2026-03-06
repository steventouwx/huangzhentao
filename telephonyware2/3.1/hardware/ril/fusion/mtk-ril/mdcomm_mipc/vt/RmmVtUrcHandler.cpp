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

#include "RmmVtUrcHandler.h"
#include "RfxIntsData.h"

#define RFX_LOG_TAG "VT RIL URC"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmVtUrcHandler, RIL_CMD_PROXY_URC);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxRawData, RFX_MSG_EVENT_VT_COMMON_DATA);

RmmVtUrcHandler::RmmVtUrcHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {

    logI(RFX_LOG_TAG, "RmmVtUrcHandler constructor");
    const int urc[] = {
        MIPC_NW_ANBR_IND,
        MIPC_NW_IRAT_IND,
    };

    registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));
    //registerToHandleURC(urc, sizeof(urc)/sizeof(char*));
}

RmmVtUrcHandler::~RmmVtUrcHandler() {
}

void RmmVtUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() != NULL) {
        logD(RFX_LOG_TAG, "onHandleUrc MipcMsgId:%d, MipcSlotId:%d",
            msg->getMipcData()->getMipcMsgId(), msg->getMipcData()->getMipcSlotId());
        uint16_t msgId = msg->getMipcData()->getMipcMsgId();
        switch (msgId) {
            case MIPC_NW_ANBR_IND:
                handleEANBR(msg);
                break;

            case MIPC_NW_IRAT_IND:
                handleEIRAT(msg);
                break;

            default:
                logE(RFX_LOG_TAG, "Should not be here");
            break;
        }
    } else {
        logE(RFX_LOG_TAG, "onHandleUrc, getMipcData NULL");
    }
}

void RmmVtUrcHandler::handleEANBR(const sp<RfxMclMessage>& msg) {

    /* +EANBR:<anbrq_config>[,<ebi>,<is_ul>,<bitrate>,<bearer_id>,<pdu_session_id>,<ext_param>]
     *
     * <anbrq_config>: 0, anbrq not support; 1, anbrq support; 2, NW anbr value
     * <ebi>:ebi value
     * <is_ul>: 0, downlink; 1,uplink
     * <bitrate>: bitrate value
     * <bearer_id>: bearer_id value
     * <pdu_session_id>: pdu_session_id value
     * <ext_param>: ext param value
     *
     */

    RfxMipcData* mipcData = msg->getMipcData();

    uint16_t len = 0;
    mipc_nw_anbr_info_struct4* pInfo =
        (mipc_nw_anbr_info_struct4*)(mipcData->getMipcVal(MIPC_NW_ANBR_IND_T_INFO, &len));

    if (pInfo != NULL && len == sizeof(mipc_nw_anbr_info_struct4)) {

        int config = pInfo->anbrq_config;

        switch (config) {
            case 0:
                logI(RFX_LOG_TAG, "EANBR URC not support");
                break;

            case 1:
                logI(RFX_LOG_TAG, "EANBR URC support");
                break;

            case 2: {
                int ebi = pInfo->ebi;
                int is_ul = pInfo->is_ul;
                int bitrate = pInfo->bitrate;
                int bearer_id = pInfo->beare_id;
                int pdu_session_id = pInfo->pdu_session_id;
                int ext_param = pInfo->ext_param;

                logI(RFX_LOG_TAG, "EANBR URC ebi=%d, is_ul=%d, bitrate=%d, bearer_id=%d, pdu_session_id=%d, ext_param=%d",
                        ebi, is_ul, bitrate, bearer_id, pdu_session_id, ext_param);

                int msg_id = MSG_ID_WRAP_IMSVT_MD_ANBR_CONFIG_UPDATE_IND;
                int dataLen = sizeof(RIL_EANBR);
                RIL_EANBR anbr;
                anbr.anbrq_config = config;
                anbr.ebi = ebi;
                anbr.is_ul = is_ul;
                anbr.bitrate = bitrate;
                anbr.bearer_id = bearer_id;
                anbr.pdu_session_id = pdu_session_id;
                anbr.ext_param = ext_param;

                int buffer_size = sizeof(int) + sizeof(int) + sizeof(RIL_EANBR);

                uint8_t buffer[buffer_size];
                int offset = 0;

                memcpy(&buffer[offset], &msg_id, sizeof(int));
                offset += sizeof(int);

                memcpy(&buffer[offset], &dataLen, sizeof(int));
                offset += sizeof(int);

                memcpy(&buffer[offset], &anbr, sizeof(RIL_EANBR));

                sendEvent(RFX_MSG_EVENT_VT_COMMON_DATA, RfxRawData(buffer, buffer_size),
                        RIL_CMD_PROXY_2, m_slot_id);
                break;
            }
            default:
                break;
        }
    } else {
        logE(RFX_LOG_TAG, "[%s] ptr:%p, len:%d", __FUNCTION__, pInfo, len);
    }
}

void RmmVtUrcHandler::handleEIRAT(const sp<RfxMclMessage>& msg) {

    /* +EIRAT:<irat_status>[,<is_successful>]
     *
     * <irat_status>: integer.
     * 0                    Idle (inter-RAT end)
     * 1                    Inter-RAT from LTE to GSM start
     * 2                    Inter-RAT from LTE to UMTS start
     * 3                    Inter-RAT from GSM to LTE start
     * 4                    Inter-RAT from UMTS to LTE start
     * 5                    Inter-RAT from LTE to GSM_UMTS(TBD) start
     * 6                    Inter-RAT from GSM_UMTS(TBD) to LTE start
     * 7                    Inter-RAT from NR to GSM start
     * 8                    Inter-RAT from NR to UMTS start
     * 9                    Inter-RAT from NR to GSM_UMTS(TBD) start
     * 10                   Inter-RAT from NR to LTE start
     * 11                   Inter-RAT from GSM to NR start
     * 12                   Inter-RAT from UMTS to NR start
     * 13                   Inter-RAT from GSM_UMTS(TBD) to NR start
     * 14                   Inter-RAT from LTE to NR start
     * 15                   Inter-RAT from GSM to UMTS start
     * 16                   Inter-RAT from UMTS to GSM start
     * 17                   Inter-RAT from GSM_UMTS(TBD) to GSM start
     * 18                   Inter-RAT from GSM to GSM_UMTS(TBD) start
     * 19                   Inter-RAT from GSM_UMTS(TBD) to UMTS start
     * 20                   Inter-RAT from UMTS to GSM_UMTS(TBD) start
     * <is_successful>: integer. Present only when <irat_status> is 0
     * 0, Inter-RAT procedure failed; 1, Inter-RAT procefure is successful
     *
     */

    RfxMipcData* mipcData = msg->getMipcData();

    uint16_t len = 0;
    mipc_nw_irat_info_struct4* pInfo =
        (mipc_nw_irat_info_struct4*)(mipcData->getMipcVal(MIPC_NW_IRAT_IND_T_INFO, &len));

    if (pInfo != NULL && len == sizeof(mipc_nw_irat_info_struct4)) {
        // Construct msg to vtservice
        int msg_id = MSG_ID_WRAP_IMSVT_MD_INTER_RAT_STATUS_IND;

        int dataLen = sizeof(RIL_EIRAT);
        RIL_EIRAT irat;
        irat.sim_slot_id = m_slot_id;
        irat.irat_status = pInfo->irat_status;
        if (irat.is_successful != 0xFF) {
            irat.is_successful = pInfo->is_successful;
        } else {
            irat.is_successful = -1;
        }

        logI(RFX_LOG_TAG, "EIRAT URC irat_status=%d, is_successful=%d",
                irat.irat_status, irat.is_successful);

        int buffer_size = sizeof(int) + sizeof(int) + sizeof(RIL_EIRAT);

        uint8_t buffer[buffer_size];
        int offset = 0;

        memcpy(&buffer[offset], &msg_id, sizeof(int));
        offset += sizeof(int);

        memcpy(&buffer[offset], &dataLen, sizeof(int));
        offset += sizeof(int);

        memcpy(&buffer[offset], &irat, sizeof(RIL_EIRAT));

        sendEvent(RFX_MSG_EVENT_VT_COMMON_DATA, RfxRawData(buffer, buffer_size),
                RIL_CMD_PROXY_2, m_slot_id);
    } else {
        logE(RFX_LOG_TAG, "[%s] ptr:%p, len:%d", __FUNCTION__, pInfo, len);

    }
}

