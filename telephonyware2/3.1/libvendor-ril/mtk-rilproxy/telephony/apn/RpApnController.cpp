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
 * MediaTek Inc. (C) 2015. All rights reserved.
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

#include "RpApnController.h"
#include "data.h"

#include "RfxStatusDefs.h"
#include "RpDataUtils.h"
#include <cutils/jstring.h>
#include <RfxMainThread.h>
#include <prop/properties.h>
#include <mtk_properties.h>
#include <unistd.h>
#include <binder/Parcel.h>
#include "RfxMessage.h"
#include <string>
#include <thread>
#include <iostream>
#include <cstdio>
#include <sys/stat.h>

#define RFX_LOG_TAG "RPApnController"

extern int rfx_get_data_allowed_slotid(void);

#define XML_FILE_PATH           "/system/etc/tele/apns-conf.xml"
#define DB_FILE_NAME            "/data/vendor/telephony/apn.db"
#define SYSTEM_CMD_MAX          (100)
#define APN_DB_REGENERATE       "persist.vendor.apn.db.regenerate"


typedef void* (*PthreadPtr)(void*);
pthread_mutex_t gApnMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gApnDBMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gApnDoneCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t gApnQMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gApnUpdateMut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gApnQDnCond = PTHREAD_COND_INITIALIZER;

#define APNDB_READY (1)
#define APNDB_NOTREADY (0)
#define APNQ_READY (1)
#define APNQ_NOTREADY (0)
bool gApnDoneCond_flag = APNDB_NOTREADY;
bool gApnQ_flag = APNQ_NOTREADY;



/*****************************************************************************
 * Class RfxDataController
 * The class is created if the slot is single mode, LWG or C,
 * During class life time always communicate with one modem, gsm or c2k.
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RpApnController", RpApnController, RfxController);

RpApnController::RpApnController() :
    mDataprofileid(-1),
    g_apnlist(NULL),
    mPreferredApn(NULL),
    mdefaultApnRecord(NULL),
    mfirstApnRecord(NULL),
    mApnInit(false),
    mcc(String8("")),
    mnc(String8("")),
    emptyStr(String8("")) {
}

RpApnController::~RpApnController() {
}

void RpApnController::onInit() {
    RfxController::onInit();  // Required: invoke super class implementation

    logD(RFX_LOG_TAG," onInit");
    const int request_id_list[] = {
            RIL_REQUEST_SET_INITIAL_ATTACH_APN,  // 111
#ifdef UPDATE_APN
            RIL_REQUEST_LOCAL_UPDATE_APN,
            RIL_REQUEST_LOCAL_GET_APN,
#endif
            RIL_REQUEST_SET_DATA_PROFILE,    // 128
            RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD
            };

    registerToHandleRequest(request_id_list,
            sizeof(request_id_list) / sizeof(int));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_GSM_OPERATOR_NUMERIC,
        RfxStatusChangeCallback(this, &RpApnController::onOperatorNumberChanged));
}

void RpApnController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    if(g_apnlist){
        apn_free_list(g_apnlist);
        g_apnlist = NULL;
    }
    if(mApnInit){
        apn_destroy();
        mApnInit = false;
    }
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_GSM_OPERATOR_NUMERIC,
        RfxStatusChangeCallback(this, &RpApnController::onOperatorNumberChanged));
    RfxController::onDeinit();
}
#ifdef UPDATE_APN
bool RpApnController::onHandleRequest(const sp<RfxMessage> &message) {
    logD(RFX_LOG_TAG, "Handle request %s.",
            RpDataUtils::requestOrUrcToString(message->getId()));
    switch (message->getId()) {
    case RIL_REQUEST_LOCAL_UPDATE_APN:
        handleUpdateApnRequest(message);
        break;
    case RIL_REQUEST_LOCAL_GET_APN:
        handleGetApn(message);
        break;
    }

    return true;
}

void RpApnController::handleGetApn(const sp<RfxMessage> &message) {
    logD(RFX_LOG_TAG, " handleGetApn()");
    std::thread apn_thread = std::thread(&RpApnController::getApn, this,message);
    apn_thread.detach();
}

void RpApnController::updateApn(const sp<RfxMessage> &message) {
    pthread_mutex_lock(&gApnQMut);
    if(gApnQ_flag == APNQ_NOTREADY) {
        logD(RFX_LOG_TAG, "updateApn, apn db isn't ready ,wait mccmnc changed!");
        pthread_cond_wait(&gApnQDnCond, &gApnQMut);
        logD(RFX_LOG_TAG, "updateApn, continue");
    }
    pthread_mutex_unlock(&gApnQMut);
    pthread_mutex_lock(&gApnUpdateMut);
    logD(RFX_LOG_TAG, " updateApn mcc:%s, mnc:%s", mcc.string(),mnc.string());
    if (mcc.isEmpty() || mnc.isEmpty()) {
        logD(RFX_LOG_TAG, " updateApn fail, mcc or mnc is empty");
        pthread_mutex_unlock(&gApnUpdateMut);
        return;
    }
    Parcel *parcel = message->getParcel();
    // make sure get the right position
    parcel->setDataPosition(sizeof(int32_t) * 2);
    int32_t t = 0;
    android::status_t status;
    sp<RfxMessage> response;
    std::string protocol;
    std::string authtype;
    char *apn = NULL;
    char *user = NULL;
    char* password = NULL;
    apn_record_t *record = NULL;
    apn_record_t *dele_record = NULL;
    Parcel* responseParcel;
    RIL_Errno e = RIL_Errno::RIL_E_GENERIC_FAILURE;
    //type
    char *type = RpDataUtils::strdupReadString(parcel);
    if (type == NULL) {
        logD(RFX_LOG_TAG, "updateApn ,type is null,return");
        goto invalid_error;
    }

    //protocol
    status = parcel->readInt32(&t);
    if (status != android::NO_ERROR || t < 0) {
        logD(RFX_LOG_TAG, " updateApn protocol(%d) error, just return",t);
        goto invalid_error;
    }
    if (t == 0) {
        // APN_PDP_TYPE_IPV4
        protocol = "IP";
    } else if (t == 1) {
        //APN_PDP_TYPE_PPP
        protocol = "PPP";
    } else if (t == 2) {
        //APN_PDP_TYPE_IPV6
        protocol = "IPV6";
    } else if (t == 3) {
        //APN_PDP_TYPE_IPV4V6
        protocol = "IPV4V6";
    } else {
        logD(RFX_LOG_TAG, " updateApn protocol(%d) error, return", t);
        goto invalid_error;
    }

    //authtype
    status = parcel->readInt32(&t);
    if (status != android::NO_ERROR || t < 0 || t > 4) {
        logD(RFX_LOG_TAG, " updateApn authtype(%d) error, return", t);
        goto invalid_error;
    }
    authtype = std::to_string(t-1); //MTK only support NONO, PAP,CHAP, CHAP_PAP, don't support default
    //apn
    apn = RpDataUtils::strdupReadString(parcel);
    if (apn == NULL) {
        logD(RFX_LOG_TAG, "updateApn ,apn is null");
        goto invalid_error;
    }
    //user
    user = RpDataUtils::strdupReadString(parcel);
    if (user == NULL) {
        logD(RFX_LOG_TAG, "updateApn ,user is null");
        goto invalid_error;
    }
    //password
    password = RpDataUtils::strdupReadString(parcel);
    if (password == NULL) {
        logD(RFX_LOG_TAG, "%s ,password is null", __FUNCTION__);
        goto invalid_error;
    }
    logI(RFX_LOG_TAG,
            "%s type:%s, apn:%s, user:%s, password:%s, authtype: %s, protocol:%s",
            __FUNCTION__, type, apn, user, password, authtype.c_str(),
            protocol.c_str());
    record = apn_malloc_record();
    apn_build_iRecord(record, APN_PROP_iTYPE, type);
    apn_build_iRecord(record, APN_PROP_iMCC, mcc.string());
    apn_build_iRecord(record, APN_PROP_iMNC, mnc.string());
    apn_build_iRecord(record, APN_PROP_iAPN, apn);
    apn_build_iRecord(record, APN_PROP_iUSER, user);
    apn_build_iRecord(record, APN_PROP_iPASSWORD, password);
    apn_build_iRecord(record, APN_PROP_iAUTH_TYPE, authtype.c_str());
    apn_build_iRecord(record, APN_PROP_iPROTOCOL, protocol.c_str());
    apn_build_iRecord(record, APN_PROP_iROAMING_PROTOCOL, protocol.c_str());

    //step1: delete record that existed in db;
    dele_record = apn_malloc_record();
    apn_build_iRecord(dele_record, APN_PROP_iMCC, mcc.string());
    apn_build_iRecord(dele_record, APN_PROP_iMNC, mnc.string());
    apn_build_iRecord(dele_record, APN_PROP_iTYPE, type);
    status = apn_delete_db(dele_record);
    if (status != APN_OK) {
        logW(RFX_LOG_TAG, "%s delete(%s) fail", __FUNCTION__, type);
    }
    if(dele_record) {
        apn_free_record(dele_record);
    }

    //step2: insert record;
    status = apn_insert_record_db(record);
    if (status != APN_OK) {
        logW(RFX_LOG_TAG, "%s insert fail",__FUNCTION__);
        goto invalid_error;
    }
    GetApnDBBymccmnc();
    setDataProfilesAsNeeded();
    setInitialAttachApn();
    apn_ready_singal.postEmit();
    response = RfxMessage::obtainResponse(RIL_Errno::RIL_E_SUCCESS, message);
    responseParcel = response->getParcel();
    responseParcel->setDataPosition(sizeof(int32_t) * 3);
    responseParcel->writeInt32(0);
    goto last;
invalid_error:
    response = RfxMessage::obtainResponse(e, message);
last:
    if (user) {
        RpDataUtils::freeMemory(user);
    }
    if (password) {
        RpDataUtils::freeMemory(password);
    }
    if(apn) {
        RpDataUtils::freeMemory(apn);
    }
    if(type) {
        RpDataUtils::freeMemory(type);
    }
    if(record) {
        apn_free_record(record);
    }
    responseToRilj(response);
    logD(RFX_LOG_TAG, " %s finish",__FUNCTION__);
    pthread_mutex_unlock(&gApnUpdateMut);
}

void RpApnController::getApn(const sp<RfxMessage> &message) {
    sp<RfxMessage> response;

    logI(RFX_LOG_TAG," getApn thread!");
    pthread_mutex_lock(&gApnQMut);
    if(gApnQ_flag == APNQ_NOTREADY) {
        logD(RFX_LOG_TAG, "getApn, apn db isn't ready ,wait mccmnc changed!");
        pthread_cond_wait(&gApnQDnCond, &gApnQMut);
        logD(RFX_LOG_TAG, "getApn, continue");
    }
    pthread_mutex_unlock(&gApnQMut);
    Vector<ApnsettingT*>* list = NULL;
    Parcel *parcel = message->getParcel();
    // make sure get the right position
    parcel->setDataPosition(sizeof(int32_t) * 2);
    //type
    char *type = RpDataUtils::strdupReadString(parcel);
    if (type == NULL) {
        logW(RFX_LOG_TAG, "handleGetApn ,type is null,return");
        goto invalid_error;
    }
    list = getApnInformation(std::string(type));
    if (list == NULL || list->size() == 0) {
        logW(RFX_LOG_TAG, "handleGetApn ,list size is empty");
        goto invalid_error;
    }

    if(list->size()) {
        response = RfxMessage::obtainResponse(RIL_Errno::RIL_E_SUCCESS, message);
        Parcel* responseParcel = response->getParcel();
        responseParcel->setDataPosition(sizeof(int32_t) * 3);
        ApnsettingT* apnSetting = list->itemAt(list->size()-1);
        RpDataUtils::writeStringToParcel(responseParcel, (apnSetting->types).c_str());
        std::string protocol = apnSetting->protocol;
        int32_t pdp_type = 3;
        if(protocol == "IP") {
            pdp_type = 0;
        } else if(protocol == "PPP") {
            pdp_type = 1;
        } else if (protocol == "IPV6") {
            pdp_type = 2;
        }
        responseParcel->writeInt32(pdp_type);
        responseParcel->writeInt32((apnSetting->auth_type+1));
        RpDataUtils::writeStringToParcel(responseParcel, (apnSetting->apn).c_str());
        RpDataUtils::writeStringToParcel(responseParcel, (apnSetting->user).c_str());
        RpDataUtils::writeStringToParcel(responseParcel, (apnSetting->password).c_str());
        goto last;
    }

invalid_error:
    response = RfxMessage::obtainResponse(RIL_Errno::RIL_E_GENERIC_FAILURE, message);
last:
    responseToRilj(response);
    if(type) {
        RpDataUtils::freeMemory(type);
    }
  if(list) {
    freeApnList(list);
  }
}

void RpApnController::handleUpdateApnRequest(const sp<RfxMessage> &message) {
    logD(RFX_LOG_TAG, " handleUpdateApnRequest()");
    std::thread apn_thread = std::thread(&RpApnController::updateApn, this,message);
    apn_thread.detach();
}
#endif

bool RpApnController::onHandleResponse(const sp<RfxMessage>& message) {
//    logD(RFX_LOG_TAG, "Handle response %s.", RpDataUtils::requestToString(message->getId()));
    switch (message->getId()) {
    case RIL_REQUEST_SET_DATA_PROFILE:
        handleSetDataProfileResponse(message);
        break;
    case RIL_REQUEST_SET_INITIAL_ATTACH_APN:
        handleSetInitialAttachApnResponse(message);
        break;
    case RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD:
        handleSyncDataSetToMdResponse(message);
        break;
    default:
        logD(RFX_LOG_TAG, "unknown response, ignore!,%d",message->getId());
        break;
    }
    return true;
}
void RpApnController::handleSetInitialAttachApnResponse(const sp<RfxMessage>& response) {
    responseToRilj(response);

    if (response->getError() != RIL_E_SUCCESS) {
        logD(RFX_LOG_TAG, "SetInitialAttach response fail!");
        setInitialAttachApn();
        return;
    }else{
        logD(RFX_LOG_TAG, "SetInitialAttach response success!");
    }
}

void RpApnController::handleSyncDataSetToMdResponse(const sp<RfxMessage>& response){
    responseToRilj(response);
    if (response->getError() != RIL_E_SUCCESS) {
        logD(RFX_LOG_TAG, "SyncDataSetToMd response fail!");
        //SyncDefaultDataSimToModem();
        return;
    }else{
        logD(RFX_LOG_TAG, "SyncDataSetToMd response success!");
    }
}

void RpApnController::handleSetDataProfileResponse(const sp<RfxMessage>& response) {
    responseToRilj(response);
    if (response->getError() != RIL_E_SUCCESS) {
        logD(RFX_LOG_TAG, "Setdataprofile response fail!");
        setDataProfilesAsNeeded();
        return;
    }else{
        logD(RFX_LOG_TAG, "Setdataprofile response success!");
    }
}

#define MD5_STRING_LENGTH 33
void RpApnController::calculate_md5_with_md5sum(const char *filename, char *md5_string) {
    if (filename == NULL || md5_string == NULL) {
        logE(RFX_LOG_TAG, "calculate_md5_with_md5sum: filename or md5_string is NULL");
        return;
    }

    FILE *fp;
    char command[256] = {0};
    snprintf(command, sizeof(command), "md5sum %s", filename);
    logD(RFX_LOG_TAG, "calculate_md5_with_md5sum command: [%s]", command);
    fp = popen(command, "r");
    if (fp == NULL) {
        struct stat buf_stat;
        if (stat(filename, &buf_stat) == 0) {
            logD(RFX_LOG_TAG, "calculate_md5_with_md5sum file[%s] exists", filename);
        } else {
            logE(RFX_LOG_TAG, "calculate_md5_with_md5sum file[%s] not exists", filename);
        }

        logE(RFX_LOG_TAG, "calculate_md5_with_md5sum Failed to run command");
        strncpy(md5_string, "Error", 5);
        return;
    }

    logD(RFX_LOG_TAG, "calculate_md5_with_md5sum go to get result");
    char buffer[MD5_STRING_LENGTH] = {0};
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strncpy(md5_string, buffer, 32);
        md5_string[32] = '\0';
        logD(RFX_LOG_TAG, "calculate_md5_with_md5sum fgets return no null, result: [%s]", md5_string);
    } else {
        strncpy(md5_string, "Error", 5);
        logE(RFX_LOG_TAG, "calculate_md5_with_md5sum fgets return null");
    }

    pclose(fp);
}

void RpApnController::generateApnDB()
{
    logD(RFX_LOG_TAG," generateApnDB start,this =%p slot_id = %d", this, getSlotId());
    pthread_mutex_lock(&gApnDBMutex);
    pthread_mutex_lock(&gApnMutex);

    char apn_db_ready[PROPERTY_VALUE_MAX] = { 0 };
    property_get(APN_DB_REGENERATE, apn_db_ready, "true");
    logD(RFX_LOG_TAG," generateApnDB need regenerate apn_db=%s", apn_db_ready);

    if(strncmp(apn_db_ready, "true", 4) == 0)
    {
        if(APNDB_READY == gApnDoneCond_flag){
            apn_destroy();
        }

        if(access(DB_FILE_NAME, R_OK)==0)
        {
            logD(RFX_LOG_TAG," generateApnDB delete file=%s, start", DB_FILE_NAME);
            unlink(DB_FILE_NAME); //how to detele the file.
            while(access(DB_FILE_NAME, R_OK)==0)
            {
                logD(RFX_LOG_TAG," generateApnDB file=%s still exist", DB_FILE_NAME);
                usleep(10);
            }
            logD(RFX_LOG_TAG," generateApnDB delete file=%s, done", DB_FILE_NAME);
        }
    }

    char md5_string[MD5_STRING_LENGTH] = {0};
    calculate_md5_with_md5sum(XML_FILE_PATH, md5_string);
    logD(RFX_LOG_TAG, "MD5 of %s: %s\n", XML_FILE_PATH, md5_string);

    char apn_xml_md5sum_prop[PROPERTY_VALUE_MAX] = {0};
    property_get("persist.vendor.apn.xml.md5sum", apn_xml_md5sum_prop, "null");
    logD(RFX_LOG_TAG, "last prop value persist.vendor.apn.xml.md5sum=%s", apn_xml_md5sum_prop);
    if (strncmp("null", apn_xml_md5sum_prop, 4) == 0) {
        logD(RFX_LOG_TAG, "MD5 changed from null, set persist.vendor.apn.xml.md5sum to %s", md5_string);
        property_set("persist.vendor.apn.xml.md5sum", md5_string);
    }

    if (strncmp(md5_string, apn_xml_md5sum_prop, 32) != 0 && strncmp("null", apn_xml_md5sum_prop, 4) != 0) {
        struct stat buffer;
        if (stat(DB_FILE_NAME, &buffer) == 0) {
            logD(RFX_LOG_TAG, "MD5 changed, need to delete apn.db");
            int ret = remove(DB_FILE_NAME);
            if (ret != 0) {
                logE(RFX_LOG_TAG, "delete apn.db failed");
            }
        }
        property_set("persist.vendor.apn.xml.md5sum", md5_string);
        property_get("persist.vendor.apn.xml.md5sum", apn_xml_md5sum_prop, "null");
        logD(RFX_LOG_TAG, "set prop value persist.vendor.apn.xml.md5sum=%s", apn_xml_md5sum_prop);
    }

    if(APNDB_NOTREADY == gApnDoneCond_flag)
    {
        apn_init(apncallback);
        pthread_cond_wait(&gApnDoneCond, &gApnMutex);
    }
    pthread_mutex_unlock(&gApnMutex);
    pthread_mutex_unlock(&gApnDBMutex);

    logD(RFX_LOG_TAG,"  generateApnDB APNDB Done this =%p slot_id = %d", this, getSlotId());

}
void RpApnController::GetApnDBBymccmnc()
{
    logD(RFX_LOG_TAG," GetApnDBBymccmnc this =%p ,slot_id = %d", this, getSlotId());
    apn_record_t* record = apn_malloc_record();
    apn_build_cRecord(record, APN_PROP_MCC, mcc.string());
    apn_build_iRecord(record, APN_PROP_iMNC, mnc.string());
    if(g_apnlist){
        apn_free_list(g_apnlist);
        g_apnlist = NULL;
    }
    apn_query_db(record, &g_apnlist);

    apn_record_t* instance = g_apnlist->top;
    while(instance != NULL) {
        apn_dump_record(instance);
        instance = (apn_record_t* )instance->next;
    }

    // free all the malloc memory , in case of memory leak
    apn_free_record(record);
    logD(RFX_LOG_TAG," GetApnDBBymccmnc done,slot_id = %d", getSlotId());
}

extern "C"
void apncallback(int ret, void* data) {
    switch(ret)
    {
        case APN_READY:
        {
            pthread_mutex_lock(&gApnMutex);
            pthread_cond_broadcast(&gApnDoneCond);
            property_set(APN_DB_REGENERATE, "false");
            gApnDoneCond_flag = APNDB_READY;
            pthread_mutex_unlock(&gApnMutex);
            break;
        }
        case APN_DESTROYED:
        {
            pthread_mutex_lock(&gApnMutex);
            gApnDoneCond_flag = APNDB_NOTREADY;
            pthread_mutex_unlock(&gApnMutex);
            break;
        }
        case APN_NOREADY:
        default:
            break;
    }
}

RpApnController::ApnInitThread::ApnInitThread (RpApnController* controller){
    this -> apnController = controller;
}

RpApnController::ApnInitThread::~ApnInitThread() {
}

bool RpApnController::ApnInitThread::threadLoop() {

    apnController->generateApnDB();
    apnController->GetApnDBBymccmnc();
    apnController->setDataProfilesAsNeeded();
    apnController->setInitialAttachApn();
    apnController->SyncDefaultDataSimToModem();

#if  defined(ANDROID_MULTI_SIM)
    apnController->SetDataAttached();
#endif
    //notify datacontroller
    apnController->apn_ready_singal.postEmit();
    pthread_mutex_lock(&gApnQMut);
    gApnQ_flag = APNQ_READY;
    pthread_cond_broadcast(&gApnQDnCond);
    pthread_mutex_unlock(&gApnQMut);
    return false;
}

void RpApnController::onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    int oldState, newState;

    oldState = old_value.asInt();
    newState = value.asInt();
    logD(RFX_LOG_TAG,"  onRadioStateChanged (%d, %d) (slot %d)", oldState, newState,
            getSlotId());
}

void RpApnController::onOperatorNumberChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    logD(RFX_LOG_TAG,"  onOperatorNumberChanged (%s, %s) (slot %d)", old_value.asString8().c_str(), value.asString8().c_str(),
        getSlotId());
    String8 mccmnc = value.asString8();
    if (!mccmnc.isEmpty()) {
        if(mccmnc.length() > 3)
        {
            mcc = String8(string(mccmnc.string()).substr(0, 3).c_str());
            logD(RFX_LOG_TAG,"  mccmnc %s length (%lld) ", mccmnc.string(), mccmnc.length());
            mnc = String8(string(mccmnc.string()).substr(3, mccmnc.length()-3).c_str());
        }
        else{
            logD(RFX_LOG_TAG,"  can not get mcc mnc value");
        }
        apnInitThread = new ApnInitThread(this);
        apnInitThread -> run("ApnInitThread");
    } else {
        sp<RfxMessage> rilRequest;
        int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
        logD(RFX_LOG_TAG,"  simState = %d", simState);
        if (simState == RFX_SIM_STATE_READY) {
            //requery the plmn

#ifdef TELE_FWK

#else
            rilRequest = RfxMessage::obtainRequest(getSlotId(),
                    RADIO_TECH_GROUP_GSM, RIL_LOCAL_REQUEST_GET_OPERATOR_NUMBER);

            RfxMainThread::enqueueMessage(rilRequest);
#endif
        }
    }
}

int RpApnController::getApnProp(apn_record_t* apn, int columnIdx, char ** value)
{
    for(int i = 0; i < apn->count; i++){
        if(columnIdx == apn->columnIdx[i])
        {
            *value = (char *)apn->values[i];
            return 0;
        }
    }
    *value = (char*)emptyStr.string();
    return -1;
}

int RpApnController::getApntypeBitmask(char *type)
{
    int len = sizeof(apntype_2_bitmask_id)/sizeof(ApnType_2_Bitmask_Id_T);
    for(int i = 0; i < len ; i++){
        if(strcasecmp(type,apntype_2_bitmask_id[i].type) == 0)
            return apntype_2_bitmask_id[i].typebitmask;
     }
    return DATAASST_PDN_APN_TYPE_UNKNOWN;
}

bool RpApnController::isApnContains(apn_record_t* apn, int columnIdx, const char *values)
{
    for(int i = 0; i < apn->count; i++){
        if(columnIdx == apn->columnIdx[i])
        {
            logD(RFX_LOG_TAG,"isApnContains %d, %s, %s",i,apn->values[i], values);
            if(strcmp(apn->values[i], values) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}
bool  RpApnController::apnCanHandleType(apn_record_t* apn , const char * type)
{
    int retApnTypes;
    char *apnTypes = NULL;
    retApnTypes = getApnProp(apn,APN_PROP_iTYPE,&apnTypes);
    logD(RFX_LOG_TAG,"apnCanHandleType apnTypes: %s",apnTypes);
    if((retApnTypes == 0 ) && (strstr(apnTypes,type) ||
        strstr(apnTypes,TEXT_APN_TYPE_ALL) ||
        strstr(apnTypes,TEXT_APN_TYPE_DEFAULT) ||
        strstr(apnTypes,TEXT_APN_TYPE_HIPRI))){
        return true;
    }
    return false;
}

int RpApnController::initDataProfile(apn_record_t* apn, RIL_DataProfileInfo *dp, int isRoaming)
{
    int ret = 0;
    int apnTypeId = 0;
    for(int i = 0; i < apn->count; i++)
    {
        logD(RFX_LOG_TAG," %s : %s \n",apn_get_prop_name(apn->columnIdx[i]),apn->values[i]);
        switch(apn->columnIdx[i])
        {
            case APN_PROP_iTYPE:
            {
                logD(RFX_LOG_TAG,"apntype %s\n",(apn->values[i]));
                dp->profileId = 0;
                char *start =(char *) (apn->values[i]);
                char *end = NULL;
                char type[12] = {0};
                while(end = strstr(start,",")){
                    memset(type,0,sizeof(type));
                    strncpy(type,start,end-start);
                    start = end + 1;
                    logD(RFX_LOG_TAG,"type %s\n",type);
                    dp->profileId  |= getApntypeBitmask(type);
                }
                memset(type,0,sizeof(type));
                strncpy(type,start,
                    (strlen(start) > (sizeof(type) - 1))? (sizeof(type) - 1):strlen(start));
                dp->profileId  |= getApntypeBitmask(type);

                logD(RFX_LOG_TAG,"type %s\n",type);

                if((strstr(apn->values[i], "default") != NULL) && (strstr(apn->values[i], "_default") == NULL)) {
                    apnTypeId = 1;
                    // mtk_property_set("vendor.ril.default.profile.mtu", instance->values[APN_PROP_iMTU]);
                } else if (strstr(apn->values[i], "rcs") != NULL) {
                    apnTypeId = 2;
                    // mtk_property_set("vendor.ril.rcs.profile.mtu", instance->values[APN_PROP_iMTU]);
                } else if (strstr(apn->values[i], "bip") != NULL) {
                    apnTypeId = 3;
                    // mtk_property_set("vendor.ril.bip.profile.mtu", instance->values[APN_PROP_iMTU]);
                }
                break;
            }
            case APN_PROP_iAPN:
            {
                dp->apn = (char *)apn->values[i];
                break;
            }
            case APN_PROP_iPROTOCOL:
            {
                if(!isRoaming){
                    dp->protocol = (char *)apn->values[i];
                }
                break;
            }
            case APN_PROP_iROAMING_PROTOCOL:
            {
                if(isRoaming){
                    dp->protocol = (char *)apn->values[i];
                }
                break;
            }
            case APN_PROP_iAUTH_TYPE:
            {
                dp->authType = atoi(apn->values[i]);
                break;
            }
            case APN_PROP_iUSER:
            {
                dp->user = (char *)apn->values[i];
                break;
            }
            case APN_PROP_iPASSWORD:
            {
                dp->password= (char *)apn->values[i];
                break;
            }
            case APN_PROP_iBEARER_BITMASK:
            {
                int bearerBitMask = atoi(apn->values[i]);
                dp->type = bearerBitMask;
                break;
            }
            case APN_PROP_iMAXCONNSTIME:
            {
                dp->maxConnsTime = atoi(apn->values[i]);
                break;
            }
            case APN_PROP_iMAXCONNS:
            {
                dp->maxConns = atoi(apn->values[i]);
                break;
            }
            case APN_PROP_iWAITTIME:
            {
                dp->waitTime = atoi(apn->values[i]);
                break;
            }
            case APN_PROP_iCARRIER_ENABLED:
            {
                dp->enabled = atoi(apn->values[i]);
                break;
            }
            case APN_PROP_iMTU:
            {
                switch (apnTypeId)
                {
                case 1:
                    mtk_property_set("vendor.ril.default.profile.mtu", apn->values[i]);
                    break;
                case 2:
                    mtk_property_set("vendor.ril.rcs.profile.mtu", apn->values[i]);
                    break;
                case 3:
                    mtk_property_set("vendor.ril.bip.profile.mtu", apn->values[i]);
                    break;
                default:
                    break;
                }
                break;
            }
            default:
                break;
        }
    }
    if(dp->user == NULL){
        dp->user = (char*)emptyStr.string();
    }
    if(dp->password == NULL){
        dp->password = (char*)emptyStr.string();
    }
    if(dp->apn == NULL){
        dp->apn = (char*)emptyStr.string();
    }
    if(dp->protocol == NULL){
        dp->protocol = (char*)emptyStr.string();
    }
    return ret;
}

void RpApnController::SyncDefaultDataSimToModem() {
#define SKIP_VALUE (-2)
#define TOTAL_PARAM (5)

    int data_sim = rfx_get_data_allowed_slotid();

    logD(RFX_LOG_TAG, "SyncDefaultDataSimToModem getSlotId()=%d", getSlotId());

    if(0!= getSlotId())
    {
        return;
    }
    logD(RFX_LOG_TAG, "SyncDefaultDataSimToModem RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD data_sim =%d", data_sim);
    sp<RfxMessage> newMsg = RfxMessage::obtainRequest(getSlotId(),RADIO_TECH_GROUP_GSM, RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD);
    Parcel* newParcel = newMsg->getParcel();
    size_t pos = newParcel->dataPosition();
    newParcel->writeInt32(TOTAL_PARAM);
    newParcel->writeInt32(SKIP_VALUE);
    newParcel->writeInt32(SKIP_VALUE);
    newParcel->writeInt32(data_sim);
    newParcel->writeInt32(SKIP_VALUE);
    newParcel->writeInt32(SKIP_VALUE);
    requestToRild(newMsg);
    return;
}

int RpApnController::setInitialAttachApn() {
    logD(RFX_LOG_TAG, "SetInitialAttachApn getSlotId()=%d", getSlotId());
    apn_record_t* iaApnRecord = NULL;
    char* apn =  NULL;
    char* protocol = NULL;
    char *authType = NULL;
    int iauthType = -1;
    char *username = NULL;
    char *password = NULL;
    char *cnull = "";
    int retApn,retAuthType,retProtocol,retUsrname,retPassword;
    if(g_apnlist != NULL && (g_apnlist->count > 0))
    {
        mfirstApnRecord = g_apnlist->top;
        apn_record_t* instance = g_apnlist->top;
        while(instance!= NULL)
        {
            if(isApnContains(instance,APN_PROP_iTYPE,APN_TYPE_IA)){
                iaApnRecord = instance;
                logD(RFX_LOG_TAG,"setInitialAttachApn iaApnRecord");
                break;
            }else if((mdefaultApnRecord == NULL) &&
                (apnCanHandleType(instance,APN_TYPE_DEFAULT))){
                mdefaultApnRecord = instance;
            }
            instance = (apn_record_t* )instance->next;
        }
    }

    apn_record_t *initialAttachApnRecord = NULL;
    if(iaApnRecord != NULL){
        initialAttachApnRecord = iaApnRecord;
    }else if(mPreferredApn != NULL){
        initialAttachApnRecord = mPreferredApn;
    }else if(mdefaultApnRecord != NULL){
        initialAttachApnRecord = mdefaultApnRecord;
    }else if(mfirstApnRecord != NULL){
        initialAttachApnRecord = mfirstApnRecord;
    }

    if(initialAttachApnRecord == NULL){
        return -1;
    }
    retApn = getApnProp(initialAttachApnRecord,APN_PROP_iAPN,&apn);
    retProtocol = getApnProp(initialAttachApnRecord,APN_PROP_iPROTOCOL,&protocol);
    retAuthType = getApnProp(initialAttachApnRecord,APN_PROP_iAUTH_TYPE,&authType);
    retUsrname = getApnProp(initialAttachApnRecord,APN_PROP_iUSER,&username);
    retPassword = getApnProp(initialAttachApnRecord,APN_PROP_iPASSWORD,&password);
    //prevent RFX_STATUS_KEY_GSM_OPERATOR_NUMERIC change many times. mdefaultApnRecord value become invalid.
    mdefaultApnRecord = NULL;
    if(retApn){
        apn = cnull;
    }
    if(retAuthType == 0){
        iauthType = atoi(authType);
    }
    if(retProtocol){
        protocol = "IP";
    }
    if(retUsrname){
        username = cnull;
    }
    if(retPassword){
        password = cnull;
    }
    sp<RfxMessage> newMsg = RfxMessage::obtainRequest(getSlotId(),RADIO_TECH_GROUP_GSM, RIL_REQUEST_SET_INITIAL_ATTACH_APN);
    Parcel* newParcel = newMsg->getParcel();
    size_t pos = newParcel->dataPosition();

    if(iauthType == -1){
        iauthType = (strcmp(username, "") == 0) ? 0 : 3;
    }
    char16_t *pString16;
    size_t len16 = 0;
    pString16 = strdup8to16(apn, &len16);
    newParcel->writeString16(pString16, len16);
    logD(RFX_LOG_TAG, "apn %s", apn);
    free(pString16);
    pString16 = NULL;
    pString16 = strdup8to16(protocol, &len16);
    newParcel->writeString16(pString16, len16);
    logD(RFX_LOG_TAG, "protocol %s", protocol);
    free(pString16);
    pString16 = NULL;
    newParcel->writeInt32(iauthType);
    pString16 = strdup8to16(username, &len16);
    newParcel->writeString16(pString16, len16);
    free(pString16);
    pString16 = strdup8to16(password, &len16);
    newParcel->writeString16(pString16, len16);
    free(pString16);
    newParcel->setDataPosition(pos);
    requestToRild(newMsg);
    return 0;
}

int RpApnController::setDataProfilesAsNeeded()
{
    mtk_property_set("vendor.ril.default.profile.mtu", "0");
    mtk_property_set("vendor.ril.rcs.profile.mtu", "0");
    mtk_property_set("vendor.ril.bip.profile.mtu", "0");
    if(g_apnlist != NULL && (g_apnlist->count > 0))
    {
        apn_record_t* instance = g_apnlist->top;
        RIL_DataProfileInfo *dps;
        mDataprofileid = 0;
        dps = (RIL_DataProfileInfo*)malloc(sizeof(RIL_DataProfileInfo) * (g_apnlist->count + 1));
        if(dps == NULL){
            return -1;
        }
        int dpsSize = 0;
        int ret = 0;
        while(instance!= NULL)
        {
            RIL_DataProfileInfo dp = {-1,NULL,NULL,-1,NULL,NULL,-1,0,0,0,0};
            ret = initDataProfile(instance,&dp,0/*getDataRoamingFromRegistration()*/);
            if(ret == 0){
                memcpy(&dps[dpsSize],&dp,sizeof(RIL_DataProfileInfo));
                mDataprofileid |= dps[dpsSize].profileId;
                dpsSize++;
                if(dpsSize > g_apnlist->count){
                    logD(RFX_LOG_TAG,"Error: setDataProfilesAsNeeded dpssize > apnlist count ");
                    break;
                }
            }
            instance = (apn_record_t* )instance->next;
        }

        apn_list_t* apnlist = NULL;
        if((mDataprofileid & DATAASST_PDN_APN_TYPE_EMERGENCY) == 0){
            apn_record_t* record = apn_malloc_record();
            apn_build_cRecord(record, APN_PROP_MCC, "");
            apn_build_cRecord(record, APN_PROP_MNC, "");
            apn_build_cRecord(record, APN_PROP_TYPE, "emergency");
            apn_query_db(record, &apnlist);
            if(apnlist && apnlist->top){
                apn_record_t* emerginstance = apnlist->top;
                RIL_DataProfileInfo dp = {-1,NULL,NULL,-1,NULL,NULL,-1,0,0,0,0};
                initDataProfile(emerginstance,&dp,0);
                memcpy(&dps[dpsSize],&dp,sizeof(RIL_DataProfileInfo));
                mDataprofileid |= DATAASST_PDN_APN_TYPE_EMERGENCY;
                dpsSize++;
            }
            apn_free_record(record);
        }
        logD(RFX_LOG_TAG,"mDataprofileid %x, %d",mDataprofileid,(mDataprofileid & DATAASST_PDN_APN_TYPE_IMS));
        if((mDataprofileid & DATAASST_PDN_APN_TYPE_IMS) != 0){
            logD(RFX_LOG_TAG,"Ims type exist in apn db\n");
        }else{
            logD(RFX_LOG_TAG,"Ims type not exist in apn db\n");
        }
        sp<RfxMessage> newMsg = RfxMessage::obtainRequest(getSlotId(),RADIO_TECH_GROUP_GSM, RIL_REQUEST_SET_DATA_PROFILE);
        Parcel* newParcel = newMsg->getParcel();
        size_t pos = newParcel->dataPosition();
        logD(RFX_LOG_TAG,"dpssize %d\n",dpsSize);
        newParcel->writeInt32(dpsSize);
        for(int i = 0; i < dpsSize; i++)
        {
            logD(RFX_LOG_TAG,"dps[%d]: profileId:%d, apn:%s, protocol:%s, authType:%d,"
                "user=%s, password=%s, type=%d,maxConnsTime=%d, maxConns=%d,"
                "waitTime=%d,enabled=%d ",
                i,dps[i].profileId,dps[i].apn,dps[i].protocol,dps[i].authType,
                dps[i].user,dps[i].password,dps[i].type,dps[i].maxConnsTime,dps[i].maxConns,
                dps[i].waitTime,dps[i].enabled);

            newParcel->writeInt32(dps[i].profileId);
            char16_t *pString16;
            size_t len16 = 0;
            pString16 = strdup8to16(dps[i].apn, &len16);
            newParcel->writeString16(pString16, len16);
            free(pString16);
            pString16 = strdup8to16(dps[i].protocol, &len16);
            newParcel->writeString16(pString16, len16);
            free(pString16);
            if(dps[i].authType == -1){
                dps[i].authType = (strcmp(dps[i].user, "") == 0) ? 0 : 3;
            }
            newParcel->writeInt32(dps[i].authType);
            pString16 = strdup8to16(dps[i].user, &len16);
            newParcel->writeString16(pString16, len16);
            free(pString16);
            pString16 = strdup8to16(dps[i].password, &len16);
            newParcel->writeString16(pString16, len16);
            free(pString16);
            newParcel->writeInt32(dps[i].type);
            newParcel->writeInt32(dps[i].maxConnsTime);
            newParcel->writeInt32(dps[i].maxConns);
            newParcel->writeInt32(dps[i].waitTime);
            newParcel->writeInt32(dps[i].enabled);
        }
        newParcel->setDataPosition(pos);
        requestToRild(newMsg);
        apn_free_list(apnlist);
        free(dps);
        return 0;
    }
    return -1;
}
int RpApnController::SetDataAttached()
{
#if  defined(ANDROID_MULTI_SIM)

    sp<RfxMessage> newMsg = RfxMessage::obtainRequest(getSlotId(),RADIO_TECH_GROUP_GSM, RIL_REQUEST_ALLOW_DATA);
    Parcel* newParcel = newMsg->getParcel();
    size_t pos = newParcel->dataPosition();
    newParcel->writeInt32(1); //num of ints

#if  defined(MODE_DSSS)//dsss
    if(getSlotId() == rfx_get_data_allowed_slotid())
    {
        newParcel->writeInt32(1); //default set the allow data enabled.
        logD(RFX_LOG_TAG,"SetDataAttached = 1");
    }
    else
    {
        newParcel->writeInt32(0); //default set the allow data enabled.
        logD(RFX_LOG_TAG,"SetDataAttached = 0");
    }
#else //dsds
    if(getSlotId() == rfx_get_data_allowed_slotid())
    {
        newParcel->writeInt32(1); //default set the allow data enabled.
        logD(RFX_LOG_TAG,"SetDataAttached = 1");
    }
    else
    {
        newParcel->writeInt32(0); //default set the allow data enabled.
        logD(RFX_LOG_TAG,"SetDataAttached = 0");
    }

#endif
    requestToRild(newMsg);

#endif
    return 0;
}

int RpApnController::parseApnRecord(ApnsettingT* apnInfo,apn_record_t* record)
{
    int ret = 0;
    for(int i = 0; i < record->count; i++)
    {
        logV(RFX_LOG_TAG," %s : %s \n",apn_get_prop_name(record->columnIdx[i]),record->values[i]);
        switch(record->columnIdx[i])
        {
            case APN_PROP_iCARRIER:
                apnInfo->carrier = record->values[i];
                break;
            case APN_PROP_iAPN:
                apnInfo->apn = record->values[i];
                break;
            case APN_PROP_iPROXY:
                apnInfo->proxy = record->values[i];
                break;
            case APN_PROP_iPORT:
                apnInfo->port = record->values[i];
                break;
            case APN_PROP_iMMSC:
                apnInfo->mmsc = record->values[i];
                break;
            case APN_PROP_iMMSPROXY:
                apnInfo->mms_proxy = record->values[i];
                break;
            case APN_PROP_iMMSPORT:
                apnInfo->mms_port = record->values[i];
                break;
            case APN_PROP_iUSER:
                apnInfo->user = record->values[i];
                break;
            case APN_PROP_iPASSWORD:
                apnInfo->password = record->values[i];
                break;
            case APN_PROP_iAUTH_TYPE:
                apnInfo->auth_type = atoi(record->values[i]);
                break;
            case APN_PROP_iTYPE:
                apnInfo->types = record->values[i];
                break;
            case APN_PROP_iId:
                apnInfo->id = atoi(record->values[i]);
                break;
            case APN_PROP_iPROTOCOL:
                apnInfo->protocol = record->values[i];
                break;
            case APN_PROP_iROAMING_PROTOCOL:
                apnInfo->roaming_protocol = record->values[i];
                break;
            case APN_PROP_iMTU:
                apnInfo->mtu = atoi(record->values[i]);
                break;
            case APN_PROP_iCARRIER_ENABLED:
                apnInfo->carrier_enabled = atoi(record->values[i]);
                break;
            case APN_PROP_iBEARER:
                apnInfo->bear = atoi(record->values[i]);
                break;
            case APN_PROP_iBEARER_BITMASK:
                apnInfo->bear_bitmask = atoi(record->values[i]);
                break;
            case APN_PROP_iPROFILE_ID:
                apnInfo->profile_id = atoi(record->values[i]);
                break;
            case APN_PROP_iMODEM_COGNITIVE:
                apnInfo->modem_cognitive = atoi(record->values[i]);
                break;
            case APN_PROP_iMAXCONNS:
                apnInfo->max_conns = atoi(record->values[i]);
                break;
            case APN_PROP_iWAITTIME:
                apnInfo->wait_time = atoi(record->values[i]);
                break;
            case APN_PROP_iMAXCONNSTIME:
                apnInfo->max_conns_time = atoi(record->values[i]);
                break;
            case APN_PROP_iMVNO_TYPE:
                apnInfo->mvno_type = record->values[i];
                break;
            default:
                break;
        }
    }
    return ret;
}

Vector<ApnsettingT*>* RpApnController::getApnInformation(string apntype){
    pthread_mutex_lock(&gApnUpdateMut);
    apn_list_t* apnlist = g_apnlist;
    if (apnlist == NULL) {
        logW(RFX_LOG_TAG, "getApnInformation ,list size is empty");
        return NULL;
    }

    char* apnTypes;
    Vector<ApnsettingT*>* list = new Vector<ApnsettingT*>();
    apn_record_t* instance = apnlist->top;
    int apntypeAll = (apntype.compare(APN_TYPE_ALL) == 0);

    for(int i = 0; i < apnlist->count; i++){
        getApnProp(instance,APN_PROP_iTYPE,&apnTypes);
        if((strstr(apnTypes,apntype.c_str())) || apntypeAll){
            ApnsettingT* apnInfo = new ApnsettingT();
            parseApnRecord(apnInfo,instance);
            //update profileid start
            apnInfo->profile_id= 0;
            char *start = apnTypes;
            char *end = NULL;
            char type[12] = {0};
            while(end = strstr(start,",")){
                memset(type,0,sizeof(type));
                strncpy(type,start,end-start);
                start = end + 1;
                apnInfo->profile_id|= getApntypeBitmask(type);
            }
            memset(type,0,sizeof(type));
            strncpy(type,start,
                (strlen(start) > (sizeof(type) - 1))? (sizeof(type) - 1):strlen(start));
            apnInfo->profile_id|= getApntypeBitmask(type);
            logD(RFX_LOG_TAG,"apn:[%s],apntype:[%s],profile id:[%x]\n",apnInfo->apn.c_str(),apnTypes,apnInfo->profile_id);
            //update profileid end
            list->add(apnInfo);
        }
        instance = (apn_record_t* )instance->next;
    }
    pthread_mutex_unlock(&gApnUpdateMut);
    return list;
}
void RpApnController::freeApnList(Vector<ApnsettingT*>* list) {
    if (list != NULL) {
        int size = list->size();
        for (int i = 0; i < size; i++) {
            delete list->itemAt(i);
        }
        delete list;
    }
}

// M: lwcg mode end
