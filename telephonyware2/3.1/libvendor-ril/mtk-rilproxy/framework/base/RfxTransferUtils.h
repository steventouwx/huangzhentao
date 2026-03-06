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
 /*
  * rfx_data_to_parcel_utils.h
  *
  *  Created on: 2015/09/02
  *  Author: MTK10602
  *
  */
#ifndef __RFX_TRANSFER_H__
#define __RFX_TRANSFER_H__

#include <binder/Parcel.h>
#include "../include/telephony/ril.h"
#include "RfxLog.h"
#include <cutils/jstring.h>

#ifdef HAVE_AEE_FEATURE
#include "aee.h"
#endif

using ::android::Parcel;

#define LOG_TAG "RfxTransferUtils"
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define NUM_ELEMS(x) (sizeof(x)/sizeof(x[0]))

#if defined(ANDROID_MULTI_SIM)
#define RESPONSE_TO_RILJ(id, data, datalen, rilSocketId) do { \
            if (data != NULL) { \
                RfxRilAdapter::responseToRilj(id, data, datalen, rilSocketId); \
            } else { \
                RfxRilAdapter::responseToRilj(id, NULL, 0, rilSocketId); \
            } \
        } while (0)
#else
#define RESPONSE_TO_RILJ(id, data, datalen, rilSocketId) \
           RfxRilAdapter::responseToRilj(id, data, datalen)
#endif

extern const char *getVersion();
typedef struct DataToParcelInfo {
    int request;
    Parcel* (*dataToParcelFunction) (int request, int token, void* data, int datalen, RIL_Token t);
    void (*parcleToDataFunction) (RIL_Token t, RIL_Errno e, int request, Parcel* p, int slotId);
#if defined(TELEPHONYWARE)
    int support;
#endif
} DataToParcelInfo;

typedef struct ParcelToDataInfo {
    int urc;
    void (*parcleToDataFunction) (RIL_Token t, RIL_Errno e, int urc, Parcel* p, int slotId);
#if defined(TELEPHONYWARE)
    int support;
#endif
} ParcelToDataInfo;

typedef struct {
    int requestNumber;
    void(*dispatchFunction) (Parcel &p, struct RequestInfo *pRI);
    int(*responseFunction) (Parcel &p, void *response, size_t responselen);
} CommandInfo;

typedef struct RequestInfo {
    int32_t token;      // this is not RIL_Token
    CommandInfo *pCI;
    struct RequestInfo *p_next;
    char cancelled;
    char local;         // responses to local commands do not go back to command process
    RIL_SOCKET_ID socket_id;
} RequestInfo;


static Parcel* callForwardToParcel(int request, int token, void* data, int datalen, RIL_Token t);
//static Parcel* requestKeepaliveToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* cdmaBrSmsCnfToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* cdmaSmsToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* cdmaSmsAckToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* cdmaSubscriptionSourceToParcel(int request, int token, void* data,
        int datalen, RIL_Token t);
static Parcel* dataCallToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* dataProfileToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* dialToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* fd_ModeToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* gsmBrSmsCnfToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* imsCdmaSmsToParcel(Parcel *parcel, RIL_CDMA_SMS_Message *rcsm);
static Parcel* imsGsmSmsToParcel(Parcel *parcel, void **gsmMessage, int datalen, RIL_Token t);
static Parcel* imsSmsToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* intsToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* nvReadItemToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* nvWriteItemToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* phbEntryToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* radioCapabilityToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* rawToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* rilCdmaSmsWriteArgsToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* setInitialAttachApnToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* simAuthToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* simAuthenticationToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* sim_APDUToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* sim_IOToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* sim_IO_EXToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* smsParamsToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* smsWriteToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* stringToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* stringsToParcel(int request, int token, void* data, int datalen, RIL_Token t);
//static Parcel* uiccAuthenticationToParcel(int request, int token, void* data, int datalen, RIL_Token t);
//static Parcel* uiccIoToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* uiccSubscriptonToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* voiceRadioTechToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* voidToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* vsimEventToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* vsimOperationEventToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* writePhbEntryExtToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* writePbToRuimToParcel(int request, int token, void* data, int datalen, RIL_Token t);
//static Parcel* mtkWritePbToRuimToParcel(int request, int token, void* data, int datalen, RIL_Token t);
//static Parcel* playDTMFToneToParcel(int request, int token, void* data, int datalen, RIL_Token t);
//static Parcel* playToneSeqToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* cdmaSmsToParcelInternal(Parcel *parcel, RIL_CDMA_SMS_Message *rcsm);

/* M: eMBMS feature */
static Parcel* embmsStartSessionReqToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* embmsStopSessionReqToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static void fillHeader(Parcel *parcel, int request, int token);

static void parcelToUssdStrings(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToVsimOperationEvent(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToInts(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToFailCause(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToStrings(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToString(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToVoid(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToUnused(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCallList(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToSMS(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToSIM_IO(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCallForwards(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToDataCallList(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToSetupDataCall(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToRaw(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToSsn(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCrssN(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToSimStatus(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToGsmBrSmsCnf(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCdmaBrSmsCnf(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCdmaSms(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCellList(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCdmaInformationRecords(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToRilSignalStrength(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCallRing(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCdmaSignalInfoRecord(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCdmaCallWaiting(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToSimRefresh(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCellInfoList(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToPhbEntries(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToGetSmsSimMemStatusCnf(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToGetPhbMemStorage(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToReadPhbEntryExt(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToSmsParams(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToCbConfigInfo(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToEtwsNotification(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToSetupDedicateDataCall(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
//static void parcelToEpcNetworkFeatureInfo(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToIratStateChange(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToHardwareConfig(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToDcRtInfo(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToRadioCapability(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToSSData(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToLceStatus(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToLceData(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToActivityData(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToGetNitzTime(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToMTKReadPbFromRuim(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToReadPbFromRuim(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parcelToGetSmsStatus(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static void parserSetupDedicateDataCall(RIL_Dedicate_Data_Call_Struct *data, Parcel *p);
//static int isImsSupport();
static int isReqFromMAL();

#ifdef HAVE_AEE_FEATURE
static void triggerNE(char *pErrMsg);
#endif
/* M: eMBMS feature */
static Parcel* embmsStartSessionReqToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* embmsStopSessionReqToParcel(int request, int token, void* data, int datalen, RIL_Token t);
/// [MT2635] add ecall related function
static Parcel* eCallToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static Parcel* eCallNumConfigToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static void parcelToEcallMsdStatus(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);
static Parcel* setMsdToParcel(int request, int token, void* data, int datalen, RIL_Token t);
static void parcelToEcallInd(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId);

class RfxTransferUtils {
private:
    RfxTransferUtils();

    virtual ~RfxTransferUtils();
    static DataToParcelInfo * getDataToParcelInfo(int request);
    static ParcelToDataInfo * getParcelToDataInfo(int urc);
public:
    static Parcel* dataToParcel(int request, int token, void* data, int datalen, RIL_Token t);
    static void parcelToDataResponse(RIL_Token t, RIL_Errno e, int request, Parcel * parcel, int slotId);
    static void parcelToDataUrc(int urc, Parcel *parcel, int slotId);
    static void checkTableIndex();
#if defined(TELEPHONYWARE)
    static int checkReqSupport(int request);
    static int checkUnsolSupport(int urc);
#endif
};
#endif
