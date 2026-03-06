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
#include <iostream>
#include <chrono>
#include <utils/Log.h>
#include <prop/properties.h>
#include "MbrainMessageSender.h"
#include "MbrainClient.h"

#define LOG_TAG "MbrainMessageSender"

void MbrainMessageSender::addMessage(const teleNetworkStatus& status) {
    std::lock_guard<std::mutex> lock(mutex_);
    char tmpString[PROPERTY_VALUE_MAX] = {0};
    property_get("vendor.mbrain.enabled", tmpString, "0");
    int enabledMBrain = atoi(tmpString);
    if (!enabledMBrain) {
        RLOGI("%s: MBrain not enabled.", __func__);
        return;
    }
    if (messages_.size() >= MBRAIN_MESSAGE_MAX_SIZE) {
        RLOGI("%s: Message queue full(1000).", __func__);
        messages_.erase(messages_.begin());
    }
    messages_.push_back(status);
    cond_var_.notify_one();
}

void MbrainMessageSender::addDataStatusMessage(const teleDataStatus& status) {
    std::lock_guard<std::mutex> lock(mutex_);
    char tmpString[PROPERTY_VALUE_MAX] = {0};
    property_get("vendor.mbrain.enabled", tmpString, "0");
    int enabledMBrain = atoi(tmpString);
    if (!enabledMBrain) {
        RLOGI("%s: MBrain not enabled.", __func__);
        return;
    }
    if (DataStatusMessages_.size() >= MBRAIN_MESSAGE_MAX_SIZE) {
        RLOGI("%s: Message queue full(1000).", __func__);
        DataStatusMessages_.erase(DataStatusMessages_.begin());
    }
    DataStatusMessages_.push_back(status);
    cond_var_.notify_one();
}

void MbrainMessageSender::startSending() {
    sending_thread_ = std::thread(&MbrainMessageSender::sendMessages, this);
}

void MbrainMessageSender::stopSending() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
    }
    cond_var_.notify_one();
    sending_thread_.join();
}

void MbrainMessageSender::sendMessages() {
    int ret = -1;
    while (true) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this] { return !messages_.empty()
            || DataStatusMessages_.empty()  || stop_; });

        if (stop_ && messages_.empty() && DataStatusMessages_.empty()) {
            break;
        }

        if (!messages_.empty()) {
            ret = notifyTeleServiceNetworkStatus(messages_);
            if(ret == 0) {
                messages_.clear();
            } else {
                RLOGE("Failed to notifyTeleServiceNetworkStatus to MBrain, size = %d", messages_.size());
            }
        }

        if (!DataStatusMessages_.empty()) {
            ret = notifyTeleServiceDataStatus(DataStatusMessages_);
            if(ret == 0) {
                DataStatusMessages_.clear();
            } else {
                RLOGE("Failed to notifyTeleServiceDataStatus to MBrain, size = %d", DataStatusMessages_.size());
            }
        }

        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}