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

#include "RmmCapabilitySwitchRequestHandler.h"
#include "RmmCapabilitySwitchUtil.h"
#include <telephony/mtk_ril.h>
#include "RfxRilUtils.h"
#include "RfxRadioCapabilityData.h"
#include "RfxVoidData.h"
#include "ratconfig.h"
#include <libmtkrilutils.h>
#include "mipc_msg_tlv_const.h"


#define RFX_LOG_TAG "RmmCapa"

#define NR_TYPE_NOT_SUPPORT (0)
#define NR_TYPE_SUB6        (1)
#define NR_TYPE_MMWAVE      (2)
#define NR_TYPE_SUB6_MMWAVE (3)

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmCapabilitySwitchRequestHandler, RIL_CMD_PROXY_9);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_RADIO_CAPABILITY_UPDATED);

Mutex RmmCapabilitySwitchRequestHandler::s_sim_init_state_mutex;
int RmmCapabilitySwitchRequestHandler::s_sim_init_state;

Mutex RmmCapabilitySwitchRequestHandler::s_first_instance_mutex;
bool RmmCapabilitySwitchRequestHandler::s_first_instance = true;

RmmCapabilitySwitchRequestHandler::RmmCapabilitySwitchRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "constructor entered");
    // setSIMInitState(0);
    const int request1[] = {
        RFX_MSG_REQUEST_SET_RADIO_CAPABILITY,
        RFX_MSG_REQUEST_GET_RADIO_CAPABILITY,
        RFX_MSG_REQUEST_CAPABILITY_SWITCH_SET_MAJOR_SIM
    };
    registerToHandleRequest(request1, sizeof(request1) / sizeof(int));

    s_first_instance_mutex.lock();
    if (s_first_instance) {
        int main_sim = queryMainProtocol();
        s_first_instance = false;
        rfx_property_set(
                "persist.vendor.radio.simswitch", String8::format("%d", main_sim).string());
        getNonSlotMclStatusManager()->setIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, main_sim - 1, false, false);
        queryNoResetSupport();
        queryTplusWSupport();
        queryKeep3GMode();
        if(false == RfxRilUtils::isTplusWSupport()) {
            queryActiveMode();
        }
        queryNrType();
    }
    s_first_instance_mutex.unlock();
    queryBearer();
    sendRadioCapabilityDoneIfNeeded();
    sendEGRAT();
    sendEvent(RFX_MSG_EVENT_CAPABILITY_INIT_DONE, RfxVoidData(), m_channel_id, m_slot_id);
}

RmmCapabilitySwitchRequestHandler::~RmmCapabilitySwitchRequestHandler() {
}

void RmmCapabilitySwitchRequestHandler::onHandleRequest(const sp<RfxMclMessage> &msg) {
    //logD(RFX_LOG_TAG, "onHandleRequest: %s", idToString(msg->getId()));
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_SET_RADIO_CAPABILITY:
            logE(RFX_LOG_TAG, "Should not receive RFX_MSG_REQUEST_SET_RADIO_CAPABILITY");
            break;
        case RFX_MSG_REQUEST_CAPABILITY_SWITCH_SET_MAJOR_SIM:
            requestSetRadioCapability(msg);
            break;
        case RFX_MSG_REQUEST_GET_RADIO_CAPABILITY:
            requestGetRadioCapability(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmmCapabilitySwitchRequestHandler::requestSetRadioCapability(const sp<RfxMclMessage> &msg) {
    sp<RfxMclMessage> resMsg;
    int *new_major_slot = (int *)msg->getData()->getData();
    int old_major_slot = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    uint32_t ps_list[4];
    int response_data[1];
    sp<RfxMipcData> mipcRet = NULL;
    mipc_result_const_enum ret = mipc_result_const_NONE;

    logI(RFX_LOG_TAG, "RadioCapability old_major_slot=%d, new_major_slot=%d",
         old_major_slot, new_major_slot[0]);

    sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(MIPC_SYS_SET_MAPPING_REQ, old_major_slot);
    mipcMsg->addMipcTlvUint8(
            MIPC_SYS_SET_MAPPING_REQ_T_MAPPING_COUNT, RfxRilUtils::rfxGetSimCount());
    if (new_major_slot[0] == 0) {
        // PS1 on SIM1
        ps_list[0] = 0;
        ps_list[1] = 1;
        ps_list[2] = 2;
        ps_list[3] = 3;
    } else if (new_major_slot[0] == 1) {
        // PS1 on SIM2
        ps_list[0] = 1;
        ps_list[1] = 0;
        ps_list[2] = 2;
        ps_list[3] = 3;
    }  else if (new_major_slot[0] == 2) {
        // PS1 on SIM3
        ps_list[0] = 1;
        ps_list[1] = 2;
        ps_list[2] = 0;
        ps_list[3] = 3;
    }   else if (new_major_slot[0] == 3) {
        // PS1 on SIM3
        ps_list[0] = 1;
        ps_list[1] = 2;
        ps_list[2] = 3;
        ps_list[3] = 0;
    }

    mipcMsg->addMipcTlv(
            MIPC_SYS_SET_MAPPING_REQ_T_MAPPING_LIST,
            RfxRilUtils::rfxGetSimCount() * sizeof(uint32_t),
            ps_list);

    mipcRet = callToMipcMsgSync(mipcMsg, 0);
    ret = (mipc_result_const_enum) mipcRet->getResult();

    logD(RFX_LOG_TAG, "sendMipcRequest: ret=%d", ret);

    setSimSwitchProp(old_major_slot, new_major_slot[0]);
    response_data[0] = new_major_slot[0];
    resMsg = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS, RfxIntsData(response_data, 1), msg, true);
    responseToTelCore(resMsg);

}

void RmmCapabilitySwitchRequestHandler::queryTplusWSupport() {
    logD(RFX_LOG_TAG, "queryTplusWSupport, Gen98 don't support TDS");
    rfx_property_set("vendor.ril.simswitch.tpluswsupport", "1");
}

void RmmCapabilitySwitchRequestHandler::queryKeep3GMode() {
    logD(RFX_LOG_TAG, "queryKeep3GMode, Gen98 don't support TDS");
    //keep_3g_mode, 0:keep TD-SCDMA, 1:keep WCDMA
    rfx_property_set("vendor.ril.nw.worldmode.keep_3g_mode", "1");
}

void RmmCapabilitySwitchRequestHandler::queryNrType() {
    // Modem don't support mmWave before Gen98, only support SUB6m_slot_id
    sp<RfxMipcData> mipcReq, mipcRsp;
    void* p = NULL;
    uint16_t len =0;
    mipc_sys_nr_band_struct4* pNrBand = NULL;

    if (RatConfig_isNrSupported() == 0) {
        rfx_property_set("vendor.ril.nr.type",
                    String8::format("%d", NR_TYPE_NOT_SUPPORT).string());
        return;
    }

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_BAND_MODE_REQ, m_slot_id);
    mipcReq->addMipcTlvUint8(MIPC_NW_GET_BAND_MODE_REQ_T_OPTION,
            MIPC_NW_BAND_OPTION_SYSTEM_SUPPORT);
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp != NULL) {
        // nr band
        p = mipcRsp->getMipcVal(MIPC_NW_GET_BAND_MODE_CNF_T_NR_BAND, &len);
    }

    // default nr type is sub6
    if ((mipcRsp->getResult() != MIPC_RESULT_SUCCESS) || (p == NULL)) {
        rfx_property_set("vendor.ril.nr.type",
                String8::format("%d", NR_TYPE_SUB6).string());
        return;
    }

    pNrBand = (mipc_sys_nr_band_struct4 *) p;
    // bit 256 ~ 262 is for mmWave
    if ((pNrBand->nr_band_class[8] & 126) != 0) {
        rfx_property_set("vendor.ril.nr.type",
                String8::format("%d", NR_TYPE_SUB6_MMWAVE).string());
    } else {
        rfx_property_set("vendor.ril.nr.type",
                String8::format("%d", NR_TYPE_SUB6).string());
    }
}

void RmmCapabilitySwitchRequestHandler::queryNoResetSupport() {
    logD(RFX_LOG_TAG, "queryNoResetSupport, Gen98 always support");
    rfx_property_set("vendor.ril.simswitch.no_reset_support", "1");
}

void RmmCapabilitySwitchRequestHandler::queryActiveMode() {
    // Gen 98 is always on FDD mode
    rfx_property_set("vendor.ril.nw.worldmode.activemode", "1");
}

int RmmCapabilitySwitchRequestHandler::getActiveMode() {
    char world_mode_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    int world_mode = 0;

    rfx_property_get("vendor.ril.nw.worldmode.activemode", world_mode_prop, "1");
    world_mode = atoi(world_mode_prop);
    return world_mode;
}

int RmmCapabilitySwitchRequestHandler::queryMainProtocol() {
    int main_sim = RfxRilUtils::getMajorSim();
    sp<RfxMipcData> mipcRet = NULL;
    mipc_result_const_enum ret = mipc_result_const_NONE;
    int count = 0;
    uint32_t *ps_list;
    uint16_t length = 0;

    sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(MIPC_SYS_GET_MAPPING_REQ, main_sim - 1);
    mipcRet = callToMipcMsgSync(mipcMsg);

    ret = (mipc_result_const_enum) mipcRet->getResult();
    if (ret != MIPC_RESULT_SUCCESS) {
        logI(RFX_LOG_TAG, "queryMainProtocol failed, return %d", main_sim);
        return main_sim;
    }

    count = mipcRet->getMipcUint8Val(MIPC_SYS_GET_MAPPING_CNF_T_MAPPING_COUNT, 0);
    ps_list = (uint32_t*)mipcRet->getMipcVal(MIPC_SYS_GET_MAPPING_CNF_T_MAPPING_LIST, &length);
    logI(RFX_LOG_TAG, "queryMainProtocol, main_sim:%d, count:%d, len:%d",
            main_sim, count, length);

    if (length >= RfxRilUtils::rfxGetSimCount()) {
        for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            if (ps_list[i] == 0) {
                return (i + 1);
            }
        }
    }
    return main_sim;
}

void RmmCapabilitySwitchRequestHandler::queryBearer() {
    sp<RfxAtResponse> p_response = NULL;
    int ap_max_rat = 0;
    int radio_capability;
    int main_slot = RmmCapabilitySwitchUtil::getMajorSim() - 1;
    char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    char feature_version_1[] = "MODE_SWITCH_BY_DATA";
    int version_1 = getFeatureVersion(feature_version_1, 0);
    char feature_version_2[] = "EVDO DISABLE";
    int version_2 = getFeatureVersion(feature_version_2, 0);

    logD(RFX_LOG_TAG, "queryBearer, m_slot_id=%d, MODE_SWITCH_BY_DATA=%d, EVDO DISABLE=%d",
            m_slot_id, version_1, version_2);

    if (main_slot == m_slot_id) {
        rfx_property_get(rat_properties[0], tempstr, "G");
    } else if (main_slot > m_slot_id) {
        rfx_property_get(rat_properties[m_slot_id + 1], tempstr, "G");
    } else {
        rfx_property_get(rat_properties[m_slot_id], tempstr, "G");
    }

    // query project config capability
    if (strchr(tempstr, 'G') != NULL && RatConfig_isGsmSupported()) {
        ap_max_rat |= RAF_GSM;
    }
    if (strchr(tempstr, 'W') != NULL && RatConfig_isWcdmaSupported()) {
        ap_max_rat |= RAF_UMTS;
    }
    if (strchr(tempstr, 'T') != NULL && RatConfig_isTdscdmaSupported()) {
        ap_max_rat |= RAF_TD_SCDMA;
    }
    if (strchr(tempstr, 'L') != NULL &&
        (RatConfig_isLteFddSupported() || RatConfig_isLteTddSupported())) {
        ap_max_rat |= RAF_LTE;
    }
    if (strchr(tempstr, 'N') != NULL && RatConfig_isNrSupported()) {
        ap_max_rat |= RAF_NR;
    }

    int major_slot = RfxRilUtils::getMajorSim() - 1;
    if (RfxRilUtils::rfxGetSimCount() > 2 || version_1 != 1 || version_2 != 1) {
        if (major_slot == m_slot_id) {
            // use RAF_GPRS as the main protocol flag
            // for 3 SIMs project, if the rat value is same for two protocols
            // AOSP logic cannot find out the main protocol
            ap_max_rat |= RAF_GPRS;
        } else {
            ap_max_rat &= (~RAF_GPRS);
        }
    }
    radio_capability = ap_max_rat;
    getMclStatusManager(m_slot_id)->setIntValue(
            RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, radio_capability, false, false);

    if (major_slot != m_slot_id && getActiveMode() == 2 &&
        RfxRilUtils::isTplusWSupport() == false &&
        RfxRilUtils::getKeep3GMode() == 0) {
        // Remove 3G raf for non major SIMs in TDD mode
        radio_capability &= ~RAF_UMTS;
    }
    if(RmmCapabilitySwitchUtil::isDisableC2kCapability() == false &&
            RatConfig_isC2kSupported()) {
        memset(tempstr, 0, sizeof(tempstr));
        rfx_property_get("persist.vendor.radio.c_capability_slot", tempstr, "1");
        int cslot = atoi(tempstr) - 1;
        logI(RFX_LOG_TAG, "queryBearer, cslot=%d", cslot);
        if (cslot == m_slot_id || (cslot < 0 && m_slot_id == 0)) {
            radio_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
        }
    }
    logD(RFX_LOG_TAG, "radio_capability=%d", radio_capability);
    getMclStatusManager(m_slot_id)->setIntValue(
            RFX_STATUS_KEY_SLOT_CAPABILITY, radio_capability, false, false);
}

void RmmCapabilitySwitchRequestHandler::sendRadioCapabilityDoneIfNeeded() {
    int radio_capability = getMclStatusManager(m_slot_id)->getIntValue(
            RFX_STATUS_KEY_SLOT_CAPABILITY, 0);
    int event_data[1];

    if(RmmCapabilitySwitchUtil::isDisableC2kCapability() == false &&
            RatConfig_isC2kSupported()) {
        // always send CDMA capability from Android Q
        radio_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
    }

    event_data[0] = radio_capability;
    sendEvent(RFX_MSG_EVENT_RADIO_CAPABILITY_UPDATED, RfxIntsData(event_data, 1),
              m_channel_id, m_slot_id);
}

void RmmCapabilitySwitchRequestHandler::requestGetRadioCapability(const sp<RfxMclMessage> &msg) {
    int radio_capability;
    int session_id = -1;
    RIL_RadioCapability rc;
    memset(&rc, 0, sizeof(RIL_RadioCapability));
    rc.version = RIL_RADIO_CAPABILITY_VERSION;
    rc.session = session_id;
    rc.phase = RC_PHASE_UNSOL_RSP;
    rc.status = RC_STATUS_SUCCESS;
    radio_capability = getMclStatusManager(m_slot_id)->getIntValue(
            RFX_STATUS_KEY_SLOT_CAPABILITY, 0);
    if(RmmCapabilitySwitchUtil::isDisableC2kCapability() == false &&
            RatConfig_isC2kSupported()) {
        // always send CDMA capability from Android Q
        radio_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
    }

    logD(RFX_LOG_TAG, "requestGetRadioCapability, cap[%d] = %d, sizeof(RIL_RadioCapability)=%d",
         m_slot_id, radio_capability, (int)sizeof(RIL_RadioCapability));
    rc.rat = radio_capability;
    RfxRilUtils::getLogicalModemId(rc.logicalModemUuid, MAX_UUID_LENGTH, m_slot_id);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
            RIL_E_SUCCESS, RfxRadioCapabilityData(&rc, sizeof(RIL_RadioCapability)), msg);
    responseToTelCore(response);
}

void RmmCapabilitySwitchRequestHandler::setSimSwitchProp(int old_major_slot, int new_major_slot) {
    switchCapability(old_major_slot, new_major_slot);
    rfx_property_set("persist.vendor.radio.simswitch",
                     String8::format("%d", new_major_slot + 1).string());
    getNonSlotMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, new_major_slot, false, false);
}

//shift capability for the new EPSMAP mapping logic
void RmmCapabilitySwitchRequestHandler::shiftCapability(int old_major_slot, int new_major_slot) {
    if (old_major_slot == new_major_slot) {
        logD(RFX_LOG_TAG, "shiftCapability:old=new=%d", old_major_slot);
        return;
    }
    int step = (new_major_slot > old_major_slot) ? 1 : -1;
    for (int i = old_major_slot; i != new_major_slot; i += step) {
        switchFixedCapability(i, i + step);
        setCapabilityByConfig(i, false);
    }
    setCapabilityByConfig(new_major_slot, true);
}

void RmmCapabilitySwitchRequestHandler::switchFixedCapability(int slot_a, int slot_b) {
    int tmp_capability = getMclStatusManager(slot_a)->getIntValue(
            RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0);
    getMclStatusManager(slot_a)->setIntValue(
            RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, getMclStatusManager(slot_b)->getIntValue(
                    RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0), false, false);
    getMclStatusManager(slot_b)->setIntValue(
            RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, tmp_capability, false, false);
}

void RmmCapabilitySwitchRequestHandler::setCapabilityByConfig(int slot, bool is_major_slot) {
    int cslot = -1;
    char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    if (RmmCapabilitySwitchUtil::isDisableC2kCapability() == false && RatConfig_isC2kSupported()) {
        memset(tempstr, 0, sizeof(tempstr));
        rfx_property_get("persist.vendor.radio.c_capability_slot", tempstr, "1");
        cslot = atoi(tempstr) - 1;
    }
    int tmp_capability = getMclStatusManager(slot)->getIntValue(
        RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0);
    if (!is_major_slot && getActiveMode() == 2 && RfxRilUtils::isTplusWSupport() == false &&
        RfxRilUtils::getKeep3GMode() == 0) {
        // Remove 3G raf for non major SIMs in TDD mode
        tmp_capability &= ~RAF_UMTS;
    }
    if (cslot == slot) {
        tmp_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
    }
    logD(RFX_LOG_TAG, "setCapabilityByConfig, cslot=%d,capa=%d", cslot, tmp_capability);
    getMclStatusManager(slot)->setIntValue(
            RFX_STATUS_KEY_SLOT_CAPABILITY, tmp_capability, false, false);
}

void RmmCapabilitySwitchRequestHandler::switchCapability(int old_major_slot, int new_major_slot) {
    int tmp_capability;
    int cslot = -1;
    char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };

    if (RfxRilUtils::rfxGetSimCount() > 2) {
        return shiftCapability(old_major_slot, new_major_slot);
    }
    if (old_major_slot != 0) {
        tmp_capability = getMclStatusManager(old_major_slot)->getIntValue(
                RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0);
        getMclStatusManager(old_major_slot)->setIntValue(
                RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, getMclStatusManager(0)->getIntValue(
                        RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0), false, false);
        getMclStatusManager(0)->setIntValue(
                RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, tmp_capability, false, false);
    }
    if (new_major_slot != 0) {
        tmp_capability = getMclStatusManager(new_major_slot)->getIntValue(
                RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0);
        getMclStatusManager(new_major_slot)->setIntValue(
                RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, getMclStatusManager(0)->getIntValue(
                        RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0), false, false);
        getMclStatusManager(0)->setIntValue(
                RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, tmp_capability, false, false);
    }
    if (RmmCapabilitySwitchUtil::isDisableC2kCapability() == false && RatConfig_isC2kSupported()) {
        memset(tempstr, 0, sizeof(tempstr));
        rfx_property_get("persist.vendor.radio.c_capability_slot", tempstr, "1");
        cslot = atoi(tempstr) - 1;
        logI(RFX_LOG_TAG, "switchCapability, cslot=%d", cslot);
    }
    if (old_major_slot != 0 && new_major_slot != 0) {
        tmp_capability = getMclStatusManager(0)->getIntValue(
                RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0);
        if (getActiveMode() == 2 && RfxRilUtils::isTplusWSupport() == false &&
            RfxRilUtils::getKeep3GMode() == 0) {
            // Remove 3G raf for non major SIMs in TDD mode
            tmp_capability &= ~RAF_UMTS;
        }
        if (cslot == 0) {
            tmp_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
        }
        getMclStatusManager(0)->setIntValue(
                RFX_STATUS_KEY_SLOT_CAPABILITY, tmp_capability, false, false);
    }

    tmp_capability = getMclStatusManager(old_major_slot)->getIntValue(
            RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0);
    if (getActiveMode() == 2 && RfxRilUtils::isTplusWSupport() == false &&
        RfxRilUtils::getKeep3GMode() == 0) {
        // Remove 3G raf for non major SIMs in TDD mode
        tmp_capability &= ~RAF_UMTS;
    }
    if (cslot == old_major_slot) {
        tmp_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
    }
    getMclStatusManager(old_major_slot)->setIntValue(
            RFX_STATUS_KEY_SLOT_CAPABILITY, tmp_capability, false, false);

    tmp_capability = getMclStatusManager(new_major_slot)->getIntValue(
            RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0);
    if (cslot == new_major_slot) {
        tmp_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
    }
    getMclStatusManager(new_major_slot)->setIntValue(
            RFX_STATUS_KEY_SLOT_CAPABILITY, tmp_capability, false, false);
}

void RmmCapabilitySwitchRequestHandler::sendEGRAT() {
    int raf = getMclStatusManager(m_slot_id)->getIntValue(RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0);
    int rat;
    sp<RfxMipcData> mipcRet = NULL;
    mipc_result_const_enum ret = mipc_result_const_NONE;
    uint32_t nw_type = 0;
    char feature_version_1[] = "MODE_SWITCH_BY_DATA";
    int version_1 = getFeatureVersion(feature_version_1, 0);
    char feature_version_2[] = "EVDO DISABLE";
    int version_2 = getFeatureVersion(feature_version_2, 0);
    int gemini_mode = getGeminiMode();

    if(RmmCapabilitySwitchUtil::isDisableC2kCapability() == false &&
            RatConfig_isC2kSupported()) {
        raf |= RAF_CDMA_GROUP;
    } else {
        raf &= ~RAF_CDMA_GROUP;
    }
    if(!RatConfig_isGsmSupported()) {
        raf &= ~RAF_GSM_GROUP;
    }
    raf = RmmCapabilitySwitchUtil::getAdjustedRaf(raf);
    switch (raf) {
        case RAF_GSM_GROUP:
            rat = 0;
            break;
        case RAF_WCDMA_GROUP:
            rat = 1;
            break;
        case RAF_GSM_GROUP | RAF_WCDMA_GROUP:
            rat = 2;
            break;
        case RAF_LTE:
            rat = 3;
            break;
        case RAF_GSM_GROUP | RAF_LTE:
            rat = 4;
            break;
        case RAF_WCDMA_GROUP | RAF_LTE:
            rat = 5;
            break;
        case RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_LTE:
            rat = 6;
            break;
        case RAF_CDMA_GROUP:
            rat = 7;
            break;
        case RAF_GSM_GROUP | RAF_CDMA_GROUP:
            rat = 8;
            break;
        case RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_CDMA_GROUP:
            rat = 10;
            break;
        case RAF_LTE | RAF_CDMA_GROUP:
            rat = 11;
            break;
        case RAF_GSM_GROUP | RAF_LTE | RAF_CDMA_GROUP:
            rat = 12;
            break;
        case RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_LTE | RAF_CDMA_GROUP:
            rat = 14;
            break;
        case RAF_NR:
            rat = 15;
            break;
        case RAF_NR | RAF_GSM_GROUP:
            rat = 16;
            break;
        case RAF_NR | RAF_WCDMA_GROUP:
            rat = 17;
            break;
        case RAF_NR | RAF_GSM_GROUP | RAF_WCDMA_GROUP:
            rat = 18;
            break;
        case RAF_NR | RAF_LTE:
            rat = 19;
            break;
        case RAF_NR | RAF_LTE | RAF_GSM_GROUP:
            rat = 20;
            break;
        case RAF_NR | RAF_LTE | RAF_WCDMA_GROUP:
            rat = 21;
            break;
        case RAF_NR | RAF_LTE | RAF_WCDMA_GROUP | RAF_GSM_GROUP:
            rat = 22;
            break;
        case RAF_NR | RAF_CDMA_GROUP:
            rat = 23;
            break;
        case RAF_NR | RAF_GSM_GROUP | RAF_CDMA_GROUP:
            rat = 24;
            break;
        case RAF_NR | RAF_WCDMA_GROUP | RAF_CDMA_GROUP:
            rat = 25;
            break;
        case RAF_NR | RAF_LTE | RAF_CDMA_GROUP:
            rat = 26;
            break;
        case RAF_NR | RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_CDMA_GROUP:
            rat = 27;
            break;
        case RAF_NR | RAF_LTE | RAF_GSM_GROUP | RAF_CDMA_GROUP:
            rat = 28;
            break;
        case RAF_NR | RAF_LTE | RAF_WCDMA_GROUP | RAF_CDMA_GROUP:
            rat = 29;
            break;
        case RAF_NR | RAF_LTE | RAF_CDMA_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP:
            rat = 30;
            break;
        default:
            rat = 14;
            break;
    }

    // bit0: GSM, bit1: UMTS, bit2: LTE, bit3: NR, bit4: C2K
    if ((raf & RAF_GSM_GROUP) == RAF_GSM_GROUP) {
        nw_type |= 1;
    }
    if ((raf & RAF_WCDMA_GROUP) == RAF_WCDMA_GROUP) {
        nw_type |= 2;
    }
    if ((raf & RAF_LTE) == RAF_LTE) {
        nw_type |= 4;
    }
    if ((raf & RAF_NR) == RAF_NR) {
        nw_type |= 8;
    }
    if ((raf & RAF_CDMA_GROUP) == RAF_CDMA_GROUP) {
        nw_type |= 16;
    }

    sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(
            MIPC_NW_SET_RADIO_CAPABILITY_REQ, m_slot_id);
    mipcMsg->addMipcTlvUint32(MIPC_NW_SET_RADIO_CAPABILITY_REQ_T_RADIO_CAPABILITY, nw_type);

    if (((raf & RAF_CDMA_GROUP) == RAF_CDMA_GROUP) &&
            version_1 == 1 && version_2 == 1 &&
            (gemini_mode == GEMINI_MODE_L_AND_L) && (RfxRilUtils::isMultipleImsSupport() != 0)) {
        mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_CAPABILITY_REQ_T_SWITCH_MODE_BY_DATA, 1);
        logD(RFX_LOG_TAG, "sendEGRAT: switch mode by data");
    }

    logI(RFX_LOG_TAG, "sendEGRAT: raf=%d, rat=%d, nw_type=%d", raf, rat, nw_type);

    mipcRet = callToMipcMsgSync(mipcMsg);
    ret = (mipc_result_const_enum) mipcRet->getResult();

    logD(RFX_LOG_TAG, "sendEGRAT: ret=%d", ret);
}

void RmmCapabilitySwitchRequestHandler::notifySIMInitDone(int slot_id) {
    RFX_UNUSED(slot_id);
}
