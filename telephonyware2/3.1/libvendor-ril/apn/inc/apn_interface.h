// SPDX-License-Identifier: MediaTekProprietary
/*
 * apn_interface.h
 *
 *  Created on: Nov 3, 2016
 *      Author: mtk54498
 */

#ifndef INC_APN_INTERFACE_H_
#define INC_APN_INTERFACE_H_

// APN TYPE @{
#define APN_TYPE_ALL "*"
#define APN_TYPE_DEFAULT "default"
#define APN_TYPE_MMS "mms"
#define APN_TYPE_SUPL "supl"
#define APN_TYPE_DUN "dun"
#define APN_TYPE_HIPRI "hipri"
#define APN_TYPE_FOTA "fota"
#define APN_TYPE_IMS "ims"
#define APN_TYPE_CBS "cbs"
#define APN_TYPE_IA "ia"
#define APN_TYPE_EMERGENCY "emergency"
// @}

typedef enum {
    PROP_TEXT, PROP_INTEGER, PROP_BOOLEAN
} PROP_VALUE_TYPE;

typedef struct {
    const char* prop_name;
    PROP_VALUE_TYPE type;
} prop_attr_t;

/** apn collumn type index */
typedef enum {
    APN_PROP_iId,
    APN_PROP_iTYPE,
    APN_PROP_iMCC,
    APN_PROP_iMNC,
    APN_PROP_iCARRIER,
    APN_PROP_iAPN,
    APN_PROP_iPORT,
    APN_PROP_iPROXY,
    APN_PROP_iMMSC,
    APN_PROP_iMMSPROXY,
    APN_PROP_iMMSPORT,
    APN_PROP_iUSER,
    APN_PROP_iPASSWORD,
    APN_PROP_iAUTH_TYPE,
    APN_PROP_iSERVER,
    APN_PROP_iPROTOCOL,
    APN_PROP_iROAMING_PROTOCOL,
    APN_PROP_iMTU,
    APN_PROP_iCARRIER_ENABLED,
    APN_PROP_iBEARER,
    APN_PROP_iBEARER_BITMASK,
    APN_PROP_iPROFILE_ID,
    APN_PROP_iMODEM_COGNITIVE,
    APN_PROP_iMAXCONNS,
    APN_PROP_iWAITTIME,
    APN_PROP_iMAXCONNSTIME,
    APN_PROP_iMVNO_TYPE,
    APN_PROP_iUSER_VISIBLE,
    APN_PROP_iCURRENT,
    APN_PROP_iTOTAL
} APN_PROP_ENUM;

extern prop_attr_t APN_PROP_ATTR[APN_PROP_iTOTAL];
extern int APN_DEBUG;
/** APN prop name @{*/
#define APN_PROP_ID "_id"
#define APN_PROP_TYPE "type"
#define APN_PROP_MCC "mcc"
#define APN_PROP_MNC "mnc"
#define APN_PROP_CARRIER "carrier"
#define APN_PROP_APN "apn"
#define APN_PROP_PORT "port"
#define APN_PROP_PROXY "proxy"
#define APN_PROP_MMSC "mmsc"
#define APN_PROP_MMSPROXY "mmsproxy"
#define APN_PROP_MMSPORT "mmsport"
#define APN_PROP_USER "user"
#define APN_PROP_PASSWORD "password"
#define APN_PROP_AUTH_TYPE "authtype"
#define APN_PROP_SERVER "server"
#define APN_PROP_PROTOCOL "protocol"
#define APN_PROP_ROAMING_PROTOCOL "roaming_protocol"
#define APN_PROP_MTU "mtu"
#define APN_PROP_CARRIER_ENABLED "carrier_enabled"
#define APN_PROP_BEARER "bearer"
#define APN_PROP_BEARER_BITMASK "bearer_bitmask"
#define APN_PROP_PROFILE_ID "profile_id"
#define APN_PROP_MODEM_COGNITIVE "modem_cognitive"
#define APN_PROP_MAXCONNS "maxConns"
#define APN_PROP_WAITTIME "waitTime"
#define APN_PROP_MAXCONNSTIME "maxConnsTime"
#define APN_PROP_MVNO_TYPE "mvno_type"
#define APN_PROP_USER_VISIBLE "user_visible"
#define APN_PROP_CURRENT "current"
/** @} */

// APN result code
typedef enum {
    APN_ERR = -1,
    APN_OK = 0,
    APN_DB_ERROR,
    APN_FILE_NOEXIST,
    APN_PARSE_FAILED
} apnErrorCode;

typedef enum {
    CMD_INSERT,
    CMD_DELETE,
    CMD_QUERY,
    CMD_UPDATE
} apnSqlCmd;

typedef enum {
    APN_READY,
    APN_NOREADY,
    APN_DESTROYED
} apnNotifyCode;

typedef struct apn_record_ {
    int count;
    char* values[APN_PROP_iTOTAL];
    int columnIdx[APN_PROP_iTOTAL];
    struct apn_record_* next;
} apn_record_t;

typedef struct {
    int count;
    apn_record_t* top;
} apn_list_t;

typedef enum {
    DATA_PROFILE_DEFAULT   = 0,
    DATA_PROFILE_TETHERED  = 1,
    DATA_PROFILE_IMS       = 2,
    DATA_PROFILE_FOTA      = 3,
    DATA_PROFILE_CBS       = 4,
    DATA_PROFILE_OEM_BASE  = 1000,
    DATA_PROFILE_MMS        = DATA_PROFILE_OEM_BASE + 1,
    DATA_PROFILE_SUPL       = DATA_PROFILE_OEM_BASE + 2,
    DATA_PROFILE_HIPRI      = DATA_PROFILE_OEM_BASE + 3,
    DATA_PROFILE_WAP        = DATA_PROFILE_OEM_BASE + 4,
    DATA_PROFILE_EMERGENCY  = DATA_PROFILE_OEM_BASE + 5,
    DATA_PROFILE_XCAP       = DATA_PROFILE_OEM_BASE + 6,
    DATA_PROFILE_RCS        = DATA_PROFILE_OEM_BASE + 7,
    DATA_PROFILE_BIP        = DATA_PROFILE_OEM_BASE + 8,
    DATA_PROFILE_VSIM       = DATA_PROFILE_OEM_BASE + 9,
    DATA_PROFILE_INVALID   = 0xFFFFFFFF
} default_profile_id;

#ifdef __cplusplus
extern "C" {
#endif

typedef void* apn_ctx;
typedef void (*apn_callback)(int resultCode, void* data);

int apn_init(apn_callback);

int apn_destroy();
/** need free record returned by apn_free_record */
apn_record_t* apn_malloc_record();

int apn_build_cRecord(apn_record_t* record, const char* prop, const char* value);
int apn_build_iRecord(apn_record_t* record, int col, const char* value);

/** get prop attr from APN_PROP_ATTR, no need to free */
const prop_attr_t* apn_get_attr_by_name(const char* name);
const prop_attr_t* apn_get_prop_attr_by_idx(int idx);

/*
 *  get prop name from APN_PROP_ATTR, no need to free
*/
const char* apn_get_prop_name(int idx);

int apn_get_idx_by_name(const char* name);
int apn_get_prop_idx(prop_attr_t* attr);
int apn_query_db(const apn_record_t* record, apn_list_t** list);
int apn_update_record_db(const apn_record_t * record);
int apn_delete_record_db(int id);
int apn_delete_db(const apn_record_t *record);
int apn_insert_record_db(const apn_record_t * record);
int apn_cmd_record_db(apnSqlCmd cmd, const apn_record_t* record);

/** if db doesn't offer profile id, use default id */
int apn_get_default_profile_id(const char* apnType);
/** remember to free record malloced */
void apn_free_record(apn_record_t* record);
/** remember to free apn list */
void apn_free_list(apn_list_t* record);

const char* apn_get_prop_from_record(const apn_record_t* record, int idx);
void apn_dump_record(const apn_record_t* record);
int apn_get_prop_value_type(int idx);


#ifdef __cplusplus
}
#endif

#endif /* INC_APN_INTERFACE_H_ */
