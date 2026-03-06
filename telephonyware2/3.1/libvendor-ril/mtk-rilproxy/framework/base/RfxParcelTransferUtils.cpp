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
  * RfxParcelTranferUtil.cpp
  *
  *  Created on: 2018/08/21
  *  Author: MTK06564
  *
  */
#include "RfxParcelTransferUtils.h"
#include "../include/telephony/ril.h"
#include "../../radio-hal-adapter/radio_service_proxy.h"
#include "../../radio-hal-adapter/modem/radio_modem.h"
#include "../../radio-hal-adapter/modem/mtkradioex_modem.h"
#include "../../radio-hal-adapter/ims/mtkradioex_ims.h"
#include "../../radio-hal-adapter/sim/radio_sim.h"
#include "../../radio-hal-adapter/data/radio_data.h"
#include "../../radio-hal-adapter/data/mtkradioex_data.h"
#include "../../radio-hal-adapter/sim/mtkradioex_sim.h"
#include "../../radio-hal-adapter/network/radio_network.h"
#include "../../radio-hal-adapter/sms/radio_messaging.h"
#include "../../radio-hal-adapter/voice/radio_voice.h"
#include "../../radio-hal-adapter/voice/mtkradioex_voice.h"
#include "../../radio-hal-adapter/ecall/mtkradioex_ecall.h"
#include "../../radio-hal-adapter/config/radio_config.h"
#include "../../radio-hal-adapter/network/mtkradioex_network.h"

#include "RpDataUtils.h"
#include "telephony/ril_cdma_sms.h"
#include <aidl/android/hardware/radio/AccessNetwork.h>
#include <cstdint>
#include <optional>
#include <stdint.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <arpa/inet.h>

#define LOG_TAG "RfxParcelTransferUtils"

#define fillReqID(parcel, ID) do {\
      (parcel)->setDataPosition(0);\
      (parcel)->writeInt32((ID) & 0xFFFFFFFF);\
    }while(0)

#define copyReqHeader(new_p, old_p) do {\
        (old_p)->setDataPosition(0);\
        (new_p)->appendFrom(old_p, 0, sizeof(int32_t)*2);\
        (old_p)->setDataPosition(sizeof(int32_t)*2);\
        (new_p)->setDataPosition(sizeof(int32_t)*2);\
    }while(0)

#define copyRespHeader(new_p, old_p) do {\
         (old_p)->setDataPosition(0);\
         (new_p)->appendFrom(old_p, 0, sizeof(int32_t)*3);\
         (old_p)->setDataPosition(sizeof(int32_t)*3);\
         (new_p)->setDataPosition(sizeof(int32_t)*3);\
    }while(0)

#define copyUrcHeader(new_p, old_p) do {\
        (old_p)->setDataPosition(0);\
        (new_p)->appendFrom(old_p, 0, sizeof(int32_t)*2);\
        (old_p)->setDataPosition(sizeof(int32_t)*2);\
        (new_p)->setDataPosition(sizeof(int32_t)*2);\
    }while(0)

#if 0
#define fillUrcID(parcel, ID) do {\
         (parcel)->setDataPosition(sizeof(int32_t));\
         (parcel)->writeInt32((ID) & 0xFFFFFFFF);\
         (parcel)->setDataPosition(sizeof(int32_t)*2);\
    }while(0)
#endif

static char *
strdupReadString(Parcel *p) {
    size_t stringlen;
    const char16_t *s16;

    s16 = p->readString16Inplace(&stringlen);

    return strndup16to8(s16, stringlen);
}

static void writeStringToParcel(Parcel *p, const char *s) {
    char16_t *s16;
    size_t s16_len = 0;
    s16 = strdup8to16(s, &s16_len);
    p->writeString16(s16, s16_len);
    free(s16);
}



void TransferUtilsPreferredNetworkType( Parcel *old_parcel,  Parcel *new_parcel)
{
     int value = 0, new_value;
    /*profile num*/

    old_parcel->readInt32(&value);
    RFX_LOG_D(LOG_TAG, "TransferUtilsPreferredNetworkType = %d", value);
    new_parcel->writeInt32(value);
    old_parcel->readInt32(&value);
    RFX_LOG_D(LOG_TAG, "TransferUtilsPreferredNetworkType 1= %d", value);
    switch(value)
    {
        case PREF_NET_TYPE_GSM_WCDMA:
        case PREF_NET_TYPE_GSM_ONLY:
        case PREF_NET_TYPE_WCDMA:
        case PREF_NET_TYPE_GSM_WCDMA_AUTO:
        case PREF_NET_TYPE_CDMA_EVDO_AUTO:
        case PREF_NET_TYPE_CDMA_ONLY:
        case PREF_NET_TYPE_EVDO_ONLY:
        case PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO:
        case PREF_NET_TYPE_LTE_CDMA_EVDO:
        case PREF_NET_TYPE_LTE_GSM_WCDMA:
        case PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA:
        case PREF_NET_TYPE_LTE_ONLY:
        case PREF_NET_TYPE_LTE_WCDMA:
        case PREF_NET_TYPE_NR_ONLY:
        case PREF_NET_TYPE_NR_LTE:
        case PREF_NET_TYPE_NR_LTE_CDMA_EVDO:
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_CDMA_EVDO_GSM_WCDMA:
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_GSM_WCDMA:
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_WCDMA:
            new_value = value;
            break;
        default:
            new_value = PREF_NET_TYPE_LTE_GSM_WCDMA;  //Todo. needs modify
            break;
    }
    RFX_LOG_D(LOG_TAG, "TransferUtilsPreferredNetworkType 2= %d", new_value);

    new_parcel->writeInt32(value);

}
void TransferUtilsRadioTechnology( Parcel *old_parcel,  Parcel *new_parcel)
{
    int value, new_value;
    android::status_t status;
    /*profile num*/
    old_parcel->readInt32(&value);
    status = new_parcel->writeInt32(value);
    if(status != android::OK) {
        RFX_LOG_E("TransferUtilsRadioTechnology", "writeInt32 failed!");
    }
    old_parcel->readInt32(&value);
    switch(value)
    {
        case RADIO_TECH_UNKNOWN :
        case RADIO_TECH_GPRS :
        case RADIO_TECH_EDGE :
        case RADIO_TECH_UMTS :
        case RADIO_TECH_IS95A :
        case RADIO_TECH_IS95B :
        case RADIO_TECH_1xRTT :
        case RADIO_TECH_EVDO_0 :
        case RADIO_TECH_EVDO_A :
        case RADIO_TECH_HSDPA :
        case RADIO_TECH_HSUPA :
        case RADIO_TECH_HSPA :
        case RADIO_TECH_EVDO_B :
        case RADIO_TECH_EHRPD :
        case RADIO_TECH_LTE :
        case RADIO_TECH_HSPAP :
        case RADIO_TECH_GSM :
        case RADIO_TECH_TD_SCDMA :
        case RADIO_TECH_IWLAN :
           new_value = value;
           break;
       default:
           new_value = RADIO_TECH_UNKNOWN;  //Todo. needs modify
           break;
    }
    status = new_parcel->writeInt32(new_value);
    if(status != android::OK) {
        RFX_LOG_E("TransferUtilsRadioTechnology", "writeInt32 failed!");
    }
}
// Must be the same as CellInfo.TYPE_XXX

#if 0
typedef struct {
    char* long_name;
    char* short_name;
} RIL_OperName;
typedef struct {
    int mcc;    /* 3-digit Mobile Country Code, 0..999, INT_MAX if unknown */
    int mnc;    /* 2 or 3-digit Mobile Network Code, 0..999, INT_MAX if unknown */
    int mnc_len;
    int lac;    /* 16-bit Location Area Code, 0..65535, INT_MAX if unknown  */
    int cid;    /* 16-bit GSM Cell Identity described in TS 27.007, 0..65535, INT_MAX if unknown  */
    int arfcn;  /* 16-bit GSM Absolute RF channel number, INT_MAX if unknown */
    uint8_t bsic;/* 6-bit Base Station Identity Code, 0xFF if unknown */
    RIL_OperName operName;
} RIL_CellIdentityGsm_v12;


typedef struct {
    int signalStrength;  /* Valid values are (0-31, 99) as defined in TS 27.007 8.5 */
    int bitErrorRate;    /* bit error rate (0-7, 99) as defined in TS 27.007 8.5 */
    int timingAdvance;   /* Timing Advance in bit periods. 1 bit period = 48/13 us.
              * INT_MAX denotes invalid value */
} RIL_GSM_SignalStrength_v12;

typedef struct {
    RIL_CellIdentityGsm_v12   cellIdentityGsm;
    RIL_GSM_SignalStrength_v12 signalStrengthGsm;
} RIL_CellInfoGsm_v12;
typedef struct {
    int networkId;      /* Network Id 0..65535, INT_MAX if unknown */
    int systemId;       /* CDMA System Id 0..32767, INT_MAX if unknown  */
    int basestationId;  /* Base Station Id 0..65535, INT_MAX if unknown  */
    int longitude;      /* Longitude is a decimal number as specified in 3GPP2 C.S0005-A v6.0.
                         * It is represented in units of 0.25 seconds and ranges from -2592000
                         * to 2592000, both values inclusive (corresponding to a range of -180
                         * to +180 degrees). INT_MAX if unknown */

    int latitude;       /* Latitude is a decimal number as specified in 3GPP2 C.S0005-A v6.0.
                         * It is represented in units of 0.25 seconds and ranges from -1296000
                         * to 1296000, both values inclusive (corresponding to a range of -90
                         * to +90 degrees). INT_MAX if unknown */
    RIL_OperName operName;
} RIL_CellIdentityCdma_v12;

typedef struct {
  RIL_CellIdentityCdma_v12      cellIdentityCdma;
  RIL_CDMA_SignalStrength   signalStrengthCdma;
  RIL_EVDO_SignalStrength   signalStrengthEvdo;
} RIL_CellInfoCdma_v12;
typedef struct {
    int mcc;    /* 3-digit Mobile Country Code, 0..999, INT_MAX if unknown  */
    int mnc;    /* 2 or 3-digit Mobile Network Code, 0..999, INT_MAX if unknown  */
    int mnc_len;
    int ci;     /* 28-bit Cell Identity described in TS ???, INT_MAX if unknown */
    int pci;    /* physical cell id 0..503; this value must be reported */
    int tac;    /* 16-bit tracking area code, INT_MAX if unknown  */
    int earfcn; /* 18-bit LTE Absolute RF Channel Number; this value must be reported */
    RIL_OperName operName;
    int bandwidth;  /* Bandwidht in KHz */
} RIL_CellIdentityLte_v12;

typedef struct {
    RIL_CellIdentityLte_v12    cellIdentityLte;
    RIL_LTE_SignalStrength_v8  signalStrengthLte;  //the same with V11
} RIL_CellInfoLte_v12;

typedef struct {
    int mcc;    /* 3-digit Mobile Country Code, 0..999, INT_MAX if unknown  */
    int mnc;    /* 2 or 3-digit Mobile Network Code, 0..999, INT_MAX if unknown  */
    int mnc_len;
    int lac;    /* 16-bit Location Area Code, 0..65535, INT_MAX if unknown  */
    int cid;    /* 28-bit UMTS Cell Identity described in TS 25.331, 0..268435455, INT_MAX if unknown  */
    int psc;    /* 9-bit UMTS Primary Scrambling Code described in TS 25.331, 0..511, INT_MAX if unknown */
    int uarfcn; /* 16-bit UMTS Absolute RF Channel Number, INT_MAX if unknown */
    RIL_OperName operName;
} RIL_CellIdentityWcdma_v12;


typedef struct {
    RIL_CellIdentityWcdma_v12 cellIdentityWcdma;
    RIL_SignalStrengthWcdma signalStrengthWcdma;
} RIL_CellInfoWcdma_v12;

typedef struct {
   int mcc;    /* 3-digit Mobile Country Code, 0..999, INT_MAX if unknown  */
   int mnc;    /* 2 or 3-digit Mobile Network Code, 0..999, INT_MAX if unknown  */
   int mnc_len;
   int lac;    /* 16-bit Location Area Code, 0..65535, INT_MAX if unknown  */
   int cid;    /* 28-bit UMTS Cell Identity described in TS 25.331, 0..268435455, INT_MAX if unknown  */
   int cpid;    /* 8-bit Cell Parameters ID described in TS 25.331, 0..127, INT_MAX if unknown */
   RIL_OperName operName;
} RIL_CellIdentityTdscdma_v12;

typedef struct {
       /**
           * UTRA carrier RSSI as defined in TS 25.225 5.1.4
           * Valid values are (0-31, 99) as defined in TS 27.007 8.5
           */
       int signalStrength;
       /**
           * Transport Channel BER as defined in TS 25.225 5.2.5
           * Valid values are (0-7, 99) as defined in TS 27.007 8.5
           */
       int bitErrorRate;
       /**
           * P-CCPCH RSCP as defined in TS 25.225 5.1.1
           * Valid values are (0-96, 255) as defined in TS 27.007 8.69
           */
       int rscp;
} RIL_TD_SCDMA_SignalStrength_v12;

typedef struct {
    RIL_CellIdentityTdscdma_v12 cellIdentityTdscdma;
    RIL_TD_SCDMA_SignalStrength_v12 signalStrengthTdscdma;
} RIL_CellInfoTdscdma_v12;

typedef struct {
  RIL_CellInfoType  cellInfoType;   /* cell type for selecting from union CellInfo */
  union {
    RIL_CellIdentityGsm_v12 cellIdentityGsm;
    RIL_CellIdentityWcdma_v12 cellIdentityWcdma;
    RIL_CellIdentityLte_v12 cellIdentityLte;
    RIL_CellIdentityTdscdma_v12 cellIdentityTdscdma;
    RIL_CellIdentityCdma_v12 cellIdentityCdma;
  };
}RIL_CellIdentity_v16;


typedef enum {
    /**
        * Cell is not a serving cell.
        */
    NONE_SERVING = 0,
    /**
        * UE has connection to cell for signalling and possibly data (3GPP 36.331, 25.331).
        */
    PRIMARY_SERVING,
    /**
        * UE has connection to cell for data (3GPP 36.331, 25.331).
        */
    SECONDARY_SERVING
} RIL_CellConnectionStatus;


typedef struct {
    RIL_CellInfoType  cellInfoType;   /* cell type for selecting from union CellInfo */
    int               registered;     /* !0 if this cell is registered 0 if not registered */
    RIL_TimeStampType timeStampType;  /* type of time stamp represented by timeStamp */
    uint64_t          timeStamp;      /* Time in nanos as returned by ril_nano_time */
        union {
        RIL_CellInfoGsm_v12     gsm;
        RIL_CellInfoCdma_v12        cdma;
        RIL_CellInfoLte_v12     lte;
        RIL_CellInfoWcdma_v12   wcdma;
        RIL_CellInfoTdscdma_v12     tdscdma;
        } CellInfo;
    RIL_CellConnectionStatus connectionStatus;
} RIL_CellInfo_v12;

typedef enum {
    RIL_NOT_REG_AND_NOT_SEARCHING = 0,           // Not registered, MT is not currently searching
                                                 // a new operator to register
    RIL_REG_HOME = 1,                            // Registered, home network
    RIL_NOT_REG_AND_SEARCHING = 2,               // Not registered, but MT is currently searching
                                                 // a new operator to register
    RIL_REG_DENIED = 3,                          // Registration denied
    RIL_UNKNOWN = 4,                             // Unknown
    RIL_REG_ROAMING = 5,                         // Registered, roaming
    RIL_NOT_REG_AND_EMERGENCY_AVAILABLE_AND_NOT_SEARCHING = 10,   // Same as
                                                 // RIL_NOT_REG_AND_NOT_SEARCHING but indicates that
                                                 // emergency calls are enabled.
    RIL_NOT_REG_AND_EMERGENCY_AVAILABLE_AND_SEARCHING = 12,  // Same as RIL_NOT_REG_AND_SEARCHING
                                                 // but indicates that
                                                 // emergency calls are enabled.
    RIL_REG_DENIED_AND_EMERGENCY_AVAILABLE = 13, // Same as REG_DENIED but indicates that
                                                 // emergency calls are enabled.
    RIL_UNKNOWN_AND_EMERGENCY_AVAILABLE = 14,    // Same as UNKNOWN but indicates that
                                                 // emergency calls are enabled.
} RIL_RegState;


typedef struct {
    RIL_RegState regState;                // Valid reg states are RIL_NOT_REG_AND_NOT_SEARCHING,
                                          // REG_HOME, RIL_NOT_REG_AND_SEARCHING, REG_DENIED,
                                          // UNKNOWN, REG_ROAMING defined in RegState
    RIL_RadioTechnology rat;              // indicates the available voice radio technology,
                                          // valid values as defined by RadioTechnology.
    int32_t cssSupported;                 // concurrent services support indicator. if
                                          // registered on a CDMA system.
                                          // 0 - Concurrent services not supported,
                                          // 1 - Concurrent services supported
    int32_t roamingIndicator;             // TSB-58 Roaming Indicator if registered
                                          // on a CDMA or EVDO system or -1 if not.
                                          // Valid values are 0-255.
    int32_t systemIsInPrl;                // indicates whether the current system is in the
                                          // PRL if registered on a CDMA or EVDO system or -1 if
                                          // not. 0=not in the PRL, 1=in the PRL
    int32_t defaultRoamingIndicator;      // default Roaming Indicator from the PRL,
                                          // if registered on a CDMA or EVDO system or -1 if not.
                                          // Valid values are 0-255.
    int32_t reasonForDenial;              // reasonForDenial if registration state is 3
                                          // (Registration denied) this is an enumerated reason why
                                          // registration was denied. See 3GPP TS 24.008,
                                          // 10.5.3.6 and Annex G.
                                          // 0 - General
                                          // 1 - Authentication Failure
                                          // 2 - IMSI unknown in HLR
                                          // 3 - Illegal MS
                                          // 4 - Illegal ME
                                          // 5 - PLMN not allowed
                                          // 6 - Location area not allowed
                                          // 7 - Roaming not allowed
                                          // 8 - No Suitable Cells in this Location Area
                                          // 9 - Network failure
                                          // 10 - Persistent location update reject
                                          // 11 - PLMN not allowed
                                          // 12 - Location area not allowed
                                          // 13 - Roaming not allowed in this Location Area
                                          // 15 - No Suitable Cells in this Location Area
                                          // 17 - Network Failure
                                          // 20 - MAC Failure
                                          // 21 - Sync Failure
                                          // 22 - Congestion
                                          // 23 - GSM Authentication unacceptable
                                          // 25 - Not Authorized for this CSG
                                          // 32 - Service option not supported
                                          // 33 - Requested service option not subscribed
                                          // 34 - Service option temporarily out of order
                                          // 38 - Call cannot be identified
                                          // 48-63 - Retry upon entry into a new cell
                                          // 95 - Semantically incorrect message
                                          // 96 - Invalid mandatory information
                                          // 97 - Message type non-existent or not implemented
                                          // 98 - Message type not compatible with protocol state
                                          // 99 - Information element non-existent or
                                          //      not implemented
                                          // 100 - Conditional IE error
                                          // 101 - Message not compatible with protocol state;
    RIL_CellIdentity_v16 cellIdentity;    // current cell information
}RIL_VoiceRegistrationStateResponse;


typedef struct {
    RIL_RegState regState;                // Valid reg states are RIL_NOT_REG_AND_NOT_SEARCHING,
                                          // REG_HOME, RIL_NOT_REG_AND_SEARCHING, REG_DENIED,
                                          // UNKNOWN, REG_ROAMING defined in RegState
    RIL_RadioTechnology rat;              // indicates the available data radio technology,
                                          // valid values as defined by RadioTechnology.
    int32_t reasonDataDenied;             // if registration state is 3 (Registration
                                          // denied) this is an enumerated reason why
                                          // registration was denied. See 3GPP TS 24.008,
                                          // Annex G.6 "Additional cause codes for GMM".
                                          // 7 == GPRS services not allowed
                                          // 8 == GPRS services and non-GPRS services not allowed
                                          // 9 == MS identity cannot be derived by the network
                                          // 10 == Implicitly detached
                                          // 14 == GPRS services not allowed in this PLMN
                                          // 16 == MSC temporarily not reachable
                                          // 40 == No PDP context activated
    int32_t maxDataCalls;                 // The maximum number of simultaneous Data Calls that
                                          // must be established using setupDataCall().
    RIL_CellIdentity_v16 cellIdentity;    // Current cell information
}RIL_DataRegistrationStateResponse;
#endif
static int getCellInfoTypeRadioTechnology(int rat) {
    switch(rat) {
        case 1://RADIO_TECH_GPRS
        case 2://RADIO_TECH_UMTS
        case 16://RADIO_TECH_GSM
        {
            return 1;
        }
        case 3://RADIO_TECH_UMTS
        case 9://RADIO_TECH_HSDPA
        case 10://RADIO_TECH_HSUPA
        case 11://RADIO_TECH_HSPA
        case 15://RADIO_TECH_HSPAP:
        {
            return 4; //RIL_CELL_INFO_TYPE_WCDMA;
        }
        case 4://RADIO_TECH_IS95A:
        case 5://RADIO_TECH_IS95B:
        case 6://RADIO_TECH_1xRTT:
        case 7://RADIO_TECH_EVDO_0:
        case 8://RADIO_TECH_EVDO_A:
        case 12://RADIO_TECH_EVDO_B:
        case 13://RADIO_TECH_EHRPD:
        {
            return 2; //RIL_CELL_INFO_TYPE_CDMA;
        }
        case 14://RADIO_TECH_LTE:
        case 19: //RADIO_TECH_LTE_CA
        {
            return 3; //RIL_CELL_INFO_TYPE_LTE;
        }
        case 17://RADIO_TECH_TD_SCDMA:
        {
            return 5; //RIL_CELL_INFO_TYPE_TD_SCDMA;
        }
        default: {
            break;
        }
    }
    return 0;
}
void TransferUtilsDataCallList( Parcel *oldp,  Parcel *newp)
{
    Parcel *new_parcel = newp;
    Parcel *p = oldp;
    int ver, tmp, num;
    size_t         stringlen = 0;
    const char16_t    *tmp_s16=NULL;

    if((new_parcel == NULL) || (p == NULL)) {
        RFX_LOG_E(LOG_TAG, "TransferUtilsDataCallList, invalid input paramter ");
        return;
    }

    //-->todo, tranfer parcel
    RFX_LOG_D(LOG_TAG, "TransferUtilsDataCallList, ");
    if (p->dataAvail() > 0) {
        p->readInt32(&ver);
        new_parcel->writeInt32(ver);

        p->readInt32(&num);
        new_parcel->writeInt32(num);
        RFX_LOG_D(LOG_TAG, "TransferUtilsDataCallList, num=%d",num);
        //appendPrintBuf("%d, ", (int)num);
        //list = (RIL_Data_Call_Response_v11 *) calloc(1, sizeof(RIL_Data_Call_Response_v11) * num);
        for (int i = 0; i < num; i++) {
            p->readInt32(&tmp); //status
            new_parcel->writeInt32(tmp);
            p->readInt32(&tmp); //suggestedRetryTime
            new_parcel->writeInt32(tmp);
            p->readInt32(&tmp); //cid
            new_parcel->writeInt32(tmp);
            p->readInt32(&tmp);//active
            new_parcel->writeInt32(tmp);
            tmp_s16 = p->readString16Inplace(&stringlen); //type
            newp->writeString16(tmp_s16, stringlen);
            tmp_s16 = p->readString16Inplace(&stringlen); //ifname
            newp->writeString16(tmp_s16, stringlen);
            tmp_s16 = p->readString16Inplace(&stringlen); //addresses
            newp->writeString16(tmp_s16, stringlen);
            tmp_s16 = p->readString16Inplace(&stringlen); //dnses
            newp->writeString16(tmp_s16, stringlen);
            tmp_s16 = p->readString16Inplace(&stringlen); //gateways
            newp->writeString16(tmp_s16, stringlen);
            tmp_s16 = p->readString16Inplace(&stringlen); //pcscf
            newp->writeString16(tmp_s16, stringlen);

            p->readInt32(&tmp);//mtu
            new_parcel->writeInt32(tmp);
            p->readInt32(&tmp);//mtuV6
            new_parcel->writeInt32(tmp);
            //p->readInt32(&tmp);//rat -v11 not support this
        }
    }

}

void TransferUtilsCellInfo( Parcel *old_parcel,  Parcel *new_parcel)
{

    int  cellInfoType = 0, tmp_value = 0;
    int64_t timeStamp = 0;
    int num;
    uint64_t tmp_value_64 = 0;

    int  responselen = old_parcel->dataSize();

    old_parcel->readInt32(&num);
    new_parcel->writeInt32(num);
    RLOGD("responseCellInfoList: old_parcel->dataAvail()=%d, datasize()=%d, sizeof=%d, num=%d",old_parcel->dataAvail(), old_parcel->dataSize(), sizeof(RIL_CellInfo_v12), num);

    android::status_t status;
    for(int i=0; i<num; i++)
    {
        /*CellInfo.cellInfoType*/
        old_parcel->readInt32(&cellInfoType);
        status = new_parcel->writeInt32(cellInfoType);
        if (status != android::OK) {
            RLOGE(" TransferUtilsCellInfo writeInt32 fail");
            return;
        }
        /*CellInfo.registered*/
        old_parcel->readInt32(&tmp_value);
        status = new_parcel->writeInt32(tmp_value);
        if (status != android::OK) {
            RLOGE(" TransferUtilsCellInfo writeInt32 fail");
            return;
        }
        /*CellInfo.timeStampType*/
        old_parcel->readInt32(&tmp_value);
        status = new_parcel->writeInt32(tmp_value);
        if (status != android::OK) {
            RLOGE(" TransferUtilsCellInfo writeInt32 fail");
            return;
        }
        /*CellInfo.timeStamp*/
        status = old_parcel->readInt64(&timeStamp);
        if (status != android::OK) {
            RLOGE(" TransferUtilsCellInfo readInt64 fail");
            return;
        }
        status = new_parcel->writeInt64(timeStamp);
        if (status != android::OK) {
            RLOGE(" TransferUtilsCellInfo writeInt64 fail");
            return;
        }
        switch(cellInfoType) {
            case RIL_CELL_INFO_TYPE_GSM: {
                /*CellInfo..gsm.cellIdentityGsm.mcc*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo..gsm.cellIdentityGsm.mnc*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo..gsm.cellIdentityGsm.mnc_len  -diff*/

                /*CellInfo..gsm.cellIdentityGsm.lac*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.gsm.cellIdentityGsm.cid*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.gsm.cellIdentityGsm.arfcn*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.gsm.cellIdentityGsm.bsic*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.gsm.signalStrengthGsm.operName.long_name    -diff*/
                /*CellInfo.gsm.signalStrengthGsm.operName.short_name   -diff*/

                /*CellInfo.gsm.signalStrengthGsm.signalStrength*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.gsm.signalStrengthGsm.bitErrorRate*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.gsm.signalStrengthGsm.timingAdvance   -diff*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                break;
            }
            case RIL_CELL_INFO_TYPE_WCDMA: {
                /*CellInfo.wcdma.cellIdentityWcdma.mcc*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.wcdma.cellIdentityWcdma.mnc*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.cdma.cellIdentityCdma.mnc_len  -diff*/

                /*CellInfo.wcdma.cellIdentityWcdma.lac*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.wcdma.cellIdentityWcdma.cid*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.wcdma.cellIdentityWcdma.psc*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.wcdma.cellIdentityWcdma.uarfcn*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.wcdma.cellIdentityWcdma.operName.long_name   -diff*/
                /*CellInfo.wcdma.cellIdentityWcdma.operName.short_name  -diff*/

                /*CellInfo.wcdma.signalStrengthWcdma.signalStrength*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.wcdma.signalStrengthWcdma.bitErrorRate*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.wcdma.signalStrengthWcdma.rscp*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.wcdma.signalStrengthWcdma.ecno*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                break;
            }
            case RIL_CELL_INFO_TYPE_CDMA: {
                /*CellInfo.cdma.cellIdentityCdma.networkId*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.cdma.cellIdentityCdma.systemId*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.cdma.cellIdentityCdma.basestationId*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.cdma.cellIdentityCdma.longitude*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.cdma.cellIdentityCdma.latitude*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.wcdma.cellIdentityWcdma.operName.long_name   -diff*/
                /*CellInfo.wcdma.cellIdentityWcdma.operName.short_name  -diff*/

                /*CellInfo.cdma.signalStrengthCdma.dbm*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.cdma.signalStrengthCdma.ecio*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.cdma.signalStrengthEvdo.dbm*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.cdma.signalStrengthEvdo.ecio*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                break;
            }
            case RIL_CELL_INFO_TYPE_LTE: {

                /*CellInfo.lte.cellIdentityLte.mcc*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.cellIdentityLte.mnc*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.cellIdentityLte.mnc_len  -diff*/
                /*CellInfo.lte.cellIdentityLte.ci*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.cellIdentityLte.pci*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.cellIdentityLte.tac*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.cellIdentityLte.earfcn*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.lte.cellIdentityLte.operName.longname  -diff*/
                /*CellInfo.lte.cellIdentityLte.operName.shortname  -diff*/
                /*CellInfo.lte.cellIdentityLte.bandwidth  -diff*/

                /*CellInfo.lte.signalStrengthLte.signalStrength*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.signalStrengthLte.rsrp*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.signalStrengthLte.rsrq*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.signalStrengthLte.rssnr*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.signalStrengthLte.cqi*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.signalStrengthLte.timingAdvance*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                break;
            }
            case RIL_CELL_INFO_TYPE_TD_SCDMA: {
                /*CellInfo.lte.signalStrengthLte.timingAdvance*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.signalStrengthLte.timingAdvance*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.lte.signalStrengthLte.timingAdvance   -diff */
                /*CellInfo.tdscdma.cellIdentityTdscdma.lac*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.tdscdma.cellIdentityTdscdma.cid*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.tdscdma.cellIdentityTdscdma.cpid*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.tdscdma.cellIdentityTdscdma.operName.longname    -diff */
                /*CellInfo.tdscdma.cellIdentityTdscdma.operName.shortname   -diff */

                /*CellInfo.tdscdma.signalStrengthTdscdma.signalStrength   -diff */
                /*CellInfo.tdscdma.signalStrengthTdscdma.bitErrorRate   -diff */

                /*CellInfo.tdscdma.signalStrengthTdscdma.rscp*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                break;
            }
            case RIL_CELL_INFO_TYPE_NR: {

                /*CellInfo.nr.cellIdentityNr.mcc*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.nr.cellIdentityNr.mnc*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.nr.cellIdentityNr.nci*/
                old_parcel->readUint64(&tmp_value_64);
                new_parcel->writeUint64(tmp_value_64);
                /*CellInfo.nr.cellIdentityNr.pci*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.nr.cellIdentityNr.tac*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.nr.cellIdentityNr.nr_arfcn  */
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);

                /*CellInfo.nr.SignalStrengthNr.rsrp*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.nr.SignalStrengthNr.rsrq*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.nr.SignalStrengthNr.sinr*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.nr.SignalStrengthNr.csirsrp*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.nr.SignalStrengthNr.csirsrq*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                /*CellInfo.nr.SignalStrengthNr.csisinr*/
                old_parcel->readInt32(&tmp_value);
                new_parcel->writeInt32(tmp_value);
                break;
            }
            default :
                break;
        }

    }

}

void writeStringToParcel(Parcel &p, const char *s) {
    char16_t *s16;
    size_t s16_len = 0;
    s16 = strdup8to16(s, &s16_len);
    p.writeString16(s16, s16_len);
    free(s16);
}

static int transferErrNum(int request, int err)
{
    // int new_err = RIL_E_GENERIC_FAILURE;
    // switch(err){
    //     case RIL_E_SUCCESS:
    //     case RIL_E_RADIO_NOT_AVAILABLE:
    //     case RIL_E_GENERIC_FAILURE:
    //     case RIL_E_PASSWORD_INCORRECT:
    //     case RIL_E_SIM_PIN2:
    //     case RIL_E_SIM_PUK2:
    //     case RIL_E_REQUEST_NOT_SUPPORTED:
    //     case RIL_E_CANCELLED:
    //     case RIL_E_OP_NOT_ALLOWED_DURING_VOICE_CALL:
    //     case RIL_E_OP_NOT_ALLOWED_BEFORE_REG_TO_NW:
    //     case RIL_E_SMS_SEND_FAIL_RETRY:
    //     case RIL_E_SIM_ABSENT:
    //     case RIL_E_SUBSCRIPTION_NOT_AVAILABLE:
    //     case RIL_E_MODE_NOT_SUPPORTED:
    //     case RIL_E_FDN_CHECK_FAILURE:
    //     case RIL_E_ILLEGAL_SIM_OR_ME:
    //     case RIL_E_MISSING_RESOURCE:
    //     case RIL_E_NO_SUCH_ELEMENT:
    //     case RIL_E_DIAL_MODIFIED_TO_USSD:
    //     case RIL_E_DIAL_MODIFIED_TO_SS:
    //     case RIL_E_DIAL_MODIFIED_TO_DIAL:
    //     case RIL_E_USSD_MODIFIED_TO_DIAL:
    //     case RIL_E_USSD_MODIFIED_TO_SS:
    //     case RIL_E_USSD_MODIFIED_TO_USSD:
    //     case RIL_E_SS_MODIFIED_TO_DIAL:
    //     case RIL_E_SS_MODIFIED_TO_USSD:
    //     case RIL_E_SUBSCRIPTION_NOT_SUPPORTED:
    //     case RIL_E_SS_MODIFIED_TO_SS:
    //     case RIL_E_LCE_NOT_SUPPORTED:
    //     case RIL_E_DIAL_STRING_TOO_LONG:
    //     case RIL_E_TEXT_STRING_TOO_LONG:
    //     case RIL_E_SIM_MEM_FULL:
    //     //case RIL_E_CALL_BARRED:
    //     case RIL_E_EXTERNAL_APP_CAUSE_BEGIN:
    //     case RIL_E_BT_SAP_UNDEFINED:
    //     case RIL_E_BT_SAP_NOT_ACCESSIBLE:
    //     case RIL_E_BT_SAP_CARD_REMOVED:
    //     case RIL_E_BT_SAP_UIM_NOT_INSERTED:
    //     case RIL_E_BT_SAP_ERROR_NO_REASON_DEFINED:
    //     //case RIL_E_BT_SAP_ALREADY_POWERED_OFF:
    //     //case RIL_E_BT_SAP_ALREADY_POWERED_ON:
    //     case RIL_E_VERIFICATION_FAILED:
    //     case RIL_E_REBOOT_REQUEST:
    //     //case RIL_E_HOLD_FAILED_CAUSED_BY_TERMINATED:
    //     case RIL_E_POWER_ON_UIM_NOT_READY:
    //         new_err = err;
    //         break;
    //     default:
    //         new_err = RIL_E_GENERIC_FAILURE;
    //         break;
    // }
    return err;
}

// request example

//case 1: parcel don't need to be modified, default
int32_t handleMtkRadioSim(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto mtkRadioSim = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_SIM_EX, slot);
  if (!mtkRadioSim) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy mtkRadioSim, request =%d, failed",
              requset);
    return -1;
  } else {
    std::shared_ptr<MtkRadioExSim> ptr =
        std::dynamic_pointer_cast<MtkRadioExSim>(mtkRadioSim);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleMtkRadioSim mtkRadioSim dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
      android::status_t status;
      int32_t request = 0;
      int32_t token;
      status = p->readInt32(&request);
      status = p->readInt32(&token);
      ret = ptr->getIccid(token);
    }
  }

  return ret;
}

int32_t handleMtkRadioIms(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto mtkRadioIms = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_IMS_EX, slot);
  if (!mtkRadioIms) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy mtkRadioIms, request =%d, failed",
              requset);
    return -1;
  } else {
    std::shared_ptr<MtkRadioExIms> ptr =
        std::dynamic_pointer_cast<MtkRadioExIms>(mtkRadioIms);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleMtkRadioIms mtkRadioSim dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
        android::status_t status;
        int32_t request = 0;
        int32_t token;
        status = p->readInt32(&request);
        status = p->readInt32(&token);
        if (requset == RIL_REQUEST_SET_IMSCFG) {
            int num;
            status = p->readInt32(&num);
            int32_t t;
            status = p->readInt32(&t);
            bool volteEnable = t ? true : false;
            status = p->readInt32(&t);
            bool vilteEnable = t ? true : false;
            status = p->readInt32(&t);
            bool vowifiEnale = t ? true : false;
            status = p->readInt32(&t);
            bool viwifiEnable = t ? true : false;
            status = p->readInt32(&t);
            bool smsEnable = t ? true : false;
            status = p->readInt32(&t);
            bool eimsEnable = t ? true : false;
            ret = ptr->setImscfg(token, volteEnable, vilteEnable, vowifiEnale,
                                viwifiEnable, smsEnable, eimsEnable);
      } else if (requset == RIL_REQUEST_HOLD_CALL) {
        int num;
        int controlType;
        int callId;
        status = p->readInt32(&num);
        status = p->readInt32(&controlType);
        status = p->readInt32(&callId);
        ret = ptr->controlCall(token, controlType, callId, CLIENT_TBOX_IMS);
      } else if (request == RIL_REQUEST_GET_IMSCFG) {
        ret = ptr->getImscfg(token);
      }
    }
  }

  return ret;
}

int32_t handleMtkRadioCall(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto mtkRadioCall = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_VOICE_EX, slot);
  if (!mtkRadioCall) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy mtkRadioSim, request =%d, failed",
              requset);
    return -1;
  } else {
    std::shared_ptr<MtkRadioExVoice> ptr =
        std::dynamic_pointer_cast<MtkRadioExVoice>(mtkRadioCall);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleMtkRadioCall mtkRadioSim dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
    android::status_t status;
    int32_t request = 0;
    int32_t token;
    status = p->readInt32(&request);
    status = p->readInt32(&token);
    if(requset == RIL_REQUEST_HANGUP_ALL) {
        ret = ptr->hangupAll(token, CLIENT_TBOX);
    } else if(requset == RIL_REQUEST_SET_CALL_INDICATION) {
        int num, mode, callId, seqNumber;
        status = p->readInt32(&num);
        status = p->readInt32(&mode);
        status = p->readInt32(&callId);
        status = p->readInt32(&seqNumber);
        ret = ptr->setCallIndication(token, mode, callId, seqNumber, 0, CLIENT_TBOX);
    }
  }
  }

  return ret;
}

int32_t handleMtkRadioEcall(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto mtkRadioEcall = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_ECALL_EX, slot);
  if (!mtkRadioEcall) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy mtkRadioEcall, request =%d, failed",
              requset);
    return -1;
  } else {
    std::shared_ptr<MtkRadioExEcall> ptr =
        std::dynamic_pointer_cast<MtkRadioExEcall>(mtkRadioEcall);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleMtkRadioEcall mtkRadioEcall dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
      android::status_t status;
      int32_t request = 0;
      int32_t token;
      status = p->readInt32(&request);
      status = p->readInt32(&token);
      if(requset == RIL_REQUEST_ECALL_RESET_IVS) {
        ret = ptr->resetIvs(token, CLIENT_TBOX);
      } else if(requset == RIL_REQUEST_ECALL_SET_MSD) {
        int call_id, length, digitLimit;
        unsigned char* msd_data;
        uint8_t uct = 0;

        status = p->readInt32(&call_id);
        status = p->readInt32(&length);

        digitLimit= MIN((length), MSD_MAX_LENGTH);
        msd_data = (unsigned char *)alloca(digitLimit);
        for(int digitCount = 0 ; digitCount < digitLimit; digitCount ++) {
            status = p->read(&uct, sizeof(uint8_t));
            msd_data[digitCount] = (uint8_t) uct;
        }

        ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetMSD
            in_setMSD;
        in_setMSD.call_id = call_id;
        in_setMSD.length = length;
        std::string str(reinterpret_cast<const char *>(msd_data));
        in_setMSD.msd_data = str;

        ret = ptr->setMSD(token, in_setMSD, CLIENT_TBOX);
        memset(msd_data, 0, length);
      } else if (requset == RIL_REQUEST_ECALL_SET_TEST_NUM) {
        int num, type;
        char* address;

        status = p->readInt32(&num);
        status = p->readInt32(&type);
        address = RpDataUtils::strdupReadString(p);

        ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum
            in_setTestNum;
        in_setTestNum.arg_num = num;
        in_setTestNum.type = type;
        in_setTestNum.address = std::string(address);
        ret = ptr->setTestNum(token, in_setTestNum, CLIENT_TBOX);
        RpDataUtils::freeMemory(address);
      } else if (requset == RIL_REQUEST_ECALL_SET_RECONF_NUM) {
        int num, type;
        char* address;

        status = p->readInt32(&num);
        status = p->readInt32(&type);
        address = RpDataUtils::strdupReadString(p);

        ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum
            in_setReconfNum;
        in_setReconfNum.arg_num = num;
        in_setReconfNum.type = type;
        in_setReconfNum.address = std::string(address);
        ret = ptr->setReconfNum(token, in_setReconfNum, CLIENT_TBOX);
        RpDataUtils::freeMemory(address);
      } else if (requset == RIL_REQUEST_ECALL_FAST_MAKE_ECALL) {
        int category = 0;
        int varient = 0;
        int length = 0;
        int domain = 0;
        int digitLimit = 0;
        int uct = 0;
        char* address;
        unsigned char* msd_data;

        status = p->readInt32(&category);
        status = p->readInt32(&varient);
        address = RpDataUtils::strdupReadString(p);
        status = p->readInt32(&length);

        digitLimit= MIN((length), MSD_MAX_LENGTH);
        msd_data = (unsigned char *)alloca(digitLimit);
        for(int digitCount = 0 ; digitCount < digitLimit; digitCount ++) {
            status = p->read(&uct, sizeof(uint8_t));
            msd_data[digitCount] = (uint8_t) uct;
            RFX_LOG_E(LOG_TAG, "tranferRequestRaw msd_data[%d]=%d", digitCount, msd_data[digitCount]);
        }

        status = p->readInt32(&domain);

        RFX_LOG_E(LOG_TAG, "tranferRequestRaw makefastecall in category=%d, varient=%d, address=%s, length=%d, domain=%d",
            category, varient, address, length, domain);

        ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallReqMsg
            in_ecallReqMsg;
        in_ecallReqMsg.ecall_cat = (::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallCategory)category;
        in_ecallReqMsg.ecall_variant = (::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallVariant)varient;
        in_ecallReqMsg.address = std::string(address);
        in_ecallReqMsg.length = length;

        for (int i = 0; i < length; i++) {
            in_ecallReqMsg.msd_data.push_back(msd_data[i]);
        }
        in_ecallReqMsg.domain = (::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallDomain)domain;

        RFX_LOG_E(LOG_TAG, "tranferRequestRaw makefastecall out category=%d, varient=%d, address=%s, length=%d, domain=%d",
            (int)in_ecallReqMsg.ecall_cat, (int)in_ecallReqMsg.ecall_variant, in_ecallReqMsg.address.c_str(), in_ecallReqMsg.length, (int)in_ecallReqMsg.domain);

        ret = ptr->makeFastEcall(token, in_ecallReqMsg, CLIENT_TBOX);
        RpDataUtils::freeMemory(address);
        memset(msd_data, 0, length);
      } else if (requset == RIL_REQUEST_ECALL_SET_PRI) {
        int num, data1, data2, data3, data4;
        status = p->readInt32(&num);
        status = p->readInt32(&data1);
        status = p->readInt32(&data2);
        status = p->readInt32(&data3);
        status = p->readInt32(&data4);

        ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallPRI
            in_ecallPri;
        in_ecallPri.data1 = data1;
        in_ecallPri.data2 = data2;
        in_ecallPri.data3 = data3;
        in_ecallPri.data4 = data4;
        ret = ptr->setEmsdPri(token, in_ecallPri, CLIENT_TBOX);
      } else if (requset == RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME) {
        int num, purpose, mode, timer1, timer2;
        status = p->readInt32(&num);
        status = p->readInt32(&purpose);
        status = p->readInt32(&mode);
        status = p->readInt32(&timer1);
        status = p->readInt32(&timer2);

        ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallTime
            in_ecallTime;
        in_ecallTime.purpose = purpose;
        in_ecallTime.mode = mode;
        in_ecallTime.timer1 = timer1;
        in_ecallTime.timer2 = timer2;
        ret = ptr->setNadDeregTime(token, in_ecallTime, CLIENT_TBOX);
      } else if (requset == RIL_REQUEST_ECALL_SET_OPRT_MODE) {
        int num, mode;
        status = p->readInt32(&num);
        status = p->readInt32(&mode);
        ret = ptr->setOprtMode(token, mode, CLIENT_TBOX);
      } else if (requset == RIL_REQUEST_ECALL_GET_OPRT_MODE) {
        ret = ptr->getOprtMode(token, CLIENT_TBOX);
      }
    }
  }
  return ret;
}

int32_t handleRadioModem(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto radioModem = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_MODEM, slot);
  if (!radioModem) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy radioModem, request =%d, failed",
              requset);
    return -1;
  } else {
    std::shared_ptr<RadioModem> ptr =
        std::dynamic_pointer_cast<RadioModem>(radioModem);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleRadioModem radioModem dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
      android::status_t status;
      int32_t request = 0;
      int32_t token;
      status = p->readInt32(&request);
      status = p->readInt32(&token);
      if (requset == RIL_REQUEST_DEVICE_IDENTITY) {
        ret = ptr->getDeviceIdentity(token);
      } else if (requset == RIL_REQUEST_RADIO_POWER) {
        int num, onff;
        status = p->readInt32(&num);
        status = p->readInt32(&onff);
      RFX_LOG_E(LOG_TAG,
                "RIL_REQUEST_RADIO_POWER request %d  token %d num %d, onff %d ",
                requset,token,num,onff);
        ret = ptr->setRadioPower(token, ((onff == 0) ? false : true), false,
                                 false);
      }
    }
  }

  return ret;
}

int32_t handleMtkRadioModem(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto radioModemEx = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_MODEM_EX, slot);
  if (!radioModemEx) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy radioModemEx, request =%d, failed",
              requset);
    return -1;
  } else {
    std::shared_ptr<MtkRadioExModem> ptr =
        std::dynamic_pointer_cast<MtkRadioExModem>(radioModemEx);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleMtkRadioModem radioModemEx dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
      android::status_t status;
      int32_t request = 0;
      int32_t token;
      status = p->readInt32(&request);
      status = p->readInt32(&token);
      if (requset == RIL_REQUEST_MODEM_POWERON) {
        ret = ptr->setModemPower(token, true);
      } else if (requset == RIL_REQUEST_MODEM_POWEROFF) {
        ret = ptr->setModemPower(token, false);
      }else if (requset == RIL_REQUEST_OEM_HOOK_RAW) {
        int num;
        int len;
        std::vector<uint8_t> data;
        status = p->readInt32(&len);
        const void* tmp = p->readInplace(len);
        num = len / sizeof(uint8_t);
        uint8_t *resp = (uint8_t *) tmp;
        for (int i = 0; i < num; i++) {
            data.push_back(resp[i]);
        }
        ret = ptr->sendRequestRaw(token, data);
      }
    }
  }

  return ret;
}

int32_t handleMtkRadioNetwork(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto radioNetwokrEx = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_NETWORK_EX, slot);
  if (!radioNetwokrEx) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy radioNetworkEx, request =%d, failed",
              requset);
    return -1;
  } else {
    std::shared_ptr<MtkRadioExNetWork> ptr =
        std::dynamic_pointer_cast<MtkRadioExNetWork>(radioNetwokrEx);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleMtkRadioNetwork radioNetwokrEx dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
      android::status_t status;
      int32_t request = 0;
      int32_t token;
      status = p->readInt32(&request);
      status = p->readInt32(&token);
      if (requset == RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT) {
        ret = ptr->getAvailableNetworksWithAct(token);
      } else if (requset == RIL_REQUEST_GET_NITZ) {
        ret = ptr->getNitzTime(token);
      }
    }
  }
  return ret;
}

int32_t handleRadioNetwork(int& requset, android::Parcel*& p, int slot) {
  int32_t ret = -1;
  auto radioNetwork = RadioServiceProxy::getInstance().getProxy(RadioServiceProxy::HAL_SERVICE_NETWORK, slot);
  if (!radioNetwork) {
    RFX_LOG_E(LOG_TAG, "tranferRequestRaw getProxy radioNetwork, request =%d, failed", requset);
    return -1;
  } else {
    std::shared_ptr<RadioNetWork> ptr = std::dynamic_pointer_cast<RadioNetWork>(radioNetwork);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleRadioNetwork radioNetwork dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
      android::status_t status;
      int32_t request = 0;
      int32_t token;
      status = p->readInt32(&request);
      status = p->readInt32(&token);
      if (request == RIL_REQUEST_VOICE_REGISTRATION_STATE) {
        ret = ptr->getVoiceRegistrationState(token);
      } else if (request == RIL_REQUEST_DATA_REGISTRATION_STATE) {
        ret = ptr->getDataRegistrationState(token);
      } else if (request == RIL_REQUEST_IMS_REGISTRATION_STATE) {
        ret = ptr->getImsRegistrationState(token);
      } else if (request == RIL_REQUEST_SIGNAL_STRENGTH) {
        ret = ptr->getSignalStrength(token);
      } else if (request == RIL_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP) {
        ret = ptr->getAllowedNetworkTypesBitmap(token);
      } else if (request == RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP) {
        int len;
        int networkTypeBitmap;
        status = p->readInt32(&len);
        status = p->readInt32(&networkTypeBitmap);
        ret = ptr->setAllowedNetworkTypesBitmap(token, networkTypeBitmap);
      } else if (request == RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER) {
        int len;
        int filter;
        status = p->readInt32(&len);
        status = p->readInt32(&filter);
        ret = ptr->setIndicationFilter(token, filter);
      } else if (request == RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE) {
        int len;
        int rate;
        status = p->readInt32(&len);
        status = p->readInt32(&rate);
        ret = ptr->setCellInfoListRate(token, rate);
      } else if (request == RIL_REQUEST_SET_USAGE_SETTING) {
        int settings = p->readInt32();
        ret = ptr->setUsageSetting(token, settings);
      } else if (request == RIL_REQUEST_GET_USAGE_SETTING) {
        ret = ptr->getUsageSetting(token);
      } else if (request == RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC) {
        ret = ptr->setNetworkSelectionModeAutomatic(token);
      } else if (request == RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL) {
        char* opNumeric = RpDataUtils::strdupReadString(p);
        int32_t ran = p->readInt32();
        ret = ptr->setNetworkSelectionModeManual(token, (opNumeric == NULL ? "" : opNumeric), static_cast<AccessNetwork>(ran));
        if (opNumeric != NULL) {
          RpDataUtils::freeMemory(opNumeric);
        }
      } else if (request == RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE) {
        ret = ptr->getNetworkSelectionMode(token);
      } else if (request == RIL_REQUEST_GET_CELL_INFO_LIST) {
        ret = ptr->getCellInfoList(token);
      } else if (request == RIL_REQUEST_OPERATOR) {
        ret = ptr->getOperator(token);
      }
    }
    return ret;
  }
}

int32_t handleRadioSim(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto radioSim = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_SIM, slot);
  if (!radioSim) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy radioSim, request =%d, failed",
              requset);
    return -1;
  } else {
    std::shared_ptr<RadioSim> ptr =
        std::dynamic_pointer_cast<RadioSim>(radioSim);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleRadioSim radioSim dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
        android::status_t status;
        int32_t request = 0;
        int32_t token;
        int num = 0;
        status = p->readInt32(&request);
        status = p->readInt32(&token);
        switch (requset){
        case RIL_REQUEST_GET_SIM_STATUS:{
            ret = ptr->getIccCardStatus(token);
            break;
        }
        case RIL_REQUEST_GET_IMSI:{
            status = p->readInt32(&num);
            char *aid_ptr = RpDataUtils::strdupReadString(p);
            ret = ptr->getImsiForApp(token, std::string(aid_ptr));
            RpDataUtils::freeMemory(aid_ptr);
            break;
        }
        case RIL_REQUEST_ENTER_SIM_PIN: {
            status = p->readInt32(&num);
            if (num < 2) {
                RLOGE("enterSimPinForApp: invalid parameter");
                return -1;
            }
            char *pin = RpDataUtils::strdupReadString(p);
            char *aid = RpDataUtils::strdupReadString(p);
            ret = ptr->supplyIccPinForApp(token, std::string(pin), std::string(aid));
            RpDataUtils::freeMemory(pin);
            RpDataUtils::freeMemory(aid);
            break;
        }
        case RIL_REQUEST_ENTER_SIM_PIN2: {
            status = p->readInt32(&num);
            if (num < 2) {
                RLOGE("enterSimPin2ForApp: invalid parameter");
                return -1;
            }
            char *pin2 = RpDataUtils::strdupReadString(p);
            char *aid = RpDataUtils::strdupReadString(p);
            ret = ptr->supplyIccPin2ForApp(token, std::string(pin2), std::string(aid));
            RpDataUtils::freeMemory(pin2);
            RpDataUtils::freeMemory(aid);
            break;
        }
        case RIL_REQUEST_ENTER_SIM_PUK: {
            status = p->readInt32(&num);
            if (num < 3) {
                RLOGE("enterSimPukForApp: invalid parameter");
                return -1;
            }
            char *puk = RpDataUtils::strdupReadString(p);
            char *pin = RpDataUtils::strdupReadString(p);
            char *aid = RpDataUtils::strdupReadString(p);
            ret = ptr->supplyIccPukForApp(token, std::string(puk), std::string(pin), std::string(aid));
            RpDataUtils::freeMemory(puk);
            RpDataUtils::freeMemory(pin);
            RpDataUtils::freeMemory(aid);
            break;
        }
        case RIL_REQUEST_ENTER_SIM_PUK2: {
            status = p->readInt32(&num);
            if (num < 3) {
                RLOGE("enterSimPuk2ForApp: invalid parameter");
                return -1;
            }
            char *puk2 = RpDataUtils::strdupReadString(p);
            char *pin2 = RpDataUtils::strdupReadString(p);
            char *aid = RpDataUtils::strdupReadString(p);
            ret = ptr->supplyIccPuk2ForApp(token, std::string(puk2), std::string(pin2), std::string(aid));
            RpDataUtils::freeMemory(puk2);
            RpDataUtils::freeMemory(pin2);
            RpDataUtils::freeMemory(aid);
            break;
        }
        case RIL_REQUEST_CHANGE_SIM_PIN: {
            status = p->readInt32(&num);
            if (num < 3) {
                RLOGE("changeSimPinForApp: invalid parameter");
                return -1;
            }
            char *old_pin = RpDataUtils::strdupReadString(p);
            char *new_pin = RpDataUtils::strdupReadString(p);
            char *aid = RpDataUtils::strdupReadString(p);
            ret = ptr->changeIccPinForApp(token, std::string(old_pin), std::string(new_pin), std::string(aid));
            RpDataUtils::freeMemory(old_pin);
            RpDataUtils::freeMemory(new_pin);
            RpDataUtils::freeMemory(aid);
            break;
        }
        case RIL_REQUEST_CHANGE_SIM_PIN2: {
            status = p->readInt32(&num);
            if (num < 3) {
                RLOGE("changeSimPin2ForApp: invalid parameter");
                return -1;
            }
            char *old_pin2 = RpDataUtils::strdupReadString(p);
            char *new_pin2 = RpDataUtils::strdupReadString(p);
            char *aid = RpDataUtils::strdupReadString(p);
            ret = ptr->changeIccPin2ForApp(token, std::string(old_pin2), std::string(new_pin2), std::string(aid));
            RpDataUtils::freeMemory(old_pin2);
            RpDataUtils::freeMemory(new_pin2);
            RpDataUtils::freeMemory(aid);
            break;
        }
        case RIL_REQUEST_SET_FACILITY_LOCK: {
            int32_t lockState;
            int32_t serviceClass;
            status = p->readInt32(&num);
            if (num < 5) {
                RLOGE("changeSimPin2ForApp: invalid parameter");
                return -1;
            }
            char *facility = RpDataUtils::strdupReadString(p);
            status = p->readInt32(&lockState);
            char *password = RpDataUtils::strdupReadString(p);
            status = p->readInt32(&serviceClass);
            char *appId = RpDataUtils::strdupReadString(p);
            ret = ptr->setFacilityLockForApp(token, std::string(facility), lockState, std::string(password), serviceClass, std::string(appId));
            RpDataUtils::freeMemory(facility);
            RpDataUtils::freeMemory(password);
            RpDataUtils::freeMemory(appId);
            break;
        }
        case RIL_REQUEST_SIM_IO: {
            ::aidl::android::hardware::radio::sim::IccIo iccio;
            int32_t command;
            int32_t fileid;
            int32_t in_p1;
            int32_t in_p2;
            int32_t in_p3;
            status = p->readInt32(&num);
            if (num < 9) {
                RLOGE("iccIoForApp: invalid parameter");
                return -1;
            }

            status = p->readInt32(&command);
            status = p->readInt32(&fileid);
            char *pathid = RpDataUtils::strdupReadString(p);
            if (pathid != nullptr) {
                iccio.path = std::string(pathid);
                RpDataUtils::freeMemory(pathid);
            } else {
                RFX_LOG_E(LOG_TAG,"RIL_REQUEST_SIM_IO error, pathid is nullptr");
                return -1;
            }
            status = p->readInt32(&in_p1);
            status = p->readInt32(&in_p2);
            status = p->readInt32(&in_p3);
            char *data = RpDataUtils::strdupReadString(p);
            if (data != nullptr) {
                iccio.data = std::string(data);
                RpDataUtils::freeMemory(data);
            } else {
                RFX_LOG_E(LOG_TAG,"RIL_REQUEST_SIM_IO error, data is nullptr");
                return -1;
            }
            char *pin2 = RpDataUtils::strdupReadString(p);
            if (pin2 != nullptr) {
                iccio.pin2 = std::string(pin2);
                RpDataUtils::freeMemory(pin2);
            } else {
                RFX_LOG_E(LOG_TAG,"RIL_REQUEST_SIM_IO error, pin2 is nullptr");
                return -1;
            }
            char *aid = RpDataUtils::strdupReadString(p);
            if (aid != nullptr) {
                iccio.data = std::string(aid);
                RpDataUtils::freeMemory(aid);
            } else {
                RFX_LOG_E(LOG_TAG,"RIL_REQUEST_SIM_IO error, aid is nullptr");
                return -1;
            }

            iccio.command = command;
            iccio.fileId = fileid;
            iccio.p1 = in_p1;
            iccio.p2 = in_p2;
            iccio.p3 = in_p3;
            ret = ptr->iccIoForApp(token,iccio);
            break;
        }
        case RIL_REQUEST_SET_SIM_CARD_POWER: {
            int32_t CardPowerStateInt;
            ::aidl::android::hardware::radio::sim::CardPowerState CardPowerState;
            status = p->readInt32(&num);
            if (num < 1) {
                RLOGE("changeSimPin2ForApp: invalid parameter");
                return -1;
            }

            status = p->readInt32(&CardPowerStateInt);
            CardPowerState = (::aidl::android::hardware::radio::sim::CardPowerState)CardPowerStateInt;
            RLOGE("CardPowerStateInt: %d ",CardPowerStateInt);
            ret = ptr->setSimCardPower(token, CardPowerState);
            break;
        }
        default:
            break;

        }
    }
  }
  return ret;
}

int32_t handleRadioSms(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto radioSms = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_MESSAGING, slot);
      if (!radioSms) {
        RFX_LOG_E(LOG_TAG,
                  "tranferRequestRaw getProxy radioSms, request =%d, failed",
                  requset);
        return -1;
      } else {
        std::shared_ptr<RadioMessaging> ptr =
            std::dynamic_pointer_cast<RadioMessaging>(radioSms);
        if (!ptr) {
          RFX_LOG_E(LOG_TAG,
                    "handleRadioSms radioSms dynamic_pointer_cast, request "
                    "=%d, failed",
                    requset);
        } else {
          android::status_t status;
          int32_t request = 0;
          int32_t token;
          int32_t pdunum;
          status = p->readInt32(&request);
          status = p->readInt32(&token);
          if (requset == RIL_REQUEST_SEND_SMS) {
            ::aidl::android::hardware::radio::messaging::GsmSmsMessage SmsMessage;
            p->setDataPosition(sizeof(uint32_t)*2);
            p->readInt32(&pdunum);
            char* smscPduStr = RpDataUtils::strdupReadString(p);
            if (smscPduStr != nullptr) {
                SmsMessage.smscPdu = std::string(smscPduStr);
                RpDataUtils::freeMemory(smscPduStr);
            } else {
                // Handle the error for null smscPduStr
                RFX_LOG_E(LOG_TAG,"RadioData::handleMtkRadioModem send sms error, smscPduStr is nullptr");
                return -1;
            }

            char* pduStr = RpDataUtils::strdupReadString(p);
            if (pduStr != nullptr) {
                SmsMessage.pdu = std::string(pduStr);
                RpDataUtils::freeMemory(pduStr);
            } else {
                // Handle the error for null pduStr
                RpDataUtils::freeMemory(smscPduStr);
                RFX_LOG_E(LOG_TAG,"RadioData::handleMtkRadioModem send sms error, pduStr is nullptr");
                return -1;
            }

            ret = ptr->sendSms(token, SmsMessage);
          } else if (request == RIL_REQUEST_SMS_ACKNOWLEDGE) {
            int32_t num = p->readInt32();
            int32_t success = p->readInt32();
            int32_t cause = p->readInt32();
            ret = ptr->acknowledgeLastIncomingGsmSms(
                token, (success == 1 ? true : false),
                cause == 0 ? SmsAcknowledgeFailCause::UNSPECIFIED_ERROR : static_cast<SmsAcknowledgeFailCause>(cause));
           }
        }
      }
  return ret;
}

int32_t handleRadioCall(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto radioCall = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_VOICE, slot);
      if (!radioCall) {
        RFX_LOG_E(LOG_TAG,
                  "tranferRequestRaw getProxy radioSms, request =%d, failed",
                  requset);
        return -1;
      } else {
        std::shared_ptr<RadioVoice> ptr =
            std::dynamic_pointer_cast<RadioVoice>(radioCall);
        if (!ptr) {
          RFX_LOG_E(LOG_TAG,
                    "handleRadioSms radioSms dynamic_pointer_cast, request "
                    "=%d, failed",
                    requset);
        } else {
          ::aidl::android::hardware::radio::voice::Dial dial;
          android::status_t status;
          int32_t request = 0;
          int32_t token;
          status = p->readInt32(&request);
          status = p->readInt32(&token);
          if (requset == RIL_REQUEST_DIAL) {
            int cli = 0;
            char *phonenum = RpDataUtils::strdupReadString(p);
            if (phonenum != nullptr) {
                dial.address = std::string(phonenum);
                RpDataUtils::freeMemory(phonenum);
            } else {
                RFX_LOG_E(LOG_TAG,"RIL_REQUEST_SIM_IO error, pin2 is nullptr");
                return -1;
            }
            status = p->readInt32(&cli);
            dial.clir = cli;
            ret = ptr->dial(token, dial);
          } else if (requset == RIL_REQUEST_GET_CURRENT_CALLS) {
            ret = ptr->getCurrentCalls(token);
          } else if (requset == RIL_REQUEST_ANSWER) {
            ret = ptr->acceptCall(token);
          } else if (requset == RIL_REQUEST_DTMF_START) {
            char *phonenum = RpDataUtils::strdupReadString(p);
            ret = ptr->startDtmf(token, phonenum);
          } else if (requset == RIL_REQUEST_DTMF_STOP) {
            ret = ptr->stopDtmf(token);
          } else if (requset == RIL_REQUEST_HANGUP) {
            int len;
            int callId;
            status = p->readInt32(&len);
            status = p->readInt32(&callId);
            ret = ptr->hangup(token, callId);
          } else if (requset == RIL_REQUEST_ENABLE_VONR) {
            bool enable = p->readBool();
            ret = ptr->setVoNrEnabled(token, enable);
          } else if (requset == RIL_REQUEST_IS_VONR_ENABLED) {
            ret = ptr->isVoNrEnabled(token);
          } else if (requset == RIL_REQUEST_SET_CALL_WAITING) {
            bool enable = p->readBool();
            int serviceClass;
            status = p->readInt32(&serviceClass);
            ret = ptr->setCallWaiting(token, enable, serviceClass);
          } else if (requset == RIL_REQUEST_QUERY_CALL_WAITING) {
            int serviceClass;
            status = p->readInt32(&serviceClass);
            ret = ptr->getCallWaiting(token, serviceClass);
          } else if (requset == RIL_REQUEST_LAST_CALL_FAIL_CAUSE) {
            ret = ptr->getLastCallFailCause(token);
          }
        }
      }
  return ret;
}

int32_t handleRadioConfig(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto radioConfig = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_CONFIG, slot);
      if (!radioConfig) {
        RFX_LOG_E(LOG_TAG,
                  "tranferRequestRaw getProxy radioConfig, request =%d, failed",
                  requset);
        return -1;
      } else {
        std::shared_ptr<RadioConfig> ptr =
            std::dynamic_pointer_cast<RadioConfig>(radioConfig);
        if (!ptr) {
          RFX_LOG_E(LOG_TAG,
                    "handleRadioConfig radioConfig dynamic_pointer_cast, request "
                    "=%d, failed",
                    requset);
        } else {
          android::status_t status;
          int32_t request = 0;
          int32_t token;
          status = p->readInt32(&request);
          status = p->readInt32(&token);
          if (requset == RIL_REQUEST_SET_PREFERRED_DATA_MODEM) {
            int num;
            status = p->readInt32(&num);
            int32_t t;
            status = p->readInt32(&t);
            ret = ptr->setPreferredDataModem(token, (int8_t)t);
          }
        }
      }
  return ret;
}

std::vector<uint8_t> ip_to_bytes(const std::string& ip_address) {
    std::vector<uint8_t> bytes;

    // Attempt to resolve the IP address to IPv4
    struct in_addr ipv4_addr;
    if (inet_pton(AF_INET, ip_address.c_str(), &ipv4_addr) == 1) {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&ipv4_addr);
        bytes.assign(ptr, ptr + sizeof(ipv4_addr));
        return bytes;
    }

    // Attempt to resolve the IP address to IPv6
    struct in6_addr ipv6_addr;
    if (inet_pton(AF_INET6, ip_address.c_str(), &ipv6_addr) == 1) {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&ipv6_addr);
        bytes.assign(ptr, ptr + sizeof(ipv6_addr));
        return bytes;
    }

    // "If neither, return an empty vector"
    RFX_LOG_E(LOG_TAG, "Invalid IP address format.");
    return bytes;
}

int32_t handleRadioData(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto radioData = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_DATA, slot);
  if (!radioData) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy radioData, request =%d, failed",
              requset);
    return -1;
  } else {

    std::shared_ptr<RadioData> ptr =
        std::dynamic_pointer_cast<RadioData>(radioData);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleRadioData RadioData dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
      android::status_t status;
      int32_t request = 0;
      int32_t token;
      status = p->readInt32(&request);
      status = p->readInt32(&token);
      if (requset == RIL_REQUEST_ALLOW_DATA) {
        int num;
        int allow;
        status = p->readInt32(&num);
        status = p->readInt32(&allow);
        ret = ptr->setDataAllowed(token, (allow ? true : false));
      } else if (requset == RIL_REQUEST_SET_INITIAL_ATTACH_APN) {
        ::aidl::android::hardware::radio::data::DataProfileInfo info;
        char *apn = RpDataUtils::strdupReadString(p);
        char *protocol = RpDataUtils::strdupReadString(p);
        int32_t authType = p->readInt32();
        char *username = RpDataUtils::strdupReadString(p);
        char *password = RpDataUtils::strdupReadString(p);
        info.apn = std::string(apn);
        info.protocol = RpDataUtils::convertStringToPdpProtocolType(protocol);
        info.authType =
            (::aidl::android::hardware::radio::data::ApnAuthType)authType;
        info.user = std::string(username);
        info.password = std::string(password);
        ptr->setInitialAttachApn(token, info);
        RpDataUtils::freeMemory(apn);
        RpDataUtils::freeMemory(protocol);
        RpDataUtils::freeMemory(username);
        RpDataUtils::freeMemory(password);
      } else if (requset == RIL_REQUEST_SETUP_DATA_CALL) {
        char *radioTechnology;
        char *profileid;
        char *apnName;
        char *user;
        char *password;
        char *authType;
        char *protocol;
        char *roamingProtocol;
        char *supportedApnTypesBitmap;
        char *bearerBitmap;
        char *modemCognitive;
        char *mtu;
        char *mvnoType;
        char *mvnoMatchData;
        char *roamingAllowed;
        char *reason;

        // The first bytes of the RIL parcel contain the request number and
        // the serial number - see processCommandBuffer(). Copy them over too.
        // int pos = p.dataPosition();

        int numParams = p->readInt32();

        if (numParams < 16) {
          RFX_LOG_D(LOG_TAG, "dispatchDataCall numParams(%d) < 16, error!",
                    numParams);
          return -1;
        }

        radioTechnology = RpDataUtils::strdupReadString(p);
        profileid = RpDataUtils::strdupReadString(p);
        apnName = RpDataUtils::strdupReadString(p);
        user = RpDataUtils::strdupReadString(p);
        password = RpDataUtils::strdupReadString(p);
        authType = RpDataUtils::strdupReadString(p);
        protocol = RpDataUtils::strdupReadString(p);
        roamingProtocol = RpDataUtils::strdupReadString(p);
        supportedApnTypesBitmap = RpDataUtils::strdupReadString(p);
        bearerBitmap = RpDataUtils::strdupReadString(p);
        modemCognitive = RpDataUtils::strdupReadString(p);
        mtu = RpDataUtils::strdupReadString(p);
        mvnoType = RpDataUtils::strdupReadString(p);
        mvnoMatchData = RpDataUtils::strdupReadString(p);
        roamingAllowed = RpDataUtils::strdupReadString(p);
        reason = RpDataUtils::strdupReadString(p);

        ::aidl::android::hardware::radio::AccessNetwork in_accessNetwork =
            ::aidl::android::hardware::radio::AccessNetwork::UNKNOWN;
        ::aidl::android::hardware::radio::data::DataProfileInfo
            in_dataProfileInfo;
        in_dataProfileInfo.profileId = atoi(profileid);
        in_dataProfileInfo.authType =
            (::aidl::android::hardware::radio::data::ApnAuthType)(
                atoi(authType));
        in_dataProfileInfo.apn = std::string(apnName);
        in_dataProfileInfo.protocol =
            RpDataUtils::convertStringToPdpProtocolType(protocol);
        in_dataProfileInfo.roamingProtocol =
            RpDataUtils::convertStringToPdpProtocolType(roamingProtocol);
        in_dataProfileInfo.user = std::string(user);
        in_dataProfileInfo.password = std::string(password);

        in_dataProfileInfo.type = 0;
        in_dataProfileInfo.maxConnsTime = 0;
        in_dataProfileInfo.maxConns = 0;
        in_dataProfileInfo.waitTime = 0;
        in_dataProfileInfo.enabled = true;
        in_dataProfileInfo.supportedApnTypesBitmap =
            atoi(supportedApnTypesBitmap);
        in_dataProfileInfo.bearerBitmap = atoi(bearerBitmap);
        in_dataProfileInfo.mtuV4 = atoi(mtu);
        in_dataProfileInfo.mtuV6 = atoi(mtu);

        bool in_roamingAllowed = atoi(roamingAllowed) ? true : false;
        ::aidl::android::hardware::radio::data::DataRequestReason in_reason =
            ::aidl::android::hardware::radio::data::DataRequestReason::NORMAL;
        std::vector<::aidl::android::hardware::radio::data::LinkAddress>
            in_addresses = {};
        const std::vector<std::string> in_dnses = {};
        int32_t in_pduSessionId = 0;
        std::optional<::aidl::android::hardware::radio::data::SliceInfo>
            in_sliceInfo = std::nullopt;
        bool in_matchAllRuleAllowed = true;
        ret = ptr->setupDataCall(token, in_accessNetwork, in_dataProfileInfo,
                                 in_roamingAllowed, in_reason, in_addresses,
                                 in_dnses, in_pduSessionId, in_sliceInfo,
                                 in_matchAllRuleAllowed);
        RpDataUtils::freeMemory(radioTechnology);
        RpDataUtils::freeMemory(profileid);
        RpDataUtils::freeMemory(apnName);
        RpDataUtils::freeMemory(user);
        RpDataUtils::freeMemory(password);
        RpDataUtils::freeMemory(authType);
        RpDataUtils::freeMemory(protocol);
        RpDataUtils::freeMemory(roamingProtocol);
        RpDataUtils::freeMemory(supportedApnTypesBitmap);
        RpDataUtils::freeMemory(bearerBitmap);
        RpDataUtils::freeMemory(modemCognitive);
        RpDataUtils::freeMemory(mtu);
        RpDataUtils::freeMemory(mvnoType);
        RpDataUtils::freeMemory(mvnoMatchData);
        RpDataUtils::freeMemory(roamingAllowed);
        RpDataUtils::freeMemory(reason);
      } else if (requset == RIL_REQUEST_DEACTIVATE_DATA_CALL) {
        android::status_t status;
        int num;
        char *cId;
        char *reason;
        status = p->readInt32(&num);
        cId = RpDataUtils::strdupReadString(p);
        reason = RpDataUtils::strdupReadString(p);

        ::aidl::android::hardware::radio::data::DataRequestReason in_reason =
            ::aidl::android::hardware::radio::data::DataRequestReason::NORMAL;
        int int_cId = atoi(cId);
        int int_reason = atoi(reason);
        switch (int_reason)
        {
        case 1:
            in_reason = ::aidl::android::hardware::radio::data::DataRequestReason::NORMAL;
            break;
        case 2:
            in_reason = ::aidl::android::hardware::radio::data::DataRequestReason::SHUTDOWN;
            break;
        case 3:
            in_reason = ::aidl::android::hardware::radio::data::DataRequestReason::HANDOVER;
            break;
        default:
            break;
        }
        ptr->deactivateDataCall(token, int_cId, in_reason);
        RpDataUtils::freeMemory(cId);
        RpDataUtils::freeMemory(reason);
      } else if (requset == RIL_REQUEST_DATA_CALL_LIST) {
        ptr->getDataCallList(token);
      } else if (request == RIL_REQUEST_START_KEEPALIVE) {
        ::aidl::android::hardware::radio::data::KeepaliveRequest in_keepalive;
        int32_t type = 0;
        char* sourceAddress;
        int32_t sourcePort = 0;
        char* destinationAddress;
        int32_t destinationPort = 0;
        int32_t maxKeepaliveIntervalMillis = 0;
        int32_t cid = 0;
        status = p->readInt32(&type);
        sourceAddress = RpDataUtils::strdupReadString(p);
        if (!sourceAddress) {
            RFX_LOG_E(LOG_TAG, "sourceAddress is null");
            return -1;
        }
        status = p->readInt32(&sourcePort);
        destinationAddress = RpDataUtils::strdupReadString(p);
        if (!destinationAddress) {
            RFX_LOG_E(LOG_TAG, "destinationAddress is null");
            RpDataUtils::freeMemory(sourceAddress);
            return -1;
        }
        status = p->readInt32(&destinationPort);
        status = p->readInt32(&maxKeepaliveIntervalMillis);
        status = p->readInt32(&cid);
        int srcLen = strlen(sourceAddress);
        int dstLen = strlen(destinationAddress);

        std::string srcStr(sourceAddress, srcLen);
        std::string dstStr(destinationAddress, dstLen);

        in_keepalive.type = type;
        in_keepalive.sourceAddress = ip_to_bytes(srcStr);
        in_keepalive.sourcePort = sourcePort;
        in_keepalive.destinationAddress = ip_to_bytes(dstStr);
        in_keepalive.destinationPort = destinationPort;
        in_keepalive.maxKeepaliveIntervalMillis = maxKeepaliveIntervalMillis;
        in_keepalive.cid = cid;
        ret = ptr->startKeepalive(token, in_keepalive);
        RpDataUtils::freeMemory(sourceAddress);
        RpDataUtils::freeMemory(destinationAddress);
      } else if (request == RIL_REQUEST_STOP_KEEPALIVE) {
        int32_t sessionHandle;
        status = p->readInt32(&sessionHandle);
        ret = ptr->stopKeepalive(token, sessionHandle);
      }
    }
  }

  return ret;
}

int32_t handleMtkRadioData(int &requset, android::Parcel *&p, int slot) {
  int32_t ret = -1;
  auto mtkRadioData = RadioServiceProxy::getInstance().getProxy(
      RadioServiceProxy::HAL_SERVICE_DATA_EX, slot);
  if (!mtkRadioData) {
    RFX_LOG_E(LOG_TAG,
              "tranferRequestRaw getProxy mtkRadioData, request =%d, failed",
              requset);
    return -1;
  } else {
    std::shared_ptr<MtkRadioExData> ptr =
        std::dynamic_pointer_cast<MtkRadioExData>(mtkRadioData);
    if (!ptr) {
      RFX_LOG_E(LOG_TAG,
                "handleMtkRadioData mtkRadioData dynamic_pointer_cast, request "
                "=%d, failed",
                requset);
    } else {
      android::status_t status;
      int32_t request = 0;
      int32_t token;
      status = p->readInt32(&request);
      status = p->readInt32(&token);
      if (requset == RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD) {
        std::vector<int32_t> in_settings;
        int num;
        status = p->readInt32(&num);
        int32_t t;
        for (int i = 0; i < num; i++) {
            status = p->readInt32(&t);
            in_settings.push_back(t);
        }
        ret = ptr->syncDataSettingsToMd(token, in_settings);
      }
    }
    return ret;
  }
}

bool IsNWrequest(int requset) {
    if(requset == RIL_REQUEST_VOICE_REGISTRATION_STATE ||
        requset == RIL_REQUEST_DATA_REGISTRATION_STATE ||
        requset == RIL_REQUEST_IMS_REGISTRATION_STATE ||
        requset == RIL_REQUEST_SIGNAL_STRENGTH ||
        requset == RIL_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP ||
        requset == RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP||
        requset == RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER ||
        requset == RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE ||
        requset == RIL_REQUEST_SET_USAGE_SETTING ||
        requset == RIL_REQUEST_GET_USAGE_SETTING ||
        requset == RIL_REQUEST_GET_CELL_INFO_LIST ||
        requset == RIL_REQUEST_OPERATOR ||
        requset == RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC ||
        requset == RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE ||
        requset == RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL) {
        return true;
    } else {
        return false;
    }
}

bool IsCallrequest(int requset) {
    if(requset == RIL_REQUEST_DIAL ||
        requset == RIL_REQUEST_GET_CURRENT_CALLS ||
        requset == RIL_REQUEST_ANSWER ||
        requset == RIL_REQUEST_ENABLE_VONR ||
        requset == RIL_REQUEST_IS_VONR_ENABLED ||
        requset == RIL_REQUEST_DTMF_START ||
        requset == RIL_REQUEST_DTMF_STOP ||
        requset == RIL_REQUEST_HANGUP ||
        requset == RIL_REQUEST_QUERY_CALL_WAITING ||
        requset == RIL_REQUEST_SET_CALL_WAITING ||
        requset == RIL_REQUEST_LAST_CALL_FAIL_CAUSE) {
        return true;
    } else {
        return false;
    }
}

bool IsDatarequest(int requset) {
    if(requset == RIL_REQUEST_ALLOW_DATA ||
        requset == RIL_REQUEST_SETUP_DATA_CALL ||
        requset == RIL_REQUEST_DEACTIVATE_DATA_CALL ||
        requset == RIL_REQUEST_DATA_CALL_LIST ||
        requset == RIL_REQUEST_SET_INITIAL_ATTACH_APN||
        requset == RIL_REQUEST_START_KEEPALIVE ||
        requset == RIL_REQUEST_STOP_KEEPALIVE) {
        return true;
    } else {
        return false;
    }
}

bool isSimRequest(int request) {
    if(request == RIL_REQUEST_GET_IMSI ||
        request == RIL_REQUEST_GET_SIM_STATUS ||
        request == RIL_REQUEST_ENTER_SIM_PIN ||
        request == RIL_REQUEST_ENTER_SIM_PUK ||
        request == RIL_REQUEST_ENTER_SIM_PIN2||
        request == RIL_REQUEST_ENTER_SIM_PUK2 ||
        request == RIL_REQUEST_CHANGE_SIM_PIN ||
        request == RIL_REQUEST_CHANGE_SIM_PIN2 ||
        request == RIL_REQUEST_SET_FACILITY_LOCK ||
        request == RIL_REQUEST_SIM_IO ||
        request == RIL_REQUEST_SET_SIM_CARD_POWER) {
        return true;
    } else {
        return false;
    }
}

int32_t handleAospRadio(int &requset, android::Parcel *&p, int slot) {
  int ret = 0;
  if (requset == RIL_REQUEST_DEVICE_IDENTITY ||
      requset == RIL_REQUEST_RADIO_POWER) {
    if (handleRadioModem(requset, p, slot) < 0) {
        ret =1;
    }
  } else if (isSimRequest(requset)) {
    if (handleRadioSim(requset, p, slot) < 0) {
        ret =1;
    }
  } else if (IsDatarequest(requset)) {
    if (handleRadioData(requset, p, slot) < 0) {
        ret =1;
    }
  } else if (IsNWrequest(requset)) {
    if (handleRadioNetwork(requset, p, slot) < 0) {
        ret =1;
    }
  } else if (requset == RIL_REQUEST_SEND_SMS || requset == RIL_REQUEST_SMS_ACKNOWLEDGE) {
    if (handleRadioSms(requset, p, slot) < 0) {
        ret =1;
    }
  } else if (IsCallrequest(requset)) {
    if (handleRadioCall(requset, p, slot) < 0) {
        ret =1;
    }
  } else if (requset == RIL_REQUEST_SET_PREFERRED_DATA_MODEM) {
    if (handleRadioConfig(requset, p, slot) < 0) {
        ret =1;
    }
  }
  return ret;
}

bool IsEcallrequest(int requset) {
    if(requset == RIL_REQUEST_ECALL_RESET_IVS ||
            requset == RIL_REQUEST_ECALL_SET_MSD ||
            requset == RIL_REQUEST_ECALL_SET_TEST_NUM ||
            requset == RIL_REQUEST_ECALL_SET_RECONF_NUM ||
            requset == RIL_REQUEST_ECALL_FAST_MAKE_ECALL ||
            requset == RIL_REQUEST_ECALL_SET_PRI ||
            requset == RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME ||
            requset == RIL_REQUEST_ECALL_SET_OPRT_MODE ||
            requset == RIL_REQUEST_ECALL_GET_OPRT_MODE) {
        return true;
    } else {
        return false;
    }
}

bool isMtkRadioImsRequest(int request) {
    if(request == RIL_REQUEST_SET_IMSCFG ||
             request == RIL_REQUEST_GET_IMSCFG ||
             request == RIL_REQUEST_HOLD_CALL) {
        return true;
    }
    return false;
}

int32_t handleMtkRadio(int &request, android::Parcel *&p, int slot) {
  int ret = 0;
  if (request == RIL_REQUEST_SIM_GET_ICCID) {
    if (handleMtkRadioSim(request, p, slot) < 0) {
        ret =1;
    }
  } else if (request == RIL_REQUEST_MODEM_POWERON ||
                request == RIL_REQUEST_MODEM_POWEROFF ||
                request == RIL_REQUEST_OEM_HOOK_RAW) {
    if (handleMtkRadioModem(request, p, slot) < 0) {
        ret =1;
    }
  } else if (isMtkRadioImsRequest(request)) {
        ret = handleMtkRadioIms(request, p, slot) < 0 ? 1 : 0;
  } else if (request == RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD) {
    if (handleMtkRadioData(request, p, slot) < 0) {
        ret =1;
    }
  } else if (request == RIL_REQUEST_HANGUP_ALL ||
                request == RIL_REQUEST_SET_CALL_INDICATION) {
    if (handleMtkRadioCall(request, p, slot) < 0) {
        ret =1;
    }
  } else if (IsEcallrequest(request)) {
    if (handleMtkRadioEcall(request, p, slot) < 0) {
        ret =1;
    }
  } else if (request == RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT ||
             request == RIL_REQUEST_GET_NITZ) {
     if (handleMtkRadioNetwork(request, p, slot) < 0) {
        ret =1;
    }
  }
  return ret;
}

const std::unordered_set<int> kMtkRequests = {
    RIL_REQUEST_SIM_GET_ICCID,
    RIL_REQUEST_MODEM_POWERON,
    RIL_REQUEST_MODEM_POWEROFF,
    RIL_REQUEST_OEM_HOOK_RAW,
    RIL_REQUEST_SET_IMSCFG,
    RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD,
    RIL_REQUEST_HANGUP_ALL,
    RIL_REQUEST_SET_CALL_INDICATION,
    RIL_REQUEST_ECALL_RESET_IVS,
    RIL_REQUEST_ECALL_SET_MSD,
    RIL_REQUEST_ECALL_SET_TEST_NUM,
    RIL_REQUEST_ECALL_SET_RECONF_NUM,
    RIL_REQUEST_ECALL_FAST_MAKE_ECALL,
    RIL_REQUEST_ECALL_SET_PRI,
    RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME,
    RIL_REQUEST_ECALL_SET_OPRT_MODE,
    RIL_REQUEST_ECALL_GET_OPRT_MODE,
    RIL_REQUEST_HOLD_CALL,
    RIL_REQUEST_GET_NITZ,
    RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT,
    RIL_REQUEST_GET_IMSCFG,
};

bool IsMtkRequest(int request) {
    return kMtkRequests.count(request) > 0;
}

const std::unordered_set<int> kAospRequests = {
    RIL_REQUEST_DEVICE_IDENTITY,
    RIL_REQUEST_RADIO_POWER,
    RIL_REQUEST_GET_IMSI,
    RIL_REQUEST_GET_SIM_STATUS,
    RIL_REQUEST_ALLOW_DATA,
    RIL_REQUEST_SETUP_DATA_CALL,
    RIL_REQUEST_DEACTIVATE_DATA_CALL,
    RIL_REQUEST_DATA_CALL_LIST,
    RIL_REQUEST_SET_INITIAL_ATTACH_APN,
    RIL_REQUEST_VOICE_REGISTRATION_STATE,
    RIL_REQUEST_DATA_REGISTRATION_STATE,
    RIL_REQUEST_IMS_REGISTRATION_STATE,
    RIL_REQUEST_SIGNAL_STRENGTH,
    RIL_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP,
    RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP,
    RIL_REQUEST_SEND_SMS,
    RIL_REQUEST_SMS_ACKNOWLEDGE,
    RIL_REQUEST_DIAL,
    RIL_REQUEST_DTMF_START,
    RIL_REQUEST_DTMF_STOP,
    RIL_REQUEST_GET_CURRENT_CALLS,
    RIL_REQUEST_ANSWER,
    RIL_REQUEST_START_KEEPALIVE,
    RIL_REQUEST_STOP_KEEPALIVE,
    RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER,
    RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE,
    RIL_REQUEST_ENTER_SIM_PIN,
    RIL_REQUEST_ENTER_SIM_PIN2,
    RIL_REQUEST_ENTER_SIM_PUK,
    RIL_REQUEST_ENTER_SIM_PUK2,
    RIL_REQUEST_CHANGE_SIM_PIN,
    RIL_REQUEST_CHANGE_SIM_PIN2,
    RIL_REQUEST_SET_FACILITY_LOCK,
    RIL_REQUEST_SIM_IO,
    RIL_REQUEST_SET_SIM_CARD_POWER,
    RIL_REQUEST_HANGUP,
    RIL_REQUEST_SET_PREFERRED_DATA_MODEM,
    RIL_REQUEST_SET_USAGE_SETTING,
    RIL_REQUEST_GET_USAGE_SETTING,
    RIL_REQUEST_OPERATOR,
    RIL_REQUEST_GET_CELL_INFO_LIST,
    RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL,
    RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC,
    RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE,
    RIL_REQUEST_ENABLE_VONR,
    RIL_REQUEST_IS_VONR_ENABLED,
    RIL_REQUEST_QUERY_CALL_WAITING,
    RIL_REQUEST_SET_CALL_WAITING,
    RIL_REQUEST_LAST_CALL_FAIL_CAUSE,
};

bool IsAospRequest(int request) {
    return kAospRequests.count(request) > 0;
}

Parcel *tranferRequestRaw(int slot, int requset, Parcel *p) {
  p->setDataPosition(0);
  if (IsAospRequest(requset)) {
    if (handleAospRadio(requset, p, slot) < 0) {
      return nullptr;
    }
  } else if (IsMtkRequest(requset)) {
    if (handleMtkRadio(requset, p, slot) < 0) {
      return nullptr;
    }
  }

  Parcel *newp = new Parcel();
  p->setDataPosition(0);
  RFX_LOG_D(LOG_TAG, "tranferRequestRaw, request =%d", requset);
  newp->appendFrom(p, 0, p->dataSize());
  fillReqID(newp, requset);
  return newp;
}
//case 2: parcel need tarfer, by request id:
Parcel* tranferRequestExample(int version, int requset, Parcel *p){
    Parcel *newp = new Parcel();
    copyReqHeader(newp, p);
    //-->todo, tranfer parcel
    fillReqID(newp, requset);
    return newp;
}

Parcel* tranferRequestOpenChannel(int slot, int requset, Parcel *p){
    char *aid = NULL;
    Parcel *newp = new Parcel();

    copyReqHeader(newp, p);
    aid = strdupReadString(p);
    writeStringToParcel(newp, aid);
    newp->writeInt32(0);
    fillReqID(newp, requset);
    if(aid)
        free(aid);
    return newp;
}
Parcel* tranferRequestSetDataProfile(int slot, int requset, Parcel *p){
    Parcel *newp = new Parcel();
    //p->setDataPosition(0);
    int profile_num;
    const char16_t    *tmp_s16 = NULL, *protocol=NULL;
    size_t         stringlen = 0, protocol_len= 0;
    int         authype = 0, tmp_value = 0, profile_id= 0, supportedTypesBitmask=0;
#define DATAASST_PDN_APN_TYPE_DEFAULT      0x1
//for IOT
#define IOT_DATAASST_PDN_APN_TYPE_DEFAULT   (0x00100000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_0     (0x00200000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_1     (0x00400000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_2     (0x00800000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_3     (0x01000000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_4     (0x02000000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_5     (0x04000000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_6     (0x08000000)
    RFX_LOG_D(LOG_TAG, "tranferRequestSetDataProfile, request =%d", requset);
    if (newp == NULL) {
        return NULL;
    }
    /*profile num*/
    copyReqHeader(newp, p);
    p->readInt32(&profile_num);
    newp->writeInt32(profile_num);
    RFX_LOG_D(LOG_TAG, "tranferRequestSetDataProfile, profile_num =%d", profile_num);
    /**/
    for (int i = 0 ; i < profile_num ; i++ )
    {
        /*profileId*/
        p->readInt32(&profile_id);
        newp->writeInt32(profile_id);
        RFX_LOG_D(LOG_TAG, "tranferRequestSetDataProfile, profile_id =%d", profile_id);
        /*apn*/
        stringlen = 0;
        tmp_s16 = p->readString16Inplace(&stringlen);
        newp->writeString16(tmp_s16, stringlen);
        /*protocol*/
        protocol_len = 0;
        protocol = p->readString16Inplace(&protocol_len);
        newp->writeString16(protocol, protocol_len);

        /*auth_type*/
        p->readInt32(&authype);
        newp->writeInt32(authype);
            /*user name*/
        tmp_s16 = p->readString16Inplace(&stringlen);
        newp->writeString16(tmp_s16, stringlen);
        /*password*/
        tmp_s16 = p->readString16Inplace(&stringlen);
        newp->writeString16(tmp_s16, stringlen);
        /*type*/
        p->readInt32(&tmp_value);
        newp->writeInt32(tmp_value);
          /*maxConnsTime*/
        p->readInt32(&tmp_value);
        newp->writeInt32(tmp_value);
        /*maxConns*/
        p->readInt32(&tmp_value);
        newp->writeInt32(tmp_value);
        /*waitTime*/
        p->readInt32(&tmp_value);
        newp->writeInt32(tmp_value);
        /*enabled*/
        p->readInt32(&tmp_value);
        newp->writeInt32(tmp_value);

        /*can handle type bitmask*/
        supportedTypesBitmask = profile_id;
        if(profile_id&(IOT_DATAASST_PDN_APN_TYPE_DEFAULT
            |IOT_DATAASST_PDN_APN_TYPE_NET_0
            |IOT_DATAASST_PDN_APN_TYPE_NET_1
            |IOT_DATAASST_PDN_APN_TYPE_NET_2
            |IOT_DATAASST_PDN_APN_TYPE_NET_3
            |IOT_DATAASST_PDN_APN_TYPE_NET_4
            |IOT_DATAASST_PDN_APN_TYPE_NET_5
            |IOT_DATAASST_PDN_APN_TYPE_NET_6
                )
            )
        {
            supportedTypesBitmask = (profile_id & (~(IOT_DATAASST_PDN_APN_TYPE_DEFAULT
                |IOT_DATAASST_PDN_APN_TYPE_NET_0
                |IOT_DATAASST_PDN_APN_TYPE_NET_1
                |IOT_DATAASST_PDN_APN_TYPE_NET_2
                |IOT_DATAASST_PDN_APN_TYPE_NET_3
                |IOT_DATAASST_PDN_APN_TYPE_NET_4
                |IOT_DATAASST_PDN_APN_TYPE_NET_5
                |IOT_DATAASST_PDN_APN_TYPE_NET_6
                ) ) | DATAASST_PDN_APN_TYPE_DEFAULT);
        }

        RFX_LOG_D(LOG_TAG,"above supportedApnTypesBitmap: %d", supportedTypesBitmask);
        newp->writeInt32(supportedTypesBitmask);
         /*roamingProtocol*/
        RFX_LOG_D(LOG_TAG,"above roamingProtocol: %s", protocol);
        newp->writeString16(protocol, protocol_len);

        /*bearerBitmask*/
        newp->writeInt32(0);

        /*mtu, not used in syc APN table*/
        newp->writeInt32(1500);

        /*mvnoType*/
        newp->writeString16(NULL, 0);

        /*mvnoMatchData*/
        newp->writeString16(NULL, 0);

        // read modemCognitive
        newp->writeInt32(0);
    }
    fillReqID(newp, requset);

    newp->setDataPosition(0);
    if(requset == RIL_REQUEST_SET_DATA_PROFILE) {
        auto radioData = RadioServiceProxy::getInstance().getProxy(RadioServiceProxy::HAL_SERVICE_DATA, slot);
        if (radioData.get() == nullptr) {
            RFX_LOG_E(LOG_TAG, "tranferRequestSetDataProfile getProxy radioData, request =%d, failed", requset);
            return nullptr;
        } else {
            std::shared_ptr<RadioData> ptr= std::dynamic_pointer_cast<RadioData>(radioData);
            if(ptr.get() == nullptr) {
                RFX_LOG_E(LOG_TAG, "tranferRequestSetDataProfile RadioData dynamic_pointer_cast, request =%d, failed", requset);
            } else {
                android::status_t status;
                int32_t request = 0;
                int32_t token;
                status = newp->readInt32(&request);
                status = newp->readInt32 (&token);
                RFX_LOG_E(LOG_TAG, "tranferRequestSetDataProfile1");
                if (requset == RIL_REQUEST_SET_DATA_PROFILE) {
                RFX_LOG_E(LOG_TAG, "tranferRequestSetDataProfile2");
                    int32_t num = 0;
                    int32_t t = 0;
                    ::aidl::android::hardware::radio::data::DataProfileInfo info;
                    std::vector<::aidl::android::hardware::radio::data::DataProfileInfo> in_profiles;
                    status = newp->readInt32(&num);
                    in_profiles.resize(num);
                    if (status != android::NO_ERROR || num < 0) {
                        RFX_LOG_E(LOG_TAG, "tranferRequestSetDataProfile read num failed");
                    }
                    RFX_LOG_D(LOG_TAG, "tranferRequestSetDataProfile, num =%d", num);
                    for (int i = 0 ; i < num ; i++ ) {
                        status = newp->readInt32(&t);
                        in_profiles[i].profileId = (int) t;
                        RFX_LOG_D(LOG_TAG, "tranferRequestSetDataProfile, profileId =%d", t);

                        char* apn =  strdupReadString(newp);
                        if(apn){
                            RFX_LOG_I(LOG_TAG,"apn:%s",apn);
                            in_profiles[i].apn = std::string(apn);
                            RpDataUtils::freeMemory(apn);
                        } else {
                            RFX_LOG_E(LOG_TAG,"apn is null");
                            in_profiles[i].apn = std::string("");
                        }


                        char* protocol =  strdupReadString(newp);
                        if (protocol) {
                            RFX_LOG_D(LOG_TAG,"protocol: %s",protocol);
                            in_profiles[i].protocol = RpDataUtils::convertStringToPdpProtocolType(protocol);
                            RpDataUtils::freeMemory(protocol);
                        } else {
                            RFX_LOG_E(LOG_TAG,"protocol is null, set IP");
                            in_profiles[i].protocol = RpDataUtils::convertStringToPdpProtocolType(protocol);
                        }


                        status = newp->readInt32(&t);
                        RFX_LOG_D(LOG_TAG,"authType: %d",t);
                        if(t >= 0 && t <= 3) {
                            in_profiles[i].authType = (::aidl::android::hardware::radio::data::ApnAuthType) t;
                        } else {
                             in_profiles[i].authType = ::aidl::android::hardware::radio::data::ApnAuthType::NO_PAP_NO_CHAP; //0
                        }


                        char* user =  strdupReadString(newp);
                        if (user) {
                            RFX_LOG_D(LOG_TAG,"usr: %s",user);
                            in_profiles[i].user = std::string(user);
                            RpDataUtils::freeMemory(user);
                        } else {
                            RFX_LOG_E(LOG_TAG,"usr is null");
                            in_profiles[i].user = std::string("");
                        }


                        char* password =  strdupReadString(newp);
                        if (password) {
                            RFX_LOG_D(LOG_TAG,"password: %s", password);
                            in_profiles[i].password = std::string(password);
                            RpDataUtils::freeMemory(password);
                        } else {
                            RFX_LOG_E(LOG_TAG,"password is null");
                            in_profiles[i].password = std::string("");
                        }


                        status = newp->readInt32(&t);
                        in_profiles[i].type = t;
                        RFX_LOG_D(LOG_TAG,"type: %d", t);

                        status = newp->readInt32(&t);
                        in_profiles[i].maxConnsTime = t;
                        RFX_LOG_D(LOG_TAG,"maxConnsTime: %d", t);

                        status = newp->readInt32(&t);
                        in_profiles[i].maxConns =  t;
                        RFX_LOG_D(LOG_TAG,"maxConns: %d", t);

                        status = newp->readInt32(&t);
                        in_profiles[i].waitTime = t;
                        RFX_LOG_D(LOG_TAG,"waitTime: %d", t);

                        status = newp->readInt32(&t);
                        in_profiles[i].enabled = t ? true : false;
                        RFX_LOG_D(LOG_TAG,"enabled: %d", t);

                        status = newp->readInt32(&t);
                        in_profiles[i].supportedApnTypesBitmap = t;
                        RFX_LOG_D(LOG_TAG,"supportedApnTypesBitmap: %d", t);

                        char* roamingProtocol = strdupReadString(newp);
                        RFX_LOG_D(LOG_TAG,"roamingProtocol: %s", roamingProtocol);
                        in_profiles[i].roamingProtocol = RpDataUtils::convertStringToPdpProtocolType(roamingProtocol);
                        RpDataUtils::freeMemory(roamingProtocol);

                        status = newp->readInt32(&t);
                        in_profiles[i].bearerBitmap = t;
                        RFX_LOG_D(LOG_TAG,"bearerBitmap: %d", t);

                        status = newp->readInt32(&t);
                        in_profiles[i].mtuV4 = t;
                        RFX_LOG_D(LOG_TAG, "tranferRequestSetDataProfile, mtu =%d", t);

                        //unused, only by pass
                        char* mvnoType =  strdupReadString(newp);
                        if(mvnoType == nullptr) {
                            RFX_LOG_D(LOG_TAG, "tranferRequestSetDataProfile, mvnoType is null");
                        }
                        RpDataUtils::freeMemory(mvnoType);
                        char* mvnoMatchData =  strdupReadString(newp);
                        if(mvnoMatchData == nullptr) {
                            RFX_LOG_D(LOG_TAG, "tranferRequestSetDataProfile, mvnoMatchData is null");
                        }
                        RpDataUtils::freeMemory(mvnoMatchData);
                        newp->readInt32(&t);
                        RFX_LOG_D(LOG_TAG, "tranferRequestSetDataProfile, modemCognitive =%d", t);
                    }
                    ptr->setDataProfile(token, in_profiles);
                }
            }
        }
    }
    newp->setDataPosition(0);
    return newp;
}

Parcel* tranferRequestSetPreferredNetworkType(int slot, int requset, Parcel *p){
    Parcel *newp = new Parcel();

    copyReqHeader(newp, p);
    //-->todo, tranfer parcel
    RFX_LOG_D(LOG_TAG, "tranferRequestSetPreferredNetworkType, request =%d", requset);
    TransferUtilsPreferredNetworkType(p, newp);
    fillReqID(newp, requset);
    return newp;
}
Parcel* tranferRequestRadioTech(int slot, int requset, Parcel *p){
    Parcel *newp = new Parcel();
    RFX_LOG_D(LOG_TAG, "tranferRequestRadioTech, request =%d", requset);
    copyReqHeader(newp, p);
    //-->todo, tranfer parcel
    TransferUtilsRadioTechnology(p,newp);
    fillReqID(newp, requset);
    return newp;
}

//response
Parcel* tranferResponseRaw(int version, int response, Parcel *p, int* error){
    int new_err;
    Parcel *newp = new Parcel();
    RFX_LOG_D(LOG_TAG, "tranferResponseRaw, response =%d", response);
    newp->appendFrom(p, 0, p->dataSize());
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}
Parcel* tranferResponseExample(int version, int response, Parcel *p, int* error){
    Parcel *newp = new Parcel();
    int new_err;
    copyRespHeader(newp, p);

    //-->todo, tranfer parcel


    //please remember to transfer error to correct value
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}

Parcel* tranferSignalStrength(int version, int response, Parcel *p, int* error){
    Parcel *newp = new Parcel();
    int new_err;
    RIL_SignalStrength_v14 sig_strength;
    int count = 0;
    int tmp;

    copyRespHeader(newp, p);
    if (p->dataAvail() > 0) {
        p->readInt32(&count);
        if(count>=17){
            //gsm_signal_strength
            p->readInt32(&sig_strength.GW_SignalStrength.signalStrength);
            //gsm_bit_error_rate
            p->readInt32(&sig_strength.GW_SignalStrength.bitErrorRate);
            //gsm_timing_advance
            p->readInt32(&sig_strength.GW_SignalStrength.timingAdvance);
            //cdma_dbm
            p->readInt32(&sig_strength.CDMA_SignalStrength.dbm);
            //cdma_ecio
            p->readInt32(&sig_strength.CDMA_SignalStrength.ecio);
            //evdo_dbm
            p->readInt32(&sig_strength.EVDO_SignalStrength.dbm);
            //evdo_ecio
            p->readInt32(&sig_strength.EVDO_SignalStrength.ecio);
            //evdo_snr
            p->readInt32(&sig_strength.EVDO_SignalStrength.signalNoiseRatio);
            //lte_signal_strength
            p->readInt32(&sig_strength.LTE_SignalStrength.signalStrength);
            //lte_rsrp
            p->readInt32(&sig_strength.LTE_SignalStrength.rsrp);
            //lte_rsrq
            p->readInt32(&sig_strength.LTE_SignalStrength.rsrq);
            //lte_rssnr
            p->readInt32(&sig_strength.LTE_SignalStrength.rssnr);
            //lte_cqi
            p->readInt32(&sig_strength.LTE_SignalStrength.cqi);
            //lte_timing_advance
            p->readInt32(&sig_strength.LTE_SignalStrength.timingAdvance);
            //tdscdma_signal_strength
            p->readInt32(&sig_strength.TD_SCDMA_SignalStrength.signalStrength);
            //tdscdma_bit_error_rate
            p->readInt32(&sig_strength.TD_SCDMA_SignalStrength.bitErrorRate);
            //tdscdma_rscp
            p->readInt32(&sig_strength.TD_SCDMA_SignalStrength.rscp);
            //wcdma.signalStrength
            p->readInt32(&sig_strength.WCDMA_SignalStrength.signalStrength);
            //wcdma.bitErrorRate
            p->readInt32(&sig_strength.WCDMA_SignalStrength.bitErrorRate);
            //wcdma.rscp
            p->readInt32(&sig_strength.WCDMA_SignalStrength.rscp);
            //wcdma.ecno
            p->readInt32(&sig_strength.WCDMA_SignalStrength.ecno);
            //nr.ss_rsrp
            p->readInt32(&sig_strength.NR_SignalStrength.ssRsrp);
            //nr.ss_rsrq
            p->readInt32(&sig_strength.NR_SignalStrength.ssRsrq);
            //nr.ss_sinr
            p->readInt32(&sig_strength.NR_SignalStrength.ssSinr);
            //nr.csi_rsrp
            p->readInt32(&sig_strength.NR_SignalStrength.csiRsrp);
            //nr.csi_rsrq
            p->readInt32(&sig_strength.NR_SignalStrength.csiRsrq);
            //nr.csi_sinr
            p->readInt32(&sig_strength.NR_SignalStrength.csiSinr);
            RFX_LOG_D(LOG_TAG, "tranferSignalStrength: gsm_signalStrength=%d, gsm_bitErrorRate=%d, gsm_timingAdvance=%d, \
                CDMA_dbm=%d, CDMA_ecio=%d, \
                EVDO_dbm=%d, EVDO_ecio=%d, EVDO_snr=%d, \
                LTE_signalStrength=%d, LTE_rsrp=%d, LTE_rsrq=%d, LTE_rssnr=%d, LTE_cqi=%d, LTE_timingAdvance=%d, \
                TD_SCDMA_signalStrength=%d, TD_SCDMA_bitErrorRate=%d, TD_SCDMA_rscp=%d \
                WCDMA_signalStrength=%d, WCDMA_bitErrorRate=%d, WCDMA_rscp=%d, WCDMA_ecno=%d\
                NR_SignalStrength.ssRsrp=%d, NR_SignalStrength.ssRsrq=%d, NR_SignalStrength.ssSinr=%d\
                NR_SignalStrength.csiRsrp=%d, NR_SignalStrength.csiRsrq=%d, NR_SignalStrength.csiSinr=%d",
                sig_strength.GW_SignalStrength.signalStrength,sig_strength.GW_SignalStrength.bitErrorRate,sig_strength.GW_SignalStrength.timingAdvance,
                sig_strength.CDMA_SignalStrength.dbm,sig_strength.CDMA_SignalStrength.ecio,sig_strength.EVDO_SignalStrength.dbm,
                sig_strength.EVDO_SignalStrength.ecio,sig_strength.EVDO_SignalStrength.signalNoiseRatio,sig_strength.LTE_SignalStrength.signalStrength,
                sig_strength.LTE_SignalStrength.rsrp, sig_strength.LTE_SignalStrength.rsrq,sig_strength.LTE_SignalStrength.rssnr,
                sig_strength.LTE_SignalStrength.cqi,sig_strength.LTE_SignalStrength.timingAdvance,
                sig_strength.TD_SCDMA_SignalStrength.signalStrength, sig_strength.TD_SCDMA_SignalStrength.bitErrorRate, sig_strength.TD_SCDMA_SignalStrength.rscp,
                sig_strength.WCDMA_SignalStrength.signalStrength, sig_strength.WCDMA_SignalStrength.bitErrorRate, sig_strength.WCDMA_SignalStrength.rscp, sig_strength.WCDMA_SignalStrength.ecno,
                sig_strength.NR_SignalStrength.ssRsrp, sig_strength.NR_SignalStrength.ssRsrq, sig_strength.NR_SignalStrength.ssSinr,
                sig_strength.NR_SignalStrength.csiRsrp, sig_strength.NR_SignalStrength.csiRsrq, sig_strength.NR_SignalStrength.csiSinr);
            newp->writeInt32(sig_strength.GW_SignalStrength.signalStrength);
            newp->writeInt32(sig_strength.GW_SignalStrength.bitErrorRate);
            newp->writeInt32(sig_strength.GW_SignalStrength.timingAdvance);
            newp->writeInt32(sig_strength.CDMA_SignalStrength.dbm);
            newp->writeInt32(sig_strength.CDMA_SignalStrength.ecio);
            newp->writeInt32(sig_strength.EVDO_SignalStrength.dbm);
            newp->writeInt32(sig_strength.EVDO_SignalStrength.ecio);
            newp->writeInt32(sig_strength.EVDO_SignalStrength.signalNoiseRatio);
            newp->writeInt32(sig_strength.LTE_SignalStrength.signalStrength);
            newp->writeInt32(sig_strength.LTE_SignalStrength.rsrp);
            newp->writeInt32(sig_strength.LTE_SignalStrength.rsrq);
            newp->writeInt32(sig_strength.LTE_SignalStrength.rssnr);
            newp->writeInt32(sig_strength.LTE_SignalStrength.cqi);
            newp->writeInt32(sig_strength.LTE_SignalStrength.timingAdvance);
            newp->writeInt32(sig_strength.TD_SCDMA_SignalStrength.signalStrength);
            newp->writeInt32(sig_strength.TD_SCDMA_SignalStrength.bitErrorRate);
            newp->writeInt32(sig_strength.TD_SCDMA_SignalStrength.rscp);
            newp->writeInt32(sig_strength.WCDMA_SignalStrength.signalStrength);
            newp->writeInt32(sig_strength.WCDMA_SignalStrength.bitErrorRate);
            newp->writeInt32(sig_strength.WCDMA_SignalStrength.rscp);
            newp->writeInt32(sig_strength.WCDMA_SignalStrength.ecno);
            newp->writeInt32(sig_strength.NR_SignalStrength.ssRsrp);
            newp->writeInt32(sig_strength.NR_SignalStrength.ssRsrq);
            newp->writeInt32(sig_strength.NR_SignalStrength.ssSinr);
            newp->writeInt32(sig_strength.NR_SignalStrength.csiRsrp);
            newp->writeInt32(sig_strength.NR_SignalStrength.csiRsrq);
            newp->writeInt32(sig_strength.NR_SignalStrength.csiSinr);
        }else {
            RFX_LOG_E(LOG_TAG, "tranferSignalStrength: count=%d", count);
        }
    }
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}

Parcel* tranferResponseDataCallList(int version, int response, Parcel *p, int* error){
    Parcel *newp = new Parcel();
    int new_err;
    copyRespHeader(newp, p);
    RFX_LOG_D(LOG_TAG, "tranferResponseDataCallList, response =%d", response);
    TransferUtilsDataCallList(p, newp);

    //please remember to transfer error to correct value
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}
Parcel* tranferResponseGetPreferredNetworkType(int version, int response, Parcel *p, int* error){
    Parcel *newp = new Parcel();
    int  new_err;

    RFX_LOG_D(LOG_TAG, "tranferResponseGetPreferredNetworkType, response =%d", response);
    copyRespHeader(newp,p);
    /*Read old parcel value, set the unknow value to PDP_FAIL_ERROR_UNSPECIFIED*/
    TransferUtilsPreferredNetworkType(p,newp);
    //please remember to transfer error to correct value
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}

Parcel* tranferResponseRadioTech(int version, int response, Parcel *p, int* error){
    Parcel *newp = new Parcel();
    int new_err;
    copyRespHeader(newp, p);

    //-->todo, tranfer parcel
    RFX_LOG_D(LOG_TAG, "tranferResponseRadioTech, response =%d", response);
    TransferUtilsRadioTechnology(p,newp);

    //please remember to transfer error to correct value
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}
Parcel* tranferResponseCellInfoList(int version, int response, Parcel *p, int* error){
    Parcel *newp = new Parcel();
    int new_err;
    copyRespHeader(newp, p);
    //-->todo, tranfer parcel
    RFX_LOG_D(LOG_TAG, "tranferResponseCellInfoList, response =%d", response);
    TransferUtilsCellInfo(p,newp);

    //please remember to transfer error to correct value
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}


Parcel* tranferResponseGetCurrentCalls(int version, int response, Parcel *p, int* error){
    Parcel *newp = new Parcel();
    int new_err;
    copyRespHeader(newp, p);
    //-->todo, tranfer parcel

    typedef struct _uus_info{
        int    uusType;
        int    uusDcs;
        int    uusLength;
        const char16_t *uusData;
    } uus_Info_t;

    typedef struct _cc_call_list{
        int state;
        int index;
        int toa;
        int isMpty;
        int isMT;
        int als;
        int isVoice;
        int isVoicePrivacy;
        const char16_t *number;
        int numberPresentation;
        const char16_t *name;
        int namePresentation;
        int isUUSpresent;
        uus_Info_t *uusInfo;
    } cc_call_list_t;

    int num;
    int speechCodec;
    cc_call_list_t callinfo;
    uus_Info_t uusinfo;
    size_t stringlen;

    p->readInt32(&num);
    newp->writeInt32(num);

    RFX_LOG_D(LOG_TAG, "[tranferGetCurrentCallsResponse] number is %d",num);

    for(int i=0;i<num;i++){

    p->readInt32(&callinfo.state);
    newp->writeInt32(callinfo.state);

    p->readInt32(&callinfo.index);
    newp->writeInt32(callinfo.index);

    p->readInt32(&callinfo.toa);
    newp->writeInt32(callinfo.toa);

    p->readInt32(&callinfo.isMpty);
    newp->writeInt32(callinfo.isMpty);

    p->readInt32(&callinfo.isMT);
    newp->writeInt32(callinfo.isMT);

    p->readInt32(&callinfo.als);
    newp->writeInt32(callinfo.als);

    p->readInt32(&callinfo.isVoice);
    newp->writeInt32(callinfo.isVoice);

    p->readInt32(&callinfo.isVoicePrivacy);
    newp->writeInt32(callinfo.isVoicePrivacy);

    stringlen = 0;
    callinfo.number = p->readString16Inplace(&stringlen);
    newp->writeString16(callinfo.number, stringlen);

    p->readInt32(&callinfo.numberPresentation);
    newp->writeInt32(callinfo.numberPresentation);

    stringlen = 0;
    callinfo.name = p->readString16Inplace(&stringlen);
    newp->writeString16(callinfo.name, stringlen);

    p->readInt32(&callinfo.numberPresentation);
    newp->writeInt32(callinfo.numberPresentation);

    p->readInt32(&callinfo.isUUSpresent);
    newp->writeInt32(callinfo.isUUSpresent);

    if(callinfo.isUUSpresent == 1)
    {
        p->readInt32(&uusinfo.uusType);
        newp->writeInt32(uusinfo.uusType);

        p->readInt32(&uusinfo.uusDcs);
        newp->writeInt32(uusinfo.uusDcs);

        p->readInt32(&uusinfo.uusLength);
        newp->writeInt32(uusinfo.uusLength);

        stringlen = 0;
        uusinfo.uusData = p->readString16Inplace(&stringlen);
        newp->writeString16(uusinfo.uusData, stringlen);
    }

    p->readInt32(&speechCodec);

    }
    //please remember to transfer error to correct value
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}



Parcel* transferResponseSimStatus(int version, int response, Parcel *p, int* error){
    RIL_CardStatus_v6* card_status;
    Parcel *newp = new Parcel();
    int32_t v = 0;
    int new_err;
    RFX_LOG_D(LOG_TAG, "transferResponseSimStatus, response =%d", response);
    copyRespHeader(newp, p);
    new_err = transferErrNum(response, *error);
    *error = new_err;
    if (p->dataAvail() > 0) {
        card_status = (RIL_CardStatus_v6 *)calloc(1, sizeof(RIL_CardStatus_v6));
        if(card_status == NULL) {
            RFX_LOG_D(LOG_TAG, "transferResponseSimStatus calloc fail!", card_status);
            return newp;
        }
        p->readInt32(&v);
        if(v > RIL_CARDSTATE_ERROR) {
            v = RIL_CARDSTATE_ERROR;
        }
        card_status->card_state = RIL_CardState(v);
        p->readInt32(&v);
        card_status->universal_pin_state = RIL_PinState(v);
        p->readInt32(&card_status->gsm_umts_subscription_app_index);
        p->readInt32(&card_status->cdma_subscription_app_index);
        p->readInt32(&card_status->ims_subscription_app_index);
        p->readInt32(&card_status->num_applications);
        for(int i = 0; i < card_status->num_applications; i++) {
            p->readInt32(&v);
            card_status->applications[i].app_type = RIL_AppType(v);
            p->readInt32(&v);
            card_status->applications[i].app_state = RIL_AppState(v);
            p->readInt32(&v);
            card_status->applications[i].perso_substate = RIL_PersoSubstate(v);
            card_status->applications[i].aid_ptr = strdupReadString(p);
            card_status->applications[i].app_label_ptr = strdupReadString(p);
            p->readInt32(&card_status->applications[i].pin1_replaced);
            p->readInt32(&v);
            card_status->applications[i].pin1 = RIL_PinState(v);
            p->readInt32(&v);
            card_status->applications[i].pin2 = RIL_PinState(v);
        }
    }else {
        return newp;
    }

    newp->writeInt32(card_status->card_state);
    newp->writeInt32(card_status->universal_pin_state);
    newp->writeInt32(card_status->gsm_umts_subscription_app_index);
    newp->writeInt32(card_status->cdma_subscription_app_index);
    newp->writeInt32(card_status->ims_subscription_app_index);
    newp->writeInt32(card_status->num_applications);
    for (int i = 0; i < card_status->num_applications; i++) {
        newp->writeInt32(card_status->applications[i].app_type);
        newp->writeInt32(card_status->applications[i].app_state);
        newp->writeInt32(card_status->applications[i].perso_substate);
        writeStringToParcel(newp, (const char*)(card_status->applications[i].aid_ptr));
        writeStringToParcel(newp, (const char*)(card_status->applications[i].app_label_ptr));
        newp->writeInt32(card_status->applications[i].pin1_replaced);
        newp->writeInt32(card_status->applications[i].pin1);
        newp->writeInt32(card_status->applications[i].pin2);
        RFX_LOG_D(LOG_TAG, "app_type=%d,app_state=%d,perso_substate=%d,\
                    aid_ptr=%s,app_label_ptr=%s,pin1_replaced=%d,pin1=%d,pin2=%d",
                    card_status->applications[i].app_type,
                    card_status->applications[i].app_state,
                    card_status->applications[i].perso_substate,
                    card_status->applications[i].aid_ptr,
                    card_status->applications[i].app_label_ptr,
                    card_status->applications[i].pin1_replaced,
                    card_status->applications[i].pin1,
                    card_status->applications[i].pin2);
    }

    for (int i = 0; i < card_status->num_applications; i++) {
        free(card_status->applications[i].aid_ptr);
        free(card_status->applications[i].app_label_ptr);
    }
    free(card_status);
    RFX_LOG_D(LOG_TAG, "transferResponseSimStatus end");
    return newp;
}


Parcel* tranferResponseVoiceRegisterState(int version, int response, Parcel *p, int* error){
    Parcel *newp = new Parcel();
    Parcel *old_parcel = p;
    Parcel *new_parcel = newp;
    int     rat = 0, tmp_value = 0, valid_count =0;
    char            *responseStr[15]={NULL};
    int     regState;
    RIL_CellInfoType type;
    int new_err;
    copyRespHeader(newp, p);

    memset(responseStr, 0, sizeof(char *) * 15);
    //-->todo, tranfer parcel

    /*regState*/
    old_parcel->readInt32(&regState);
    asprintf(&responseStr[0], "%d", regState);

    /*rat*/
    old_parcel->readInt32(&rat);
    if(19 == rat)
    {
        rat=14;
    }
    //type = (RIL_CellInfoType)getCellInfoTypeRadioTechnology(rat);
    asprintf(&responseStr[3], "%d", rat);
    /*cssSupported -diff*/
    old_parcel->readInt32(&tmp_value);
    asprintf(&responseStr[7], "%d", tmp_value);
    /*roamingIndicator -diff*/
    old_parcel->readInt32(&tmp_value);
    asprintf(&responseStr[10], "%d", tmp_value);

    /*systemIsInPrl -diff*/
    old_parcel->readInt32(&tmp_value);
    asprintf(&responseStr[11], "%d", tmp_value);
    /*defaultRoamingIndicator -diff*/
    old_parcel->readInt32(&tmp_value);
    asprintf(&responseStr[12], "%d", tmp_value);
    /*reasonForDenial -diff*/
    old_parcel->readInt32(&tmp_value);
    asprintf(&responseStr[13], "%d", tmp_value);
    // TODO: cellIdentity
    /*cellInfoType*/
    old_parcel->readInt32(&tmp_value);
    type = RIL_CellInfoType(tmp_value);
    switch(type)
    {
        case RIL_CELL_INFO_TYPE_GSM: {
            /*cellIdentity.mcc*/
            old_parcel->readInt32(&tmp_value);
            /*cellIdentity.mnc*/
            old_parcel->readInt32(&tmp_value);
            /*cellIdentitymnc_len  -diff*/
            old_parcel->readInt32(&tmp_value);
            /*cellIdentity.lac*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[1], "%d", tmp_value);
            /*cellIdentity.cid*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*cellIdentity.arfcn   -diff*/
            old_parcel->readInt32(&tmp_value);
            /*cellIdentity.bsic  -diff*/
            old_parcel->readInt32(&tmp_value);

            /*cellIdentity.operName.long_name    -diff*/
            /*cellIdentity.operName.short_name   -diff*/

            break;
        }
        case RIL_CELL_INFO_TYPE_WCDMA: {
            /*CellInfo.wcdma.cellIdentityWcdma.mcc*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.mnc*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.mnc_len  -diff*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.lac*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[1], "%d", tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.cid*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.psc*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[14], "%d", tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.uarfcn   -diff*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.operName.long_name   -diff*/
            /*CellInfo.wcdma.cellIdentityWcdma.operName.short_name  -diff*/

            break;
        }
        case RIL_CELL_INFO_TYPE_CDMA: {
            /*networkId*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[9], "%d", tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.systemId*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[8], "%d", tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.basestationId*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[4], "%d", tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.longitude*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[6], "%d", tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.latitude*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[5], "%d", tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.operName.long_name   -diff*/
            /*CellInfo.wcdma.cellIdentityWcdma.operName.short_name  -diff*/

            break;
        }
        case RIL_CELL_INFO_TYPE_LTE: {

            /*CellInfo.lte.cellIdentityLte.mcc*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.lte.cellIdentityLte.mnc*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.lte.cellIdentityLte.mnc_len  -diff*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.lte.cellIdentityLte.ci*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*CellInfo.lte.cellIdentityLte.pci*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.lte.cellIdentityLte.tac*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.lte.cellIdentityLte.earfcn  -diff*/
            old_parcel->readInt32(&tmp_value);

            /*CellInfo.lte.cellIdentityLte.operName.longname  -diff*/
            /*CellInfo.lte.cellIdentityLte.operName.shortname  -diff*/
            /*CellInfo.lte.cellIdentityLte.bandwidth  -diff*/

            break;
        }
        case RIL_CELL_INFO_TYPE_TD_SCDMA: {
            /*mcc*/
            old_parcel->readInt32(&tmp_value);
            /*mnc*/
            old_parcel->readInt32(&tmp_value);
            /*mnc_len  -diff */
            old_parcel->readInt32(&tmp_value);
            /*lac*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[1], "%d", tmp_value);
            /*cid*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*cpid*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.tdscdma.cellIdentityTdscdma.operName.longname    -diff */
            /*CellInfo.tdscdma.cellIdentityTdscdma.operName.shortname   -diff */

            break;
        }
        case RIL_CELL_INFO_TYPE_NR: {
            /*nr_cell.mcc*/
            old_parcel->readInt32(&tmp_value);
            /*nr_cell.mnc*/
            old_parcel->readInt32(&tmp_value);
            /*nr_cell.mnc_len  -diff*/
            old_parcel->readInt32(&tmp_value);
            /*nr_cell.cid*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*nr_cell.pci*/
            old_parcel->readInt32(&tmp_value);
            /*nr_cell.tac*/
            old_parcel->readInt32(&tmp_value);
            /*nr_cell.nr_arfcn  -diff*/
            old_parcel->readInt32(&tmp_value);
            break;
        }
        default :
            tmp_value=-1;
            asprintf(&responseStr[1], "%d", tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            break;
    }

    if(regState != 1 && regState != 5)
    {
        for (int i = 1; i < 4; i ++) {
            if(responseStr[i] != NULL) {
                free(responseStr[i]);
            }
        }

        tmp_value=-1;
        asprintf(&responseStr[1], "%d", tmp_value);
        asprintf(&responseStr[2], "%d", tmp_value);
        rat = 0;
        asprintf(&responseStr[3], "%d", rat);
    }

    new_parcel->writeInt32 (15);
    for(int i = 0; i < 15; i ++)
    {
        if(NULL != responseStr[i])
        {

            writeStringToParcel(newp,responseStr[i]);
            valid_count++;
            free(responseStr[i]);
        }
        else
        {
            writeStringToParcel(newp,"");
        }

    }
    RFX_LOG_D(LOG_TAG, "tranferResponseVoiceRegisterState, response =%d,rat = %d,type=%d,valid_count=%d", response, rat, type, valid_count);

    //please remember to transfer error to correct value
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}
Parcel* tranferResponseDataRegisterState(int version, int response, Parcel *p, int* error){
    Parcel *newp = new Parcel();
    Parcel *old_parcel = p;
    Parcel *new_parcel = newp;
    int rat = 0, tmp_value = 0, valid_count=0;
    char *responseStr[11]={NULL};
    int regState = 0;
    RIL_CellInfoType type;
    int new_err;

    copyRespHeader(newp, p);

    memset(responseStr, 0, sizeof(char *) * 11);
    //-->todo, tranfer parcel

    /*regState*/
    old_parcel->readInt32(&regState);
    asprintf(&responseStr[0], "%d", regState);

    /*rat*/
    old_parcel->readInt32(&rat);
    if(19 == rat)
    {
        RFX_LOG_D(LOG_TAG, "The_Data_register_status is:4G+");
        rat=14;
    }
    else if(14 == rat)
    {
        RFX_LOG_D(LOG_TAG, "The_Data_register_status is:4G");
    }
    else
    {
        RFX_LOG_D(LOG_TAG, "The_Data_register_status is:%d", rat);
    }
    //type = (RIL_CellInfoType)getCellInfoTypeRadioTechnology(rat);
    asprintf(&responseStr[3], "%d", rat);

    /*reasonDataDenied*/
    old_parcel->readInt32(&tmp_value);
    asprintf(&responseStr[4], "%d", tmp_value);
    /*maxDataCalls*/
    old_parcel->readInt32(&tmp_value);
    asprintf(&responseStr[5], "%d", tmp_value);

    old_parcel->readInt32(&tmp_value);
    type = RIL_CellInfoType(tmp_value);
    // TODO: cellIdentity
    switch(type)
    {
        case RIL_CELL_INFO_TYPE_GSM: {
            /*cellIdentity.mcc*/
            old_parcel->readInt32(&tmp_value);
            /*cellIdentity.mnc*/
            old_parcel->readInt32(&tmp_value);
            /*cellIdentitymnc_len  -diff*/
            old_parcel->readInt32(&tmp_value);
            /*cellIdentity.lac*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[1], "%d", tmp_value);
            /*cellIdentity.cid*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*cellIdentity.arfcn   -diff*/
            old_parcel->readInt32(&tmp_value);
            /*cellIdentity.bsic  -diff*/
            old_parcel->readInt32(&tmp_value);

            /*cellIdentity.operName.long_name    -diff*/
            /*cellIdentity.operName.short_name   -diff*/

            break;
        }
        case RIL_CELL_INFO_TYPE_WCDMA: {
            /*CellInfo.wcdma.cellIdentityWcdma.mcc*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.mnc*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.mnc_len  -diff*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.lac*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[1], "%d", tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.cid*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.psc*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.uarfcn   -diff*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.operName.long_name   -diff*/
            /*CellInfo.wcdma.cellIdentityWcdma.operName.short_name  -diff*/

            break;
        }
        case RIL_CELL_INFO_TYPE_CDMA: {
            /*CellInfo.cdma.cellIdentityCdma.networkId*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.systemId*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.basestationId*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.longitude*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.cdma.cellIdentityCdma.latitude*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.wcdma.cellIdentityWcdma.operName.long_name   -diff*/
            /*CellInfo.wcdma.cellIdentityWcdma.operName.short_name  -diff*/

            break;
        }
        case RIL_CELL_INFO_TYPE_LTE: {

            /*CellInfo.lte.cellIdentityLte.mcc*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.lte.cellIdentityLte.mnc*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.lte.cellIdentityLte.mnc_len  -diff*/
            old_parcel->readInt32(&tmp_value);
            /*CellInfo.lte.cellIdentityLte.ci*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*CellInfo.lte.cellIdentityLte.pci*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[7], "%d", tmp_value);
            /*CellInfo.lte.cellIdentityLte.tac*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[6], "%d", tmp_value);
            /*CellInfo.lte.cellIdentityLte.earfcn  -diff*/
            old_parcel->readInt32(&tmp_value);

            /*CellInfo.lte.cellIdentityLte.operName.longname  -diff*/
            /*CellInfo.lte.cellIdentityLte.operName.shortname  -diff*/
            /*CellInfo.lte.cellIdentityLte.bandwidth  -diff*/

            break;
        }
        case RIL_CELL_INFO_TYPE_TD_SCDMA: {
            /*RIL_CellIdentity_v16.RIL_CellIdentityTdscdma.mcc*/
            old_parcel->readInt32(&tmp_value);
            /*RIL_CellIdentity_v16.RIL_CellIdentityTdscdma..mnc*/
            old_parcel->readInt32(&tmp_value);
            /*RIL_CellIdentity_v16.RIL_CellIdentityTdscdma.mnc_len   -diff */
            old_parcel->readInt32(&tmp_value);
            /*RIL_CellIdentity_v16.RIL_CellIdentityTdscdma.lac*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[1], "%d", tmp_value);
            /*RIL_CellIdentity_v16.RIL_CellIdentityTdscdma.cid*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*RIL_CellIdentity_v16.RIL_CellIdentityTdscdma.cpid*/
            old_parcel->readInt32(&tmp_value);
            /*RIL_CellIdentity_v16.RIL_CellIdentityTdscdma.longname    -diff */
            /*RIL_CellIdentity_v16.RIL_CellIdentityTdscdma.shortname   -diff */

            break;
        }
        case RIL_CELL_INFO_TYPE_NR: {
            /*nr_cell.mcc*/
            old_parcel->readInt32(&tmp_value);
            /*nr_cell.mnc*/
            old_parcel->readInt32(&tmp_value);
            /*nr_cell.mnc_len  -diff*/
            old_parcel->readInt32(&tmp_value);
            /*nr_cell.cid*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            /*nr_cell.pci*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[7], "%d", tmp_value);
            /*nr_cell.tac*/
            old_parcel->readInt32(&tmp_value);
            asprintf(&responseStr[6], "%d", tmp_value);
            /*nr_cell.nr_arfcn  -diff*/
            old_parcel->readInt32(&tmp_value);

            break;
        }
        default :
            asprintf(&responseStr[1], "%d", tmp_value);
            asprintf(&responseStr[2], "%d", tmp_value);
            break;
    }
    if(regState != 1 && regState != 5)
    {
        for (int i = 1; i < 4; i ++) {
            if(responseStr[i] != NULL) {
                free(responseStr[i]);
            }
        }

        tmp_value=-1;
        asprintf(&responseStr[1], "%d", tmp_value);
        asprintf(&responseStr[2], "%d", tmp_value);
        rat = 0;
        asprintf(&responseStr[3], "%d", rat);

    }

    new_parcel->writeInt32 (11);
    for(int i = 0; i < 11; i ++)
    {

        if(NULL != responseStr[i])
        {
            RFX_LOG_V(LOG_TAG, "tranferResponseDataRegisterState, response =%d, id=%d, %s", response, i,responseStr[i]);
            writeStringToParcel(newp,responseStr[i]);
            valid_count++;
            free(responseStr[i]);
        }
        else
        {
            writeStringToParcel(newp,"");
        }
    }
    RFX_LOG_D(LOG_TAG, "tranferResponseDataRegisterState, response =%d, rat = %d, type=%d, valid_count= %d", response, rat, type, valid_count);

    //please remember to transfer error to correct value
    new_err = transferErrNum(response, *error);
    *error = new_err;
    return newp;
}

//unsol
Parcel* tranferUnsolRaw(int version, int unsol, Parcel *p){
    Parcel *newp = new Parcel();
    RFX_LOG_D(LOG_TAG, "tranferUnsolRaw, unsol =%d", unsol);
    newp->appendFrom(p, 0, p->dataSize());
    return newp;
}


Parcel* tranferUnsolExample(int version, int unsol, Parcel *p){

    Parcel *newp = new Parcel();
    RFX_LOG_D(LOG_TAG, "tranferUnsolExample, unsol =%d", unsol);
    copyUrcHeader(newp, p);
    //-->todo, tranfer parcel

    return newp;
}

Parcel* transferUnsolSignalStrength(int version, int unsol, Parcel *p){
    Parcel *newp = new Parcel();
    int count = 0;
    int tmp;
    RIL_SignalStrength_v14 sig_strength;

    copyUrcHeader(newp, p);
    if (p->dataAvail() > 0) {
        p->readInt32(&count);
        if(count>=17){
            //gsm_signal_strength
            p->readInt32(&sig_strength.GW_SignalStrength.signalStrength);
            //gsm_bit_error_rate
            p->readInt32(&sig_strength.GW_SignalStrength.bitErrorRate);
            //gsm_timing_advance
            p->readInt32(&sig_strength.GW_SignalStrength.timingAdvance);
            //cdma_dbm
            p->readInt32(&sig_strength.CDMA_SignalStrength.dbm);
            //cdma_ecio
            p->readInt32(&sig_strength.CDMA_SignalStrength.ecio);
            //evdo_dbm
            p->readInt32(&sig_strength.EVDO_SignalStrength.dbm);
            //evdo_ecio
            p->readInt32(&sig_strength.EVDO_SignalStrength.ecio);
            //evdo_snr
            p->readInt32(&sig_strength.EVDO_SignalStrength.signalNoiseRatio);
            //lte_signal_strength
            p->readInt32(&sig_strength.LTE_SignalStrength.signalStrength);
            //lte_rsrp
            p->readInt32(&sig_strength.LTE_SignalStrength.rsrp);
            //lte_rsrq
            p->readInt32(&sig_strength.LTE_SignalStrength.rsrq);
            //lte_rssnr
            p->readInt32(&sig_strength.LTE_SignalStrength.rssnr);
            //lte_cqi
            p->readInt32(&sig_strength.LTE_SignalStrength.cqi);
            //lte_timing_advance
            p->readInt32(&sig_strength.LTE_SignalStrength.timingAdvance);
            //tdscdma_signal_strength
            p->readInt32(&sig_strength.TD_SCDMA_SignalStrength.signalStrength);
            //tdscdma_bit_error_rate
            p->readInt32(&sig_strength.TD_SCDMA_SignalStrength.bitErrorRate);
            //tdscdma_rscp
            p->readInt32(&sig_strength.TD_SCDMA_SignalStrength.rscp);
            //wcdma.signalStrength
            p->readInt32(&sig_strength.WCDMA_SignalStrength.signalStrength);
            //wcdma.bitErrorRate
            p->readInt32(&sig_strength.WCDMA_SignalStrength.bitErrorRate);
            //wcdma.rscp
            p->readInt32(&sig_strength.WCDMA_SignalStrength.rscp);
            //wcdma.ecno
            p->readInt32(&sig_strength.WCDMA_SignalStrength.ecno);
            //nr.ss_rsrp
            p->readInt32(&sig_strength.NR_SignalStrength.ssRsrp);
            //nr.ss_rsrq
            p->readInt32(&sig_strength.NR_SignalStrength.ssRsrq);
            //nr.ss_sinr
            p->readInt32(&sig_strength.NR_SignalStrength.ssSinr);
            //nr.csi_rsrp
            p->readInt32(&sig_strength.NR_SignalStrength.csiRsrp);
            //nr.csi_rsrq
            p->readInt32(&sig_strength.NR_SignalStrength.csiRsrq);
            //nr.csi_sinr
            p->readInt32(&sig_strength.NR_SignalStrength.csiSinr);
            RFX_LOG_D(LOG_TAG, "transferUnsolSignalStrength: gsm_signalStrength=%d, gsm_bitErrorRate=%d, gsm_timingAdvance=%d, \
                CDMA_dbm=%d, CDMA_ecio=%d, \
                EVDO_dbm=%d, EVDO_ecio=%d, EVDO_snr=%d, \
                LTE_signalStrength=%d, LTE_rsrp=%d, LTE_rsrq=%d, LTE_rssnr=%d, LTE_cqi=%d, LTE_timingAdvance=%d, \
                TD_SCDMA_signalStrength=%d, TD_SCDMA_bitErrorRate=%d, TD_SCDMA_rscp=%d \
                WCDMA_signalStrength=%d, WCDMA_bitErrorRate=%d, WCDMA_rscp=%d, WCDMA_ecno=%d\
                NR_SignalStrength.ssRsrp=%d, NR_SignalStrength.ssRsrq=%d, NR_SignalStrength.ssSinr=%d\
                NR_SignalStrength.csiRsrp=%d, NR_SignalStrength.csiRsrq=%d, NR_SignalStrength.csiSinr=%d",
                sig_strength.GW_SignalStrength.signalStrength,sig_strength.GW_SignalStrength.bitErrorRate,sig_strength.GW_SignalStrength.timingAdvance,
                sig_strength.CDMA_SignalStrength.dbm,sig_strength.CDMA_SignalStrength.ecio,sig_strength.EVDO_SignalStrength.dbm,
                sig_strength.EVDO_SignalStrength.ecio,sig_strength.EVDO_SignalStrength.signalNoiseRatio,sig_strength.LTE_SignalStrength.signalStrength,
                sig_strength.LTE_SignalStrength.rsrp, sig_strength.LTE_SignalStrength.rsrq,sig_strength.LTE_SignalStrength.rssnr,
                sig_strength.LTE_SignalStrength.cqi,sig_strength.LTE_SignalStrength.timingAdvance,
                sig_strength.TD_SCDMA_SignalStrength.signalStrength, sig_strength.TD_SCDMA_SignalStrength.bitErrorRate, sig_strength.TD_SCDMA_SignalStrength.rscp,
                sig_strength.WCDMA_SignalStrength.signalStrength, sig_strength.WCDMA_SignalStrength.bitErrorRate, sig_strength.WCDMA_SignalStrength.rscp, sig_strength.WCDMA_SignalStrength.ecno,
                sig_strength.NR_SignalStrength.ssRsrp, sig_strength.NR_SignalStrength.ssRsrq, sig_strength.NR_SignalStrength.ssSinr,
                sig_strength.NR_SignalStrength.csiRsrp, sig_strength.NR_SignalStrength.csiRsrq, sig_strength.NR_SignalStrength.csiSinr);
            newp->writeInt32(sig_strength.GW_SignalStrength.signalStrength);
            newp->writeInt32(sig_strength.GW_SignalStrength.bitErrorRate);
            newp->writeInt32(sig_strength.GW_SignalStrength.timingAdvance);
            newp->writeInt32(sig_strength.CDMA_SignalStrength.dbm);
            newp->writeInt32(sig_strength.CDMA_SignalStrength.ecio);
            newp->writeInt32(sig_strength.EVDO_SignalStrength.dbm);
            newp->writeInt32(sig_strength.EVDO_SignalStrength.ecio);
            newp->writeInt32(sig_strength.EVDO_SignalStrength.signalNoiseRatio);
            newp->writeInt32(sig_strength.LTE_SignalStrength.signalStrength);
            newp->writeInt32(sig_strength.LTE_SignalStrength.rsrp);
            newp->writeInt32(sig_strength.LTE_SignalStrength.rsrq);
            newp->writeInt32(sig_strength.LTE_SignalStrength.rssnr);
            newp->writeInt32(sig_strength.LTE_SignalStrength.cqi);
            newp->writeInt32(sig_strength.LTE_SignalStrength.timingAdvance);
            newp->writeInt32(sig_strength.TD_SCDMA_SignalStrength.signalStrength);
            newp->writeInt32(sig_strength.TD_SCDMA_SignalStrength.bitErrorRate);
            newp->writeInt32(sig_strength.TD_SCDMA_SignalStrength.rscp);
            newp->writeInt32(sig_strength.WCDMA_SignalStrength.signalStrength);
            newp->writeInt32(sig_strength.WCDMA_SignalStrength.bitErrorRate);
            newp->writeInt32(sig_strength.WCDMA_SignalStrength.rscp);
            newp->writeInt32(sig_strength.WCDMA_SignalStrength.ecno);
            newp->writeInt32(sig_strength.NR_SignalStrength.ssRsrp);
            newp->writeInt32(sig_strength.NR_SignalStrength.ssRsrq);
            newp->writeInt32(sig_strength.NR_SignalStrength.ssSinr);
            newp->writeInt32(sig_strength.NR_SignalStrength.csiRsrp);
            newp->writeInt32(sig_strength.NR_SignalStrength.csiRsrq);
            newp->writeInt32(sig_strength.NR_SignalStrength.csiSinr);
        }else {
            RFX_LOG_E(LOG_TAG, "transferUnsolSignalStrength: count=%d", count);
        }
    }
    return newp;
}

Parcel* tranferUnsolRilConnected(int version, int unsol, Parcel *p){
    Parcel *newp = new Parcel();
    RFX_LOG_D(LOG_TAG, "tranferUnsolRilConnected, unsol =%d", unsol);
    copyUrcHeader(newp, p);
    //-->todo, tranfer parcel
    newp->writeInt32(1);
    newp->writeInt32(RIL_VERSION);
    return newp;
}

Parcel* tranferUnsolCellInfoList(int version, int unsol, Parcel *p){
    Parcel *newp = new Parcel();
    RFX_LOG_D(LOG_TAG, "tranferUnsolCellInfoList, unsol =%d", unsol);
    copyUrcHeader(newp, p);

    TransferUtilsCellInfo(p,newp);
    return newp;
}

Parcel* tranferUnsolDataCallList(int version, int unsol, Parcel *p){
    Parcel *newp = new Parcel();
    RFX_LOG_D(LOG_TAG, "tranferUnsolDataCallList, unsol =%d", unsol);
    copyUrcHeader(newp, p);
    TransferUtilsDataCallList(p, newp);
    return newp;
}


