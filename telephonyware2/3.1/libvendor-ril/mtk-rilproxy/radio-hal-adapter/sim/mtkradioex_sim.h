// SPDX-License-Identifier: MediaTekProprietary
#include <memory>
#include <cstdint>
#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/BnMtkRadioExSimIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/BnMtkRadioExSimResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/IMtkRadioExSim.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rsu/RsuRequest.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rsu/RsuRequestInfo.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rsu/RsuResponseInfo.h>
#include <aidl/android/hardware/radio/sim/BnRadioSimIndication.h>
#include <aidl/android/hardware/radio/sim/BnRadioSimResponse.h>
#include <aidl/android/hardware/radio/sim/IRadioSim.h>

#include "RfxDispatchThread.h"
#include "mtkradioex_sim_response.h"
#include "mtkradioex_sim_indication.h"
#include "../radio_service.h"
#include "../radio_hal_utils.h"

using namespace aidl::vendor::mediatek::hardware::mtkradioex::sim;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::rsu;
using namespace aidl::android::hardware::radio::sim;

class MtkRadioExSim : public RadioService {
private:
    /* data */
public:
    /* mtkradioex SIM service handle */
    std::shared_ptr<IMtkRadioExSim> mtkradioex_sim;
    /* mtkradioex SIM response handle */
    std::shared_ptr<MtkRadioExSimResponse> mtkradioexRsp_sim;
    /* mtkradioex SIM indication handle */
    std::shared_ptr<MtkRadioExSimIndication> mtkradioexInd_sim;
public:
     MtkRadioExSim(int slot, RfxDispatchThread* dispatchThread);
public:
    //TDB: need modify to CLIENT_TBOX
    int32_t getIccid(int32_t in_serial, int32_t in_clientId = CLIENT_TBOX);
};