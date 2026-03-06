// SPDX-License-Identifier: MediaTekProprietary
/*
 * Common.h
 *
 *  Created on: Apr 21, 2017
 *      Author: mtk54458
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include "../include/telephony/ril.h"

using namespace std;



#define TEXT_APN_TYPE_ALL "*"
#define TEXT_APN_TYPE_DEFAULT "default"
#define TEXT_APN_TYPE_IMS "ims"
#define TEXT_APN_TYPE_MMS "mms"
#define TEXT_APN_TYPE_SUPL "supl"
#define TEXT_APN_TYPE_DUN "dun"
#define TEXT_APN_TYPE_HIPRI "hipri"
#define TEXT_APN_TYPE_FOTA "fota"
#define TEXT_APN_TYPE_CBS "cbs"
#define TEXT_APN_TYPE_EMERGENCY "emergency"
#define TEXT_APN_TYPE_IA "ia"
#define TEXT_APN_TYPE_DM "dm"
#define TEXT_APN_TYPE_WAP "wap"
#define TEXT_APN_TYPE_NET "net"
#define TEXT_APN_TYPE_CMMAIL "cmmail"
#define TEXT_APN_TYPE_TETHERING "tethering"
#define TEXT_APN_TYPE_RCSE "rcse"
#define TEXT_APN_TYPE_XCAP "xcap"
#define TEXT_APN_TYPE_RCS "rcs"
#define TEXT_APN_TYPE_BIP "bip"
#define TEXT_APN_TYPE_VISIM "vsim"

//for IOT card type
#define TEXT_IOT_APN_TYPE_DEFAULT "iot_default"
#define TEXT_IOT_APN_TYPE_NET_0 "iot_net_0"
#define TEXT_IOT_APN_TYPE_NET_1 "iot_net_1"
#define TEXT_IOT_APN_TYPE_NET_2 "iot_net_2"
#define TEXT_IOT_APN_TYPE_NET_3 "iot_net_3"
#define TEXT_IOT_APN_TYPE_NET_4 "iot_net_4"
#define TEXT_IOT_APN_TYPE_NET_5 "iot_net_5"
#define TEXT_IOT_APN_TYPE_NET_6 "iot_net_6"


#if defined(TELEPHONYWARE)||defined(TELE_FWK)
#define DATAASST_PDN_APN_TYPE_UNKNOWN       (0x00000000)
#define DATAASST_PDN_APN_TYPE_DEFAULT       (0x00000001)
#define DATAASST_PDN_APN_TYPE_MMS           (0x00000002)
#define DATAASST_PDN_APN_TYPE_SUPL          (0x00000004)
#define DATAASST_PDN_APN_TYPE_DUN           (0x00000008)
#define DATAASST_PDN_APN_TYPE_HIPRI         (0x00000010)
#define DATAASST_PDN_APN_TYPE_FOTA          (0x00000020)
#define DATAASST_PDN_APN_TYPE_IMS           (0x00000040)
#define DATAASST_PDN_APN_TYPE_CBS           (0x00000080)
#define DATAASST_PDN_APN_TYPE_IA            (0x00000100)
#define DATAASST_PDN_APN_TYPE_EMERGENCY     (0x00000200)
#define DATAASST_PDN_APN_TYPE_WAP           (0x00000400)
#define DATAASST_PDN_APN_TYPE_XCAP          (0x00000800)
#define DATAASST_PDN_APN_TYPE_VSIM          (0x00001000)
#define DATAASST_PDN_APN_TYPE_BIP           (0x00002000)
#define DATAASST_PDN_APN_TYPE_TETHERING     (0x00004000)
#define DATAASST_PDN_APN_TYPE_RCS           (0x00008000)


#else
#define DATAASST_PDN_APN_TYPE_UNKNOWN       (0x00000000)
#define DATAASST_PDN_APN_TYPE_DEFAULT       (0x00000001)
#define DATAASST_PDN_APN_TYPE_IMS           (0x00000002)
#define DATAASST_PDN_APN_TYPE_MMS           (0x00000004)
#define DATAASST_PDN_APN_TYPE_SUPL          (0x00000008)
#define DATAASST_PDN_APN_TYPE_DUN           (0x00000010)
#define DATAASST_PDN_APN_TYPE_HIPRI         (0x00000020)
#define DATAASST_PDN_APN_TYPE_FOTA          (0x00000040)
#define DATAASST_PDN_APN_TYPE_CBS           (0x00000080)
#define DATAASST_PDN_APN_TYPE_EMERGENCY     (0x00000100)
#define DATAASST_PDN_APN_TYPE_IA            (0x00000200)
#define DATAASST_PDN_APN_TYPE_DM            (0x00000400)
#define DATAASST_PDN_APN_TYPE_WAP           (0x00000800)
#define DATAASST_PDN_APN_TYPE_NET           (0x00001000)
#define DATAASST_PDN_APN_TYPE_CMMAIL        (0x00002000)
#define DATAASST_PDN_APN_TYPE_TETHERING     (0x00004000)
#define DATAASST_PDN_APN_TYPE_RCSE          (0x00008000)
#define DATAASST_PDN_APN_TYPE_XCAP          (0x00010000)
#define DATAASST_PDN_APN_TYPE_RCS           (0x00020000)
#endif


//for IOT
#define IOT_DATAASST_PDN_APN_TYPE_DEFAULT   (0x00100000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_0     (0x00200000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_1     (0x00400000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_2     (0x00800000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_3     (0x01000000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_4     (0x02000000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_5     (0x04000000)
#define IOT_DATAASST_PDN_APN_TYPE_NET_6     (0x08000000)



typedef enum {
    MTK_RIL_DATA_PROFILE_MMS        = RIL_DATA_PROFILE_OEM_BASE + 1,
    MTK_RIL_DATA_PROFILE_SUPL       = RIL_DATA_PROFILE_OEM_BASE + 2,
    MTK_RIL_DATA_PROFILE_HIPRI      = RIL_DATA_PROFILE_OEM_BASE + 3,
    MTK_RIL_DATA_PROFILE_EMERGENCY  = RIL_DATA_PROFILE_OEM_BASE + 4,
    MTK_RIL_DATA_PROFILE_XCAP       = RIL_DATA_PROFILE_OEM_BASE + 5,
    MTK_RIL_DATA_PROFILE_RCS        = RIL_DATA_PROFILE_OEM_BASE + 6,
    MTK_RIL_DATA_PROFILE_BIP        = RIL_DATA_PROFILE_OEM_BASE + 7,
    MTK_RIL_DATA_PROFILE_VSIM       = RIL_DATA_PROFILE_OEM_BASE + 8,
    MTK_RIL_DATA_PROFILE_MCX        = RIL_DATA_PROFILE_OEM_BASE + 9
} MTK_RIL_DataProfile;



typedef struct  {
    char *type;
    unsigned int  typebitmask;
    unsigned int  mtk_profile_id;
}ApnType_2_Bitmask_Id_T;

const ApnType_2_Bitmask_Id_T  apntype_2_bitmask_id[] = {
            {TEXT_APN_TYPE_DEFAULT,     DATAASST_PDN_APN_TYPE_DEFAULT,      RIL_DATA_PROFILE_DEFAULT},
            {TEXT_APN_TYPE_TETHERING,   DATAASST_PDN_APN_TYPE_TETHERING,    RIL_DATA_PROFILE_TETHERED},
            {TEXT_APN_TYPE_IMS,         DATAASST_PDN_APN_TYPE_IMS,          RIL_DATA_PROFILE_IMS},
            {TEXT_APN_TYPE_DUN,         DATAASST_PDN_APN_TYPE_DUN,          RIL_DATA_PROFILE_INVALID},
            {TEXT_APN_TYPE_FOTA,        DATAASST_PDN_APN_TYPE_FOTA,         RIL_DATA_PROFILE_FOTA},
            {TEXT_APN_TYPE_CBS,         DATAASST_PDN_APN_TYPE_CBS,          RIL_DATA_PROFILE_CBS},
            {TEXT_APN_TYPE_IA,          DATAASST_PDN_APN_TYPE_IA,           RIL_DATA_PROFILE_INVALID},
#if !(defined(TELEPHONYWARE)||defined(TELE_FWK))
            {TEXT_APN_TYPE_DM,          DATAASST_PDN_APN_TYPE_DM,           RIL_DATA_PROFILE_INVALID},
#endif

            {TEXT_APN_TYPE_MMS,         DATAASST_PDN_APN_TYPE_MMS,          MTK_RIL_DATA_PROFILE_MMS},
            {TEXT_APN_TYPE_SUPL,        DATAASST_PDN_APN_TYPE_SUPL,         MTK_RIL_DATA_PROFILE_SUPL},
            {TEXT_APN_TYPE_HIPRI,       DATAASST_PDN_APN_TYPE_HIPRI,        MTK_RIL_DATA_PROFILE_HIPRI},
            // {TEXT_APN_TYPE_WAP,         DATAASST_PDN_APN_TYPE_WAP,          MTK_RIL_DATA_PROFILE_WAP},
            {TEXT_APN_TYPE_EMERGENCY,   DATAASST_PDN_APN_TYPE_EMERGENCY,    MTK_RIL_DATA_PROFILE_EMERGENCY},
#if !(defined(TELEPHONYWARE)||defined(TELE_FWK))
            {TEXT_APN_TYPE_NET,         DATAASST_PDN_APN_TYPE_NET,          RIL_DATA_PROFILE_INVALID},
            {TEXT_APN_TYPE_CMMAIL,      DATAASST_PDN_APN_TYPE_CMMAIL,       RIL_DATA_PROFILE_INVALID},
            {TEXT_APN_TYPE_RCSE,        DATAASST_PDN_APN_TYPE_RCSE,         RIL_DATA_PROFILE_INVALID},
#endif
            {TEXT_APN_TYPE_XCAP,        DATAASST_PDN_APN_TYPE_XCAP,         MTK_RIL_DATA_PROFILE_XCAP},
            {TEXT_APN_TYPE_RCS,         DATAASST_PDN_APN_TYPE_RCS,          MTK_RIL_DATA_PROFILE_RCS},
            {TEXT_APN_TYPE_BIP,         DATAASST_PDN_APN_TYPE_BIP,          MTK_RIL_DATA_PROFILE_BIP},
            {TEXT_APN_TYPE_VISIM,       DATAASST_PDN_APN_TYPE_VSIM,         MTK_RIL_DATA_PROFILE_VSIM},
            {TEXT_IOT_APN_TYPE_DEFAULT, IOT_DATAASST_PDN_APN_TYPE_DEFAULT,  RIL_DATA_PROFILE_DEFAULT},
            {TEXT_IOT_APN_TYPE_NET_0 ,  IOT_DATAASST_PDN_APN_TYPE_NET_0,    RIL_DATA_PROFILE_DEFAULT},
            {TEXT_IOT_APN_TYPE_NET_1 ,  IOT_DATAASST_PDN_APN_TYPE_NET_1,    RIL_DATA_PROFILE_DEFAULT},
            {TEXT_IOT_APN_TYPE_NET_2 ,  IOT_DATAASST_PDN_APN_TYPE_NET_2,    RIL_DATA_PROFILE_DEFAULT},
            {TEXT_IOT_APN_TYPE_NET_3 ,  IOT_DATAASST_PDN_APN_TYPE_NET_3,    RIL_DATA_PROFILE_DEFAULT},
            {TEXT_IOT_APN_TYPE_NET_4 ,  IOT_DATAASST_PDN_APN_TYPE_NET_4,    RIL_DATA_PROFILE_DEFAULT},
            {TEXT_IOT_APN_TYPE_NET_5 ,  IOT_DATAASST_PDN_APN_TYPE_NET_5,    RIL_DATA_PROFILE_DEFAULT},
            {TEXT_IOT_APN_TYPE_NET_6 ,  IOT_DATAASST_PDN_APN_TYPE_NET_6,    RIL_DATA_PROFILE_DEFAULT},
    };

#define HIGH_THROUGHPUT_APN_NUM  5
const string HIGH_THROUGHPUT_APN[HIGH_THROUGHPUT_APN_NUM] = {
        TEXT_APN_TYPE_TETHERING,
        TEXT_APN_TYPE_DEFAULT,
        TEXT_APN_TYPE_DUN,
        TEXT_APN_TYPE_HIPRI,
        TEXT_IOT_APN_TYPE_DEFAULT};

#define SUPPORTED_APN_NUM  14
const string SUPPORTED_APN[SUPPORTED_APN_NUM] = {
        TEXT_APN_TYPE_DEFAULT,
        TEXT_APN_TYPE_SUPL,
        TEXT_APN_TYPE_WAP,
        TEXT_APN_TYPE_RCS,
        TEXT_APN_TYPE_BIP,
        TEXT_APN_TYPE_MMS,
        TEXT_IOT_APN_TYPE_DEFAULT,
        TEXT_IOT_APN_TYPE_NET_0,
        TEXT_IOT_APN_TYPE_NET_1,
        TEXT_IOT_APN_TYPE_NET_2,
        TEXT_IOT_APN_TYPE_NET_3,
        TEXT_IOT_APN_TYPE_NET_4,
        TEXT_IOT_APN_TYPE_NET_5,
        TEXT_IOT_APN_TYPE_NET_6};

//ApnContext type
typedef enum ApnContextType {
    TYPE_MOBILE = 0,
    TYPE_WIFI   = 1,
    TYPE_MOBILE_MMS = 2,
    TYPE_MOBILE_SUPL = 3,
    TYPE_MOBILE_DUN = 4,
    TYPE_MOBILE_HIPRI = 5,
    TYPE_WIMAX = 6,
    TYPE_BLUETOOTH = 7,
    TYPE_DUMMY = 8,
    TYPE_ETHERNET = 9,
    TYPE_MOBILE_FOTA = 10,
    TYPE_MOBILE_IMS = 11,
    TYPE_MOBILE_CBS = 12,
    TYPE_MOBILE_IA = 14,
    TYPE_MOBILE_DM = 34,
    TYPE_MOBILE_WAP = 35,
    TYPE_MOBILE_NET = 36,
    TYPE_MOBILE_CMMAIL = 37,
    TYPE_MOBILE_EMERGENCY = 15,
    TYPE_MOBILE_RCSE = 38,
    TYPE_MOBILE_XCAP = 40,
    TYPE_MOBILE_RCS = 41,
    TYPE_MOBILE_BIP = 42,
    TYPE_IOT_DEFAULT = 43, //IOT
    TYPE_IOT_NET_0 = 44, //IOT
    TYPE_IOT_NET_1 = 45, //IOT
    TYPE_IOT_NET_2 = 46, //IOT
    TYPE_IOT_NET_3 = 47, //IOT
    TYPE_IOT_NET_4 = 48, //IOT
    TYPE_IOT_NET_5 = 49, //IOT
    TYPE_IOT_NET_6 = 50, //IOT
} ApnContextType;
// Apn related
const int APN_CONTEXT_NUMBER = 28;
const string NETWORK_CONFIG[APN_CONTEXT_NUMBER] = {
        "wifi,1,1,2,-1,true",
        "mobile,0,0,0,-1,true",
        "mobile_mms,2,0,2,300000,true",
        "mobile_supl,3,0,2,300000,true",
        "mobile_dun,4,0,3,300000,true",
        "mobile_hipri,5,0,3,300000,true",
        "bluetooth,7,7,0,-1,true",
        "mobile_fota,10,0,2,300000,true",
        "mobile_ims,11,0,-1,-1,true",
        "mobile_cbs,12,0,2,300000,true",
        "mobile_dm,34,0,3,300000,true",
        "mobile_wap,35,0,3,300000,true",
        "mobile_net,36,0,3,300000,true",
        "mobile_cmmail,37,0,3,300000,true",
        "mobile_rcse,38,0,3,300000,true",
        "mobile_ia,14,0,2,-1,true",
        "mobile_emergency,15,0,2,-1,true",
        "mobile_xcap,40,0,3,300000,true",
        "mobile_rcs,41,0,3,300000,true",
        "mobile_bip,42,0,3,300000,true",
        "iot_default,43,0,0,-1,true", //for IOT
        "iot_net_0,44,0,0,-1,true", //for IOT
        "iot_net_1,45,0,0,-1,true", //for IOT
        "iot_net_2,46,0,0,-1,true", //for IOT
        "iot_net_3,47,0,0,-1,true", //for IOT
        "iot_net_4,48,0,0,-1,true", //for IOT
        "iot_net_5,49,0,0,-1,true", //for IOT
        "iot_net_6,50,0,0,-1,true"}; //for IOT


//Fail cause related
extern const string NOT_ATTACHED;
extern const string APN_NOT_READY;
extern const string ROAMING_DISABLED;
extern const string INVALID_PHONE_STATE;
extern const string CONCURRENT_VOICE_DATA_NOT_ALLOWED;
extern const string PS_RESTRICTED;
extern const string UNDESIRED_POWER_STATE;
extern const string REASON_DATA_ATTACHED;
extern const string REASON_CALL_END;
extern const string REASON_APN_READY;
extern const string REASON_PS_RESTRICT;

//RetryManager related
extern const string DEFAULT_DATA_RETRY_CONFIG ;
extern const string OTHERS_DATA_RETRY_CONFIG;
extern const long DEFAULT_INTER_APN_DELAY;
extern const long NO_RETRY;
extern const long NO_SUGGESTED_RETRY_DELAY;
extern const int MAX_SAME_APN_RETRY;

//DataConnection related
extern const int DC_INVALID_ID;
extern const int MIN_ID_HIGH_TROUGHPUT;
extern const int MAX_ID_HIGH_TROUGHPUT;
extern const int MIN_ID_OTHERS_TROUGHPUT;
extern const int MAX_ID_OTHERS_TROUGHPUT;

//DataCallResponse related
extern const int INVALID_NET_ID;
extern const int INVALID_CID;
extern const int INVALID_MTU;



#endif /* COMMON_H_ */
