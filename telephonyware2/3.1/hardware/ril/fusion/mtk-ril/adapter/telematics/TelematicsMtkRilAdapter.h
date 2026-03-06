#ifndef TELEMATICSMTKRILADAPTER_H_
#define TELEMATICSMTKRILADAPTER_H_

#include "MtkRilAdapter.h"
#include <telephony/mtk_ril_request_info.h>
#include <telephony/mtk_ril_ivt.h>
#include <string>
#include "../../../libril/ril_internal.h"

namespace mtk {

class TelematicsMtkRilAdapter: public mtk::MtkRilAdapter {
public:
    TelematicsMtkRilAdapter(const struct RIL_Env *env);
    virtual ~TelematicsMtkRilAdapter();

    int onHandleRequest(int request, void *data, size_t datalen, RIL_Token t, int slotId);
    int onHandleSapRequest(int request, void *data, size_t datalen, RIL_Token t, int slotId);
    int onHandleUnsolicitedResponse(int unsolResponse, void *data, size_t datalen, int slotId);
    int onHandleSapUnsolicitedResponse(int unsolResponse, void *data, size_t datalen, int slotId);
    void onHandleSetRadioState(RIL_RadioState oldState, RIL_RadioState newState,
            RIL_SOCKET_ID socId);

    void onHandleRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen);
    void onHandleSapRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen);

protected:
    virtual void onLibRilReady();

private:
    void sendEnableImsRequestInternal(RIL_SOCKET_ID socId, int volte_enable);
    void handleImsConfigDynamicImsSwitchComplete(RIL_SOCKET_ID socId);
    void handleRequestEnableImsInternal(int slotId, void * data, RIL_Token t);
    int handleRequestSetImsCfgInternal(int slotId, void * data, RIL_Token t);
    int handleSmsRequestInternal(int request, void *data, size_t datalen, RIL_Token t, int slotId);
    int transferToImsRequest(int request);
    void handleImsRegStateResponse(const void *data, int slotId);
    int handleRequestMakeEcall(int request, void *data, size_t datalen, RIL_Token t, int slotId);
    int handleRequestDial(int request, void *data, size_t datalen, RIL_Token t, int slotId);
    void onHandleUrcSrvcc(const void *data, int slotId);
    void handleGetCurrentCallsResponse(android::RequestInfo *pRI, RIL_Errno e, void *response, size_t responselen);
    void handleMakeECallResponse(android::RequestInfo *pRI, RIL_Errno e, void *response, size_t responselen);
    void handleEmergencyDialResponse(android::RequestInfo *pRI, RIL_Errno e, void *response, size_t responselen);
    bool isEccMatchInList(String8 number, String8 eccList);
    int turnOffRadioBeforeFastECall(int request, void * data, size_t datalen, RIL_Token t, int slotId);
    void requestOppositeRadioPower(int power, int slotId);
    void onHandleEcallIndications(void *data, int slotId);
    void copyEcallData(RIL_ECallReqMsg *dst, RIL_ECallReqMsg *src);
    static const std::string hash256(const char* str);
    void updateIccId(size_t datalen, int slotId, void *data);
    bool isOp09SimCard(int slot_id);
    void updateCurrerntIccidForNull(int slotId);
    int mIsImsReg[MAX_SIM_COUNT];
    int mOnceStartIms[MAX_SIM_COUNT];
    bool mIsSmsReady;
    int mSrvccState[MAX_SIM_COUNT];
    int mImsEccCount[MAX_SIM_COUNT];
    bool mCsCallStateChanged[MAX_SIM_COUNT];

    static char sLastBootIccId[MAX_SIM_COUNT][40];
    static char sCurrentIccId[MAX_SIM_COUNT][40];
    static const char PROPERTY_LAST_ICCID_SIM[MAX_SIM_COUNT][40];
    static const char* PROPERTY_VOLTE_STATE;

    /* DSDS ECall feature */
    RIL_RadioState mRadioState[MAX_SIM_COUNT];
    int mDesiredPowerState[MAX_SIM_COUNT];
    bool mWaitForRadioPowerOffForECall;
    bool mShouldRecoverRadioState;
    bool mRejectRadioPowerDuringECall;
    RIL_ECallReqMsg mEcallMessage;
    int mEcallSlotId;
    RIL_Token mEcallToken;

    enum {
        SRVCC_STATE_NONE = -1,
        SRVCC_STATE_STARTED = 0,
        SRVCC_STATE_COMPLETED = 1,
        SRVCC_STATE_FAILED = 2,
        SRVCC_STATE_CANCELED = 3,
    };
};
} /* namespace mtk */
#endif /* TELEMATICSMTKRILADAPTER_H_ */
