/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2017. All rights reserved.
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

#ifndef __RFX_MIPC_DATA__
#define __RFX_MIPC_DATA__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "mipc_msg.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include "utils/RefBase.h"
#include "RfxBaseData.h"

using ::android::RefBase;
using ::android::sp;
/*****************************************************************************
 * Class RfxMipcData
 *****************************************************************************/

class RfxMipcData : public virtual RefBase {

private:
    RfxMipcData();
    explicit RfxMipcData(mipc_msg_t *mipcData);
    RfxMipcData(uint16_t msgId, int slotId);
    RfxMipcData(uint16_t msgId, int slotId, uint16_t msgTxId);

private:
    mipc_msg_t *data;

public:
    virtual ~RfxMipcData();
    RfxMipcData(mipc_msg_t *mipcData, bool copyData);

public:
    static sp<RfxMipcData> obtainMipcData(uint16_t msgId, int slotId);
    static sp<RfxMipcData> obtainMipcData(uint16_t msgId, int slotId, uint16_t msgTxId);
    static sp<RfxMipcData> obtainMipcData(uint16_t msgId, const RfxBaseData &data, int slotId);
    static sp<RfxMipcData> obtainMipcData(mipc_msg_t *mipcData, bool copyData = false);
    /* return data for module using the generation api */
    mipc_msg_t* getData();
    /* get result */
    unsigned int getResult();
    /* get mipc data id */
    uint16_t getMipcMsgId();
    /* get mipc data tx id */
    uint16_t getMipcMsgTxId();
    /* get mipc data slot id */
    unsigned int getMipcSlotId();
    /* add mipc data tlv */
    void addMipcTlv(uint16_t type, uint16_t len, void* valPtr);
    /* add string */
    void addMipcTlvStr(uint16_t type, const char *string);
    void addMipcTlvStr(uint16_t type, const String8 &string);
    /* add tlv8 */
    void addMipcTlv8(uint16_t type, void* valPtr);
    /* add tlv16 */
    void addMipcTlv16(uint16_t type, void* valPtr);
    /* add tlv32 */
    void addMipcTlv32(uint16_t type, void* valPtr);
    /* add tlv int8 */
    void addMipcTlvInt8(uint16_t type, int8_t val);
    /* add tlv int16 */
    void addMipcTlvInt16(uint16_t type, int16_t val);
    /* add tlv int32 */
    void addMipcTlvInt32(uint16_t type, int32_t val);
    /* add tlv uint8 */
    void addMipcTlvUint8(uint16_t type, uint8_t val);
    /* add tlv uint16 */
    void addMipcTlvUint16(uint16_t type, uint16_t val);
    /* add tlv uint32 */
    void addMipcTlvUint32(uint16_t type, uint32_t val);
    /* add tlv array string */
    void addMipcTlvArrayStr(uint16_t type, uint16_t len, char** valPtr);
    /* add tlv array uint8 */
    void addMipcTlvArrayUint8(uint16_t type, uint16_t len, uint8_t* valPtr);
    /* add tlv array uint16 */
    void addMipcTlvArrayUint16(uint16_t type, uint16_t len, uint16_t* valPtr);
    /* add tlv array uint32 */
    void addMipcTlvArrayUint32(uint16_t type, uint16_t len, uint32_t* valPtr);
    /* add tlv array uint8[] */
    void addMipcTlvArrayUint8Array(uint16_t type, uint16_t count, uint8_t** valPtr, uint16_t fixed_len);
    /* get value pointer for tag */
    void* getMipcVal(uint16_t type, uint16_t* valLenPtr);
    /* get string*/
    char* getMipcStr(uint16_t type);
    /* get int8 value */
    int8_t getMipcInt8Val(uint16_t type, int8_t defVal);
    /* get int16 value */
    int16_t getMipcInt16Val(uint16_t type, int16_t defVal);
    /* get int32 value */
    int32_t getMipcInt32Val(uint16_t type, int32_t defVal);
    /* get uint8 value */
    uint8_t getMipcUint8Val(uint16_t type, uint8_t defVal);
    /* get uint16 value */
    uint16_t getMipcUint16Val(uint16_t type, uint16_t defVal);
    /* get uint32 value */
    uint32_t getMipcUint32Val(uint16_t type, uint32_t defVal);

    /* get tlv number in the Array */
    uint16_t getMipcArrayTlvNum(uint16_t type);
    /* get value pointer for tag */
    void* getMipcIdxVal(uint16_t type, uint16_t idx, uint16_t* valLenPtr);
    /* get value type for tag */
    uint16_t getMipcIdxType(uint16_t type, uint16_t idx);
    /* get int8 value */
    int8_t getMipcInt8IdxVal(uint16_t type, uint16_t idx, int8_t defVal);
    /* get int16 value */
    int16_t getMipcInt16IdxVal(uint16_t type, uint16_t idx, int16_t defVal);
    /* get int32 value */
    int32_t getMipcInt32IdxVal(uint16_t type, uint16_t idx, int32_t defVal);
    /* get uint8 value */
    uint8_t getMipcUint8IdxVal(uint16_t type, uint16_t idx, uint8_t defVal);
    /* get uint16 value */
    uint16_t getMipcUint16IdxVal(uint16_t type, uint16_t idx, uint16_t defVal);
    /* get uint32 value */
    uint32_t getMipcUint32IdxVal(uint16_t type, uint16_t idx, uint32_t defVal);
};


class RfxMipcDataConverter {

    public:

        static void fromBaseData(const RfxBaseData *baseData, mipc_msg_t *mipcData) {
            baseData->toMipc(mipcData);
        }

};

#endif /* __RFX_MIPC_DATA__ */
