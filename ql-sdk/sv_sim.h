/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_sim.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_sim相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_SIM_H__
#define __SV_SIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "sv_sim_common.h"

/**
 * sim status callback function
 * @param[in] slot: sim slot number
 * @param[in] p_info:sim info
 * @return no value
 */
typedef void (*sv_sim_card_status_cb_f)(SV_SIM_SLOT_E slot, sv_sim_card_info_t *p_info);

/**
 * sim card refresh callback function
 * @param[in] p_info refresh status
 * @return no value
 */
typedef void (*sv_sim_card_refresh_cb_f)(sv_sim_refresh_info_t* p_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Initializes SIM service.
  @note You must call this function before other functions can be used in this module.
  @return Whether the SIM service was intialized successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_init(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Deinitializes SIM service.
  @return Whether the SIM service was deintialized successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Gets the IMSI (for 3GPP) or IMSI_M (for 3GPP2) from the SIM in ASCII form.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [out] imsi Buffer to fill IMSI data.
  @param [in] imsi_len Buffer length.
  @return Whether the IMSI was successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_get_imsi(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, char *imsi, int imsi_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Retrieves the Integrated Circuit Card ID (ICCID) stored on the card.
  @param [in] slot Slot to be used.
  @param [out] iccid Buffer to fill ICCID data.
  @param [in] iccid_len Buffer length.
  @return Whether the ICCID was successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_get_iccid(SV_SIM_SLOT_E slot, char *iccid, int iccid_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Retrieves the device phone number stored on the card.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [out] phone_num Buffer to fill phone number.
  @param [in] phone_num_len Buffer length.
  @return Whether the phone number was successfully retrieved.
  @retval SV_ERR_OK successful
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_get_phone_num(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, 
                          char *phone_num, int phone_num_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Retrieves the preferred operators stored on the card.
  @param [in] slot Slot to be used.
  @param [in] list Buffer to hold operators
  @note This function is only supported by 3GPP applications.
  @return Whether the preferred operators were successfully retrieved.
  @retval SV_ERR_OK successful
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_get_operators(SV_SIM_SLOT_E slot, sv_sim_operator_list_t *list);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Enables the PIN on an application.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [in] pin PIN to be used.
  @param [in] pin_value PIN value. NULL terminated.
  @return Whether the PIN was successfully enabled.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_enable_pin(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, 
                             SV_SIM_PIN_E pin, const char *pin_value);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Disables the PIN on an application.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [in] pin PIN to be used.
  @param [in] pin_value PIN value. NULL terminated.
  @return Whether the PIN was successfully disabled.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_disable_pin(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, 
                             SV_SIM_PIN_E pin, const char *pin_value);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Verifies the PIN value of an application.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [in] pin PIN to be used.
  @param [in] pin_value PIN value. NULL terminated.
  @note PIN must be enabled before calling this function.
  @return Whether the PIN was successfully verified.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_verify_pin(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, 
                             SV_SIM_PIN_E pin, const char *pin_value);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Changes the PIN value of an application.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [in] pin PIN to be used.
  @param [in] old_pin_value Old PIN value. NULL terminated.
  @param [in] new_pin_value New PIN value. NULL terminated.
  @return Whether the PIN was successfully changed.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_change_pin(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, 
                             SV_SIM_PIN_E pin, const char *old_pin_value, const char *new_pin_value);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Unblocks a blocked PIN using the PUK code.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [in] pin PIN to be used.
  @param [in] puk_value PUK value.  NULL terminated.
  @param [in] pin_value New PIN value.  NULL terminated.
  @note The user must pass PUK1 to unblock PIN1 or PUK2 to unblock PIN2.
  @return Whether the PIN was successfully unblocked.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_unblock_pin(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, 
                             SV_SIM_PIN_E pin, const char *puk_value, const char *pin_value);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Retrieves the card info stored on a card.
  @param [in] slot Slot to be used.
  @param [out] p_info Pointer of sv_sim_card_info_t.
  @return Whether the card info was successfully retrieved.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_get_card_info(SV_SIM_SLOT_E slot, sv_sim_card_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Reads data from a specific file on a specified application on the card.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [inout] p_file Pointer of sv_sim_file_t.
  @return Whether the file was successfully read.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_read_file(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, sv_sim_file_t *p_file);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Writes data to a specific file on a specified application on the card. 
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [in] p_file Pointer of sv_sim_file_t
  @note The type of file is determined by the record number field,
    which indicates a transparent file when zero and a record-based file otherwise.
  @return Whether the file was successfully written.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_write_file(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, sv_sim_file_t *p_file);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Retrieves the info of a specific file on a specified application on the card.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [inout] p_info Pointer of sv_sim_file_info_t.
  @return Whether the file info was successfully retrieved.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_get_file_info(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, 
                                 sv_sim_file_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Reads phone book on a specified application on the card.
  @param [in] slot Slot to be used.
  @param [in] app_type Spplication type.
  @param [in] pb_path Phone book path. NULL terminated.
  @param [in] record_idx Record index to read. Starts from 1.
  @param [out] p_record Pointer of sv_sim_phone_book_record_t.
  @return Whether the phone book record was successfully retrieved.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_read_phone_book(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type,
                                    const char *pb_path, uint8_t record_idx,
                                    sv_sim_phone_book_record_t *p_record);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Writes phone book on a specified application on the card.
  @param [in] slot Slot to be used.
  @param [in] app_type Application type.
  @param [in] pb_path Phone book path. NULL terminated.
  @param [in] record_idx Record index to write. Starts from 1.
  @param [in] p_record Pointer of sv_sim_phone_book_record_t.
  @note If p_record->name[0] = 0 and p_record->number[0] = 0, record will be deleted.
  @return Whether the phone book record was successfully saved.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_write_phone_book(SV_SIM_SLOT_E slot, SV_SIM_APP_TYPE_E app_type, 
                                    const char *pb_path, uint8_t record_idx,
                                    sv_sim_phone_book_record_t *p_record);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Opens a logical channel on a UICC card.
  @param [in] slot Slot to be used.
  @param [out] channel_id Channel opened.
  @return Whether the logical channel was successfully opened.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_open_logical_channel(SV_SIM_SLOT_E slot, uint8_t *channel_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Closes a logical channel on a UICC card.
  @param [in] slot Slot to be used.
  @param [in] channel_id Channel to be closed.
  @return Whether the logical channel was successfully closed.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_close_logical_channel(SV_SIM_SLOT_E slot, uint8_t channel_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Sends an APDU to the card.
  @param [in] slot Slot to be used.
  @param [in] channel_id Channel to be used.
  @param [inout] p_apdu Pointer of sv_sim_apdu_t.
  @note You must call sv_sim_open_logical_channel before sending an APDU.
  @return Whether the APDU was successfully sent.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_send_apdu(SV_SIM_SLOT_E slot, uint8_t channel_id, sv_sim_apdu_t *p_apdu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets SIM card status callback handler
  @param[in] cb call back handler.
  @return Whether the card status callback handler was successfully set.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_set_card_status_cb(sv_sim_card_status_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets SIM card refresh callback handler
  @param[in] cb call back handler.
  @return Whether the card status callback handler was successfully set.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_set_card_refresh_cb(sv_sim_card_refresh_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Switches slot.
  @param [in] log_slot Logical slot to be switched.
  @param [in] phy_slot Physical slot to be switched.
  @return Whether the slot was successfully switched.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_switch_slot(SV_SIM_SLOT_E log_slot, SV_SIM_PHY_SLOT_E phy_slot);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Gets active slots.
  @param [Out] p_active_slots Active slots.
  @return Whether the active slots were successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_get_active_slots(sv_sim_active_slots_t *p_active_slots);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is SV_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_set_service_error_cb(sv_sim_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Retrieves the EID stored on the card.
  @param [in] slot Slot to be used.
  @param [out] eid Buffer to fill EID data.
  @param [in] eid_len Buffer length.
  @return Whether the EID was successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sim_get_eid(SV_SIM_SLOT_E slot, char *eid, int eid_len);

#ifdef __cplusplus
}
#endif

#endif  /* __SV_SIM_H__ */

