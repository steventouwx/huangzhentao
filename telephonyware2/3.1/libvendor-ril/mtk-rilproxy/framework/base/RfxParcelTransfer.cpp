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
 /*
  * RfxParcelTranfer.cpp
  *
  *  Created on: 2018/08/21
  *  Author: MTK06564
  *
  */
#include "RfxParcelTransfer.h"
#include "RfxParcelTransferUtils.h"
#include <prop/properties.h>
#include "../include/telephony/ril.h"  //Note, This Ril.h is privide by libvendor-ril
#define LOG_TAG "RfxParcelTransfer"
#define RIL_UNSOL_NOT_SUPPORT -1


static ReqParcelTransferInfo s_parcel_transfer_request[] = {
    #include "parcel_transfer_ril_commands.h"
};
static ReqParcelTransferInfo s_parcel_transfer_mtk_request[] = {
    #include "parcel_transfer_mtk_ril_commands.h"
};
static ReqParcelTransferInfo s_parcel_transfer_mtk_ivt_request[] = {
    #include "parcel_transfer_mtk_ivt_ril_commands.h"
};
static UnsolParcelTransferInfo s_parcel_transfer_urc[] = {
    #include "parcel_transfer_unsol_commands.h"
};
static UnsolParcelTransferInfo s_parcel_transfer_mtk_urc[] = {
    #include "parcel_transfer_mtk_unsol_commands.h"
};
static UnsolParcelTransferInfo s_parcel_transfer_mtk_ivt_urc[] = {
    #include "parcel_transfer_mtk_ivt_unsol_commands.h"
};

#ifdef HAVE_AEE_FEATURE
void triggerNE(char *pErrMsg) {
    if (pErrMsg != NULL) {
        aee_system_exception("ril-proxy", NULL, DB_OPT_DEFAULT, pErrMsg);
        exit(0);
    } else {
        assert(0);
    }
}
#endif


int RfxParcelTransfer::requsetIdTransfer(int requset){
    return requset;
}

int RfxParcelTransfer::unsolIdTransfer(int unsol){
    return unsol;
}

ReqParcelTransferInfo * RfxParcelTransfer::getReqParcelTransferInfo(int request) {
    ReqParcelTransferInfo *ReqInfo = NULL;
    RFX_LOG_V(LOG_TAG, "RfxParcelTransfer::getReqParcelTransferInfo request = %d.", request);
    if(request >= 1 && request < (int32_t)NUM_ELEMS(s_parcel_transfer_request)){
        RFX_LOG_V(LOG_TAG, "RfxParcelTransfer::ReqParcelTransfer request = %d, parcel_transfer_ril_commands.h", request);
        ReqInfo = &(s_parcel_transfer_request[request]);
    }  else if(request >= RIL_REQUEST_VENDOR_IVT_BASE && (request < RIL_REQUEST_VENDOR_IVT_BASE +
            (int32_t) NUM_ELEMS(s_parcel_transfer_mtk_ivt_request))){
        RFX_LOG_V(LOG_TAG, "RfxParcelTransfer::ReqParcelTransfer request = %d, parcel_transfer_mtk_ivt_ril_commands.h", request);
        ReqInfo = &(s_parcel_transfer_mtk_ivt_request[request-RIL_REQUEST_VENDOR_IVT_BASE]);
    } else if(request >= RIL_REQUEST_VENDOR_BASE) {
        RFX_LOG_V(LOG_TAG, "RfxParcelTransfer::ReqParcelTransfer request = %d, parcel_transfer_mtk_ril_commands.h", request);
        for (int i = 0; i < (int) NUM_ELEMS(s_parcel_transfer_mtk_request); i++) {
            if(request == s_parcel_transfer_mtk_request[i].request) {
                ReqInfo = &(s_parcel_transfer_mtk_request[i]);
                break;
            }
        }
    }
    return ReqInfo;
}

Parcel* RfxParcelTransfer::ReqParcelTransfer(int request,  Parcel* p, int slot){
    ReqParcelTransferInfo *ReqInfo = getReqParcelTransferInfo(request);

    if(ReqInfo == NULL) {
        RFX_LOG_W(LOG_TAG, "[RfxParcelTransfer][ReqParcelTransfer] Should not be here");
        return NULL;
    }
    RFX_LOG_D(LOG_TAG, "RfxParcelTransfer::ReqParcelTransfer request = %d", request);
    return ReqInfo->ReqParcelFunction(slot, requsetIdTransfer(request),p);
}

Parcel* RfxParcelTransfer::RespParcelTransfer(int response,  Parcel* p, int* err){
    ReqParcelTransferInfo *RespInfo = getReqParcelTransferInfo(response);

    if(RespInfo == NULL) {
        RFX_LOG_W(LOG_TAG, "[RfxParcelTransfer][RespParcelTransfer] Should not be here");
        return NULL;
    }
    return RespInfo->RespParcelFunction(RIL_VERSION, response, p, err);
}

Parcel* RfxParcelTransfer::UnsolParcelTransfer(int *unsolid,  Parcel* p) {
    UnsolParcelTransferInfo *UnsolInfo = NULL;
    int unsol = unsolIdTransfer(*unsolid);
    if(unsol >= RIL_UNSOL_RESPONSE_BASE && (unsol < RIL_UNSOL_RESPONSE_BASE +
            (int32_t)NUM_ELEMS(s_parcel_transfer_urc))){
        RFX_LOG_V(LOG_TAG, "[RfxParcelTransfer][UnsolParcelTransfer] use s_parcel_transfer_request");
        UnsolInfo = &(s_parcel_transfer_urc[unsol-RIL_UNSOL_RESPONSE_BASE]);
    } else if(unsol >= RIL_UNSOL_VENDOR_IVT_BASE && (unsol < RIL_UNSOL_VENDOR_IVT_BASE +
            (int32_t)NUM_ELEMS(s_parcel_transfer_mtk_ivt_urc))){
        RFX_LOG_V(LOG_TAG, "[RfxParcelTransfer][UnsolParcelTransfer] use s_parcel_transfer_mtk_ivt_request");
        UnsolInfo = &(s_parcel_transfer_mtk_ivt_urc[unsol-RIL_UNSOL_VENDOR_IVT_BASE]);
    } else if(unsol >= RIL_UNSOL_VENDOR_BASE){
        RFX_LOG_V(LOG_TAG, "[RfxParcelTransfer][UnsolParcelTransfer] use s_parcel_transfer_mtk_request");
        for (int i = 0; i < (int) NUM_ELEMS(s_parcel_transfer_mtk_urc); i++) {
            if(unsol == s_parcel_transfer_mtk_urc[i].unsol) {
                UnsolInfo = &(s_parcel_transfer_mtk_urc[i]);
                break;
            }
        }
    }
    if (UnsolInfo == NULL) {
        RFX_LOG_W(LOG_TAG, "[RfxParcelTransfer][UnsolParcelTransfer] Should not be here");
        return NULL;
    }
    *unsolid = unsol;
    RFX_LOG_D(LOG_TAG, "RfxParcelTransfer::UnsolParcelTransfer unsol = %d", *unsolid);
    return UnsolInfo->UnsolParcelFunction(RIL_VERSION, unsol, p);
}

void RfxParcelTransfer::checkTableIndex() {
    // -->Todo, similar with RfxTransfrUtils::checkTableIndex
    for (int i = 0; i < (int)NUM_ELEMS(s_parcel_transfer_request); i++) {
        if (i != s_parcel_transfer_request[i].request) {
        RLOGE("s_parcel_transfer_request table error. index : %d, requestNumber = %d",
                    i, s_parcel_transfer_request[i].request);
        #ifdef HAVE_AEE_FEATURE
        char *msg = NULL;
            asprintf(&msg, "s_parcel_transfer_request table error. index : %d, requestNumber = %d",
                    i, s_parcel_transfer_request[i].request);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }

    for (int i = 0; i < (int)NUM_ELEMS(s_parcel_transfer_urc); i++) {
        if (i + RIL_UNSOL_RESPONSE_BASE != s_parcel_transfer_urc[i].unsol) {
            RLOGE("s_parcel_transfer_urc table error. index : %d, requestNumber = %d",
                    i, s_parcel_transfer_urc[i].unsol);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "s_parcel_transfer_urc table error. index : %d, requestNumber \
                    = %d", i, s_parcel_transfer_urc[i].unsol);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }

#if 0
    for (int i = 0; i < (int)NUM_ELEMS(s_parcel_transfer_mtk_request); i++) {
        if (i + RIL_REQUEST_VENDOR_BASE != s_parcel_transfer_mtk_request[i].request) {
            RLOGE("s_parcel_transfer_mtk_request table error. index : %d, requestNumber = %d",
                    i, s_parcel_transfer_mtk_request[i].request);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "s_parcel_transfer_mtk_request table error. index : %d, requestNumber \
                    = %d", i, s_parcel_transfer_mtk_request[i].request);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }

    for (int i = 0; i < (int)NUM_ELEMS(s_parcel_transfer_mtk_urc); i++) {
        if (i + RIL_UNSOL_VENDOR_BASE != s_parcel_transfer_mtk_urc[i].unsol) {
            RLOGE("s_parcel_transfer_mtk_urc table error. index : %d, requestNumber = %d",
                    i, s_parcel_transfer_mtk_urc[i].unsol);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "s_parcel_transfer_mtk_urc table error. index : %d, \
                    requestNumber = %d", i, s_parcel_transfer_mtk_urc[i].unsol);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }
#endif

    // check ivt table
    for (int i = 0; i < (int)NUM_ELEMS(s_parcel_transfer_mtk_ivt_request); i++) {
        if (i + RIL_REQUEST_VENDOR_IVT_BASE != s_parcel_transfer_mtk_ivt_request[i].request) {
            RLOGE("s_parcel_transfer_mtk_ivt_request table error. index : %d, requestNumber = %d",
                    i, s_parcel_transfer_mtk_ivt_request[i].request);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "s_parcel_transfer_mtk_ivt_request table error. index : %d, requestNumber \
                    = %d", i, s_parcel_transfer_mtk_ivt_request[i].request);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }

     for (int i = 0; i < (int)NUM_ELEMS(s_parcel_transfer_mtk_ivt_urc); i++) {
        if (i + RIL_UNSOL_VENDOR_IVT_BASE != s_parcel_transfer_mtk_ivt_urc[i].unsol) {
            RLOGE("s_parcel_transfer_mtk_ivt_urc table error. index : %d, requestNumber = %d",
                    i, s_parcel_transfer_mtk_ivt_urc[i].unsol);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "s_parcel_transfer_mtk_ivt_urc table error. index : %d, requestNumber \
                    = %d", i, s_parcel_transfer_mtk_ivt_urc[i].unsol);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
     }

}

