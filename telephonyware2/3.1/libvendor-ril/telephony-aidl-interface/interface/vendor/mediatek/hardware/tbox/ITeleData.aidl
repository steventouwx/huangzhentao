/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
package vendor.mediatek.hardware.tbox;

import vendor.mediatek.hardware.tbox.ITeleDataIndication;
import vendor.mediatek.hardware.tbox.ITeleDataResponse;
import vendor.mediatek.hardware.tbox.KeepaliveRequest;
import vendor.mediatek.hardware.tbox.KeepaliveStatus;

/**
 * This interface is used by data network framework to talk to tele-fwk.
 * All the functions have minimum one parameter:
 * serial: which corresponds to serial no. of request. Serial numbers must only be memorized for the
 * duration of a method call. If clients provide colliding serials (including passing the same
 * serial to different methods), multiple responses (one for each method call) must still be served.
 */
// Interface inherits from vendor.mediatek.hardware.tbox@1.1::ITeleData but AIDL does not support interface inheritance (methods have been flattened).
@VintfStability
interface ITeleData {
    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int deactiveDataCall(in int profileid);

    oneway void setResponseFunctions(in ITeleDataResponse dataResponseParam,
        in ITeleDataIndication dataIndicationParam);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setupDataCall(in int profileId);

    /**
     * Start a Keepalive session
     *
     * @param keepalive A request structure containing all necessary info to describe a keepalive
     *
     */
    KeepaliveStatus startKeepalive(in KeepaliveRequest keepalive);

    /**
     * Stop an ongoing Keepalive session
     *
     * @param sessionHandle The handle that was provided by StartKeepalive Response
     *
     */
    int stopKeepalive(in int sessionHandle);
}
