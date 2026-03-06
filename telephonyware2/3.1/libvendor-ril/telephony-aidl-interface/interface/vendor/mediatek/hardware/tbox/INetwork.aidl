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

import vendor.mediatek.hardware.tbox.DataCall;
import vendor.mediatek.hardware.tbox.DataCallInfoStruct;
import vendor.mediatek.hardware.tbox.INetworkCallBack;

import vendor.mediatek.hardware.tbox.parcelable_dataCallInfoGet;
import vendor.mediatek.hardware.tbox.parcelable_dataCallMarkIdGet;

/**
 * This interface is used by data network framework to talk to MtkLayer client.
 */
// Interface inherits from vendor.mediatek.hardware.tbox@1.1::INetwork but AIDL does not support interface inheritance (methods have been flattened).
@VintfStability
interface INetwork {
    // Adding return type to method instead of out param int err since there is only one return value.
    int AddURLBindnetwork(in int profile, in String strURL);

    // Adding return type to method instead of out param int err since there is only one return value.
    int RemoveURLBindnetwork(in int profile, in String strURL);

    // FIX done
    parcelable_dataCallInfoGet dataCallInfoGet(in int profile, in int ipfamily);

    // FIX done
    parcelable_dataCallMarkIdGet dataCallMarkIdGet(in int profile);

    // Adding return type to method instead of out param int err since there is only one return value.
    int deactiveDataCall(in int profile, in int ipfamily);

    // Adding return type to method instead of out param int err since there is only one return value.
    int delDataCall();

    // Adding return type to method instead of out param long tx_bytes since there is only one return value.
    long getCurNetFlowCount(in int profile_idx, in int stattype);

    // Adding return type to method instead of out param int err since there is only one return value.
    int networkSwitchToHU(in boolean enable);

    void setCallBack(in INetworkCallBack networkCb);

    // Adding return type to method instead of out param int err since there is only one return value.
    int setDnsServer(in String dns_server_ip);

    // Adding return type to method instead of out param int err since there is only one return value.
    int setRoute();

    // Adding return type to method instead of out param int err since there is only one return value.
    int setupDataCall(in DataCall dataCall);
}
