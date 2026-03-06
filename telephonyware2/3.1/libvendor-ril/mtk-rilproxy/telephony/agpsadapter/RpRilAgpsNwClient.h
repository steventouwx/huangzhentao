// SPDX-License-Identifier: MediaTekProprietary
/*
 * RpRilAgpsNwClient.h
 *
 *  Created on: Jun 12, 2017
 *      Author: Kaiduan Cao
 */

#ifndef MTK_RILPROXY_TELEPHONY_AGPSADAPTER_RPRILAGPSNWCLIENT_H_
#define MTK_RILPROXY_TELEPHONY_AGPSADAPTER_RPRILAGPSNWCLIENT_H_

#include "client/RilClient.h"

class RpRilAgpsNwClient: public RilClient {
public:
    RpRilAgpsNwClient(int identity, char* clientName);
    virtual ~RpRilAgpsNwClient();
protected:
    void onRequestComplete(int slotId, int token, RIL_Errno e, void *response, size_t responselen);
    void onUnsolicitedResponse(int slotId, int unsolResponse, const void *data, size_t datalen);
};

#endif /* MTK_RILPROXY_TELEPHONY_AGPSADAPTER_RPRILAGPSNWCLIENT_H_ */
