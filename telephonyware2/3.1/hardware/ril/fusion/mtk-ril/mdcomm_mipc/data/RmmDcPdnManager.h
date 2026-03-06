/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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

#ifndef __RMM_DC_PDN_MANAGER_H__
#define __RMM_DC_PDN_MANAGER_H__

#include "RfxBaseHandler.h"
#include "RmmDataDefs.h"
#include "RmmDcUtility.h"

class RmmDcPdnManager: public RfxBaseHandler {
    public:
        RmmDcPdnManager(int slot_id, int channel_id);
        virtual ~RmmDcPdnManager();

    public:
        int getPdnTableSize();
        bool validateAid(int aid);
        bool validatePdnState(int pdnState);

        // Get Pdn info of the specified AID.
        PdnInfo getPdnInfo(int aid);
        void setPdnInfo(int aid, PdnInfo* info);
        void clearPdnInfo(int aid);
        void clearAllPdnInfo();

        // A set of set functions to update PDN attributes of a specific AID.
        bool updatePdnActiveStatus(int aid, int pdnActiveStatus);
        void updateApnName(int aid, const char* apnName);
        void updateTransIntfId(int aid, int transIntfId);
        void updateIpAddress(int aid, const char* ipv4Addr, int index, const char* ipv6Addr);
        void updatePcscfAddress(int aid, int index, const char* pcscfAddr);
        void updateIpv4Dns(int aid, int index, const char* v4Dns);
        void updateIpv6Dns(int aid, int index, const char* v6Dns);
        bool updateIpv4Address(int aid, const char* ipv4Addr);
        bool updateIpv6Address(int aid, int index, const char* ipv6Addr);
        bool updateIpv6Info(int aid, int index, Ipv6Info ipv6Info);
        void updateDns(int aid, int v4DnsNumber, int v6DnsNumber,
                const char** v4Dns, const char** v6Dns);
        bool updateMtuV4(int aid, int mtuV4);
        bool updateMtuV6(int aid, int mtuV6);
        void updateRat(int aid, int rat);
        void updateBearerId(int aid, int bearerId);

        void setAid(int index, int aid);
        void setPrimaryAid(int index, int primaryAid);
        void setAidAndPrimaryAid(int index, int aid, int primaryAid);
        void setIsDedicateBearer(int aid, bool isDedicateBearer);
        void setIsEmergency(int aid, bool isEmergency);
        void setReason(int aid, int reason);
        void setDeactReason(int aid, int deactReason);
        void setSscMode(int aid, int sscMode);
        void setPdnType(int aid, int pdnType);
        void setSignalingFlag(int aid, int flag);
        void setProfileId(int aid, int profileId);
        void setInterfaceId(int aid, int interfaceId);
        void setAccessNetwork(int aid, int accessNetwork);
        void setDefaultQos(int aid, const RIL_Qos* defaultQos);
        void setQosSessionCount(int aid, int qosSessionCount);
        void setQosSession(int aid, RIL_QosSession* qosSession);
        void setHandoverFailureMode(int aid, int handoverFailureMode);
        void setPduSessionId(int aid, int pduSessionId);
        void setSliceInfo(int aid, RIL_SliceInfo* optionalSliceInfo);
        void setTds(int aid, int count, RIL_TrafficDescriptor* Tds);

        void clearDefaultQos(int aid);
        void clearQosSession(int aid);
        void clearSliceInfo(int aid);
        void clearTds(int aid);

        // A set of get functions to get PDN attributes of a specific AID.
        int getPdnActiveStatus(int aid);
        char* getApnName(int aid);
        int getTransIntfId(int aid);
        int getAid(int index);
        int getPrimaryAid(int index);
        char* getIpv4Dns(int aid, int index);
        char* getIpv6Dns(int aid, int index);
        int getMtuV4(int aid);
        int getMtuV6(int aid);
        int getRat(int aid);
        int getSignalingFlag(int aid);
        char* getIpv4Address(int aid);
        char* getIpv6Address(int aid, int index);
        int getReason(int aid);
        int getDeactReason(int aid);
        int getSscMode(int aid);
        int getPdnType(int aid);
        int getProfileId(int aid);
        int getInterfaceId(int aid);
        int getAccessNetwork(int aid);
        RIL_Qos* getDefaultQos(int aid);
        int getQosSessionCount(int aid);
        RIL_QosSession* getQosSession(int aid);
        int getHandoverFailureMode(int aid);
        int getPduSessionId(int aid);
        RIL_SliceInfo* getSliceInfo(int aid);
        int getTdCount(int aid);
        RIL_TrafficDescriptor* getTds(int aid);

        bool isDedicateBearer(int aid);
        void initAtCmds();

    private:
        void initPdnTable();
        int getModemSupportPdnNumber();
        void clearPdnInfo(PdnInfo* info);

        void dumpAllPdnInfo();
        // PdnInfo to string.
        // TODO: public and move to utility class if other class needs it.
        static String8 pdnInfoToString(PdnInfo* pdnInfo);

        // Update status key to notify PDN status, requested by AGPS.
        void updateAndNotifyPdnConnectStatusChanged();

    private:
        int mPdnTableSize;
        PdnInfo *m_pPdnInfo;
        int mNumOfConnectedPdn;
        static const int DEFAULT_PDN_TABLE_SIZE = 50;

        // Add variable to config internal debug info.
        static const bool DEBUG_MORE_INFO = false;
};

// Validate whether the aid is valid, return TRUE if it is valid.
// Use it before access PDN info to avoid bad memory access behavior.
inline bool RmmDcPdnManager::validateAid(int aid) {
    return (aid >= 0 && aid < mPdnTableSize);
}

inline bool RmmDcPdnManager::validatePdnState(int pdnState) {
    return (pdnState >= DATA_STATE_INACTIVE && pdnState <= DATA_STATE_ACTIVE);
}

inline void RmmDcPdnManager::updateApnName(int aid, const char* apnName) {
    RFX_ASSERT(validateAid(aid));
    strncpy(m_pPdnInfo[aid].apn, apnName, MAX_APN_NAME_LENGTH - 1);
}

inline void RmmDcPdnManager::updateTransIntfId(int aid, int transIntfId) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].transIntfId = transIntfId;
}

inline bool RmmDcPdnManager::updateMtuV4(int aid, int mtuV4) {
    RFX_ASSERT(validateAid(aid));
    if (m_pPdnInfo[aid].mtuV4 == mtuV4) return false;
    m_pPdnInfo[aid].mtuV4 = mtuV4;
    return true;
}

inline bool RmmDcPdnManager::updateMtuV6(int aid, int mtuV6) {
    RFX_ASSERT(validateAid(aid));
    if (m_pPdnInfo[aid].mtuV6 == mtuV6) return false;
    m_pPdnInfo[aid].mtuV6 = mtuV6;
    return true;
}

inline void RmmDcPdnManager::updateRat(int aid, int rat) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].rat = rat;
}

inline void RmmDcPdnManager::setSignalingFlag(int aid, int flag) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].signalingFlag = flag;
}

inline void RmmDcPdnManager::updateBearerId(int aid, int bearerId) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].bearerId = bearerId;
}

inline void RmmDcPdnManager::updatePcscfAddress(int aid, int index, const char* pcscfAddr) {
    RFX_ASSERT(validateAid(aid));
    RFX_ASSERT(index >= 0 && index < MAX_PCSCF_NUMBER);
    RFX_ASSERT(pcscfAddr != NULL);
    strncpy(m_pPdnInfo[aid].pcscf[index], pcscfAddr, MAX_IPV6_ADDRESS_LENGTH - 1);
}

inline void RmmDcPdnManager::updateIpv4Dns(int aid, int index, const char* v4Dns) {
    RFX_ASSERT(validateAid(aid));
    if (RmmDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_DNS_ADDRESS_NUMBER) {
            RFX_LOG_E("RmmDcPdnManager", "[%d][%s] index%d is out of bound on aid%d",
                    m_slot_id, __FUNCTION__, index, aid);
            return;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_DNS_ADDRESS_NUMBER);
    RFX_ASSERT(v4Dns != NULL);
    strncpy(m_pPdnInfo[aid].dnsV4[index], v4Dns, MAX_IPV4_ADDRESS_LENGTH - 1);
}

inline void RmmDcPdnManager::updateIpv6Dns(int aid, int index, const char* v6Dns) {
    RFX_ASSERT(validateAid(aid));
    if (RmmDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_DNS_ADDRESS_NUMBER) {
            RFX_LOG_E("RmmDcPdnManager", "[%d][%s] index%d is out of bound on aid%d",
                    m_slot_id, __FUNCTION__, index, aid);
            return;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_DNS_ADDRESS_NUMBER);
    RFX_ASSERT(v6Dns != NULL);
    strncpy(m_pPdnInfo[aid].dnsV6[index], v6Dns, MAX_IPV6_ADDRESS_LENGTH - 1);
}

inline bool RmmDcPdnManager::updateIpv4Address(int aid, const char* ipv4Addr) {
    RFX_ASSERT(validateAid(aid));
    RFX_ASSERT(ipv4Addr != NULL);
    if (strcmp(ipv4Addr, m_pPdnInfo[aid].addressV4) == 0) return false;
    strncpy(m_pPdnInfo[aid].addressV4, ipv4Addr, MAX_IPV4_ADDRESS_LENGTH - 1);
    return true;
}

inline bool RmmDcPdnManager::updateIpv6Address(int aid, int index, const char* ipv6Addr) {
    RFX_ASSERT(validateAid(aid));
    if (RmmDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_IPV6_ADDRESS_NUMBER) {
            RFX_LOG_E("RmmDcPdnManager", "[%d][%s] index%d is out of bound on aid%d",
                    m_slot_id, __FUNCTION__, index, aid);
            return false;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_IPV6_ADDRESS_NUMBER);
    RFX_ASSERT(ipv6Addr != NULL);

    if (strcmp(ipv6Addr, m_pPdnInfo[aid].addressV6Info[index].addressV6) == 0) return false;
    strncpy(m_pPdnInfo[aid].addressV6Info[index].addressV6,
            ipv6Addr, MAX_IPV6_ADDRESS_LENGTH - 1);
    return true;
}

inline bool RmmDcPdnManager::updateIpv6Info(int aid, int index, Ipv6Info ipv6Info) {
    RFX_ASSERT(validateAid(aid));
    if (index < 0 || index >= MAX_NUM_IPV6_ADDRESS_NUMBER) {
        RFX_LOG_E("RmmDcPdnManager", "[%d][%s] index%d is out of bound on aid%d",
                    m_slot_id, __FUNCTION__, index, aid);
        return false;
    }

    bool updated = false;
    if (strcmp(ipv6Info.addressV6, m_pPdnInfo[aid].addressV6Info[index].addressV6) != 0) {
        strncpy(m_pPdnInfo[aid].addressV6Info[index].addressV6, ipv6Info.addressV6,
                MAX_IPV6_ADDRESS_LENGTH - 1);
        updated = true;
    }
    if (m_pPdnInfo[aid].addressV6Info[index].prefixLength != ipv6Info.prefixLength) {
        m_pPdnInfo[aid].addressV6Info[index].prefixLength = ipv6Info.prefixLength;
        updated = true;
    }
    if (m_pPdnInfo[aid].addressV6Info[index].flags != ipv6Info.flags) {
        m_pPdnInfo[aid].addressV6Info[index].flags = ipv6Info.flags;
        updated = true;
    }
    if (m_pPdnInfo[aid].addressV6Info[index].deprecationTime != ipv6Info.deprecationTime) {
        m_pPdnInfo[aid].addressV6Info[index].deprecationTime = ipv6Info.deprecationTime;
        updated = true;
    }
    if (m_pPdnInfo[aid].addressV6Info[index].expirationTime != ipv6Info.expirationTime) {
        m_pPdnInfo[aid].addressV6Info[index].expirationTime = ipv6Info.expirationTime;
        updated = true;
    }
    return updated;
}

inline void RmmDcPdnManager::setAid(int index, int aid) {
    RFX_ASSERT(validateAid(index));
    m_pPdnInfo[index].aid = aid;
}

inline void RmmDcPdnManager::setPrimaryAid(int index, int primaryAid) {
    RFX_ASSERT(validateAid(index));
    m_pPdnInfo[index].primaryAid = primaryAid;
}

inline void RmmDcPdnManager::setAidAndPrimaryAid(int index, int aid, int primaryAid) {
    RFX_ASSERT(validateAid(index));
    m_pPdnInfo[index].aid = aid;
    m_pPdnInfo[index].primaryAid = primaryAid;
}

inline void RmmDcPdnManager::setIsEmergency(int aid, bool isEmergency) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].isEmergency = isEmergency;
}

inline void RmmDcPdnManager::setIsDedicateBearer(int aid, bool isDedicateBearer) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].isDedicateBearer = isDedicateBearer;
}

inline void RmmDcPdnManager::setReason(int aid, int reason) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].reason = reason;
}

inline void RmmDcPdnManager::setDeactReason(int aid, int deactReason) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].deactReason = deactReason;
}

inline void RmmDcPdnManager::setSscMode(int aid, int sscMode) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].sscMode = sscMode;
}

inline void RmmDcPdnManager::setPdnType(int aid, int pdnType) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].pdnType = pdnType;
}

inline void RmmDcPdnManager::setProfileId(int aid, int profileId) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].profileId = profileId;
}

inline void RmmDcPdnManager::setInterfaceId(int aid, int interfaceId) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].interfaceId = interfaceId;
}

inline void RmmDcPdnManager::setAccessNetwork(int aid, int accessNetwork) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].accessNetwork = accessNetwork;
}

inline void RmmDcPdnManager::setQosSessionCount(int aid, int qosSessionCount) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].qosSessionCount = qosSessionCount;
}

inline void RmmDcPdnManager::setHandoverFailureMode(int aid, int handoverFailureMode) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].handoverFailureMode = handoverFailureMode;
}

inline void RmmDcPdnManager::setPduSessionId(int aid, int pduSessionId) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].pduSessionId = pduSessionId;
}

inline int RmmDcPdnManager::getPdnActiveStatus(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].active;
}

inline int RmmDcPdnManager::getTransIntfId(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].transIntfId;
}

inline char* RmmDcPdnManager::getApnName(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].apn;
}

inline int RmmDcPdnManager::getAid(int index) {
    RFX_ASSERT(validateAid(index));
    return m_pPdnInfo[index].aid;
}

inline int RmmDcPdnManager::getPrimaryAid(int index) {
    RFX_ASSERT(validateAid(index));
    return m_pPdnInfo[index].primaryAid;
}

inline char* RmmDcPdnManager::getIpv4Dns(int aid, int index) {
    RFX_ASSERT(validateAid(aid));
    if (RmmDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_DNS_ADDRESS_NUMBER) {
            RFX_LOG_E("RmmDcPdnManager", "[%d][%s] index%d is out of bound on aid%d",
                    m_slot_id, __FUNCTION__, index, aid);
            return NULL;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_DNS_ADDRESS_NUMBER);
    return m_pPdnInfo[aid].dnsV4[index];
}

inline char* RmmDcPdnManager::getIpv6Dns(int aid, int index) {
    RFX_ASSERT(validateAid(aid));
    if (RmmDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_DNS_ADDRESS_NUMBER) {
            RFX_LOG_E("RmmDcPdnManager", "[%d][%s] index%d is out of bound on aid%d",
                    m_slot_id, __FUNCTION__, index, aid);
            return NULL;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_DNS_ADDRESS_NUMBER);
    return m_pPdnInfo[aid].dnsV6[index];
}

inline int RmmDcPdnManager::getMtuV4(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].mtuV4;
}

inline int RmmDcPdnManager::getMtuV6(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].mtuV6;
}

inline int RmmDcPdnManager::getRat(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].rat;
}

inline int RmmDcPdnManager::getSignalingFlag(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].signalingFlag;
}

inline char* RmmDcPdnManager::getIpv4Address(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].addressV4;
}

inline char* RmmDcPdnManager::getIpv6Address(int aid, int index) {
    RFX_ASSERT(validateAid(aid));
    if (RmmDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_IPV6_ADDRESS_NUMBER) {
            RFX_LOG_E("RmmDcPdnManager", "[%d][%s] index%d is out of bound on aid%d",
                    m_slot_id, __FUNCTION__, index, aid);
            return NULL;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_IPV6_ADDRESS_NUMBER);
    return m_pPdnInfo[aid].addressV6Info[index].addressV6;
}

inline int RmmDcPdnManager::getPdnType(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].pdnType;
}

inline int RmmDcPdnManager::getProfileId(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].profileId;
}

inline int RmmDcPdnManager::getInterfaceId(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].interfaceId;
}

inline int RmmDcPdnManager::getReason(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].reason;
}

inline int RmmDcPdnManager::getDeactReason(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].deactReason;
}

inline int RmmDcPdnManager::getSscMode(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].sscMode;
}

inline bool RmmDcPdnManager::isDedicateBearer(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].isDedicateBearer;
}

inline int RmmDcPdnManager::getAccessNetwork(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].accessNetwork;
}

inline RIL_Qos* RmmDcPdnManager::getDefaultQos(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].defaultQos;
}

inline int RmmDcPdnManager::getQosSessionCount(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].qosSessionCount;
}

inline RIL_QosSession* RmmDcPdnManager::getQosSession(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].qosSession;
}

inline int RmmDcPdnManager::getHandoverFailureMode(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].handoverFailureMode;
}

inline int RmmDcPdnManager::getPduSessionId(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].pduSessionId;
}

inline RIL_SliceInfo* RmmDcPdnManager::getSliceInfo(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].optionalSliceInfo;
}

inline int RmmDcPdnManager::getTdCount(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].tdCount;
}

inline RIL_TrafficDescriptor* RmmDcPdnManager::getTds(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].TDs;
}

#endif /* __RMM_DC_PDN_MANAGER_H__ */
