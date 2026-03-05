/**
 * @file ql_sms_common.h
 * @brief Sms service API.
 *
 * @details 
 * AG59x series module sim service. 
 *
 * @htmlonly 
 * <span style="font-weight: bold">History</span> 
 * @endhtmlonly
 *
 * when|who|why
 * ----------|--------------|-------------------------------------------
 * 
 */

#ifndef __QL_SMS_COMMON_H__
#define __QL_SMS_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define QL_SMS_MAX_SEND_MSG_LENGTH  1440    /**<  Maximum length of an sending SMS (9*160). */
#define QL_SMS_MAX_SEND_PDU_LENGTH  255     /**<  Maximum length of an sms PDU. */
#define QL_SMS_MAX_RECV_MSG_LENGTH  160     /**<  Maximum length of an recv SMS. */
#define QL_SMS_MAX_ADDR_LENGTH      252     /**<  Maximum length of SCA addr. */

#define QL_SMS_MAX_NUMBER_LENGTH          (32)    /**<  Maximum length of SMS number. */
#define QL_SMS_MAX_WHITE_LIST_MEM_SIZE    (512)   /**<  Maximum length of SMS white-list memory size. */
#define QL_SMS_MAX_WHITE_LIST_COUNT       (QL_SMS_MAX_WHITE_LIST_MEM_SIZE / QL_SMS_MAX_NUMBER_LENGTH) /**<  Maximum length of SMS white-list count. */

/** The SMS message format. */
typedef enum 
{
    QL_SMS_MSG_FORMAT_GSM_7BIT      = 0,    /**<  GSM 7-bit */
    QL_SMS_MSG_FORMAT_BINARY_DATA   = 1,    /**<  Binary SMS */
    QL_SMS_MSG_FORMAT_UCS2          = 2,    /**<  short message encoding. */
} QL_SMS_MSG_FORMAT_E;

/** */
typedef enum {
  QL_SMS_PRIMARY_SUBSCRIPTION      = 0,  /**< The primary card */
  QL_SMS_SECONDARY_SUBSCRIPTION    = 1,  /**< The second card*/
} QL_SMS_SUBSCRIPTION_E;


/** The SMS message structure. */
typedef struct 
{
    QL_SMS_MSG_FORMAT_E format; /**< SMS format. */
    char addr[QL_SMS_MAX_ADDR_LENGTH + 1]; /**< Source or destinamtion address. */
    int content_size;  /**< SMS Length */
    char content[QL_SMS_MAX_SEND_MSG_LENGTH];  /**< SMS Content */
} ql_sms_msg_t;

/** The SMS timestamp structure. */
typedef struct 
{
    uint8_t year;       /**< Year. since 2000. e.g. 19 means 2019 */
    uint8_t month;      /**< Month.*/
    uint8_t day;        /**< Day.*/
    uint8_t hours;      /**< Hour. 00 ~ 23 */
    uint8_t minutes;    /**< Minutes.*/
    uint8_t seconds;    /**< Seconds.*/
    /** 
     * The Time Zone indicates the difference, expressed in quarters of an hour, 
     * between the local time and GMT. 
     * */
    uint8_t timezone;
} ql_sms_timestamp_t;

/** Definition of long and short message header structure */
typedef struct
{
    
    uint8_t valid;       /**< Indicate whether following is valid, 1 - valid; 0 - invalid; */
    uint8_t total_seg;   /**< The number of long message segment*/
    uint8_t cur_seg_num; /**< Current number.*/
    uint8_t ref_num;     /**< reference number.*/
} ql_sms_user_data_head_t;

/**
 * recv SMS messages asynchronously error callback function
 * @param[in] id: ID of the event that sends SMS messages asynchronously.
 * @param[in] result: Result of sending SMS messages asynchronously
 * @return no value
 */
typedef void (*ql_sms_msg_async_cb_f)(int id, int result);


/**
 * send SMS messages asynchronously error callback function
 * @param[in] p_msg: Text messages
 * @param[in] timestamp: SMS timestamp
 * @param[in] head: Long message header information
 * @return no value
 */
typedef void (*ql_sms_msg_recv_cb_f)(ql_sms_msg_t *p_msg, ql_sms_timestamp_t *timestamp,
                                             ql_sms_user_data_head_t *head);

/**Enum The SMS PDU format. */
typedef enum
{
    QL_SMS_PDU_FORMAT_CDMA  = 0,   /**< CDMA.*/
    QL_SMS_PDU_FORMAT_GW_PP = 6,   /**<GSM & WCDMA - Point to point.*/
} QL_SMS_PDU_FORMAT_E;

/** The SMS PDU structure. */
typedef struct 
{
    QL_SMS_PDU_FORMAT_E format;   /**< PDU SMS format. */
    int content_size;   /**< PDU Length */ 
    char content[QL_SMS_MAX_SEND_PDU_LENGTH];   /**< PDU Content */
} ql_sms_pdu_t;


/** The SMS white-list info structure. */
typedef struct 
{
    int  list_count;
    char list_info[QL_SMS_MAX_WHITE_LIST_COUNT][QL_SMS_MAX_NUMBER_LENGTH];
} ql_sms_white_list_info_t;

/**
 * The SMS exception callback function
 * @param[in] error: Error number. See QL type.h for details.
 * @return no value
 */
typedef void (*ql_sms_service_error_cb_f)(int error);

#ifdef __cplusplus
}
#endif

#endif  /* __QL_SMS_H__ */
