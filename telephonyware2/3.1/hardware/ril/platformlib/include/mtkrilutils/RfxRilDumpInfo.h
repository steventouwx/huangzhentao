/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2016. All rights reserved.
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

#ifndef __RFX_RIL_DUMP_INFO_H__
#define __RFX_RIL_DUMP_INFO_H__

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "utils/Timers.h"
#include "utils/Mutex.h"
#include "utils/String8.h"

using std::string;
using std::vector;
using std::to_string;
using ::android::Mutex;
using ::android::String8;


class RfxRilDumpInfo {
public:
    RfxRilDumpInfo(const char* key, int num);
    RfxRilDumpInfo(char* key, int num);
    RfxRilDumpInfo(int num);
    virtual ~RfxRilDumpInfo();

public:
    void addDumpInfoMessage(string info);
    void outPutRilDumpInfo(FILE* out);
    char* getDumpInfoKey(void);

private:
    vector<string> rilDumpInfoMessage;
    int m_numOfDumpMsg;
    char* m_key;
    pthread_mutex_t m_mutex;
};

class LibDumpRildInfo {
    private:
        static Mutex s_create_dump_info_mutex;
        static vector<RfxRilDumpInfo*> m_rildumpinfo;
        static bool m_create_channel_done;
    public:
        static int createNewDumpMessage(const char* key, int num);
        static int createNewDumpMessage(int num);
        static int AddDumpMessage(const char* key, char* info);
        static int AddDumpMessage(const char* info);
        static int AddDumpMessage(char* info);
        static int AddDumpMessage(string info);
        static int AddDumpMessage(String8 info);
        static int AddDumpMessage(const char* key, String8 info);
        static int AddDumpMessage(const char* key, string info);
        static int dumpRilMessageInfo(FILE* out);
        static int setCreateChannelDone(bool isDone);
};

#endif
