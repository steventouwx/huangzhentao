/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_dm_common.h 
  @brief device management common define

  @detailes
  AG59x series module dm service.

  @htmlonly
  <span style="font-weight: bold">History</span>
  @endhtmlonly
  
  when     |  who      |    what, where, why
  -------- |  ---      |    ----------------------------------------------------------
-------------------------------------------------------------------------------------------------*/

#ifndef __QL_DM_COMMON_H__
#define __QL_DM_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define QL_DM_IMEI_MAX_LEN             32
#define QL_DM_MEID_MAX_LEN             32
#define QL_DM_FIRMWARE_REV_MAX_LEN     256
#define QL_DM_NV_ITEM_NAME_MAX_LEN     127
#define QL_DM_NV_ITEM_VALUE_MAX_LEN    2048
#define QL_DM_CPU_COUNT_MAX            16
#define QL_DM_MIN_LOG_TIMER_VALUE      30000
#define QL_DM_MIN_INTERVAL_TIMER_VALUE 60000

//enable
#define QL_DM_QOOS_ENABLE_MAX          2
#define QL_DM_QOOS_ENABLE_MIN          0

//pre_time
#define QL_DM_QOOS_PRE_TIME_MAX        600
#define QL_DM_QOOS_PRE_TIME_MIN        1

//sleep_time
#define QL_DM_QOOS_SLEEP_TIME_MAX      (60*60*12)
#define QL_DM_QOOS_SLEEP_TIME_MIN      1

//acq_time
#define QL_DM_QOOS_ACQ_TIME_MAX        600
#define QL_DM_QOOS_ACQ_TIME_MIN        1

//cir_cnt
#define QL_DM_QOOS_CIR_CNT_MAX         255
#define QL_DM_QOOS_CIR_CNT_MIN         1

//sleep_step
#define QL_DM_QOOS_SLEEP_STEP_MAX      60
#define QL_DM_QOOS_SLEEP_STEP_MIN      0

//acq_step
#define QL_DM_QOOS_ACQ_STEP_MAX        60
#define QL_DM_QOOS_ACQ_STEP_MIN        0        

//adj_cnt
#define QL_DM_QOOS_ADJ_CNT_MAX         5
#define QL_DM_QOOS_ADJ_CNT_MIN         0

//idle_interval
#define QL_DM_QOOS_IDLE_INTERVAL_MAX   60
#define QL_DM_QOOS_IDLE_INTERVAL_MIN   1


/*
* @par 
*xxxxxxxxxxxxx
*/
/**enum air plane mode type*/
typedef enum QL_DW_AIR_PLANE_MODE_TYPE_ENUM
{
    QL_DM_AIR_PLANE_MODE_UNKNOWN   = 0,    /**<  Unknown airplane mode. */
    QL_DM_AIR_PLANE_MODE_ON        = 1,    /**<  Airplane mode is on. */
    QL_DM_AIR_PLANE_MODE_OFF       = 2,    /**<  Airplane mode is off. */
    //QL_DM_AIR_PLANE_MODE_FTM       = 3,    /**<  cfun 5 */
    //QL_DM_AIR_PLANE_MODE_OFFLINE   = 4,    /**<  cfun 7 */
    QL_DM_AIR_PLANE_MODE_NA        = 3     /**<  Airplane mode is unavailable. */
}QL_DM_AIR_PLANE_MODE_TYPE_E;

/**enum radio mode type*/
typedef enum QL_DM_RADIO_MODE_TYPE_ENUM
{
    QL_DM_RADIO_MODE_UNKNOWN   = 0,    /**<  Unknown radio mode. */
    QL_DM_RADIO_MODE_LOW_POWER = 1,    /**<  radio mode lowpower. */
    QL_DM_RADIO_MODE_ONLINE    = 2,    /**<  radio mode online. */
    QL_DM_RADIO_MODE_FTM       = 3,    /**<  radio mode Factory Test mode.*/
    QL_DM_RADIO_MODE_OFFLINE   = 4,    /**<  radio mode offline. */
    QL_DM_RADIO_MODE_NA        = 5     /**<  radio mode is unavailable. */
}QL_DM_RADIO_MODE_TYPE_E;

/**enum modem state type*/
typedef enum QL_DM_MODEM_STATE_TYPE_ENUM
{
    QL_DM_MODEM_STATE_OFFLINE = 0,
    QL_DM_MODEM_STATE_ONLINE = 1,
    QL_DM_MODEM_STATE_UNKNOWN = 2
}QL_DM_MODEM_STATE_TYPE_E;

/**enum qoos enable type*/
typedef enum QL_DM_QOOS_ENABLE_TYPE_ENUM
{
    QL_DM_QOOS_ENABLE_STATE_OFF           = 0,  /**<  Disable QOOS. */
    QL_DM_QOOS_ENABLE_STATE_ON_NO_SERVICE = 1,  /**<  Enable QOOS under the status of no service. */
    QL_DM_QOOS_ENABLE_STATE_ON            = 2,  /**<  Enable QOOS under the status of no service or limited service.. */
}QL_DM_QOOS_ENABLE_TYPE_E;

/**enum bind subscription type*/
typedef enum QL_DM_BIND_SUB_TYPE_ENUM
{
  QL_DM_BIND_SUB_PRIMARY = 0, 
  QL_DM_BIND_SUB_SECONDARY = 1, 
}QL_DM_BIND_SUB_TYPE_E;

/**enum boot up reason*/
typedef enum QL_DM_BOOT_UP_REASON_ENUM
{
    QL_DM_BOOT_UP_INVALID    = -1,
    QL_DM_BOOT_UP_HARD_RESET = 0,
    QL_DM_BOOT_UP_KPDPWR_N   = 1,
    QL_DM_BOOT_UP_RTC_ALARM  = 2
}QL_DM_BOOT_UP_REASON_E;

/**enum shutdown reason */
typedef enum QL_DM_SHUTDOWN_REASON_ENUM
{
    QL_DM_SHUTDOWN_INVALID          = -1,
    QL_DM_SHUTDOWN_PS_HOLD          = 0,
    QL_DM_SHUTDOWN_KPDPWR_N_S2      = 1,
    QL_DM_SHUTDOWN_RESIN_N_S2       = 2,
}QL_DM_SHUTDOWN_REASON_E;

/**enum poweroff mode */
typedef enum QL_DM_POWEROFF_MODE_ENUM
{
    QL_DM_POWEROFF_MODE_INVALID          = -1,
    QL_DM_POWEROFF_MODE_POWEROFF         = 0,    /**<   Switch off the machine */
    QL_DM_POWEROFF_MODE_REBOOT           = 1,    /**<   Reboot the machine */
    QL_DM_POWEROFF_MODE_FORCE_POWEROFF   = 2,    /**<   Force immediate poweroff */
    QL_DM_POWEROFF_MODE_FORCE_REBOOT     = 3,    /**<   Force immediate reboot */
}QL_DM_POWEROFF_MODE_E;

/**enum thermal sensor type */
enum QL_DM_THERMAL_SENSOR_ENUM {
    QL_DM_THERMAL_SDR0_PA = 0,    /**<  Thermal zone sdr0_pa. */
    QL_DM_THERMAL_SDR0,           /**<  Thermal zone sdr0. */
    QL_DM_THERMAL_DIE_TEMP_AOSS,  /**<  Thermal zone aoss-0. */
    QL_DM_THERMAL_DIE_TEMP_CPU0,  /**<  Thermal zone cpuss-0. */
    QL_DM_THERMAL_DIE_TEMP_CPU1,  /**<  Thermal zone cpuss-1. */
    QL_DM_THERMAL_DIE_TEMP_CPU2,  /**<  Thermal zone cpuss-2. */
    QL_DM_THERMAL_DIE_TEMP_CPU3,  /**<  Thermal zone cpuss-3. */
    QL_DM_THERMAL_DIE_TEMP_ETH,   /**<  Thermal zone ethphy-0. */
    QL_DM_THERMAL_DIE_TEMP_MVM,   /**<  Thermal zone mvmss-0. */
    QL_DM_THERMAL_DIE_TEMP_DSP,   /**<  Thermal zone mdmq6-0. */
    QL_DM_THERMAL_DIE_TEMP_CTILE, /**<  Thermal zone ctile. */
    QL_DM_THERMAL_DIE_TEMP_MDM0,  /**<  Thermal zone mdmss-0. */
    QL_DM_THERMAL_DIE_TEMP_MDM1,  /**<  Thermal zone mdmss-1  */
    QL_DM_THERMAL_DIE_TEMP_MDM2,  /**<  Thermal zone mdmss-2  */
    QL_DM_THERMAL_THERM_PMX75_TZ, /**<  Thermal zone pmx75_tz  */
    QL_DM_THERMAL_SYS_THERM1,     /**<  Thermal zone sys-therm-1  */
    QL_DM_THERMAL_SYS_THERM2,     /**<  Thermal zone sys-therm-2  */
    QL_DM_THERMAL_SYS_THERM3,     /**<  Thermal zone sys-therm-3  */
    QL_DM_THERMAL_SYS_THERM4,     /**<  Thermal zone sys-therm-4  */
    QL_DM_THERMAL_XO_THERM,       /**<  Thermal zone xo-therm  */
    QL_DM_THERMAL_SDR1_PA,        /**<  Thermal zone sdr1_pa  */
    QL_DM_THERMAL_SDR1,           /**<  Thermal zone sdr1  */
    QL_DM_THERMAL_THERM_MAX
};

typedef enum QL_DM_ANTENNA_ENUM
{
    QL_DM_ANTENNA_INVALID = -1,
    QL_DM_ANTENNA_0       = 0, 
    QL_DM_ANTENNA_1       = 1, 
    QL_DM_ANTENNA_2       = 2, 
    QL_DM_ANTENNA_3       = 3, 
    QL_DM_ANTENNA_4       = 4
}QL_DM_ANTENNA_E;

/**enum ims apn lost recovery status*/
typedef enum QL_IMS_APN_LOST_RECOVERY_STATUS
{
    QL_IMS_APN_LOST_RECOVERY_ERROR   = 0,
    QL_IMS_APN_LOST_RECOVERY_OK      = 1
}QL_IMS_APN_LOST_RECOVERY_STATUS;

/** QOOS configuration*/
typedef struct {
    uint16_t pre_time;      /**<  Time for first network searching. Range: 1–600. Default value: 180. Unit: S. */
    uint16_t sleep_time;    /**<  Interval for network searching. Range: 1–43200. Default value: 30. Unit: S.*/
    uint16_t acq_time;      /**<  Network searching time. Range: 1–600. Default value: 20. Unit: S. */
    uint8_t cir_cnt;        /**<  Loop count. Range: 5–255. Default value: 10. Unit: times. */
    uint8_t sleep_step;     /**<  Adjustment time for sleep_time. Range: 0–60. Default value: 15. Unit: S. */
    uint8_t acq_step;       /**<  Adjustment time for acq_time. Range: 0–60. Default value: 0. Unit: S. */
    uint8_t adj_cnt;        /**<  Adjustment count. Range: 0–5. Default value: 2. Unit: times. */
    uint8_t idle_interval;  /**<  Interval for network disconnection check. Range: 1–60. Default value: 1. Unit: S. */
}ql_dm_qoos_config_t;

/**device serial number info*/
typedef struct 
{
    uint8_t imei_valid; 
    char imei[QL_DM_IMEI_MAX_LEN + 1];    /**<  imei.*/
    uint8_t imei2_valid; 
    char imei2[QL_DM_IMEI_MAX_LEN + 1];    /**<  imei2.*/
    uint8_t meid_valid; 
    char meid[QL_DM_MEID_MAX_LEN + 1];    /**<  meid.*/
}ql_dm_device_serial_numbers_info_t;

/** CPU info */
typedef struct
{
    int cpu_index;
    int enabled;
}
ql_dm_cpu_state_t;

/** CPU list info*/
typedef struct
{
    int cpu_count;
    ql_dm_cpu_state_t cpu_info[QL_DM_CPU_COUNT_MAX];  /** only first cpu_count elements are valid */
}ql_dm_cpu_list_state_t;

/**
  @brief modem state indication call back.
  @param[in] modem_state modem state
  @retval no value.
*/
typedef void (*ql_dm_modem_state_ind_cb)(int modem_state);

/**
  @brief service error call back.
  @param[in] error type
  @retval no value.
*/
typedef void (*ql_dm_service_error_cb_f)(int error);

/**
  @brief modem offline reason indication call back.
  @param[in] modem offline reason
  @retval no value.
*/
typedef void (*ql_dm_modem_offline_reason_ind_cb)(char *reason, int reason_len);

/**
  @brief The function of this callback is to notify ims that the apn loss has been recovered or an error has occurred.
  @param[in] status Indicates whether recovery succeeded or retry
  @retval no value.
*/
typedef void (*ql_dm_mbn_recovery_cb_f)(QL_IMS_APN_LOST_RECOVERY_STATUS status);

#ifdef __cplusplus
}
#endif


#endif

