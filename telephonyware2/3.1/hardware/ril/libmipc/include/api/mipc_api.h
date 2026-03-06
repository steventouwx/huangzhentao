#ifndef _MIPC_API_H_
#define _MIPC_API_H_

#include "mipc.h"

typedef enum {
    MIPC_API_RESULT_SUCCESS = 0,    /*!< API result is SUCCESS */
    MIPC_API_RESULT_FAIL = 1,       /*!< API result is FAIL */
    MIPC_API_RESULT_TIMEOUT = 2     /*!< API result is TIMEOUT */
} mipc_api_result_enum;

typedef enum {
    MIPC_API_SIM0 = MIPC_SIM0,
    MIPC_API_SIM1 = MIPC_SIM1,
    MIPC_API_SIM2 = MIPC_SIM2,
    MIPC_API_SIM3 = MIPC_SIM3,
    MIPC_API_PS0 = MIPC_PS0,
    MIPC_API_PS1 = MIPC_PS1,
    MIPC_API_PS2 = MIPC_PS2,
    MIPC_API_PS3 = MIPC_PS3,
    MIPC_API_ALL = MIPC_ALL
} mipc_sim_ps_id_enum;


#endif
