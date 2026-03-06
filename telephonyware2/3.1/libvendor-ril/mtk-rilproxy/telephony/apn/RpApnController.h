// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2015. All rights reserved.
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

#ifndef __RP_APN_CONTROLLER_H__
#define __RP_APN_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <utils/Vector.h>
#include "RfxController.h"
#include "RfxTimer.h"
#include <utils/Thread.h>
#include <binder/Parcel.h>

extern "C"
{
#include "apn_interface.h"
}

/*****************************************************************************
 * Class RpDataController
 *****************************************************************************/


typedef struct Apnsetting{
    std::string carrier;
    std::string apn;
    std::string proxy;
    std::string port;
    std::string mmsc;
    std::string mms_proxy;
    std::string mms_port;
    std::string user;
    std::string password;
    int auth_type;
    std::string types;
    int id;
    std::string numberic;
    std::string protocol;
    std::string roaming_protocol;
    int mtu;
    bool carrier_enabled;
    int bear;
    int bear_bitmask;
    int profile_id;
    bool modem_cognitive;
    int max_conns;
    int wait_time;
    int max_conns_time;
    std::string mvno_type;
    std::string mvno_match_data;
}ApnsettingT;


extern "C"
void apncallback(int ret, void* data);

using ::std::string;
using ::android::Thread;
using ::android::sp;
using ::android::Parcel;

class RpApnController : public RfxController {
    RFX_DECLARE_CLASS(RpApnController);  // Required: declare this class

public:
    RpApnController();
    virtual ~RpApnController();
    void SyncDefaultDataSimToModem();
    int setInitialAttachApn();
    int setDataProfilesAsNeeded();
    int SetDataAttached();
    Vector<ApnsettingT*>* getApnInformation(std::string apntype);
    void freeApnList(Vector<ApnsettingT*>* list);
    void generateApnDB();
    void calculate_md5_with_md5sum(const char *filename, char *md5_string);
    void GetApnDBBymccmnc();
    class ApnInitThread: public Thread {
    public:
        ApnInitThread(RpApnController* controller);
        virtual ~ApnInitThread();
    protected:
        RpApnController* apnController;
        virtual bool threadLoop();
    };

    sp<ApnInitThread> apnInitThread;
// Override
protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
#ifdef UPDATE_APN
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
#endif
private:
    void onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value);
    void onOperatorNumberChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value);
    void handleSetDataProfileResponse(const sp<RfxMessage>& response);
    void handleSetInitialAttachApnResponse(const sp<RfxMessage>& response);
    void handleSetInitialAttachApnRequest(const sp<RfxMessage>& request);
    void handleSyncDataSetToMdResponse(const sp<RfxMessage>& request);
    int getApnProp(apn_record_t* apn, int columnIdx, char ** value);
    int getApntypeBitmask(char *type);
    bool isApnContains(apn_record_t* apn, int columnIdx, const char *values);
    bool  apnCanHandleType(apn_record_t* apn , const char * type);
    int initDataProfile(apn_record_t* apn, RIL_DataProfileInfo *dp, int isRoaming);
    int parseApnRecord(ApnsettingT* apnInfo,apn_record_t* record);
#ifdef UPDATE_APN
    void handleUpdateApnRequest(const sp<RfxMessage>& message);
    void updateApn(const android::sp<RfxMessage> &message);
    void handleGetApn(const sp<RfxMessage> &message);
    void getApn(const android::sp<RfxMessage> &message);
#endif


public:
    RfxSignal0 apn_ready_singal;
    apn_list_t* g_apnlist;
    String8 mcc;
    String8 mnc;
private:
    apn_record_t *mPreferredApn;
    apn_record_t *mdefaultApnRecord;
    apn_record_t *mfirstApnRecord;
    bool mApnInit;
    String8 emptyStr;
    int mDataprofileid;
};

#endif /* __RP_APN_CONTROLLER_H__ */

