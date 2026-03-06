/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2017. All rights reserved.
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

#include <utils/Mutex.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include "atcid.h"
#include "atcid_adaptation.h"

#include "aidl/vendor/mediatek/hardware/atci/BnAtcid.h"
#include "aidl/vendor/mediatek/hardware/atci/IAtcidCommandHandler.h"
#include "aidl/vendor/mediatek/hardware/atci/IAtcidResponse.h"


extern "C" {
#include "atcid_mipc.h"
}

using aidl::vendor::mediatek::hardware::atci::BnAtcid;
using aidl::vendor::mediatek::hardware::atci::IAtcidCommandHandler;
using aidl::vendor::mediatek::hardware::atci::IAtcidResponse;


class AtcidImpl : public aidl::vendor::mediatek::hardware::atci::BnAtcid {

    ndk::ScopedAStatus sendCommand(const std::string& data) override;

    ndk::ScopedAStatus setCommandHandler(const std::shared_ptr<IAtcidCommandHandler>& atciCommandHandler) override;

    ndk::ScopedAStatus sendCommandResponse(const std::string& data) override;

    ndk::ScopedAStatus setResponseFunction(const std::shared_ptr<IAtcidResponse>& atcidResponse) override;

private:
    std::vector<std::shared_ptr<IAtcidCommandHandler> > mAtcidCommandHandler;
    std::shared_ptr<IAtcidResponse> mAtcidResponse;
    int mFdAtciService = -1;
    int mFd = -1;

public:
    void setUp(int fd) { mFd = fd; }
    void setSocketFdForAtciService(int fd) { mFdAtciService = fd; }
    bool sendCommandToAtciService(const char *data, int size);
    bool sendResponse(const char *data, int size);
};

static pthread_t aidl_thread = 0;
static std::shared_ptr<AtcidImpl> g_aidl_service = NULL;
static bool registered= 0;

static void* aidlRegisterThread(void *arg) {
    binder_status_t status;
    LOGATCI(LOG_INFO, "Oentrer AIDL_Recv_Thread, %p", arg);
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    g_aidl_service = ndk::SharedRefBase::make<AtcidImpl>();
    const std::string name = std::string() + AtcidImpl::descriptor + "/default";
    status = AServiceManager_addService(g_aidl_service->asBinder().get(), name.c_str());
    if (status != STATUS_OK) {
        LOGATCI(LOG_INFO, "ATCI ... AIDL thread add service failed!!");
    } else {
        LOGATCI(LOG_INFO, "ATCI : service added.");
        registered = true;
        ABinderProcess_joinThreadPool();
    }

    LOGATCI(LOG_INFO, "ATCI AIDL : aidl service is terminating...");
    return 0;
}

ndk::ScopedAStatus AtcidImpl::setCommandHandler(const std::shared_ptr<IAtcidCommandHandler>& atciCommandHandler) {
    LOGATCI(LOG_INFO, "setCommandHandler");
    mAtcidCommandHandler.push_back(atciCommandHandler);
    return ndk::ScopedAStatus::ok();
}

/*aidl API for atci_service_sys and atciservice.apk*/
ndk::ScopedAStatus AtcidImpl::sendCommandResponse(const std::string& data) {
    const char *str = data.c_str();
    int sendLen = 0;
    int len = 0;
    if (str != NULL) {
        len = strlen(str);
        sendLen = send(mFdAtciService, str, data.size(), 0);
        if (sendLen != len) {
            LOGATCI(LOG_ERR, "lose data when sendCommandResponse to atcid. errno = %d, sendLen = %d, len = %d", errno, sendLen, len);
        }
    }
    return ndk::ScopedAStatus::ok();
}

//for swift tool only
::ndk::ScopedAStatus AtcidImpl::setResponseFunction(const std::shared_ptr<IAtcidResponse>& atcidResponse) {
    LOGATCI(LOG_INFO, "setResponseFunction");
    mAtcidResponse = atcidResponse;
    return ::ndk::ScopedAStatus::ok();
}

//for swift tool only
ndk::ScopedAStatus AtcidImpl::sendCommand(const std::string& data) {
    const char *str = data.c_str();
    int sendLen = 0;
    int len = 0;
    if (str != NULL) {
        len = strlen(str);
        sendLen = send(mFd, str, len, 0);
        if (sendLen != len) {
            LOGATCI(LOG_ERR, "lose data when sendCommand to atcid. errno = %d, sendLen = %d, len = %d", errno, sendLen, len);
        }
    }
    return ndk::ScopedAStatus::ok();
}

bool AtcidImpl::sendCommandToAtciService(const char *data, int size) {
    if (mAtcidCommandHandler.size() > 0 && data != NULL) {
        LOGATCI(LOG_ERR, "sendCommandToAtciService mAtcidCommandHandler != null");
        std::string str(data);
        for (unsigned int i = 0; i < mAtcidCommandHandler.size(); i++) {
            ndk::ScopedAStatus ret = mAtcidCommandHandler[i]->sendCommand(str);
            if (!ret.isOk()) {
                LOGATCI(LOG_ERR, "sendCommandToAtciService sendCommand error");
            }
        }
        return true;
    } else {
        LOGATCI(LOG_ERR, "sendCommandToAtciService mAtcidCommandHandler == null");
    }
    return false;
}

/*send response to swift tool*/
bool AtcidImpl::sendResponse(const char *data, int size) {
    if (mAtcidResponse != NULL && data != NULL) {
        LOGATCI(LOG_ERR, "sendResponse mAtcidResponse != null");
        std::string str(data);
        ndk::ScopedAStatus ret = mAtcidResponse->sendCommandResponse(str);
        if (ret.isOk()) {
            return true;
        }
    } else {
        LOGATCI(LOG_ERR, "sendResponse mAtcidResponse == null");
    }
    return false;
}

void setupAidlSocket(int fd) {
    while (!registered) {
        usleep(10*1000);
    }
    if (g_aidl_service != NULL) {
        g_aidl_service->setUp(fd);
    }
}

void initAtcidAidlService() {
    LOGATCI(LOG_INFO, "create AIDL thread, %d");
    int ret = pthread_create(&aidl_thread, NULL, aidlRegisterThread, NULL);
    if(ret != 0){
        ALOGE("PAL initBinder ... AIDL thread FAILED!!!");
    }
    LOGATCI(LOG_INFO, "PAL has AIDL thread started");
}

void setSocketFdForAtciService(int fd) {
    if (g_aidl_service != NULL) {
        g_aidl_service->setSocketFdForAtciService(fd);
    }
}

bool sendCommandToSystemAtciService(const char *data, int size) {
    if (g_aidl_service != NULL) {
        return g_aidl_service->sendCommandToAtciService(data, size);
    }
    return false;
}

/*when response receiver is swift tool*/
bool sendResponse(const char *data, int size) {
    if (g_aidl_service != NULL) {
        return g_aidl_service->sendResponse(data, size);
    }
    return false;
}

int connectToSystemAtciService() {
    int fd[2] = {0};
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) >= 0) {
        setSocketFdForAtciService(fd[1]);
        return fd[0];
    } else {
        LOGATCI(LOG_ERR, "socketpair failed. errno:%d", errno);
        return -1;
    }
}
