/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_sim_common.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_sim_common相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_SIM_COMMON_H__
#define __SV_SIM_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SV_SIM_IMSI_LENGTH      16      /**<  Maximum length of IMSI data. */
#define SV_SIM_ICCID_LENGTH     20      /**<  Maximum length of ICCID data. */
#define SV_SIM_NUM_OPERATOR_MAX 24      /**<  Maximum number of operators */
#define SV_SIM_PATH_MAX         20      /**<  Maximum length of a full file path in ASCII format. */
#define SV_SIM_DATA_MAX         4096    /**<  Maximum size of data to be read/written. */
#define SV_SIM_PIN_MAX          8       /**<  Maximum length of PIN data. */
#define SV_SIM_MAX_NUM_CARDS    2       /**<  Maximum number of cards. */
#define SV_SIM_PHONE_NUMBER_MAX 82      /**<  Maximum phone number length. */
#define SV_SIM_MCC_LENGHT       3       /**<  Length of the MCC. */
#define SV_SIM_MNC_MAX          3       /**<  Maximum length of the MNC. */

#define SV_SIM_MAX_REFRESH_FILES    35   /**  Maximum number of refresh files. */
#define SV_SIM_CHAR_PATH_MAX        20   /**  Maximum length of a full file path in ASCII format. */

#define SV_SIM_PHONE_BOOK_NAME_MAX      15  /**<  Maximum length of phone book user name. */
#define SV_SIM_PHONE_BOOK_NUMBER_MAX    21  /**<  Maximum length of phone book number. */

#define SV_SIM_APDU_DATA_MAX    1024  /**<  Maximum number of APDU data. */

#define SV_SIM_PB_DEFAULT_PATH   "3F007F106F3A"   /**< Default phone book path. */
#define SV_SIM_EID_LENGTH       32      /**<  Maximum length of EID data. */

/** Ligcial slot */
/**> Currently only one active slot is supported, so slot should be always SV_SIM_SLOT_1.*/
typedef enum 
{
    SV_SIM_SLOT_INVALID = 0x000, /**< Invalid slot.  */   
    SV_SIM_SLOT_1       = 0xB01, /**< Identify card in  slot 1.  */
    SV_SIM_SLOT_2       = 0xB02, /**< Identify card in slot 2.  */
} SV_SIM_SLOT_E;

/**Enum Physical slot */
typedef enum 
{
    SV_SIM_PHY_SLOT_INVALID = 0x000, /**< Invalid slot.  */   
    SV_SIM_PHY_SLOT_1       = 0xB01, /**< Identify card in slot 1.  */
    SV_SIM_PHY_SLOT_2       = 0xB02, /**< Identify card in slot 2.  */
} SV_SIM_PHY_SLOT_E;

/**Enum SIM PIN */
typedef enum 
{
    SV_SIM_PIN_1 = 0xB01, /**< Level 1 user verification.  */
    SV_SIM_PIN_2 = 0xB02, /**< Level 2 user verification.  */
} SV_SIM_PIN_E;

/** Enum Application Type */
typedef enum
{
    SV_SIM_APP_TYPE_UNKNOWN = 0xB00, /**<  Unknown application type  */
    SV_SIM_APP_TYPE_3GPP    = 0xB01, /**< Identify the SIM/USIM application on the card.  */
    SV_SIM_APP_TYPE_3GPP2   = 0xB02, /**< Identify the RUIM/CSIM application on the card.  */
    SV_SIM_APP_TYPE_ISIM    = 0xB03, /**< Identify the ISIM application on the card.  */
} SV_SIM_APP_TYPE_E;

/** Enum Sim operator */
typedef struct 
{
    char mcc[SV_SIM_MCC_LENGHT];    /**< MCC value in ASCII characters.*/
    uint8_t mnc_len;                /**< The number of elements in the MNC, 2 or 3. */
    char mnc[SV_SIM_MNC_MAX];       /**< MNC value in ASCII characters.*/
} sv_sim_operator_t;

/** Multi Sim operator structure*/
typedef struct 
{
    int len;    /**< Length of actual operators */
    sv_sim_operator_t operators[SV_SIM_NUM_OPERATOR_MAX];
} sv_sim_operator_list_t;

/** Enum Sim Card state */
typedef enum {
    /** Card state unknown. */
    SV_SIM_CARD_STATE_UNKNOWN                       = 0xB01,
    /** Card is absent. */
    SV_SIM_CARD_STATE_ABSENT                        = 0xB02,  
    /** Card is present. */
    SV_SIM_CARD_STATE_PRESENT                       = 0xB03,
    /** Unknown error state. */
    SV_SIM_CARD_STATE_ERROR_UNKNOWN                 = 0xB04,
    /** Power down. */
    SV_SIM_CARD_STATE_ERROR_POWER_DOWN              = 0xB05,
    /** Poll error. */
    SV_SIM_CARD_STATE_ERROR_POLL_ERROR              = 0xB06,  
    /**  Failed to receive an answer to reset.  */
    SV_SIM_CARD_STATE_ERROR_NO_ATR_RECEIVED         = 0xB07, 
    /** Voltage mismatch. */
    SV_SIM_CARD_STATE_ERROR_VOLT_MISMATCH           = 0xB08, 
    /** Parity error. */
    SV_SIM_CARD_STATE_ERROR_PARITY_ERROR            = 0xB09,    
    /** Card returned technical problems. */
    SV_SIM_CARD_STATE_ERROR_SIM_TECHNICAL_PROBLEMS  = 0xB0A,  
} SV_SIM_CARD_STATE_E;  /**< Card state. */

/** Enum Sim Subscription */
typedef enum {
    SV_SIM_SUBSCRIPTION_NONE    = 0xB00, /**<  Nonprovisioning.  */
    SV_SIM_SUBSCRIPTION_PRI     = 0xB01, /**<  Primary provisioning subscription.  */
    SV_SIM_SUBSCRIPTION_SEC     = 0xB02, /**<  Secondary provisioning subscription.  */
} SV_SIM_SUBSCRIPTION_E;

/** Enum Sim type */
typedef enum
{
    SV_SIM_CARD_TYPE_UNKNOWN    = 0xB00, /**<  Unidentified card type.  */
    SV_SIM_CARD_TYPE_ICC        = 0xB01, /**<  Card of SIM or RUIM type.  */
    SV_SIM_CARD_TYPE_UICC       = 0xB02, /**<  Card of USIM or CSIM type.  */
} SV_SIM_CARD_TYPE_E;

/** Enum Sim app state */
typedef enum {
    SV_SIM_APP_STATE_UNKNOWN                    = 0xB00, /**<  Application state unknown. */
    SV_SIM_APP_STATE_DETECTED                   = 0xB01, /**<  Detected state.  */
    SV_SIM_APP_STATE_PIN1_REQ                   = 0xB02, /**<  PIN1 required.  */
    SV_SIM_APP_STATE_PUK1_REQ                   = 0xB03, /**<  PUK1 required.  */
    SV_SIM_APP_STATE_INITALIZATING              = 0xB04, /**<  Initializing.  */
    SV_SIM_APP_STATE_PERSO_CK_REQ               = 0xB05, /**<  Personalization control key required.  */
    SV_SIM_APP_STATE_PERSO_PUK_REQ              = 0xB06, /**<  Personalization unblock key required.  */
    SV_SIM_APP_STATE_PERSO_PERMANENTLY_BLOCKED  = 0xB07, /**<  Personalization is permanently blocked.  */
    SV_SIM_APP_STATE_PIN1_PERM_BLOCKED          = 0xB08, /**<  PIN1 is permanently blocked.  */
    SV_SIM_APP_STATE_ILLEGAL                    = 0xB09, /**<  Illegal application state.  */
    SV_SIM_APP_STATE_READY                      = 0xB0A, /**<  Application ready state. */
} SV_SIM_APP_STATE_E;

/** Enum Personalization function */
typedef enum {
    /**  Unknown personalization feature.  */
    SV_SIM_PERSO_FEATURE_UNKNOWN                = 0xB00,
    /**  Featurization based on 3GPP MCC and MNC.  */
    SV_SIM_PERSO_FEATURE_3GPP_NETWORK           = 0xB01, 
    /**  Featurization based on 3GPP MCC, MNC, and IMSI digits 6 and 7.  */
    SV_SIM_PERSO_FEATURE_3GPP_NETWORK_SUBSET    = 0xB02, 
    /**  Featurization based on 3GPP MCC, MNC, and GID1.  */
    SV_SIM_PERSO_FEATURE_3GPP_SERVICE_PROVIDER  = 0xB03,
    /**  Featurization based on 3GPP MCC, MNC, GID1, and GID2.  */
    SV_SIM_PERSO_FEATURE_3GPP_CORPORATE         = 0xB04, 
    /**  Featurization based on the 3GPP IMSI.  */
    SV_SIM_PERSO_FEATURE_3GPP_SIM               = 0xB05, 
    /**  Featurization based on 3GPP2 MCC and MNC.  */
    SV_SIM_PERSO_FEATURE_3GPP2_NETWORK_TYPE_1   = 0xB06, 
    /**  Featurization based on 3GPP2 IRM code.  */
    SV_SIM_PERSO_FEATURE_3GPP2_NETWORK_TYPE_2   = 0xB07, 
    /**  Featurization based on 3GPP2 IMSI_M.  */
    SV_SIM_PERSO_FEATURE_3GPP2_RUIM             = 0xB08, 
} SV_SIM_PERSO_FEATURE_E;

/** Enum The PIN state  */
typedef enum {
    /** Unknown PIN state. */
    SV_SIM_PIN_STATE_UNKNOWN                = 0xB01, 
    /**  PIN required, but has not been verified.  */
    SV_SIM_PIN_STATE_ENABLED_NOT_VERIFIED   = 0xB02, 
    /**  PIN required and has been verified.  */
    SV_SIM_PIN_STATE_ENABLED_VERIFIED       = 0xB03, 
    /**  PIN not required.  */
    SV_SIM_PIN_STATE_DISABLED               = 0xB04, 
    /**  PIN verification has failed too many times and is blocked. Recoverable through PUK verification.  */
    SV_SIM_PIN_STATE_BLOCKED                = 0xB05, 
    /**  PUK verification has failed too many times and is not recoverable.  */
    SV_SIM_PIN_STATE_PERMANENTLY_BLOCKED    = 0xB06, 
 } SV_SIM_PIN_STATE_E;

/** Sim app info structure  */
typedef struct {
    /**   Type of subscription (i.e., primary, secondary, etc.). */
    SV_SIM_SUBSCRIPTION_E subscription;
    /**   Current state of the application. */
    SV_SIM_APP_STATE_E app_state;
    /**   Current personalization state and feature enabled. */
    SV_SIM_PERSO_FEATURE_E perso_feature;
    /**   Number of personalization retries. */
    uint8_t perso_retries;
    /**   Number of personalization unblock retries. */
    uint8_t perso_unblock_retries;
    /**   Current PIN 1 state. */
    SV_SIM_PIN_STATE_E pin1_state;
    /**   Number of PIN 1 retries. */
    uint8_t pin1_num_retries;
    /**   Number of PUK 1 retries. */
    uint8_t puk1_num_retries;
    /**   Current PIN 2 state. */
    SV_SIM_PIN_STATE_E pin2_state;
    /**   Number of PIN 2 retries. */
    uint8_t pin2_num_retries; 
    /**   Number of PUK 2 retries. */
    uint8_t puk2_num_retries;
}sv_sim_app_info_t;  /* Type */

/** Sim card info structure  */
typedef struct
{
    SV_SIM_CARD_STATE_E state;      /**<   Sim Card State. */
    SV_SIM_CARD_TYPE_E type;        /**<   Sim Card Type. */
    sv_sim_app_info_t app_3gpp;     /**<   Stores 3GPP application information. */
    sv_sim_app_info_t app_3gpp2;    /**<   Stores 3GPP2 application information. */
    sv_sim_app_info_t app_isim;     /**<   Stores ISIM application information. */
} sv_sim_card_info_t;

typedef enum {
    SV_SIM_REFRESH_RESET              = 0xB01,  /**< Refresh reset. */
    SV_SIM_REFRESH_NAA_INIT           = 0xB02,  /**< Refresh NAA initialization. */
    SV_SIM_REFRESH_NAA_FCN            = 0xB03,  /**< Refresh NAA file change notification. */
    SV_SIM_REFRESH_NAA_INIT_FCN       = 0xB04,  /**< Refresh NAA initalization and file change notification. */
    SV_SIM_REFRESH_NAA_INIT_FULL_FCN  = 0xB05,  /**< Refresh NAA initalization and full file change notification. */
    SV_SIM_REFRESH_NAA_APP_RESET      = 0xB06,  /**< Refresh NAA application reset. */
    SV_SIM_REFRESH_3G_SESSION_RESET   = 0xB07,  /**< Refresh 3G session reset. */
}SV_SIM_REFRESH_MODE_E;  /**< SIM refresh modes. */

typedef struct {

    uint32_t path_value_len;                /**< Must be set to the number of elements in path_value. */
    char path_value[SV_SIM_CHAR_PATH_MAX];  /**< Path value. */
}sv_sim_refresh_file_list_t;  /* Type */

typedef struct
{
    /**< Indicates the slot to be used. Valid values: \n
      - 1 -- Slot 1
      - 2 -- Slot 2 @tablebulletend
    */
    SV_SIM_SLOT_E slot_id;
    /**< Indicates the type of the application. Valid values: \n
      - 0 -- Unknown
      - 1 -- 3GPP application
      - 2 -- 3GPP2 application
      - 3 -- ISIM application
      Other values are reserved for the future and are to be handled as Unknown.
    */
    SV_SIM_APP_TYPE_E app_type;
    /*  Refresh Mode */
    SV_SIM_REFRESH_MODE_E refresh_mode;
    
    /*  Refresh File Data */
    uint32_t refresh_files_len;  /**< Must be set to the number of elements in refresh_files. */
    sv_sim_refresh_file_list_t refresh_files[SV_SIM_MAX_REFRESH_FILES];    /**< Refresh file data. */
} sv_sim_refresh_info_t;

/** Sim file structure  */
typedef struct {
    uint32_t path_len;          /**< Must be set to the number of elements in the path. */
    char path[SV_SIM_PATH_MAX]; /**< File path in ASCII characters. */
    /** Offset is only required for write transparent file access where data length is indicated.*/
    uint16_t offset; 
    /** Index of records involved in file access. A record index of 0 indicates transparent file access.*/
    uint8_t record_idx;

    /* == filled by caller when writing file == */
    uint32_t data_len;  /**< When reading file, it's the length of file data been read.
                             When writing file, it's the length of file data to be written. */
    uint8_t data[SV_SIM_DATA_MAX];
} sv_sim_file_t;  /* Type */

/** Sim File type */
typedef enum {
    SV_SIM_FILE_TYPE_UNKNOWN        = 0xB00, /**<  Unknown file type  */
    SV_SIM_FILE_TYPE_TRANSPARENT    = 0xB01, /**< File structure consisting of a sequence of bytes.  */
    SV_SIM_FILE_TYPE_CYCLIC         = 0xB02, /**< File structure consisting of a sequence of records,
                                                  each containing the same fixed size in 
                                                  chronological order.Once all the records have been
                                                  used, the oldest data is overwritten.  */
    SV_SIM_FILE_TYPE_LINEAR_FIXED   = 0xB03, /**< File structure consisting of a sequence of records, 
                                                  each containing the same fixed size.  */
} SV_SIM_FILE_TYPE_E;

/** Sim file info structure  */
typedef struct {
    /* == filled by caller == */
    uint32_t path_len;          /**< Must be set to the number of elements in the path. */
    char path[SV_SIM_PATH_MAX]; /**< File path in ASCII characters. */

    /* == filled by callee == */
    SV_SIM_FILE_TYPE_E file_type;   /**<   File type */
    uint16_t file_size;             /**<   Size of transparent files.*/
    uint16_t record_size;           /**<   Size of each cyclic or linear fixed file record.*/
    uint16_t record_count;          /**<   Number of cyclic or linear fixed file records.*/
} sv_sim_file_info_t;  /* Type */

/** Sim phone book info structure  */
typedef struct 
{
    /** User name. Null-terminated */
    char name[SV_SIM_PHONE_BOOK_NAME_MAX];   
    /** Phone number, a '+' can also be preappended to the number. Null-terminated. */
    char number[SV_SIM_PHONE_BOOK_NUMBER_MAX]; 
} sv_sim_phone_book_record_t;

/** Sim APDU data structure  */
typedef struct
{
    uint32_t req_apdu_len;                      /**< Request APDU data length. */
    uint8_t req_apdu[SV_SIM_APDU_DATA_MAX];     /**< Request APDU data. */

    uint32_t resp_apdu_len;                     /**< Response APDU data length. */
    uint8_t resp_apdu[SV_SIM_APDU_DATA_MAX];    /**< Response APDU data. */
} sv_sim_apdu_t;

/** Indicates the slot number of the active SIM card structure */
typedef struct 
{
    int active_slots_len;  /**< Number of physical card slots */
    SV_SIM_PHY_SLOT_E active_slots[SV_SIM_MAX_NUM_CARDS]; /**< Id of the active SIM card slot */
} sv_sim_active_slots_t;

/**
 * sim service error callback function
 * @param[in] error: Error number. See QL type.h for details.
 * @return no value
 */
typedef void (*sv_sim_service_error_cb_f)(int error);

#ifdef __cplusplus
}
#endif

#endif  /* __SV_SIM_COMMON_H__ */

