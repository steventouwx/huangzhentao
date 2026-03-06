#ifndef _CUSTOM_L5_MIPC_NTF_H_
#define _CUSTOM_L5_MIPC_NTF_H_

#undef MIPC_NTF
#define MIPC_NTF(msg, handler) MIPC_VENDOR_##msg##_NTF,
enum {
    MIPC_VENDOR_NTF_START = MIPC_NTF_BEGIN + 0xF00,
#include "custom_l5_mipc_ntf.def"
};

#endif
