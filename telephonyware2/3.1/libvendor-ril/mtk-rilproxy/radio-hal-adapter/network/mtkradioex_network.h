// SPDX-License-Identifier: MediaTekProprietary
#pragma once
#include <cstdint>
#include <memory>
#include <aidl/vendor/mediatek/hardware/mtkradioex/network/BnMtkRadioExNetworkIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/network/BnMtkRadioExNetworkResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/network/IMtkRadioExNetwork.h>

#include "mtkradioex_network_response.h"
#include "mtkradioex_network_indication.h"
#include "../radio_service.h"
#include "../radio_hal_utils.h"
#include "RfxDispatchThread.h"
using namespace aidl::vendor::mediatek::hardware::mtkradioex::network;
using namespace aidl::android::hardware::radio;

class MtkRadioExNetWork : public RadioService{
private:
    // Private static instance of MtkRadioExNetWork
    //static MtkRadioExNetWork* instance;

    /* mtkradioex network service handle */
    std::shared_ptr<IMtkRadioExNetwork> mtkradioex_network;
    /* mtkradioex network response handle */
    std::shared_ptr<MtkRadioExNetworkResponse> mtkradioexRsp_network;
    /* mtkradioex network indication handle */
    std::shared_ptr<MtkRadioExNetworkIndication> mtkradioexInd_network;

public:
    // Public accessor for the instance
    //static MtkRadioExNetWork* getInstance();
    MtkRadioExNetWork(int slot, RfxDispatchThread* dispatchThread);
    // Destructor

    // Delete copy constructor and assignment operator
    //MtkRadioExNetWork(const MtkRadioExNetWork&) = delete;
    //MtkRadioExNetWork& operator=(const MtkRadioExNetWork&) = delete;

    // Member functions for the MtkRadioExNetWork can be declared here...

public:
    int32_t getAvailableNetworksWithAct(int serial, int32_t in_clientId = CLIENT_TBOX);
    int32_t getNitzTime(int serial, int32_t in_clientId = CLIENT_TBOX);
};