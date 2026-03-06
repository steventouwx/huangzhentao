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
#include "RfxMipcData.h"
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RmmSimBaseHandler.h"
#include "RmmCommSimDefs.h"
#include "rfx_properties.h"
#include <telephony/mtk_ril.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "usim_fcp_parser.h"
#ifdef __cplusplus
}
#endif

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

/* OPERATOR*/
//#define OPERATOR_OP09 "OP09"
//#define SPEC_OP09_A "SEGDEFAULT"

RmmSimBaseHandler::RmmSimBaseHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    setTag(String8("RmmSimBaseHandler"));
}

RmmSimBaseHandler::~RmmSimBaseHandler() {
}

void RmmSimBaseHandler::setTag(String8 s) {
    mTag = s;
}

/* //if need keep
UICC_Status RmmSimBaseHandler::getSimStatus() {
} */

int RmmSimBaseHandler::queryAppTypeId(String8 aid) {
    int appTypeId = UICC_APP_SIM; // Default is SIM
    char *string8Null = NULL;

    if (aid.isEmpty() || aid == String8::format("%s", string8Null)) {
        // SIM or RUIM
        int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
        logD(mTag, "queryAppTypeId, aid is empty or \"(null)\", aid = %s, cardType = %d",
                aid.string(), cardType);
        if (cardType & RFX_CARD_TYPE_SIM) {
            appTypeId = UICC_APP_SIM;
        } else if (cardType & RFX_CARD_TYPE_USIM) {
            appTypeId = UICC_APP_USIM;
        } else if (cardType & RFX_CARD_TYPE_CSIM) {
            appTypeId = UICC_APP_CSIM;
        } else if (cardType & RFX_CARD_TYPE_RUIM) {
            appTypeId = UICC_APP_RUIM;
        } else {
            logD(mTag, "queryAppTypeId, Could not get app id because card type is not ready!");
        }
    } else if (strncmp(aid.string(), "A0000000871002", 14) == 0) {
        // USIM
        appTypeId = UICC_APP_USIM; // USIM
    } else if (strncmp(aid.string(), "A0000000871004", 14) == 0) {
        // ISIM
        appTypeId = UICC_APP_ISIM;
    } else if (strncmp(aid.string(), "A0000003431002", 14) == 0) {
        // CSIM
        appTypeId = UICC_APP_CSIM;
    } else if (strncmp(aid.string(), "A000000000RUIM", 14) == 0) {
        // RUIM
        appTypeId = UICC_APP_RUIM;
    } else {
        logD(mTag, "queryAppTypeId, Not support the aid %s", aid.string());
        appTypeId = -1;
    }

    return appTypeId;
}

bool RmmSimBaseHandler::isSimInserted() {
    String8 iccid = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);

    if ((!iccid.isEmpty()) && (strcmp(iccid.string(), "N/A") != 0)){
        return true;
    }
    return false;
}

bool RmmSimBaseHandler::getIccId(char* value) {
    bool isIccIdReady = false;

    String8 iccid = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);

    if (!iccid.isEmpty()) {
        isIccIdReady = true;

        if (strcmp(iccid.string(), "N/A") != 0) {
            if (sprintf(value, "%s", iccid.string()) < 0) {
                logE(mTag, "getIccId, copy string fail");
            }
        }
    }

    return isIccIdReady;
}

bool RmmSimBaseHandler::isSimSlotLockSupport() {
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("ro.vendor.sim_me_lock_mode", property_value, "");

    if ((strlen(property_value) > 0) && (strcmp(property_value, "3") == 0)){
        return true;
    }
    return false;
}

bool RmmSimBaseHandler::isCommontSlotSupport() {
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(PROPERTY_COMMON_SLOT_SUPPORT, property_value, "0");
    return atoi(property_value) == 1 ? true:false;
}

RmmSimPinPukCount* RmmSimBaseHandler::getPinPukRetryCount() {
    RmmSimPinPukCount *retry = NULL;

    //p_response = atSendCommandSingleline("AT+EPINC", "+EPINC:");
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_PIN_COUNT_QUERY_REQ, m_slot_id);
    sp<RfxMipcData> rsp_Data = callToMipcMsgSync(mipc_data);
    int  mipc_result = rsp_Data->getResult();

    if (mipc_result == MIPC_RESULT_SUCCESS) {
        retry = (RmmSimPinPukCount*)calloc(1, sizeof(RmmSimPinPukCount));
        RFX_ASSERT(retry != NULL);
        // Success
        retry->pin1 =
            (int) rsp_Data->getMipcUint8Val(MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PIN1, 0xff);
        retry->pin2 =
            (int) rsp_Data->getMipcUint8Val(MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PIN2, 0xff);
        retry->puk1 =
            (int) rsp_Data->getMipcUint8Val(MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PUK1, 0xff);
        retry->puk2 =
            (int) rsp_Data->getMipcUint8Val(MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PUK2, 0xff);
        setPinPukRetryCountProp(retry);
        logD(mTag, "getPinPukRetryCount, pin1:%d, pin2:%d, puk1:%d, puk2:%d",
                retry->pin1,retry->pin2,retry->puk1,retry->puk2);
    } else {
        logE(mTag, "getPinPukRetryCount, Fail to get PIN and PUK retry count!");
    }

    return retry;
}

void RmmSimBaseHandler::setPinPukRetryCountProp(RmmSimPinPukCount *retry) {
    String8 pin1("vendor.gsm.sim.retry.pin1");
    String8 pin2("vendor.gsm.sim.retry.pin2");
    String8 puk1("vendor.gsm.sim.retry.puk1");
    String8 puk2("vendor.gsm.sim.retry.puk2");

    pin1.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id+1)));
    pin2.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id+1)));
    puk1.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id+1)));
    puk2.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id+1)));

    rfx_property_set(pin1.string(), String8::format("%d", retry->pin1).string());
    rfx_property_set(pin2.string(), String8::format("%d", retry->pin2).string());
    rfx_property_set(puk1.string(), String8::format("%d", retry->puk1).string());
    rfx_property_set(puk2.string(), String8::format("%d", retry->puk2).string());

    if (isAOSPPropSupport()) {
        String8 aospPin;
        String8 aospPin2;
        String8 aospPuk;
        String8 aospPuk2;

        if (RfxRilUtils::rfxGetSimCount() >= 2) {
            String8 slot("gsm.slot");
            slot.append(String8::format("%d", (m_slot_id+1)));
            aospPin = String8::format("%s%s", slot.string(), ".num.pin1");
            aospPin2 = String8::format("%s%s", slot.string(), ".num.pin2");
            aospPuk = String8::format("%s%s", slot.string(), ".num.puk1");
            aospPuk2 = String8::format("%s%s", slot.string(), ".num.puk2");
        } else {
            aospPin = String8::format("%s", "gsm.sim.num.pin");
            aospPin2 = String8::format("%s", "gsm.sim.num.pin2");
            aospPuk = String8::format("%s", "gsm.sim.num.puk");
            aospPuk2 = String8::format("%s", "gsm.sim.num.puk2");
        }

        rfx_property_set(aospPin, String8::format("%d", retry->pin1).string());
        rfx_property_set(aospPin2, String8::format("%d", retry->pin2).string());
        rfx_property_set(aospPuk, String8::format("%d", retry->puk1).string());
        rfx_property_set(aospPuk2, String8::format("%d", retry->puk2).string());
    }
}

void RmmSimBaseHandler::setPin1RetryCountProp(int retry_count) {
    String8 pin1("vendor.gsm.sim.retry.pin1");
    pin1.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id+1)));
    rfx_property_set(pin1.string(), String8::format("%d", retry_count).string());

    if (isAOSPPropSupport()) {
        String8 aospPin;

        if (RfxRilUtils::rfxGetSimCount() >= 2) {
            String8 slot("gsm.slot");
            slot.append(String8::format("%d", (m_slot_id+1)));
            aospPin = String8::format("%s%s", slot.string(), ".num.pin1");
        } else {
            aospPin = String8::format("%s", "gsm.sim.num.pin");
        }

        rfx_property_set(aospPin, String8::format("%d", retry_count).string());
    }
}

void RmmSimBaseHandler::setPin2RetryCountProp(int retry_count) {
    String8 pin2("vendor.gsm.sim.retry.pin2");
    pin2.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id+1)));
    rfx_property_set(pin2.string(), String8::format("%d", retry_count).string());

    if (isAOSPPropSupport()) {
        String8 aospPin2;

        if (RfxRilUtils::rfxGetSimCount() >= 2) {
            String8 slot("gsm.slot");
            slot.append(String8::format("%d", (m_slot_id+1)));
            aospPin2 = String8::format("%s%s", slot.string(), ".num.pin2");
        } else {
            aospPin2 = String8::format("%s", "gsm.sim.num.pin2");
        }

        rfx_property_set(aospPin2, String8::format("%d", retry_count).string());
    }
}

void RmmSimBaseHandler::setPuk1RetryCountProp(int retry_count) {
    String8 puk1("vendor.gsm.sim.retry.puk1");
    puk1.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id+1)));
    rfx_property_set(puk1.string(), String8::format("%d", retry_count).string());

    if (isAOSPPropSupport()) {
        String8 aospPuk;

        if (RfxRilUtils::rfxGetSimCount() >= 2) {
            String8 slot("gsm.slot");
            slot.append(String8::format("%d", (m_slot_id+1)));
            aospPuk = String8::format("%s%s", slot.string(), ".num.puk1");
        } else {
            aospPuk = String8::format("%s", "gsm.sim.num.puk");
        }
        rfx_property_set(aospPuk, String8::format("%d", retry_count).string());
    }
}

void RmmSimBaseHandler::setPuk2RetryCountProp(int retry_count) {
    String8 puk2("vendor.gsm.sim.retry.puk2");
    puk2.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id+1)));
    rfx_property_set(puk2.string(), String8::format("%d", retry_count).string());

    if (isAOSPPropSupport()) {
        String8 aospPuk2;

        if (RfxRilUtils::rfxGetSimCount() >= 2) {
            String8 slot("gsm.slot");
            slot.append(String8::format("%d", (m_slot_id+1)));
            aospPuk2 = String8::format("%s%s", slot.string(), ".num.puk2");
        } else {
            aospPuk2 = String8::format("%s", "gsm.sim.num.puk2");
        }
        rfx_property_set(aospPuk2, String8::format("%d", retry_count).string());
    }
}

char* RmmSimBaseHandler::stringToUpper(char *str) {
    char *p = str;
    for (; *p != '\0'; p++) {
        *p = toupper(*p);
    }
    return str;
}

bool RmmSimBaseHandler::isAOSPPropSupport() {
    return true;
}

bool RmmSimBaseHandler::isSimIoFcp(char* response) {
    bool isFcp = false;

    if (NULL != response && strlen(response) >= 2) {
        if ('6' == response[0] && '2' == response[1]) {
            isFcp = true;
        }
    }

    return isFcp;
}

void RmmSimBaseHandler::sendSimStatusChanged() {
    logD(mTag, "sendSimStatusChanged, shall has RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED");
    //sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
    //        m_slot_id, RfxVoidData());
    //responseToTelCore(unsol);
}

void RmmSimBaseHandler::updateCdmaCardType(const char *iccid) {
    int cardType = UNKOWN_CARD;
    String8 cdmaCardType("vendor.ril.cdma.card.type");
    cdmaCardType.append(String8::format(".%d", (m_slot_id + 1)));
    int aospType = -1;

    bool cdmalockedcard =  getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_CDMA_LOCKED_CARD);

    // Card is locked if ESIMS:0,16 is reported.
    if (cdmalockedcard == true) {
        cardType = LOCKED_CARD;
    } else {
        int eusim = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
        // Card is absent if iccid is NULL
        if (iccid == NULL) {
            cardType = CARD_NOT_INSERTED;
        } else {
            if (isOp09Card(iccid)) {
                // OP09 card type
                if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_CDMA3G_DUALMODE_CARD)
                        == 1) {
                    // OP09 3G dual mode card.
                    cardType = CT_UIM_SIM_CARD;
                } else {
                    if (eusim < 0) {
                        cardType = UNKOWN_CARD;
                        logE(mTag, "updateCdmaCardType, invalid CDMA card type: %d !", cardType);
                    } else if (eusim == 0) {
                        // Rarely happen, iccid exists but card type is empty.
                        cardType = CARD_NOT_INSERTED;
                    } else if (((eusim & RFX_CARD_TYPE_SIM) == 0) &&
                            ((eusim & RFX_CARD_TYPE_USIM) == 0) &&
                            ((eusim & RFX_CARD_TYPE_ISIM) == 0)) {
                        // OP09 3G single mode card.
                        cardType = CT_3G_UIM_CARD;
                    } else if (((eusim & RFX_CARD_TYPE_CSIM) != 0) &&
                            ((eusim & RFX_CARD_TYPE_USIM) != 0)) {
                        // Typical OP09 4G dual mode card.
                        cardType = CT_4G_UICC_CARD;
                    } else if (eusim == RFX_CARD_TYPE_SIM) {
                        // For non-C2K supported project, CT 3G card type is reported as
                        // "SIM" card and "+ECT3G" value is false.
                        cardType = CT_UIM_SIM_CARD;
                    } else if (((eusim & RFX_CARD_TYPE_USIM) != 0) &&
                            (strStartsWith(iccid, "8985231"))) {
                        // OP09 CTEXCEL card.
                        cardType = CT_EXCEL_GG_CARD;
                    } else if ((eusim == RFX_CARD_TYPE_USIM) ||
                            (eusim == (RFX_CARD_TYPE_USIM | RFX_CARD_TYPE_ISIM))) {
                        if (!RatConfig_isC2kSupported()) {
                            // CT 4G card
                            cardType = CT_4G_UICC_CARD;
                        } else {
                            // SIM card.
                            cardType = SIM_CARD;
                        }
                    } else {
                        cardType = UNKOWN_CARD;
                        logE(mTag, "updateCdmaCardType, invalid CDMA card type: %d !", cardType);
                    }
                }
            } else {
                // Non-OP09 card type.
                if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_CDMA3G_DUALMODE_CARD)
                        == 1) {
                    // Non-OP09 CDMA 3G dual mode card.
                    cardType = UIM_SIM_CARD;
                } else {
                    int eusim = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);

                    if (eusim < 0) {
                        cardType = UNKOWN_CARD;
                        logE(mTag, "updateCdmaCardType, invalid CDMA card type: %d !", cardType);
                    } else if (eusim == 0) {
                        // Rarely happen, iccid exists but card type is empty.
                        cardType = CARD_NOT_INSERTED;
                    } else if (((eusim & RFX_CARD_TYPE_SIM) == 0) &&
                            ((eusim & RFX_CARD_TYPE_USIM) == 0) &&
                            ((eusim & RFX_CARD_TYPE_ISIM) == 0)) {
                        // Non-OP09 3G single mode card.
                        cardType = UIM_CARD;
                    } else if (((eusim & RFX_CARD_TYPE_CSIM) != 0) &&
                            ((eusim & RFX_CARD_TYPE_USIM) != 0)) {
                        if (strStartsWith(iccid, "898601")) {
                            cardType = CT_4G_UICC_CARD;
                        } else {
                            // Typical non-OP09 4G dual mode card.
                            cardType = NOT_CT_UICC_CARD;
                        }
                        // Typical non-OP09 4G dual mode card.
                        cardType = NOT_CT_UICC_CARD;
                    } else if (eusim == RFX_CARD_TYPE_SIM) {
                        // SIM card.
                        cardType = SIM_CARD;
                    } else if ((eusim == RFX_CARD_TYPE_USIM) ||
                            (eusim == (RFX_CARD_TYPE_USIM | RFX_CARD_TYPE_ISIM))) {
                        // Wait Gsm +ESIMAPP: reported and set CDMA card type again ALPS04691985.
                        // ALPS08656734 sim lock no cdma card type status
                        cardType = SIM_CARD;
                    } else {
                        cardType = UNKOWN_CARD;
                        logE(mTag, "updateCdmaCardType, invalid CDMA card type: %d !", cardType);
                    }
                }
            }
        }
    }

    // Set cdma card type.
    rfx_property_set(cdmaCardType, String8::format("%d", cardType).string());
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE, cardType);

    if (isAOSPPropSupport()) {
        String8 simType("gsm.sim");
        simType.append(String8::format("%d%s", (m_slot_id + 1), ".type"));

        if (cardType ==  SIM_CARD) {
            if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE)
                    == RFX_CARD_TYPE_SIM) {
                aospType = SINGLE_MODE_SIM_CARD;
            } else if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE)
                    == RFX_CARD_TYPE_USIM) {
                aospType = SINGLE_MODE_USIM_CARD;
            }
        } else if ((cardType ==  UIM_CARD) || (cardType ==  CT_3G_UIM_CARD)) {
            aospType = SINGLE_MODE_RUIM_CARD;
        } else if ((cardType ==  UIM_SIM_CARD) || (cardType ==  CT_UIM_SIM_CARD)) {
            if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE)
                    == RFX_CARD_TYPE_SIM) {
                aospType = SINGLE_MODE_SIM_CARD;
            } else if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE)
                    == RFX_CARD_TYPE_USIM) {
                aospType = SINGLE_MODE_USIM_CARD;
            } else {
                aospType = SINGLE_MODE_RUIM_CARD;
            }
        } else if (cardType ==  CT_EXCEL_GG_CARD) {
            aospType = CT_NATIONAL_ROAMING_CARD;
        }

        rfx_property_set(simType, String8::format("%d", aospType).string());
    }
    logD(mTag, "updateCdmaCardType, cdmaCardType:%d, aospType:%d !", cardType, aospType);
}

bool RmmSimBaseHandler::isOp09Card(const char *iccid) {
    bool isOp09Card = false;

    // Compare with OP09 iccid prefix.
    if ((strStartsWith(iccid, "898603")) ||
            (strStartsWith(iccid, "898611")) ||
            (strStartsWith(iccid, "8985302")) ||
            (strStartsWith(iccid, "8985307")) ||
            (strStartsWith(iccid, "8985231"))) {
        isOp09Card = true;
    }
    return isOp09Card;
}

void RmmSimBaseHandler::makeSimRspFromUsimFcp(unsigned char ** simResponse, unsigned short fcpLen, int fileId) {
     RFX_UNUSED(fileId);
    int format_wrong = 0;
    unsigned char * fcpByte = NULL;
    //unsigned short fcpLen = 0;
    usim_file_descriptor_struct fDescriptor = {0, 0, 0, 0};
    usim_file_size_struct fSize  = {0};
    unsigned char simRspByte[GET_RESPONSE_EF_SIZE_BYTES] = {0};

    //fcpLen = hexStringToByteArray(*simResponse, &fcpByte); ????TODO
    fcpByte = *simResponse;
    //fcpLen = strlen((const char*)fcpByte);

    if (FALSE == usim_fcp_query_tag(fcpByte, fcpLen, FCP_FILE_DES_T, &fDescriptor)) {
        logE(mTag, "makeSimRspFromUsimFcp, USIM FD Parse fail:%s", *simResponse);
        format_wrong = 1;
        goto done;
    }

    if ((!IS_DF_ADF(fDescriptor.fd)) && (FALSE == usim_fcp_query_tag(fcpByte, fcpLen,
            FCP_FILE_SIZE_T, &fSize))) {
        logE(mTag, "makeSimRspFromUsimFcp, USIM File Size fail:%s", *simResponse);
        format_wrong = 1;
        goto done;
    }

    if (IS_DF_ADF(fDescriptor.fd)) {
        simRspByte[RESPONSE_DATA_FILE_TYPE] = TYPE_DF;
        goto done;
    } else {
        simRspByte[RESPONSE_DATA_FILE_TYPE] = TYPE_EF;
    }

    simRspByte[RESPONSE_DATA_FILE_SIZE_1] = (fSize.file_size & 0xFF00) >> 8;
    simRspByte[RESPONSE_DATA_FILE_SIZE_2] = fSize.file_size & 0xFF;

    if (IS_LINEAR_FIXED_EF(fDescriptor.fd)) {
        simRspByte[RESPONSE_DATA_STRUCTURE] = EF_TYPE_LINEAR_FIXED;
        simRspByte[RESPONSE_DATA_RECORD_LENGTH] = fDescriptor.rec_len;
    } else if (IS_TRANSPARENT_EF(fDescriptor.fd)) {
        simRspByte[RESPONSE_DATA_STRUCTURE] = EF_TYPE_TRANSPARENT;

    } else if (IS_CYCLIC_EF(fDescriptor.fd)) {
        simRspByte[RESPONSE_DATA_STRUCTURE] = EF_TYPE_CYCLIC;
        simRspByte[RESPONSE_DATA_RECORD_LENGTH] = fDescriptor.rec_len;
    }

done:
    free(*simResponse);
    //free(fcpByte); ???
    if (format_wrong != 1) {
        *simResponse = byteArrayToHexString(simRspByte, GET_RESPONSE_EF_SIZE_BYTES);
        if (RfxRilUtils::isEngLoad()) {
            logD(mTag, "makeSimRspFromUsimFcp, simRsp done:%s", *simResponse);
        }
    } else {
        *simResponse = NULL;
        logE(mTag, "makeSimRspFromUsimFcp, simRsp is null because command format may be wrong");
    }
}

void RmmSimBaseHandler::resetSimPropertyAndStatusKey() {
    // reset CDMA systemProperty and statusKey
    String8 cdmaMccMnc("vendor.cdma.ril.uicc.mccmnc");
    cdmaMccMnc.append((m_slot_id == 0)? "" : String8::format(".%d", m_slot_id));
    rfx_property_set(cdmaMccMnc.string(), "");
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_UICC_CDMA_NUMERIC, String8(""));

    String8 cdmaSubscriberId("vendor.ril.uim.subscriberid");
    cdmaSubscriberId.append(String8::format(".%d", (m_slot_id + 1)));
    rfx_property_set(cdmaSubscriberId.string(), "");
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_C2K_IMSI, String8(""));

    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_CDMA_SPN, String8(""));
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_UICC_SUB_CHANGED_STATUS, -1);

    // reset GSM systemProperty and statusKey
    String8 pin1("vendor.gsm.sim.retry.pin1");
    String8 pin2("vendor.gsm.sim.retry.pin2");
    String8 puk1("vendor.gsm.sim.retry.puk1");
    String8 puk2("vendor.gsm.sim.retry.puk2");
    pin1.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
    pin2.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
    puk1.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
    puk2.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
    rfx_property_set(pin1.string(), "");
    rfx_property_set(pin2.string(), "");
    rfx_property_set(puk1.string(), "");
    rfx_property_set(puk2.string(), "");

    String8 gsmMccMnc("vendor.gsm.ril.uicc.mccmnc");
    gsmMccMnc.append((m_slot_id == 0)? "" : String8::format(".%d", m_slot_id));
    rfx_property_set(gsmMccMnc.string(), "");
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_UICC_GSM_NUMERIC,
            String8(""));

    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_IMSI, String8(""));
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_IMPI, String8(""));
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_SPN, String8(""));
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_GID1, String8(""));
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_PNN, String8(""));
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_CACHE_FCP, String8(""));
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_CACHE_BINARY, String8(""));

    if (isAOSPPropSupport()) {
        String8 iccidPre("gsm.sim.preiccid_");
        String8 lockCount("gsm.sim");
        String8 aospPin;
        String8 aospPin2;
        String8 aospPuk;
        String8 aospPuk2;
        String8 simType("gsm.sim");
        String8 mlplMspl("ril.csim.mlpl_mspl_ver");

        mlplMspl.append(String8::format("%d", m_slot_id));
        iccidPre.append(String8::format("%d", m_slot_id));
        lockCount.append((m_slot_id == 0)? ".num.simlock" : String8::format("%d%s", m_slot_id,
                ".num.simlock"));
        simType.append(String8::format("%d%s", (m_slot_id + 1), ".type"));
        if (RfxRilUtils::rfxGetSimCount() >= 2) {
            String8 slot("gsm.slot");
            slot.append(String8::format("%d", (m_slot_id+1)));
            aospPin = String8::format("%s%s", slot.string(), ".num.pin1");
            aospPin2 = String8::format("%s%s", slot.string(), ".num.pin2");
            aospPuk = String8::format("%s%s", slot.string(), ".num.puk1");
            aospPuk2 = String8::format("%s%s", slot.string(), ".num.puk2");
        } else {
            aospPin = String8::format("%s", "gsm.sim.num.pin");
            aospPin2 = String8::format("%s", "gsm.sim.num.pin2");
            aospPuk = String8::format("%s", "gsm.sim.num.puk");
            aospPuk2 = String8::format("%s", "gsm.sim.num.puk2");
        }

        rfx_property_set(mlplMspl, ",");
        rfx_property_set(iccidPre, "");
        rfx_property_set(lockCount, "");
        rfx_property_set(simType, "");
        rfx_property_set(aospPin, "");
        rfx_property_set(aospPin2, "");
        rfx_property_set(aospPuk, "");
        rfx_property_set(aospPuk2, "");
    }
}

RIL_SimSlotStatus** RmmSimBaseHandler::decodeMepSlotsinfo(sp<RfxMipcData> spData,
        RfxMipcData *mipc_data, int *decodeerr, int *slotnum) {
    int slotNum = 0;
    RIL_SimSlotStatus *p_slot_status = NULL;
    RIL_SimSlotStatus **pp_slot_status = NULL;
    *decodeerr = -1;
    char *p_atr = NULL;
    char *p_eid = NULL;
    char *tempStr = NULL;

    if (spData != NULL) {
        slotNum = spData->getMipcUint8Val(MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PHY_SLOT_COUNT, 0);
    } else {
        slotNum = mipc_data->getMipcUint8Val(MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PHY_SLOT_COUNT, 0);
    }

    if (slotNum == 0 || slotNum > 4) {
        goto error;
    }

    pp_slot_status = (RIL_SimSlotStatus **)calloc(slotNum, sizeof(RIL_SimSlotStatus *));
    RFX_ASSERT(pp_slot_status != NULL);

    p_slot_status = (RIL_SimSlotStatus *)calloc(slotNum, sizeof(RIL_SimSlotStatus));
    RFX_ASSERT(p_slot_status != NULL);

    memset(p_slot_status, 0, slotNum * sizeof(RIL_SimSlotStatus));

    p_atr = (char *)calloc(slotNum, (sizeof(char)* (MIPC_MAX_ATR_LEN + 1)));
    RFX_ASSERT(p_atr != NULL);
    memset(p_atr, 0, slotNum * (MIPC_MAX_ATR_LEN + 1));

    p_eid = (char *)calloc(slotNum, (sizeof(char)* (MIPC_MAX_SIM_EID_LEN + 1)));
    RFX_ASSERT(p_eid != NULL);
    memset(p_eid, 0, slotNum * (MIPC_MAX_SIM_EID_LEN + 1));

    /* init the pointer array */
    for (int i = 0; i < slotNum; i++) {
        pp_slot_status[i] = &(p_slot_status[i]);
    }

    for (int i = 0; i < slotNum; i++) {
        p_slot_status[i].atr = &p_atr[i * (MIPC_MAX_ATR_LEN + 1)];
        p_slot_status[i].eid = &p_eid[i * (MIPC_MAX_SIM_EID_LEN + 1)];
        uint16_t valLen = 0;

        if (spData != NULL) {
            p_slot_status[i].mepVersion = 1;
            p_slot_status[i].card_state = (RIL_CardState)spData->getMipcUint8IdxVal(
                    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_CARD_STATE, i, 0);

            tempStr = (char*)spData->getMipcIdxVal(
                    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_ATR, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_MAX_ATR_LEN + 1)) {
                memcpy(p_slot_status[i].atr, tempStr, valLen);
                p_slot_status[i].atr[MIPC_MAX_ATR_LEN] = '\0';
            }

            tempStr = (char*)spData->getMipcIdxVal(
                    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_EID, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_MAX_SIM_EID_LEN + 1)) {
                memcpy(p_slot_status[i].eid, tempStr, valLen);
                p_slot_status[i].eid[MIPC_MAX_SIM_EID_LEN] = '\0';
            }

            uint16_t potInfoType = spData->getMipcIdxType(
                    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PORT_INFO, i);
            uint16_t portCount = spData->getMipcArrayTlvNum(potInfoType);
            uint16_t portTotal = portCount;
            if (p_slot_status[i].card_state == RIL_CARDSTATE_ABSENT) {
                portTotal = (portCount == 0)? 1: portCount;
            }
            p_slot_status[i].port_count = portTotal;
            for (int j = 0; j < portTotal && j < MAX_RFX_SLOT_COUNT; j++) {
                /// Generate inactive port for e-sim and absent case
                if (j >= portCount) {
                    p_slot_status[i].info[j].logicalSlotId = -1;
                    p_slot_status[i].info[j].portActive = 0;
                    continue;
                }

                tempStr = (char*) spData->getMipcIdxVal(potInfoType, j, &valLen);
                if (tempStr != NULL && valLen > 0 && valLen <= sizeof(RIL_SimPortInfo)) {
                    memcpy(&p_slot_status[i].info[j], tempStr, valLen);
                }

                if (p_slot_status[i].info[j].emptyEuicc > 0) {
                    memset(p_slot_status[i].info[j].iccId, 0, MIPC_SIM_FIX_ICCID_STR_LEN);
                }

                if(strlen(p_slot_status[i].info[j].iccId) != 0) {
                    stringToUpper(p_slot_status[i].info[j].iccId);
                }
            }
            if (p_slot_status[i].eid[0] != '\0') {
                p_slot_status[i].mepMode = (RIL_MepMode) spData->getMipcUint8Val(
                            MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_MEP_MODE, 0);
            } else {
                p_slot_status[i].mepMode = RIL_MEP_MODE_NONE;
            }
            logD(mTag, "decodeMepSlotsinfo, slot:%d, portInfoType:%d, portCount:%d, portTotal:%d, mepMode:%d, eid[0]:%d",
                    i, potInfoType, portCount, portTotal, p_slot_status[i].mepMode, p_slot_status[i].eid[0]);
        } else {
            p_slot_status[i].mepVersion = 1;
            p_slot_status[i].card_state = (RIL_CardState)mipc_data->getMipcUint8IdxVal(
                    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_CARD_STATE, i, 0);

            tempStr = (char*)mipc_data->getMipcIdxVal(
                    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_ATR, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_MAX_ATR_LEN + 1)) {
                memcpy(p_slot_status[i].atr, tempStr, valLen);
                p_slot_status[i].atr[MIPC_MAX_ATR_LEN] = '\0';
            }

            tempStr = (char*)mipc_data->getMipcIdxVal(
                    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_EID, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_MAX_SIM_EID_LEN + 1)) {
                memcpy(p_slot_status[i].eid, tempStr, valLen);
                p_slot_status[i].eid[MIPC_MAX_SIM_EID_LEN] = '\0';
            }

            /// int portCount = mipc_data->getMipcUint8IdxVal(
            ///         MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PORT_NUM, i, 0);
            /// logD(mTag, "decodeMepSlotsinfo, portCount:%d", portCount);
            uint16_t potInfoType = mipc_data->getMipcIdxType(
                    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PORT_INFO, i);
            uint16_t portCount = mipc_data->getMipcArrayTlvNum(potInfoType);
            uint16_t portTotal = portCount;
            if (p_slot_status[i].card_state == RIL_CARDSTATE_ABSENT) {
                portTotal = (portCount == 0)? 1: portCount;
            }
            p_slot_status[i].port_count = portTotal;
            for (int j = 0; j < portTotal && j < MAX_RFX_SLOT_COUNT; j++) {
                /// Generate inactive port for e-sim and absent case
                if (j >= portCount) {
                    p_slot_status[i].info[j].logicalSlotId = -1;
                    p_slot_status[i].info[j].portActive = 0;
                    continue;
                }

                tempStr = (char*) mipc_data->getMipcIdxVal(potInfoType, j, &valLen);
                if (tempStr != NULL && valLen > 0 && valLen <= sizeof(RIL_SimPortInfo)) {
                    memcpy(&p_slot_status[i].info[j], tempStr, valLen);
                }
                logD(mTag, "decodeMepSlotsinfo, value size: %d, structure size: %d",
                        valLen, sizeof(RIL_SimPortInfo));

                if (p_slot_status[i].info[j].emptyEuicc > 0) {
                    memset(p_slot_status[i].info[j].iccId, 0, MIPC_SIM_FIX_ICCID_STR_LEN);
                }

                if(strlen(p_slot_status[i].info[j].iccId) != 0) {
                    stringToUpper(p_slot_status[i].info[j].iccId);
                }
            }

            if (p_slot_status[i].eid[0] != '\0') {
                p_slot_status[i].mepMode = (RIL_MepMode) mipc_data->getMipcUint8Val(
                        MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_MEP_MODE, 0);
            } else {
                p_slot_status[i].mepMode = RIL_MEP_MODE_NONE;
            }
            logD(mTag, "decodeMepSlotsinfo, slot:%d, portInfoType:%d, portCount:%d, portTotal:%d, mepMode:%d, eid[0]:%d",
                    i, potInfoType, portCount, portTotal, p_slot_status[i].mepMode, p_slot_status[i].eid[0]);
        }

        bool eidExist = (p_slot_status[i].eid != NULL) && (strlen(p_slot_status[i].eid) > 0);
        for (int j = 0; j < p_slot_status[i].port_count && j < MAX_RFX_SLOT_COUNT; j++) {
            int logicSlotId = p_slot_status[i].info[j].logicalSlotId;
            if ((logicSlotId >= 0) && (logicSlotId < RfxRilUtils::rfxGetSimCount())) {
                if ((p_slot_status[i].info[j].portActive != 0) && eidExist) {
                    getMclStatusManager(logicSlotId)->setBoolValue(
                            RFX_STATUS_KEY_IS_EUICC_CARD, true);
                    getMclStatusManager(logicSlotId)->setString8Value(
                            RFX_STATUS_KEY_SIM_EID,
                            String8::format("%s", p_slot_status[i].eid));

                } else {
                    getMclStatusManager(logicSlotId)->setBoolValue(
                            RFX_STATUS_KEY_IS_EUICC_CARD, false);
                    getMclStatusManager(logicSlotId)->setString8Value(
                            RFX_STATUS_KEY_SIM_EID, String8("N/A"));
                }
            }
            logD(mTag, "decodeMepSlotsinfo, phy slot[%d], port:%d, logicalSlotId:%d, iccid:%.7s",
                    i, j, logicSlotId,
                    p_slot_status[i].info[j].iccId);
        }

        logD(mTag, "decodeMepSlotsinfo, [%d] cardState:%d, atr:%.6s, eid:%.6s",
                i, p_slot_status[i].card_state, p_slot_status[i].atr, p_slot_status[i].eid);
    }

    *decodeerr = RIL_E_SUCCESS;

error:
    *slotnum = slotNum;
    logD(mTag, "decodeMepSlotsinfo, decodeerr:%d, slotnum:%d, pp_slot_status:%p",
             *decodeerr, *slotnum, pp_slot_status);
    return pp_slot_status;
}

RIL_SimSlotStatus** RmmSimBaseHandler::decodeEslotsinfo(sp<RfxMipcData> spData, RfxMipcData *mipc_data,
        int *decodeerr, int *slotnum) {
    int slotNum = 0;
    RIL_SimSlotStatus *p_slot_status = NULL;
    RIL_SimSlotStatus **pp_slot_status = NULL;
    *decodeerr = -1;
    char *p_atr = NULL;
    char *p_eid = NULL;
    char *p_iccid = NULL;
    char *tempStr = NULL;

    if (spData != NULL) {
        slotNum = spData->getMipcUint8Val(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_INFO_COUNT, 0);
    } else {
        slotNum = mipc_data->getMipcUint8Val(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_INFO_COUNT, 0);
    }

    if (slotNum == 0 || slotNum > 4) {
        goto error;
    }

    pp_slot_status = (RIL_SimSlotStatus **)calloc(slotNum, sizeof(RIL_SimSlotStatus *));
    RFX_ASSERT(pp_slot_status != NULL);
    p_slot_status = (RIL_SimSlotStatus *)calloc(slotNum, sizeof(RIL_SimSlotStatus));
    RFX_ASSERT(p_slot_status != NULL);
    memset(p_slot_status, 0, slotNum * sizeof(RIL_SimSlotStatus));
    p_atr = (char *)calloc(slotNum, (sizeof(char)* (MIPC_MAX_ATR_LEN + 1)));
    RFX_ASSERT(p_atr != NULL);
    memset(p_atr, 0, slotNum * (MIPC_MAX_ATR_LEN + 1));
    p_eid = (char *)calloc(slotNum, (sizeof(char)* (MIPC_MAX_SIM_EID_LEN + 1)));
    RFX_ASSERT(p_eid != NULL);
    memset(p_eid, 0, slotNum * (MIPC_MAX_SIM_EID_LEN + 1));
    p_iccid = (char *)calloc(slotNum, (sizeof(char)* (MIPC_SIM_FIX_ICCID_LEN + 1)));
    RFX_ASSERT(p_iccid != NULL);
    memset(p_iccid, 0, slotNum * (MIPC_SIM_FIX_ICCID_LEN + 1));

    /* init the pointer array */
    for (int i = 0; i < slotNum; i++) {
        pp_slot_status[i] = &(p_slot_status[i]);
    }

    for (int i = 0; i < slotNum; i++) {
        p_slot_status[i].atr = &(p_atr[i*(MIPC_MAX_ATR_LEN + 1)]);
        p_slot_status[i].eid = &(p_eid[i*(MIPC_MAX_SIM_EID_LEN + 1)]);
        p_slot_status[i].iccId = &(p_iccid[i*(MIPC_SIM_FIX_ICCID_LEN + 1)]);

        if (spData != NULL) {
            p_slot_status[i].card_state =
                (RIL_CardState)spData->getMipcInt8IdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_CARD_STATE, i, 0);
            p_slot_status[i].slotState =
                (RIL_SlotState)spData->getMipcInt8IdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_STATE, i, 0);
            p_slot_status[i].logicalSlotId =
                (uint32_t)spData->getMipcInt8IdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_LOGICAL_IDX, i, 0);

            uint16_t valLen = 0;
            tempStr =
                (char*)spData->getMipcIdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_ATR, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_MAX_ATR_LEN + 1)) {
                memcpy(p_slot_status[i].atr, tempStr, valLen);
                p_slot_status[i].atr[MIPC_MAX_ATR_LEN] = '\0';
            }

            tempStr =
                (char*)spData->getMipcIdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_EID, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_MAX_SIM_EID_LEN + 1)) {
                memcpy(p_slot_status[i].eid, tempStr, valLen);
                p_slot_status[i].eid[MIPC_MAX_SIM_EID_LEN] = '\0';
            }

            tempStr =
                (char*)spData->getMipcIdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_ICCID, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_SIM_FIX_ICCID_LEN + 1)) {
                memcpy(p_slot_status[i].iccId, tempStr, valLen);
                p_slot_status[i].iccId[MIPC_SIM_FIX_ICCID_LEN] = '\0';
            }
            //logD(mTag, "decodeEslotsinfo[%d], valLen:%d, iccid:%.7s", i, valLen, tempStr);
        } else {
            p_slot_status[i].card_state =
                (RIL_CardState)mipc_data->getMipcInt8IdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_CARD_STATE, i, 0);
            p_slot_status[i].slotState =
                (RIL_SlotState)mipc_data->getMipcInt8IdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_STATE, i, 0);
            p_slot_status[i].logicalSlotId =
                (uint32_t)mipc_data->getMipcInt8IdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_LOGICAL_IDX, i, 0);

            uint16_t valLen = 0;
            tempStr =
                (char*)mipc_data->getMipcIdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_ATR, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_MAX_ATR_LEN + 1)) {
                memcpy(p_slot_status[i].atr, tempStr, valLen);
                p_slot_status[i].atr[MIPC_MAX_ATR_LEN] = '\0';
            }

            tempStr =
                (char*)mipc_data->getMipcIdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_EID, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_MAX_SIM_EID_LEN + 1)) {
                memcpy(p_slot_status[i].eid, tempStr, valLen);
                p_slot_status[i].eid[MIPC_MAX_SIM_EID_LEN] = '\0';
            }

            tempStr =
                (char*)mipc_data->getMipcIdxVal(MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_ICCID, i, &valLen);
            if (tempStr != NULL && valLen > 0  && valLen <= (MIPC_SIM_FIX_ICCID_LEN + 1)) {
                memcpy(p_slot_status[i].iccId, tempStr, valLen);
                p_slot_status[i].iccId[MIPC_SIM_FIX_ICCID_LEN] = '\0';
            }
            //logD(mTag, "decodeEslotsinfo[%d], valLen:%d, iccid:%.7s", i, valLen, tempStr);
        }

        if (i < RfxRilUtils::rfxGetSimCount()) {
            if (p_slot_status[i].eid != NULL && strlen(p_slot_status[i].eid) > 0) {
                getMclStatusManager(i)->setBoolValue(RFX_STATUS_KEY_IS_EUICC_CARD, true);
                getMclStatusManager(i)->setString8Value(RFX_STATUS_KEY_SIM_EID,
                        String8::format("%s", p_slot_status[i].eid));
            } else {
                getMclStatusManager(i)->setBoolValue(RFX_STATUS_KEY_IS_EUICC_CARD, false);
                getMclStatusManager(i)->setString8Value(RFX_STATUS_KEY_SIM_EID, String8("N/A"));
            }
        }

        // Convert all the lower case characters in iccid to upper case
        if (strlen(p_slot_status[i].iccId) != 0) {
            stringToUpper(p_slot_status[i].iccId);
        }

        logD(mTag, "decodeEslotsinfo, [%d] cardState:%d, atr:%.6s, eid:%.6s, iccid:%.7s",
             i, p_slot_status[i].card_state,
             p_slot_status[i].atr, p_slot_status[i].eid, p_slot_status[i].iccId);
    }

    *decodeerr = RIL_E_SUCCESS;

error:
    *slotnum = slotNum;
    logD(mTag, "decodeEslotsinfo, decodeerr:%d, slotnum:%d, pp_slot_status:%p",
             *decodeerr, *slotnum, pp_slot_status);
    return pp_slot_status;
}

/*
RIL_SimSlotStatus** RmmSimBaseHandler::decodeEslotsTlvInfo(sp<RfxMipcData> spData, RfxMipcData *mipc_data,
        int *decodeerr, int *slotnum) {
    int err = -1;
    int slotNum = 0;
    RIL_SimSlotStatus *p_slot_status = NULL;
    RIL_SimSlotStatus **pp_slot_status = NULL;
    *decodeerr = -1;
    char *p_atr = NULL;
    char *p_eid = NULL;
    char *p_iccid = NULL;
    uint16_t info_len = 0;
    mipc_sim_slots_info_struct4 slot_info[4];
    void* slot_data = NULL;
    uint8_t count = 0;

    //if (MIPC_RESULT_SUCCESS != msg.getResult()) {
    //    goto error;
    //}

    if (spData != NULL) {
        slotNum = spData->getMipcUint8Val(MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_COUNT, 0);
    } else {
        slotNum = mipc_data->getMipcUint8Val(MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_COUNT, 0);
    }

    if (slotNum == 0 || slotNum > 4) {
        goto error;
    }
    // Just report slot info with valid logical slotId.
    // TODO: it should be udpated if AOSP changes and needs to reports all of slot infos.
    //if (slotNum > RfxRilUtils::rfxGetSimCount()) {
        //slotNum = RfxRilUtils::rfxGetSimCount();
    //}

    pp_slot_status = (RIL_SimSlotStatus **)calloc(slotNum, sizeof(RIL_SimSlotStatus *));
    RFX_ASSERT(pp_slot_status != NULL);
    p_slot_status = (RIL_SimSlotStatus *)calloc(slotNum, sizeof(RIL_SimSlotStatus));
    RFX_ASSERT(p_slot_status != NULL);
    memset(p_slot_status, 0, slotNum * sizeof(RIL_SimSlotStatus));
    p_atr = (char *)calloc(slotNum, (sizeof(char)* (MIPC_MAX_ATR_LEN + 1)));
    RFX_ASSERT(p_atr != NULL);
    memset(p_atr, 0, slotNum * (MIPC_MAX_ATR_LEN + 1));
    p_eid = (char *)calloc(slotNum, (sizeof(char)* (MIPC_MAX_SIM_EID_LEN + 1)));
    RFX_ASSERT(p_eid != NULL);
    memset(p_eid, 0, slotNum * (MIPC_MAX_SIM_EID_LEN + 1));
    p_iccid = (char *)calloc(slotNum, (sizeof(char)* (MIPC_SIM_FIX_ICCID_LEN + 1)));
    RFX_ASSERT(p_iccid != NULL);
    memset(p_iccid, 0, slotNum * (MIPC_SIM_FIX_ICCID_LEN + 1));

    if (spData != NULL) {
        slot_data = spData->getMipcVal(MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_LIST, &info_len);
    } else {
        slot_data = mipc_data->getMipcVal(MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_LIST, &info_len);
    }

    memcpy(slot_info, slot_data, info_len);
    // init the pointer array
    for (int i = 0; i < slotNum; i++) {
        pp_slot_status[i] = &(p_slot_status[i]);
    }

    for (int i = 0; i < slotNum; i++) {
        char *pinState = slot_info[i].card_state;
        pinState[19] = '\0';
        if (i < RfxRilUtils::rfxGetSimCount() &&
                (strstr(pinState, "+CME ERROR: 14") != NULL ||
                strstr(pinState, "+CME ERROR:14") != NULL)) {
            *decodeerr = RIL_E_SIM_BUSY;
        }

        p_slot_status[i].slotState = (RIL_SlotState)slot_info[i].slots_state;
        p_slot_status[i].logicalSlotId = (uint32_t) slot_info[i].logical_idx;

        p_slot_status[i].atr = &(p_atr[i*(MIPC_MAX_ATR_LEN + 1)]);
        memcpy(p_slot_status[i].atr, slot_info[i].atr, MIPC_MAX_ATR_LEN);
        p_slot_status[i].atr[MIPC_MAX_ATR_LEN] = '\0';
        p_slot_status[i].eid = &(p_eid[i*(MIPC_MAX_SIM_EID_LEN + 1)]);
        memcpy(p_slot_status[i].eid, slot_info[i].eid, MIPC_MAX_SIM_EID_LEN);
        p_slot_status[i].eid[MIPC_MAX_SIM_EID_LEN] = '\0';

        if (i < RfxRilUtils::rfxGetSimCount()) {
            if (!((p_slot_status[i].eid == NULL) || (strlen(p_slot_status[i].eid) == 0))) {
                getMclStatusManager(i)->setBoolValue(RFX_STATUS_KEY_IS_EUICC_CARD, true);
                getMclStatusManager(i)->setString8Value(RFX_STATUS_KEY_SIM_EID,
                        String8::format("%s", p_slot_status[i].eid));
            } else {
                getMclStatusManager(i)->setBoolValue(RFX_STATUS_KEY_IS_EUICC_CARD, false);
                getMclStatusManager(i)->setString8Value(RFX_STATUS_KEY_SIM_EID, String8("N/A"));
            }
        }

        p_slot_status[i].iccId = &(p_iccid[i*(MIPC_SIM_FIX_ICCID_LEN + 1)]);
        memcpy(p_slot_status[i].iccId, slot_info[i].iccid, MIPC_SIM_FIX_ICCID_LEN);
        p_slot_status[i].iccId[MIPC_SIM_FIX_ICCID_LEN] = '\0';
        // Convert all the lower case characters in iccid to upper case
        if (strlen(p_slot_status[i].iccId) != 0) {
            stringToUpper(p_slot_status[i].iccId);
        }
        //+CPIN: CARD_RESTRICTED
        if (strstr(pinState, "+CPIN: CARD_RESTRIC") != NULL) {
            p_slot_status[i].card_state = RIL_CARDSTATE_RESTRICTED;
        } else if (((p_slot_status[i].eid != NULL) && (strlen(p_slot_status[i].eid) > 0))
                || ((p_slot_status[i].iccId != NULL) && (strlen(p_slot_status[i].iccId) > 0))) {
            p_slot_status[i].card_state = RIL_CARDSTATE_PRESENT;
        } else {
            p_slot_status[i].card_state = RIL_CARDSTATE_ABSENT;
        }

        logD(mTag, "decodeEslotsinfo, [%d] cpinState:%s, atr:%.6s, eid:%.6s, iccid:%.7s",
             i, pinState, p_slot_status[i].atr, p_slot_status[i].eid, p_slot_status[i].iccId);
    }


    if (*decodeerr != RIL_E_SIM_BUSY) {
        *decodeerr = RIL_E_SUCCESS;
    }

error:
    *slotnum = slotNum;
    logD(mTag, "decodeEslotsinfo, decodeerr:%d, slotnum:%d, pp_slot_status:%p",
             *decodeerr, *slotnum, pp_slot_status);
    return pp_slot_status;
}
*/
void RmmSimBaseHandler::sendIccidChanged(const char *iccid) {
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_ICCID_CHANGED, m_slot_id,
            RfxStringData((void *)iccid, strlen(iccid)));
    responseToTelCore(unsol);
}

RIL_AppType RmmSimBaseHandler::getRilAppTypeFromMipc(uint8_t mipc_app_type) {
    RIL_AppType app_type = RIL_APPTYPE_UNKNOWN;

    switch (mipc_app_type) {
        case MIPC_SIM_APP_TYPE_MFSIM:
            app_type = RIL_APPTYPE_SIM;
            break;
        case MIPC_SIM_APP_TYPE_USIM:
            app_type = RIL_APPTYPE_USIM;
            break;
        case MIPC_SIM_APP_TYPE_CSIM:
            app_type = RIL_APPTYPE_CSIM;
            break;
        case MIPC_SIM_APP_TYPE_MFRUIM:
            app_type = RIL_APPTYPE_RUIM;
            break;
        case MIPC_SIM_APP_TYPE_ISIM:
            app_type = RIL_APPTYPE_ISIM;
            break;
        default:
            break;
    }

    logD(mTag, "getRilAppTypeFromMipc, app_type:%d, mipc_app_type:%d",
            app_type, mipc_app_type);

    return app_type;
}

uint8_t RmmSimBaseHandler::getMipcAppTypeFromRil(int appTypeId) {
    uint8_t mipc_sim_app_id;
    if (appTypeId == UICC_APP_SIM) {
        mipc_sim_app_id = MIPC_SIM_APP_TYPE_MFSIM;
    } else if (appTypeId == UICC_APP_USIM) {
        mipc_sim_app_id = MIPC_SIM_APP_TYPE_USIM;
    } else if (appTypeId == UICC_APP_CSIM) {
        mipc_sim_app_id = MIPC_SIM_APP_TYPE_CSIM;
    } else if (appTypeId == UICC_APP_RUIM) {
        mipc_sim_app_id = MIPC_SIM_APP_TYPE_MFRUIM;
    } else if (appTypeId == UICC_APP_ISIM) {
        mipc_sim_app_id = MIPC_SIM_APP_TYPE_ISIM;
    } else {
        mipc_sim_app_id = MIPC_SIM_APP_TYPE_UNKNOWN;
    }

    logD(mTag, "getMipcAppTypeFromRil, mipc_app_type:%d, rilappTypeId:%d",
            mipc_sim_app_id, appTypeId);

    return mipc_sim_app_id;
}

RIL_AppState RmmSimBaseHandler::getRilAppStateFromMipc(uint8_t mipc_app_state) {
    RIL_AppState ril_app_state = RIL_APPSTATE_UNKNOWN;

    switch (mipc_app_state) {
        case MIPC_SIM_APP_STATE_DETECTED:
            ril_app_state = RIL_APPSTATE_DETECTED;
            break;
        case MIPC_SIM_APP_STATE_PIN:
            ril_app_state = RIL_APPSTATE_PIN;
            break;
        case MIPC_SIM_APP_STATE_PUK:
            ril_app_state = RIL_APPSTATE_PUK;
            break;
        case MIPC_SIM_APP_STATE_SUBSTATE:
            ril_app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
            break;
        case MIPC_SIM_APP_STATE_READY:
            ril_app_state = RIL_APPSTATE_READY;
            break;
        default:
            break;
    }

    logD(mTag, "getRilAppStateFromMipc, ril_app_state:%d, mipc_app_state:%d",
            ril_app_state, mipc_app_state);

    return ril_app_state;
}

RIL_PersoSubstate RmmSimBaseHandler::getPersoSubstateFromMipc(uint8_t mipc_sub_state) {
    RIL_PersoSubstate perso_substate = RIL_PERSOSUBSTATE_UNKNOWN;

    switch (mipc_sub_state) {
        case MIPC_SIM_APP_SUB_STATUS_IN_PROGRESS:
            perso_substate = RIL_PERSOSUBSTATE_IN_PROGRESS;
            break;
        case MIPC_SIM_APP_SUB_STATUS_READY:
            perso_substate = RIL_PERSOSUBSTATE_READY;
            break;
        case MIPC_SIM_APP_SUB_STATUS_NW:
            perso_substate = RIL_PERSOSUBSTATE_SIM_NETWORK;
            break;
        case MIPC_SIM_APP_SUB_STATUS_NW_PUK:
            perso_substate = RIL_PERSOSUBSTATE_SIM_NETWORK_PUK;
            break;
        case MIPC_SIM_APP_SUB_STATUS_SUB_NW:
            perso_substate = RIL_PERSOSUBSTATE_SIM_NETWORK_SUBSET; //????TODO
            break;
        case MIPC_SIM_APP_SUB_STATUS_SUB_NW_PUK:
            perso_substate = RIL_PERSOSUBSTATE_SIM_NETWORK_SUBSET_PUK; //????TODO
            break;
        case MIPC_SIM_APP_SUB_STATUS_CORP:
            perso_substate = RIL_PERSOSUBSTATE_SIM_CORPORATE;
            break;
        case MIPC_SIM_APP_SUB_STATUS_CORP_PUK:
            perso_substate = RIL_PERSOSUBSTATE_SIM_CORPORATE_PUK;
            break;
        case MIPC_SIM_APP_SUB_STATUS_SP:
            perso_substate = RIL_PERSOSUBSTATE_SIM_SERVICE_PROVIDER;
            break;
        case MIPC_SIM_APP_SUB_STATUS_SP_PUK:
            perso_substate = RIL_PERSOSUBSTATE_SIM_SERVICE_PROVIDER_PUK;
            break;
        case MIPC_SIM_APP_SUB_STATUS_IMSI:
            perso_substate = RIL_PERSOSUBSTATE_SIM_IMPI;
            break;
        case MIPC_SIM_APP_SUB_STATUS_IMSI_PUK:
            perso_substate = RIL_PERSOSUBSTATE_SIM_IMPI_PUK;
            break;
        case MIPC_SIM_APP_SUB_STATUS_LINK_NS_SP:
            perso_substate = RIL_PERSOSUBSTATE_SIM_NS_SP;
            break;
        case MIPC_SIM_APP_SUB_STATUS_LINK_NS_SP_PUK:
            perso_substate = RIL_PERSOSUBSTATE_SIM_NS_SP_PUK;
            break;
        case MIPC_SIM_APP_SUB_STATUS_LINK_SIM_C:
            perso_substate = RIL_PERSOSUBSTATE_SIM_SIM_C;
            break;
        case MIPC_SIM_APP_SUB_STATUS_LINK_SIM_C_PUK:
            perso_substate = RIL_PERSOSUBSTATE_SIM_SIM_C_PUK;
            break;
        default:
            break;
    }

    logD(mTag, "getPersoSubstateFromMipc, perso_substate:%d, mipc_sub_state:%d",
            perso_substate, mipc_sub_state);

    return perso_substate;
}

UICC_Status RmmSimBaseHandler::getUiccStatusFromMipc(uint8_t mipc_status) {
    UICC_Status uicc_status = UICC_NOT_READY;
    int esim_cause = 0;
    switch (mipc_status) {
        case MIPC_SIM_STATUS_NOT_INSERT: // sim not inserted
            // MTK-START: AOSP SIM PLUG IN/OUT
            esim_cause = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ESIMS_CAUSE);
            if (ESIMS_CAUSE_SIM_NO_INIT == esim_cause) {
                uicc_status = UICC_NO_INIT;
            } else {
                uicc_status = UICC_ABSENT;
            }
            // MTK-END
            break;
        case MIPC_SIM_STATUS_BUSY:       // CPIN response BUSY
            uicc_status = UICC_BUSY;
            break;
        case MIPC_SIM_STATUS_READY:      // CPIN response READY
            uicc_status = UICC_READY;
            break;
        case MIPC_SIM_STATUS_SIM_PIN:    // MT is waiting SIM PIN to be given
            uicc_status = UICC_PIN;
            break;
        case MIPC_SIM_STATUS_SIM_PUK:    // MT is waiting SIM PUK to be given
            uicc_status = UICC_PUK;
            break;
        case MIPC_SIM_STATUS_PH_SIM_PIN: // MT is waiting phone to SIM card password to be givenn
            //uicc_status = ; ??
            break;
        case MIPC_SIM_STATUS_PH_FSIM_PIN: // MT is waiting phone-to-very first SIM card password to be given
            uicc_status = UICC_SIMP;
            break;
        case MIPC_SIM_STATUS_PH_FSIM_PUK: // MT is waiting phone-to-very first SIM card unblocking password to be given
            uicc_status = UICC_SIMP_PUK;
            break;
        case MIPC_SIM_STATUS_SIM_PIN2:  // MT is waiting SIM PIN2 to be given
            uicc_status = UICC_READY;  // TODO?
            break;
        case MIPC_SIM_STATUS_SIM_PUK2:  // MT is waiting SIM PUK2 to be given
            uicc_status = UICC_READY; // TODO?
            break;
        case MIPC_SIM_STATUS_PH_NET_PIN:  // MT is waiting network personalization password to be given
            uicc_status = UICC_NP;
            break;
        case MIPC_SIM_STATUS_PH_NET_PUK:  // MT is waiting network personalization unblocking password to be given
            uicc_status = UICC_NP_PUK;
            break;
        case MIPC_SIM_STATUS_PH_NETSUB_PIN: // MT is waiting network subset personalization password to be given
            uicc_status = UICC_NSP;
            break;
        case MIPC_SIM_STATUS_PH_NETSUB_PUK:  // MT is waiting network subset personalization unblocking password to be given
            uicc_status = UICC_NSP_PUK;
            break;
        case MIPC_SIM_STATUS_PH_SP_PIN:     // MT is waiting service provider personalization password to be given
            uicc_status = UICC_SP;
            break;
        case MIPC_SIM_STATUS_PH_SP_PUK:     // MT is waiting service provider personalization unblocking password to be given
            uicc_status = UICC_SP_PUK;
            break;
        case MIPC_SIM_STATUS_PH_CORP_PIN:   // MT is waiting corporate personalization password to be given
            uicc_status = UICC_CP;
            break;
        case MIPC_SIM_STATUS_PH_CORP_PUK:   // MT is waiting corporate personalization unblocking password to be given
            uicc_status = UICC_CP_PUK;
            break;
        case MIPC_SIM_STATUS_CARD_REBOOT:   // CARD Reboot
            uicc_status = UICC_CARD_REBOOT;
            break;
        case MIPC_SIM_STATUS_CARD_RESTRICTED: // CARD restricted
            if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE)
                   == RFX_CARD_TYPE_USIM) {
                uicc_status = UICC_USIM_RESTRICTED;
            } else {
                uicc_status = UICC_SIM_RESTRICTED;
            }
            break;
        //case :
        //    uicc_status = UICC_NS_SP;
        //    break;
        //case :
        //    uicc_status = UICC_NS_SP_PUK;
        //    break;
        //case :
        //    uicc_status = UICC_SIM_C;
        //    break;
        //case :
        //    uicc_status = UICC_SIM_C_PUK;
        //    break;
        case MIPC_SIM_STATUS_EMPT_EUICC:      // eUICC but no profile
            uicc_status = UICC_EMPTY_EUICC;
            break;
        case MIPC_SIM_STATUS_COMPLETE_READY:  // SIM initialization done after CPIN ready
            uicc_status = UICC_READY;
            break;
        default:
            if (RfxRilUtils::isVsimEnabledNotInserted(m_slot_id)) {
                uicc_status = UICC_ABSENT;
            }
            break;
        //UICC_NETWORK_PERSONALIZATION = 5, not in use
        //UICC_PERM_BLOCKED = 15, // PERM_DISABLED
        //UICC_NO_INIT = 16,
    }

    //if (isRjioRsuSupport() && cpinStr == String8::format("SIM PIN-DEFER")) { //move to L5
    //   /* Subsidy lock feature only */
    //   uicc_status = UICC_NOT_READY;
    //}

    logD(mTag, "getUiccStatusFromMipc, uicc_status:%d, mipc_status:%d",
            uicc_status, mipc_status);

return uicc_status;
}

int RmmSimBaseHandler::getSimStatebyUiccStatus(int uicc_status) {
    int sim_state = RFX_SIM_STATE_NOT_READY;

    switch (uicc_status) {
        case UICC_PIN:
            sim_state = RFX_SIM_STATE_PIN_REQUIRED;
            break;
        case UICC_PUK:
            sim_state = RFX_SIM_STATE_PUK_REQUIRED;
            break;
        case UICC_NP:
        case UICC_NP_PUK:
        case UICC_NSP:
        case UICC_NSP_PUK:
        case UICC_SP:
        case UICC_SP_PUK:
        case UICC_CP:
        case UICC_CP_PUK:
        case UICC_SIMP:
        case UICC_SIMP_PUK:
        case UICC_NS_SP:
        case UICC_NS_SP_PUK:
        case UICC_SIM_C:
        case UICC_SIM_C_PUK:
        case UICC_SIM_RESTRICTED:
        case UICC_USIM_RESTRICTED:
            sim_state = RFX_SIM_STATE_NETWORK_LOCKED;
            break;
        case UICC_NOT_READY:
        case UICC_EMPTY_EUICC:
        case UICC_CARD_REBOOT:
            sim_state = RFX_SIM_STATE_NOT_READY;
            break;
        case UICC_ABSENT: // sim not inserted
            sim_state = RFX_SIM_STATE_ABSENT;
            break;
        case UICC_READY:
            sim_state = RFX_SIM_STATE_READY;
            break;
         case UICC_BUSY:
            sim_state = RFX_SIM_STATE_UNKNOWN;
            break;
         case UICC_PERM_BLOCKED:
            sim_state = RFX_SIM_STATE_PERM_DISABLED;
            break;
        default:
            if (RfxRilUtils::isVsimEnabledNotInserted(m_slot_id)) {
                sim_state = RFX_SIM_STATE_ABSENT;
            }
            break;
    }

    logD(mTag, "getSimStatebyUiccStatus, sim_state:%d, uicc_status:%d",
            sim_state, uicc_status);

    return sim_state;
}


