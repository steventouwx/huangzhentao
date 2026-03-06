#ifndef MTKRILADAPTER_H_
#define MTKRILADAPTER_H_

#include <mtk_log.h>
#include <telephony/ril.h>
#include "RfxStatusManager.h"
#include "RfxRootController.h"

extern struct RIL_Env *s_rilenv;
extern "C" const char * requestToString(int request);

namespace mtk {

class MtkRilAdapter {
public:
    MtkRilAdapter(const struct RIL_Env *env);
    virtual ~MtkRilAdapter();

    virtual int onHandleRequest(int request, void *data, size_t datalen, RIL_Token t, int slotId);
    virtual int onHandleSapRequest(int request, void *data, size_t datalen, RIL_Token t,
            int slotId);
    virtual int onHandleUnsolicitedResponse(int unsolResponse, void *data, size_t datalen,
            int slotId);
    virtual int onHandleSapUnsolicitedResponse(int unsolResponse, void *data, size_t datalen, int slotId);
    virtual void onHandleSetRadioState(RIL_RadioState oldState, RIL_RadioState newState,
            RIL_SOCKET_ID socId);

    virtual void onHandleRequestComplete(RIL_Token t, RIL_Errno e, void *response,
            size_t responselen);
    virtual void onHandleSapRequestComplete(RIL_Token t, RIL_Errno e, void *response,
            size_t responselen);

protected:
    /* do some initialization here if necessary */
    virtual void onLibRilReady();
    inline RfxStatusManager* getStatusManager(int slotId) {
        return RfxRootController::getInstance()->getStatusManager(slotId);
    }

    void issueLocalRequest(int request, void *data, int len, RIL_SOCKET_ID socId);
    void requestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen);

private:
    const struct RIL_Env *mPRilEnv;
    bool mLibRilReady;
};

} /* namespace mtk */
#endif /* MTKRILADAPTER_H_ */
