// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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

#include <cutils/sockets.h>
#include <mtk_log.h>
#include <assert.h>

#include "../include/telephony/ril.h"
#include "socket_util.h"

#define LOG_TAG "RilpSocketUtil"

#if defined(MODE_DSSS)
#include <prop/properties.h>

#define DISABLE_SIM_NUM "persist.vendor.radio.dsss.sim.disable"
#define getDisableSim(value) {\
                                                     char property_value[PROPERTY_VALUE_MAX] = {0};\
                                                     property_get(DISABLE_SIM_NUM, property_value, "2");\
                                                     value = atoi(property_value);\
                                                     RLOGD("[RilpSocketUtil]property:%s = %d", DISABLE_SIM_NUM, value);\
                                                 } while(0)

#endif

//#if defined(TELEPHONYWARE)
//#include <stdlib.h>
//#include <stdio.h>
//#include <fcntl.h>
//#include <sys/stat.h>
//#include <semaphore.h>
//#include <prop/properties.h>
//
//#define SOCKET_COUNT "persist.vendor.ril.client.num"    //to be confirmed
////#define SEM_NAME "/sem_socket1"
////#define SEM_NAME2 "/sem_socket2"
////#define SLOTCOUNT 2
//
//char socket_status[RIL_SOCKET_NUM][35]= {
//    "rild.socket.status1",
//#if (SIM_COUNT >= 2)
//    "rild.socket.status2",
//#endif
//};
//
//int RilpSocketUtil::selectSocket(RIL_SOCKET_ID socket_id)
//{
//    char property_count[PROPERTY_VALUE_MAX] = {0};
//    int count;
//    char property_status[PROPERTY_VALUE_MAX] = {0};
//    long long status;
//
//    property_get(SOCKET_COUNT, property_count, "0");
//    count = atoi(property_count);
//    RLOGD("[selectSocket]%s = %s, count = %d", SOCKET_COUNT, property_count, count);
//    property_get(socket_status[socket_id], property_status, "0");
//    sscanf(property_status, "%lld", &status);
//    RLOGD("[selectSocket]%s = %s, status = %lld", socket_status[socket_id], property_status, status);
//    if(count > (8*sizeof(long long) -1)) {
//        RLOGE("[selectSocket]socket count reach the upper limit, count = %d, limit = %d", count, (8*sizeof(long long) -1));
//    }
//    for(int i = 0; i < count; i++) {
//        if(((1<<i) & status) == 0) {
//            //status = status | (1<<i);
//            //sprintf(property_status, "%lld", status);
//            //property_set(socket_status[socket_id], property_status);
//            RLOGD("[selectSocket]selected client id = %d", i);
//            return i;
//        }
//    }
//    RLOGD("[selectSocket]no valid socket to use");
//    return -1;
//}
//#endif

char* RilpSocketUtil::getSocketName(RILD_RadioTechnology_Group group, RIL_SOCKET_ID socket_id) {
    char *socketName = "null";
//    #if defined(TELEPHONYWARE)
//    int socket_num = -1;
//    //sem_t *sem;
//    #endif
    #if  defined(MODE_DSSS)
    int disableSimNum=-1;
    #endif
    if(group == RADIO_TECH_GROUP_GSM) {
        switch(socket_id) {
            case RIL_SOCKET_1:
        #if  defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)  //DSDS
//            #if defined(TELEPHONYWARE)
//                //sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
//                //RLOGD("[Sherry]sem = %p", sem);
//                //if(SEM_FAILED == sem){
//                    //RLOGD("[ RilpSocketUtil]sem_open failed, errno = %d (%s)", errno, strerror(errno));
//                    //exit(1);
//                //}
//               // sem_wait(sem);
//                //RLOGD("[Sherry]enter sem");
//                socket_num = selectSocket(socket_id);
//                //sem_post(sem);
//               // RLOGD("[Sherry]exit sem");
//                if(-1 == socket_num){
//                    socketName = NULL;
//                } else {
//                    socketName = (char*)malloc(20);
//                    sprintf(socketName, "rild-client%d-0", socket_num);   //to be confirmed
//                }
//                //RLOGD("[getSocketName]socketName = %s", socketName);
//            #elif defined(TELE_FWK)
            #if defined(TELE_FWK)
                socketName = "rild" /*"rild-mtts1"*/;
            #endif
        #elif defined(MODE_DSSS)    //DSSS
                getDisableSim(disableSimNum);
//            #if defined(TELEPHONYWARE)
//                if (2 == disableSimNum) {   //enable slot1
//                   //sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
//                    //RLOGD("[Sherry]sem = %p", sem);
//                   // if(SEM_FAILED == sem){
//                       // RLOGD("[ RilpSocketUtil]sem_open failed, errno = %d (%s)", errno, strerror(errno));
//                       // exit(1);
//                   // }
//                    //sem_wait(sem);
//                    //RLOGD("[Sherry]enter sem");
//                    socket_num = selectSocket(socket_id);
//                    //sem_post(sem);
//                   // RLOGD("[Sherry]exit sem");
//                    if(-1 == socket_num){
//                        socketName = NULL;
//                    } else {
//                        socketName = (char*)malloc(20);
//                        sprintf(socketName, "rild-client%d", socket_num);   //to be confirmed
//                    }
//                } else {  //enable slot2
//                    socketName = "disable";
//                }
//            #elif defined(TELE_FWK)
            #if defined(TELE_FWK)
                if (2 == disableSimNum) {   //enable slot1
                    socketName = "rild" /*"rild-mtts1"*/;
                } else {
                    socketName = "disable";
                }
            #endif
         #else   //MT2635, only slot1 is enable
                 socketName = "mil";
         #endif

                break;
        #if (SIM_COUNT >= 2)
            case RIL_SOCKET_2:
          #if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)    //DSDS
//            #if defined(TELEPHONYWARE)
//                //sem = sem_open(SEM_NAME2, O_CREAT, 0644, 1);
//                //if(SEM_FAILED == sem){
//                    //RLOGD("[ RilpSocketUtil]sem_open failed, errno = %d (%s)", errno, strerror(errno));
//                    //exit(1);
//                //}
//                //sem_wait(sem);
//                socket_num = selectSocket(socket_id);
//                //sem_post(sem);
//                if(-1 == socket_num){
//                    socketName = NULL;
//                } else {
//                    socketName = (char*)malloc(20);
//                    sprintf(socketName, "rild-client%d-1", socket_num);   //to be confirmed
//                }
//            #elif defined(TELE_FWK)
            #if defined(TELE_FWK)
                socketName = "rild2";
            #endif
          #elif defined(MODE_DSSS)  //DSSS
                getDisableSim(disableSimNum);
//            #if defined(TELEPHONYWARE)
//                if (1 == disableSimNum) {   //enable slot2
//                  // sem = sem_open(SEM_NAME2, O_CREAT, 0644, 1);
//                    //RLOGD("[Sherry]sem = %p", sem);
//                    //if(SEM_FAILED == sem){
//                        //RLOGD("[ RilpSocketUtil]sem_open failed, errno = %d (%s)", errno, strerror(errno));
//                       // exit(1);
//                    //}
//                    //sem_wait(sem);
//                    //RLOGD("[Sherry]enter sem");
//                    socket_num = selectSocket(socket_id);
//                    //sem_post(sem);
//                   // RLOGD("[Sherry]exit sem");
//                    if(-1 == socket_num){
//                        socketName = NULL;
//                    } else {
//                        socketName = (char*)malloc(20);
//                        sprintf(socketName, "rild-client%d", socket_num);   //to be confirmed
//                    }
//                } else {  //enable slot1
//                    socketName = "disable";
//                }
//            #elif defined(TELE_FWK)
            #if defined(TELE_FWK)
                if (1 == disableSimNum) {   //enable slot2
                    socketName = "rild2";
                } else {   //enable slot1
                    socketName = "disable";
                }
            #endif
          #endif
                break;
        #endif
        #if (SIM_COUNT >= 3)
            case RIL_SOCKET_3:
                socketName = "mrild3";
                break;
        #endif
        #if (SIM_COUNT >= 4)
            case RIL_SOCKET_4:
                socketName = "mrild4";
                break;
        #endif
            default:
                RLOGE("Socket id is wrong!!");
                break;
        }
    }/** [MT2635] No need for C2K and ATCI
     else if (group == RADIO_TECH_GROUP_C2K) {
        socketName = "rild-via";
    } else if (group == RADIO_TECH_GROUP_ATCI) {
        socketName = "rilproxy-atci";
    }*/ else if (group == RADIO_TECH_GROUP_SAP_GSM) {
        switch (socket_id) {
            case RIL_SOCKET_1:
                socketName = "msap_uim_socket1";
                break;
        #if (SIM_COUNT >= 2)
            case RIL_SOCKET_2:
                socketName = "msap_uim_socket2";
                break;
        #endif
        #if (SIM_COUNT >= 3)
            case RIL_SOCKET_3:
                socketName = "msap_uim_socket3";
                break;
        #endif
        #if (SIM_COUNT >= 4)
            case RIL_SOCKET_4:
                socketName = "msap_uim_socket4";
                break;
        #endif
            default:
                RLOGE("Socket id is wrong!!");
                break;
        }
    }/** [MT2635] No need for C2K, ATCI and SAP-BT @{
     else if (group == RADIO_TECH_GROUP_SAP_C2K) {
        switch (socket_id) {
            case RIL_SOCKET_1:
                socketName = "msap_c2k_socket1";
                break;
        #if (SIM_COUNT >= 2)
            case RIL_SOCKET_2:
                socketName = "msap_c2k_socket2";
                break;
        #endif
        #if (SIM_COUNT >= 3)
            case RIL_SOCKET_3:
                socketName = "msap_c2k_socket3";
                break;
        #endif
        #if (SIM_COUNT >= 4)
            case RIL_SOCKET_4:
                socketName = "msap_c2k_socket4";
                break;
        #endif
            default:
                RLOGE("Socket id is wrong!!");
                break;
        }
    } else if (group == RADIO_TECH_GROUP_SAP_BT) {
        switch (socket_id) {
            case RIL_SOCKET_1:
                socketName = "sap_uim_socket1";
                break;
        #if (SIM_COUNT >= 2)
            case RIL_SOCKET_2:
                socketName = "sap_uim_socket2";
                break;
        #endif
        #if (SIM_COUNT >= 3)
            case RIL_SOCKET_3:
                socketName = "sap_uim_socket3";
                break;
        #endif
        #if (SIM_COUNT >= 4)
            case RIL_SOCKET_4:
                socketName = "sap_uim_socket4";
                break;
        #endif
            default:
                RLOGE("Socket id is wrong!!");
                break;
        }
    } @}**/ else {
        RLOGE("Socket group is wrong!!");
    }
    RLOGI ("getSocketName %s\n", socketName);
    return socketName;
}
