// SPDX-License-Identifier: MediaTekProprietary
/*
 * PdnManager.cpp
 *
 *  Created on: Jun 28, 2017
 *      Author: mtk54458
 */
#include "PdnManager.h"
#include "RpDataUtils.h"

#define RFX_LOG_TAG "RpDfwPdnManager"

PdnManager::PdnManager() {
    PdnManager::mSetupRequestPool.clear();
    PdnManager::mTearDownRequestPool.clear();
}

void PdnManager::addSetupRequest(sp<RfxMessage> setupRequest) {
    PdnInfo oldPdnInfo = PdnManager::getSetupPdnInfoByToken(setupRequest->getToken());
    if (oldPdnInfo.token != -1) {
        RFX_LOG_W(RFX_LOG_TAG, "PdnManager: addSetupRequest, the setup request with token: %d is already in.",
                setupRequest->getToken());
        return;
    }

    Parcel* parcel = setupRequest->getParcel();

    // make sure get the right position
    parcel->setDataPosition(sizeof(int32_t)*2);
    char* apnInfo = RpDataUtils::strdupReadString(parcel);
    if (apnInfo == NULL) {
        RFX_LOG_W(RFX_LOG_TAG, "PdnManager: addSetupRequest with invalid argument %d", parcel->dataSize());
        return;
    }
    if (apnInfo != NULL && strlen(apnInfo) == 0) {
        RFX_LOG_W(RFX_LOG_TAG, "PdnManager: addSetupRequest with invalid argument %d", parcel->dataSize());
        RpDataUtils::freeMemory(apnInfo);
        return;
    }

    string apnType = std::string(apnInfo);
    RpDataUtils::freeMemory(apnInfo);

    PdnInfo pdnInfo = {
            setupRequest->getToken(),
            apnType,
            setupRequest
    };

    PdnManager::m_setupReqVecMutex.lock();
    PdnManager::mSetupRequestPool.push_back(pdnInfo);

    RFX_LOG_D(RFX_LOG_TAG, "PdnManager::addSetupRequest. dump setupPdnInfo begin---------begin.");
    PdnManager::dumpPdnInfo(PdnManager::mSetupRequestPool);
    RFX_LOG_D(RFX_LOG_TAG, "PdnManager::addSetupRequest. dump setupPdnInfo end---------end.");
    PdnManager::m_setupReqVecMutex.unlock();
}

void PdnManager::removeSetupRequestByToken(int token) {
    PdnManager::m_setupReqVecMutex.lock();
    vector<PdnInfo>::iterator itor = PdnManager::mSetupRequestPool.begin();
    while (itor != PdnManager::mSetupRequestPool.end()) {
        if (itor->token == token) {
            itor = PdnManager::mSetupRequestPool.erase(itor);
            break;
        } else {
            itor++;
        }
    }

    RFX_LOG_D(RFX_LOG_TAG, "PdnManager::removeSetupRequest. dump setupPdnInfo begin---------begin.");
    PdnManager::dumpPdnInfo(PdnManager::mSetupRequestPool);
    RFX_LOG_D(RFX_LOG_TAG, "PdnManager::removeSetupRequest. dump setupPdnInfo end---------end.");
    PdnManager::m_setupReqVecMutex.unlock();
}

PdnInfo PdnManager::getSetupPdnInfoByToken(int token) {
    PdnInfo pdnInfo = {-1, "", 0};

    PdnManager::m_setupReqVecMutex.lock();
    vector<PdnInfo>::iterator itor = PdnManager::mSetupRequestPool.begin();
    while (itor != PdnManager::mSetupRequestPool.end()) {
        if (itor->token == token) {
            pdnInfo = *itor;
            break;
        }
        itor++;
    }
    RFX_LOG_D(RFX_LOG_TAG, "PdnManager: getSetupPdnInfoByToken. with token: %d, with apnType: %s.",
            pdnInfo.token, pdnInfo.apnType.c_str());
    PdnManager::m_setupReqVecMutex.unlock();

    return pdnInfo;
}

void PdnManager::addTearDownRequst(sp<RfxMessage> tearDownRequest) {
    PdnInfo oldPdnInfo = PdnManager::getTearDownPdnInfoByToken(tearDownRequest->getToken());
    if (oldPdnInfo.token != -1) {
        RFX_LOG_W(RFX_LOG_TAG, "PdnManager: addTearDownRequst, the tear down request with token: %d is already in.",
                tearDownRequest->getToken());

        return;
    }

    Parcel* parcel = tearDownRequest->getParcel();

    // make sure get the right position
    parcel->setDataPosition(sizeof(int32_t)*2);
    char* apnInfo = RpDataUtils::strdupReadString(parcel);
    if (apnInfo == NULL) {
        RFX_LOG_W(RFX_LOG_TAG, "PdnManager: addTearDownRequst with invalid argument %d", parcel->dataSize());
        return;
    }
    if (apnInfo != NULL && strlen(apnInfo) == 0) {
        RFX_LOG_W(RFX_LOG_TAG, "PdnManager: addSetupRequest with invalid argument %d", parcel->dataSize());
        RpDataUtils::freeMemory(apnInfo);
        return;
    }

    string apnType = std::string(apnInfo);
    RpDataUtils::freeMemory(apnInfo);

    PdnInfo pdnInfo = {
            tearDownRequest->getToken(),
            apnType,
            tearDownRequest
    };

    PdnManager::m_teardownReqVecMutex.lock();
    PdnManager::mTearDownRequestPool.push_back(pdnInfo);

    RFX_LOG_D(RFX_LOG_TAG, "PdnManager::addTearDownRequst. dump tearDonwPdnInfo begin---------begin.");
    PdnManager::dumpPdnInfo(PdnManager::mTearDownRequestPool);
    RFX_LOG_D(RFX_LOG_TAG, "PdnManager::addTearDownRequst. dump tearDownPdnInfo end---------end.");
    PdnManager::m_teardownReqVecMutex.unlock();
}

void PdnManager::removeTearDownRequestByToken(int token) {
    PdnManager::m_teardownReqVecMutex.lock();
    vector<PdnInfo>::iterator itor = PdnManager::mTearDownRequestPool.begin();
    while (itor != PdnManager::mTearDownRequestPool.end()) {
        if (itor->token == token) {
            itor = PdnManager::mTearDownRequestPool.erase(itor);
            break;
        } else {
            itor++;
        }
    }

    RFX_LOG_D(RFX_LOG_TAG, "PdnManager::removeTearDownRequest. dump tearDonwPdnInfo begin---------begin.");
    PdnManager::dumpPdnInfo(PdnManager::mTearDownRequestPool);
    RFX_LOG_D(RFX_LOG_TAG, "PdnManager::removeTearDownRequest. dump tearDownPdnInfo end---------end.");
    PdnManager::m_teardownReqVecMutex.unlock();
}

PdnInfo PdnManager::getTearDownPdnInfoByToken(int token) {
    PdnInfo pdnInfo = { -1, "", 0 };
    PdnManager::m_teardownReqVecMutex.lock();
    vector<PdnInfo>::iterator itor = PdnManager::mTearDownRequestPool.begin();
    while (itor != PdnManager::mTearDownRequestPool.end()) {
        if (itor->token == token) {
            pdnInfo = *itor;
        }
        itor++;
    }
    RFX_LOG_D(RFX_LOG_TAG, "PdnManager: getTearDownPdnInfoByToken. with token: %d, with apnType: %s.",
            pdnInfo.token, pdnInfo.apnType.c_str());
    PdnManager::m_teardownReqVecMutex.unlock();

    return pdnInfo;
}

PdnInfo PdnManager::getTearDownPdnInfoByApn(string apnType) {
    PdnInfo pdnInfo = { -1, "", 0 };
    PdnManager::m_teardownReqVecMutex.lock();
    vector<PdnInfo>::iterator itor = PdnManager::mTearDownRequestPool.begin();
    while (itor != PdnManager::mTearDownRequestPool.end()) {
        if (itor->apnType == apnType) {
            pdnInfo = *itor;
        }
        itor++;
    }
    RFX_LOG_D(RFX_LOG_TAG, "PdnManager: getTearDownPdnInfoByApn. with token: %d, with apnType: %s.",
            pdnInfo.token, pdnInfo.apnType.c_str());
    PdnManager::m_teardownReqVecMutex.unlock();
    return pdnInfo;
}

void PdnManager::addGetFailCauseRequest(int token, PdnInfo pdnInfo) {
    PdnManager::m_retrypdninfoMapMutex.lock();
    PdnManager::mRetryPdnInfoPool.insert(pair<int, PdnInfo>(token, pdnInfo));

    RFX_LOG_D(RFX_LOG_TAG, "PdnManager: addGetFailCauseRequest. with token: %d, mapped pdn info token: %d.",
            token, pdnInfo.token);
    PdnManager::m_retrypdninfoMapMutex.unlock();
}

PdnInfo PdnManager::getRetryPdnInfoByToken(int token) {
    PdnInfo pdnInfo = { -1, "", 0 };
    PdnManager::m_retrypdninfoMapMutex.lock();
    if (PdnManager::mRetryPdnInfoPool.find(token) != PdnManager::mRetryPdnInfoPool.end()) {
        pdnInfo = PdnManager::mRetryPdnInfoPool.find(token)->second;
    }

    RFX_LOG_D(RFX_LOG_TAG, "PdnManager: getRetryPdnInfoByToken. with token: %d, with apnType: %s.",
            pdnInfo.token, pdnInfo.apnType.c_str());
    PdnManager::m_retrypdninfoMapMutex.unlock();
    return pdnInfo;
}

void PdnManager::removeGetFailCauseRequestByToken(int token) {
    PdnManager::m_retrypdninfoMapMutex.lock();
    PdnManager::mRetryPdnInfoPool.erase(token);

    RFX_LOG_D(RFX_LOG_TAG, "PdnManager: removeGetFailCauseRequestByToken. with token: %d, now the size is: %d",
            token, PdnManager::mRetryPdnInfoPool.size());
    PdnManager::m_retrypdninfoMapMutex.unlock();
}

void PdnManager::dumpPdnInfo(vector<PdnInfo> pdnInfoPool) {
    vector<PdnInfo>::iterator itor = pdnInfoPool.begin();

    while (itor != pdnInfoPool.end()) {
        RFX_LOG_D(RFX_LOG_TAG, "PdnManager: dumpPdnInfo, with token: %d, with apn type: %s.",
                itor->token, itor->apnType.c_str());

        itor++;
    }
}
