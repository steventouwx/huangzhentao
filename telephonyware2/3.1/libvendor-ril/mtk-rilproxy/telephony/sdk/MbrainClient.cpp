// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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
#define LOG_TAG "TELE_MBRAIN"

#include <string>
#include <utils/Log.h>
#include "MbrainMessageSender.h"

static int mPs[2] = {-2,-2};
static int mCs[2] = {-2,-2};
static int mRadio[2] = {-2,-2};
static int mCsEct[2] = {-2,-2};
static int mPsEct[2] = {-2,-2};
static int mData_default_slot = 0;
static int mDataEnableState = 0;
static int mApn0_state = 0;
static std::string mApn0_ifname = std::string("");
static std::string mApn0_address = std::string("");
static int mApn0_cid = 0;
static int mApn1_state = 0;
static std::string mApn1_ifname = std::string("");
static std::string mApn1_address = std::string("");
static int mApn1_cid = 0;
static int mApn2_state = 0;
static std::string mApn2_ifname = std::string("");
static std::string mApn2_address = std::string("");
static int mApn2_cid = 0;
static std::string mMccMnc[2] = {std::string(""), std::string("")};
static std::string mSimStatus[2] = {std::string(""), std::string("")};
static std::string mRfCalibrationState[2] = {std::string(""), std::string("")};

static int mPoint[3] = {-1, -1, -1};
static int mCause[3] = {-1, -1, -1};
static int mError[3] = {-1, -1, -1};

uint64_t GetCurrentTimeInMS()
{
    struct timespec spec;
    if (clock_gettime(CLOCK_REALTIME, &spec) == -1)
    {
        return 0;
    }

    return spec.tv_sec * 1000 + spec.tv_nsec / 1e6;
}
#ifdef MTK_MBRAIN_SUPPORT
#include <aidl/vendor/mediatek/hardware/mbrain/IMBrain.h>
#include <aidl/vendor/mediatek/hardware/mbrain/BnMBrainCallbacks.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/IServiceManager.h>
#include <json/json.h>
#include <utils/Mutex.h>

using ::ndk::SpAIBinder;
using ::aidl::vendor::mediatek::hardware::mbrain::IMBrain;
using ::aidl::vendor::mediatek::hardware::mbrain::MBrain_Event;
using ::aidl::vendor::mediatek::hardware::mbrain::MBrain_Parcelable;
using android::Mutex;

std::shared_ptr<IMBrain> mbrainServer = nullptr;
static Mutex gLock;
ndk::ScopedAIBinder_DeathRecipient m_deathRecipient;

static const std::string service_name = std::string() +
    (IMBrain::descriptor ? IMBrain::descriptor : "") + "/default";


void mbrainBinderDead(void* ptr) {
    Mutex::Autolock autoLock(gLock);
    AIBinder_unlinkToDeath(mbrainServer->asBinder().get(), m_deathRecipient.get(), nullptr);
    mbrainServer = nullptr;
}

std::shared_ptr<IMBrain> get_mbrain_server() {

    if (mbrainServer == nullptr) {
        mbrainServer = IMBrain::fromBinder(SpAIBinder(AServiceManager_checkService(service_name.c_str())));
        if (mbrainServer != nullptr)
        {
            m_deathRecipient = ndk::ScopedAIBinder_DeathRecipient(AIBinder_DeathRecipient_new(mbrainBinderDead));
            AIBinder_linkToDeath(mbrainServer->asBinder().get(), m_deathRecipient.get(), nullptr);
        }
    }
    return mbrainServer;
}

int mbrain_notify(int32_t event_id, std::string msg) {
    MBrain_Parcelable mbrain_parcelable_input;
    int ret = 0;
    Mutex::Autolock autoLock(gLock);
    std::shared_ptr<IMBrain> ptr = get_mbrain_server();
    if (ptr == nullptr) {
        RLOGW("mbrain service not exist!\n");
        return -1;
    }

    ptr->IsMBrainSupport(&ret);
    if (ret != 1) {
        return -1;
    }

    RLOGD("Event id:%d, msg = %s", event_id, msg.c_str());

    mbrain_parcelable_input.privData = android::String8(msg.c_str());
    ndk::ScopedAStatus status = ptr->NotifyExt(event_id, mbrain_parcelable_input, &ret);
    if (!status.isOk()) {
        RLOGE("Failed to call NotifyExt");
        return -1;
    }
    return 0;
}

#endif

int notifyTeleServiceMainThreadTimeout(int slotId, int msgId, int msgType,
    long start_sec, long start_nsec, long end_sec, long end_nsec, std::string extraInfo) {
#ifdef MTK_MBRAIN_SUPPORT
    Json::Value root = Json::Value(Json::objectValue);
    Json::Value mainThreadTimeout = Json::Value(Json::objectValue);
    mainThreadTimeout["timestamp"] = GetCurrentTimeInMS();
    mainThreadTimeout["slotId"] = slotId;
    mainThreadTimeout["msgId"] = msgId;
    mainThreadTimeout["msgType"] = msgType;
    mainThreadTimeout["start_sec"] = start_sec;
    mainThreadTimeout["start_nsec"] = start_nsec;
    mainThreadTimeout["end_sec"] = end_sec;
    mainThreadTimeout["end_nsec"] = end_nsec;
    mainThreadTimeout["extraInfo"] = extraInfo;
    root["mainThreadTimeout"] = mainThreadTimeout;

    Json::StreamWriterBuilder builder;
    std::string result = Json::writeString(builder, root);
    RLOGD("notifyTeleServiceMainThreadTimeout result = %s", result.c_str());

    return mbrain_notify(2073, result); // E_EVENT_AUTO_TELEPHONY_SERVICE_TIMEOUT_NOTIFY
#else
    return 0;
#endif
}

int notifyTeleServiceNetworkStatus(const std::vector<teleNetworkStatus>& messages) {
#ifdef MTK_MBRAIN_SUPPORT
    Json::Value root = Json::Value(Json::objectValue);
    Json::Value networkArrayArray = Json::Value(Json::arrayValue);
    for (const auto& status : messages) {
        Json::Value networkStatus = Json::Value(Json::objectValue);
        networkStatus["currenttime"] = status.currentTime;
        networkStatus["slotId"] = status.slotId;
        networkStatus["ps_attach"] = status.ps;
        networkStatus["cs_attach"] = status.cs;
        networkStatus["radio_state"] = status.radio;
        networkStatus["radio_tech"] = status.psEct;
        networkStatus["data_default_slot"] = status.data_default_slot;
        networkStatus["dataEnableState"] = status.dataEnableState;
        networkStatus["apn0_state"] = status.apn0_state;
        networkStatus["apn0_ifname"] = status.apn0_ifname;
        networkStatus["apn0_address"] = status.apn0_address;
        networkStatus["apn0_cid"] = status.apn0_cid;
        networkStatus["apn1_state"] = status.apn1_state;
        networkStatus["apn1_ifname"] = status.apn1_ifname;
        networkStatus["apn1_address"] = status.apn1_address;
        networkStatus["apn1_cid"] = status.apn1_cid;
        networkStatus["apn2_state"] = status.apn2_state;
        networkStatus["apn2_ifname"] = status.apn2_ifname;
        networkStatus["apn2_address"] = status.apn2_address;
        networkStatus["apn2_cid"] = status.apn2_cid;
        networkStatus["mccmnc"] = status.mccmnc;
        networkStatus["simstatus"] = status.simstatus;
        networkStatus["rfcalibration"] = status.rfcalibration;
        networkArrayArray.append(networkStatus);
    }

    root["networkStatus"] = networkArrayArray;
    Json::StreamWriterBuilder builder;
    std::string result = Json::writeString(builder, root);
    RLOGD("notifyTeleServiceNetworkStatus result = %s", result.c_str());

    return mbrain_notify(2074, result); // E_EVENT_AUTO_TELEPHONY_SERVICE_NETWORK_STATUS_NOTIFY
#endif
    return 0;
}

int notifyMbrainPsAndEctChanged(int slot, int ps, int ect) {
#ifdef MTK_MBRAIN_SUPPORT
    if ((ps != mPs[slot]) || (ect != mPsEct[slot])) {
        mPs[slot] = ps;
        mPsEct[slot] = ect;
        MbrainMessageSender& sender = MbrainMessageSender::getInstance();
        teleNetworkStatus status = {GetCurrentTimeInMS(), slot, mPs[slot], mCs[slot], mRadio[slot], mPsEct[slot], mCsEct[slot], mData_default_slot, mDataEnableState, mApn0_state, mApn0_ifname,
                                       mApn0_address, mApn0_cid, mApn1_state, mApn1_ifname, mApn1_address, mApn1_cid, mApn2_state, mApn2_ifname, mApn2_address,
                                       mApn2_cid, mMccMnc[slot], mSimStatus[slot], mRfCalibrationState[slot]};
        sender.addMessage(status);
    }
#endif
    return 0;
}

int notifyMbrainCsAndEctChanged(int slot, int cs, int ect) {
#ifdef MTK_MBRAIN_SUPPORT
    if ((cs != mCs[slot]) || (ect != mCsEct[slot])) {
        mCs[slot] = cs;
        mCsEct[slot] = ect;
        MbrainMessageSender& sender = MbrainMessageSender::getInstance();
        teleNetworkStatus status = {GetCurrentTimeInMS(),slot, mPs[slot], mCs[slot], mRadio[slot], mPsEct[slot], mCsEct[slot], mData_default_slot, mDataEnableState, mApn0_state, mApn0_ifname,
                                       mApn0_address, mApn0_cid, mApn1_state, mApn1_ifname, mApn1_address, mApn1_cid, mApn2_state, mApn2_ifname, mApn2_address,
                                       mApn2_cid, mMccMnc[slot], mSimStatus[slot], mRfCalibrationState[slot]};
        sender.addMessage(status);
    }
#endif
    return 0;
}

int notifyMbrainRadiosChanged(int slot, int radio) {
#ifdef MTK_MBRAIN_SUPPORT
    if (radio != mRadio[slot]) {
        mRadio[slot] = radio;
        MbrainMessageSender& sender = MbrainMessageSender::getInstance();
        teleNetworkStatus status = {GetCurrentTimeInMS(),slot, mPs[slot], mCs[slot], mRadio[slot], mPsEct[slot], mCsEct[slot], mData_default_slot, mDataEnableState, mApn0_state, mApn0_ifname,
                                       mApn0_address, mApn0_cid, mApn1_state, mApn1_ifname, mApn1_address, mApn1_cid, mApn2_state, mApn2_ifname, mApn2_address,
                                       mApn2_cid, mMccMnc[slot], mSimStatus[slot], mRfCalibrationState[slot]};
        sender.addMessage(status);
    }
#endif
    return 0;
}

int notifyMbrainCardStatusChanged(int slot, std::string simStatus) {
#ifdef MTK_MBRAIN_SUPPORT
    if ((slot==1 || slot==0) && simStatus != mSimStatus[slot]) {
        mSimStatus[slot] = simStatus;
        MbrainMessageSender& sender = MbrainMessageSender::getInstance();
        teleNetworkStatus status = {GetCurrentTimeInMS(),slot, mPs[slot], mCs[slot], mRadio[slot], mPsEct[slot], mCsEct[slot], mData_default_slot, mDataEnableState, mApn0_state, mApn0_ifname,
                                       mApn0_address, mApn0_cid, mApn1_state, mApn1_ifname, mApn1_address, mApn1_cid, mApn2_state, mApn2_ifname, mApn2_address,
                                       mApn2_cid, mMccMnc[slot], mSimStatus[slot], mRfCalibrationState[slot]};
        sender.addMessage(status);
    }
#endif
    return 0;
}

int notifyMbrainMccMncStatusChanged(int slot, std::string mccmnc) {
#ifdef MTK_MBRAIN_SUPPORT
    if ((slot==1 || slot==0) && mccmnc != mMccMnc[slot]) {
        mMccMnc[slot] = mccmnc;
        MbrainMessageSender& sender = MbrainMessageSender::getInstance();
        teleNetworkStatus status = {GetCurrentTimeInMS(),slot, mPs[slot], mCs[slot], mRadio[slot], mPsEct[slot], mCsEct[slot], mData_default_slot, mDataEnableState, mApn0_state, mApn0_ifname,
                                       mApn0_address, mApn0_cid, mApn1_state, mApn1_ifname, mApn1_address, mApn1_cid, mApn2_state, mApn2_ifname, mApn2_address,
                                       mApn2_cid, mMccMnc[slot], mSimStatus[slot], mRfCalibrationState[slot]};
        sender.addMessage(status);
    }
#endif
    return 0;
}

int notifyMbrainRfStatusStatusChanged(int slot, std::string rfStatus) {
#ifdef MTK_MBRAIN_SUPPORT
    if ((slot==1 || slot==0) && rfStatus != mRfCalibrationState[slot]) {
        mRfCalibrationState[slot] = rfStatus;
        MbrainMessageSender& sender = MbrainMessageSender::getInstance();
        teleNetworkStatus status = {GetCurrentTimeInMS(),slot, mPs[slot], mCs[slot], mRadio[slot], mPsEct[slot], mCsEct[slot], mData_default_slot, mDataEnableState, mApn0_state, mApn0_ifname,
                                       mApn0_address, mApn0_cid, mApn1_state, mApn1_ifname, mApn1_address, mApn1_cid, mApn2_state, mApn2_ifname, mApn2_address,
                                       mApn2_cid, mMccMnc[slot], mSimStatus[slot], mRfCalibrationState[slot]};
        sender.addMessage(status);
    }
#endif
    return 0;
}

int checkDataInfoChanged(int profileId,
    std::string ifname, std::string address, int state, int cid) {
    int mCount = 0;
    switch (profileId) {
        case 1: {
            if (mApn0_ifname != ifname) {
                mApn0_ifname = ifname;
                mCount++;
            }
            if (mApn0_address != address) {
                mApn0_address = address;
                mCount++;
            }
            if (mApn0_state != state) {
                mApn0_state = state;
                mCount++;
            }
            if (mApn0_cid != cid) {
                mApn0_cid = cid;
                mCount++;
            }
            break;
        }
        case 2: {
            if (mApn1_ifname != ifname) {
                mApn1_ifname = ifname;
                mCount++;
            }
            if (mApn1_address != address) {
                mApn1_address = address;
                mCount++;
            }
            if (mApn1_state != state) {
                mApn1_state = state;
                mCount++;
            }
            if (mApn1_cid != cid){
                mApn1_cid = cid;
                mCount++;
            }
            break;
        }
        case 3: {
            if (mApn2_ifname != ifname) {
                mApn2_ifname = ifname;
                mCount++;
            }
            if (mApn2_address != address) {
                mApn2_address = address;
                mCount++;
            }
            if (mApn2_state != state) {
                mApn2_state = state;
                mCount++;
            }
            if (mApn2_cid != cid) {
                mApn2_cid = cid;
                mCount++;
            }
            break;
        }
        default:
            break;
    }
    RLOGD("MbrainClient: checkDataInfoChanged input: "
        "profile_id=%d, ifname=%s, addresses=%s, pdnState=%d, cid=%d",
        profileId, ifname.c_str(), address.c_str(), state, cid);
    RLOGD("MbrainClient: checkDataInfoChanged output: change count=%d, "
        "mApn0_ifname=%s, mApn0_address=%s, mApn0_state=%d, mApn0_cid=%d "
        "mApn1_ifname=%s, mApn1_address=%s, mApn1_state=%d, mApn1_cid=%d "
        "mApn2_ifname=%s, mApn2_address=%s, mApn2_state=%d, mApn2_cid=%d",
        mCount, mApn0_ifname.c_str(), mApn0_address.c_str(), mApn0_state, mApn0_cid,
        mApn1_ifname.c_str(), mApn1_address.c_str(), mApn1_state, mApn1_cid,
        mApn2_ifname.c_str(), mApn2_address.c_str(), mApn2_state, mApn2_cid);
    return mCount;
}

int notifyMbrainPdnStateChanged(int slot, int dataOnoff, int profileId,
    std::string ifname, std::string address, int state, int cid) {
#ifdef MTK_MBRAIN_SUPPORT
    RLOGD("MbrainClient: notifyMbrainPdnStateChanged: "
        "slotId=%d, dataOnoff=%d, profile_id=%d, ifname=%s, addresses=%s, pdnState=%d, cid=%d",
        slot, dataOnoff, profileId, ifname.c_str(), address.c_str(), state, cid);
    int mIndChangeCount = 0;
    if (slot != mData_default_slot) {
        mData_default_slot = slot;
        mIndChangeCount++;
    }
    if (dataOnoff != mDataEnableState) {
        mDataEnableState = dataOnoff;
        mIndChangeCount++;
    }
    int ret = checkDataInfoChanged(profileId, ifname, address, state, cid);
    mIndChangeCount = mIndChangeCount + ret;
    if (mIndChangeCount > 0) {
        MbrainMessageSender& sender = MbrainMessageSender::getInstance();
        teleNetworkStatus status = {GetCurrentTimeInMS(),slot, mPs[slot], mCs[slot], mRadio[slot], mPsEct[slot], mCsEct[slot], mData_default_slot, mDataEnableState, mApn0_state, mApn0_ifname,
                                       mApn0_address, mApn0_cid, mApn1_state, mApn1_ifname, mApn1_address, mApn1_cid, mApn2_state, mApn2_ifname, mApn2_address,
                                       mApn2_cid, mMccMnc[slot], mSimStatus[slot], mRfCalibrationState[slot]};
        sender.addMessage(status);
    }
#endif
    return 0;
}

int notifyTeleServiceDataStatus(const std::vector<teleDataStatus>& messages) {
#ifdef MTK_MBRAIN_SUPPORT
    Json::Value root = Json::Value(Json::objectValue);
    Json::Value dataArray  = Json::Value(Json::arrayValue);
    for (const auto& status : messages) {
        Json::Value dataStatus = Json::Value(Json::objectValue);
        dataStatus["currenttime"] = status.currentTime;
        dataStatus["slotId"] = status.slotId;
        dataStatus["dataOnoff"] = status.dataOnoff;
        dataStatus["actType"] = status.actType;
        dataStatus["ps_attach"] = status.ps;
        dataStatus["profileId"] = status.profileId;
        dataStatus["apn0_point"] = status.apn0_point;
        dataStatus["apn0_state"] = status.apn0_state;
        dataStatus["apn0_ifname"] = status.apn0_ifname;
        dataStatus["apn0_address"] = status.apn0_address;
        dataStatus["apn0_cid"] = status.apn0_cid;
        dataStatus["apn0_failcause"] = status.apn0_failcause;
        dataStatus["apn0_error"] = status.apn0_error;
        dataStatus["apn1_point"] = status.apn1_point;
        dataStatus["apn1_state"] = status.apn1_state;
        dataStatus["apn1_ifname"] = status.apn1_ifname;
        dataStatus["apn1_address"] = status.apn1_address;
        dataStatus["apn1_cid"] = status.apn1_cid;
        dataStatus["apn1_failcause"] = status.apn1_failcause;
        dataStatus["apn1_error"] = status.apn1_error;
        dataStatus["apn2_point"] = status.apn2_point;
        dataStatus["apn2_state"] = status.apn2_state;
        dataStatus["apn2_ifname"] = status.apn2_ifname;
        dataStatus["apn2_address"] = status.apn2_address;
        dataStatus["apn2_cid"] = status.apn2_cid;
        dataStatus["apn2_failcause"] = status.apn2_failcause;
        dataStatus["apn2_error"] = status.apn2_error;
        dataArray.append(dataStatus);
    }

    root["dataStatus"] = dataArray;
    Json::StreamWriterBuilder builder;
    std::string result = Json::writeString(builder, root);
    RLOGD("notifyTeleServiceDataStatus result = %s", result.c_str());

    return mbrain_notify(2075, result); // E_EVENT_AUTO_TELEPHONY_SERVICE_DATA_STATUS_NOTIFY
#endif
    return 0;
}

int notifyMbrainDataPoint(int profileId, int point) {
    int index = (profileId - 1) < 0 ? 0 : (profileId - 1);
    mPoint[index] = point;
    RLOGD("MbrainClient: notifyMbrainDataPoint input profileId = %d, point = %d, output mPoint[%d] = %d",
        profileId, point, index, mPoint[index]);
    return 0;
}

int checkDataCauseErrorChanged(int profileId, int cause, int error) {
    int index = profileId - 1;
    int mCount = 0;
    if (mCause[index] != cause) {
        mCause[index] = cause;
        mCount++;
    }
    if (mError[index] != error) {
        mError[index] = error;
        mCount++;
    }
    RLOGD("MbrainClient: checkDataCauseErrorChanged input profileId = %d, cause = %d, error=%d, output mCause[%d] = %d, mError[%d] = %d, count=%d",
        profileId, cause, error, index, mCause[index], index, mError[index], mCount);
    return mCount;
}

int notifyMbrainDataResponse(int slot, int dataOnoff, int actType, int profileId,
    std::string ifname, std::string address, int state, int cid, int failcause, int error) {
#ifdef MTK_MBRAIN_SUPPORT
    RLOGD("MbrainClient: notifyMbrainDataResponse: "
        "slotId=%d, dataOnoff=%d, actType=%d, mPs[%d]=%d, profile_id=%d, mPoint=%d"
        " ifname=%s, addresses=%s, pdnState=%d, cid=%d, failcause=%d, error=%d",
        slot, dataOnoff, actType, slot, mPs[slot], profileId, mPoint[profileId - 1],
        ifname.c_str(), address.c_str(), state, cid, failcause, error);

    int mRspChangeCount = checkDataInfoChanged(profileId, ifname, address, state, cid);
    int ret = checkDataCauseErrorChanged(profileId, failcause, error);
    mRspChangeCount = mRspChangeCount + ret;
    if (mRspChangeCount > 0) {
        MbrainMessageSender& sender = MbrainMessageSender::getInstance();
        teleDataStatus status = {GetCurrentTimeInMS(), slot, dataOnoff, actType, mPs[slot], profileId,
            mPoint[0], mApn0_state, mApn0_ifname, mApn0_address, mApn0_cid, mCause[0], mError[0],
            mPoint[1], mApn1_state, mApn1_ifname, mApn1_address, mApn1_cid, mCause[1], mError[1],
            mPoint[2], mApn2_state, mApn2_ifname, mApn2_address, mApn2_cid, mCause[2], mError[2]};
        sender.addDataStatusMessage(status);
    }
#endif
    return 0;
}
