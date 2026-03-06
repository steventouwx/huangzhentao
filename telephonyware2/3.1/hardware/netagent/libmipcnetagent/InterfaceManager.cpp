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

#include <mtk_log.h>
#include "InterfaceManager.h"

#define NA_LOG_TAG "InterfaceManager"

pthread_mutex_t InterfaceManager::sMapMutex = PTHREAD_MUTEX_INITIALIZER;

InterfaceManager::InterfaceManager() {
}

InterfaceManager::~InterfaceManager() {
    std::map<int, InterfaceInfo*>::iterator it = mInterfaceInfo.begin();
    for (; it != mInterfaceInfo.end(); ++it) {
        FREEIF(it->second);
    }
    mInterfaceInfo.clear();
}

int InterfaceManager::isValid(int interfaceId) {
    if (interfaceId == INVALID_INTERFACE_ID || mInterfaceInfo.count(interfaceId) <= 0
            || mInterfaceInfo.at(interfaceId) == NULL) {
        NA_LOG_E("[%s] invalid interface %d", __FUNCTION__, interfaceId);
        return 0;
    }
    return 1;
}

int InterfaceManager::initInterfaceInfo(int interfaceId) {
    if (interfaceId == INVALID_INTERFACE_ID) return -1;

    if (mInterfaceInfo.count(interfaceId) <= 0) {
        NA_LOG_D("[%s] new interface %d", __FUNCTION__, interfaceId);
        InterfaceInfo *pInterfaceInfo = (InterfaceInfo*)calloc(1, sizeof(InterfaceInfo));
        if (pInterfaceInfo == NULL) {
            NA_LOG_E("[%s] calloc interface fail", __FUNCTION__);
            return -1;
        }
        pInterfaceInfo->interfaceId = interfaceId;
        mInterfaceInfo[interfaceId] = pInterfaceInfo;
    }

    return isValid(interfaceId);
}

int InterfaceManager::updateIpv4Addr(int interfaceId, char* address) {
    if (interfaceId == INVALID_INTERFACE_ID || address == NULL) return -1;

    pthread_mutex_lock(&sMapMutex);
    if (initInterfaceInfo(interfaceId) < 1) {
        pthread_mutex_unlock(&sMapMutex);
        return -1;
    }

    strncpy(mInterfaceInfo[interfaceId]->addressV4, address,
            MAX_IPV4_ADDRESS_LENGTH - 1);
    pthread_mutex_unlock(&sMapMutex);
    return 0;
}

int InterfaceManager::updateIpv6Addr(int interfaceId, char *address, int prefixLength,
        int flags, long deprecationTime, long expirationTime) {
    if (interfaceId == INVALID_INTERFACE_ID || address == NULL) return -1;

    pthread_mutex_lock(&sMapMutex);
    if (initInterfaceInfo(interfaceId) < 1) {
        pthread_mutex_unlock(&sMapMutex);
        return -1;
    }

    int count = mInterfaceInfo[interfaceId]->globalIpv6Count;
    if (count < 0 || count >= MAX_IPV6_ADDRESS_COUNT) {
        NA_LOG_E("[%s] error %d ipv6 addresses", __FUNCTION__, count);
        pthread_mutex_unlock(&sMapMutex);
        return -1;
    }
    if (count == 1
            && strcmp(address, mInterfaceInfo[interfaceId]->ipv6Info[0].addressV6) == 0) {
        NA_LOG_E("[%s] why save same ipv6 address", __FUNCTION__);
        pthread_mutex_unlock(&sMapMutex);
        return -1;
    }

    mInterfaceInfo[interfaceId]->ipv6Info[count].prefixLength = prefixLength;
    mInterfaceInfo[interfaceId]->ipv6Info[count].flags = flags;
    mInterfaceInfo[interfaceId]->ipv6Info[count].deprecationTime = deprecationTime;
    mInterfaceInfo[interfaceId]->ipv6Info[count].expirationTime = expirationTime;
    strncpy(mInterfaceInfo[interfaceId]->ipv6Info[count].addressV6,
            address, MAX_IPV6_ADDRESS_LENGTH - 1);

    mInterfaceInfo[interfaceId]->globalIpv6Count++;

    NA_LOG_D("[%s] interfaceId=%d, prefixLength=%d, flags=%d, deprecationTime=%ld,"
            " expirationTime=%ld count=%d", __FUNCTION__, interfaceId, prefixLength,
            flags, deprecationTime, expirationTime,
            mInterfaceInfo[interfaceId]->globalIpv6Count);

    int ipv6Count = mInterfaceInfo[interfaceId]->globalIpv6Count;
    pthread_mutex_unlock(&sMapMutex);
    return ipv6Count;
}

int InterfaceManager::updateMtuV4(int interfaceId, int mtuV4) {
    pthread_mutex_lock(&sMapMutex);
    if (isValid(interfaceId) < 1) {
        pthread_mutex_unlock(&sMapMutex);
        return -1;
    }

    if (mInterfaceInfo[interfaceId]->mtuV4 != mtuV4) {
        mInterfaceInfo[interfaceId]->mtuV4 = mtuV4;
        pthread_mutex_unlock(&sMapMutex);
        return 1;
    }

    pthread_mutex_unlock(&sMapMutex);
    return -1;
}

int InterfaceManager::updateMtuV6(int interfaceId, int mtuV6) {
    pthread_mutex_lock(&sMapMutex);
    if (isValid(interfaceId) < 1) {
        pthread_mutex_unlock(&sMapMutex);
        return -1;
    }

    if (mInterfaceInfo[interfaceId]->mtuV6 != mtuV6) {
        mInterfaceInfo[interfaceId]->mtuV6 = mtuV6;
        pthread_mutex_unlock(&sMapMutex);
        return 1;
    }

    pthread_mutex_unlock(&sMapMutex);
    return -1;
}

int InterfaceManager::getIpv4Addr(int interfaceId, char* address) {
    if (isValid(interfaceId) < 1 || address == NULL) return -1;
    strncpy(address, mInterfaceInfo.at(interfaceId)->addressV4, MAX_IPV4_ADDRESS_LENGTH - 1);
    return 1;
}

int InterfaceManager::getIpv6Info(int interfaceId, Ipv6Info** ipv6Info) {
    if (isValid(interfaceId) < 1 || ipv6Info == NULL) return -1;

    int count = mInterfaceInfo.at(interfaceId)->globalIpv6Count;
    if (count <= 0) {
        NA_LOG_E("[%s] no ipv6 addresses", __FUNCTION__);
        return -1;
    }

    int length = 0;
    for (int i = 0; i < count; i++) {
        if (ipv6Info[length] == NULL) break;
        ipv6Info[length]->prefixLength = mInterfaceInfo.at(interfaceId)->ipv6Info[i].prefixLength;
        ipv6Info[length]->flags = mInterfaceInfo.at(interfaceId)->ipv6Info[i].flags;
        ipv6Info[length]->deprecationTime =
                mInterfaceInfo.at(interfaceId)->ipv6Info[i].deprecationTime;
        ipv6Info[length]->expirationTime =
                mInterfaceInfo.at(interfaceId)->ipv6Info[i].expirationTime;
        strncpy(ipv6Info[length]->addressV6,
                mInterfaceInfo.at(interfaceId)->ipv6Info[i].addressV6,
                MAX_IPV6_ADDRESS_LENGTH - 1);
        length++;
        NA_LOG_D("[%s] interfaceId=%d count=%d", __FUNCTION__, interfaceId, length);
    }

    return length;
}

int InterfaceManager::getMatchedIpv6Count(int interfaceId, char* address) {
    if (isValid(interfaceId) < 1 || address == NULL) return 0;

    int match = 0;
    int count = mInterfaceInfo.at(interfaceId)->globalIpv6Count;
    for (int i = 0; i < count; i++) {
        if (strcmp(mInterfaceInfo.at(interfaceId)->ipv6Info[i].addressV6, address) == 0) {
            match++;
        }
    }
    NA_LOG_D("[%s] interfaceId=%d match=%d", __FUNCTION__, interfaceId, match);
    return match;
}

int InterfaceManager::getInterfaceInfo(int interfaceId, InterfaceInfo** interfaceInfo) {
    pthread_mutex_lock(&sMapMutex);
    if (isValid(interfaceId) < 1) {
        pthread_mutex_unlock(&sMapMutex);
        return -1;
    }

    memcpy(*interfaceInfo, mInterfaceInfo[interfaceId], sizeof(InterfaceInfo));

    pthread_mutex_unlock(&sMapMutex);
    return 1;
}

int InterfaceManager::getIpv6Count(int interfaceId) {
    if (isValid(interfaceId) < 1) return 0;
    return mInterfaceInfo.at(interfaceId)->globalIpv6Count;
}

int InterfaceManager::getMtuV4(int interfaceId) {
    if (isValid(interfaceId) < 1) return -1;
    return mInterfaceInfo.at(interfaceId)->mtuV4;
}

int InterfaceManager::getMtuV6(int interfaceId) {
    if (isValid(interfaceId) < 1) return -1;
    return mInterfaceInfo.at(interfaceId)->mtuV6;
}

int InterfaceManager::removeIpv6Info(int interfaceId, char* address) {
    pthread_mutex_lock(&sMapMutex);
    if (isValid(interfaceId) < 1 || address == NULL || strlen(address) <= 0) {
        pthread_mutex_unlock(&sMapMutex);
        return -1;
    }

    int count = mInterfaceInfo[interfaceId]->globalIpv6Count;
    int removeCount = 0;
    NA_LOG_D("[%s] interfaceId=%d count=%d", __FUNCTION__, interfaceId, count);
    for (int i = 0; i < count; i++) {
        if (strlen(mInterfaceInfo[interfaceId]->ipv6Info[i].addressV6) <= 0) continue;
        if (strcmp(mInterfaceInfo[interfaceId]->ipv6Info[i].addressV6, address) == 0) {
            memset(&mInterfaceInfo[interfaceId]->ipv6Info[i], 0, sizeof(Ipv6Info));
            if (i == 0 && count > 1) {
                mInterfaceInfo[interfaceId]->ipv6Info[i].prefixLength
                        = mInterfaceInfo[interfaceId]->ipv6Info[i + 1].prefixLength;
                mInterfaceInfo[interfaceId]->ipv6Info[i].flags
                        = mInterfaceInfo[interfaceId]->ipv6Info[i + 1].flags;
                mInterfaceInfo[interfaceId]->ipv6Info[i].deprecationTime
                        = mInterfaceInfo[interfaceId]->ipv6Info[i + 1].deprecationTime;
                mInterfaceInfo[interfaceId]->ipv6Info[i].expirationTime
                        = mInterfaceInfo[interfaceId]->ipv6Info[i + 1].expirationTime;
                strncpy(mInterfaceInfo[interfaceId]->ipv6Info[i].addressV6,
                        mInterfaceInfo[interfaceId]->ipv6Info[i + 1].addressV6,
                        MAX_IPV6_ADDRESS_LENGTH - 1);
            }
            mInterfaceInfo[interfaceId]->globalIpv6Count--;
            removeCount++;
        }
    }

    if (removeCount > 1) {
        NA_LOG_E("[%s] weird case removeCount=%d", __FUNCTION__, removeCount);
        for (int j = 0; j < MAX_IPV6_ADDRESS_COUNT; j++) {
            memset(&mInterfaceInfo[interfaceId]->ipv6Info[j], 0, sizeof(Ipv6Info));
        }
        mInterfaceInfo[interfaceId]->globalIpv6Count = 0;
    }

    if (mInterfaceInfo[interfaceId]->globalIpv6Count < 0) {
        NA_LOG_E("[%s] weird case count=%d", __FUNCTION__,
                mInterfaceInfo[interfaceId]->globalIpv6Count);
        mInterfaceInfo[interfaceId]->globalIpv6Count = 0;
    }

    pthread_mutex_unlock(&sMapMutex);
    return removeCount;
}

int InterfaceManager::clearIpv6Info(int interfaceId) {
    pthread_mutex_lock(&sMapMutex);
    if (isValid(interfaceId) < 1) {
        pthread_mutex_unlock(&sMapMutex);
        return -1;
    }

    NA_LOG_D("[%s] interfaceId=%d", __FUNCTION__, interfaceId);
    for (int i = 0; i < MAX_IPV6_ADDRESS_COUNT; i++) {
        memset(&mInterfaceInfo[interfaceId]->ipv6Info[i], 0, sizeof(Ipv6Info));
    }

    mInterfaceInfo[interfaceId]->globalIpv6Count = 0;

    pthread_mutex_unlock(&sMapMutex);
    return 0;
}

void InterfaceManager::clearInterface(int interfaceId) {
    pthread_mutex_lock(&sMapMutex);
    if (isValid(interfaceId) < 1) {
        pthread_mutex_unlock(&sMapMutex);
        return;
    }
    FREEIF(mInterfaceInfo[interfaceId]);
    mInterfaceInfo.erase(interfaceId);

    pthread_mutex_unlock(&sMapMutex);
}
