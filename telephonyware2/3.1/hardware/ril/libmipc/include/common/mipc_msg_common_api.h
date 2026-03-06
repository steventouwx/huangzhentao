#ifndef __MIPC_MSG_COMMON_API_H__
#define __MIPC_MSG_COMMON_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_result_enum mipc_get_result(mipc_msg_t *msg_ptr)
{
    if (!msg_ptr) return MIPC_RESULT_FAILURE;
    return (mipc_result_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_T_RESULT, MIPC_RESULT_FAILURE);
}


#endif /* __MIPC_MSG_COMMON_API_H__ */
