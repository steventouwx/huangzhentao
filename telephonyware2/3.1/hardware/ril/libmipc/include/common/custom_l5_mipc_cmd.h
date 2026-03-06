#ifndef _CUSTOM_L5_MIPC_CMD_H_
#define _CUSTOM_L5_MIPC_CMD_H_

#undef MIPC_CMD
#define MIPC_CMD(msg) MIPC_VENDOR_##msg##_CMD, MIPC_VENDOR_##msg##_RSP,
enum {
    MIPC_VENDOR_CMD_START = MIPC_CMD_BEGIN + 0xF00,
#include "custom_l5_mipc_cmd.def"
};

#endif
