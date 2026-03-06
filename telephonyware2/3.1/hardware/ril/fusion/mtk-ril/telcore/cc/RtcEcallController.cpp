/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2020. All rights reserved.
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

#ifdef TBOX_ECALL

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <mtk_log.h>
#include <ratconfig.h>
#include <string.h>
#include <telephony/mtk_ril.h>
#include "RfxBasics.h"
#include "RfxDefs.h"
#include "RfxLog.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RfxEcallIndicationData.h"
#include "RfxEcallData.h"
#include "RfxEcallMsdData.h"
#include "RfxEcallSetNumData.h"
#include "RfxMainThread.h"
#include "RfxRilUtils.h"
#include "RfxStatusDefs.h"
#include "RtcEcallController.h"
#include "RtcEccNumberController.h"
#include <SpeechDrv.h>
#include <pthread.h>
extern "C"
void *ecallasst_report_handle(void *arg);

extern "C"
void* ecallasst_report_unsol_handle(void *arg);

extern "C"
int handle_upload_data(int16_t *data, size_t len);

/*****************************************************************************
 * Utility function
 *****************************************************************************/

/*****************************************************************************
 * Class RtcEcallController
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtcEcallController", RtcEcallController, RfxController);

// register request to RfxData
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_IVS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEcallSetNumData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_TEST_NUM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEcallSetNumData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEcallMsdData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_MSD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_PSAP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_MAKE_ECALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ECALL_IVS_PUSH_MSD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ECALL_PSAP_PULL_MSD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_MSD_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEcallData, RfxVoidData, RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_ECALL_CTRL_SEQUENCE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ECALL_RESET_IVS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxStringData, RFX_MSG_REQUEST_ECALL_QUERY_ECALL_NUM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_PRI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_ECALL_GET_SIM_INFO);

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_ECALL_MSDHACK);
RFX_REGISTER_DATA_TO_URC_ID(RfxEcallIndicationData, RFX_MSG_URC_ECALL_INDICATIONS);

RtcEcallController::RtcEcallController() :
        WaitFastEcallResp(0),
        isInFastEcall(0),
        isInNormalEcall(false),
        mFastEcallHangupCall(0),
        ivsMode(0),
        isMSDSetDone(0),
        Start_cnt(0),
        SendMSD_cnt(0),
        isMSDReady(0),
        mIsInImsEcall(false),
        mWaitImsEcallConnected(false),
        FastEcallType(-1),
        CtrlSeqFlag(0),
        HasResetTnum(0),
        mFastEcallMessage(NULL),
        mImsInd(NULL),
        mIsImsUpdateMsd(0),
        ecall_report_fd(0),
        ecall_report_unsol_fd(0),
        mECallTimerHandle(NULL),
        mEcallSimType(0),
        mWaitingDeregistrationNotification(false) {
    memset(&ctrl_sequence_tmp, 0, sizeof(ctrl_sequence_tmp));
    memset(msdData, 0, 140);
    memset(address, 0, 40);
}

RtcEcallController::~RtcEcallController() {
    logD(LOG_ECALL_TAG, "~RtcEcallController");
}

void RtcEcallController::onInit() {
    RfxController::onInit();
    logD(LOG_ECALL_TAG, "onInit !!");

    const int request_id_list[] = {
        RFX_MSG_REQUEST_ECALL_SET_IVS,
        RFX_MSG_REQUEST_ECALL_SET_TEST_NUM,
        RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM,
        RFX_MSG_REQUEST_ECALL_SET_MSD,
        RFX_MSG_REQUEST_ECALL_SET_PSAP,
        RFX_MSG_REQUEST_ECALL_MAKE_ECALL,
        RFX_MSG_REQUEST_ECALL_IVS_PUSH_MSD,
        RFX_MSG_REQUEST_ECALL_PSAP_PULL_MSD,
        RFX_MSG_REQUEST_ECALL_SET_MSD_MODE,
        RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL,
        RFX_MSG_REQUEST_ECALL_CTRL_SEQUENCE,
        RFX_MSG_REQUEST_ECALL_RESET_IVS,
        RFX_MSG_REQUEST_ECALL_QUERY_ECALL_NUM,
        RFX_MSG_REQUEST_ECALL_SET_PRI,
        RFX_MSG_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME,
        RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE,
        RFX_MSG_REQUEST_ECALL_GET_SIM_INFO,
    };

    const int urc_id_list[] = {
        RFX_MSG_URC_ECALL_MSDHACK,
        RFX_MSG_URC_ECALL_INDICATIONS,
    };

    char prop_value[RFX_PROPERTY_VALUE_MAX] = {0};
    int  dsss_disabledSim;

    isInFastEcall = 0;
    mFastEcallHangupCall = 0;
    FastEcallType = -1;
    HasResetTnum = 0;
    ivsMode = 0;
    isMSDSetDone = 0;
    CtrlSeqFlag = 0;
    WaitFastEcallResp = 0;
    isMSDReady = 0;
    Start_cnt = 0;
    SendMSD_cnt = 0;
    mIsImsUpdateMsd = 0;
    memset(msdData, 0, 140);
    memset(address, 0, 40);
    mFastEcallMessage = NULL;
    mImsInd = NULL;

    registerToHandleRequest(request_id_list, sizeof(request_id_list) / sizeof(int));

    registerToHandleUrc(urc_id_list, sizeof(urc_id_list) / sizeof(const int));

    getStatusManager()->registerStatusChangedEx(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
        RfxStatusChangeCallbackEx(this, &RtcEcallController::onCallCountChanged));

    getStatusManager()->registerStatusChangedEx(RFX_STATUS_KEY_VOICE_CALL_INDEX,
            RfxStatusChangeCallbackEx(this, &RtcEcallController::onCallIdChanged));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_LAST_CALL_FAIL_CAUSE,
            RfxStatusChangeCallback(this, &RtcEcallController::onCallDisconnected));

    // Only need to create threads once
    unsigned int simCount = (unsigned int)RfxRilUtils::rfxGetSimCount();
    rfx_property_get("persist.radio.dsss.sim.disable", prop_value, "2");
    dsss_disabledSim = atoi(prop_value);
    logD(LOG_ECALL_TAG, "simCount = %d, persist.radio.dsss.sim.disable = %d",
            simCount, dsss_disabledSim);
    // DSDS: Always create threads in RtcEcallController of slot 0
    // DSSS(Dual SIM Single Standby): Create threads in active SIM of RtcEcallController
    if (((simCount > 1) && (m_slot_id == 0)) ||
            ((simCount == 1) && (dsss_disabledSim != (m_slot_id + 1)))) {
        pthread_attr_t ecall_report_attr;
        pthread_attr_init(&ecall_report_attr);
        pthread_attr_setdetachstate(&ecall_report_attr, PTHREAD_CREATE_DETACHED);

        if (pthread_create(&ecall_report_fd,
                &ecall_report_attr, ecallasst_report_handle, this) != 0) {
            logE(LOG_ECALL_TAG, "create report handler error");
        }

        pthread_attr_destroy(&ecall_report_attr);

        pthread_attr_t ecall_report_unsol_attr;
        pthread_attr_init(&ecall_report_unsol_attr);
        pthread_attr_setdetachstate(&ecall_report_unsol_attr, PTHREAD_CREATE_DETACHED);

        if (pthread_create(&ecall_report_unsol_fd,
                &ecall_report_unsol_attr, ecallasst_report_unsol_handle, this) != 0) {
            logE(LOG_ECALL_TAG, "create report unsol handler error");
        }

        pthread_attr_destroy(&ecall_report_unsol_attr);
    }
}

extern "C"
void *ecallasst_report_handle(void *arg) {
    int size = 0;
    int16_t res[8192];
    int16_t seq_buf[8192 * 3];
    int len = 8192;
    int testseq_switch;
    int buf_len = 0;
    int seq_done = 0;

    // ensure the fifo diretory exists.

    struct stat st = {0};
    if (stat("/tmp/ecall", &st) == -1) {
        mkdir("/tmp/ecall", 0700);
    }

    int ready = mkfifo(ECALL_IPC_FOR_UPLINK, 0600);
    mtkLogD(LOG_ECALL_TAG, "mkfifo result is %d\n", ready);

    int ipc_recv_handler = open(ECALL_IPC_FOR_UPLINK, O_RDONLY);
    mtkLogD(LOG_ECALL_TAG, "open FIFO here, fd is %d!\n", ipc_recv_handler);

    while (1) {
        mtkLogD(LOG_ECALL_TAG, "read data from ipc\n");

        memset(res, 0, sizeof(res));

        size = read(ipc_recv_handler, res, sizeof(int16_t) * len);

        if (!size) {
            mtkLogD(LOG_ECALL_TAG, "ecall report handler reset fifo\n");

            if (seq_done) {
                seq_done = 0;
                testseq_switch = 0;
                speechdrv_set_ecall_testseq_switch(0);
                mtkLogD(LOG_ECALL_TAG, "disable ecall testseq switch result is %d\n", testseq_switch);
            }

            close(ipc_recv_handler);

            ipc_recv_handler = open(ECALL_IPC_FOR_UPLINK, O_RDONLY);

            mtkLogD(LOG_ECALL_TAG, "open FIFO here, fd is %d!\n", ipc_recv_handler);

            continue;
        } else if (size == -1) {
            mtkLogD(LOG_ECALL_TAG, "read error, read size is -1!\n");

            testseq_switch = 0;
            speechdrv_set_ecall_testseq_switch(0);

            mtkLogD(LOG_ECALL_TAG, "disable ecall testseq switch result is %d\n", testseq_switch);

            break;
        }

        mtkLogD(LOG_ECALL_TAG, "res[0] is %d\n", res[0]);

        for (int i = 0; i < size / sizeof(int16_t); i++) {
            mtkLogD(LOG_ECALL_TAG, "telephony_buff[%d] is %x!\n", i, res[i]);
        }

        if (res[0] == 0xDD || res[0] == 0xEE) {
            memcpy(seq_buf + buf_len, res + 1, size - sizeof(int16_t));
            buf_len += size - sizeof(int16_t);  // remove header
            mtkLogD(LOG_ECALL_TAG, "receive dd or ee\n");
            continue;
        } else if (res[0] == 0xFF) {
            memcpy(seq_buf + buf_len, res, size);
            buf_len += size - sizeof(int16_t);  // remove header
            seq_done = 1;
            mtkLogD(LOG_ECALL_TAG, "receive ff\n");
        }

        int number = buf_len / (sizeof(int16_t) * 3);
        mtkLogD(LOG_ECALL_TAG, "ipc read total size is %d, number is %d\n", buf_len, number);

        handle_upload_data(seq_buf, number);

        memset(seq_buf, 0, sizeof(seq_buf));
        buf_len = 0;
    }

    close(ipc_recv_handler);

    return NULL;
}

// data is a 3 column array, len is each column's length.
extern "C"
int handle_upload_data(int16_t *data, size_t len) {
    int cnt = 0;
    int index = 1;
    char file_name[128] = {0};
    char buf[128] = {0};

    if (data == NULL) {
        return 1;
    }

    // mkdir for the files to save.
    struct stat test = {0};

    if (stat("/home/root/ecall/", &test) == -1) {
        mtkLogD(LOG_ECALL_TAG, "directory is not exist\n");
        mkdir("/home/root/ecall", 0700);
    }

    char *file_path = "/home/root/ecall/";

    sprintf(file_name, "%s%s%d%s", file_path, "portivsrx", index, ".txt");

    while (access(file_name, F_OK) == 0) {
        mtkLogD(LOG_ECALL_TAG, "%s exist\n", file_name);
        memset(file_name, 0, 128);
        index++;
        sprintf(file_name, "%s%s%d%s", file_path, "portivsrx", index, ".txt");
    }

    FILE *file_ptr = fopen(file_name, "w");

    if (file_ptr == NULL) {
        mtkLogD(LOG_ECALL_TAG, "open file error\n");
        return 1;
    }

    mtkLogD(LOG_ECALL_TAG, "begin to write data, length is %d\n", len);

    while (cnt < len) {
        memset(buf, 0, sizeof(buf));

        if (data[cnt] == 0 || data[cnt] == 1 || data[cnt] == 2) {
            sprintf(buf, "%05d  %s%3d  %s%4d  %s%5d\n", cnt + 1, "port.state", data[cnt],
                    "port.rx.dlData", data[cnt + len], "port.rx.dlMetric", data[cnt + 2 * len]);
        } else if (data[cnt] == 3 || data[cnt] == 4) {
            sprintf(buf, "%05d  %s%3d  %s%4d  %s%6d\n", cnt + 1, "port.state", data[cnt],
                    "port.tx.dlData", data[cnt + len], "port.rx.dlIndex", data[cnt + 2 * len]);
        }

        cnt++;

        fputs(buf, file_ptr);
    }

    mtkLogD(LOG_ECALL_TAG, "finish write control sequence once!\n");

    fclose(file_ptr);

    return 0;
}

extern "C"
void* ecallasst_report_unsol_handle(void *arg) {
    int size = 0;
    int ready = -1, ipc_recv_handler = -1;
    uint32_t buf[2] = {0, 0};
    RtcEcallController* controller = (RtcEcallController*)arg;

    unsigned int simCount = (unsigned int)RfxRilUtils::rfxGetSimCount();

    struct stat st = {0};
    if (stat("/tmp/ecall", &st) == -1) {
        mkdir("/tmp/ecall", 0700);
    }

    if ((ready = mkfifo(ECALL_IPC_FOR_INDICATION, 0600)) != 0) {
        mtkLogD(LOG_ECALL_TAG, "mkfifo error number is %d\n", errno);
    }

    if ((ipc_recv_handler = open(ECALL_IPC_FOR_INDICATION, O_RDONLY)) == -1) {
        mtkLogD(LOG_ECALL_TAG, "open fifo error number is %d\n", errno);
    }

    while (1) {
        mtkLogD(LOG_ECALL_TAG, "Get eCall indication from AP speech\n");

        memset(buf, 0, sizeof(buf));
        size = read(ipc_recv_handler, buf, sizeof(buf));

        if (!size) {
            close(ipc_recv_handler);
            ipc_recv_handler = open(ECALL_IPC_FOR_INDICATION, O_RDONLY);
            continue;
        } else if (size == -1) {
            mtkLogD(LOG_ECALL_TAG, "read error, read size is -1!\n");
            break;
        }

        // Get correct controller to handle ECall indication
        if (simCount > 1) {
            controller = controller->getActiveController();
        }

        RIL_Ecall_Unsol_Indications* indication =
                (RIL_Ecall_Unsol_Indications*)calloc(1, sizeof(RIL_Ecall_Unsol_Indications));
        indication->call_id = controller->getFastEcallIndex();

        RIL_ECall_Indication type;

        mtkLogD(LOG_ECALL_TAG, "buf[0] is %x!\n", buf[0]);
        switch (buf[0]) {
            case 0xE1:
                type = RIL_UNSOL_ECALL_SENDING_MSD;
                controller->setSendMSDCount(controller->getSendMSDCount() + 1);
                break;

            case 0xE2:
                type = RIL_UNSOL_ECALL_LLACK_RECEIVED;
                break;

            case 0xE3:
                if (size != sizeof(uint32_t) * 2) {
                    mtkLogD(LOG_ECALL_TAG, "error, HLACK shall contain more message\n");
                    type = RIL_UNSOL_ECALL_UNSPECIFIED;
                }

                if (controller->getImsIndications() != NULL &&
                        controller->getImsIndications()->ind == RIL_UNSOL_ECALL_IMS_IN_BAND_TRANSFER) {
                    controller->handleImsInBandMsdSuccess();
                }

                mtkLogD(LOG_ECALL_TAG, "buf[1] is %x!\n", buf[1]);
                if (buf[1] & 0x2) {
                    type = RIL_UNSOL_ECALL_ALACK_CLEARDOWN_RECEIVED;
                } else {
                    type = RIL_UNSOL_ECALL_ALACK_POSITIVE_RECEIVED;
                }
                break;

            case 0xE4:
                type = RIL_UNSOL_ECALL_SENDING_START;
                controller->handleEcallStartMessage();
                break;

            default:
                type = RIL_UNSOL_ECALL_UNSPECIFIED;
                break;
        }

        mtkLogD(LOG_ECALL_TAG, "type is %d!\n", type);

        indication->ind = type;

        /* only the first start message shall be reported! */
        if (type == RIL_UNSOL_ECALL_SENDING_START && controller->getStartCount() != 1) {
            mtkLogD(LOG_ECALL_TAG, "start_cnt=%d, continue!\n", controller->getStartCount());
            free(indication);
            continue;
        }

        /* Only the first sending MSD message shall be reported */
        if (type == RIL_UNSOL_ECALL_SENDING_MSD && controller->getSendMSDCount() != 1) {
            mtkLogD(LOG_ECALL_TAG, "SendMSD_cnt is %d, ignore sending_msd_indication\n",
                    controller->getSendMSDCount());
            free(indication);
            continue;
        }

        controller->SendEcallIndication(indication, sizeof(RIL_Ecall_Unsol_Indications));
        free(indication);
    }
    return NULL;
}

void RtcEcallController::SendEcallIndication(RIL_Ecall_Unsol_Indications *resp_data,
        int resp_data_size) {
    logD(LOG_ECALL_TAG, "SendEcallIndication is called\n");
    sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(), RFX_MSG_URC_ECALL_INDICATIONS,
            RfxEcallIndicationData(resp_data, resp_data_size));
    responseToRilj(urc);
}

int RtcEcallController::handleEcallStartMessage() {
    logD(LOG_ECALL_TAG, "handleEcallStartMessage is called\n");
    sp<RfxMessage> responseMsg;
    int result;
    Start_cnt++;

    if (Start_cnt == 1 && isMSDReady) {
        logD(LOG_ECALL_TAG, "In pull mode, send EVENT_ECALL_FAST_ECALL_PULL_MSD\n");

        logD(LOG_ECALL_TAG, "EVENT_ECALL_FAST_ECALL_PULL_MSD\n");

        // Reconfig call, don't send msd
        if (FastEcallType == 1) {
            isMSDReady = 0;

            if (WaitFastEcallResp && mFastEcallMessage != NULL) {
                responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                        mFastEcallMessage, true);
                responseToRilj(responseMsg);
                WaitFastEcallResp = 0;
            }
        }

        if (isMSDSetDone == 0 && ivsMode == 1) {
            logD(LOG_ECALL_TAG, "enter pull msd procedure!!\n");

            result = 0;
            speechdrv_set_speech_on(1, 0, 0);

            result = 0;
            speechdrv_set_ecall_testseq_switch(0);

            result = 0;
            speechdrv_set_ecall_msd(msdData, sizeof(msdData));

            isMSDReady = 0;  // upon set msd, set isMSDReady  = 0;

            // To avoid voice(sound) polluting msd data, mute speech first
            speechdrv_set_dl_mute(true);
            speechdrv_set_ul_mute(true);

            result = 0;
            speechdrv_set_ivs_switch(1);

            if (WaitFastEcallResp && mFastEcallMessage != NULL) {
                if (result != 0) {
                    responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                            mFastEcallMessage, true);
                } else {
                    responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS,
                            mFastEcallMessage, true);
                }
                responseToRilj(responseMsg);
                WaitFastEcallResp = 0;
            }

            isMSDSetDone = 1;
         }
     }
    return result;
}

void RtcEcallController::onCallIdChanged(int slotId, RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    bool isImsEcall = getStatusManager()->getBoolValue(RFX_STATUS_KEY_IMS_CALL_EXIST, false);

    logD(LOG_ECALL_TAG, "onCallIdChanged isInFastEcall:%d isInNormalEcall:%d isImsEcall:%d\n",
            isInFastEcall, isInNormalEcall, isImsEcall);

    if (isInFastEcall == 1) {
        if (!isImsEcall) {
            handleFastEcall();
        } else {
            mIsInImsEcall = true;
            sendECallConnectedStatus(true);
            if (mWaitImsEcallConnected) {
                handleFastEcall();
            }
        }
    } else if (isInNormalEcall && !isImsEcall) {
        logD(LOG_ECALL_TAG, "Normal Ecall connected");
        sendECallConnectedStatus(true);
    }
}

void RtcEcallController::onCallCountChanged(int slotId, RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    int oldCount = old_value.asInt();
    mCallCount = value.asInt();
    logD(LOG_ECALL_TAG, "onCallCountChanged isInFastEcall:%d callCount:%d FastEcallHangupCall:%d\n",
          isInFastEcall, mCallCount, mFastEcallHangupCall);

    // When current call finished, execute pended mode switch.
    // TODO: if mFastEcallHangupCall == 1 && isInFastEcall == 0
    if (mCallCount == 0 && isInFastEcall == 1) {
        if (mFastEcallHangupCall == 1) {
            mFastEcallHangupCall = 0;
            dialFastEcall();
        } else {
            if (WaitFastEcallResp && (mFastEcallMessage != NULL)) {
                sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS,
                        mFastEcallMessage, true);
                responseToRilj(responseMsg);
                WaitFastEcallResp = 0;
            }
            handleEcallError();
        }
    }
}

void RtcEcallController::onCallDisconnected(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    if (getStatusManager()->getBoolValue(RFX_STATUS_KEY_ECALL_EXIST, false)
            && mFastEcallHangupCall != 1) {
        // To notify ECall disconnected status to upper layer
        sendECallConnectedStatus(false, value.asInt());

        if (RfxRilUtils::rfxGetSimCount() > 1) {
            requestToMcl(RfxMessage::obtainRequest(
                    getSlotId(), RFX_MSG_REQUEST_ECALL_GET_SIM_INFO, RfxVoidData()));
            startT9Timer();
        }

        mWaitingDeregistrationNotification = true;
    } else if (isInNormalEcall) {
        logD(LOG_ECALL_TAG, "Normal Ecall disconnected");
        sendECallConnectedStatus(false, value.asInt());
        isInNormalEcall = false;
    }
}

int RtcEcallController::getCallCount() {
    return mCallCount;
}

/*
  Used by thread, we need to get the correct RtcEcallController object to handle ECall flow.
*/
RtcEcallController* RtcEcallController::getActiveController() {
    unsigned int simCount = (unsigned int)RfxRilUtils::rfxGetSimCount();
    for (int i = 0; i < simCount; i++) {
        RtcEcallController *ctrl = (RtcEcallController *) findController(i,
                RFX_OBJ_CLASS_INFO(RtcEcallController));
        logD(LOG_ECALL_TAG, "simCount > 1, i = %d\n", i);
        if (ctrl->getCallCount() > 0) {
            logD(LOG_ECALL_TAG, "controller found, i = %d\n", i);
            return ctrl;
        }
    }

    return this;
}

int RtcEcallController::getFastEcallIndex() {
    return getStatusManager()->getIntValue(RFX_STATUS_KEY_VOICE_CALL_INDEX, -1);
}

int RtcEcallController::getSendMSDCount() {
    return SendMSD_cnt;
}

void RtcEcallController::setSendMSDCount(int count) {
    SendMSD_cnt = count;
}

RIL_Ecall_Unsol_Indications* RtcEcallController::getImsIndications() {
    return mImsInd;
}

int RtcEcallController::getStartCount() {
    return Start_cnt;
}

int RtcEcallController::getIvsMode() {
    return ivsMode;
}

void RtcEcallController::handleImsInBandMsdSuccess() {
    RIL_ECallReqMsg *fastEcall;
    if (mFastEcallMessage != NULL) {
        fastEcall = ((RIL_ECallReqMsg *)mFastEcallMessage->getData()->getData());
        fastEcall->length = 0;
        sp<RfxMessage> request = RfxMessage::obtainRequest(getSlotId(),
                RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL,
                RfxEcallData(fastEcall, sizeof(RIL_ECallReqMsg)));
        requestToMcl(request);
    } else {
        logE(LOG_ECALL_TAG, "handleImsInBandMsdSuccess error mFastEcallMessage is NULL");
    }
}

void RtcEcallController::handleFastEcall() {
    logD(LOG_ECALL_TAG, "handleFastEcall isInFastEcall:%d FastEcallType:%d ivsMode:%d\n",
            isInFastEcall, FastEcallType, ivsMode);
    sp<RfxMessage> responseMsg;
    if (FastEcallType == 1) {
        isMSDReady = 0;

        if (WaitFastEcallResp && mFastEcallMessage != NULL) {
            responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                    mFastEcallMessage, true);
            responseToRilj(responseMsg);
            WaitFastEcallResp = 0;
        }
    }

    // To notify ECall connected status to upper layer
    if (!mIsInImsEcall) {
        sendECallConnectedStatus(true);
    }

    if (isMSDSetDone == 0 && ivsMode == 2) {  // only push mode, switch ivs mode upon call active
        int result = 0;
        result = 0;
        speechdrv_set_speech_on(1, 0, 0);
        result = 0;
        speechdrv_set_ecall_testseq_switch(0);
        // To avoid voice(sound) polluting msd data, mute speech first
        speechdrv_set_dl_mute(true);
        speechdrv_set_ul_mute(true);
        result = 0;
        speechdrv_set_ivs_switch(1);
        size_t len = sizeof(msdData);
        result = 0;
        speechdrv_set_ecall_msd(msdData, len);
        isMSDReady = 0;  // upon set msd, set isMSDReady = 0;
        result = 0;
        speechdrv_set_ivs_send();
        isMSDSetDone = 1;
        if (WaitFastEcallResp && (mFastEcallMessage != NULL)) {
            if (result != 0) {
                responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                        mFastEcallMessage, true);
            } else {
                responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, mFastEcallMessage, true);
            }
            responseToRilj(responseMsg);
            WaitFastEcallResp = 0;
        }
    }
}

int16_t *RtcEcallController::tokstr(char *str, int16_t *data, int *n) {
    char *pre, *next;
    size_t len = strlen(str);
    pre = next = str;

    char tmp[20] = {0};
    int i = 0;
    char *end = &str[len];

    while (next < end) {
        while ((pre < end) && (*pre == ' ' || *pre == ',')) {
            pre++;
        }

        if (*pre == '\0') {
            break;
        }

        next = pre;

        while (next < end && *next != ' ' && *next != ',') {
            if ((('0' <= *next) && ('9' >= *next)) || *next == '-') {
                next++;
            } else {
                next++;
                return NULL;
            }
        }
        memset(tmp, 0, 20);
        memcpy(tmp, pre, next - pre);

        if (i < ECALL_MAX_SEQ_LEN) {
            data[i] = (int16_t)atoi(tmp);
            i++;
        } else {
            return NULL;
        }

        pre = next;
    }

    *n = i;

    return data;
}

void RtcEcallController::handleEcallError() {
    logD(LOG_ECALL_TAG, "enter ecall error handle function!! \n");

    getStatusManager()->setBoolValue(RFX_STATUS_KEY_ECALL_EXIST, false);
    isInFastEcall = 0;
    ivsMode = 0;
    isMSDSetDone = 0;
    WaitFastEcallResp = 0;
    Start_cnt = 0;
    SendMSD_cnt = 0;
    isMSDReady = 0;
    FastEcallType = -1;
    mIsImsUpdateMsd = 0;
    mWaitImsEcallConnected = false;
    mImsInd = NULL;

    memset(msdData, 0, 140);
    memset(address, 0, 40);

    if (HasResetTnum) {
        RIL_ECallSetNum* num = (RIL_ECallSetNum*)calloc(1, sizeof(RIL_ECallSetNum));
        num->arg_num = 1;
        num->type = 0;
        num->address = NULL;
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
                RFX_MSG_REQUEST_ECALL_SET_TEST_NUM,
                RfxEcallSetNumData(num, sizeof(RIL_ECallSetNum)));
        requestToMcl(msg);
        free(num);
    }
}

void RtcEcallController::onDeinit() {
    logD(LOG_ECALL_TAG, "onDeinit");

    //pthread_cancel(ecall_report_fd);

    //pthread_cancel(ecall_report_unsol_fd);

    RfxController::onDeinit();
}

bool RtcEcallController::onHandleRequest(const sp<RfxMessage>& message) {
    bool ret = false;
    int msgId = message->getId();
    if (ECALL_DBG) {
        logD(LOG_ECALL_TAG, "onHandleRequest: %s", RFX_ID_TO_STR(msgId));
    }

    switch (msgId) {
        case RFX_MSG_REQUEST_ECALL_SET_IVS:
            handleEcallSetIvsRequest(message);
            return true;
        case RFX_MSG_REQUEST_ECALL_SET_MSD:
            handleEcallSetMsdRequest(message);
            return true;
        case RFX_MSG_REQUEST_ECALL_SET_PSAP:
            handleEcallSetPsapRequest(message);
            return true;
        case RFX_MSG_REQUEST_ECALL_IVS_PUSH_MSD:
            handleEcallIvsPushMsdRequest(message);
            return true;
        case RFX_MSG_REQUEST_ECALL_PSAP_PULL_MSD:
            handleEcallPsapPullMsdRequest(message);
            return true;
        case RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL:
            handleFastMakeEcallRequest(message);
            return true;
        case RFX_MSG_REQUEST_ECALL_CTRL_SEQUENCE:
            handleEcallCtrlSequence(message);
            return true;
        case RFX_MSG_REQUEST_ECALL_RESET_IVS:
            handleEcallResetIvs(message);
            return true;
        default:
            break;
    }

    requestToMcl(message);
    return true;
}


bool RtcEcallController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    logD(LOG_ECALL_TAG, "onHandleUrc: %s", RFX_ID_TO_STR(msg_id));

    if (msg_id == RFX_MSG_URC_ECALL_INDICATIONS) {
        mImsInd = ((RIL_Ecall_Unsol_Indications *)message->getData()->getData());
        switch (mImsInd->ind) {
            case RIL_UNSOL_ECALL_IMS_UPDATE_MSD:
                mIsImsUpdateMsd = 1;
                break;
            case RIL_UNSOL_ECALL_IMS_IN_BAND_TRANSFER:
                if (!mIsInImsEcall) {
                    mWaitImsEcallConnected = true;
                } else {
                    handleFastEcall();
                }
                return true;
            case RIL_UNSOL_ECALL_IMS_MSD_ACK:
                memset(msdData, 0, 140);
                break;
            case RIL_UNSOL_ECALL_IMS_SRVCC:
                handleFastEcall();
                break;
            default:
                break;
        }

        RIL_Ecall_Unsol_Indications *ind
                = ((RIL_Ecall_Unsol_Indications *)message->getData()->getData());
        if (ind->ind == RIL_UNSOL_ECALL_ONLY_DEREGISTRATION) {
            if (!mWaitingDeregistrationNotification) {
                logD(LOG_ECALL_TAG, "ignore ECALL_ONLY_DEREGISTRATION, we're not waiting for it");
                return true;
            }
            mWaitingDeregistrationNotification = false;
            if (mEcallSimType != SIM_ECALL_ONLY) {
                logD(LOG_ECALL_TAG, "ignore ECALL_ONLY_DEREGISTRATION for non-eCall only SIM");
                return true;
            }
        }
    }

    responseToRilj(message);
    return true;
}

bool RtcEcallController::onHandleResponse(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    switch (msg_id) {
        case RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL:
            if (WaitFastEcallResp) {
                if (isInFastEcall && mFastEcallMessage != NULL) {
                    responseToRilj(RfxMessage::obtainResponse(message->getError(),
                            mFastEcallMessage, true));
                    WaitFastEcallResp = 0;
                }
            }
            if (message->getError() != RIL_E_SUCCESS) {
                /* clear ECall exist key when ECall failed */
                handleEcallError();
            }
            return true;
        case RFX_MSG_REQUEST_ECALL_MAKE_ECALL:
            if (message->getError() == RIL_E_SUCCESS) {
                logD(LOG_ECALL_TAG, "Normal Ecall start");
                isInNormalEcall = true;
            }
            break;
        case RFX_MSG_REQUEST_ECALL_SET_TEST_NUM:
            if (isInFastEcall) {
                if (message->getError() != RIL_E_SUCCESS) {
                    handleEcallError();
                } else {
                    if (HasResetTnum == 0) {
                        HasResetTnum = 1;
                        handleFastEcallHangupAll();
                    } else {
                        HasResetTnum = 0;
                    }
                }
                return true;
            }
            if (HasResetTnum == 1) {
                HasResetTnum = 0;
                return true;
            }
            break;
        case RFX_MSG_REQUEST_ECALL_GET_SIM_INFO:
            mEcallSimType = ((int *)message->getData()->getData())[0];
            logD(LOG_ECALL_TAG, "mEcallSimType = %d", mEcallSimType);
            break;
        default:
            break;
    }
    responseToRilj(message);
    return true;
}

bool RtcEcallController::onCheckIfRejectMessage(
        const sp<RfxMessage>& message, bool isModemPowerOff, int radioState) {
    int msgId = message->getId();
    if (radioState == (int)RADIO_STATE_OFF) {
        if (msgId == RFX_MSG_REQUEST_ECALL_SET_TEST_NUM ||
                msgId == RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM ||
                msgId == RFX_MSG_REQUEST_ECALL_SET_PRI) {
            return false;
        }
    }
    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

void RtcEcallController::handleEcallSetIvsRequest(const sp<RfxMessage>& message) {
    int ivsData = ((int *)message->getData()->getData())[0];

    if (ivsData == 1) {
        if (CtrlSeqFlag == 0) {
            speechdrv_set_ecall_testseq_switch(0);
        } else {
            CtrlSeqFlag = 0;
        }
    }
    // remove dummy speech interface
    int result = 0;
    speechdrv_set_ivs_switch(ivsData);
    sp<RfxMessage> responseMsg;
    if (result != 0) {
        responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
    } else {
        responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
    }
    responseToRilj(responseMsg);
}

void RtcEcallController::handleEcallSetMsdRequest(const sp<RfxMessage>& message) {
    logD(LOG_ECALL_TAG, "RtcEcallController handleEcallSetMsdRequest");

    RIL_ECallSetMSD *msdInfo = ((RIL_ECallSetMSD *)message->getData()->getData());

    if (mIsImsUpdateMsd == 1) {
        logD(LOG_ECALL_TAG, "RtcEcallController IMS update MSD");
        memset(msdData, 0, 140);
        memcpy(msdData, msdInfo->msd_data, msdInfo->length);
        sp<RfxMessage> request = RfxMessage::obtainRequest(getSlotId(),
                RFX_MSG_REQUEST_ECALL_SET_MSD, RfxEcallMsdData(msdInfo, sizeof(RIL_ECallSetMSD)));
        requestToMcl(request);
        mIsImsUpdateMsd = 0;
    } else {
        unsigned char * msdData = (unsigned char *)calloc(140, sizeof(unsigned char));
        RFX_ASSERT(msdData != NULL);
        memset(msdData, 0, 140);
        memcpy(msdData, msdInfo->msd_data, msdInfo->length);

        int result = 0;
        speechdrv_set_ecall_msd(msdData, 140);

        // To avoid voice(sound) polluting msd data, mute speech first
        speechdrv_set_dl_mute(true);
        speechdrv_set_ul_mute(true);

        /* in resend msd case, user resend msd, MTK auto switch IVS */
        int result2 = 0;
        speechdrv_set_ivs_switch(1);

        sp<RfxMessage> responseMsg;
        if (result != 0 || result2 != 0) {
            responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
        } else {
            responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
        }
        responseToRilj(responseMsg);

        if (msdData) {
            free(msdData);
            msdData = NULL;
        }
    }
}

void RtcEcallController::handleEcallSetPsapRequest(const sp<RfxMessage>& message) {
    int psapData = ((int *)message->getData()->getData())[0];
    int result = 0;
    speechdrv_set_psap_switch(psapData);

    sp<RfxMessage> responseMsg;
    if (result != 0) {
        responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
    } else {
        responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
    }
    responseToRilj(responseMsg);
}

void RtcEcallController::handleEcallIvsPushMsdRequest(const sp<RfxMessage>& message) {
    int result = 0;
    speechdrv_set_ivs_send();

    sp<RfxMessage> responseMsg;
    if (result != 0) {
        responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
    } else {
        responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
    }
    responseToRilj(responseMsg);
}

void RtcEcallController::handleEcallPsapPullMsdRequest(const sp<RfxMessage>& message) {
    int result = 0;
    speechdrv_set_psap_send();

    sp<RfxMessage> responseMsg;
    if (result != 0) {
        responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
    } else {
        responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
    }
    responseToRilj(responseMsg);
}

void RtcEcallController::handleFastMakeEcallRequest(const sp<RfxMessage>& message) {
    RIL_ECallReqMsg *fastEcall = ((RIL_ECallReqMsg *)message->getData()->getData());
    int result = 0;
    size_t len = 0;
    int ecall_cat = -1;
    int ecall_variant = -1;
    int ecall_type = -1;

    if (getStatusManager()->getBoolValue(RFX_STATUS_KEY_ECALL_EXIST, false)) {
        responseToRilj(RfxMessage::obtainResponse(RIL_E_REQUEST_RATE_LIMITED, message, true));
        return;
    }

    // return error if eCall on-going.
    if (fastEcall == NULL) {
        result = -1;
    } else {
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_ECALL_EXIST, true);

        ecall_cat = fastEcall->ecall_cat;
        ecall_variant = fastEcall->ecall_variant;
        memcpy(address, fastEcall->address, 40);
        WaitFastEcallResp = 1;
        // ecall_cat: 1-Manual Emergency eCall; 2-Automatic Emergency eCall
        // ecall_variant: 1-Test eCall; 2-Emergency eCall; 3-Reconfiguration eCall
        switch (ecall_variant) {
            case 1:
                ecall_type = 0; //Test eCall
                break;
            case 2:
                if (ecall_cat == 1) {
                    ecall_type = 2; //Manual Emergency eCall
                    break;
                } else if (ecall_cat == 2) {
                    ecall_type = 3; //Automatic Emergency eCall
                    break;
                } else {
                    break;
                }
            case 3:
                ecall_type = 1; //Reconfiguration eCall
                break;
            default:
                break;
        }

        if (ecall_type == -1) {
            result = -1;
        } else {
            FastEcallType = ecall_type;
            isInFastEcall = 1;
            memcpy(msdData, fastEcall->msd_data, fastEcall->length);
            isMSDReady = 1;
            ivsMode = ECALL_IVS_PUSH_MODE;
            mFastEcallMessage = message;

            /* check call address, query ecall num */
            if (*address != NULL && strcmp(address, "") != 0) {
                setTestNumber(address);
            } else {
                handleFastEcallHangupAll();
            }
        }
    }

    sp<RfxMessage> responseMsg;
    if (result != 0) {
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_ECALL_EXIST, false);
        responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
        responseToRilj(responseMsg);
    }
}

void RtcEcallController::handleFastEcallHangupAll() {
    if (getStatusManager()->getIntValue(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, 0) > 0) {
        mFastEcallHangupCall = 1;
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
                RFX_MSG_REQUEST_HANGUP_ALL, RfxVoidData());
        requestToMcl(msg);
    } else {
        dialFastEcall();
    }
    if (mECallTimerHandle != NULL) {
        RfxTimer::stop(mECallTimerHandle);
        mECallTimerHandle = NULL;
    }
}

void RtcEcallController::handleEcallCtrlSequence(const sp<RfxMessage>& message) {
    char *ctrlData = (char*)(message->getData()->getData());
    int result = -1;
    char tmp_value[20] = {0};
    int16_t  data[ECALL_MAX_SEQ_LEN];
    int num;

    memset(&data, 0, ECALL_MAX_SEQ_LEN * sizeof(int16_t));

    if (ctrlData == NULL) {
        result = -1;
    }

    strcpy(tmp_value, ((char**)ctrlData)[0]);

    int is_split = atoi(tmp_value);

    if (is_split == 1) {
        if (((char**)ctrlData)[1] == NULL || ((char**)ctrlData)[2] == NULL) {
            result = -1;
        }

        int seqlen = strlen(((char**)ctrlData)[2]);

        if (ctrl_sequence_tmp.index == 0) {  // init ctrlData and  save sequence ctrlData
            ctrl_sequence_tmp.data = (char *)malloc(ECALL_CTRLSEQ_LEN * sizeof(char));

            memset(ctrl_sequence_tmp.data, 0, ECALL_CTRLSEQ_LEN * sizeof(char));

            strcpy(ctrl_sequence_tmp.data, ((char**)ctrlData)[2]);

            ctrl_sequence_tmp.index++;
            ctrl_sequence_tmp.buff_size = ECALL_CTRLSEQ_LEN * sizeof(char);
        } else {
            if (ctrl_sequence_tmp.buff_size <= seqlen + strlen(ctrl_sequence_tmp.data)) {
                // realloc a larger memory
                char *tmp_ptr = (char*)realloc(ctrl_sequence_tmp.data, ctrl_sequence_tmp.buff_size
                        + ECALL_CTRLSEQ_LEN * sizeof(char));
                if (tmp_ptr == NULL) {
                    result = -1;
                } else {
                    ctrl_sequence_tmp.data = tmp_ptr;
                    ctrl_sequence_tmp.buff_size += ECALL_CTRLSEQ_LEN * sizeof(char);
                }
            }

            strcpy(ctrl_sequence_tmp.data + strlen(ctrl_sequence_tmp.data), ((char**)ctrlData)[2]);
            ctrl_sequence_tmp.index++;
        }

        memset(tmp_value, 0, sizeof(tmp_value));
        strcpy(tmp_value, ((char**)ctrlData)[1]);
        int is_ready = atoi(((char**)ctrlData)[1]);

        if (is_ready != 1) {
            result = -1;
        }

        if (tokstr(ctrl_sequence_tmp.data, data, &num) == NULL) {
            if (ctrl_sequence_tmp.data) {
                free(ctrl_sequence_tmp.data);
                ctrl_sequence_tmp.data = NULL;
            }
            ctrl_sequence_tmp.index = 0;
            ctrl_sequence_tmp.buff_size = 0;

            result = -1;
        } else {
            if (ctrl_sequence_tmp.data) {
                free(ctrl_sequence_tmp.data);
                ctrl_sequence_tmp.data = NULL;
            }
            ctrl_sequence_tmp.index = 0;
            ctrl_sequence_tmp.buff_size = 0;
        }
    } else {  // not split case
        if (tokstr(((char**)ctrlData)[2], data, &num) == NULL) {
            result = -1;
        }
    }

    int16_t *ctrlseq = (int16_t *)malloc(sizeof(int16_t) * num);

    memset(ctrlseq, 0, sizeof(int16_t) * num);

    memcpy(ctrlseq, data, sizeof(int16_t) * num);

    int ctrl_seq_switch = 0;
    speechdrv_set_ecall_testseq_switch(1);

    if (ctrl_seq_switch == 0) {
        CtrlSeqFlag = 1;
    }

    result = 0;
    speechdrv_set_ecall_testseq((void *)ctrlseq, num / 3);

    if (ctrlseq) {
        free(ctrlseq);
        ctrlseq = NULL;
    }

    sp<RfxMessage> responseMsg;
    if (result != 0) {
        responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
    } else {
        responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
    }
    responseToRilj(responseMsg);
}

void RtcEcallController::handleEcallResetIvs(const sp<RfxMessage>& message) {
    int result = -1;
    logD(LOG_ECALL_TAG, "handleEcallResetIvs WaitFastEcallResp=%d", WaitFastEcallResp);

    if (WaitFastEcallResp) {
        if (isInFastEcall && mFastEcallMessage != NULL) {
            sp<RfxMessage> responseFastEcall = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                    mFastEcallMessage, true);
            responseToRilj(responseFastEcall);
            WaitFastEcallResp = 0;
        }
    }

    result = resetIvs();

    sp<RfxMessage> responseMsg;
    if (result != 0) {
        responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
    } else {
        responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
    }
    responseToRilj(responseMsg);
}

// To notify ECall connected status to upper layer.
void RtcEcallController::sendECallConnectedStatus(bool isConnected, int cause) {
    RIL_Ecall_Unsol_Indications* indication =
        (RIL_Ecall_Unsol_Indications*)calloc(1, sizeof(RIL_Ecall_Unsol_Indications));
    indication->call_id = getFastEcallIndex();
    if (mIsInImsEcall) {
        indication->ind = isConnected ?
                RIL_UNSOL_ECALL_IMS_ACTIVE : RIL_UNSOL_ECALL_IMS_DISCONNECTED;
        if (!isConnected) {
            mIsInImsEcall = false;
            memset(msdData, 0, 140);
        }
    } else {
        if (isConnected) {
            indication->ind = RIL_UNSOL_ECALL_CONNECTED;
        } else if (getStatusManager()->getBoolValue(RFX_STATUS_KEY_IMS_CALL_DISCONNECTED, false)) {
            indication->ind = RIL_UNSOL_ECALL_IMS_DISCONNECTED;
        } else {
            logD(LOG_ECALL_TAG, "sendECallConnectedStatus, cause = %d", cause);
            if (cause == CALL_FAIL_NORMAL || cause == CALL_FAIL_NORMAL_UNSPECIFIED) {
                // [MOLY01026808] Customer requirement: reset IVS if call disconnected
                // with normal cause
                resetIvs();
                indication->ind = RIL_UNSOL_ECALL_DISCONNECTED;
            } else {
                indication->ind = RIL_UNSOL_ECALL_ABNORMAL_HANGUP;
            }
        }
    }
    SendEcallIndication(indication, sizeof(RIL_Ecall_Unsol_Indications));
    free(indication);

    if (!isInNormalEcall) {  // this status key is only designed for fast Ecall
        /* Normal MO and MT calls should be rejected when ECall exist */
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_ECALL_EXIST, isConnected);
    }
}

void RtcEcallController::sendECallConnectedStatus(bool isConnected) {
    sendECallConnectedStatus(isConnected, 0);
}

void RtcEcallController::setTestNumber(char* number) {
    logD(LOG_ECALL_TAG, "user has set eCall address(%s), query the number first!", number);

    RtcEccNumberController* eccNumberController = (RtcEccNumberController *)findController(
            getSlotId(), RFX_OBJ_CLASS_INFO(RtcEccNumberController));
    bool isValidNumber = true;
    if (strspn(number, "+*#0123456789") != strlen(number)) {
        logD(LOG_ECALL_TAG, "user number is invalid!");
        isValidNumber = false;
    } else if (FastEcallType != 0) {
        logD(LOG_ECALL_TAG, "number shall not be set for non-test eCall!");
        isValidNumber = false;
    } else if (eccNumberController->isEmergencyNumber(String8::format("%s", number))) {
        logD(LOG_ECALL_TAG, "ECC number shall not be used in test ecall!");
        isValidNumber = false;
    }

    if (isValidNumber) {
        logD(LOG_ECALL_TAG, "address(%s) is not ECC, config test number before make ecall!!\n",
                number);

        RIL_ECallSetNum* num = (RIL_ECallSetNum*)calloc(1, sizeof(RIL_ECallSetNum));
        num->arg_num = 2;
        num->type = 0;
        num->address = number;
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
                RFX_MSG_REQUEST_ECALL_SET_TEST_NUM,
                RfxEcallSetNumData(num, sizeof(RIL_ECallSetNum)));
        requestToMcl(msg);
        free(num);
    } else {
        if (WaitFastEcallResp) {
            sp<RfxMessage> responseMsg;
            if (isInFastEcall && mFastEcallMessage != NULL) {
                responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                        mFastEcallMessage, true);
                responseToRilj(responseMsg);
                WaitFastEcallResp = 0;
            }
            handleEcallError();
        }
    }
}

void RtcEcallController::startT9Timer() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("vendor.ecall.timer9.sec", prop, ECALL_DEFAULT_T9_TIMER);
    int value = atoi(prop);

    logD(LOG_ECALL_TAG, "startT9Timer, value = %d", value);

    if (mECallTimerHandle != NULL) {
        RfxTimer::stop(mECallTimerHandle);
    }
    mECallTimerHandle = RfxTimer::start(RfxCallback0(this,
           &RtcEcallController::onTimeOut), s2ns(value));
}

void RtcEcallController::onTimeOut() {
    logD(LOG_ECALL_TAG, "onTimeOut");
    mECallTimerHandle = NULL;
    if (mEcallSimType != SIM_ECALL_ONLY) {
        RIL_Ecall_Unsol_Indications* indication =
               (RIL_Ecall_Unsol_Indications*)calloc(1, sizeof(RIL_Ecall_Unsol_Indications));
        indication->ind = RIL_UNSOL_ECALL_MAY_DEREGISTER;
        SendEcallIndication(indication, sizeof(RIL_Ecall_Unsol_Indications));
    }
}

void RtcEcallController::dialFastEcall() {
    RtcCallController *imsCallCtrl = (RtcCallController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtcCallController));
    RIL_ECallReqMsg *fastEcall = ((RIL_ECallReqMsg *)mFastEcallMessage->getData()->getData());
    sp<RfxMessage> request = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL,
            RfxEcallData(fastEcall, sizeof(RIL_ECallReqMsg)));
    char imsEcallSupport[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.ims_ecall_support", imsEcallSupport, "0");
    logD(LOG_ECALL_TAG, "dialFastEcall imsEcallSupport: %s", imsEcallSupport);
    if ((atoi(imsEcallSupport) == 1)) {
        imsCallCtrl->handleImsDialRequest(request);
    }
    requestToMcl(request);
}

int RtcEcallController::resetIvs() {
    logD(LOG_ECALL_TAG, "resetIvs");
    if (HasResetTnum) {
        RIL_ECallSetNum* num = (RIL_ECallSetNum*)calloc(1, sizeof(RIL_ECallSetNum));
        num->arg_num = 1;
        num->type = 0;
        num->address = NULL;
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
                RFX_MSG_REQUEST_ECALL_SET_TEST_NUM,
                RfxEcallSetNumData(num, sizeof(RIL_ECallSetNum)));
        requestToMcl(msg);
        free(num);
    }

    isInFastEcall = 0;
    ivsMode = 0;
    isMSDSetDone = 0;
    isMSDReady = 0;
    Start_cnt = 0;
    SendMSD_cnt = 0;
    memset(msdData, 0, 140);
    memset(address, 0, 40);

    int result = 0;
    speechdrv_set_ivs_switch(0);
    logD(LOG_ECALL_TAG, "speechdrv_set_ivs_switch return: %d", result);

    // To unmute speech since no msd data to sent
    speechdrv_set_dl_mute(false);
    speechdrv_set_ul_mute(false);
    return result;
}

#endif
