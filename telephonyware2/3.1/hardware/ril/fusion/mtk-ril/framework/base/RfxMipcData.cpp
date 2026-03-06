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
/*****************************************************************************
 * Include
 *****************************************************************************/

#include <mipc_msg.h>
#include "RfxMipcData.h"
#include "RfxMipcUtils.h"
/*****************************************************************************
 * Class RfxMipcData
 *****************************************************************************/
RfxMipcData::RfxMipcData() {
    data = NULL;
}

RfxMipcData::~RfxMipcData() {
    if (data != NULL) {
        mipc_msg_deinit(data);
        data = NULL;
    }
}

RfxMipcData::RfxMipcData(mipc_msg_t *mipcData) {
    data = mipcData;
}


RfxMipcData::RfxMipcData(mipc_msg_t *mipcData, bool copyData) {
    if (copyData == false) {
        data = mipcData;
    } else {
        data = mipc_msg_copy_hdr(mipcData);
        mipc_msg_copy_tlv(mipcData, data);
    }
}

RfxMipcData::RfxMipcData(uint16_t msgId, int slotId) {
    mipc_msg_sim_ps_id_enum mipcSimId = RfxMipcUtils::slotIdChangeToMipcSlotId(slotId);
    data = mipc_msg_init(msgId, mipcSimId);
}


RfxMipcData::RfxMipcData(uint16_t msgId, int slotId, uint16_t msgTxId) {
    mipc_msg_sim_ps_id_enum mipcSimId = RfxMipcUtils::slotIdChangeToMipcSlotId(slotId);
    data = mipc_msg_init_with_txid(msgId, mipcSimId, msgTxId);
}

sp<RfxMipcData> RfxMipcData::obtainMipcData(uint16_t msgId, int slotId) {
    sp<RfxMipcData> mipcData = new RfxMipcData(msgId, slotId);
    return mipcData;
}

sp<RfxMipcData> RfxMipcData::obtainMipcData(uint16_t msgId, int slotId, uint16_t msgTxId) {
    sp<RfxMipcData> mipcData = new RfxMipcData(msgId, slotId, msgTxId);
    return mipcData;
}

sp<RfxMipcData> RfxMipcData::obtainMipcData(uint16_t msgId, const RfxBaseData &baseData, int slotId) {
    sp<RfxMipcData> mipcData = new RfxMipcData(msgId, slotId);
    RfxMipcDataConverter::fromBaseData(&baseData, mipcData->getData());
    return mipcData;
}

sp<RfxMipcData> RfxMipcData::obtainMipcData(mipc_msg_t *mipcData, bool copyData) {
    sp<RfxMipcData> mipc = NULL;
    if (copyData == false) {
        mipc = new RfxMipcData(mipcData);
    } else {
        mipc_msg_t* datatr = mipc_msg_copy_hdr(mipcData);
        mipc_msg_copy_tlv(mipcData, datatr);
        mipc = new RfxMipcData(datatr);
    }
    return mipc;
}

mipc_msg_t* RfxMipcData::getData() {
    return data;
}

unsigned int RfxMipcData::getResult() {
    if (data != NULL) {
        return mipc_msg_get_val_uint32(data, MIPC_T_RESULT, MIPC_RESULT_FAILURE);
    }
    return MIPC_RESULT_FAILURE;
}

uint16_t RfxMipcData::getMipcMsgId() {
    if (data != NULL) {
        return data->hdr.msg_id;
    }
    return 0;
}

uint16_t RfxMipcData::getMipcMsgTxId() {
    if (data != NULL) {
        return data->hdr.msg_txid;
    }
    return 0;
}

void RfxMipcData::addMipcTlv(uint16_t type, uint16_t len, void* valPtr) {
    mipc_msg_add_tlv(data, type, len, valPtr);
}

unsigned int RfxMipcData::getMipcSlotId() {
    if (data != NULL) {
        return data->hdr.msg_sim_ps_id;
    }
    return 0;
}


void RfxMipcData::addMipcTlvStr(uint16_t type, const char *string){
    mipc_msg_add_tlv(data, type, strlen(string) + 1, string);
}

void RfxMipcData::addMipcTlvStr(uint16_t type, const String8 &string){
    mipc_msg_add_tlv(data, type, string.size() + 1, string.string());
}

void RfxMipcData::addMipcTlv8(uint16_t type, void* valPtr) {
    mipc_msg_add_tlv8(data, type, valPtr);
}

void RfxMipcData::addMipcTlv16(uint16_t type, void* valPtr) {
    mipc_msg_add_tlv16(data, type, valPtr);
}

void RfxMipcData::addMipcTlv32(uint16_t type, void* valPtr) {
    mipc_msg_add_tlv32(data, type, valPtr);
}

void RfxMipcData::addMipcTlvInt8(uint16_t type, int8_t val) {
    mipc_msg_add_tlv_int8(data, type, val);
}

void RfxMipcData::addMipcTlvInt16(uint16_t type, int16_t val) {
    mipc_msg_add_tlv_int16(data, type, val);
}

void RfxMipcData::addMipcTlvInt32(uint16_t type, int32_t val) {
    mipc_msg_add_tlv_int32(data, type, val);
}

void RfxMipcData::addMipcTlvUint8(uint16_t type, uint8_t val) {
    mipc_msg_add_tlv_uint8(data, type, val);
}

void RfxMipcData::addMipcTlvUint16(uint16_t type, uint16_t val) {
    mipc_msg_add_tlv_uint16(data, type, val);
}

void RfxMipcData::addMipcTlvUint32(uint16_t type, uint32_t val) {
    mipc_msg_add_tlv_uint32(data, type, val);
}

void RfxMipcData::addMipcTlvArrayStr(uint16_t type, uint16_t len, char** valPtr) {
    mipc_msg_tlv_array_t *a_ptr;
    a_ptr = mipc_msg_add_tlv_array(data, type, len);
    for (int i = 0; i < len ; i++) {
        mipc_msg_add_idx(data, a_ptr, i, strlen(valPtr[i]), valPtr[i]);
    }
}

void RfxMipcData::addMipcTlvArrayUint8(uint16_t type, uint16_t len, uint8_t* valPtr) {
    mipc_msg_tlv_array_t *a_ptr;
    a_ptr = mipc_msg_add_tlv_array(data, type, len);
    for(int i = 0; i < len ; i++) {
        mipc_msg_add_idx_uint8(data, a_ptr, i, valPtr[i]);
    }
}

void RfxMipcData::addMipcTlvArrayUint16(uint16_t type, uint16_t len, uint16_t* valPtr) {
    mipc_msg_tlv_array_t *a_ptr;
    a_ptr = mipc_msg_add_tlv_array(data, type, len);
    for(int i = 0; i < len ; i++) {
        mipc_msg_add_idx_uint16(data, a_ptr, i, valPtr[i]);
    }
}

void RfxMipcData::addMipcTlvArrayUint32(uint16_t type, uint16_t len, uint32_t* valPtr) {
    mipc_msg_tlv_array_t *a_ptr;
    a_ptr = mipc_msg_add_tlv_array(data, type, len);
    for(int i = 0; i < len ; i++) {
        mipc_msg_add_idx_uint32(data, a_ptr, i, valPtr[i]);
    }
}

void RfxMipcData::addMipcTlvArrayUint8Array(uint16_t type, uint16_t count, uint8_t** valPtr, uint16_t fixed_len) {
    mipc_msg_tlv_array_t *a_ptr;
    a_ptr = mipc_msg_add_tlv_array(data, type, count);
    for (int i = 0; i < count ; i++) {
        mipc_msg_add_idx(data, a_ptr, i, fixed_len, valPtr[i]);
    }
}

void* RfxMipcData::getMipcVal(uint16_t type, uint16_t* valLenPtr) {
    if (data == NULL) {
        return NULL;
    }
    return mipc_msg_get_val_ptr(data, type, valLenPtr);
}

char* RfxMipcData::getMipcStr(uint16_t type) {
    uint16_t  valLenPtr = 0;
    return mipc_msg_get_val_str(data, type, &valLenPtr);
}

int8_t RfxMipcData::getMipcInt8Val(uint16_t type, int8_t defVal) {
    return mipc_msg_get_val_int8(data, type, defVal);
}

int16_t RfxMipcData::getMipcInt16Val(uint16_t type, int16_t defVal) {
    return mipc_msg_get_val_int16(data, type, defVal);
}

int32_t RfxMipcData::getMipcInt32Val(uint16_t type, int32_t defVal) {
    return mipc_msg_get_val_int32(data, type, defVal);
}

uint8_t RfxMipcData::getMipcUint8Val(uint16_t type, uint8_t defVal) {
    return mipc_msg_get_val_uint8(data, type, defVal);
}

uint16_t RfxMipcData::getMipcUint16Val(uint16_t type, uint16_t defVal) {
    return mipc_msg_get_val_uint16(data, type, defVal);
}

uint32_t RfxMipcData::getMipcUint32Val(uint16_t type, uint32_t defVal) {
    return mipc_msg_get_val_uint32(data, type, defVal);
}

uint16_t RfxMipcData::getMipcArrayTlvNum(uint16_t type) {
    return mipc_msg_get_tlv_num(data, type);
}

void* RfxMipcData::getMipcIdxVal(uint16_t type, uint16_t idx, uint16_t* valLenPtr) {
    if (data == NULL) {
        return NULL;
    }
    return mipc_msg_get_idx_ptr(data, type, idx, valLenPtr);
}

uint16_t RfxMipcData::getMipcIdxType(uint16_t type, uint16_t idx) {
    if (data == NULL) {
        return 0;
    }
    return MIPC_MSG_GET_TLV_TYPE(mipc_msg_get_idx(data, type, idx));
}

int8_t RfxMipcData::getMipcInt8IdxVal(uint16_t type, uint16_t idx, int8_t defVal) {
    return mipc_msg_get_idx_int8(data, type, idx, defVal);
}

int16_t RfxMipcData::getMipcInt16IdxVal(uint16_t type, uint16_t idx, int16_t defVal) {
    return mipc_msg_get_idx_int16(data, type, idx, defVal);
}

int32_t RfxMipcData::getMipcInt32IdxVal(uint16_t type, uint16_t idx, int32_t defVal) {
    return mipc_msg_get_idx_int32(data, type, idx, defVal);
}

uint8_t RfxMipcData::getMipcUint8IdxVal(uint16_t type, uint16_t idx, uint8_t defVal) {
    return mipc_msg_get_idx_uint8(data, type, idx, defVal);
}

uint16_t RfxMipcData::getMipcUint16IdxVal(uint16_t type, uint16_t idx, uint16_t defVal) {
    return mipc_msg_get_idx_uint16(data, type, idx, defVal);
}

uint32_t RfxMipcData::getMipcUint32IdxVal(uint16_t type, uint16_t idx, uint32_t defVal) {
    return mipc_msg_get_idx_uint32(data, type, idx, defVal);
}
