/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2024. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */


/*****************************************************************************
 * Include
 *****************************************************************************/
#include <ril_adapter/RadioDataAdapater.h>
#include <bitset>
#include <limits.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex>
#include <rilAidlUtils.h>
#include <mtk_properties.h>
#include <mutex>

#include <aidl/android/hardware/radio/data/BnRadioData.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/BnMtkRadioExData.h>

/*****************************************************************************
 * Define
 *****************************************************************************/
#define LOG_TAG "RadioDataAdapater"

/*****************************************************************************
 * Class RadioDataAdapater
 *****************************************************************************/

namespace aidl_radio = ::aidl::android::hardware::radio;

namespace aidl_radio_data = ::aidl::android::hardware::radio::data;
namespace aidl_mtk_radio_data = ::aidl::vendor::mediatek::hardware::mtkradioex::data;

#define SetupDataCallResult aidl_radio_data::SetupDataCallResult
#define SetupDataCallResultSlice aidl_mtk_radio_data::SetupDataCallResultSlice
#define LinkAddress aidl_radio_data::LinkAddress

using rilAidlUtils::retChecker;

struct RadioDataAdapaterImpl {
    std::shared_ptr<aidl_mtk_radio_data::IMtkRadioExDataResponse> mtkDataResponse;
    std::shared_ptr<aidl_mtk_radio_data::IMtkRadioExDataIndication> mtkDataIndication;
    std::shared_ptr<aidl_radio_data::IRadioDataResponse> dataResponse;
    std::shared_ptr<aidl_radio_data::IRadioDataIndication> dataIndication;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::assist::IAssistRadioResponse> mtkAssistDataResponse;
};

struct NetworkMapInfo{
    int profileId;
    std::string ipv4Address;
    std::string ipv4Gateway;
    std::string ifname;
};

struct RilData{
    int serial;
    int slotId;
    void* setupDataCallInfo = nullptr;
    bool setupDataDone;
    void* setupDataResult = nullptr;
    int responseType;
    int tdType;
};

struct Node {
    RilData* ril_data; // node data
    Node* next; // next point
};

static Node* head = nullptr;
static std::mutex rilData_mutex;

void addDataToNodeList(RilData* ril_Data) {
    Node* newNode = new Node{ril_Data, nullptr};
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

struct RilData* findRilDataBySerial(int serial) {
    mtkLogI(LOG_TAG, "%s: serial = %d ", __FUNCTION__, serial);
    Node* current = head;
    while (current != nullptr) {
        RilData* ril_data = current->ril_data;
        if (ril_data != nullptr) {
            RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)(ril_data->setupDataCallInfo);
            if (pReq != nullptr) {
                if (ril_data->serial == serial && ril_data->setupDataResult == nullptr) {
                    mtkLogI(LOG_TAG, "%s: found ril data of serial(%d), profileId(%d)",
                        __FUNCTION__, ril_data->serial, pReq->dataProfileInfo.profileId);
                    return ril_data;
                }
            } else {
                mtkLogW(LOG_TAG, "%s: ril_data(%d)->setupDataCallInfo is nullptr", __FUNCTION__, serial);
            }
        }
        current = current->next;
    }
    return nullptr;
}

struct RilData* findRilDataByProfileId(int profileId) {
    mtkLogI(LOG_TAG, "%s: profileId = %d ", __FUNCTION__, profileId);
    Node* current = head;
    while (current != nullptr) {
        RilData* ril_data = current->ril_data;
        if (ril_data != nullptr) {
            RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)(ril_data->setupDataCallInfo);
            if (pReq != nullptr) {
                if (pReq->dataProfileInfo.profileId == profileId) {
                    mtkLogI(LOG_TAG, "%s: found ril data of serial(%d), profileId(%d)",
                        __FUNCTION__, ril_data->serial, pReq->dataProfileInfo.profileId);
                    return ril_data;
                }
            } else {
                mtkLogW(LOG_TAG, "%s: ril_data->setupDataCallInfo of profileId(%d) is nullptr", __FUNCTION__, profileId);
            }
        }
        current = current->next;
    }
    return nullptr;
}

struct RilData* findRilDataByCid(int cid) {
    mtkLogI(LOG_TAG, "%s: cid = %d ", __FUNCTION__, cid);
    Node* current = head;
    while (current != nullptr) {
        RilData* ril_data = current->ril_data;
        mtkLogI(LOG_TAG, "1-: %s ", __FUNCTION__);
        if (ril_data != nullptr) {
            SetupDataCallResult *pReqResult = (SetupDataCallResult*)(ril_data->setupDataResult);
            mtkLogI(LOG_TAG, "2-: %s, ril_data->serial: %d ", __FUNCTION__, ril_data->serial);
            mtkLogI(LOG_TAG, "2-1: %s ", __FUNCTION__);
            if (pReqResult != nullptr) {
                mtkLogI(LOG_TAG, "3-1: %s ", __FUNCTION__);
                mtkLogI(LOG_TAG, "3-1: %s, pReqResult->cid: %d ", __FUNCTION__, pReqResult->cid);
                if (pReqResult->cid == cid) {
                    mtkLogI(LOG_TAG, "3-2: %s ", __FUNCTION__);
                    return ril_data;
                }
            }
        }
        mtkLogI(LOG_TAG, "4-1: %s ", __FUNCTION__);
        current = current->next;
    }
    mtkLogI(LOG_TAG, "5-1: %s ", __FUNCTION__);
    return nullptr;
}

bool DeleteNodeDataBySerial(int serial) {
    mtkLogI(LOG_TAG, "%s: serial = %d ", __FUNCTION__, serial);
    Node* current = head;
    Node* previous = nullptr;
    while (current != nullptr) {
        RilData* ril_data = current->ril_data;
        if (ril_data != nullptr) {
            if (ril_data->serial == serial) {
                if (previous == nullptr) {
                    head = current->next;
                } else {
                    previous->next = current->next;
                }
                if (ril_data->setupDataCallInfo != nullptr) {
                    delete ril_data->setupDataCallInfo;
                    ril_data->setupDataCallInfo = nullptr;
                }

                if (ril_data->setupDataCallInfo != nullptr) {
                    delete ril_data->setupDataResult;
                    ril_data->setupDataResult = nullptr;
                }
                delete ril_data;
                ril_data = nullptr;

                delete current;
                current  = nullptr;

                return true;
            }
        }
        previous = current;
        current = current->next;
    }
    return false; // not find data
}

bool DeleteNodeDataByCid(int cid) {
    mtkLogI(LOG_TAG, "%s: cid = %d ", __FUNCTION__, cid);
    Node* current = head;
    Node* previous = nullptr;
    while (current != nullptr) {
        RilData* ril_data = current->ril_data;
        if (ril_data != nullptr) {
            SetupDataCallResult *pReqResult = (SetupDataCallResult*)(ril_data->setupDataResult);
            if (pReqResult != nullptr) {
                if (pReqResult->cid == cid) {
                    if (previous == nullptr) {
                        head = current->next;
                    } else {
                        previous->next = current->next;
                    }

                    if (ril_data->setupDataCallInfo != nullptr) {
                        delete ril_data->setupDataCallInfo;
                        ril_data->setupDataCallInfo = nullptr;
                    }

                    if (ril_data->setupDataResult != nullptr) {
                        delete ril_data->setupDataResult;
                        ril_data->setupDataResult = nullptr;
                    }
                    delete ril_data;
                    ril_data = nullptr;

                    delete current;
                    current  = nullptr;

                    return true;
                }
            }
        }
        previous = current;
        current = current->next;
    }
    return false; // not find data
}

void clear() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        if (temp->ril_data->setupDataCallInfo != nullptr) {
            delete temp->ril_data->setupDataCallInfo;
            temp->ril_data->setupDataCallInfo = nullptr;
        }
        if (temp->ril_data->setupDataResult != nullptr) {
            delete temp->ril_data->setupDataResult;
            temp->ril_data->setupDataResult = nullptr;
        }
        delete temp;
        temp = nullptr;
    }
}

// Network map info  for android
static NetworkMapInfo networkInfos[] = {
    {RIL_DATA_PROFILE_DEFAULT, "172.17.0.2/24", "172.17.0.1", "eth1"},
    {RIL_DATA_PROFILE_VENDOR_RCS, "172.17.1.2/24", "172.17.1.1", "eth2"},
    {RIL_DATA_PROFILE_VENDOR_BIP, "172.17.2.2/24", "172.17.2.1", "eth3"}
};

//
const NetworkMapInfo* getNetworkInfoById(int profileId) {
    mtkLogI(LOG_TAG, "%s: profileId = %d ", __FUNCTION__, profileId);
    int size = sizeof(networkInfos)/sizeof(networkInfos[0]);
    for (int i = 0; i < size; ++i) {
        if (networkInfos[i].profileId == profileId) {
            mtkLogI(LOG_TAG, "%s: found networkInfos of profileId(%d), ipv4Address(%s), ipv4Gateway(%s), ifname(%s), index(%d)",
                __FUNCTION__, networkInfos[i].profileId, networkInfos[i].ipv4Address.c_str(),
                networkInfos[i].ipv4Gateway.c_str(), networkInfos[i].ifname.c_str(), i);
            return &(networkInfos[i]);
        }
    }
    return nullptr;
}

bool isValidIPv4(const std::string& address) {
    std::regex ipv4Pattern("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    return std::regex_match(address, ipv4Pattern);
}

bool isValidIPv6(const std::string& str) {
    std::vector<std::string> parts;
    std::string part;
    std::istringstream ss(str);
    while (std::getline(ss, part, ':')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }

    if (parts.size() != 8) {
        return false;
    }

    for (const auto& p : parts) {
        if (p.empty() || p.size() > 4) {
            return false;
        }
        for (char c : p) {
            if (!std::isxdigit(c)) {
                return false;
            }
        }
    }
    return true;
}

// Function to increment the last segment of an IPv6 address by a specified value
std::string incrementIPv6String(const std::string& address, int value) {
    std::vector<std::string> segments;
    std::stringstream ss(address);
    std::string segment;

    // Extract the string array of the IPv6 address
    while (std::getline(ss, segment, ':')) {
        segments.push_back(segment);
        mtkLogI(LOG_TAG, "%s , segment: %s", __FUNCTION__, segment.c_str());
    }

    // Check if the last segment is a valid hexadecimal number
    try {
        int lastSegment = std::stoi(segments.back(), 0, 16) + value;
        std::stringstream ssLastSegment;
        ssLastSegment << std::hex << lastSegment;
        segments.back() = ssLastSegment.str();
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid IPv6 address segment: " << segments.back() << std::endl;
        mtkLogE(LOG_TAG, "%s , Invalid IPv6 address segment: %s", __FUNCTION__, segments.back().c_str());
        return ""; // Return an empty string to indicate error
    }

    mtkLogI(LOG_TAG, "%s , -1", __FUNCTION__);
    // Reconstruct the IPv6 address
    std::stringstream result;
    for (const auto& seg : segments) {
        if (!seg.empty()) {
            //result << std::hex << std::setw(4) << std::setfill('0') << std::stoi(seg, 0, 16) << ":";
            result << std::hex << std::stoi(seg, 0, 16) << ":";
        } else {
            result << ":";
        }
    }
    mtkLogI(LOG_TAG, "%s , -2", __FUNCTION__);
    std::string newIPv6 = result.str();
    mtkLogI(LOG_TAG, "%s , newIPv6: %s", __FUNCTION__, newIPv6.c_str());
    newIPv6.pop_back(); // Remove the extra colon at the end

    return newIPv6;
}

void mapResultRouteInfo(void *result, int profileId) {
    mtkLogI(LOG_TAG, "%s: profileId = %d ", __FUNCTION__, profileId);
    if (result == nullptr) {
        mtkLogE(LOG_TAG, "result is nullptr");
        return;
    }

    SetupDataCallResult * dataCallResult = (SetupDataCallResult *) result;
    if (dataCallResult == nullptr) {
        mtkLogE(LOG_TAG, "dataCallResult is nullptr");
        return;
    }

    const NetworkMapInfo* networkInfo = getNetworkInfoById(profileId);
    if (networkInfo == nullptr) {
        mtkLogE(LOG_TAG, "networkInfo is nullptr");
        return;
    }

    mtkLogI(LOG_TAG, "mapResultRouteInfo() - addresses for profileId(%d)", profileId);
    int i = 0;
    for (const auto& addr : dataCallResult->addresses){
        mtkLogI(LOG_TAG, "mapResultRouteInfo() - raw address: %s", dataCallResult->addresses[i].address.c_str());
        std::string address = addr.address;
        std::string subTailStr = "";
        size_t pos = address.find('/');
        if (pos != std::string::npos) {
            subTailStr = address.substr(pos);
            address = address.substr(0, pos);
        }

        mtkLogI(LOG_TAG, "mapResultRouteInfo() - address: %s", address.c_str());
        if (isValidIPv4(address)) {
            //dataCallResult->addresses[i].address = networkInfo->ipv4Address + subTailStr;
            dataCallResult->addresses[i].address = networkInfo->ipv4Address;
            dataCallResult->gateways[i] =  networkInfo->ipv4Gateway;
            mtkLogI(LOG_TAG, "IPV4 map address: %s", dataCallResult->addresses[i].address.c_str());
            mtkLogI(LOG_TAG, "IPV4 map gateways: %s", dataCallResult->gateways[i].c_str());
        } else {
            std::string address1 = incrementIPv6String(address, 2);
            mtkLogI(LOG_TAG, "IPV6 map address1: %s", address1.c_str());
            dataCallResult->addresses[i].address = address1 + subTailStr;
            if (!(dataCallResult->gateways[i].empty())) {
                dataCallResult->gateways[i] = incrementIPv6String(address, 1);
            }
            mtkLogI(LOG_TAG, "IPV6 map address: %s", dataCallResult->addresses[i].address.c_str());
            mtkLogI(LOG_TAG, "IPV6 map gateways: %s", dataCallResult->gateways[i].c_str());
        }
        i++;
    }
    dataCallResult->ifname = networkInfo->ifname;
}

void mapResultSliceRouteInfo(void *result, int profileId) {
    mtkLogI(LOG_TAG, "%s: profileId = %d ", __FUNCTION__, profileId);
    if (result == nullptr) {
        mtkLogE(LOG_TAG, "result is nullptr");
        return;
    }

    SetupDataCallResultSlice * dataCallResultSlice = (SetupDataCallResultSlice *) result;
    if (dataCallResultSlice == nullptr) {
        mtkLogE(LOG_TAG, "mapResultSliceRouteInfo is nullptr");
        return;
    }

    const NetworkMapInfo* networkInfo = getNetworkInfoById(profileId);
    if (networkInfo == nullptr) {
        mtkLogE(LOG_TAG, "networkInfo is nullptr");
        return;
    }

    mtkLogI(LOG_TAG, "mapResultSliceRouteInfo() - addresses for profileId(%d)", profileId);
    int i = 0;
    for (const auto& addr : dataCallResultSlice->addresses){
        mtkLogI(LOG_TAG, "mapResultSliceRouteInfo() - raw address: %s", dataCallResultSlice->addresses[i].address.c_str());
        std::string address = addr.address;
        std::string subTailStr = "";
        size_t pos = address.find('/');
        if (pos != std::string::npos) {
            subTailStr = address.substr(pos);
            address = address.substr(0, pos);
        }

        mtkLogI(LOG_TAG, "mapResultSliceRouteInfo() - address: %s", address.c_str());
        if (isValidIPv4(address)) {
            //dataCallResultSlice->addresses[i].address = networkInfo->ipv4Address + subTailStr;
            dataCallResultSlice->addresses[i].address = networkInfo->ipv4Address;
            dataCallResultSlice->gateways[i] =  networkInfo->ipv4Gateway;
            mtkLogI(LOG_TAG, "IPV4 map address: %s", dataCallResultSlice->addresses[i].address.c_str());
            mtkLogI(LOG_TAG, "IPV4 map gateways: %s", dataCallResultSlice->gateways[i].c_str());
        } else {
            std::string address1 = incrementIPv6String(address, 2);
            dataCallResultSlice->addresses[i].address = address1 + subTailStr;
            if (!(dataCallResultSlice->gateways[i].empty())) {
                dataCallResultSlice->gateways[i] = incrementIPv6String(address, 1);
            }
            mtkLogI(LOG_TAG, "IPV6 map address: %s", dataCallResultSlice->addresses[i].address.c_str());
            mtkLogI(LOG_TAG, "IPV6 map gateways: %s", dataCallResultSlice->gateways[i].c_str());
        }
        i++;
    }

    dataCallResultSlice->ifname = networkInfo->ifname;

}

RadioDataAdapater::RadioDataAdapater() {

}

RadioDataAdapater::~RadioDataAdapater() {
    clear();
}

bool RadioDataAdapater::setupDataCallEx(int serial, void *setupData, size_t datalen, void *radioDataServiceImpl, int slotId) {
    mtkLogI(LOG_TAG, "%s: slotId = %d, serial = %d", __FUNCTION__, slotId, serial);

    android::ClientId client = getClientBySlot(slotId);
    if (client == android::CLIENT_RILJ) {
        mtkLogI(LOG_TAG, "%s: slotId=%d, client=%d, set vendor.ril.android.setupdatacall to 1", __FUNCTION__, slotId, client);
        mtk_property_set("vendor.ril.android.setupdatacall", "1");
    }

    RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)setupData;
    //RilData* ril_data = findRilDataBySerial(serial);
    int profileId = pReq->dataProfileInfo.profileId;
    RilData* ril_data = findRilDataByProfileId(profileId);
    //add new handling rilsetupdata info
    if (ril_data == nullptr) {
        RilData *ril_new_data = new RilData();
        memset(ril_new_data, 0, sizeof(struct RilData));
        ril_new_data->serial = serial;
        ril_new_data->setupDataDone = false;
        ril_new_data->setupDataCallInfo = new RIL_SetupDataCallRequest(*pReq);
        ril_new_data->slotId = slotId;
        ril_new_data->setupDataResult = nullptr;
        ril_new_data->responseType = -1;
        ril_new_data->tdType = -1;
        mtkLogI(LOG_TAG, "%s: add new ril data for profileId(%d), ril_data(serial:%d)",
            __FUNCTION__, profileId, ril_new_data->serial);
        addDataToNodeList(ril_new_data);
        return true;
    } else {
        if (((RadioDataAdapaterImpl*)radioDataServiceImpl)->dataResponse == NULL) {
            mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d]->dataResponse == NULL", __FUNCTION__, slotId);
            return false;
        }
        mtkLogI(LOG_TAG, "%s: had found the established datacall for profileId(%d), ril_data(serial:%d)->setupDataDone=%s",
            __FUNCTION__, profileId, ril_data->serial, ril_data->setupDataDone == true ? "true" : "false");
        //If this PDN has been created before using the same profileid,  and call directly setupDataCallResponse  to Client by using this created PDN info.
        if (ril_data->setupDataDone) {
            SetupDataCallResult *result = (SetupDataCallResult *)(ril_data->setupDataResult);
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, ril_data->responseType, RIL_E_SUCCESS);
            if (client == android::CLIENT_RILJ && result != nullptr) {   // return directly original result to rilJ
                SetupDataCallResult setupDataDoneResult = *result;
                //CopyDataResult(setupDataDoneResult, result, slotId);
                mapResultRouteInfo(&setupDataDoneResult, profileId);
                ndk::ScopedAStatus ret = ((RadioDataAdapaterImpl*)radioDataServiceImpl)->dataResponse->setupDataCallResponse(
                        responseInfo, setupDataDoneResult);
                if (!ret.isOk()) {
                     mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
                     retChecker->aospAidlReturnStatusChecker(slotId, ret);
                 }
            } else if(client == android::CLIENT_TBOX && result != nullptr) {   //return directly map result to tbox
                ndk::ScopedAStatus ret = ((RadioDataAdapaterImpl*)radioDataServiceImpl)->dataResponse->setupDataCallResponse(
                        responseInfo, *result);
                if (!ret.isOk()) {
                     mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
                     retChecker->aospAidlReturnStatusChecker(slotId, ret);
                }
            }
        } else {
            mtkLogE(LOG_TAG, "%s: ril_data(serial:%d)->setupDataDone=%s", __FUNCTION__, ril_data->serial,
                ril_data->setupDataDone == true ? "true" : "false");
        }
        return false;
    }
}

bool RadioDataAdapater::setupDataCallSliceEx(int serial, void *setupData, size_t datalen, void *radioDataServiceImpl, int slotId) {
    mtkLogI(LOG_TAG, "%s: slotId = %d, serial = %d", __FUNCTION__, slotId, serial);
    android::ClientId client = getClientBySlot(slotId);
    RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)setupData;
    //RilData* ril_data = findRilDataBySerial(serial);
    int profileId = pReq->dataProfileInfo.profileId;
    RilData* ril_data = findRilDataByProfileId(profileId);
    //add new handling rilsetupdata info
    if (ril_data == nullptr) {
        RilData *ril_new_data = new RilData();
        memset(ril_new_data, 0, sizeof(struct RilData));
        ril_new_data->serial = serial;
        ril_new_data->setupDataDone = false;
        ril_new_data->setupDataCallInfo = new RIL_SetupDataCallRequest(*pReq);
        ril_new_data->slotId = slotId;
        ril_new_data->setupDataResult = nullptr;
        ril_new_data->responseType = -1;
        ril_new_data->tdType = -1;
        mtkLogI(LOG_TAG, "%s: add new ril data for profileId(%d), ril_data(serial:%d)",
            __FUNCTION__, profileId, ril_new_data->serial);
        addDataToNodeList(ril_new_data);
        return true;
    } else {
        if (radioDataServiceImpl == nullptr || ((RadioDataAdapaterImpl*)radioDataServiceImpl)->dataResponse == NULL) {
            mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d]->dataResponse == NULL", __FUNCTION__, slotId);
            return false;
        }
        mtkLogI(LOG_TAG, "%s: had found the established datacall for profileId(%d), ril_data(serial:%d)->setupDataDone=%s",
            __FUNCTION__, profileId, ril_data->serial, ril_data->setupDataDone == true ? "true" : "false");
        //If this PDN has been created before using the same profileid,  and call directly setupDataCallResponse  to Client by using this created PDN info.
        if (ril_data->setupDataDone) {

            if (ril_data->tdType == SETUP_DATA_CALL_RESPONSE_AOSP) {  // AOSP setupDataCallResponse
                SetupDataCallResult *result = (SetupDataCallResult *)(ril_data->setupDataResult);
                aidl_radio::RadioResponseInfo responseInfo = {};
                rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, ril_data->responseType, RIL_E_SUCCESS);
                if (client == android::CLIENT_RILJ && result != nullptr) {   // return directly original result to rilJ
                    SetupDataCallResult setupDataDoneResult = *result;
                    //CopyDataResult(setupDataDoneResult, result, slotId);
                    mapResultRouteInfo(&setupDataDoneResult, profileId);
                    ndk::ScopedAStatus ret = ((RadioDataAdapaterImpl*)radioDataServiceImpl)->dataResponse->setupDataCallResponse(
                            responseInfo, setupDataDoneResult);
                    if (!ret.isOk()) {
                         mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
                         //retChecker->aospAidlReturnStatusChecker(slotId, ret);
                     }
                } else if(client == android::CLIENT_TBOX && result != nullptr) {   //return directly map result to tbox
                    ndk::ScopedAStatus ret = ((RadioDataAdapaterImpl*)radioDataServiceImpl)->dataResponse->setupDataCallResponse(
                            responseInfo, *result);
                    if (!ret.isOk()) {
                         mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
                         //retChecker->aospAidlReturnStatusChecker(slotId, ret);
                    }
                }
            } else { // customized setupDataCallResponseSlice
                SetupDataCallResultSlice *result = (SetupDataCallResultSlice *)(ril_data->setupDataResult);
                aidl_radio::RadioResponseInfo responseInfo = {};
                rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, ril_data->responseType, RIL_E_SUCCESS);
                if (client == android::CLIENT_RILJ && result != nullptr) {   // return directly original result to rilJ
                    SetupDataCallResultSlice setupDataDoneResultSlice = *result;
                    //CopyDataResultSlice(setupDataDoneResultSlice, result, slotId);
                    mapResultSliceRouteInfo(&setupDataDoneResultSlice, profileId);
                    ndk::ScopedAStatus ret = ((RadioDataAdapaterImpl*)radioDataServiceImpl)->mtkDataResponse->setupDataCallResponseSlice(
                            responseInfo, setupDataDoneResultSlice);
                    if (!ret.isOk()) {
                         mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
                         //retChecker->aospAidlReturnStatusChecker(slotId, ret);
                     }
                } else if(client == android::CLIENT_TBOX && result != nullptr) {   //return directly map result to tbox
                    ndk::ScopedAStatus ret = ((RadioDataAdapaterImpl*)radioDataServiceImpl)->mtkDataResponse->setupDataCallResponseSlice(
                            responseInfo, *result);
                    if (!ret.isOk()) {
                         mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
                         //retChecker->aospAidlReturnStatusChecker(slotId, ret);
                    }
                }
            }
        } else {
            mtkLogE(LOG_TAG, "%s: ril_data(serial:%d)->setupDataDone=%s", __FUNCTION__, ril_data->serial,
                ril_data->setupDataDone == true ? "true" : "false");
        }
        return false;
    }
}

void RadioDataAdapater::setupDataCallResponseEx(int serial, void *result, int responseType, int tdType, int slotId) {
    mtkLogI(LOG_TAG, "%s: serial = %d, slotId = %d ", __FUNCTION__, serial, slotId);
    android::ClientId client = getClientBySlot(slotId);
    RilData* ril_data = findRilDataBySerial(serial);

    // find handling setup datacall
    if (ril_data != nullptr && result != nullptr) {
        //CopyDataResult(setupDataDoneResult, result, slotId);
        SetupDataCallResult setupDataDoneResult = *((SetupDataCallResult*)result);
        mtkLogI(LOG_TAG, "%s: 1-slotId = %d ", __FUNCTION__, slotId);
        RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)(ril_data->setupDataCallInfo);
        mtkLogI(LOG_TAG, "%s: 2-slotId = %d ", __FUNCTION__, slotId);
        if (setupDataDoneResult.cause == aidl_radio_data::DataCallFailCause::NONE) {
            //setup data success, save setupdata info in nodelist
            ril_data->setupDataDone = true;
            ril_data->setupDataResult = new SetupDataCallResult(setupDataDoneResult);
            ril_data->responseType = responseType;
            ril_data->tdType = tdType;

            mtkLogI(LOG_TAG, "%s: 3-slotId = %d ", __FUNCTION__, slotId);
            // setup data success, set cid
            std::string tempCid = std::to_string(setupDataDoneResult.cid);
            const char* strCid = tempCid.c_str();

            mtkLogI(LOG_TAG, "%s: 4-slotId = %d, cid(%d)", __FUNCTION__, slotId, setupDataDoneResult.cid);
            if (pReq != nullptr) {
                mtkLogI(LOG_TAG, "%s: profileId(%d)", __FUNCTION__, pReq->dataProfileInfo.profileId);
                if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_DEFAULT) {
                    mtk_property_set("vendor.ril.default.profile.cid", strCid);
                } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_RCS) {
                    mtk_property_set("vendor.ril.private1.profile.cid", strCid);
                } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_BIP) {
                    mtk_property_set("vendor.ril.private2.profile.cid", strCid);
                } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_IMS) {
                    mtk_property_set("vendor.ril.ims.profile.cid", strCid);
                }
            }
        } else {
            //setupdata fail, so remove setupdata info from nodelist
            DeleteNodeDataBySerial(serial);
        }

        //if reponse to RILJ, must map route
        if (client == android::CLIENT_RILJ) {
            mapResultRouteInfo(result, pReq->dataProfileInfo.profileId);
        }
    }

}

void RadioDataAdapater::setupDataCallSliceResponseEx(int serial, void *result, int responseType, int tdType, int slotId) {
    mtkLogI(LOG_TAG, "%s: slotId = %d ", __FUNCTION__, slotId);
    android::ClientId client = getClientBySlot(slotId);
    RilData* ril_data = findRilDataBySerial(serial);
    SetupDataCallResult setupDataDoneResult = {};
    SetupDataCallResultSlice setupDataDoneResultSlice = *((SetupDataCallResultSlice*)result);

    // find handling setup datacall
    if (ril_data != nullptr && result != nullptr) {
        RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)(ril_data->setupDataCallInfo);

        if (tdType == SETUP_DATA_CALL_RESPONSE_AOSP) {
            //CopyDataResult(setupDataDoneResult, result, slotId);
            if (setupDataDoneResult.cause == aidl_radio_data::DataCallFailCause::NONE) {
                // setup data success, save setupdataSlice info in nodelist
                ril_data->setupDataDone = true;
                ril_data->responseType = responseType;
                ril_data->tdType = tdType;
                ril_data->setupDataResult = new SetupDataCallResult(setupDataDoneResult);

                // setup data success, set cid
                std::string tempCid = std::to_string(setupDataDoneResult.cid);
                const char* strCid = tempCid.c_str();
                mtkLogI(LOG_TAG, "%s(AOSP response): cid(%d)", __FUNCTION__, slotId, setupDataDoneResult.cid);
                if (pReq != nullptr) {
                    mtkLogI(LOG_TAG, "%s(AOSP response): profileId(%d)", __FUNCTION__, pReq->dataProfileInfo.profileId);
                    if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_DEFAULT) {
                        mtk_property_set("vendor.ril.default.profile.cid", strCid);
                    } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_RCS) {
                        mtk_property_set("vendor.ril.private1.profile.cid", strCid);
                    } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_BIP) {
                        mtk_property_set("vendor.ril.private2.profile.cid", strCid);
                    } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_IMS) {
                        mtk_property_set("vendor.ril.ims.profile.cid", strCid);
                    }
                }
            } else {
                 //setupdata fail, so remove setupdata info from nodelist
                 DeleteNodeDataBySerial(serial);
            }
        } else {
            //CopyDataResultSlice(setupDataDoneResultSlice, result, slotId);
            if (setupDataDoneResultSlice.cause == aidl_radio_data::DataCallFailCause::NONE) {
                // setup data success, save setupdataSlice info in nodelist
                ril_data->setupDataDone = true;
                ril_data->responseType = responseType;
                ril_data->tdType = tdType;
                ril_data->setupDataResult = new SetupDataCallResultSlice(setupDataDoneResultSlice);

                std::string tempCid = std::to_string(setupDataDoneResultSlice.cid);
                const char* strCid = tempCid.c_str();
                mtkLogI(LOG_TAG, "%s(yocto response): cid(%d)", __FUNCTION__, slotId, setupDataDoneResult.cid);
                if (pReq != nullptr) {
                    mtkLogI(LOG_TAG, "%s(yocto response): profileId(%d)", __FUNCTION__, pReq->dataProfileInfo.profileId);
                    if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_DEFAULT) {
                        mtk_property_set("vendor.ril.default.profile.cid", strCid);
                    } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_RCS) {
                        mtk_property_set("vendor.ril.private1.profile.cid", strCid);
                    } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_BIP) {
                        mtk_property_set("vendor.ril.private2.profile.cid", strCid);
                    } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_IMS) {
                        mtk_property_set("vendor.ril.ims.profile.cid", strCid);
                    }
                }

            } else {
                 //setupdata fail, so remove setupdata info from nodelist
                 DeleteNodeDataBySerial(serial);
            }
        }
        //if reponse to RILJ, must map route
        if (client == android::CLIENT_RILJ && pReq != nullptr) {
            mapResultSliceRouteInfo(result, pReq->dataProfileInfo.profileId);
        }
    }
}

void RadioDataAdapater::getDataCallListResponseEx(void* resultList, int size, int slotId) {
    mtkLogI(LOG_TAG, "%s: slotId = %d ", __FUNCTION__, slotId);
    if (resultList == nullptr) {
        mtkLogE(LOG_TAG, "resultList is nullptr");
        return;
    }

    SetupDataCallResult* setupDataResultList = static_cast<SetupDataCallResult*>(resultList);
    if (setupDataResultList == nullptr) {
        mtkLogE(LOG_TAG, "setupDataResultList cast failed, it is nullptr");
        return;
    }

    android::ClientId client = getClientBySlot(slotId);
    for (size_t i = 0; i < size; ++i) {
        mtkLogI(LOG_TAG, "%s: 1-slotId = %d ", __FUNCTION__, slotId);
        RilData* ril_data = findRilDataByCid(setupDataResultList[i].cid);
        if (ril_data == nullptr) {
            mtkLogE(LOG_TAG, "ril_data is nullptr");
            continue;
        }

        mtkLogI(LOG_TAG, "%s: 1-1-slotId = %d ", __FUNCTION__, slotId);
        RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)(ril_data->setupDataCallInfo);
        mtkLogI(LOG_TAG, "%s: 1-2-slotId = %d ", __FUNCTION__, slotId);
        if (pReq == nullptr) {
            mtkLogE(LOG_TAG, "SetupDataCallInformation is nullptr for CID: %d", setupDataResultList[i].cid);
            continue;
        }

        mtkLogI(LOG_TAG, "%s: 2-slotId = %d ", __FUNCTION__, slotId);
        if (ril_data->tdType == SETUP_DATA_CALL_RESPONSE_AOSP && client == android::CLIENT_RILJ) {
            mapResultRouteInfo(&(setupDataResultList[i]), pReq->dataProfileInfo.profileId);
        } else if(ril_data->tdType == SETUP_DATA_CALL_RESPONSE_EXT && client == android::CLIENT_RILJ) {
            mapResultSliceRouteInfo(&(setupDataResultList[i]), pReq->dataProfileInfo.profileId);
        }
        mtkLogI(LOG_TAG, "%s: 3-slotId = %d ", __FUNCTION__, slotId);

        // data disconnect, remove Node Rildata for node list.
        // response.getLinkStatus() == DataCallResponse.LINK_STATUS_INACTIVE <->
        // SetupDataCallResult.active == DATA_CONNECTION_STATUS_INACTIVE
        if (setupDataResultList[i].active == SetupDataCallResult::DATA_CONNECTION_STATUS_INACTIVE) {
            // disconnect datacall, set cid to null
            if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_DEFAULT) {
                mtk_property_set("vendor.ril.default.profile.cid", "");
            } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_RCS) {
                mtk_property_set("vendor.ril.private1.profile.cid", "");
            } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_BIP) {
                mtk_property_set("vendor.ril.private2.profile.cid", "");
            } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_IMS) {
                mtk_property_set("vendor.ril.ims.profile.cid", "");
            }

            // remove Node Rildata for node list
            DeleteNodeDataByCid(setupDataResultList[i].cid);
        }
    }
}

static void removeAllRilData() {
    mtkLogI(LOG_TAG, "%s", __FUNCTION__);
    Node* current = head;
    while (current != nullptr) {
        RilData* ril_data = current->ril_data;
        mtkLogI(LOG_TAG, "%s, current node not null", __FUNCTION__);
        if (ril_data != nullptr) {
            SetupDataCallResult *pReqResult = (SetupDataCallResult*)(ril_data->setupDataResult);
            mtkLogI(LOG_TAG, "%s, ril_data->serial: %d ", __FUNCTION__, ril_data->serial);
            if (pReqResult != nullptr) {
                mtkLogI(LOG_TAG, "%s, (ril_data) pReqResult->cid: %d", __FUNCTION__, pReqResult->cid);
                current = current->next;
                DeleteNodeDataByCid(pReqResult->cid);
                continue;
            }
        }
        current = current->next;
    }
}

static void removeRilDataIfNotExisted(void* resultList, int size) {
    mtkLogI(LOG_TAG, "%s: resultList size = %d", __FUNCTION__, size);
    SetupDataCallResult* setupDataResultList = static_cast<SetupDataCallResult*>(resultList);
    if (setupDataResultList == nullptr) {
        mtkLogW(LOG_TAG, "setupDataResultList cast failed, it is nullptr, then remove all ril data if existed");
        removeAllRilData();
        return;
    }

    Node* current = head;
    while (current != nullptr) {
        bool foundMatch = false;
        bool isReqing = false;
        bool rilDataNull = false;
        RilData* ril_data = current->ril_data;
        if (ril_data != nullptr) {
            SetupDataCallResult *pReqResult = (SetupDataCallResult*)(ril_data->setupDataResult);
            mtkLogI(LOG_TAG, "%s, ril_data->serial: %d ", __FUNCTION__, ril_data->serial);
            if (pReqResult != nullptr) {
                mtkLogI(LOG_TAG, "%s, (ril_data) pReqResult->cid: %d ", __FUNCTION__, pReqResult->cid);
                for (size_t i = 0; i < size; ++i) {
                    mtkLogI(LOG_TAG, "%s, setupDataResultList[%d].cid: %d ", __FUNCTION__, i, setupDataResultList[i].cid);
                    if (pReqResult->cid == setupDataResultList[i].cid) {
                        mtkLogI(LOG_TAG, "%s, (ril_data) pReqResult->cid: %d, found in call list", __FUNCTION__, pReqResult->cid);
                        foundMatch = true;
                    }
                }
                if (!foundMatch) {
                    mtkLogW(LOG_TAG, "%s, not found in calllist, remove pReqResult->cid: %d ", __FUNCTION__, pReqResult->cid);
                    current = current->next;
                    DeleteNodeDataByCid(pReqResult->cid);
                } else {
                    mtkLogW(LOG_TAG, "%s, has found in calllist and go next, pReqResult->cid: %d ", __FUNCTION__, pReqResult->cid);
                }
            } else {
                mtkLogW(LOG_TAG, "%s, request done waiting for result, need to check next ril data node", __FUNCTION__);
                isReqing = true;
            }
        } else {
            mtkLogW(LOG_TAG, "%s, ril data null, need to check next ril data node", __FUNCTION__);
            rilDataNull = true;
        }
        if (foundMatch || isReqing || rilDataNull) {
            mtkLogI(LOG_TAG, "%s, already found in calllist or waiting for result, go to next ril data node", __FUNCTION__);
            current = current->next;
        }
    }
}

void RadioDataAdapater::dataCallListChangedIndEx(void* resultList, int size, int slotId) {
    mtkLogI(LOG_TAG, "%s: slotId = %d, size = %d ", __FUNCTION__, slotId, size);
    int mtuV4 = 0;
    int mtuV6 = 0;

    rilData_mutex.lock();
    removeRilDataIfNotExisted(resultList, size);

    android::ClientId client = getClientBySlot(slotId);
    SetupDataCallResult* setupDataResultList = static_cast<SetupDataCallResult*>(resultList);
    if (setupDataResultList == nullptr) {
        mtkLogE(LOG_TAG, "setupDataResultList cast failed, it is nullptr");
        rilData_mutex.unlock();
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        mtkLogI(LOG_TAG, "%s: setupDataResultList[%d].cid = %d ", __FUNCTION__, i, setupDataResultList[i].cid);
        RilData* ril_data = findRilDataByCid(setupDataResultList[i].cid);
        if (ril_data == nullptr) {
            continue;
        }

        RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)(ril_data->setupDataCallInfo);
        mtkLogI(LOG_TAG, "%s: 2-slotId = %d, profileId = %d of cid(%d)", __FUNCTION__, slotId, pReq->dataProfileInfo.profileId, setupDataResultList[i].cid);
        if (pReq == nullptr) {
            mtkLogE(LOG_TAG, "SetupDataCallInformation is nullptr for CID: %d", setupDataResultList[i].cid);
            continue;
        }

        //update setupDataResult
        if (ril_data->setupDataResult != nullptr) {
            mtuV4 = ((SetupDataCallResult*)(ril_data->setupDataResult))->mtuV4;
            mtuV6 = ((SetupDataCallResult*)(ril_data->setupDataResult))->mtuV6;
            delete ril_data->setupDataResult;
            ril_data->setupDataResult = nullptr;
        }
        mtkLogI(LOG_TAG, "%s: 3-1-slotId = %d, old mtu4= %d, mtu6= %d", __FUNCTION__, slotId, setupDataResultList[i].mtuV4, setupDataResultList[i].mtuV6);
        if(mtuV4 > 0) {
            setupDataResultList[i].mtuV4 = mtuV4;
        }
        if (mtuV6 > 0) {
            setupDataResultList[i].mtuV6 = mtuV6;
        }
        mtkLogI(LOG_TAG, "%s: 3-2-slotId = %d, update mtu4= %d, mtu6= %d", __FUNCTION__, slotId, setupDataResultList[i].mtuV4, setupDataResultList[i].mtuV6);
        ril_data->setupDataResult = new SetupDataCallResult(setupDataResultList[i]);

        // map route info
        if (ril_data->tdType == SETUP_DATA_CALL_RESPONSE_AOSP && client == android::CLIENT_RILJ) {
            mapResultRouteInfo(&(setupDataResultList[i]), pReq->dataProfileInfo.profileId);

        } else if(ril_data->tdType == SETUP_DATA_CALL_RESPONSE_EXT && client == android::CLIENT_RILJ) {
            mapResultSliceRouteInfo(&(setupDataResultList[i]), pReq->dataProfileInfo.profileId);
        }

        // data disconnect, remove Node Rildata for node list.
        // response.getLinkStatus() == DataCallResponse.LINK_STATUS_INACTIVE <->
        // SetupDataCallResult.active == DATA_CONNECTION_STATUS_INACTIVE
        if (setupDataResultList[i].active == SetupDataCallResult::DATA_CONNECTION_STATUS_INACTIVE) {
            // disconnect datacall, set cid to null
            if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_DEFAULT) {
                mtk_property_set("vendor.ril.default.profile.cid", "");
            } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_RCS) {
                mtk_property_set("vendor.ril.private1.profile.cid", "");
            } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_BIP) {
                mtk_property_set("vendor.ril.private2.profile.cid", "");
            } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_IMS) {
                mtk_property_set("vendor.ril.ims.profile.cid", "");
            }
            // remove Node Rildata for node list
            DeleteNodeDataByCid(setupDataResultList[i].cid);
        }
    }
    rilData_mutex.unlock();
}

void RadioDataAdapater::deactivateDataCallEx(int32_t cid) {
    mtkLogI(LOG_TAG, "%s: cid = %d ", __FUNCTION__, cid);

    rilData_mutex.lock();
    RilData* ril_data = findRilDataByCid(cid);
    if (ril_data != nullptr) {
        RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)(ril_data->setupDataCallInfo);

        // disconnect datacall, set cid to null
        if (pReq != nullptr) {
            mtkLogI(LOG_TAG, "%s: profileId(%d)", __FUNCTION__, pReq->dataProfileInfo.profileId);
            if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_DEFAULT) {
                mtk_property_set("vendor.ril.default.profile.cid", "");
            } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_RCS) {
                mtk_property_set("vendor.ril.private1.profile.cid", "");
            } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_VENDOR_BIP) {
                mtk_property_set("vendor.ril.private2.profile.cid", "");
            } else if (pReq->dataProfileInfo.profileId == RIL_DATA_PROFILE_IMS) {
                mtk_property_set("vendor.ril.ims.profile.cid", "");
            }
        }
    }
    // remove Node Rildata for node list
    DeleteNodeDataByCid(cid);
    rilData_mutex.unlock();
}

bool RadioDataAdapater::syncDataSettingsToMdEx(int32_t dataOnOff, int32_t clientId) {
    mtkLogI(LOG_TAG, "%s: android set dataOnOff %d, clientId: %d\n",
        __FUNCTION__, dataOnOff, clientId);

    if (clientId != android::CLIENT_ASSIST) {
        mtkLogE(LOG_TAG, "NOT android client, no need to sync dataOnOff");
        return false;
    }

    if (dataOnOff == 1) {
        mtk_property_set("persist.android.dataOnOff", "1");
    } else if (dataOnOff == 0) {
        mtk_property_set("persist.android.dataOnOff", "0");
    } else {
        // -2 means skip to set.
        mtkLogW(LOG_TAG, "dataOnOff=%d value error! no need set.", dataOnOff);
    }
    return true;
}