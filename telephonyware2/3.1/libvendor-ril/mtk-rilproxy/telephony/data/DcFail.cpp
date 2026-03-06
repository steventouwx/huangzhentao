// SPDX-License-Identifier: MediaTekProprietary
/*
 * DcFail.cpp
 *
 *  Created on: Apr 28, 2017
 *      Author: mtk54458
 */

#include "DcFail.h"

DcFail::DcFail() {
    DcFail::mDcFailCause = RIL_DataCallFailCause::PDP_FAIL_NONE;
}

DcFail::DcFail(int dcFailCause) {
    DcFail::mDcFailCause = RIL_DataCallFailCause(dcFailCause);
}

bool DcFail::isPermanentFail() {
    return (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_OPERATOR_BARRED)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_MISSING_UKNOWN_APN)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_UNKNOWN_PDP_ADDRESS_TYPE)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_USER_AUTHENTICATION)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_ACTIVATION_REJECT_GGSN)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_SERVICE_OPTION_NOT_SUPPORTED)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_SERVICE_OPTION_NOT_SUBSCRIBED)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_NSAPI_IN_USE)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_ONLY_IPV4_ALLOWED)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_ONLY_IPV6_ALLOWED)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_PROTOCOL_ERRORS)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_RADIO_POWER_OFF)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_TETHERED_CALL_ACTIVE)
//            || (DcFail::mDcFailCause == RIL_DataCallFailCause::RADIO_NOT_AVAILABLE)
//            || (DcFail::mDcFailCause == RIL_DataCallFailCause::UNACCEPTABLE_NETWORK_PARAMETER)
            || (DcFail::mDcFailCause == RIL_DataCallFailCause::PDP_FAIL_SIGNAL_LOST);
}
