// SPDX-License-Identifier: MediaTekProprietary
/*
 * Common.cpp
 *
 *  Created on: May 4, 2017
 *      Author: mtk54458
 */

#include "Common.h"


//Fail cause related
const string NOT_ATTACHED = " - Not attached";
const string APN_NOT_READY = " - APN not ready";
const string ROAMING_DISABLED = " - Roaming and data roaming not enabled";
const string INVALID_PHONE_STATE = " - PhoneState is not idle";
const string CONCURRENT_VOICE_DATA_NOT_ALLOWED = " - Concurrent voice and data not allowed";
const string PS_RESTRICTED = " - mIsPsRestricted = true";
const string UNDESIRED_POWER_STATE = " - desiredPowerState = false";

const string REASON_DATA_ATTACHED = "dataAttached";
const string REASON_CALL_END = "call-End";
const string REASON_APN_READY = "apn-ready";
const string REASON_PS_RESTRICT = "ps-not-restrict";

//DataConnection related
const int DC_INVALID_ID = -1;
const int MIN_ID_HIGH_TROUGHPUT = 0;
const int MAX_ID_HIGH_TROUGHPUT = 1;
const int MIN_ID_OTHERS_TROUGHPUT = 2;
const int MAX_ID_OTHERS_TROUGHPUT = 3;

//DataCallResponse related
const int INVALID_NET_ID = -1;
const int INVALID_CID = -1;
const int INVALID_MTU = -1;

//RetryManager related
const string DEFAULT_DATA_RETRY_CONFIG = "default_randomization=2000,5000,10000,20000,40000,80000:5000,160000:5000,320000:5000,640000:5000,1280000:5000,1800000:5000";
const string OTHERS_DATA_RETRY_CONFIG = "max_retries=3, 5000, 5000, 5000";
const long DEFAULT_INTER_APN_DELAY = 20000;
const long NO_RETRY = -1;
const long NO_SUGGESTED_RETRY_DELAY = -2;
const int MAX_SAME_APN_RETRY = 3;

