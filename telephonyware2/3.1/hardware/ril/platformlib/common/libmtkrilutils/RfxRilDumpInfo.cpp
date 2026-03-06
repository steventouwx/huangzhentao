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

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxRilDumpInfo.h"
#include <sys/prctl.h>


#define RFX_LOG_TAG "RfxRilDumpInfo"

RfxRilDumpInfo::RfxRilDumpInfo(const char* key, int num) {
    m_numOfDumpMsg = num;
    m_key = NULL;
    if (key == NULL || strlen(key) == 0) {
        return;
    }
    m_key = strdup(key);
    m_mutex = PTHREAD_MUTEX_INITIALIZER;
}

RfxRilDumpInfo::RfxRilDumpInfo(char* key, int num) {
    m_numOfDumpMsg = num;
    if (key == NULL || strlen(key) == 0) {
        m_key = NULL;
        return;
    }
    m_key = strdup(key);
    m_mutex = PTHREAD_MUTEX_INITIALIZER;
}

RfxRilDumpInfo::~RfxRilDumpInfo() {
    if (m_key != NULL) {
        free(m_key);
    }
}

void RfxRilDumpInfo::outPutRilDumpInfo(FILE* out) {
    pthread_mutex_lock(&m_mutex);
    int ret = 0;
    ret = fprintf(out, "RILD debug info key:%s\n", m_key);
    if (ret < 0) {
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    for (unsigned int i = 0; i < rilDumpInfoMessage.size(); i++) {
        ret = fprintf(out, "%s\n", rilDumpInfoMessage[i].c_str());
        if (ret < 0) {
            pthread_mutex_unlock(&m_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&m_mutex);
}

void RfxRilDumpInfo::addDumpInfoMessage(string info) {
    struct tm now;
    time_t tNow = time(NULL);
    if (tNow < 0) {
        return;
    }
    localtime_r(&tNow, &now);
    char str[20];
    int ret = 0;
    ret = strftime(str, sizeof(str), "%m-%d %H:%M:%S", &now);
    if (ret <= 0) {
        return;
    }

    pthread_mutex_lock(&m_mutex);
    rilDumpInfoMessage.push_back(string(str) + string(" ") + info);
    if (rilDumpInfoMessage.size() > m_numOfDumpMsg) {
        rilDumpInfoMessage.erase(rilDumpInfoMessage.begin());
    }
    pthread_mutex_unlock(&m_mutex);
}

char* RfxRilDumpInfo::getDumpInfoKey(void) {
    return m_key;
}


vector<RfxRilDumpInfo*> LibDumpRildInfo::m_rildumpinfo;
Mutex LibDumpRildInfo::s_create_dump_info_mutex;
bool LibDumpRildInfo::m_create_channel_done = false;

int LibDumpRildInfo::createNewDumpMessage(int num) {
    s_create_dump_info_mutex.lock();
    char key[30] = {0};
    prctl(PR_GET_NAME, key);

    for (unsigned int i = 0; i < m_rildumpinfo.size(); i++) {
        if (strcmp(key, m_rildumpinfo[i] -> getDumpInfoKey()) == 0) {
            s_create_dump_info_mutex.unlock();
            return -1;
        }
    }

    RfxRilDumpInfo* dumpInfo = new RfxRilDumpInfo(key, num);
    m_rildumpinfo.push_back(dumpInfo);
    s_create_dump_info_mutex.unlock();
    return 0;
}

int LibDumpRildInfo::setCreateChannelDone(bool isDone) {
    m_create_channel_done = isDone;
    return 0;
}


int LibDumpRildInfo::createNewDumpMessage(const char* key, int num) {
    s_create_dump_info_mutex.lock();
    for (unsigned int i = 0; i < m_rildumpinfo.size(); i++) {
        if (strcmp(key, m_rildumpinfo[i] -> getDumpInfoKey()) == 0) {
            s_create_dump_info_mutex.unlock();
            return -1;
        }
    }

    RfxRilDumpInfo* dumpInfo = new RfxRilDumpInfo(key, num);
    m_rildumpinfo.push_back(dumpInfo);
    s_create_dump_info_mutex.unlock();
    return 0;
}

int LibDumpRildInfo::AddDumpMessage(const char* key, string info) {
    RfxRilDumpInfo* dumpinfo = NULL;

    if (m_create_channel_done == false) {
        return -2;
    }

    for (unsigned int i = 0; i < m_rildumpinfo.size(); i++) {
        if (strcmp(key, m_rildumpinfo[i] -> getDumpInfoKey()) == 0) {
            dumpinfo = m_rildumpinfo[i];
        }
    }

    if (dumpinfo == NULL) {
        return -1;
    }

    dumpinfo-> addDumpInfoMessage(info);

    return 0;
}

int LibDumpRildInfo::AddDumpMessage(const char* key, char* info) {
    if (info == NULL) {
        return -1;
    }
    return AddDumpMessage(key, string(info));
}

int LibDumpRildInfo::AddDumpMessage(const char* key, String8 info) {
    RfxRilDumpInfo* dumpinfo = NULL;
    if (m_create_channel_done == false) {
        return -2;
    }

    for (unsigned int i = 0; i < m_rildumpinfo.size(); i++) {
        if (strcmp(key, m_rildumpinfo[i] -> getDumpInfoKey()) == 0) {
            dumpinfo = m_rildumpinfo[i];
        }
    }

    if (dumpinfo == NULL) {
        return -1;
    }

    if (info.string() == NULL) {
        return -1;
    }

    dumpinfo-> addDumpInfoMessage(string(info.string()));

    return 0;
}

int LibDumpRildInfo::AddDumpMessage(string info) {
    RfxRilDumpInfo* dumpinfo = NULL;
    char key[30] = {0};
    prctl(PR_GET_NAME, key);

    if (m_create_channel_done == false) {
        return -2;
    }

    for (unsigned int i = 0; i < m_rildumpinfo.size(); i++) {
        if (strcmp(key, m_rildumpinfo[i] -> getDumpInfoKey()) == 0) {
            dumpinfo = m_rildumpinfo[i];
        }
    }

    if (dumpinfo == NULL) {
        return -1;
    }

    dumpinfo-> addDumpInfoMessage(info);

    return 0;
}

int LibDumpRildInfo::AddDumpMessage(const char* info) {
    if (info == NULL) {
        return -1;
    }
    return AddDumpMessage(string(info));
}

int LibDumpRildInfo::AddDumpMessage(char* info) {
    if (info == NULL) {
        return -1;
    }
    return AddDumpMessage(string(info));
}

int LibDumpRildInfo::AddDumpMessage(String8 info) {
    if (info.string() == NULL) {
        return -1;
    }
    return AddDumpMessage(string(info.string()));
}

int LibDumpRildInfo::dumpRilMessageInfo(FILE* out) {
    int ret = 0;

    struct tm now;
    time_t tNow = time(NULL);
    if (tNow < 0) {
        return -1;
    }
    localtime_r(&tNow, &now);

    char str[20];
    ret = strftime(str, sizeof(str), "%m-%d %H:%M:%S", &now);
    if (ret <= 0) {
        return -1;
    }

    ret = fprintf(out, "%s Dump RILD debug info begin\n", str);
    if (ret < 0) {
        return ret;
    }

    for (unsigned int i = 0; i < m_rildumpinfo.size(); i++) {
        m_rildumpinfo[i] -> outPutRilDumpInfo(out);
    }

    return 0;
}
