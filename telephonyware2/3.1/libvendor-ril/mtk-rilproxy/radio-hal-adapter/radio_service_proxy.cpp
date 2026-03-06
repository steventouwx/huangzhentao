// SPDX-License-Identifier: MediaTekProprietary
#include <mtk_log.h>
#include <memory>
#include <libmtkrilutils.h>
#include "radio_service_proxy.h"
#include "network/radio_network.h"
#include "network/mtkradioex_network.h"
#include "modem/radio_modem.h"
#include "modem/mtkradioex_modem.h"
#include "sim/radio_sim.h"
#include "sim/mtkradioex_sim.h"
#include "data/radio_data.h"
#include "data/mtkradioex_data.h"
#include "ims/radio_ims.h"
#include "ims/mtkradioex_ims.h"
#include "sms/radio_messaging.h"
#include "sms/mtkradioex_messaging.h"
#include "voice/radio_voice.h"
#include "voice/mtkradioex_voice.h"
#include "ecall/mtkradioex_ecall.h"
#include "config/radio_config.h"


#define LOG_TAG "RadioServiceProxy"
//std::unique_ptr<RadioServiceProxy> RadioServiceProxy::sInstance = nullptr;
//RadioServiceProxy::RadioServiceProxy(){}

RadioServiceProxy& RadioServiceProxy::getInstance()
{
    static RadioServiceProxy sInstance;
    return sInstance;
}

void RadioServiceProxy::init(RfxDispatchThread* dispatchThread)
{
    RLOGD("RadioServiceProxy init");
    int simCount = getSimCount();
    RLOGD("RadioServiceProxy init simCount=%d", simCount);
    for(int i = 0; i < simCount; ++i) {
        RLOGD("RadioServiceProxy init1, %d", i);
        mRadioNetwork[i] = std::make_shared<RadioNetWork>(i, dispatchThread);
        mMtkRadioExNetwork[i] = std::make_shared<MtkRadioExNetWork>(i, dispatchThread);
        mRadioModem[i] = std::make_shared<RadioModem>(i, dispatchThread);
        mMtkRadioExModem[i] = std::make_shared<MtkRadioExModem>(i, dispatchThread);
        mRadioSim[i] = std::make_shared<RadioSim>(i, dispatchThread);
        mMtkRadioExSim[i] = std::make_shared<MtkRadioExSim>(i, dispatchThread);
        RLOGD("RadioServiceProxy init2, %d", i);
        mRadioData[i] = std::make_shared<RadioData>(i, dispatchThread);
        mMtkRadioExData[i] = std::make_shared<MtkRadioExData>(i, dispatchThread);
        mRadioIms[i] = std::make_shared<RadioIms>(i, dispatchThread);
        mMtkRadioExIms[i] = std::make_shared<MtkRadioExIms>(i, dispatchThread);
        mRadioMessaging[i] = std::make_shared<RadioMessaging>(i, dispatchThread);
        mMtkRadioExMessaging[i] = std::make_shared<MtkRadioExMessaging>(i, dispatchThread);
        mRadioVoice[i] = std::make_shared<RadioVoice>(i, dispatchThread);
        mMtkRadioExVoice[i] = std::make_shared<MtkRadioExVoice>(i, dispatchThread);
        mMtkRadioExEcall[i] = std::make_shared<MtkRadioExEcall>(i, dispatchThread);
        mRadioConfig[i] = std::make_shared<RadioConfig>(i, dispatchThread);
        RLOGD("RadioServiceProxy init3, %d", i);
    }
    RLOGD("RadioServiceProxy init4");
    mRadios[HAL_SERVICE_NETWORK] = mRadioNetwork;
    mRadios[HAL_SERVICE_NETWORK_EX] = mMtkRadioExNetwork;
    mRadios[HAL_SERVICE_MODEM] = mRadioModem;
    mRadios[HAL_SERVICE_MODEM_EX] = mMtkRadioExModem;
    mRadios[HAL_SERVICE_SIM] = mRadioSim;
    mRadios[HAL_SERVICE_SIM_EX] = mMtkRadioExSim;
    mRadios[HAL_SERVICE_DATA] = mRadioData;
    RLOGD("RadioServiceProxy init4");
    mRadios[HAL_SERVICE_DATA_EX] = mMtkRadioExData;
    mRadios[HAL_SERVICE_IMS] = mRadioIms;
    mRadios[HAL_SERVICE_IMS_EX] = mMtkRadioExIms;
    mRadios[HAL_SERVICE_MESSAGING] = mRadioMessaging;
    mRadios[HAL_SERVICE_MESSAGING_EX] = mMtkRadioExMessaging;
    mRadios[HAL_SERVICE_VOICE] = mRadioVoice;
    mRadios[HAL_SERVICE_VOICE_EX] = mMtkRadioExVoice;
    mRadios[HAL_SERVICE_ECALL_EX] = mMtkRadioExEcall;
    mRadios[HAL_SERVICE_CONFIG] = mRadioConfig;
    RLOGD("RadioServiceProxy init end");
}

std::shared_ptr<RadioService> RadioServiceProxy::getProxy(int hal, int slot)
{
    RLOGD("getProxy hal = %d, slot = %d", hal, slot);
    if(hal < 0 || hal >= HAL_SERVICE_MAX) {
        RLOGE("invalid hal %d", hal);
        return nullptr;
    }
    int simCount = getSimCount();
    if(slot < 0 || slot >= simCount) {
        RLOGE("invalid slot %d", slot);
        return nullptr;
    }
    if (mRadios.empty()) {
        RLOGE("mRadios is null(no possbile) slot %d", slot);
        return nullptr;
    }
    return mRadios.at(hal).at(slot);
}

bool RadioServiceProxy::isAidlReady() {
    if(mRadios.find(HAL_SERVICE_CONFIG) != mRadios.end()) {
        RLOGD("isAidlReady, ready");
        return true;
    }
    RLOGD("isAidlReady, no ready");
    return false;
}