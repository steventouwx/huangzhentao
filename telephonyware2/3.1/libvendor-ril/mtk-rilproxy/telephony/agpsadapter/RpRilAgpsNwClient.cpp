// SPDX-License-Identifier: MediaTekProprietary
/*
 * RpRilAgpsNwClient.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: Kaiduan Cao
 */

#include <mtk_log.h>
#include "RpRilAgpsNwClient.h"
#include "RpAgpsNwAdapter.h"

#define LOG_TAG "RpRilAgpsNwClient"

RpRilAgpsNwClient::RpRilAgpsNwClient(int identity, char* clientName) : RilClient(identity, clientName) {
    RLOGD("RpRilAgpsNwClient create");
    RpAgpsNwAdapter::getInstance()->mRilAgpsNwClient = this;
}

RpRilAgpsNwClient::~RpRilAgpsNwClient() {
    RLOGD("RpRilAgpsNwClient destroy");
    RpAgpsNwAdapter::closeInstance();
}

void RpRilAgpsNwClient::onRequestComplete(int slotId, int token, RIL_Errno e, void *response, size_t responselen) {
    RLOGD("onRequestComplete called");
    RpAgpsNwAdapter::getInstance()->onNwRequestComplete(token, e, response, responselen);
}

void RpRilAgpsNwClient::onUnsolicitedResponse(int slotId, int unsolResponse, const void *data, size_t datalen) {
    // Nothing to do.
}

