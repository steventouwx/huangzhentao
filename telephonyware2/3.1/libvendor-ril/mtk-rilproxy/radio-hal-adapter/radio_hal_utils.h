// SPDX-License-Identifier: MediaTekProprietary
#ifndef RADIO_HAL_UTILS_H
#define RADIO_HAL_UTILS_H
#include <string>
#include <vector>
#include <android/binder_ibinder.h>
#include "RpDataUtils.h"

extern const std::vector<std::string>  portNames;
extern const std::vector<std::string>  imsPortNames;

enum {
  CLIENT_RILJ = 0,
  CLIENT_IMS,
  CLIENT_SE,
  CLIENT_EM,
  CLIENT_ASSIST,
  CLIENT_RCS,
  CLIENT_CAP,
  CLIENT_SMART_RAT_SWITCH,
  CLIENT_RSU,
  CLIENT_DCH,
  CLIENT_GBA,
  CLIENT_TBOX,
  CLIENT_TBOX_IMS,
  CLIENT_COUNT
};

#endif // RADIO_HAL_UTILS_H