// SPDX-License-Identifier: MediaTekProprietary

#include <mtk_log.h>
#include "TeleDeathReipient.h"
#include "RpAidlAdapter.h"

#define LOG_TAG "TeleDeathReipient"

TeleDeathReipient::TeleDeathReipient() {
    // TODO Auto-generated constructor stub

}

TeleDeathReipient::~TeleDeathReipient() {
    // TODO Auto-generated destructor stub
}

void TeleDeathReipient::serviceDied(uint64_t cookie, const wp<IBinder>& who) {
    RLOGI("serviceDied %d", cookie);
    RpAidlAdapter::getInstance()->unhandleDeathReipient(cookie);
}
