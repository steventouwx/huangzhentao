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

#ifndef __RMC_PHB_IN_MODEM_HANDLER_H__
#define __RMC_PHB_IN_MODEM_HANDLER_H__

#include "RfxBaseHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxPhbEntryExtData.h"
#include "PhonebookBitArray.h"
#include <regex>
#include <string>

#define RFX_LOG_TAG "RmcPhbInModem"

typedef enum {
   CPBW_ENCODE_IRA,
   CPBW_ENCODE_UCS2,
   CPBW_ENCODE_UCS2_81,
   CPBW_ENCODE_UCS2_82,
   CPBW_ENCODE_GSM7BIT,
   CPBW_ENCODE_MAX
}RilPhbCpbwEncode;

#define RIL_MAX_PHB_NAME_LEN (40)

class RmcPhbInModemHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcPhbInModemHandler);

    public:
        RmcPhbInModemHandler(int slot_id, int channel_id);
        virtual ~RmcPhbInModemHandler();

    protected:
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
        virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
        void requestReadSimPhonebookRecords(const sp<RfxMclMessage>& msg);
        void requestGetSimPhonebookCapacity(const sp<RfxMclMessage>& msg);
        void requestUpdateSimPhonebookRecords(const sp<RfxMclMessage>& msg);
        void requestClearPhbEntry(int index);
        void loadUPBCapability();

    private:
        int current_phb_storage = -1;
        // int ril_max_phb_name_len = 40;
        int maxGrpNum = -1;
        int maxAnrNum = -1;
        int maxEmailNum = -1;
        int maxAdnRecords = -1;
        int usedAdnRecords = -1;
        int readAdnRecordsNum = 0;
        int selectPhbStorage(int type);
        char* getPhbStorageString(int type);
        void clearPhbStorageInfo(void);
        PhonebookBitArray* mPhonebookBitArray;
};

#endif
