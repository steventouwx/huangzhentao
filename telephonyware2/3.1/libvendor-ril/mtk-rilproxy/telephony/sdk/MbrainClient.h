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

#ifndef MTK_RILPROXY_TELEPHONY_SDK_MBRAINCLIENT_H_
#define MTK_RILPROXY_TELEPHONY_SDK_MBRAINCLIENT_H_


#include <string>

typedef enum {
    Data_Call_Error_None = 0, // can send to rild.
    Data_Call_No_APN_DB_Support,
    Data_Call_No_APN_DB_Exist,
    Data_Call_DC_Invalid_Id,
    Data_Call_ClearUpConnection_No_Connection,
    Data_Call_No_Valid_APN_Setting,
    Data_Call_PsDisabled_OR_DataNotAllowed,
    Data_Call_Get_Null_Connection,
    Data_Call_Unkown_Error,
} Data_Call_Error_Point;

int notifyTeleServiceMainThreadTimeout(int slotId, int msgId, int msgType,
    long start_sec, long start_nsec, long end_sec, long end_nsec,  std::string extraInfo);

int notifyMbrainRadiosChanged(int slot, int radio);
int notifyMbrainCsAndEctChanged(int slot, int cs, int ect);
int notifyMbrainPsAndEctChanged(int slot, int ps, int ect);
int notifyMbrainCardStatusChanged(int slot, std::string simStatus);
int notifyMbrainMccMncStatusChanged(int slot, std::string mccmnc);
int notifyMbrainRfStatusStatusChanged(int slot, std::string rfStatus);
int notifyMbrainPdnStateChanged(int slot, int dataOnoff, int profileId, std::string ifname, std::string address, int state, int cid);
int notifyMbrainDataPoint(int profileId, int point);
int notifyMbrainDataResponse(int slot, int dataOnoff, int actType, int profileId,
    std::string ifname, std::string address, int state, int cid, int failcause, int error);
#endif /* MTK_RILPROXY_TELEPHONY_SDK_MBRAINCLIENT_H_ */
