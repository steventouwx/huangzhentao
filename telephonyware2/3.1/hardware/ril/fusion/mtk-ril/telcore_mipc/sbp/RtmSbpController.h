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

#ifndef __RTM_SBP_CONTROLLER_H__
#define __RTM_SBP_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"

#define SBP_FILE_PATH "/mnt/vendor/nvdata/APCFG/APRDCL/CXP_SBP"


typedef enum {
    RTM_SBP_AUTO_SWITCH = 1,
    RTM_SBP_ONLY_CHECK_FIRST_SIM = 2,
    RTM_SBP_MANUAL_SWITCH = 3,
    RTM_SBP_ALWAYS_ASK = 4,
} RTM_SBP_SWITCH_MODE;

#define RTM_SBP_CXP_SET_SBP  (0)
#define RTM_SBP_RILD_SET_SBP (1)
#define RTM_SBP_MODEM_PARTIALLY_SET_SBP (2)
#define RTM_SBP_MODEM_SET_SBP (3)

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtmSbpController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtmSbpController);

public:
    RtmSbpController();
    virtual ~RtmSbpController();
    bool responseToRilj(const sp<RfxMessage>& message);
    void onSbpIdChanged(int slot_id, RfxStatusKeyEnum key __unused,
            RfxVariant old_value __unused, RfxVariant value __unused);

// Override
protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);

private:
    void switchSbp(int latest_sbp, int latest_sub_id);
    int getSwitchMode();
    int getCurrentModemSbp();
    int getSbpFromFile();
    void saveSbpIdToFile(int latest_sbp);
    int getSbpFromProperty();
    int getCustomSbp();
    int getCurrentSubId();
    int getSetSbpPlace();
    void sendRequestResetRadio();
    void setFirstSimCheckedFlag();
    int getFirstSimCheckedFlag();
    int getRefinedSbpId(int sbp_id);
};

#endif /* __RTM_SBP_CONTROLLER_H__ */
