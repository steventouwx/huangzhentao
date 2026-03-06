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
  * rfx_data_to_parcel_utils.cpp
  *
  *  Created on: 2015/09/02
  *  Author: MTK10602
  *
  */
#include "RfxTransferUtils.h"
#include "RfxRilAdapter.h"
#include "utils/Mutex.h"
//#if defined(__ANDROID__)
#include <prop/properties.h>
//#else
//#include <sncfg/local_system_properties.h>
//#endif
#include <inttypes.h>
#if defined(TELEPHONYWARE)
#define    COMMAND_SUPPORETD(a) a
#else
#define    COMMAND_SUPPORETD(a)
#endif
using ::android::Mutex;
using android::status_t;

extern "C" const char * requestToString(int request);

#define PRINTBUF_SIZE 8096

#if 0

#define LOG_ENABLE_FLAG "persist.it.log.enable.flag"
#define appendPrintBuf(x...)    snprintf(printBuf+strlen(printBuf),PRINTBUF_SIZE-strlen(printBuf), x)
//#define dataStart           sprintf(printBuf, "%s{", printBuf)
//#define dataEnd             sprintf(printBuf,  "%s}", printBuf)
#define clearPrintBuf           printBuf[0] = 0
#define removeLastChar          printBuf[strlen(printBuf)-1] = 0

#define dataStart do {\
        sprintf(printBuf_Temp, "%s{", printBuf);\
        sprintf(printBuf, "%s", printBuf_Temp);\
    }while(0)

#define dataEnd do {\
        sprintf(printBuf_Temp, "%s}", printBuf);\
        sprintf(printBuf,  "%s}", printBuf_Temp);\
    }while(0)

static Mutex printMutex;

#define IT_RESPONSE_START(req, slot, token, e)   \
                        do {\
                            printMutex.lock();\
                            clearPrintBuf;\
                            snprintf(printBuf, PRINTBUF_SIZE, "\nTELEPHONY_IT,response:%s,slot%d,token=%x, RIL_Errno=%d, ", (char*)req, (int)slot, (int)token,(int)e);\
                            } while(0)

#define IT_REQ_START(req, slot, token)   \
                        do {\
                            printMutex.lock();\
                            clearPrintBuf;\
                             snprintf(printBuf, PRINTBUF_SIZE,"\nTELEPHONY_IT,request:%s,slot%d,token=%x, ", (char*)req, (int)slot, (int)token);\
                        } while(0)

#define IT_UNSOL_START(unsol, slot) \
                        do{\
                            printMutex.lock();\
                            clearPrintBuf;\
                             snprintf(printBuf, PRINTBUF_SIZE,"\nTELEPHONY_IT,unsol:%s,slot%d, ", (char*)unsol, (int)slot);\
                        }while(0)

#define IT_PRINT_LOG  \
                        do{\
                            char value[PROPERTY_VALUE_MAX] = "0";\
                            property_get(LOG_ENABLE_FLAG, value, "0");\
                            RFX_LOG_D(LOG_TAG,"%s\n", printBuf);\
                            if(0 == strcmp(value, "1")) \
                                printf("%s\n", printBuf);\
                            printMutex.unlock();\
                        }while(0)
#else
#define appendPrintBuf(x...)
#define dataStart
#define dataEnd
#define clearPrintBuf
#define removeLastChar

#define IT_RESPONSE_START(req, slot, token, e)
#define IT_REQ_START(req, slot, token)
#define IT_UNSOL_START(unsol, slot)
#define IT_PRINT_LOG


#endif

static char printBuf[PRINTBUF_SIZE];
static char printBuf_Temp[PRINTBUF_SIZE];

static DataToParcelInfo s_data_to_parcel_request[] = {
    #include "data_to_parcel_ril_commands.h"
};
static DataToParcelInfo s_data_to_parcel_mtk_request[] = {
    #include "data_to_parcel_mtk_ril_commands.h"
};
static ParcelToDataInfo s_parcel_to_data_urc[] = {
    #include "parcel_to_data_unsol_commands.h"
};
static ParcelToDataInfo s_parcel_to_data_mtk_urc[] = {
    #include "parcel_to_data_mtk_unsol_commands.h"
};
///MT2635 add ivt request @{
static DataToParcelInfo s_data_to_parcel_mtk_ivt_request[] = {
    #include "data_to_parcel_mtk_ivt_ril_commands.h"
};
static ParcelToDataInfo s_parcel_to_data_mtk_ivt_urc[] = {
    #include "parcel_to_data_mtk_ivt_unsol_commands.h"
};
/// @}

DataToParcelInfo * RfxTransferUtils::getDataToParcelInfo(int request) {
    DataToParcelInfo *parcelInfo = NULL;
    if (request >= 1 && request < (int32_t)NUM_ELEMS(s_data_to_parcel_request)) {
        RFX_LOG_D(LOG_TAG, "[RFX] use s_data_to_parcel_request");
        parcelInfo = &(s_data_to_parcel_request[request]);
    }  else if (request >= RIL_REQUEST_VENDOR_IVT_BASE && (request < RIL_REQUEST_VENDOR_IVT_BASE
                            + (int32_t) NUM_ELEMS(
                                    s_data_to_parcel_mtk_ivt_request))) {
        RFX_LOG_D(LOG_TAG, "[RFX] use s_data_to_parcel_mtk_ivt_request");
        parcelInfo = &(s_data_to_parcel_mtk_ivt_request[request - RIL_REQUEST_VENDOR_IVT_BASE]);
    } else if (request >= RIL_REQUEST_VENDOR_BASE ) {
        RFX_LOG_D(LOG_TAG, "[RFX] use s_data_to_parcel_mtk_request");
        for (int i = 0; i < (int) NUM_ELEMS(s_data_to_parcel_mtk_request); i++) {
            if(request == s_data_to_parcel_mtk_request[i].request) {
                parcelInfo = &(s_data_to_parcel_mtk_request[i]);
                break;
            }
        }
    }
    return parcelInfo;
}

ParcelToDataInfo * RfxTransferUtils::getParcelToDataInfo(int urc) {
    ParcelToDataInfo *dataInfo = NULL;
    if (urc >= RIL_UNSOL_RESPONSE_BASE && (urc < RIL_UNSOL_RESPONSE_BASE +
            (int32_t)NUM_ELEMS(s_parcel_to_data_urc))) {
        dataInfo = &(s_parcel_to_data_urc[urc - RIL_UNSOL_RESPONSE_BASE]);
    }else if (urc >= RIL_UNSOL_VENDOR_IVT_BASE && (urc < RIL_UNSOL_VENDOR_IVT_BASE +
            (int32_t)NUM_ELEMS(s_parcel_to_data_mtk_ivt_urc))) {
        dataInfo = &(s_parcel_to_data_mtk_ivt_urc[urc - RIL_UNSOL_VENDOR_IVT_BASE]);
    }  else if (urc >= RIL_UNSOL_VENDOR_BASE ) {
        for (int i = 0; i < (int) NUM_ELEMS(s_parcel_to_data_mtk_urc); i++) {
            if(urc == s_parcel_to_data_mtk_urc[i].urc) {
                dataInfo = &(s_parcel_to_data_mtk_urc[i]);
                break;
            }
        }
    }
    return dataInfo;
}

Parcel* RfxTransferUtils::dataToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    // Parcel *parcel;
    DataToParcelInfo *parcelInfo = getDataToParcelInfo(request);
    if(parcelInfo == NULL) {
        RFX_LOG_D(LOG_TAG, "[RFX][dataToParcel]request=%d Should not be here", request);
        return NULL;
    }
    return parcelInfo->dataToParcelFunction(request, token, data, datalen, t);
}

void RfxTransferUtils::parcelToDataResponse(RIL_Token t, RIL_Errno e, int request,
        Parcel * parcel, int slotId) {
    DataToParcelInfo *parcelInfo = getDataToParcelInfo(request);
    if(parcelInfo == NULL) {
        RFX_LOG_D(LOG_TAG, "[RFX][dataToParcel]request=%d Should not be here", request);
        return;
    }
    RFX_LOG_V(LOG_TAG, "parcelToDataResponse request %d", parcelInfo->request);
    parcelInfo->parcleToDataFunction(t, e, request, parcel, slotId);
}

void RfxTransferUtils::parcelToDataUrc(int urc, Parcel * parcel, int slotId) {
    ParcelToDataInfo *dataInfo = getParcelToDataInfo(urc);
    if(dataInfo == NULL) {
        RFX_LOG_D(LOG_TAG, "[RFX][parcelToDataUrc]urc=%d Should not be here", urc);
        return;
    }
    RFX_LOG_V(LOG_TAG, "parcelToDataUrc urc %d", dataInfo->urc);
    dataInfo->parcleToDataFunction(NULL, RIL_E_SUCCESS, urc, parcel, slotId);
}
#if defined(TELEPHONYWARE)
int RfxTransferUtils::checkReqSupport(int request){
    DataToParcelInfo *parcelInfo = getDataToParcelInfo(request);
    if(parcelInfo != NULL) {
        return parcelInfo->support;
    }
    return 0;
}

int RfxTransferUtils::checkUnsolSupport(int urc){
    ParcelToDataInfo *dataInfo = getParcelToDataInfo(urc);
    if(dataInfo != NULL) {
        return dataInfo->support;
    }
    return 0;
}
#endif
void RfxTransferUtils::checkTableIndex() {
    for (int i = 0; i < (int)NUM_ELEMS(s_data_to_parcel_request); i++) {
        if (i != s_data_to_parcel_request[i].request) {
            RLOGE("data_to_parcel_ril_commands table error. index : %d, requestNumber = %d",
                    i, s_data_to_parcel_request[i].request);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "data_to_parcel_ril_commands table error. index : %d, requestNumber = %d",
                        i, s_data_to_parcel_request[i].request);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }
    for (int i = 0; i < (int)NUM_ELEMS(s_parcel_to_data_urc); i++) {
        if (i + RIL_UNSOL_RESPONSE_BASE != s_parcel_to_data_urc[i].urc) {
            RLOGE("parcel_to_data_unsol_commands table error. index : %d, requestNumber = %d",
                    i, s_parcel_to_data_urc[i].urc);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "parcel_to_data_unsol_commands table error. index : %d, requestNumber \
                    = %d", i, s_parcel_to_data_urc[i].urc);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }
#if 0//remove mtk_request check, as the 5G mtk_ril.h is not consistent
    for (int i = 0; i < (int)NUM_ELEMS(s_data_to_parcel_mtk_request); i++) {
        if (i + RIL_REQUEST_VENDOR_BASE != s_data_to_parcel_mtk_request[i].request) {
            RLOGE("data_to_parcel_mtk_ril_commands table error. index : %d, requestNumber = %d",
                    i, s_data_to_parcel_mtk_request[i].request);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "data_to_parcel_mtk_ril_commands table error. index : %d, requestNumber \
                    = %d", i, s_data_to_parcel_mtk_request[i].request);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }

    for (int i = 0; i < (int)NUM_ELEMS(s_parcel_to_data_mtk_urc); i++) {
        if (i + RIL_UNSOL_VENDOR_BASE != s_parcel_to_data_mtk_urc[i].urc) {
            RLOGE("parcel_to_data_mtk_unsol_commands table error. index : %d, requestNumber = %d",
                    i, s_parcel_to_data_mtk_urc[i].urc);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "parcel_to_data_mtk_unsol_commands table error. index : %d, \
                    requestNumber = %d", i, s_parcel_to_data_mtk_urc[i].urc);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }
#endif
    ///MT2635: check ivt table
    for (int i = 0; i < (int)NUM_ELEMS(s_data_to_parcel_mtk_ivt_request); i++) {
        if (i + RIL_REQUEST_VENDOR_IVT_BASE != s_data_to_parcel_mtk_ivt_request[i].request) {
            RLOGE("data_to_parcel_mtk_ivt_request table error. index : %d, requestNumber = %d",
                    i, s_data_to_parcel_mtk_ivt_request[i].request);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "data_to_parcel_mtk_ivt_request table error. index : %d, requestNumber \
                    = %d", i, s_data_to_parcel_mtk_ivt_request[i].request);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }
    for (int i = 0; i < (int)NUM_ELEMS(s_parcel_to_data_mtk_ivt_urc); i++) {
        if (i + RIL_UNSOL_VENDOR_IVT_BASE != s_parcel_to_data_mtk_ivt_urc[i].urc) {
            RLOGE("parcel_to_data_mtk_ivt_urc table error. index : %d, requestNumber = %d",
                    i, s_parcel_to_data_mtk_ivt_urc[i].urc);
            #ifdef HAVE_AEE_FEATURE
            char *msg = NULL;
            asprintf(&msg, "parcel_to_data_mtk_ivt_urc table error. index : %d, requestNumber \
                    = %d", i, s_parcel_to_data_mtk_ivt_urc[i].urc);
            triggerNE(msg);
            free(msg);
            #else
            RFX_ASSERT(0);
            #endif
        }
    }
}

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
#if 0
Parcel* requestKeepaliveToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
        Parcel *parcel = new Parcel();
        RIL_RequestKeepalive_Pro *status;
        RequestInfo *requestInfo = (RequestInfo *)t;

        IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
        dataStart;
        fillHeader(parcel, request, token);
        status = (RIL_RequestKeepalive_Pro*)data;
        appendPrintBuf("type = %d, ", status->type);
        parcel->writeInt32(status->type);
        appendPrintBuf("sourceAddress=%d.%d.%d.%d, ", status->sourceAddress[0] & 0xff,
                status->sourceAddress[1] & 0xff, status->sourceAddress[2] & 0xff,
                status->sourceAddress[3] & 0xff);
        parcel->writeByteArray(MAX_INADDR_LEN, (uint8_t*)status->sourceAddress);
        appendPrintBuf("sourcePort=%d, ", status->sourcePort);
        parcel->writeInt32(status->sourcePort);
        appendPrintBuf("destinationAddress=%d.%d.%d.%d, ", status->destinationAddress[0] & 0xff,
                status->destinationAddress[1] & 0xff, status->destinationAddress[2] & 0xff,
                status->destinationAddress[3] & 0xff);
        parcel->writeByteArray(MAX_INADDR_LEN, (uint8_t*)status->destinationAddress);
        appendPrintBuf("destinationPort=%d, ", status->destinationPort);
        parcel->writeInt32(status->destinationPort);
        appendPrintBuf("netif_id=%d, ", status->netif_id);
        parcel->writeInt32(status->netif_id);
        appendPrintBuf("keepIdleTime=%d, ", status->keepIdleTime);
        parcel->writeInt32(status->keepIdleTime);
        appendPrintBuf("keepIntervalTime=%d, ", status->keepIntervalTime);
        parcel->writeInt32(status->keepIntervalTime);
        appendPrintBuf("retryCount=%d, ", status->retryCount);
        parcel->writeInt32(status->retryCount);
        dataEnd;
        IT_PRINT_LOG;
        return parcel;
}
#endif
Parcel* callForwardToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_CallForwardInfo *cff = (RIL_CallForwardInfo *) data;
    parcel->writeInt32(cff->status);
    parcel->writeInt32(cff->reason);
    parcel->writeInt32(cff->serviceClass);
    parcel->writeInt32(cff->toa);

    pString16 = strdup8to16(cff->number, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    parcel->writeInt32(cff->timeSeconds);

    return parcel;
}

Parcel* cdmaBrSmsCnfToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    int num = datalen/sizeof(RIL_CDMA_BroadcastSmsConfigInfo *);
    parcel->writeInt32(num);

    RIL_CDMA_BroadcastSmsConfigInfo **cdmaBciPtrs = (RIL_CDMA_BroadcastSmsConfigInfo **) data;
    for (int i = 0; i < num; i++) {
        parcel->writeInt32(cdmaBciPtrs[i]->service_category);
        parcel->writeInt32(cdmaBciPtrs[i]->language);
        parcel->writeInt32(cdmaBciPtrs[i]->selected);
    }

    return parcel;
}

Parcel* cdmaSmsToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_CDMA_SMS_Message *rcsm = (RIL_CDMA_SMS_Message *) data;
    return cdmaSmsToParcelInternal(parcel, rcsm);
}

Parcel* cdmaSmsAckToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_CDMA_SMS_Ack *rcsa = (RIL_CDMA_SMS_Ack *) data;
    parcel->writeInt32(rcsa->uErrorClass);
    parcel->writeInt32(rcsa->uSMSCauseCode);

    return parcel;
}

Parcel* cdmaSubscriptionSourceToParcel(int request, int token,
        void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    fillHeader(parcel, request, token);
    return parcel;
}

Parcel* dataCallToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    RequestInfo *requestInfo = (RequestInfo *)t;
    if(parcel == NULL) {
        return NULL;
    }

    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    dataStart;

    fillHeader(parcel, request, token);
    int countStrings = datalen/sizeof(char *);  // 8; // according to RIL.java
    char **pStrings = (char **) data;
    parcel->writeInt32(countStrings);
    appendPrintBuf("%s,countStrings=%d", printBuf,countStrings);
    for (int i = 0; i < countStrings; i++) {
        pString16 = strdup8to16(pStrings[i], &len16);
        parcel->writeString16(pString16, len16);
        free(pString16);
        appendPrintBuf("%s, %s", printBuf,pStrings[i]);
    }
    dataEnd;
    IT_PRINT_LOG;

    return parcel;
}

Parcel* dataProfileToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    // useless
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    return parcel;
}

Parcel* dialToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_Dial *dial = (RIL_Dial *) data;

    pString16 = strdup8to16(dial->address, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    parcel->writeInt32(dial->clir);
    if (dial->uusInfo != NULL) {
        parcel->writeInt32(1);
        parcel->writeInt32(dial->uusInfo->uusType);
        parcel->writeInt32(dial->uusInfo->uusDcs);
        if (dial->uusInfo->uusLength == 0) {
            parcel->writeInt32(-1);
        } else {
            parcel->writeInt32(dial->uusInfo->uusLength);
            parcel->writeByteArray(dial->uusInfo->uusLength, (uint8_t *) dial->uusInfo->uusData);
        }
    } else {
        parcel->writeInt32(0);
    }

    return parcel;
}

Parcel* fd_ModeToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_FdModeStructure *args = (RIL_FdModeStructure *) data;
    parcel->writeInt32(args->paramNumber);
    if (args->paramNumber >= 1) {
        parcel->writeInt32(args->mode);
    }
    if (args->paramNumber >= 2) {
        parcel->writeInt32(args->parameter1);
    }
    if (args->paramNumber >= 3) {
        parcel->writeInt32(args->parameter2);
    }

    return parcel;
}

Parcel* gsmBrSmsCnfToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    int num = datalen / sizeof(RIL_GSM_BroadcastSmsConfigInfo *);
    parcel->writeInt32(num);
    RIL_GSM_BroadcastSmsConfigInfo **gsmBciPtrs = (RIL_GSM_BroadcastSmsConfigInfo **) data;
    for (int i = 0; i < num; i++) {
        parcel->writeInt32(gsmBciPtrs[i]->fromServiceId);
        parcel->writeInt32(gsmBciPtrs[i]->toServiceId);
        parcel->writeInt32(gsmBciPtrs[i]->fromCodeScheme);
        parcel->writeInt32(gsmBciPtrs[i]->toCodeScheme);
        parcel->writeInt32(gsmBciPtrs[i]->selected);
    }

    return parcel;
}

Parcel* imsCdmaSmsToParcel(Parcel *parcel, RIL_CDMA_SMS_Message *rcsm) {
    return cdmaSmsToParcelInternal(parcel, rcsm);
}

Parcel* imsGsmSmsToParcel(Parcel *parcel, void **gsmMessage, int datalen, RIL_Token t) {
    int **args = (int **)gsmMessage;
    char16_t *pString16;
    size_t len16 = 0;

    int32_t countStrings =  (datalen - sizeof(RIL_RadioTechnologyFamily) - sizeof(uint8_t) -
            sizeof(int32_t))/sizeof(char *);  // 2; // according to RIL.java
    // args = args + 1;
    char **pStrings = (char **)args;
    if (pStrings == NULL) {
        parcel->writeInt32(-1);
    } else {
        parcel->writeInt32(countStrings);
        for (int i = 0; i < countStrings; i++) {
            pString16 = strdup8to16(pStrings[i], &len16);
            parcel->writeString16(pString16, len16);
            free(pString16);
        }
    }
    return parcel;
}

Parcel* imsSmsToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_IMS_SMS_Message *rism = (RIL_IMS_SMS_Message *) data;

    parcel->writeInt32(rism->tech);
    parcel->write(&(rism->retry), sizeof(uint8_t));
    parcel->write(&(rism->messageRef), sizeof(int32_t));
    if (RADIO_TECH_3GPP == rism->tech) {
        return imsGsmSmsToParcel(parcel, (void **)rism->message.gsmMessage, datalen, t);
    } else if (RADIO_TECH_3GPP2 == rism->tech) {
        return imsCdmaSmsToParcel(parcel, (RIL_CDMA_SMS_Message *) rism->message.cdmaMessage);
    } else {
        RFX_LOG_E(LOG_TAG, "rism->tech = %d should not be here!", rism->tech);
        return parcel;
    }
}

Parcel* intsToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    RequestInfo *requestInfo = (RequestInfo *)t;
    if (parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    int *pInts = (int *) data;
    int countInts = datalen/sizeof(int);
    parcel->writeInt32(countInts);
    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    dataStart;
    appendPrintBuf("d:" ,countInts);
    for (int i = 0; i < countInts; i++) {
        appendPrintBuf("%d, ",pInts[i]);
        parcel->writeInt32(pInts[i]);
    }
    dataEnd;
    IT_PRINT_LOG;
    return parcel;
}

Parcel* nvReadItemToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if (parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_NV_ReadItem *nvri = (RIL_NV_ReadItem *) data;
    parcel->writeInt32(nvri->itemID);

    return parcel;
}

Parcel* nvWriteItemToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_NV_WriteItem *nvwi = (RIL_NV_WriteItem *) data;
    parcel->writeInt32(nvwi->itemID);

    pString16 = strdup8to16(nvwi->value, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    return parcel;
}

Parcel* phbEntryToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_PhbEntryStructure *args = (RIL_PhbEntryStructure *) data;
    parcel->writeInt32(args->type);
    parcel->writeInt32(args->index);

    pString16 = strdup8to16(args->number, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    parcel->writeInt32(args->ton);

    pString16 = strdup8to16(args->alphaId, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    return parcel;
}

Parcel* radioCapabilityToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    RequestInfo *requestInfo = (RequestInfo *)t;
    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    dataStart;

    fillHeader(parcel, request, token);
    RIL_RadioCapability *rc = (RIL_RadioCapability *) data;
    parcel->writeInt32(rc->version);
    appendPrintBuf("version=%d, ", rc->version);
    parcel->writeInt32(rc->session);
    appendPrintBuf("session=%d, ", rc->session);
    parcel->writeInt32(rc->phase);
    appendPrintBuf("phase=%d, ", rc->phase);
    parcel->writeInt32(rc->rat);
    appendPrintBuf("rat=0x%X, ", rc->rat);
    appendPrintBuf("rc->logicalModemUuid=%s, ", rc->logicalModemUuid);
    pString16 = strdup8to16(rc->logicalModemUuid, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    parcel->writeInt32(rc->status);
    appendPrintBuf("rc->status=%d, ", rc->status);
    dataEnd;
    IT_PRINT_LOG;

    return parcel;
}


Parcel* rawToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    RequestInfo *requestInfo = (RequestInfo *)t;
    if(parcel == NULL) {
        return NULL;
    }

    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    dataStart;

    fillHeader(parcel, request, token);
    if (data == NULL) {
        parcel->writeInt32(-1);
        appendPrintBuf("-1 ");
    } else {
        parcel->writeInt32(datalen);
        appendPrintBuf("datalen=%d ",datalen);
        parcel->write(data, datalen);
        appendPrintBuf("datalen=%s ",(char *)data);
    }
    dataEnd;
    IT_PRINT_LOG;

    return parcel;
}

Parcel* rilCdmaSmsWriteArgsToParcel(int request, int token,
        void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    int digitCount;
    uint8_t uct;
    int digitLimit;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_CDMA_SMS_WriteArgs *rcsw = (RIL_CDMA_SMS_WriteArgs *) data;
    parcel->writeInt32(rcsw->status);
    parcel->writeInt32(rcsw->message.uTeleserviceID);
    parcel->write(&(rcsw->message.bIsServicePresent), sizeof(uct));
    parcel->writeInt32(rcsw->message.uServicecategory);
    parcel->writeInt32(rcsw->message.sAddress.digit_mode);
    parcel->writeInt32(rcsw->message.sAddress.number_mode);
    parcel->writeInt32(rcsw->message.sAddress.number_type);
    parcel->writeInt32(rcsw->message.sAddress.number_plan);
    parcel->write(&(rcsw->message.sAddress.number_of_digits), sizeof(uct));

    digitLimit= MIN(rcsw->message.sAddress.number_of_digits, RIL_CDMA_SMS_ADDRESS_MAX);
    for (digitCount = 0; digitCount < digitLimit; digitCount++) {
        parcel->write(&(rcsw->message.sAddress.digits[digitCount]), sizeof(uct));
    }
    parcel->writeInt32(rcsw->message.sSubAddress.subaddressType);
    parcel->write(&(rcsw->message.sSubAddress.odd), sizeof(uint8_t));
    parcel->write(&(rcsw->message.sSubAddress.number_of_digits), sizeof(uct));

    digitLimit= MIN(rcsw->message.sSubAddress.number_of_digits, RIL_CDMA_SMS_SUBADDRESS_MAX);
    for (digitCount = 0 ; digitCount < digitLimit; digitCount ++) {
        parcel->write(&(rcsw->message.sSubAddress.digits[digitCount]), sizeof(uct));
    }
    parcel->writeInt32(rcsw->message.uBearerDataLen);

    digitLimit= MIN(rcsw->message.uBearerDataLen, RIL_CDMA_SMS_BEARER_DATA_MAX);
    for (digitCount = 0 ; digitCount < digitLimit; digitCount ++) {
        parcel->write(&(rcsw->message.aBearerData[digitCount]), sizeof(uct));
    }

    return parcel;
}

Parcel* setInitialAttachApnToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }
    char16_t *pString16;
    size_t len16 = 0;

    RequestInfo *requestInfo = (RequestInfo *)t;

    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    dataStart;

    fillHeader(parcel, request, token);
    RIL_InitialAttachApn *pf = (RIL_InitialAttachApn *) data;
    pString16 = strdup8to16(pf->apn, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    appendPrintBuf("%s apn=%s:", printBuf,pf->apn);
    pString16 = strdup8to16(pf->protocol, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    appendPrintBuf("%s protocol=%s:", printBuf,pf->protocol);
    parcel->writeInt32(pf->authtype);
    appendPrintBuf("%s authtype=%d:", printBuf,pf->authtype);

    pString16 = strdup8to16(pf->username, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    appendPrintBuf("%s username=%s:", printBuf,pf->username);
    pString16 = strdup8to16(pf->password, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    appendPrintBuf("%s password=%s:", printBuf,pf->password);

    dataEnd;
    IT_PRINT_LOG;

    return parcel;
}

Parcel* simAuthToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_SimAuthStructure *args = (RIL_SimAuthStructure *) data;
    parcel->writeInt32(args->sessionId);
    parcel->writeInt32(args->mode);

    pString16 = strdup8to16(args->param1, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    pString16 = strdup8to16(args->param2, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    if (args->mode == 1) {
        parcel->writeInt32(args->tag);
    }

    return parcel;
}

Parcel* simAuthenticationToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_SimAuthentication *pf = (RIL_SimAuthentication *) data;
    parcel->writeInt32(pf->authContext);

    pString16 = strdup8to16(pf->authData, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    pString16 = strdup8to16(pf->aid, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    return parcel;
}

Parcel* sim_APDUToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    RequestInfo *requestInfo = (RequestInfo *)t;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    dataStart;
    RIL_SIM_APDU *apdu = (RIL_SIM_APDU *) data;
    appendPrintBuf("sessionid=%d,", apdu->sessionid);
    parcel->writeInt32(apdu->sessionid);
    appendPrintBuf("cla=%d,", apdu->cla);
    parcel->writeInt32(apdu->cla);
    appendPrintBuf("nstruction=%d,", apdu->instruction);
    parcel->writeInt32(apdu->instruction);
    appendPrintBuf("p1=%d,", apdu->p1);
    parcel->writeInt32(apdu->p1);
    appendPrintBuf("p2=%d,", apdu->p2);
    parcel->writeInt32(apdu->p2);
    appendPrintBuf("p3=%d,", apdu->p3);
    parcel->writeInt32(apdu->p3);

    appendPrintBuf("data=%s", apdu->data);
    pString16 = strdup8to16(apdu->data, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    dataEnd;
    IT_PRINT_LOG;
    return parcel;
}

Parcel* sim_IOToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    RequestInfo *requestInfo = (RequestInfo *)t;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    RIL_SIM_IO_v6 *v6 = (RIL_SIM_IO_v6 *) data;
    dataStart;
    appendPrintBuf("command= %d,", v6->command);
    parcel->writeInt32(v6->command);
    appendPrintBuf("fileid=%d,", v6->fileid);
    parcel->writeInt32(v6->fileid);

    appendPrintBuf("path=%s,", v6->path);
    pString16 = strdup8to16(v6->path, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    appendPrintBuf("p1=%d,", v6->p1);
    parcel->writeInt32(v6->p1);
    appendPrintBuf("p2=%d,", v6->p2);
    parcel->writeInt32(v6->p2);
    appendPrintBuf("p3=%d,", v6->p3);
    parcel->writeInt32(v6->p3);
    appendPrintBuf("data=%s,", v6->data);
    pString16 = strdup8to16(v6->data, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    appendPrintBuf("pin2=%s,", v6->pin2);
    pString16 = strdup8to16(v6->pin2, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    appendPrintBuf("aidPtr=%s", v6->aidPtr);
    pString16 = strdup8to16(v6->aidPtr, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    dataEnd;
    IT_PRINT_LOG;
    return parcel;
}

Parcel* sim_IO_EXToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_SIM_IO_EX_v6 *simIO = (RIL_SIM_IO_EX_v6 *) data;
    parcel->writeInt32(simIO->command);
    parcel->writeInt32(simIO->fileid);

    pString16 = strdup8to16(simIO->path, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    parcel->writeInt32(simIO->p1);
    parcel->writeInt32(simIO->p2);
    parcel->writeInt32(simIO->p3);

    pString16 = strdup8to16(simIO->data, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    pString16 = strdup8to16(simIO->pin2, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    pString16 = strdup8to16(simIO->aidPtr, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    parcel->writeInt32(simIO->sessionId);

    return parcel;
}

Parcel* smsParamsToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_SmsParams *smsParams = (RIL_SmsParams *) data;
    parcel->writeInt32(4);  // according to RIL.java
    parcel->writeInt32(smsParams->format);
    parcel->writeInt32(smsParams->vp);
    parcel->writeInt32(smsParams->pid);
    parcel->writeInt32(smsParams->dcs);

    return parcel;
}

Parcel* smsWriteToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_SMS_WriteArgs *args = (RIL_SMS_WriteArgs *) data;
    parcel->writeInt32(args->status);
    pString16 = strdup8to16(args->pdu, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    pString16 = strdup8to16(args->smsc, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    return parcel;
}

Parcel* stringToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    RequestInfo *requestInfo = (RequestInfo *)t;

    if(parcel == NULL) {
        return NULL;
    }
    fillHeader(parcel, request, token);

    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    dataStart;
    char *pString = (char *) data;
    appendPrintBuf("%s",pString);
    pString16 = strdup8to16(pString, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    dataEnd;
    IT_PRINT_LOG;

    return parcel;
}

Parcel* stringsToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    RequestInfo *requestInfo = (RequestInfo *)t;
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    int countStrings = datalen / sizeof(char *);
    parcel->writeInt32(countStrings);
    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    char **pString = (char **) data;
    dataStart;
    appendPrintBuf("%d:",countStrings);
    for (int i = 0; i < countStrings; i++) {
        appendPrintBuf("%s,",pString[i]);
        pString16 = strdup8to16(pString[i], &len16);
        parcel->writeString16(pString16, len16);
        free(pString16);
    }
    dataEnd;
    IT_PRINT_LOG;
    return parcel;
}

#if 0
Parcel* uiccAuthenticationToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16;

    fillHeader(parcel, request, token);
    RIL_UICC_Authentication *uiccAuth = (RIL_UICC_Authentication *) data;
    parcel->writeInt32(uiccAuth->session_id);
    pString16 = strdup8to16(uiccAuth->context1, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    pString16 = strdup8to16(uiccAuth->context2, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    return parcel;
}
Parcel* uiccIoToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16;

    fillHeader(parcel, request, token);
    RIL_UICC_IO_v6 *uiccIo = (RIL_UICC_IO_v6 *) data;
    parcel->writeInt32(uiccIo->sessionId);
    parcel->writeInt32(uiccIo->command);
    parcel->writeInt32(uiccIo->fileId);
    pString16 = strdup8to16(uiccIo->path, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    parcel->writeInt32(uiccIo->p1);
    parcel->writeInt32(uiccIo->p2);
    parcel->writeInt32(uiccIo->p3);

    pString16 = strdup8to16(uiccIo->data, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    pString16 = strdup8to16(uiccIo->pin2, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    return parcel;
}
#endif

Parcel* uiccSubscriptonToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_SelectUiccSub *uicc_sub = (RIL_SelectUiccSub *) data;
    parcel->writeInt32(uicc_sub->slot);
    parcel->writeInt32(uicc_sub->app_index);
    parcel->writeInt32(uicc_sub->sub_type);
    parcel->writeInt32(uicc_sub->act_status);

    return parcel;
}

Parcel* voiceRadioTechToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    RequestInfo *requestInfo = (RequestInfo *)t;

    fillHeader(parcel, request, token);

    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    appendPrintBuf("{}");
    IT_PRINT_LOG;

    return parcel;
}

Parcel* voidToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    RequestInfo *requestInfo = (RequestInfo *)t;
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    IT_REQ_START(requestToString(request), requestInfo->socket_id, token);
    appendPrintBuf("{}");
    IT_PRINT_LOG;
    return parcel;
}

Parcel* vsimEventToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_VsimEvent *args = (RIL_VsimEvent *) data;
    parcel->writeInt32(args->transaction_id);
    parcel->writeInt32(args->eventId);
    parcel->writeInt32(args->sim_type);

    return parcel;
}

Parcel* vsimOperationEventToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_VsimOperationEvent *args = (RIL_VsimOperationEvent *) data;
    parcel->writeInt32(args->transaction_id);
    parcel->writeInt32(args->eventId);
    parcel->writeInt32(args->result);
    parcel->writeInt32(args->data_length);

    pString16 = strdup8to16(args->data, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    return parcel;
}

Parcel* writePhbEntryExtToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_PHB_ENTRY *args = (RIL_PHB_ENTRY *) data;
    parcel->writeInt32(args->index);

    pString16 = strdup8to16(args->number, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    parcel->writeInt32(args->type);

    pString16 = strdup8to16(args->text, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    parcel->writeInt32(args->hidden);

    pString16 = strdup8to16(args->group, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    pString16 = strdup8to16(args->adnumber, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    parcel->writeInt32(args->adtype);

    pString16 = strdup8to16(args->secondtext, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    pString16 = strdup8to16(args->email, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    return parcel;
}

Parcel* writePbToRuimToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    char **pStrings = (char **) data;
    parcel->writeInt32(*pStrings[0]);

    pString16 = strdup8to16(pStrings[1], &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    parcel->writeInt32(*pStrings[2]);

    pString16 = strdup8to16(pStrings[3], &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    parcel->writeInt32(*pStrings[4]);
    for (int i = 0; i < 5; i++) {
        pString16 = strdup8to16(pStrings[i], &len16);
        parcel->writeString16(pString16, len16);
        free(pString16);
    }

    return parcel;
}

#if 0
Parcel* mtkWritePbToRuimToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16;

    fillHeader(parcel, request, token);
    RIL_MTK_Ruim_Phonebook_Record *args = (RIL_MTK_Ruim_Phonebook_Record *) data;
    parcel->writeInt32(args->type);
    parcel->writeInt32(args->index);

    pString16 = strdup8to16(args->number, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    parcel->writeInt32(args->ton);

    pString16 = strdup8to16(args->alphaid, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);

    return parcel;
}
Parcel* playDTMFToneToParcel(int request, int token,
        void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();

    fillHeader(parcel, request, token);
    RIL_Play_DTMF_Tone *dtmfTone = (RIL_Play_DTMF_Tone *) data;
    parcel->writeInt32(dtmfTone->mode);
    if (dtmfTone->mode == 1) {
        parcel->writeInt32(dtmfTone->dtmfIndex);
        parcel->writeInt32(dtmfTone->volume);
        parcel->writeInt32(dtmfTone->duration);
    }

    return parcel;
}

Parcel* playToneSeqToParcel(int request, int token, void* data,
        int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();

    fillHeader(parcel, request, token);
    RIL_Play_Tone_Seq *toneSeq = (RIL_Play_Tone_Seq *) data;
    parcel->writeInt32(toneSeq->num);
    parcel->writeInt32(toneSeq->interation);
    parcel->writeInt32(toneSeq->volume);
    for (int i = 0; i < toneSeq->num; i++) {
        parcel->writeInt32(toneSeq->toneData[i].duration);
        parcel->writeInt32(toneSeq->toneData[i].freq[0]);
        parcel->writeInt32(toneSeq->toneData[i].freq[1]);
    }

    return parcel;
}
#endif

Parcel* cdmaSmsToParcelInternal(Parcel *parcel,
        RIL_CDMA_SMS_Message *rcsm) {
    int32_t digitCount;
    int digitLimit;
    if((parcel == NULL) || (rcsm == NULL)) {
        RFX_LOG_E(LOG_TAG, "cdmaSmsToParcelInternal(): invalid input parameter parcel null");
        return NULL;
    }
    parcel->writeInt32(rcsm->uTeleserviceID);
    parcel->write(&(rcsm->bIsServicePresent), sizeof(uint8_t));
    parcel->writeInt32(rcsm->uServicecategory);
    parcel->writeInt32((rcsm->sAddress).digit_mode);
    parcel->writeInt32((rcsm->sAddress).number_mode);
    parcel->writeInt32((rcsm->sAddress).number_type);
    parcel->writeInt32((rcsm->sAddress).number_plan);
    parcel->writeInt32((rcsm->sAddress).number_of_digits);
    digitLimit = MIN((rcsm->sAddress).number_of_digits, RIL_CDMA_SMS_ADDRESS_MAX);
    for (digitCount = 0; digitCount < digitLimit; digitCount++) {
        parcel->write(&((rcsm->sAddress).digits[digitCount]), sizeof(uint8_t));
    }
    parcel->writeInt32((rcsm->sSubAddress).subaddressType);
    parcel->write(&((rcsm->sSubAddress).odd), sizeof(uint8_t));
    parcel->write(&((rcsm->sSubAddress).number_of_digits), sizeof(uint8_t));
    digitLimit = MIN(((rcsm->sSubAddress).number_of_digits), RIL_CDMA_SMS_SUBADDRESS_MAX);
    for (digitCount = 0; digitCount < digitLimit; digitCount++) {
        parcel->write(&((rcsm->sSubAddress).digits[digitCount]), sizeof(uint8_t));
    }
    parcel->writeInt32(rcsm->uBearerDataLen);
    digitLimit = MIN((rcsm->uBearerDataLen), RIL_CDMA_SMS_BEARER_DATA_MAX);
    for (digitCount = 0; digitCount < digitLimit; digitCount++) {
        parcel->write(&(rcsm->aBearerData[digitCount]), sizeof(uint8_t));
    }
    return parcel;
}

void fillHeader(Parcel *parcel, int request, int token) {
    RFX_LOG_V(LOG_TAG, "fillHeader(): request = %d, token = %d", request, token);
    if(parcel == NULL) {
        RFX_LOG_E(LOG_TAG, "fillHeader(): invalid input parameter parcel null");
        return;
    }
    parcel->writeInt32(request);
    parcel->writeInt32(token);
}

static int getTypeAndJumpToData(Parcel *parcel, int request) {
    RFX_LOG_V(LOG_TAG, "getTypeAndJumpToData(): request = %d", request);
    int32_t type = 0;
    int32_t token;
    int32_t err;
    int32_t urcId;
    parcel->setDataPosition(0);
    parcel->readInt32(&type);
    if(type == 0) {
        // Response
        parcel->readInt32(&token);
        parcel->readInt32(&err);
        RFX_LOG_V(LOG_TAG, "response: request = %d, token = %d, err %d", request, token, err);
    } else {
        parcel->readInt32(&urcId);
        RFX_LOG_V(LOG_TAG, "urc: request = %d, urcId = %d", request, urcId);
    }
    return type;
}

static void
invalidCommandBlock (int requestId) {
    RFX_LOG_D(LOG_TAG, "Invalid command block for request %d", requestId);
}

static char *
strdupReadString(Parcel *p) {
    size_t stringlen;
    const char16_t *s16;

    s16 = p->readString16Inplace(&stringlen);

    return strndup16to8(s16, stringlen);
}

static int
readStringFromParcelInplace(Parcel *p, char *str, size_t maxLen) {
    size_t s16Len;
    const char16_t *s16;

    s16 = p->readString16Inplace(&s16Len);
    if (s16 == NULL) {
        return 0;
    }
    size_t strLen = strnlen16to8(s16, s16Len);
    if ((strLen + 1) > maxLen) {
        return 0;
    }
    if (strncpy16to8(str, s16, strLen) == NULL) {
        return 0;
    } else {
        return 1;
    }
}

static void
memsetString (char *s) {
    if (s != NULL) {
        memset (s, 0, strlen(s));
    }
}

// TODO
//static void parcelToUssdStrings(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
//}
#if 0
static void parcelToKeepaliveStatus(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int type = getTypeAndJumpToData(p, id);
    RequestInfo *requestInfo = (RequestInfo *)t;
    RIL_KeepaliveStatus* status = NULL;
    int32_t temp;

    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;
    if (p->dataAvail() > 0) {
        status = (RIL_KeepaliveStatus*)calloc(1, sizeof(RIL_KeepaliveStatus));
        p->readInt32 (&temp);
        status->sessionHandle = temp;
        appendPrintBuf("sessionHandle=%d, ", status->sessionHandle);
        p->readInt32 (&status->code);
        appendPrintBuf("code=%d", status->code);
    }
    dataEnd;
    IT_PRINT_LOG;
    if (type == 0) {
        //response
        if((NULL != status) || (e==0)) {
            RfxRilAdapter::responseToRilj(t, e, status, sizeof(RIL_KeepaliveStatus));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    }else {
        //URC
        RESPONSE_TO_RILJ(id, status, sizeof(RIL_KeepaliveStatus), RIL_SOCKET_ID(slotId));
    }
}
#endif
static void parcelToInts(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int32_t count = 0;
    size_t datalen = 0;
    int *pInts = NULL;
    int type = getTypeAndJumpToData(p, id);
    RequestInfo *requestInfo = (RequestInfo *)t;

    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;
    if (p->dataAvail() > 0) {
        p->readInt32 (&count);
        appendPrintBuf("count %d:", count);
        RFX_LOG_V(LOG_TAG, "parcelToInts request count = %d", count);
        if (count == 0) {
            goto invalid;
        }

        datalen = sizeof(int) * count;
        pInts = (int *)alloca(datalen);

        for (int i = 0 ; i < count ; i++) {
            int32_t v = 0;

            p->readInt32(&v);
            appendPrintBuf("%d, ", (int)v);
            pInts[i] = (int)v;
            RFX_LOG_V(LOG_TAG, "parcelToInts request pInts[%d] = %d", i, pInts[i]);
       }
   }
   dataEnd;
   IT_PRINT_LOG;
   RFX_LOG_V(LOG_TAG, "parcelToInts request %d, type %d", id, type);
   if (type == 0) {
       // Response
       if (e == 0 || pInts != NULL) {
           RfxRilAdapter::responseToRilj(t, e, const_cast<int *>(pInts), datalen);
       } else {
           RfxRilAdapter::responseToRilj(t, e, NULL, 0);
       }
   } else {
       // URC
       RESPONSE_TO_RILJ(id, const_cast<int *>(pInts), datalen, RIL_SOCKET_ID(slotId));
   }

#ifdef MEMSET_FREED
    memset(pInts, 0, datalen);
#endif

    return;
invalid:
    IT_PRINT_LOG;
    invalidCommandBlock(id);
    return;
}

static void parcelToFailCause(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_LastCallFailCauseInfo *info = NULL;
    int32_t res = 0;

    int type = getTypeAndJumpToData(p, id);
    if (type == 0) { //response
         IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
     } else {     //urc
         IT_UNSOL_START(requestToString(id), slotId);
     }

     dataStart;

    if (p->dataAvail() > 0) {
        info = (RIL_LastCallFailCauseInfo *) calloc(1, sizeof(RIL_LastCallFailCauseInfo));
        if(info == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        p->readInt32(&res);
        info->cause_code = (RIL_LastCallFailCause) res;
        appendPrintBuf("res=%d, ", info->cause_code);
        info->vendor_cause = strdupReadString(p);
        appendPrintBuf("res=%s, ", info->vendor_cause);
    }
    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToFailCause request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || info != NULL) {
            RfxRilAdapter::responseToRilj(t, e, info, sizeof(RIL_LastCallFailCauseInfo));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, info, sizeof(RIL_LastCallFailCauseInfo), RIL_SOCKET_ID(slotId));
    }

    if (info != NULL) {
        free(info->vendor_cause);
        free(info);
    }
    return;
calloc_error:
    dataEnd;
    IT_PRINT_LOG;
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToStrings(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int32_t countStrings = 0;
    size_t datalen = 0;
    char **pStrings = NULL;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);

    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    dataStart;
    if (p->dataAvail() > 0) {
        p->readInt32 (&countStrings);
        appendPrintBuf("%d:",countStrings);

        if (countStrings == 0) {
            // just some non-null pointer
            pStrings = (char **)alloca(sizeof(char *));
            datalen = 0;
        } else if (((int)countStrings) == -1) {
            pStrings = NULL;
            datalen = 0;
        } else {
            datalen = sizeof(char *) * countStrings;

            pStrings = (char **)alloca(datalen);

            for (int i = 0 ; i < countStrings ; i++) {
                pStrings[i] = strdupReadString(p);
                appendPrintBuf("%s, ",pStrings[i]);
            }
        }
    }

    RFX_LOG_D(LOG_TAG, "parcelToStrings request %d, type %d", id, type);
    dataEnd;
    IT_PRINT_LOG;
    if (type == 0) {
        // Response
        if (e == 0 || pStrings != NULL) {
            RfxRilAdapter::responseToRilj(t, e, pStrings,datalen);
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, pStrings, datalen, RIL_SOCKET_ID(slotId));
    }

    if (pStrings != NULL) {
        for (int i = 0 ; i < countStrings ; i++) {
#ifdef MEMSET_FREED
            memsetString(pStrings[i]);
#endif
            if (pStrings[i] != NULL) {
                free(pStrings[i]);
            }
        }

#ifdef MEMSET_FREED
        memset(pStrings, 0, datalen);
#endif
    }

    return;
}

static void parcelToString(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    size_t datalen;
    size_t stringlen = 0;
    char *string8 = NULL;
    int type = getTypeAndJumpToData(p, id);
    RequestInfo *requestInfo = (RequestInfo *)t;

    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    dataStart;
    if (p->dataAvail() > 0) {
        string8 = strdupReadString(p);
        appendPrintBuf("%s",string8);
        stringlen = strlen(string8);
    }

    RFX_LOG_D(LOG_TAG, "parcelToString request %d, type %d", id, type);
    dataEnd;
    IT_PRINT_LOG;
    if (type == 0) {
        // Response
        if (e == 0 || string8 != NULL) {
            RfxRilAdapter::responseToRilj(t, e, string8, stringlen);
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, string8, strlen(string8), RIL_SOCKET_ID(slotId));
    }

#ifdef MEMSET_FREED
    memsetString(string8);
#endif

    free(string8);
    return;
}

static void parcelToVoid(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int type = getTypeAndJumpToData(p, id);
    RequestInfo *requestInfo = (RequestInfo *)t;

    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    RFX_LOG_D(LOG_TAG, "parcelToVoid request %d, type %d", id, type);
    appendPrintBuf("{}");
    IT_PRINT_LOG;
    if (type == 0) {
        // Response
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    } else {
        // URC
#if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, NULL, 0, RIL_SOCKET_ID(slotId));
#else
        RfxRilAdapter::responseToRilj(id, NULL, 0);
#endif
    }
    return;
}

static void parcelToUnused(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int type = getTypeAndJumpToData(p, id);
    RFX_LOG_E(LOG_TAG, "parcelToUnused request %d, type %d. should not be here.", id, type);
    return;
}

static void parcelToCallList(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int num = 0;
    int uusPresent;
    RIL_Call **pCallLists = NULL;
    RIL_Call *pCallList = NULL;
    int32_t res;

    int type = getTypeAndJumpToData(p, id);

    //add RDIT log
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    dataStart;
    if (p->dataAvail() > 0) {
        p->readInt32(&num);
        pCallLists = (RIL_Call **) calloc(1, sizeof(RIL_Call *) * num);
        if(pCallLists == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        for (int i = 0; i < num; i++) {
            pCallList = (RIL_Call *) calloc(1, sizeof(RIL_Call));
            if(pCallList == NULL) {
                RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
                goto calloc_error;
            }
            pCallLists[i] = pCallList;
            p->readInt32(&res);
            pCallLists[i]->state = (RIL_CallState) res;
            appendPrintBuf("state=%d, ", pCallLists[i]->state);
            p->readInt32(&pCallLists[i]->index);
            appendPrintBuf("index=%d, ", pCallLists[i]->index);
            p->readInt32(&pCallLists[i]->toa);
            appendPrintBuf("toa=%d, ", pCallLists[i]->toa);
            p->readInt32(&res);
            pCallLists[i]->isMpty = (uint8_t) res;
            appendPrintBuf("isMpty=%d, ",pCallLists[i]->isMpty);
            p->readInt32(&res);
            pCallLists[i]->isMT = (uint8_t) res;
            appendPrintBuf("isMT=%d, ", pCallLists[i]->isMT);
            p->readInt32(&res);
            pCallLists[i]->als = (uint8_t) res;
            appendPrintBuf("als=%d, ", pCallLists[i]->als);
            p->readInt32(&res);
            pCallLists[i]->isVoice = (uint8_t) res;
            appendPrintBuf("isVoice=%d,",pCallLists[i]->isVoice);
            p->readInt32(&res);
            pCallLists[i]->isVoicePrivacy = (uint8_t) res;
            appendPrintBuf("isVoicePrivacy=%d, ", pCallLists[i]->isVoicePrivacy);
            pCallLists[i]->number = strdupReadString(p);
            appendPrintBuf("number=%s, ", pCallLists[i]->number);
            p->readInt32(&pCallLists[i]->numberPresentation);
            appendPrintBuf("numberPresentation=%d,", pCallLists[i]->numberPresentation);
            pCallLists[i]->name = strdupReadString(p);
            appendPrintBuf("name=%s,", pCallLists[i]->name);
            p->readInt32(&pCallLists[i]->namePresentation);
            appendPrintBuf("namePresentation=%d,", pCallLists[i]->namePresentation);
            p->readInt32(&uusPresent);
            appendPrintBuf("uusPresent=%d,", uusPresent);
            if (uusPresent == 0) {
                /* UUS Information is absent */
            } else {
                /* UUS Information is present */
                p->readInt32(&res);
                pCallLists[i]->uusInfo->uusType = (RIL_UUS_Type) res;
                appendPrintBuf("uusType=%d,", res);
                p->readInt32(&res);
                pCallLists[i]->uusInfo->uusDcs = (RIL_UUS_DCS) res;
                appendPrintBuf("uusDcs=%d,", res);
                p->readInt32(&pCallLists[i]->uusInfo->uusLength);
                appendPrintBuf("uusLength=%d,", pCallLists[i]->uusInfo->uusLength);
                p->read(&pCallLists[i]->uusInfo->uusData, pCallLists[i]->uusInfo->uusLength);
                appendPrintBuf("uusData=%s,", pCallLists[i]->uusInfo->uusData);
            }
        }
    }

    dataEnd;
    IT_PRINT_LOG;
    //end add RDIT log

    RFX_LOG_D(LOG_TAG, "parcelToCallList request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || pCallLists != NULL) {
            RfxRilAdapter::responseToRilj(t, e, pCallLists, sizeof(RIL_Call *) * num);
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, pCallLists, sizeof(RIL_Call *) * num, RIL_SOCKET_ID(slotId));
    }

    if (pCallLists != NULL) {
        for (int i = 0; i < num; i++ ) {
            if (pCallLists[i] != NULL) {
                free(pCallLists[i]->number);
                free(pCallLists[i]->name);
                free(pCallLists[i]);
            }
        }
        free(pCallLists);
    }
    return;
calloc_error:
    dataEnd;
    IT_PRINT_LOG;
    if (pCallLists != NULL) {
        for (int i = 0; i < num; i++ ) {
            if (pCallLists[i] != NULL) {
                free(pCallLists[i]->number);
                free(pCallLists[i]->name);
                free(pCallLists[i]);
            }
        }
        free(pCallLists);
    }
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToSMS(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_SMS_Response *pSMS = NULL;

    int type = getTypeAndJumpToData(p, id);

    // Even if send sms is failed, it should response to framework with correct
    // error code
    if (p->dataAvail() > 0) {
        pSMS = (RIL_SMS_Response *) calloc(1, sizeof(RIL_SMS_Response));
        if(pSMS == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        p->readInt32(&pSMS->messageRef);
        pSMS->ackPDU = strdupReadString(p);
        p->readInt32(&pSMS->errorCode);
    }

    RFX_LOG_D(LOG_TAG, "parcelToSMS request %d, type %d", id, type);

    //add RDIT log
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    dataStart;
    if(pSMS != NULL){
        appendPrintBuf("errorCode=%d, messageRef=%d, ackPDU=%s",pSMS->errorCode, pSMS->messageRef,pSMS->ackPDU);
    }

    dataEnd;
    IT_PRINT_LOG;
    //end add RDIT log

    if (type == 0) {
        // Response
        if (e == 0 || pSMS != NULL) {
            RfxRilAdapter::responseToRilj(t, e, pSMS, sizeof(RIL_SMS_Response));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, pSMS, sizeof(RIL_SMS_Response), RIL_SOCKET_ID(slotId));
    }

    if (pSMS != NULL && pSMS->ackPDU != NULL) {
        free(pSMS->ackPDU);
    }
    if (pSMS != NULL) {
        free(pSMS);
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToSIM_IO(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_SIM_IO_Response *simIO = NULL;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);


    if (type == 0) { //response
            IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
            IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;
    if (p->dataAvail() > 0) {
        simIO = (RIL_SIM_IO_Response *) calloc(1, sizeof(RIL_SIM_IO_Response));
        if(simIO == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        p->readInt32(&simIO->sw1);
        appendPrintBuf("sw1=0x%x,", simIO->sw1);
        p->readInt32(&simIO->sw2);
        appendPrintBuf("sw2=0x%x,", simIO->sw2);
        simIO->simResponse = strdupReadString(p);
        appendPrintBuf("simResponse=%s", simIO->simResponse);
    }

    RFX_LOG_D(LOG_TAG, "parcelToSIM_IO request %d, type %d", id, type);
    dataEnd;
    IT_PRINT_LOG;
    if (type == 0) {
        // Response
        if (e == 0 || simIO != NULL) {
            RfxRilAdapter::responseToRilj(t, e, simIO, sizeof(RIL_SIM_IO_Response));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, simIO, sizeof(RIL_SIM_IO_Response), RIL_SOCKET_ID(slotId));
    }

    if (simIO != NULL && simIO->simResponse != NULL) {
        free(simIO->simResponse);
    }
    if (simIO != NULL) {
        free(simIO);
    }
    return;
calloc_error:
    dataEnd;
    IT_PRINT_LOG;
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToCallForwards(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CallForwardInfo **infos = NULL;
    RIL_CallForwardInfo *info = NULL;
    int32_t num = 0;

    int type = getTypeAndJumpToData(p, id);

    if (p->dataAvail() > 0) {
        p->readInt32(&num);
        infos = (RIL_CallForwardInfo **) calloc(1, sizeof(RIL_CallForwardInfo *) * num);
        if(infos == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        for (int i = 0; i < num; i++) {
            info = (RIL_CallForwardInfo*)calloc(1, sizeof(RIL_CallForwardInfo));
            if(info == NULL) {
                RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
                goto calloc_error;
            }
            infos[i] = info;

            p->readInt32(&infos[i]->status);
            p->readInt32(&infos[i]->reason);
            p->readInt32(&infos[i]->serviceClass);
            p->readInt32(&infos[i]->toa);
            infos[i]->number = strdupReadString(p);
            p->readInt32(&infos[i]->timeSeconds);
        }
    }

    RFX_LOG_D(LOG_TAG, "parcelToCallForwards request %d, type %d", id, type);
    //add RDIT log
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    dataStart;
    for (int i = 0; i < num; i++) {
        appendPrintBuf("[%s,reason=%d,cls=%d,toa=%d,%s,tout=%d],",
            (infos[i]->status==1)?"enable":"disable",
            infos[i]->reason, infos[i]->serviceClass, infos[i]->toa,
            (char*)infos[i]->number,
            infos[i]->timeSeconds);
    }
    removeLastChar;
    dataEnd;
    IT_PRINT_LOG;
    //end add RDIT log

    if (type == 0) {
        // Response
        if (e == 0 || infos != NULL) {
            RfxRilAdapter::responseToRilj(t, e, infos, sizeof(RIL_CallForwardInfo *) * num);
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
#if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, infos, sizeof(RIL_CallForwardInfo *) * num,
                RIL_SOCKET_ID(slotId));
#else
        RfxRilAdapter::responseToRilj(id, infos, sizeof(RIL_CallForwardInfo *) * num);
#endif
    }

    if(infos != NULL) {
        for (int i = 0; i < num; i++) {
            free(infos[i]->number);
            free(infos[i]);
        }
        free(infos);
    }
    return;
calloc_error:
    if(infos != NULL) {
        for (int i = 0; i < num; i++) {
            free(infos[i]->number);
            free(infos[i]);
        }
        free(infos);
    }
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToDataCallList(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_Data_Call_Response_v11 *list = NULL;
    int32_t num = 0;
    int32_t version = 0;
    int type = getTypeAndJumpToData(p, id);
    RequestInfo *requestInfo = (RequestInfo *)t;

    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;

    if (p->dataAvail() > 0) {
        p->readInt32(&version);
        appendPrintBuf("%d, ", (int)version);
        p->readInt32(&num);
        appendPrintBuf("%d, ", (int)num);
        list = (RIL_Data_Call_Response_v11 *) calloc(1, sizeof(RIL_Data_Call_Response_v11) * num);
        if(list == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        for (int i = 0; i < num; i++) {
            p->readInt32(&list[i].status);
            p->readInt32(&list[i].suggestedRetryTime);
            p->readInt32(&list[i].cid);
            p->readInt32(&list[i].active);
            list[i].type = strdupReadString(p);
            list[i].ifname = strdupReadString(p);
            list[i].addresses = strdupReadString(p);
            list[i].dnses = strdupReadString(p);
            list[i].gateways = strdupReadString(p);
            list[i].pcscf = strdupReadString(p);
            p->readInt32(&list[i].mtu);
            p->readInt32(&list[i].mtuV6);

            appendPrintBuf("[datacall_index %d, ", i);
            appendPrintBuf("%d, ", list[i].status);
            appendPrintBuf("%d, ", list[i].suggestedRetryTime);
            appendPrintBuf("%d, ", list[i].cid);
            appendPrintBuf("%d, ", list[i].active);
            appendPrintBuf("%s, ", list[i].type);
            appendPrintBuf("%s, ", list[i].ifname);
            appendPrintBuf("%s, ", list[i].addresses);
            appendPrintBuf("%s, ", list[i].dnses);
            appendPrintBuf("%s, ", list[i].gateways);
            appendPrintBuf("%s, ", list[i].pcscf);
            appendPrintBuf("%d]",  list[i].mtu);
            appendPrintBuf("%d]",  list[i].mtuV6);
        }
    }
    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToDataCallList request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || list != NULL) {
            RfxRilAdapter::responseToRilj(t, e, list, sizeof(RIL_Data_Call_Response_v11) * num);
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
#if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, list, sizeof(RIL_Data_Call_Response_v11) * num,
                RIL_SOCKET_ID(slotId));
#else
        RfxRilAdapter::responseToRilj(id, list, sizeof(RIL_Data_Call_Response_v11) * num);
#endif
    }

    for (int i = 0; i < num; i++) {
        free(list[i].type);
        free(list[i].ifname);
        free(list[i].addresses);
        free(list[i].dnses);
        free(list[i].gateways);
        free(list[i].pcscf);
    }
    free(list);
    return;
calloc_error:
    dataEnd;
    IT_PRINT_LOG;
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToSetupDataCall(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    parcelToDataCallList(t, e, id, p, slotId);
}

static void parcelToRaw(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int headerSize = 0;
    int size = 0;
    char* line = NULL;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;

    if (p->dataAvail() > 0) {
        if (type == 0) {
            // response
            headerSize = 3;
        } else {
            // urc
            headerSize = 2;
        }

        if(p->dataSize() > headerSize) {
            p->readInt32(&size);
            appendPrintBuf("size=%d,", size);
            line = (char *) calloc(1, size);
            if(line == NULL) {
                RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
                goto calloc_error;
            }
            p->read(line, size);
            appendPrintBuf("line=%s,", line);
        }
    }
    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToRaw request %d, type %d", id, type);
    //add RDIT log
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    dataStart;
    if( line != NULL){
        appendPrintBuf("size=%d, line=%s", size,line);
    }

    dataEnd;
    IT_PRINT_LOG;
    //end add RDIT log

    if (type == 0) {
        // Response
        if (e == 0 || line != NULL) {
            RfxRilAdapter::responseToRilj(t, e, line, size);
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, line, size,
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, line, size);
    #endif
    }

    if (line != NULL) {
        free(line);
    }
    return;
calloc_error:
    dataEnd;
    IT_PRINT_LOG;
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToSsn(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_SuppSvcNotification *p_cur = NULL;

    int type = getTypeAndJumpToData(p, id);

    if (p->dataAvail() > 0) {
        p_cur = (RIL_SuppSvcNotification*)calloc(1,
            sizeof(RIL_SuppSvcNotification));
        if(p_cur == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        p->readInt32(&p_cur->notificationType);
        p->readInt32(&p_cur->code);
        p->readInt32(&p_cur->index);
        p->readInt32(&p_cur->type);
        p_cur->number = strdupReadString(p);
        if(p_cur->number == NULL) {
            goto calloc_error;
        }
    }

    RFX_LOG_D(LOG_TAG, "parcelToSsn request %d, type %d", id, type);

    //add RDIT log
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    dataStart;
    if(p_cur != NULL) {
        appendPrintBuf("%s,code=%d,id=%d,type=%d,%s", (p_cur->notificationType==0)?"mo":"mt",
            p_cur->code, p_cur->index, p_cur->type,(char*)p_cur->number);
    }
    dataEnd;
    IT_PRINT_LOG;
    //end add RDIT log

    if (type == 0) {
        // Response
        if (e == 0 || p_cur != NULL) {
            RfxRilAdapter::responseToRilj(t, e, p_cur, sizeof(RIL_SuppSvcNotification));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, p_cur, sizeof(RIL_SuppSvcNotification), RIL_SOCKET_ID(slotId));
    }
    if (p_cur != NULL) {
        if(p_cur->number != NULL) {
            free(p_cur->number);
        }
        free(p_cur);
    }
    return;
calloc_error:
    if (p_cur != NULL) {
        if(p_cur->number != NULL) {
            free(p_cur->number);
        }
        free(p_cur);
    }
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToCrssN(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CrssNotification p_cur;

    memset(&p_cur, 0, sizeof(p_cur));

    int type = getTypeAndJumpToData(p, id);

    int dataAvail = p->dataAvail();
    if (dataAvail > 0) {
        p->readInt32(&p_cur.code);
        p->readInt32(&p_cur.type);
        p_cur.number = strdupReadString(p);
        p_cur.alphaid = strdupReadString(p);
        p->readInt32(&p_cur.cli_validity);
    }

    RFX_LOG_D(LOG_TAG, "parcelToCrssN request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (dataAvail > 0) {
            RfxRilAdapter::responseToRilj(t, e, &p_cur, sizeof(p_cur));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &p_cur, sizeof(p_cur),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &p_cur, sizeof(p_cur));
    #endif
    }
    if (p_cur.number != NULL) {
        free(p_cur.number);
    }
    if (p_cur.alphaid != NULL) {
        free(p_cur.alphaid);
    }
}

static void parcelToSimStatus(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CardStatus_v6 *cardStatus = NULL;
    int32_t v = 0;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);
    IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    dataStart;
    if (p->dataAvail() > 0) {
        cardStatus = (RIL_CardStatus_v6 *)calloc(1, sizeof(RIL_CardStatus_v6));
        if(cardStatus == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        p->readInt32(&v);
        cardStatus->card_state = RIL_CardState(v);
        appendPrintBuf("card_state=%d,", cardStatus->card_state);
        p->readInt32(&v);
        cardStatus->universal_pin_state = RIL_PinState(v);
        appendPrintBuf("universal_pin_state=%d,", cardStatus->universal_pin_state);
        p->readInt32(&cardStatus->gsm_umts_subscription_app_index);
        p->readInt32(&cardStatus->cdma_subscription_app_index);
        p->readInt32(&cardStatus->ims_subscription_app_index);
        p->readInt32(&cardStatus->num_applications);
        appendPrintBuf("num_applications=%d,", cardStatus->num_applications);
        for (int i = 0; i < cardStatus->num_applications; i++) {
            appendPrintBuf("[");
            p->readInt32(&v);
            cardStatus->applications[i].app_type = RIL_AppType(v);
            appendPrintBuf("app_type=%d,", cardStatus->applications[i].app_type);
            p->readInt32(&v);
            cardStatus->applications[i].app_state = RIL_AppState(v);
            appendPrintBuf("app_state=%d,", cardStatus->applications[i].app_state);
            p->readInt32(&v);
            cardStatus->applications[i].perso_substate = RIL_PersoSubstate(v);
            appendPrintBuf("perso_substate=%d,", cardStatus->applications[i].perso_substate);
            cardStatus->applications[i].aid_ptr = strdupReadString(p);
            appendPrintBuf("aid_ptr=%s,", cardStatus->applications[i].aid_ptr);
            cardStatus->applications[i].app_label_ptr = strdupReadString(p);
            appendPrintBuf("app_label_ptr=%s,", cardStatus->applications[i].app_label_ptr);
            p->readInt32(&cardStatus->applications[i].pin1_replaced);
            p->readInt32(&v);
            cardStatus->applications[i].pin1 = RIL_PinState(v);
            appendPrintBuf("pin1=%d,", cardStatus->applications[i].pin1);
            p->readInt32(&v);
            cardStatus->applications[i].pin2 = RIL_PinState(v);
            appendPrintBuf("pin2=%d", cardStatus->applications[i].pin2);
            appendPrintBuf("]");
        }
    }
    dataEnd;
    IT_PRINT_LOG;
    RFX_LOG_D(LOG_TAG, "parcelToSimStatus request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || cardStatus != NULL) {
            RfxRilAdapter::responseToRilj(t, e, cardStatus, sizeof(RIL_CardStatus_v6));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, cardStatus, sizeof(RIL_CardStatus_v6), RIL_SOCKET_ID(slotId));
    }

    if (cardStatus != NULL) {
        for (int i = 0; i < cardStatus->num_applications; i++) {
            free(cardStatus->applications[i].aid_ptr);
            free(cardStatus->applications[i].app_label_ptr);
        }
        free(cardStatus);
    }
    return;

calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToGsmBrSmsCnf(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int type = getTypeAndJumpToData(p, id);
    int32_t v = 0;
    int32_t num = 0;

    p->readInt32(&num);
    {
        RIL_GSM_BroadcastSmsConfigInfo gsmBci[num];
        RIL_GSM_BroadcastSmsConfigInfo *gsmBciPtrs[num];

        for (int i = 0 ; i < num ; i++ ) {
            gsmBciPtrs[i] = &gsmBci[i];

            p->readInt32(&v);
            gsmBci[i].fromServiceId = (int) v;

            p->readInt32(&v);
            gsmBci[i].toServiceId = (int) v;

            p->readInt32(&v);
            gsmBci[i].fromCodeScheme = (int) v;

            p->readInt32(&v);
            gsmBci[i].toCodeScheme = (int) v;

            p->readInt32(&v);
            gsmBci[i].selected = (uint8_t) v;
        }

        RFX_LOG_D(LOG_TAG, "parcelToGsmBrSmsCnf request %d, type %d", id, type);
        //add RDIT log
        if (type == 0) { //response
            IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
        } else {    //urc
            IT_UNSOL_START(requestToString(id), slotId);
        }

        dataStart;
        for (int i = 0; i < num; i++) {
            appendPrintBuf(" [%d: fromServiceId=%d, toServiceId=%d, \
                    fromCodeScheme=%d, toCodeScheme=%d, selected =%d]",
                    i, gsmBci[i].fromServiceId, gsmBci[i].toServiceId,
                    gsmBci[i].fromCodeScheme, gsmBci[i].toCodeScheme,
                    gsmBci[i].selected);
        }

        dataEnd;
        IT_PRINT_LOG;
        //end add RDIT log

        if (type == 0) {
            // Response
            RfxRilAdapter::responseToRilj(t, e, gsmBciPtrs, num*sizeof(RIL_GSM_BroadcastSmsConfigInfo*));
        } else {
            // URC
    #if defined(ANDROID_MULTI_SIM)
            RfxRilAdapter::responseToRilj(id, gsmBciPtrs, num*sizeof(RIL_GSM_BroadcastSmsConfigInfo*),
                    RIL_SOCKET_ID(slotId));
    #else
            RfxRilAdapter::responseToRilj(id, gsmBciPtrs, num*sizeof(RIL_GSM_BroadcastSmsConfigInfo*));
    #endif
        }

#ifdef MEMSET_FREED
        memset(gsmBci, 0, num * sizeof(RIL_GSM_BroadcastSmsConfigInfo));
        memset(gsmBciPtrs, 0, num * sizeof(RIL_GSM_BroadcastSmsConfigInfo *));
#endif
    }

    return;
}

static void parcelToCdmaBrSmsCnf(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int type = getTypeAndJumpToData(p, id);
    int32_t v = 0;
    int32_t num = 0;

    p->readInt32(&num);
    {
        RIL_CDMA_BroadcastSmsConfigInfo gsmBci[num];
        RIL_CDMA_BroadcastSmsConfigInfo *gsmBciPtrs[num];

        for (int i = 0 ; i < num ; i++ ) {
            gsmBciPtrs[i] = &gsmBci[i];


            p->readInt32(&v);
            gsmBci[i].service_category = (int) v;

            p->readInt32(&v);
            gsmBci[i].language = (int) v;

            p->readInt32(&v);
            gsmBci[i].selected = (int) v;

        }

        RFX_LOG_D(LOG_TAG, "parcelToCdmaBrSmsCnf request %d, type %d", id, type);
        //add RDIT log
        if (type == 0) { //response
            IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
        } else {    //urc
            IT_UNSOL_START(requestToString(id), slotId);
        }

        dataStart;
        for (int i = 0; i < num; i++) {
            appendPrintBuf(" [%d: srvice_category=%d, language =%d, \
                  selected =%d], ",
                  i, gsmBci[i].service_category, gsmBci[i].language,
                  gsmBci[i].selected);
        }

        dataEnd;
        IT_PRINT_LOG;
        //end add RDIT log

        if (type == 0) {
            // Response
            RfxRilAdapter::responseToRilj(t, e, gsmBciPtrs, num*sizeof(RIL_GSM_BroadcastSmsConfigInfo*));
        } else {
            // URC
#if defined(ANDROID_MULTI_SIM)
            RfxRilAdapter::responseToRilj(id, gsmBciPtrs, num*sizeof(RIL_GSM_BroadcastSmsConfigInfo*),
                    RIL_SOCKET_ID(slotId));
#else
            RfxRilAdapter::responseToRilj(id, gsmBciPtrs, num*sizeof(RIL_GSM_BroadcastSmsConfigInfo*));
#endif
        }

#ifdef MEMSET_FREED
        memset(gsmBci, 0, num * sizeof(RIL_GSM_BroadcastSmsConfigInfo));
        memset(gsmBciPtrs, 0, num * sizeof(RIL_GSM_BroadcastSmsConfigInfo *));
#endif

    }

}

static void parcelToCdmaSms(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CDMA_SMS_Message *p_cur = NULL;

    int type = getTypeAndJumpToData(p, id);

    if (p->dataAvail() > 0) {
        p_cur = (RIL_CDMA_SMS_Message*)calloc(1,
                sizeof(RIL_CDMA_SMS_Message));
        if(p_cur == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        uint8_t uct;
        int digitCount;
        int digitLimit;
        int32_t  v = 0;
        p->readInt32(&p_cur->uTeleserviceID);
        p->read(&(p_cur->bIsServicePresent),sizeof(uct));
        p->readInt32(&p_cur->uServicecategory);
        p->readInt32(&v);
        p_cur->sAddress.digit_mode = RIL_CDMA_SMS_DigitMode(v);
        p->readInt32(&v);
        p_cur->sAddress.number_mode = RIL_CDMA_SMS_NumberMode(v);
        p->readInt32(&v);
        p_cur->sAddress.number_type = RIL_CDMA_SMS_NumberType(v);
        p->readInt32(&v);
        p_cur->sAddress.number_plan = RIL_CDMA_SMS_NumberPlan(v);
        p->read(&(p_cur->sAddress.number_of_digits), sizeof(uct));
        digitLimit= MIN((p_cur->sAddress.number_of_digits), RIL_CDMA_SMS_ADDRESS_MAX);
        for (digitCount =0 ; digitCount < digitLimit; digitCount ++) {
            p->read(&(p_cur->sAddress.digits[digitCount]),sizeof(uct));
        }

        p->readInt32(&v);
        p_cur->sSubAddress.subaddressType = RIL_CDMA_SMS_SubaddressType(v);
        p->read(&(p_cur->sSubAddress.odd),sizeof(uct));
        p->read(&(p_cur->sSubAddress.number_of_digits),sizeof(uct));
        digitLimit= MIN((p_cur->sSubAddress.number_of_digits), RIL_CDMA_SMS_SUBADDRESS_MAX);
        for (digitCount =0 ; digitCount < digitLimit; digitCount ++) {
            p->read(&(p_cur->sSubAddress.digits[digitCount]),sizeof(uct));
        }

        p->readInt32(&p_cur->uBearerDataLen);
        digitLimit= MIN((p_cur->uBearerDataLen), RIL_CDMA_SMS_BEARER_DATA_MAX);
        RFX_LOG_D(LOG_TAG, "parcelToCdmaSms bearerdatalen =  %d", digitLimit);
        for (digitCount =0 ; digitCount < digitLimit; digitCount ++) {
           p->read(&(p_cur->aBearerData[digitCount]), sizeof(uct));
        }
    }

    RFX_LOG_D(LOG_TAG, "parcelToCdmaSms request %d, type %d", id, type);
    //add RDIT log
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    dataStart;
    if (p_cur != NULL) {
    appendPrintBuf("uTeleserviceID=%d, bIsServicePresent=%d, uServicecategory=%d, \
            sAddress.digit_mode=%d, sAddress.number_mode=%d, sAddress.number_type=%d,\
            aBearerDataLen = %d, aBearerData: ",
            p_cur->uTeleserviceID,p_cur->bIsServicePresent,p_cur->uServicecategory,
            p_cur->sAddress.digit_mode, p_cur->sAddress.number_mode,p_cur->sAddress.number_type,p_cur->uBearerDataLen);

        for(int j=0; j<MIN((p_cur->uBearerDataLen), RIL_CDMA_SMS_BEARER_DATA_MAX);j++){
            appendPrintBuf("%02x",p_cur->aBearerData[j]);
        }
    }
    dataEnd;
    IT_PRINT_LOG;
    //end add RDIT log

    if (type == 0) {
        // Response
        if (e == 0 || p_cur != NULL) {
            RfxRilAdapter::responseToRilj(t, e, p_cur, sizeof(RIL_CDMA_SMS_Message));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, p_cur, sizeof(RIL_CDMA_SMS_Message), RIL_SOCKET_ID(slotId));
    }

    if(p_cur != NULL){
        free(p_cur);
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToCellList(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_NeighboringCell *entry = NULL;
    RIL_NeighboringCell **entries = NULL;
    int32_t  v;
    int64_t  v6;
    int32_t num = 0;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;
    appendPrintBuf("p->dataAvail() =%lu ",p->dataAvail());
    if (p->dataAvail() > 0) {
        p->readInt32(&num);
        appendPrintBuf("num=%d ",num );
        entries = (RIL_NeighboringCell**)calloc(1, sizeof(RIL_NeighboringCell*) * num);
        if(entries == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        for (int i = 0; i< num; i++) {
            entry = (RIL_NeighboringCell*)calloc(1, sizeof(RIL_NeighboringCell));
            if(entry == NULL) {
                RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
                goto calloc_error;
            }
            entries[i] = entry;

            p->readInt32(&entries[i]->rssi);

            entries[i]->cid = strdupReadString(p);
            appendPrintBuf("index=%d,rssi=%d,cid=%s",i, entries[i]->rssi,entries[i]->cid);
        }
    }
    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToCellList request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || entries != NULL) {
            RfxRilAdapter::responseToRilj(t, e, entries, num*sizeof(RIL_NeighboringCell*));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
#if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, entries, num*sizeof(RIL_NeighboringCell*),
                RIL_SOCKET_ID(slotId));
#else
        RfxRilAdapter::responseToRilj(id, entries, num*sizeof(RIL_NeighboringCell*));
#endif
    }

    if (entries != NULL) {
        for (int i = 0; i< num; i++) {
            if (entries[i] != NULL) {
                free(entries[i]->cid);
            }
        }
        free(entries);
    }
    return;
calloc_error:
    dataEnd;
    IT_PRINT_LOG;
    if (entries != NULL) {
        for (int i = 0; i< num; i++) {
            if (entries[i] != NULL) {
                free(entries[i]->cid);
            }
        }
        free(entries);
    }
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToCdmaInformationRecords(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CDMA_InformationRecords *p_cur = NULL;
    char *buf = NULL;
    int32_t data;

    int type = getTypeAndJumpToData(p, id);
    int32_t numOfRecs = 0;

    if (p->dataAvail() > 0) {
         p_cur = (RIL_CDMA_InformationRecords*)calloc(1,
                sizeof(RIL_CDMA_InformationRecords));
        if(p_cur == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        p->readInt32(&numOfRecs);

        p_cur->numberOfInfoRecs = numOfRecs;

        for (int i = 0 ; i < numOfRecs ; i++) {
            p->readInt32(&data);
            p_cur->infoRec[i].name = (RIL_CDMA_InfoRecName)data;

            RFX_LOG_D(LOG_TAG, "parcelToCdmaInformationRecords name %d", p_cur->infoRec[i].name);

            buf = NULL;
            switch (p_cur->infoRec[i].name) {
                case RIL_CDMA_DISPLAY_INFO_REC:
                case RIL_CDMA_EXTENDED_DISPLAY_INFO_REC:
                    buf = strdupReadString(p);
                    p_cur->infoRec[i].rec.display.alpha_len = strlen(buf);
                    strncpy(p_cur->infoRec[i].rec.display.alpha_buf, buf,
                        p_cur->infoRec[i].rec.display.alpha_len > (CDMA_ALPHA_INFO_BUFFER_LENGTH_H - 1) ? (CDMA_ALPHA_INFO_BUFFER_LENGTH_H - 1):p_cur->infoRec[i].rec.display.alpha_len);
                    break;
                case RIL_CDMA_CALLED_PARTY_NUMBER_INFO_REC:
                case RIL_CDMA_CALLING_PARTY_NUMBER_INFO_REC:
                case RIL_CDMA_CONNECTED_NUMBER_INFO_REC:
                    buf = strdupReadString(p);
                    p_cur->infoRec[i].rec.number.len = strlen(buf);
                    strncpy(p_cur->infoRec[i].rec.number.buf, buf,
                        p_cur->infoRec[i].rec.number.len > (CDMA_NUMBER_INFO_BUFFER_LENGTH_H - 1) ? (CDMA_NUMBER_INFO_BUFFER_LENGTH_H - 1):p_cur->infoRec[i].rec.number.len);

                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.number.number_type = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.number.number_plan = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.number.pi = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.number.si = data;
                    break;
                case RIL_CDMA_SIGNAL_INFO_REC:
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.signal.isPresent = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.signal.signalType = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.signal.alertPitch = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.signal.signal = data;
                    break;
                case RIL_CDMA_REDIRECTING_NUMBER_INFO_REC:
                    buf = strdupReadString(p);
                    p_cur->infoRec[i].rec.redir.redirectingNumber.len = strlen(buf);
                    strncpy(p_cur->infoRec[i].rec.redir.redirectingNumber.buf, buf,
                        p_cur->infoRec[i].rec.redir.redirectingNumber.len > (CDMA_NUMBER_INFO_BUFFER_LENGTH_H - 1) ? (CDMA_NUMBER_INFO_BUFFER_LENGTH_H - 1):p_cur->infoRec[i].rec.redir.redirectingNumber.len);

                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.redir.redirectingNumber.number_type = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.redir.redirectingNumber.number_plan = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.redir.redirectingNumber.pi = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.redir.redirectingNumber.si = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.redir.redirectingReason = (RIL_CDMA_RedirectingReason)data;
                    break;
                case RIL_CDMA_LINE_CONTROL_INFO_REC:
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.lineCtrl.lineCtrlPolarityIncluded = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.lineCtrl.lineCtrlToggle = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.lineCtrl.lineCtrlReverse = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.lineCtrl.lineCtrlPowerDenial = data;
                    break;
                case RIL_CDMA_T53_CLIR_INFO_REC:
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.clir.cause = data;
                    break;
                case RIL_CDMA_T53_AUDIO_CONTROL_INFO_REC:
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.audioCtrl.upLink = data;
                    p->readInt32(&data);
                    p_cur->infoRec[i].rec.audioCtrl.downLink = data;
                    break;
            }
            if (buf != NULL) {
                free(buf);
            }
        }
    }

    RFX_LOG_D(LOG_TAG, "parcelToCdmaInformationRecords request %d, type %d, parcelsize %d", id, type, p->dataSize());

    if (type == 0) {
        // Response
        if (e == 0 || p_cur != NULL) {
            RfxRilAdapter::responseToRilj(t, e, p_cur, sizeof(RIL_CDMA_InformationRecords));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, p_cur, sizeof(RIL_CDMA_InformationRecords), RIL_SOCKET_ID(slotId));
    }
    if (p_cur != NULL) {
        free(p_cur);
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToRilSignalStrength_v12(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_SignalStrength_v12 *p_cur = NULL;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;

    if (p->dataAvail() > 0) {
        p_cur = (RIL_SignalStrength_v12*)calloc(1,
            sizeof(RIL_SignalStrength_v12));
        if(p_cur == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        int32_t  v;
        RFX_LOG_D(LOG_TAG, "parcelToRilSignalStrength request %d, type %d, parcelsize %d", id, type, p->dataSize());

        // Only used in Response

        p->readInt32(&p_cur->GW_SignalStrength.signalStrength);
        appendPrintBuf("V5 GW signalStrength=%d,", p_cur->GW_SignalStrength.signalStrength);
        p->readInt32(&p_cur->GW_SignalStrength.bitErrorRate);
        appendPrintBuf("GW bitErrorRate=%d,", p_cur->GW_SignalStrength.bitErrorRate);
        p->readInt32(&p_cur->GW_SignalStrength.timingAdvance);
        appendPrintBuf("GW timingAdvance=%d,", p_cur->GW_SignalStrength.timingAdvance);
        p->readInt32(&p_cur->CDMA_SignalStrength.dbm);
        appendPrintBuf("CDMA dbm=%d,", p_cur->CDMA_SignalStrength.dbm);
        p->readInt32(&p_cur->CDMA_SignalStrength.ecio);
        appendPrintBuf("CDMA ecio=%d,", p_cur->CDMA_SignalStrength.ecio);
        p->readInt32(&p_cur->EVDO_SignalStrength.dbm);
        appendPrintBuf("EVDO dbm=%d,", p_cur->EVDO_SignalStrength.dbm);
        p->readInt32(&p_cur->EVDO_SignalStrength.ecio);
        appendPrintBuf("EVDO ecio=%d,", p_cur->EVDO_SignalStrength.ecio);
        p->readInt32(&p_cur->EVDO_SignalStrength.signalNoiseRatio);
        appendPrintBuf("EVDO signalNoiseRatio=%d,", p_cur->EVDO_SignalStrength.signalNoiseRatio);
        p->readInt32(&p_cur->LTE_SignalStrength.signalStrength);
        appendPrintBuf("V6 LTE signalStrength=%d,", p_cur->LTE_SignalStrength.signalStrength);
        p->readInt32(&p_cur->LTE_SignalStrength.rsrp);
        appendPrintBuf("LTE rsrp=%d,", p_cur->LTE_SignalStrength.rsrp);
        p->readInt32(&p_cur->LTE_SignalStrength.rsrq);
        appendPrintBuf("LTE rsrq=%d,", p_cur->LTE_SignalStrength.rsrq);
        p->readInt32(&p_cur->LTE_SignalStrength.rssnr);
        appendPrintBuf("LTE rssnr=%d,", p_cur->LTE_SignalStrength.rssnr);
        p->readInt32(&p_cur->LTE_SignalStrength.cqi);
        appendPrintBuf("LTE cqi=%d,", p_cur->LTE_SignalStrength.cqi);
        p->readInt32(&p_cur->LTE_SignalStrength.timingAdvance);
        appendPrintBuf("LTE timingAdvance=%d, ", p_cur->LTE_SignalStrength.timingAdvance);
        p->readInt32(&p_cur->TD_SCDMA_SignalStrength.signalStrength);
        appendPrintBuf("TD_SCDMA signalStrength=%d,", p_cur->TD_SCDMA_SignalStrength.signalStrength);
        p->readInt32(&p_cur->TD_SCDMA_SignalStrength.bitErrorRate);
        appendPrintBuf("TD_SCDMA bitErrorRate=%d,", p_cur->TD_SCDMA_SignalStrength.bitErrorRate);
        p->readInt32(&p_cur->TD_SCDMA_SignalStrength.rscp);
        appendPrintBuf("TD_SCDMA rscp=%d,", p_cur->TD_SCDMA_SignalStrength.rscp);
        p->readInt32(&p_cur->WCDMA_SignalStrength.signalStrength);
        appendPrintBuf("WCDMA signalStrength=%d,", p_cur->WCDMA_SignalStrength.signalStrength);
        p->readInt32(&p_cur->WCDMA_SignalStrength.bitErrorRate);
        appendPrintBuf("WCDMA bitErrorRate=%d,", p_cur->WCDMA_SignalStrength.bitErrorRate);
        p->readInt32(&p_cur->WCDMA_SignalStrength.rscp);
        appendPrintBuf("WCDMA rscp=%d,", p_cur->WCDMA_SignalStrength.rscp);
        p->readInt32(&p_cur->WCDMA_SignalStrength.ecno);
        appendPrintBuf("WCDMA ecno=%d,", p_cur->WCDMA_SignalStrength.ecno);
    }
    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToRilSignalStrength request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || p_cur != NULL) {
            RfxRilAdapter::responseToRilj(t, e, p_cur, sizeof(RIL_SignalStrength_v12));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, p_cur, sizeof(RIL_SignalStrength_v12), RIL_SOCKET_ID(slotId));
    }
    if (p_cur != NULL) {
        free(p_cur);
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToRilSignalStrength_v14(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_SignalStrength_v14 *p_cur = NULL;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;

    if (p->dataAvail() > 0) {
        p_cur = (RIL_SignalStrength_v14*)calloc(1,
            sizeof(RIL_SignalStrength_v14));
        if(p_cur == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        int32_t  v;
        RFX_LOG_D(LOG_TAG, "parcelToRilSignalStrength request %d, type %d, parcelsize %d", id, type, p->dataSize());

        // Only used in Response

        p->readInt32(&p_cur->GW_SignalStrength.signalStrength);
        appendPrintBuf("V5 GW signalStrength=%d,", p_cur->GW_SignalStrength.signalStrength);
        p->readInt32(&p_cur->GW_SignalStrength.bitErrorRate);
        appendPrintBuf("GW bitErrorRate=%d,", p_cur->GW_SignalStrength.bitErrorRate);
        p->readInt32(&p_cur->GW_SignalStrength.timingAdvance);
        appendPrintBuf("GW timingAdvance=%d,", p_cur->GW_SignalStrength.timingAdvance);
        p->readInt32(&p_cur->CDMA_SignalStrength.dbm);
        appendPrintBuf("CDMA dbm=%d,", p_cur->CDMA_SignalStrength.dbm);
        p->readInt32(&p_cur->CDMA_SignalStrength.ecio);
        appendPrintBuf("CDMA ecio=%d,", p_cur->CDMA_SignalStrength.ecio);
        p->readInt32(&p_cur->EVDO_SignalStrength.dbm);
        appendPrintBuf("EVDO dbm=%d,", p_cur->EVDO_SignalStrength.dbm);
        p->readInt32(&p_cur->EVDO_SignalStrength.ecio);
        appendPrintBuf("EVDO ecio=%d,", p_cur->EVDO_SignalStrength.ecio);
        p->readInt32(&p_cur->EVDO_SignalStrength.signalNoiseRatio);
        appendPrintBuf("EVDO signalNoiseRatio=%d,", p_cur->EVDO_SignalStrength.signalNoiseRatio);
        p->readInt32(&p_cur->LTE_SignalStrength.signalStrength);
        appendPrintBuf("V6 LTE signalStrength=%d,", p_cur->LTE_SignalStrength.signalStrength);
        p->readInt32(&p_cur->LTE_SignalStrength.rsrp);
        appendPrintBuf("LTE rsrp=%d,", p_cur->LTE_SignalStrength.rsrp);
        p->readInt32(&p_cur->LTE_SignalStrength.rsrq);
        appendPrintBuf("LTE rsrq=%d,", p_cur->LTE_SignalStrength.rsrq);
        p->readInt32(&p_cur->LTE_SignalStrength.rssnr);
        appendPrintBuf("LTE rssnr=%d,", p_cur->LTE_SignalStrength.rssnr);
        p->readInt32(&p_cur->LTE_SignalStrength.cqi);
        appendPrintBuf("LTE cqi=%d,", p_cur->LTE_SignalStrength.cqi);
        p->readInt32(&p_cur->LTE_SignalStrength.timingAdvance);
        appendPrintBuf("LTE timingAdvance=%d, ", p_cur->LTE_SignalStrength.timingAdvance);
        p->readInt32(&p_cur->TD_SCDMA_SignalStrength.signalStrength);
        appendPrintBuf("TD_SCDMA signalStrength=%d,", p_cur->TD_SCDMA_SignalStrength.signalStrength);
        p->readInt32(&p_cur->TD_SCDMA_SignalStrength.bitErrorRate);
        appendPrintBuf("TD_SCDMA bitErrorRate=%d,", p_cur->TD_SCDMA_SignalStrength.bitErrorRate);
        p->readInt32(&p_cur->TD_SCDMA_SignalStrength.rscp);
        appendPrintBuf("TD_SCDMA rscp=%d,", p_cur->TD_SCDMA_SignalStrength.rscp);
        p->readInt32(&p_cur->WCDMA_SignalStrength.signalStrength);
        appendPrintBuf("WCDMA signalStrength=%d,", p_cur->WCDMA_SignalStrength.signalStrength);
        p->readInt32(&p_cur->WCDMA_SignalStrength.bitErrorRate);
        appendPrintBuf("WCDMA bitErrorRate=%d,", p_cur->WCDMA_SignalStrength.bitErrorRate);
        p->readInt32(&p_cur->WCDMA_SignalStrength.rscp);
        appendPrintBuf("WCDMA rscp=%d,", p_cur->WCDMA_SignalStrength.rscp);
        p->readInt32(&p_cur->WCDMA_SignalStrength.ecno);
        appendPrintBuf("WCDMA ecno=%d,", p_cur->WCDMA_SignalStrength.ecno);
        p->readInt32(&p_cur->NR_SignalStrength.ssRsrp);
        appendPrintBuf("NR ssRsrp=%d,", p_cur->NR_SignalStrength.ssRsrp);
        p->readInt32(&p_cur->NR_SignalStrength.ssRsrq);
        appendPrintBuf("NR ssRsrq=%d,", p_cur->NR_SignalStrength.ssRsrq);
        p->readInt32(&p_cur->NR_SignalStrength.ssSinr);
        appendPrintBuf("LTE ssSinr=%d,", p_cur->NR_SignalStrength.ssSinr);
        p->readInt32(&p_cur->NR_SignalStrength.csiRsrp);
        appendPrintBuf("NR csiRsrp=%d,", p_cur->NR_SignalStrength.csiRsrp);
        p->readInt32(&p_cur->NR_SignalStrength.csiRsrq);
        appendPrintBuf("NR csiRsrq=%d,", p_cur->NR_SignalStrength.csiRsrq);
        p->readInt32(&p_cur->NR_SignalStrength.csiSinr);
        appendPrintBuf("NR csiSinr=%d,", p_cur->NR_SignalStrength.csiSinr);
    }
    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToRilSignalStrength request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || p_cur != NULL) {
            RfxRilAdapter::responseToRilj(t, e, p_cur, sizeof(RIL_SignalStrength_v14));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, p_cur, sizeof(RIL_SignalStrength_v14), RIL_SOCKET_ID(slotId));
    }
    if (p_cur != NULL) {
        free(p_cur);
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToRilSignalStrength(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    if(RIL_VERSION < 12) {
        return parcelToRilSignalStrength_v12(t,e,id,p,slotId);
    } else {
        return parcelToRilSignalStrength_v14(t,e,id,p,slotId);
    }
}

/**
 * Marshall the signalInfoRecord into the parcel if it exists.
 */
static void readSignalInfoRecord(Parcel *p,
            RIL_CDMA_SignalInfoRecord &p_signalInfoRecord) {
    p->writeInt32(p_signalInfoRecord.isPresent);
    p->writeInt32(p_signalInfoRecord.signalType);
    p->writeInt32(p_signalInfoRecord.alertPitch);
    p->writeInt32(p_signalInfoRecord.signal);
}

static void parcelToCallRing(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int type = getTypeAndJumpToData(p, id);
    RFX_LOG_D(LOG_TAG, "parcelToCallRing request %d, type %d, parcelsize %d", id, type, p->dataSize());

    // Only used in URC
    if(p->dataSize() == 2) {
        // No response data
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, NULL, 0, RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, NULL, 0);
    #endif
    } else {
        RIL_CDMA_SignalInfoRecord entry;

        memset(&entry, 0, sizeof(entry));
        if (p->dataAvail() > 0) {
            readSignalInfoRecord(p, entry);
        }
        RFX_LOG_D(LOG_TAG, "parcelToCdmaSignalInfoRecord request %d, type %d", id, type);
        if (type == 0) {
            // Response
            if (e == 0) {
                RfxRilAdapter::responseToRilj(t, e, &entry, sizeof(RIL_CDMA_SignalInfoRecord));
            } else {
                RfxRilAdapter::responseToRilj(t, e, NULL, 0);
            }
        } else {
            // URC
    #if defined(ANDROID_MULTI_SIM)
            RfxRilAdapter::responseToRilj(id, &entry, sizeof(RIL_CDMA_SignalInfoRecord),
                    RIL_SOCKET_ID(slotId));
    #else
            RfxRilAdapter::responseToRilj(id, &entry, sizeof(RIL_CDMA_SignalInfoRecord));
    #endif
        }
    }
}

static void parcelToCdmaSignalInfoRecord(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CDMA_SignalInfoRecord entry;
    int type = getTypeAndJumpToData(p, id);

    memset(&entry, 0, sizeof(entry));
    if (p->dataAvail() > 0) {
        readSignalInfoRecord(p, entry);
    }

    RFX_LOG_D(LOG_TAG, "parcelToCdmaSignalInfoRecord request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &entry, sizeof(RIL_CDMA_SignalInfoRecord));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &entry, sizeof(RIL_CDMA_SignalInfoRecord),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &entry, sizeof(RIL_CDMA_SignalInfoRecord));
    #endif
    }
}


static void parcelToCdmaCallWaiting(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CDMA_CallWaiting_v6 entry;

    int type = getTypeAndJumpToData(p, id);
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, ((RequestInfo *)t)->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }

    dataStart;

    memset(&entry, 0, sizeof(entry));
    if (p->dataAvail() > 0) {
        entry.number = strdupReadString(p);
        appendPrintBuf("number=%s, ", entry.number);
        p->readInt32(&entry.numberPresentation);
        appendPrintBuf("numberPresentation=%d, ", entry.numberPresentation);
        entry.name = strdupReadString(p);
        appendPrintBuf("name=%s, ", entry.name);
        readSignalInfoRecord(p, entry.signalInfoRecord);
        p->readInt32(&entry.number_type);
        appendPrintBuf("number_type=%d, ", entry.number_type);
        p->readInt32(&entry.number_plan);
        appendPrintBuf("number_plan=%d, ", entry.number_plan);
    }

    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToCdmaCallWaiting request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &entry, sizeof(RIL_CDMA_CallWaiting_v6));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &entry, sizeof(RIL_CDMA_CallWaiting_v6),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &entry, sizeof(RIL_CDMA_CallWaiting_v6));
    #endif
    }
    free(entry.number);
    free(entry.name);
}

static void parcelToSimRefresh(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int32_t  v;
    int type = getTypeAndJumpToData(p, id);
    RFX_LOG_D(LOG_TAG, "RIL_VERSION %d, parcelToSimRefresh request %d, type %d",
            RIL_VERSION, id, type);

    if (RIL_VERSION >= 7) {
        RIL_SimRefreshResponse_v7 entry;

        memset(&entry, 0, sizeof(entry));
        if (p->dataAvail() > 0) {
            p->readInt32(&v);
            entry.result = RIL_SimRefreshResult(v);
            p->readInt32(&entry.ef_id);
            entry.aid = strdupReadString(p);
        }

        if (type == 0) {
            // Response
            if (e == 0) {
                RfxRilAdapter::responseToRilj(t, e, &entry, sizeof(RIL_SimRefreshResponse_v7));
            } else {
                RfxRilAdapter::responseToRilj(t, e, NULL, 0);
            }
        } else {
            // URC
        #if defined(ANDROID_MULTI_SIM)
            RfxRilAdapter::responseToRilj(id, &entry, sizeof(RIL_SimRefreshResponse_v7),
                    RIL_SOCKET_ID(slotId));
        #else
            RfxRilAdapter::responseToRilj(id, &entry, sizeof(RIL_SimRefreshResponse_v7));
        #endif
        }
        if (entry.aid != NULL) {
            free(entry.aid);
        }
    } else {
        int *entry = NULL;
        if (p->dataAvail() > 0) {
            entry = (int *)calloc(1, 2 * sizeof(int));
            if(entry == NULL) {
                RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
                goto calloc_error;
            }
            p->readInt32(&entry[0]);
            p->readInt32(&entry[1]);
        }

        if (type == 0) {
            // Response
            if (e == 0 || entry != NULL) {
                RfxRilAdapter::responseToRilj(t, e, entry, 2*sizeof(int));
            } else {
                RfxRilAdapter::responseToRilj(t, e, NULL, 0);
            }
        } else {
            // URC
        #if defined(ANDROID_MULTI_SIM)
            RfxRilAdapter::responseToRilj(id, entry, 2*sizeof(int),
                    RIL_SOCKET_ID(slotId));
        #else
            RfxRilAdapter::responseToRilj(id, entry, 2*sizeof(int));
        #endif
        }
        if (entry != NULL) {
            free(entry);
        }
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}


static void parcelToCellInfoList_v11(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CellInfo *entries = NULL;
    int32_t  v;
    int64_t  v6;
    int32_t num = 0;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;

    if (p->dataAvail() > 0) {
        p->readInt32(&num);
        appendPrintBuf("num=%d,", num);
        entries = (RIL_CellInfo *) calloc(1, sizeof(RIL_CellInfo) * num);
        if(entries == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        status_t status = 0;
        for (int i = 0; i< num; i++) {
            p->readInt32(&v);

            entries[i].cellInfoType = RIL_CellInfoType(v);
            p->readInt32(&entries[i].registered);
            p->readInt32(&v);
            entries[i].timeStampType = RIL_TimeStampType(v);
            status = p->readInt64(&v6);
            if (status != android::NO_ERROR) {
                RLOGE(" parcelToCellInfoList_v11 readInt64 fail");
                entries[i].timeStamp = 0;
            }else {
                entries[i].timeStamp = v6;
            }
            appendPrintBuf("\ni=%d, cellInfoType=%d,",i, entries[i].cellInfoType);
            appendPrintBuf("registered=%d,", entries[i].registered);
            appendPrintBuf("timeStampType=%d,", entries[i].timeStampType);
            appendPrintBuf("timeStamp=%lu,", entries[i].timeStamp);
            switch(entries[i].cellInfoType) {
                case RIL_CELL_INFO_TYPE_GSM: {
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.mcc);
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.mnc);
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.lac);
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.cid);
                    p->readInt32(&entries[i].CellInfo.gsm.signalStrengthGsm.signalStrength);
                    p->readInt32(&entries[i].CellInfo.gsm.signalStrengthGsm.bitErrorRate);
                    p->readInt32(&entries[i].CellInfo.gsm.signalStrengthGsm.timingAdvance);
                    appendPrintBuf("gsm mcc=%d,", entries[i].CellInfo.gsm.cellIdentityGsm.mcc);
                    appendPrintBuf("mnc=%d,", entries[i].CellInfo.gsm.cellIdentityGsm.mnc);
                    appendPrintBuf("lac=%d,", entries[i].CellInfo.gsm.cellIdentityGsm.lac);
                    appendPrintBuf("cid=%X,", entries[i].CellInfo.gsm.cellIdentityGsm.cid);
                    appendPrintBuf("signalStrength=%d,", entries[i].CellInfo.gsm.signalStrengthGsm.signalStrength);
                    appendPrintBuf("bitErrorRate=%d,", entries[i].CellInfo.gsm.signalStrengthGsm.bitErrorRate);
                    appendPrintBuf("timingAdvance=%d", entries[i].CellInfo.gsm.signalStrengthGsm.timingAdvance);

                    break;
                }
                case RIL_CELL_INFO_TYPE_WCDMA: {
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.mcc);
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.mnc);
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.lac);
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.cid);
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.psc);
                    p->readInt32(&entries[i].CellInfo.wcdma.signalStrengthWcdma.signalStrength);
                    p->readInt32(&entries[i].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate);
                    appendPrintBuf("wcdma mcc=%d,", entries[i].CellInfo.wcdma.cellIdentityWcdma.mcc);
                    appendPrintBuf("mnc=%d,", entries[i].CellInfo.wcdma.cellIdentityWcdma.mnc);
                    appendPrintBuf("lac=%d,", entries[i].CellInfo.wcdma.cellIdentityWcdma.lac);
                    appendPrintBuf("cid=%X,", entries[i].CellInfo.wcdma.cellIdentityWcdma.cid);
                    appendPrintBuf("psc=%d,", entries[i].CellInfo.wcdma.cellIdentityWcdma.psc);
                    appendPrintBuf("signalStrength=%d,", entries[i].CellInfo.wcdma.signalStrengthWcdma.signalStrength);
                    appendPrintBuf("bitErrorRate=%d,",  entries[i].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate);
                    break;
                }
                case RIL_CELL_INFO_TYPE_CDMA: {
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.networkId);
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.systemId);
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.basestationId);
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.longitude);
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.latitude);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthCdma.dbm);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthCdma.ecio);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthEvdo.dbm);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthEvdo.ecio);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio);

                    appendPrintBuf("Cdma networkId=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.networkId);
                    appendPrintBuf("systemId=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.systemId);
                    appendPrintBuf("basestationId=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.basestationId);
                    appendPrintBuf("longitude=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.longitude);
                    appendPrintBuf("latitude=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.latitude);
                    appendPrintBuf("dbm=%d,", entries[i].CellInfo.cdma.signalStrengthCdma.dbm);
                    appendPrintBuf("ecio=%d,", entries[i].CellInfo.cdma.signalStrengthCdma.ecio);
                    appendPrintBuf("dbm=%d,", entries[i].CellInfo.cdma.signalStrengthEvdo.dbm);
                    appendPrintBuf("ecio=%d,", entries[i].CellInfo.cdma.signalStrengthEvdo.ecio);
                    appendPrintBuf("signalNoiseRatio=%d,", entries[i].CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio);

                    break;
                }
                case RIL_CELL_INFO_TYPE_LTE: {
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.mcc);
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.mnc);
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.ci);
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.pci);
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.tac);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.signalStrength);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.rsrp);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.rsrq);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.rssnr);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.cqi);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.timingAdvance);


                    appendPrintBuf("Lte mcc=%d,", entries[i].CellInfo.lte.cellIdentityLte.mcc);
                    appendPrintBuf("mnc=%d,", entries[i].CellInfo.lte.cellIdentityLte.mnc);
                    appendPrintBuf("ci=%X,",  entries[i].CellInfo.lte.cellIdentityLte.ci);
                    appendPrintBuf("pci=%d,", entries[i].CellInfo.lte.cellIdentityLte.pci);
                    appendPrintBuf("tac=%X,", entries[i].CellInfo.lte.cellIdentityLte.tac);
                    appendPrintBuf("signalStrength=%d,", entries[i].CellInfo.lte.signalStrengthLte.signalStrength);
                    appendPrintBuf("rsrp=%d,", entries[i].CellInfo.lte.signalStrengthLte.rsrp);
                    appendPrintBuf("rsrq=%d,", entries[i].CellInfo.lte.signalStrengthLte.rsrq);
                    appendPrintBuf("rssnr=%d,", entries[i].CellInfo.lte.signalStrengthLte.rssnr);
                    appendPrintBuf("cqi=%d,", entries[i].CellInfo.lte.signalStrengthLte.cqi);
                    appendPrintBuf("timingAdvance=%d,", entries[i].CellInfo.lte.signalStrengthLte.timingAdvance);
                    break;
                }
                case RIL_CELL_INFO_TYPE_TD_SCDMA: {
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.mcc);
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.mnc);
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.lac);
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.cid);
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.cpid);
                    p->readInt32(&entries[i].CellInfo.tdscdma.signalStrengthTdscdma.rscp);


                    appendPrintBuf("tdscdma mcc=%d,", entries[i].CellInfo.tdscdma.cellIdentityTdscdma.mcc);
                    appendPrintBuf("mnc=%d,", entries[i].CellInfo.tdscdma.cellIdentityTdscdma.mnc);
                    appendPrintBuf("lac=%d,", entries[i].CellInfo.tdscdma.cellIdentityTdscdma.lac);
                    appendPrintBuf("cid=%X,", entries[i].CellInfo.tdscdma.cellIdentityTdscdma.cid);
                    appendPrintBuf("cpid=%d,",entries[i].CellInfo.tdscdma.cellIdentityTdscdma.cpid);
                    appendPrintBuf("rscp=%d,",entries[i].CellInfo.tdscdma.signalStrengthTdscdma.rscp);
                    break;
                }
            }
        }
    }
    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToCellInfoList request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || entries != NULL) {
            RfxRilAdapter::responseToRilj(t, e, entries, num * sizeof(RIL_CellInfo));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
#if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, entries, num*sizeof(RIL_CellInfo),
                RIL_SOCKET_ID(slotId));
#else
        RfxRilAdapter::responseToRilj(id, entries, num*sizeof(RIL_CellInfo));
#endif
    }
    if (entries != NULL) {
        free(entries);
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToCellInfoList_v12(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CellInfo_v12 *entries = NULL;
    int32_t  v;
    int64_t  v6;
    int32_t num = 0;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;

    if (p->dataAvail() > 0) {
        p->readInt32(&num);
        appendPrintBuf("num=%d,", num);
        entries = (RIL_CellInfo_v12 *) calloc(1, sizeof(RIL_CellInfo_v12) * num);
        if(entries == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }

        status_t status = 0;
        for (int i = 0; i< num; i++) {
            p->readInt32(&v);

            entries[i].cellInfoType = RIL_CellInfoType(v);
            p->readInt32(&entries[i].registered);
            p->readInt32(&v);
            entries[i].timeStampType = RIL_TimeStampType(v);
            status = p->readInt64(&v6);
            if (status != android::NO_ERROR) {
                RLOGE(" parcelToCellInfoList_v12 readInt64 fail");
                entries[i].timeStamp = 0;
            }else {
                entries[i].timeStamp = v6;
            }
            appendPrintBuf("\ni=%d, cellInfoType=%d,",i, entries[i].cellInfoType);
            appendPrintBuf("registered=%d,", entries[i].registered);
            appendPrintBuf("timeStampType=%d,", entries[i].timeStampType);
            appendPrintBuf("timeStamp=%lu,", entries[i].timeStamp);
            switch(entries[i].cellInfoType) {
                case RIL_CELL_INFO_TYPE_GSM: {
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.mcc);
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.mnc);
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.lac);
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.cid);
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.arfcn);
                    p->readInt32(&entries[i].CellInfo.gsm.cellIdentityGsm.bsic);
                    p->readInt32(&entries[i].CellInfo.gsm.signalStrengthGsm.signalStrength);
                    p->readInt32(&entries[i].CellInfo.gsm.signalStrengthGsm.bitErrorRate);
                    p->readInt32(&entries[i].CellInfo.gsm.signalStrengthGsm.timingAdvance);
                    appendPrintBuf("gsm mcc=%d,", entries[i].CellInfo.gsm.cellIdentityGsm.mcc);
                    appendPrintBuf("mnc=%d,", entries[i].CellInfo.gsm.cellIdentityGsm.mnc);
                    appendPrintBuf("lac=%d,", entries[i].CellInfo.gsm.cellIdentityGsm.lac);
                    appendPrintBuf("cid=%X,", entries[i].CellInfo.gsm.cellIdentityGsm.cid);
                    appendPrintBuf("arfcn=%d,", entries[i].CellInfo.gsm.cellIdentityGsm.arfcn);
                    appendPrintBuf("bsic=%d,", entries[i].CellInfo.gsm.cellIdentityGsm.bsic);
                    appendPrintBuf("signalStrength=%d,", entries[i].CellInfo.gsm.signalStrengthGsm.signalStrength);
                    appendPrintBuf("bitErrorRate=%d,", entries[i].CellInfo.gsm.signalStrengthGsm.bitErrorRate);
                    appendPrintBuf("timingAdvance=%d", entries[i].CellInfo.gsm.signalStrengthGsm.timingAdvance);

                    break;
                }
                case RIL_CELL_INFO_TYPE_WCDMA: {
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.mcc);
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.mnc);
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.lac);
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.cid);
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.psc);
                    p->readInt32(&entries[i].CellInfo.wcdma.cellIdentityWcdma.uarfcn);
                    p->readInt32(&entries[i].CellInfo.wcdma.signalStrengthWcdma.signalStrength);
                    p->readInt32(&entries[i].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate);
                    appendPrintBuf("wcdma mcc=%d,", entries[i].CellInfo.wcdma.cellIdentityWcdma.mcc);
                    appendPrintBuf("mnc=%d,", entries[i].CellInfo.wcdma.cellIdentityWcdma.mnc);
                    appendPrintBuf("lac=%d,", entries[i].CellInfo.wcdma.cellIdentityWcdma.lac);
                    appendPrintBuf("cid=%X,", entries[i].CellInfo.wcdma.cellIdentityWcdma.cid);
                    appendPrintBuf("psc=%d,", entries[i].CellInfo.wcdma.cellIdentityWcdma.psc);
                    appendPrintBuf("uarfcn=%d,", entries[i].CellInfo.wcdma.cellIdentityWcdma.uarfcn);
                    appendPrintBuf("signalStrength=%d,", entries[i].CellInfo.wcdma.signalStrengthWcdma.signalStrength);
                    appendPrintBuf("bitErrorRate=%d,",  entries[i].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate);
                    break;
                }
                case RIL_CELL_INFO_TYPE_CDMA: {
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.networkId);
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.systemId);
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.basestationId);
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.longitude);
                    p->readInt32(&entries[i].CellInfo.cdma.cellIdentityCdma.latitude);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthCdma.dbm);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthCdma.ecio);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthEvdo.dbm);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthEvdo.ecio);
                    p->readInt32(&entries[i].CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio);

                    appendPrintBuf("Cdma networkId=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.networkId);
                    appendPrintBuf("systemId=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.systemId);
                    appendPrintBuf("basestationId=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.basestationId);
                    appendPrintBuf("longitude=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.longitude);
                    appendPrintBuf("latitude=%d,", entries[i].CellInfo.cdma.cellIdentityCdma.latitude);
                    appendPrintBuf("dbm=%d,", entries[i].CellInfo.cdma.signalStrengthCdma.dbm);
                    appendPrintBuf("ecio=%d,", entries[i].CellInfo.cdma.signalStrengthCdma.ecio);
                    appendPrintBuf("dbm=%d,", entries[i].CellInfo.cdma.signalStrengthEvdo.dbm);
                    appendPrintBuf("ecio=%d,", entries[i].CellInfo.cdma.signalStrengthEvdo.ecio);
                    appendPrintBuf("signalNoiseRatio=%d,", entries[i].CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio);

                    break;
                }
                case RIL_CELL_INFO_TYPE_LTE: {
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.mcc);
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.mnc);
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.ci);
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.pci);
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.tac);
                    p->readInt32(&entries[i].CellInfo.lte.cellIdentityLte.earfcn);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.signalStrength);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.rsrp);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.rsrq);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.rssnr);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.cqi);
                    p->readInt32(&entries[i].CellInfo.lte.signalStrengthLte.timingAdvance);

                    appendPrintBuf("Lte mcc=%d,", entries[i].CellInfo.lte.cellIdentityLte.mcc);
                    appendPrintBuf("mnc=%d,", entries[i].CellInfo.lte.cellIdentityLte.mnc);
                    appendPrintBuf("ci=%X,",  entries[i].CellInfo.lte.cellIdentityLte.ci);
                    appendPrintBuf("pci=%d,", entries[i].CellInfo.lte.cellIdentityLte.pci);
                    appendPrintBuf("tac=%X,", entries[i].CellInfo.lte.cellIdentityLte.tac);
                    appendPrintBuf("earfcn=%d,", entries[i].CellInfo.lte.cellIdentityLte.earfcn);
                    appendPrintBuf("signalStrength=%d,", entries[i].CellInfo.lte.signalStrengthLte.signalStrength);
                    appendPrintBuf("rsrp=%d,", entries[i].CellInfo.lte.signalStrengthLte.rsrp);
                    appendPrintBuf("rsrq=%d,", entries[i].CellInfo.lte.signalStrengthLte.rsrq);
                    appendPrintBuf("rssnr=%d,", entries[i].CellInfo.lte.signalStrengthLte.rssnr);
                    appendPrintBuf("cqi=%d,", entries[i].CellInfo.lte.signalStrengthLte.cqi);
                    appendPrintBuf("timingAdvance=%d,", entries[i].CellInfo.lte.signalStrengthLte.timingAdvance);
                    break;
                }
                case RIL_CELL_INFO_TYPE_TD_SCDMA: {
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.mcc);
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.mnc);
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.lac);
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.cid);
                    p->readInt32(&entries[i].CellInfo.tdscdma.cellIdentityTdscdma.cpid);
                    p->readInt32(&entries[i].CellInfo.tdscdma.signalStrengthTdscdma.rscp);


                    appendPrintBuf("tdscdma mcc=%d,", entries[i].CellInfo.tdscdma.cellIdentityTdscdma.mcc);
                    appendPrintBuf("mnc=%d,", entries[i].CellInfo.tdscdma.cellIdentityTdscdma.mnc);
                    appendPrintBuf("lac=%d,", entries[i].CellInfo.tdscdma.cellIdentityTdscdma.lac);
                    appendPrintBuf("cid=%X,", entries[i].CellInfo.tdscdma.cellIdentityTdscdma.cid);
                    appendPrintBuf("cpid=%d,",entries[i].CellInfo.tdscdma.cellIdentityTdscdma.cpid);
                    appendPrintBuf("rscp=%d,",entries[i].CellInfo.tdscdma.signalStrengthTdscdma.rscp);
                    break;
                }
                case RIL_CELL_INFO_TYPE_NR: {
                    p->readInt32(&entries[i].CellInfo.nr.cellidentity.mcc);
                    p->readInt32(&entries[i].CellInfo.nr.cellidentity.mnc);
                    p->readUint64(&entries[i].CellInfo.nr.cellidentity.nci);
                    p->readUint32(&entries[i].CellInfo.nr.cellidentity.pci);
                    p->readInt32(&entries[i].CellInfo.nr.cellidentity.tac);
                    p->readInt32(&entries[i].CellInfo.nr.cellidentity.nrarfcn);
                    p->readInt32(&entries[i].CellInfo.nr.signalStrength.ssRsrp);
                    p->readInt32(&entries[i].CellInfo.nr.signalStrength.ssRsrq);
                    p->readInt32(&entries[i].CellInfo.nr.signalStrength.ssSinr);
                    p->readInt32(&entries[i].CellInfo.nr.signalStrength.csiRsrp);
                    p->readInt32(&entries[i].CellInfo.nr.signalStrength.csiRsrq);
                    p->readInt32(&entries[i].CellInfo.nr.signalStrength.csiSinr);

                    appendPrintBuf("Nr mcc=%d,", entries[i].CellInfo.nr.cellidentity.mcc);
                    appendPrintBuf("mnc=%d,", entries[i].CellInfo.nr.cellidentity.mnc);
                    appendPrintBuf("nci=%" PRIu64",",  entries[i].CellInfo.nr.cellidentity.nci);
                    appendPrintBuf("pci=%d,", entries[i].CellInfo.nr.cellidentity.pci);
                    appendPrintBuf("tac=%X,", entries[i].CellInfo.nr.cellidentity.tac);
                    appendPrintBuf("nrarfcn=%d,", entries[i].CellInfo.nr.cellidentity.nrarfcn);
                    appendPrintBuf("ssRsrp=%d,", entries[i].CellInfo.nr.signalStrength.ssRsrp);
                    appendPrintBuf("ssRsrq=%d,", entries[i].CellInfo.nr.signalStrength.ssRsrq);
                    appendPrintBuf("ssSinr=%d,", entries[i].CellInfo.nr.signalStrength.ssSinr);
                    appendPrintBuf("csiRsrp=%d,", entries[i].CellInfo.nr.signalStrength.csiRsrp);
                    appendPrintBuf("csiRsrq=%d,", entries[i].CellInfo.nr.signalStrength.csiRsrq);
                    appendPrintBuf("csiSinr=%d,", entries[i].CellInfo.nr.signalStrength.csiSinr);
                    break;
                }
            }
        }
    }
    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToCellInfoList request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || entries != NULL) {
            RfxRilAdapter::responseToRilj(t, e, entries, num * sizeof(RIL_CellInfo_v12));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
#if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, entries, num*sizeof(RIL_CellInfo_v12),
                RIL_SOCKET_ID(slotId));
#else
        RfxRilAdapter::responseToRilj(id, entries, num*sizeof(RIL_CellInfo_v12));
#endif
    }
    if (entries != NULL) {
        free(entries);
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToCellInfoList(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    if(RIL_VERSION < 12) {
        return parcelToCellInfoList_v11(t,e,id,p,slotId);
    } else {
        return parcelToCellInfoList_v12(t,e,id,p,slotId);
    }
}

static void parcelToPhbEntries(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_PhbEntryStructure *entry = NULL;
    RIL_PhbEntryStructure **entries = NULL;
    int type = getTypeAndJumpToData(p, id);
    int32_t num = 0;

    if (p->dataAvail() > 0) {
        p->readInt32(&num);

        entries = (RIL_PhbEntryStructure**)calloc(1, sizeof(RIL_PhbEntryStructure*) * num);
        if(entries == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        for (int i = 0; i< num; i++) {
            entry = (RIL_PhbEntryStructure*)calloc(1, sizeof(RIL_PhbEntryStructure));
            if(entry == NULL) {
                RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
                goto calloc_error;
            }
            entries[i] = entry;
            p->readInt32(&entries[i]->type);
            p->readInt32(&entries[i]->index);
            entries[i]->number = strdupReadString(p);
            p->readInt32(&entries[i]->ton);
            entries[i]->alphaId = strdupReadString(p);
        }
    }

    RFX_LOG_D(LOG_TAG, "parcelToPhbEntries request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || entries != NULL) {
            RfxRilAdapter::responseToRilj(t, e, entries, num*sizeof(RIL_PhbEntryStructure*));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, entries, num*sizeof(RIL_PhbEntryStructure*),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, entries, num*sizeof(RIL_PhbEntryStructure*));
    #endif
    }

    if (entries != NULL) {
        for (int i=0; i<num; i++) {
            if (entries[i] != NULL) {
                free(entries[i]->number);
                free(entries[i]->alphaId);
                free(entries[i]);
            }
        }
        free(entries);
    }
    return;
calloc_error:
    if (entries != NULL) {
        for (int i=0; i<num; i++) {
            if (entries[i] != NULL) {
                free(entries[i]->number);
                free(entries[i]->alphaId);
                free(entries[i]);
            }
        }
        free(entries);
    }
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToGetSmsSimMemStatusCnf(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_SMS_Memory_Status mem_status;
    int type = getTypeAndJumpToData(p, id);

    if (p->dataAvail() > 0) {
        p->readInt32(&mem_status.used);
        p->readInt32(&mem_status.total);
    }

    RFX_LOG_D(LOG_TAG, "parcelToGetSmsSimMemStatusCnf request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &mem_status, sizeof(mem_status));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &mem_status, sizeof(mem_status),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &mem_status, sizeof(mem_status));
    #endif
    }
}

static void parcelToGetPhbMemStorage(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_PHB_MEM_STORAGE_RESPONSE entry;

    int type = getTypeAndJumpToData(p, id);

    memset(&entry, 0, sizeof(entry));
    if (p->dataAvail() > 0) {
        entry.storage = strdupReadString(p);
        p->readInt32(&entry.used);
        p->readInt32(&entry.total);
    }

    RFX_LOG_D(LOG_TAG, "parcelToGetPhbMemStorage request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &entry, sizeof(entry));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &entry, sizeof(entry),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &entry, sizeof(entry));
    #endif
    }
}

static void parcelToReadPhbEntryExt(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int32_t num = 0;
    int32_t v;
    RIL_PHB_ENTRY *entry = NULL;
    RIL_PHB_ENTRY **entries = NULL;

    int type = getTypeAndJumpToData(p, id);

    if (p->dataAvail() > 0) {
        p->readInt32(&num);
        entries = (RIL_PHB_ENTRY**)calloc(1, sizeof(RIL_PHB_ENTRY*) * num);
        if(entries == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        for (int i = 0; i< num; i++) {
            entry = (RIL_PHB_ENTRY*)calloc(1, sizeof(RIL_PHB_ENTRY));
            if(entry == NULL) {
                RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
                goto calloc_error;
            }
            entries[i] = entry;
            // index of the entry
            p->readInt32(&entries[i]->index);
            // phone number
            entries[i]->number = strdupReadString(p);
            // Type of the number
            p->readInt32(&entries[i]->type);
            // text
            entries[i]->text = strdupReadString(p);
            // hidden
            p->readInt32(&entries[i]->hidden);

            // group
            entries[i]->group = strdupReadString(p);
            // anr
            entries[i]->adnumber = strdupReadString(p);
            // Type of the adnumber
            p->readInt32(&entries[i]->adtype);
            // SNE
            entries[i]->secondtext = strdupReadString(p);
            // email
            entries[i]->email = strdupReadString(p);
        }
    }

    RFX_LOG_D(LOG_TAG, "parcelToReadPhbEntryExt request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || entries != NULL) {
            RfxRilAdapter::responseToRilj(t, e, entries, num*sizeof(RIL_PHB_ENTRY*));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, entries, num*sizeof(RIL_PHB_ENTRY*), RIL_SOCKET_ID(slotId));
    }

    if (entries != NULL) {
        for (int i=0; i<num; i++) {
            if (entries[i] != NULL) {
                free(entries[i]->number);
                free(entries[i]->text);
                free(entries[i]->group);
                free(entries[i]->adnumber);
                free(entries[i]->secondtext);
                free(entries[i]->email);
                free(entries[i]);
            }
        }
        free(entries);
    }
    return;
calloc_error:
    if (entries != NULL) {
        for (int i=0; i<num; i++) {
            if (entries[i] != NULL) {
                free(entries[i]->number);
                free(entries[i]->text);
                free(entries[i]->group);
                free(entries[i]->adnumber);
                free(entries[i]->secondtext);
                free(entries[i]->email);
                free(entries[i]);
            }
        }
        free(entries);
    }
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToSmsParams(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_SmsParams smsParams;
    int32_t v;
    int type = getTypeAndJumpToData(p, id);

    LOGD("dispatchSmsParams Enter.");

    memset(&smsParams, 0, sizeof(smsParams));
    if (p->dataAvail() > 0) {
        p->readInt32(&v);
        smsParams.format = v;

        p->readInt32(&v);
        smsParams.vp = v;

        p->readInt32(&v);
        smsParams.pid = v;

        p->readInt32(&v);
        smsParams.dcs = v;

        RFX_LOG_D(LOG_TAG, "dispatchSmsParams format: %d", smsParams.format);
        RFX_LOG_D(LOG_TAG, "dispatchSmsParams vp: %d", smsParams.vp);
        RFX_LOG_D(LOG_TAG, "dispatchSmsParams pid: %d", smsParams.pid);
        RFX_LOG_D(LOG_TAG, "dispatchSmsParams dcs: %d", smsParams.dcs);
        RFX_LOG_D(LOG_TAG, "dispatchSmsParams Send Request..");
    }

    RFX_LOG_D(LOG_TAG, "parcelToSmsParams request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &smsParams, sizeof(smsParams));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &smsParams, sizeof(smsParams),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &smsParams, sizeof(smsParams));
    #endif
    }

#ifdef MEMSET_FREED
    memset(&smsParams, 0, sizeof(smsParams));
#endif

    return;
}

static void parcelToCbConfigInfo(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CBConfigInfo configInfo;
    int type = getTypeAndJumpToData(p, id);

    memset(&configInfo, 0, sizeof(configInfo));
    if (p->dataAvail() > 0) {
        p->readInt32(&configInfo.mode);
        configInfo.channelConfigInfo = strdupReadString(p);
        configInfo.languageConfigInfo = strdupReadString(p);
        p->readInt32(&configInfo.isAllLanguageOn);
    }

    RFX_LOG_D(LOG_TAG, "parcelToCbConfigInfo request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &configInfo, sizeof(RIL_CBConfigInfo));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &configInfo, sizeof(RIL_CBConfigInfo),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &configInfo, sizeof(RIL_CBConfigInfo));
    #endif
    }
    if (configInfo.channelConfigInfo != NULL) {
        free(configInfo.channelConfigInfo);
    }
    if (configInfo.languageConfigInfo != NULL) {
        free(configInfo.languageConfigInfo);
    }
}

static void parcelToEtwsNotification(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_CBEtwsNotification response;
    int type = getTypeAndJumpToData(p, id);

    memset(&response, 0, sizeof(response));
    if (p->dataAvail() > 0) {
        p->readInt32(&response.warningType);
        p->readInt32(&response.messageId);
        p->readInt32(&response.serialNumber);
        response.plmnId = strdupReadString(p);
        response.securityInfo = strdupReadString(p);
    }

    RFX_LOG_D(LOG_TAG, "parcelToEtwsNotification request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &response, sizeof(RIL_CBEtwsNotification));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &response, sizeof(RIL_CBEtwsNotification),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &response, sizeof(RIL_CBEtwsNotification));
    #endif
    }
    if (response.plmnId != NULL) {
        free(response.plmnId);
    }
    if (response.securityInfo != NULL) {
        free(response.securityInfo);
    }
}

static void readQosFromParcel(Parcel* p, Qos* qos) {
    p->readInt32(&qos->qci);
    p->readInt32(&qos->dlGbr);
    p->readInt32(&qos->ulGbr);
    p->readInt32(&qos->dlMbr);
    p->readInt32(&qos->ulMbr);

    LOGD("readQosFromParcel [%d %d %d %d %d]", qos->qci, qos->dlGbr, qos->ulGbr, qos->dlMbr,
            qos->ulMbr);
}

static void readTftFromParcel(Parcel* p, Tft* tft) {
    int i, j, k;

    p->readInt32(&tft->operation);
    p->readInt32(&tft->pfNumber);
    LOGD("readTftFromParcel [%d %d]", tft->operation, tft->pfNumber);

    for (i = 0; i < tft->pfNumber; i++) {
        // packet filter information
        p->readInt32(&tft->pfList[i].id);
        p->readInt32(&tft->pfList[i].precedence);
        p->readInt32(&tft->pfList[i].direction);
        p->readInt32(&tft->pfList[i].networkPfIdentifier);
        p->readInt32(&tft->pfList[i].bitmap);
        char* address = strdupReadString(p);
        char* mask = strdupReadString(p);
        if (address != NULL)
            strncpy(tft->pfList[i].address, address,
                strlen(address) > (MAX_IPV6_ADDRESS_LENGTH-1) ? (MAX_IPV6_ADDRESS_LENGTH-1) : strlen(address));
        if (mask != NULL)
            strncpy(tft->pfList[i].mask, mask,
                strlen(mask) > (MAX_IPV6_ADDRESS_LENGTH-1) ? (MAX_IPV6_ADDRESS_LENGTH-1) : strlen(mask));
        p->readInt32(&tft->pfList[i].protocolNextHeader);
        p->readInt32(&tft->pfList[i].localPortLow);
        p->readInt32(&tft->pfList[i].localPortHigh);
        p->readInt32(&tft->pfList[i].remotePortLow);
        p->readInt32(&tft->pfList[i].remotePortHigh);
        p->readInt32(&tft->pfList[i].spi);
        p->readInt32(&tft->pfList[i].tos);
        p->readInt32(&tft->pfList[i].tosMask);
        p->readInt32(&tft->pfList[i].flowLabel);

        if (address != NULL) {
            memsetString(address);
            free(address);
        }
        if (mask != NULL) {
            memsetString(mask);
            free(mask);
        }
    }

    // TftParameter
    tft->tftParameter.linkedPfNumber = p->readInt32();
    for (i=0; i < tft->tftParameter.linkedPfNumber; i++)
        tft->tftParameter.linkedPfList[i] = p->readInt32();

    tft->tftParameter.authtokenFlowIdNumber = p->readInt32();
    for (i = 0; i < tft->tftParameter.authtokenFlowIdNumber; i++) {
        tft->tftParameter.authtokenFlowIdList[i].authTokenNumber = p->readInt32();
        for (j = 0; j < tft->tftParameter.authtokenFlowIdList[i].authTokenNumber; j++)
            tft->tftParameter.authtokenFlowIdList[i].authTokenList[j] = p->readInt32();

        tft->tftParameter.authtokenFlowIdList[i].flowIdNumber = p->readInt32();
        for (j = 0; j < tft->tftParameter.authtokenFlowIdList[i].flowIdNumber; j++) {
            for (k = 0; k < 4; k++)
                tft->tftParameter.authtokenFlowIdList[i].flowIdList[j][k] = p->readInt32();
        }
    }
}


static void parcelToIratStateChange(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    // no use
}

static void parcelToHardwareConfig(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_HardwareConfig **hardwareConfigs= NULL;
    RIL_HardwareConfig *hardwareConfig = NULL;
    int num = 0, temp = 0, maxLen = 0;
    char *uuid = NULL;
    int type = getTypeAndJumpToData(p , id);

    if (p->dataAvail() > 0) {
        p->readInt32(&num);
        hardwareConfigs = (RIL_HardwareConfig **) calloc(1, sizeof(RIL_HardwareConfig*) * num);
        if(hardwareConfigs == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        for (int i = 0; i < num; i++) {
            hardwareConfig = (RIL_HardwareConfig *) calloc(1, sizeof(RIL_HardwareConfig));
            if(hardwareConfig == NULL) {
                RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
                goto calloc_error;
            }
            hardwareConfigs[i] = hardwareConfig;
            p->readInt32(&temp);
            hardwareConfigs[i]->type = (RIL_HardwareConfig_Type) temp;
            switch(hardwareConfigs[i]->type) {
                case RIL_HARDWARE_CONFIG_MODEM:
                    uuid = strdupReadString(p);
                    maxLen = strlen(uuid) > strlen(hardwareConfigs[i]->uuid)?
                            strlen(hardwareConfigs[i]->uuid): strlen(uuid);
                    strncpy(hardwareConfigs[i]->uuid, uuid, maxLen);
                    p->readInt32(&temp);
                    hardwareConfigs[i]->state = (RIL_HardwareConfig_State) temp;
                    p->readInt32(&temp);
                    hardwareConfigs[i]->cfg.modem.rat = (uint32_t) temp;
                    p->readInt32(&hardwareConfigs[i]->cfg.modem.maxVoice);
                    p->readInt32(&hardwareConfigs[i]->cfg.modem.maxData);
                    p->readInt32(&hardwareConfigs[i]->cfg.modem.maxStandby);
                    free(uuid);
                    break;
                case RIL_HARDWARE_CONFIG_SIM:
                    uuid = strdupReadString(p);
                    maxLen = strlen(uuid) > strlen(hardwareConfigs[i]->uuid)?
                            strlen(hardwareConfigs[i]->uuid): strlen(uuid);
                    strncpy(hardwareConfigs[i]->uuid, uuid, maxLen);
                    free(uuid);
                    p->readInt32(&temp);
                    hardwareConfigs[i]->state = (RIL_HardwareConfig_State) temp;
                    uuid = strdupReadString(p);
                    maxLen = strlen(uuid) > strlen(hardwareConfigs[i]->cfg.sim.modemUuid)?
                            strlen(hardwareConfigs[i]->cfg.sim.modemUuid): strlen(uuid);
                    strncpy(hardwareConfigs[i]->cfg.sim.modemUuid, uuid, maxLen);
                    free(uuid);
                    break;
            }
        }
    }

    RFX_LOG_D(LOG_TAG, "parcelToHardwareConfig request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, hardwareConfigs, sizeof(RIL_HardwareConfig *) * num);
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, hardwareConfigs, sizeof(RIL_HardwareConfig *) * num,
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, hardwareConfigs, sizeof(RIL_HardwareConfig *) * num);
    #endif
    }

    if (hardwareConfigs != NULL) {
        for (int i = 0; i < num; i++) {
            if (hardwareConfigs[i] != NULL) {
                free(hardwareConfigs[i]);
            }
        }
        free(hardwareConfigs);
    }
    return;
calloc_error:
    if (hardwareConfigs != NULL) {
        for (int i = 0; i < num; i++) {
            if (hardwareConfigs[i] != NULL) {
                free(hardwareConfigs[i]);
            }
        }
        free(hardwareConfigs);
    }
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

static void parcelToDcRtInfo(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    // have implementaton, but no one use it
}

static void parcelToRadioCapability(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_RadioCapability rc;
    int32_t v = 0;
    RequestInfo *requestInfo = (RequestInfo *)t;

    int type = getTypeAndJumpToData(p, id);
    if (type == 0) { //response
        IT_RESPONSE_START(requestToString(id), slotId, requestInfo->token, e);
    } else {    //urc
        IT_UNSOL_START(requestToString(id), slotId);
    }
    dataStart;
    memset (&rc, 0, sizeof(RIL_RadioCapability));

    if (p->dataAvail() > 0) {
        p->readInt32(&v);
        rc.version = (int)v;

        p->readInt32(&v);
        rc.session= (int)v;

        p->readInt32(&v);
        rc.phase= (int)v;

        p->readInt32(&v);
        rc.rat = (int)v;

        readStringFromParcelInplace(p, rc.logicalModemUuid, sizeof(rc.logicalModemUuid));
        p->readInt32(&v);
        rc.status = (int)v;
    }


    appendPrintBuf("version=%d,", rc.version);
    appendPrintBuf("session=%d,", rc.session);
    appendPrintBuf("phase=%d,", rc.phase);
    appendPrintBuf("rat=0x%X,", rc.rat);
    appendPrintBuf("logicalModemUuid=%s,", rc.logicalModemUuid);
    appendPrintBuf("status=%d,", rc.status);
    dataEnd;
    IT_PRINT_LOG;

    RFX_LOG_D(LOG_TAG, "parcelToRadioCapability request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &rc, sizeof(RIL_RadioCapability));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &rc, sizeof(RIL_RadioCapability),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &rc, sizeof(RIL_RadioCapability));
    #endif
    }

    return;
}

static void parcelToSSData(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    // have implementaton, but no one use it
}

static void parcelToLceStatus(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_LceStatusInfo lsi;
    int32_t v = 0;
    int type = getTypeAndJumpToData(p, id);

    memset (&lsi, 0, sizeof(RIL_LceStatusInfo));

    if (p->dataAvail() > 0) {
        p->read(&v, 1);
        lsi.lce_status = (int8_t)v;

        p->readInt32(&v);
        lsi.actual_interval_ms = (uint32_t)v;
    }

    RFX_LOG_D(LOG_TAG, "parcelToLceStatus request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &lsi, sizeof(RIL_LceStatusInfo));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, &lsi, sizeof(RIL_LceStatusInfo),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, &lsi, sizeof(RIL_LceStatusInfo));
    #endif
    }

    return;
}

static void parcelToLceData(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_LceDataInfo ldi;
    int32_t v = 0;
    int type = getTypeAndJumpToData(p, id);

    memset (&ldi, 0, sizeof(RIL_LceDataInfo));

    if (p->dataAvail() > 0) {
        p->readInt32(&v);
        ldi.last_hop_capacity_kbps = (uint32_t)v;

        p->read(&v, 1);
        ldi.confidence_level = (uint8_t)v;

        p->read(&v, 1);
        ldi.lce_suspended = (uint8_t)v;
    }

    RFX_LOG_D(LOG_TAG, "parcelToLceData request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &ldi, sizeof(RIL_LceDataInfo));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
            // URC
    #if defined(ANDROID_MULTI_SIM)
            RfxRilAdapter::responseToRilj(id, &ldi, sizeof(RIL_LceDataInfo),
                    RIL_SOCKET_ID(slotId));
    #else
            RfxRilAdapter::responseToRilj(id, &ldi, sizeof(RIL_LceDataInfo));
    #endif
        }

        return;
}

static void parcelToActivityData(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    RIL_ActivityStatsInfo asi;
    int32_t v = 0;
    int type = getTypeAndJumpToData(p, id);

    memset (&asi, 0, sizeof(RIL_ActivityStatsInfo));

    if (p->dataAvail() > 0) {
        p->readInt32(&v);
        asi.sleep_mode_time_ms = (uint32_t)v;

        p->readInt32(&v);
        asi.idle_mode_time_ms = (uint32_t)v;

        for (int index = 0; index < RIL_NUM_TX_POWER_LEVELS; index++) {
            p->readInt32(&v);
            asi.tx_mode_time_ms[index] = (uint32_t)v;
        }
        p->readInt32(&v);
        asi.rx_mode_time_ms = (uint32_t)v;
    }

    RFX_LOG_D(LOG_TAG, "parcelToActivityData request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0) {
            RfxRilAdapter::responseToRilj(t, e, &asi, sizeof(RIL_ActivityStatsInfo));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
            // URC
    #if defined(ANDROID_MULTI_SIM)
            RfxRilAdapter::responseToRilj(id, &asi, sizeof(RIL_ActivityStatsInfo),
                    RIL_SOCKET_ID(slotId));
    #else
            RfxRilAdapter::responseToRilj(id, &asi, sizeof(RIL_ActivityStatsInfo));
    #endif
        }

    return;
}

static void parcelToGetNitzTime(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    size_t datalen;
    size_t stringlen = 0;
    char *string8 = NULL;
    int type = getTypeAndJumpToData(p, id);

    if (p->dataAvail() > 0) {
        string8 = strdupReadString(p);
        if(string8 == NULL) {
            goto error;
        }else {
            stringlen = strlen(string8);
        }
    }

    RFX_LOG_D(LOG_TAG, "parcelToGetNitzTime request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || string8 != NULL) {
            RfxRilAdapter::responseToRilj(t, e, string8, stringlen);
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
        RESPONSE_TO_RILJ(id, string8, stringlen, RIL_SOCKET_ID(slotId));
    }

#ifdef MEMSET_FREED
    memsetString(string8);
#endif

    if (string8 != NULL)
        free(string8);
    return;
error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
 }

static void parcelToMTKReadPbFromRuim(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
// no use
}

static void parcelToReadPbFromRuim(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
// no use
}

static void parcelToGetSmsStatus(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
// no use
}

// int isImsSupport() {
//     char property_value[PROPERTY_VALUE_MAX] = { 0 };
//     property_get("ro.mtk_ims_support", property_value, "0");
//     return atoi(property_value);
// }

int isReqFromMAL() {
    char property_value[PROPERTY_VALUE_MAX] = { 0 };
    property_get("ril.data.mal", property_value, "0");
    return atoi(property_value);
}

/* M: eMBMS feature */
Parcel* embmsStartSessionReqToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_EMBMS_StartSessionReq *rc = (RIL_EMBMS_StartSessionReq *) data;
    RFX_LOG_D(LOG_TAG, "embmsStartSessionReqToParcel id:%d,earfcnlist_count:%d,saiList_valid %d, saiList_count %d",
        rc->trans_id, rc->earfcnlist_count, rc->saiList_valid, rc->saiList_count);
    parcel->writeInt32(rc->trans_id);

    // writeByteArray will introduce endian issue
    parcel->writeInt32(rc->tmgi_info.tmgi_len);
    uint8_t uct;
    for (uint32_t i = 0; i < rc->tmgi_info.tmgi_len; i++) {
        parcel->write(&(rc->tmgi_info.tmgi[i]), sizeof(uct));
    }

    parcel->writeInt32(rc->earfcnlist_count);

    for (int i = 0; i < rc->earfcnlist_count; i++) {
        parcel->writeInt32(rc->earfcnlist[i]);
    }

    parcel->writeInt32(rc->saiList_valid);

    parcel->writeInt32(rc->saiList_count);

    for (int i = 0; i < rc->saiList_count; i++) {
        parcel->writeInt32(rc->saiList[i]);
    }
    return parcel;
}

Parcel* embmsStopSessionReqToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16;

    fillHeader(parcel, request, token);
    RIL_EMBMS_StopSessionReq *rc = (RIL_EMBMS_StopSessionReq *) data;

    parcel->writeInt32(rc->trans_id);

    // writeByteArray will introduce endian issue
    parcel->writeInt32(rc->tmgi_info.tmgi_len);
    uint8_t uct;
    for (uint32_t i = 0; i < rc->tmgi_info.tmgi_len; i++) {
        parcel->write(&(rc->tmgi_info.tmgi[i]), sizeof(uct));
    }

    return parcel;
}

/// [MT2635] add ecall parse function
Parcel* eCallToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16 = 0;
    int digitCount;
    uint8_t uct;
    int digitLimit;

    fillHeader(parcel, request, token);
    RIL_ECallReqMsg* ecall = (RIL_ECallReqMsg*) data;
    parcel->writeInt32(ecall->ecall_cat);
    parcel->writeInt32(ecall->ecall_variant);
    //parcel->writeInt32(ecall->ivs_mode);
    pString16 = strdup8to16(ecall->address, &len16);
    parcel->writeString16(pString16, len16);
    free(pString16);
    parcel->writeInt32(ecall->length);

    digitLimit= MIN(ecall->length, MSD_MAX_LENGTH);
    for (digitCount = 0 ; digitCount < digitLimit; digitCount ++) {
        parcel->write(&(ecall->msd_data[digitCount]), sizeof(uint8_t));
        RFX_LOG_D(LOG_TAG, "eCallToParcel ecall->length = %d ecall->msd_data[%d] = %d",ecall->length, digitCount, ecall->msd_data[digitCount]);
    }
    parcel->writeInt32(ecall->domain);
    RFX_LOG_D(LOG_TAG, "eCallToParcel ecall->length = %d ecall->domain = %d",ecall->length, ecall->domain);

    return parcel;
}

Parcel* eCallNumConfigToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    char16_t *pString16;
    size_t len16;

    fillHeader(parcel, request, token);
    RIL_ECallSetNum *info = (RIL_ECallSetNum*) data;
    parcel->writeInt32(info->arg_num);
    RFX_LOG_D(LOG_TAG, "eCallNumConfig arg_num is %d\n",info->arg_num);
    if(info->arg_num == 0){
        RFX_LOG_D(LOG_TAG, "clear both test/reconfig num and uri\n");
        parcel->writeInt32(-1);
        pString16 = strdup8to16("", &len16);
        parcel->writeString16(pString16, len16);
        free(pString16);
    }else if(info->arg_num == 1){
        RFX_LOG_D(LOG_TAG, "clear test/reconfig num or uri,type(%d)\n",info->type);
        parcel->writeInt32(info->type);
        pString16 = strdup8to16("", &len16);
        parcel->writeString16(pString16, len16);
        free(pString16);
    }else if(info->arg_num == 2){
        RFX_LOG_D(LOG_TAG, "set test/reconfig num or uri,type(%d),data(%s)\n",info->type,info->address);
        parcel->writeInt32(info->type);
        pString16 = strdup8to16(info->address, &len16);
        parcel->writeString16(pString16, len16);
        free(pString16);
    }
    return parcel;
}

static void parcelToEcallMsdStatus(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int msd_tx_status = 0;
    RIL_EcallMsdStatusIndMsg* ecallMsdStat = NULL;

    int type = getTypeAndJumpToData(p, id);

    if (p->dataAvail() > 0) {
        ecallMsdStat = (RIL_EcallMsdStatusIndMsg*) calloc(1,
                sizeof(RIL_EcallMsdStatusIndMsg));
        if(ecallMsdStat == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        p->readInt32(&msd_tx_status);
        ecallMsdStat->msd_tx_status =
                (RIL_Msd_Transmission_Status) msd_tx_status;
        p->readInt32(&ecallMsdStat->call_id);
    }

    RFX_LOG_D(LOG_TAG, "parcelToEcallMsdStatus request %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || ecallMsdStat != NULL) {
        RfxRilAdapter::responseToRilj(t, e, ecallMsdStat, sizeof(RIL_EcallMsdStatusIndMsg));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, ecallMsdStat, sizeof(RIL_EcallMsdStatusIndMsg),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, ecallMsdStat, sizeof(RIL_EcallMsdStatusIndMsg));
    #endif
    }

    if (ecallMsdStat != NULL) {
        free(ecallMsdStat);
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}

/// [MT2635] add ecall parse function
Parcel* setMsdToParcel(int request, int token, void* data, int datalen, RIL_Token t) {
    Parcel *parcel = new Parcel();
    size_t len16;
    int digitCount;
    uint8_t uct;
    int digitLimit;
    if(parcel == NULL) {
        return NULL;
    }

    fillHeader(parcel, request, token);
    RIL_ECallSetMSD* ecall = (RIL_ECallSetMSD*) data;
    parcel->writeInt32(ecall->call_id);
    parcel->writeInt32(ecall->length);

    digitLimit= MIN(ecall->length, MSD_MAX_LENGTH);
    for (digitCount = 0 ; digitCount < digitLimit; digitCount ++) {
        parcel->write(&(ecall->msd_data[digitCount]), sizeof(uint8_t));
        RFX_LOG_D(LOG_TAG, "setMsdToParcel msd_data[%d] = %d", digitCount, ecall->msd_data[digitCount]);
    }

    RFX_LOG_D(LOG_TAG, "setMsdToParcel call_id = %d, length = %d", ecall->call_id, ecall->length);

    return parcel;
}

static void parcelToEcallInd(RIL_Token t, RIL_Errno e, int id, Parcel* p, int slotId) {
    int ecall_ind_type = 0;
    RIL_Ecall_Unsol_Indications* ecall_ind_data = NULL;

    int type = getTypeAndJumpToData(p, id);

    if (p->dataAvail() > 0) {
        ecall_ind_data = (RIL_Ecall_Unsol_Indications*) calloc(1,
                sizeof(RIL_Ecall_Unsol_Indications));
        if(ecall_ind_data == NULL) {
            RFX_LOG_E(LOG_TAG, "%s, %s, %d calloc fail!", __FILE__, __FUNCTION__, __LINE__);
            goto calloc_error;
        }
        p->readInt32(&ecall_ind_type);
        ecall_ind_data->ind =
                (RIL_ECall_Indication) ecall_ind_type;
        p->readInt32(&ecall_ind_data->call_id);
    }

    RFX_LOG_D(LOG_TAG, "parcelToEcallInd %d, type %d", id, type);
    if (type == 0) {
        // Response
        if (e == 0 || ecall_ind_data != NULL) {
        RfxRilAdapter::responseToRilj(t, e, ecall_ind_data, sizeof(RIL_Ecall_Unsol_Indications));
        } else {
            RfxRilAdapter::responseToRilj(t, e, NULL, 0);
        }
    } else {
        // URC
    #if defined(ANDROID_MULTI_SIM)
        RfxRilAdapter::responseToRilj(id, ecall_ind_data, sizeof(RIL_Ecall_Unsol_Indications),
                RIL_SOCKET_ID(slotId));
    #else
        RfxRilAdapter::responseToRilj(id, ecall_ind_data, sizeof(RIL_Ecall_Unsol_Indications));
    #endif
    }

    if (ecall_ind_data != NULL) {
        free(ecall_ind_data);
    }
    return;
calloc_error:
    if (type == 0) {
        RfxRilAdapter::responseToRilj(t, e, NULL, 0);
    }
}
