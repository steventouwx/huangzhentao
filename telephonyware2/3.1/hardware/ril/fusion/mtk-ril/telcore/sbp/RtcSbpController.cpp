/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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
#include "RtcSbpController.h"

#include "RfxMessage.h"
#include "RfxMainThread.h"
#include "RfxRilUtils.h"
#include "RfxRootController.h"
#include "RfxStatusDefs.h"
#include "RfxVoidData.h"
#include "rfx_properties.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RtcSbp"

RFX_IMPLEMENT_CLASS("RtcSbpController", RtcSbpController, RfxController);

const static int s_la_sbps[] = {112, 120, 7, 196, 145};
const static int s_na_sbps[] = {7, 8, 12, 20, 21, 145, 196, 236};
const static int s_na_tf_sbps[] = {7, 8, 12, 21, 145, 196};
const static int s_eu_sbps[] = {15, 132};
const static int s_ind_sbps[] = {18};
const static int s_jpn_sbps[] = {17};
const static int s_mea_sbps[] = {126};
const static int s_au_sbps[] = {19};
const static int s_rus_sbps[] = {127, 113};
const static int s_wwop_sbps[] = {15, 132, 18, 7, 8, 12, 20, 21, 145, 196, 236};
const static int s_om_sbps[] = {0};

RtcSbpController::RtcSbpController() {
}

RtcSbpController::~RtcSbpController() {
}

void RtcSbpController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    int sbp_setter = getSetSbpPlace();
    if (sbp_setter == RTC_SBP_CXP_SET_SBP || sbp_setter == RTC_SBP_MODEM_SET_SBP) {
        return;
    }
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        getStatusManager(i)->registerStatusChangedEx(RFX_STATUS_KEY_SBP_ID,
                RfxStatusChangeCallbackEx(this, &RtcSbpController::onSbpIdChanged));
        getStatusManager(i)->registerStatusChangedEx(RFX_STATUS_KEY_SBP_SUB_ID,
                RfxStatusChangeCallbackEx(this, &RtcSbpController::onSbpIdChanged));
    }
}

void RtcSbpController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    RfxController::onDeinit();
}

bool RtcSbpController::onHandleRequest(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    return true;
}

bool RtcSbpController::onHandleUrc(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    return true;
}

bool RtcSbpController::onHandleResponse(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    return true;
}

bool RtcSbpController::responseToRilj(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    return true;
}

void RtcSbpController::onSbpIdChanged(int slot_id, RfxStatusKeyEnum key __unused,
        RfxVariant old_value __unused, RfxVariant value __unused) {
    int sbp_setter = getSetSbpPlace();
    if (sbp_setter == RTC_SBP_CXP_SET_SBP || sbp_setter == RTC_SBP_MODEM_SET_SBP) {
        logI(RFX_LOG_TAG, "onSbpIdChanged[%d] not support, setter:%d", slot_id, sbp_setter);
        return;
    }
    int mdver = getFeatureVersion((char*)"NA_NO_CXP_REBOOT");
    if (sbp_setter == RTC_SBP_MODEM_PARTIALLY_SET_SBP && mdver == 1) {
        logI(RFX_LOG_TAG, "onSbpIdChanged[%d] not support, setter:%d, mdver:%d", slot_id,
                sbp_setter, mdver);
        return;
    }
    int switch_mode = getSwitchMode();
    if (switch_mode != RTC_SBP_AUTO_SWITCH && switch_mode != RTC_SBP_ONLY_CHECK_FIRST_SIM) {
        logI(RFX_LOG_TAG, "onSbpIdChanged[%d], don't set because switch_mode:%d",
                slot_id, switch_mode);
        return;
    } else if (RTC_SBP_ONLY_CHECK_FIRST_SIM == switch_mode && getFirstSimCheckedFlag() == 1) {
        logI(RFX_LOG_TAG, "onSbpIdChanged[%d], already checked first sim, switch_mode:%d,",
                slot_id, switch_mode);
        return;
    }
    int major_slot = getNonSlotScopeStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    if (major_slot != slot_id) {
        // Only set sbp according to major SIM.
        logD(RFX_LOG_TAG, "onSbpIdChanged, slot(%d) not major SIM, major_slot:%d",
                slot_id, major_slot);
        return;
    }
    String8 config_mccmnc = getStatusManager(major_slot)->getString8Value(
            RFX_STATUS_KEY_CARRIER_CONFIG_CHANGED, String8(""));
    String8 gsm_mccmnc = getStatusManager(major_slot)->getString8Value(
            RFX_STATUS_KEY_UICC_GSM_NUMERIC, String8(""));
    if (gsm_mccmnc.length() < 2 || config_mccmnc != gsm_mccmnc) {
        // Only set sbp when major SIM is valid and is USIM, don't set if no SIM
        logD(RFX_LOG_TAG,
                "onSbpIdChanged[%d], major sim[%d] config_mccmnc[%s], usim_mccmnc[%s] invalid",
                slot_id, major_slot, config_mccmnc.string(), gsm_mccmnc.string());
        return;
    }
    int current_sbp = getCurrentModemSbp();
    int current_sub_id = getCurrentSubId();
    int configured_sbp = atoi(getStatusManager(major_slot)->getString8Value(
            RFX_STATUS_KEY_SBP_ID).string());
    int latest_sbp = getRefinedSbpId(configured_sbp);
    int latest_sub_id = atoi(getStatusManager(major_slot)->getString8Value(
            RFX_STATUS_KEY_SBP_SUB_ID).string());
    logD(RFX_LOG_TAG,
            "onSbpIdChanged[%d], cur_sbp:%d, cur_sub:%d, config_sbp:%d, sbp_id:%d, sub_id:%d",
            slot_id, current_sbp, current_sub_id, configured_sbp, latest_sbp, latest_sub_id);
    if (current_sbp == latest_sbp && current_sub_id == latest_sub_id) {
        setFirstSimCheckedFlag(); // Valid SIM, mark checked even no need to switch
        return;
    }
    switchSbp(latest_sbp, latest_sub_id);
}

void RtcSbpController::switchSbp(int latest_sbp, int latest_sub_id) {
    int major_slot = getNonSlotScopeStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    logD(RFX_LOG_TAG, "switchSbp, slot:%d, sbp:%d, subid:%d", major_slot, latest_sbp,
            latest_sub_id);
    setFirstSimCheckedFlag();
    rfx_property_set("persist.vendor.operator.subid",
            String8::format("%d", latest_sub_id).string());
    rfx_property_set("persist.vendor.mtk_usp_md_sbp_code",
            String8::format("%d", latest_sbp).string());
    saveSbpIdToFile(latest_sbp);
    sendRequestResetRadio();
}

/* Get SBP switch mode
 * Return:
 *  RTC_SBP_AUTO_SWITCH: Auto switch mode
 *  RTC_SBP_ONLY_CHECK_FIRST_SIM: Only check first valid SIM
 *  RTC_SBP_MANUAL_SWITCH: Manual switch mode
 *  RTC_SBP_ALWAYS_ASK: Always ask mode
 */
int RtcSbpController::getSwitchMode() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.mtk_usp_switch_mode", prop, "1");
    return atoi(prop);
}

/* Get current SBP ID which already has been set to modem
 * Return:
 *  0: Open market
 *  > 0: Operator SBP ID
 */
int RtcSbpController::getCurrentModemSbp() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("vendor.mtk.md.sbp", prop, "-1");
    return atoi(prop);
}

/* Get SBP ID stored in file if exists
 * Return:
 *  -1: There is no SBP ID file
 *  0: Open market
 *  > 0: Operator SBP ID
 */
int RtcSbpController::getSbpFromFile() {
    int fd = 0;
    char sbp_buf[16] = {0};
    int ret = 0;
    int sbp_id = -1;
    fd = open(SBP_FILE_PATH, O_RDONLY);
    if (fd >= 0) {
        ret = read(fd, sbp_buf, sizeof(sbp_buf) - 1);
        if (ret > 0) {
            sbp_id = atoi(sbp_buf);
        }
        close(fd);
    }
    return sbp_id;
}

void RtcSbpController::saveSbpIdToFile(int latest_sbp) {
    // Write MD SBP ID for CCCI
    int fd;
    int size;
    char buf[9];
    int ret = -1;

    // set umask for necessary file permission
    umask(S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH);
    fd = open(SBP_FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // Recovery umask to RILD default setting
    umask(S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
    if (fd >= 0) {
        size = snprintf(buf, 9, "%d", latest_sbp);
        ret = write(fd, buf, size);
        logI(RFX_LOG_TAG, "[%s]fd:%d, write ret value:%d, error=%d\n",
                __func__, fd, ret, errno);
        close(fd);
    } else {
        logE(RFX_LOG_TAG, "[%s]Open NVDATA CXP_SBP fail, fd:%d, error=%d\n",
                __func__, fd, errno);
    }
}

/* Get SBP ID from property
 * Return:
 *  -1: No property
 *  0: Open market
 *  > 0: Operator SBP ID
 */
int RtcSbpController::getSbpFromProperty() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.mtk_usp_md_sbp_code", prop, "-1");
    return atoi(prop);
}

/* Get SBP ID from property
 * Return:
 *  0: Open market
 *  > 0: Operator SBP ID
 */
int RtcSbpController::getCustomSbp() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("ro.vendor.mtk_md_sbp_custom_value", prop, "0");
    return atoi(prop);
}

/* Get current subordinate ID which has been set to modem
 * Return:
 *  0: Open market
 *  > 0: Operator SBP ID
 */
int RtcSbpController::getCurrentSubId() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.operator.subid", prop, "0");
    return atoi(prop);
}

/* Get the place where the sbp is set
 * Return:
 *  0: RILD doesn't set sbp
 *  1: RILD set sbp
 *  2: Modem set sbp for some operators
 *  3: Modem set sbp for all operators
 */
int RtcSbpController::getSetSbpPlace() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("ro.vendor.ril.set_sbp_place", prop, "0");
    return atoi(prop);
}

void RtcSbpController::sendRequestResetRadio() {
    int major_slot = getNonSlotScopeStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    sp<RfxMessage> request = RfxMessage::obtainRequest(major_slot,
            RFX_MSG_REQUEST_RESTART_RILD, RfxVoidData());
    rfx_property_set("vendor.gsm.ril.eboot", "1"); // Don't show PIN again
    requestToMcl(request, true);
    logW(RFX_LOG_TAG, "sendRequestResetRadio, wait for death in sleep.");
    sleep(60); // Block this thread because the rild will reset soon.
    logW(RFX_LOG_TAG, "sendRequestResetRadio, RILD did not restart for 1 min!");
}

void RtcSbpController::setFirstSimCheckedFlag() {
    rfx_property_set("persist.vendor.radio.sbp_flag", "1");
}

int RtcSbpController::getFirstSimCheckedFlag() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.radio.sbp_flag", prop, "0");
    return atoi(prop);
}

/* Get the refined sbp id, the sbp_id to be set should belong to the carrierexpress package, or
 * will be treated as Open market.
 * Return:
 *  0: Open market sbp id.
 * > 0: SBP ID
 */
int RtcSbpController::getRefinedSbpId(int sbp_id) {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    int *sbp_table;
    int sbp_table_size = 0;
    rfx_property_get("ro.vendor.mtk_carrierexpress_pack", prop, "-1");
    if (strcmp(prop, "la") == 0) {
        sbp_table = (int*)s_la_sbps;
        sbp_table_size = sizeof(s_la_sbps) / sizeof(s_la_sbps[0]);
    } else if (strcmp(prop, "na") == 0) {
        sbp_table = (int*)s_na_sbps;
        sbp_table_size = sizeof(s_na_sbps) / sizeof(s_na_sbps[0]);
    } else if (strcmp(prop, "na_tf") == 0) {
        sbp_table = (int*)s_na_tf_sbps;
        sbp_table_size = sizeof(s_na_tf_sbps) / sizeof(s_na_tf_sbps[0]);
    } else if (strcmp(prop, "eu") == 0) {
        sbp_table = (int*)s_eu_sbps;
        sbp_table_size = sizeof(s_eu_sbps) / sizeof(s_eu_sbps[0]);
    } else if (strcmp(prop, "ind") == 0) {
        sbp_table = (int*)s_ind_sbps;
        sbp_table_size = sizeof(s_ind_sbps) / sizeof(s_ind_sbps[0]);
    } else if (strcmp(prop, "jpn") == 0) {
        sbp_table = (int*)s_jpn_sbps;
        sbp_table_size = sizeof(s_jpn_sbps) / sizeof(s_jpn_sbps[0]);
    } else if (strcmp(prop, "mea") == 0) {
        sbp_table = (int*)s_mea_sbps;
        sbp_table_size = sizeof(s_mea_sbps) / sizeof(s_mea_sbps[0]);
    } else if (strcmp(prop, "au") == 0) {
        sbp_table = (int*)s_au_sbps;
        sbp_table_size = sizeof(s_au_sbps) / sizeof(s_au_sbps[0]);
    } else if (strcmp(prop, "rus") == 0) {
        sbp_table = (int*)s_rus_sbps;
        sbp_table_size = sizeof(s_rus_sbps) / sizeof(s_rus_sbps[0]);
    } else if (strcmp(prop, "wwop") == 0) {
        sbp_table = (int*)s_wwop_sbps;
        sbp_table_size = sizeof(s_wwop_sbps) / sizeof(s_wwop_sbps[0]);
    } else {
        sbp_table = (int*)s_om_sbps;
        sbp_table_size = sizeof(s_om_sbps) / sizeof(s_om_sbps[0]);
    }
    for (int i = 0; i < sbp_table_size; i++) {
        if (sbp_id == sbp_table[i]) return sbp_id;
    }
    return 0; // Treat as Open Market if the sbp_id not belongs to carrier express pack
}

