#include <mtk_log.h>
#include "MtkRilAdapterFactory.h"
#include "MtkRilAdapter.h"

#ifdef TBOX_ADPTER
#include "TelematicsMtkRilAdapter.h"
#endif

#undef LOG_TAG
#define LOG_TAG "MtkRilAdapterFactory"

static mtk::MtkRilAdapter *g_pAdapter = NULL;

void initMtkRilAdapter(const struct RIL_Env *pEnv) {
    if(pEnv == NULL) {
        mtkAssert((char*)"Terrible failure: ril env null...");
    }
    if (g_pAdapter == NULL) {
#ifdef TBOX_ADPTER
        mtkLogD(LOG_TAG, "[Telematics] create telematics ril adapter...");
        g_pAdapter = new mtk::TelematicsMtkRilAdapter(pEnv);
#else
        mtkLogD(LOG_TAG, "[Common] create common ril adapter...");
        g_pAdapter = new mtk::MtkRilAdapter(pEnv);
#endif
    }
}

int handleRequest(int request, void *data, size_t datalen, RIL_Token t, int slotId) {
    return g_pAdapter->onHandleRequest(request, data, datalen, t, slotId);
}

int handleSapRequest(int request, void *data, size_t datalen, RIL_Token t, int slotId) {
    return g_pAdapter->onHandleSapRequest(request, data, datalen, t, slotId);
}

int handleUnsolicitedResponse(int unsolResponse, void *data, size_t datalen, int slotId) {
    return g_pAdapter->onHandleUnsolicitedResponse(unsolResponse, data, datalen, slotId);
}

int handleSapUnsolicitedResponse(int unsolResponse, void *data, size_t datalen, int slotId) {
    return g_pAdapter->onHandleSapUnsolicitedResponse(unsolResponse, data, datalen, slotId);
}

void handleSetRadioState(RIL_RadioState oldState, RIL_RadioState newState, RIL_SOCKET_ID socId) {
    return g_pAdapter->onHandleSetRadioState(oldState, newState, socId);
}

void handleRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen) {
    return g_pAdapter->onHandleRequestComplete(t, e, response, responselen);
}

void handleSapRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen) {
    return g_pAdapter->onHandleSapRequestComplete(t, e, response, responselen);
}
