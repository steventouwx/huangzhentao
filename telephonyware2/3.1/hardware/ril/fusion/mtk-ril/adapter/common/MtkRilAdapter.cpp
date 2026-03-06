#include "MtkRilAdapter.h"

#undef LOG_TAG
#define LOG_TAG "MtkRilAdapter"

namespace mtk {

MtkRilAdapter::MtkRilAdapter(const struct RIL_Env *env) : mPRilEnv(env), mLibRilReady(false) {
    // TODO Auto-generated constructor stub

}

MtkRilAdapter::~MtkRilAdapter() {
    // TODO Auto-generated destructor stub
}

/**
 * Handle request if necessary for customization.
 * @return -1 if you want to intercept this request, but make sure a requestComplete for it called.
 */
int MtkRilAdapter::onHandleRequest(int request, void */*data*/, size_t /*datalen*/, RIL_Token /*t*/,
        int /*slotId*/) {
    if (mLibRilReady == false) {
        mLibRilReady = true;
        onLibRilReady();
    }
    return request;
}

/**
 * Handle request if necessary for customization.
 * @return -1 if you want to intercept this request, but make sure a requestComplete for it called.
 */
int MtkRilAdapter::onHandleSapRequest(int request, void */*data*/, size_t /*datalen*/, RIL_Token /*t*/,
        int /*slotId*/) {
    return request;
}

int MtkRilAdapter::onHandleUnsolicitedResponse(int unsolResponse, void *data, size_t /*datalen*/,
        int /*slotId*/) {
    return unsolResponse;
}

int MtkRilAdapter::onHandleSapUnsolicitedResponse(int unsolResponse, void */*data*/, size_t /*datalen*/,
        int /*slotId*/) {
    return unsolResponse;
}

void MtkRilAdapter::onHandleSetRadioState(RIL_RadioState /*oldState*/, RIL_RadioState /*newState*/,
        RIL_SOCKET_ID /*socId*/) {

}

void MtkRilAdapter::onHandleRequestComplete(RIL_Token /*t*/, RIL_Errno /*e*/, void */*response*/,
        size_t /*responselen*/) {

}

void MtkRilAdapter::onHandleSapRequestComplete(RIL_Token /*t*/, RIL_Errno /*e*/, void */*response*/,
        size_t /*responselen*/) {

}

void MtkRilAdapter::onLibRilReady() {

}

void MtkRilAdapter::issueLocalRequest(int request, void *data, int len, RIL_SOCKET_ID socId) {
    if (mPRilEnv == NULL) {
        mtkLogE(LOG_TAG, "Can't issue local request since ril_env null");
        return;
    }
    mPRilEnv->IssueLocalRequest(request, data, len, socId);
}

void MtkRilAdapter::requestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen) {
    if (mPRilEnv == NULL) {
        mtkLogE(LOG_TAG, "Can't do requestComplete since ril_env null");
        return;
    }
    mPRilEnv->OnRequestComplete(t, e, response, responselen);
}

} /* namespace mtk */
