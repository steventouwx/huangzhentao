/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2022. All rights reserved.
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


#ifndef SAP_SERVICE_H
#define SAP_SERVICE_H


#include <ril_internal.h>
#include <RilSapSocket.h>
#include "pb_decode.h"
#include "pb_encode.h"
#include <proto/sap-api.pb.h>

namespace sap {
    void registerService(const RIL_RadioFunctions *callbacks);
    void processResponse(MsgHeader *rsp, RilSapSocket *sapSocket);
    void processUnsolResponse(MsgHeader *rsp, RilSapSocket *sapSocket);
}

#include <aidl/android/hardware/radio/sap/BnSap.h>

namespace android::hardware::radio::sap {

class RadioSap : public aidl::android::hardware::radio::sap::BnSap {

    ::ndk::ScopedAStatus addPayloadAndDispatchRequest(MsgHeader *msg,
            uint16_t reqLen, uint8_t *reqPtr);
    void sendFailedResponse(MsgId msgId, int32_t token, int numPointers, ...);
    MsgHeader* createMsgHeader(MsgId msgId, int32_t serial);

    public:
    ::ndk::ScopedAStatus apduReq(int32_t serial,
            ::aidl::android::hardware::radio::sap::SapApduType type,
            const std::vector<uint8_t>& command) override;
    ::ndk::ScopedAStatus connectReq(int32_t serial, int32_t maxMsgSize) override;
    ::ndk::ScopedAStatus disconnectReq(int32_t serial) override;
    ::ndk::ScopedAStatus powerReq(int32_t serial, bool state) override;
    ::ndk::ScopedAStatus resetSimReq(int32_t serial) override;
    ::ndk::ScopedAStatus setCallback(
    const std::shared_ptr<::aidl::android::hardware::radio::sap::ISapCallback>& sapCallback) override;
    ::ndk::ScopedAStatus setTransferProtocolReq(
            int32_t serial,
            ::aidl::android::hardware::radio::sap::SapTransferProtocol transferProtocol) override;
    ::ndk::ScopedAStatus transferAtrReq(int32_t serial) override;
    ::ndk::ScopedAStatus transferCardReaderStatusReq(int32_t serial) override;

    public:
        RadioSap(int slotId, RIL_SOCKET_ID socketid);
        int mSlotId;
        RIL_SOCKET_ID mSocketId;
};
}  // namespace android::hardware::radio::sap
#endif
