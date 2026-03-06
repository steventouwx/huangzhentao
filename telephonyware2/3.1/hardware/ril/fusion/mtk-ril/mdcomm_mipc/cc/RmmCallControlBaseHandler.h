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

#ifndef __RMM_CALL_CONTROL_BASE_HANDLER_H__
#define __RMM_CALL_CONTROL_BASE_HANDLER_H__

#include "RfxBaseHandler.h"

class RfxAtLine;

// This class provide the general function for request handler
class RmmCallControlBaseHandler : public RfxBaseHandler {
    public:
        RmmCallControlBaseHandler(int slot_id, int channel_id);
        virtual ~RmmCallControlBaseHandler();
    protected:
        /*
         * Convert clir mode to the clir prefix
         */
        char* getClirPrefix(int clir);
        /*
         * Response a void data with error to the TCL
         */
        void responseVoidDataToTcl(const sp<RfxMclMessage>& msg);
        void responseVoidDataToTcl(const sp<RfxMclMessage>& msg, RIL_Errno err);

        void setVDSAuto(bool isAuto, bool isEcc);
        void enableVDSReport(bool enable = true);
        bool hasImsCall(int slotId);
        int getDomain(bool isAuto, bool isEcc);
        int getEccPreferredDomain();

        char* extractSipUri(char *num);

        bool isGsmRat();
        bool isCdmaRat();
        bool isVoiceGsmRat();
        String8 handleNumberWithPause(char* number);
        int handleNumberWithClirString(String8& number, int clir);
        bool isVzWProject();
        int handleClirSpecial(bool isEcc, int clir, char* number);
        int mipcNumPresentationToRilType(int presentation);
        int mipcNamePresentationToRilType(int presentation);

        typedef enum {
            CLIR_DEFAULT = 0,
            CLIR_INVOCATION = 1,
            CLIR_SUPPRESSTION = 2
        } ClirMode;

        typedef enum {
            CALL_DOMAIN_AUTO = 0,        // LTE(IMS), WG(CS), 1x(C2K)
            CALL_DOMAIN_CS_ONLY = 1,     // WG(CS)
            CALL_DOMAIN_3GPP_ONLY = 2,   // LTE(IMS), WG(CS)
            CALL_DOMAIN_3GPP2_ONLY = 3,  // 1x(C2K)
            CALL_DOMAIN_IMS_AND_1X = 4,  // LTE(IMS), 1x(C2K)
            CALL_DOMAIN_CS_AND_1X = 5    // WG(CS), 1x(C2K)
        } CallDomain;

        typedef enum {
            ECC_PREFERRED_DOMAIN_AUTO = 0,
            ECC_PREFERRED_DOMAIN_3GPP = 1,
            ECC_PREFERRED_DOMAIN_3GPP2 = 2
        } EccPreferredDomain;

        typedef enum {
            PRESENTATION_ALLOWED = 0,
            PRESENTATION_RESTRICTED = 1,
            PRESENTATION_UNKNOWN = 2,
            PRESENTATION_PAYPHONE = 3,
        } NumberPrensentation;

        enum {
            SRVCC_STARTED = 0,
            SRVCC_COMPLETED = 1,
            SRVCC_FAILED = 2,
            SRVCC_CANCELED = 3,
        };

        enum {
            VDM_ADS_RESULT_CS = 0,
            VDM_ADS_RESULT_IMS_VOLTE,
            VDM_ADS_RESULT_IMS_WIFI,
            VDM_ADS_RESULT_IMS_NR,
            VDM_ADS_RESULT_REJECT,
            VDM_ADS_RESULT_IMS_VODATA_1,  // VoData on protocol 1
            VDM_ADS_RESULT_IMS_VODATA_2,  // VoData on protocol 2
            VDM_ADS_RESULT_IMS_VODATA_3,  // VoData on protocol 3
            VDM_ADS_RESULT_IMS_VODATA_4,  // VoData on protocol 4
        };
};

#endif
