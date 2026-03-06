
#ifndef MTK_RIL_ADAPTER_FAC_H_
#define MTK_RIL_ADAPTER_FAC_H_

#include <telephony/ril.h>

#ifdef __cplusplus
extern "C" {
#endif

void initMtkRilAdapter(const struct RIL_Env *pEnv);

int handleRequest(int request, void *data, size_t datalen, RIL_Token t, int slotId);
int handleSapRequest(int request, void *data, size_t datalen, RIL_Token t, int slotId);
int handleUnsolicitedResponse(int unsolResponse, void *data, size_t datalen, int slotId);
int handleSapUnsolicitedResponse(int unsolResponse, void *data, size_t datalen, int slotId);
void handleSetRadioState(RIL_RadioState oldState, RIL_RadioState newState, RIL_SOCKET_ID slotId);

void handleRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen);
void handleSapRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* MTK_RIL_ADAPTER_FAC_H_ */
