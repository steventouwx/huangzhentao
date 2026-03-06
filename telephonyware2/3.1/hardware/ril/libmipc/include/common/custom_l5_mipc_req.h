#ifndef _CUSTOM_L5_MIPC_REQ_H_
#define _CUSTOM_L5_MIPC_REQ_H_

#undef MIPC_REQ
#define MIPC_REQ(msg, handler) MIPC_VENDOR_##msg##_REQ, MIPC_VENDOR_##msg##_CNF,
enum {
    MIPC_VENDOR_REQ_START = MIPC_REQ_BEGIN + 0xF00,
#include "custom_l5_mipc_req.def"
};

#endif
