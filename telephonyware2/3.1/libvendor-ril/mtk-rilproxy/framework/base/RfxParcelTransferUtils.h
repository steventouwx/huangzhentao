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
  * RfxParcelTranferUtil.h
  *
  *  Created on: 2018/08/21
  *  Author: MTK06564
  */
#ifndef __RFX_PARCEL_TRANSFERUTIL_H__
#define __RFX_PARCEL_TRANSFERUTIL_H__

#include <binder/Parcel.h>
#include "RfxLog.h"
#include <cutils/jstring.h>
using ::android::Parcel;


//Note: next is example, please implement by request /unsol id
// request
Parcel* tranferRequestRaw(int slot, int requset, Parcel *p);
Parcel* tranferRequestOpenChannel(int slot, int requset, Parcel *p);
Parcel* tranferRequestSetDataProfile(int slot, int requset, Parcel *p);
Parcel* tranferRequestSetPreferredNetworkType(int slot, int requset, Parcel *p);
Parcel* tranferRequestRadioTech(int slot, int requset, Parcel *p);


//response
Parcel* tranferResponseRaw(int version, int response, Parcel *p, int* error);
Parcel* tranferSignalStrength(int version, int response, Parcel *p, int* error);
Parcel* tranferResponseDataCallList(int version, int response, Parcel *p, int* error);
Parcel* tranferResponseGetPreferredNetworkType(int version, int response, Parcel *p, int* error);
Parcel* tranferResponseRadioTech(int version, int response, Parcel *p, int* error);
Parcel* tranferResponseCellInfoList(int version, int response, Parcel *p, int* error);
Parcel* transferResponseSimStatus(int version, int response, Parcel *p, int* error);
Parcel* tranferResponseGetCurrentCalls(int version, int response, Parcel *p, int* error);
Parcel* tranferResponseVoiceRegisterState(int version, int response, Parcel *p, int* error);
Parcel* tranferResponseDataRegisterState(int version, int response, Parcel *p, int* error);



//unsol
Parcel* tranferUnsolRaw(int version, int unsol, Parcel *p);
Parcel* tranferUnsolRilConnected(int version, int unsol, Parcel *p);
Parcel* tranferUnsolCellInfoList(int version, int unsol, Parcel *p);
Parcel* tranferUnsolDataCallList(int version, int unsol, Parcel *p);
Parcel* transferUnsolSignalStrength(int version, int unsol, Parcel *p);
#endif

