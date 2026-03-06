 /*
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
#include <dlfcn.h>
#include "RfxVsimUtils.h"
#include "RfxRilUtils.h"

/*****************************************************************************
 * Class RfxVsimUtils
 *****************************************************************************/

#define RFX_LOG_TAG "RfxVsimUtils"

#define VSIM_RIL_PATH "libmtk-fusion-ril-prop-vsim.so"

void* RfxVsimUtils::m_dlHandler = NULL;
isInternalTestVsimComponent_t* RfxVsimUtils::m_dlIsInternalTestVsimComponent = NULL;
setVerifiedVsimClient_t* RfxVsimUtils::m_dlSetVerifiedVsimClient = NULL;
checkIsTestVsimComponent_t* RfxVsimUtils::m_dlCheckIsTestVsimComponent = NULL;


void RfxVsimUtils::initVsimHandler() {
    if (RfxRilUtils::isExternalSimSupport() == 0) {
        RFX_LOG_I(RFX_LOG_TAG, "VSIM is not supported.");
        return;
    }

    m_dlHandler = dlopen(VSIM_RIL_PATH, RTLD_NOW);
    const char* dlsym_error = dlerror();
    if (m_dlHandler == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] dlopen failed in %s: %s",
                __FUNCTION__, VSIM_RIL_PATH, dlsym_error);
    } else {
        m_dlIsInternalTestVsimComponent = (isInternalTestVsimComponent_t*)dlsym(m_dlHandler,
                "isInternalTestVsimComponent");
        dlsym_error = dlerror();
        if (m_dlIsInternalTestVsimComponent == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%s] isInternalTestVsimComponent not defined or exported: %s",
                    __FUNCTION__, dlsym_error);
        }
        m_dlSetVerifiedVsimClient = (setVerifiedVsimClient_t*)dlsym(m_dlHandler,
                "setVerifiedVsimClient");
        dlsym_error = dlerror();
        if (m_dlSetVerifiedVsimClient == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%s] setVerifiedVsimClient not defined or exported: %s",
                    __FUNCTION__, dlsym_error);
        }
        m_dlCheckIsTestVsimComponent = (checkIsTestVsimComponent_t*)dlsym(m_dlHandler,
                "checkIsTestVsimComponent");
        dlsym_error = dlerror();
        if (m_dlCheckIsTestVsimComponent == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%s] checkIsTestVsimComponent not defined or exported: %s",
                    __FUNCTION__, dlsym_error);
        }
    }
}

int RfxVsimUtils::isInternalTestVsimComponent() {
    if (m_dlIsInternalTestVsimComponent == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "m_dlIsInternalTestVsimComponent not defined or exported.");
        return 0;
    }
    return m_dlIsInternalTestVsimComponent();
}

void RfxVsimUtils::setVerifiedVsimClient(int verified) {
    if (m_dlSetVerifiedVsimClient == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "m_dlSetVerifiedVsimClient not defined or exported.");
        return;
    }
    m_dlSetVerifiedVsimClient(verified);
}

int RfxVsimUtils::checkIsTestVsimComponent(unsigned char *data, int len) {
    if (m_dlCheckIsTestVsimComponent == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "m_dlCheckIsTestVsimComponent not defined or exported.");
        return 0;
    }
    return m_dlCheckIsTestVsimComponent(data, len);
}



