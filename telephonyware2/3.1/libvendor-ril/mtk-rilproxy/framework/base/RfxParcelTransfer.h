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
  * RfxParcelTranfer.h
  *
  *  Created on: 2018/08/21
  *  Author: MTK06564
  *
  */

#ifndef __RFX_PARCEL_TRANSFER_H__
#define __RFX_PARCEL_TRANSFER_H__

#include <binder/Parcel.h>
#include "RfxLog.h"
#include <cutils/jstring.h>

#ifdef HAVE_AEE_FEATURE
#include "aee.h"
#endif

using ::android::Parcel;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define NUM_ELEMS(x) (sizeof(x)/sizeof(x[0]))

typedef struct ReqParcelTransferInfo_s {
    int request;
    Parcel* (*ReqParcelFunction)  (int slot, int id, Parcel* p);
    Parcel* (*RespParcelFunction) (int version, int id, Parcel* p,  int* err);
} ReqParcelTransferInfo;

typedef struct UnsolParcelTransferInfo_s {
    int unsol;
    Parcel* (*UnsolParcelFunction) (int version,  int id, Parcel* p);
} UnsolParcelTransferInfo;

class RfxParcelTransfer {
private:
    RfxParcelTransfer();
    virtual ~RfxParcelTransfer();
    static int requsetIdTransfer(int requset);
    static int unsolIdTransfer(int unsol);
    static ReqParcelTransferInfo * getReqParcelTransferInfo(int request);

public:
    static Parcel* ReqParcelTransfer(int requset, Parcel* p, int slot);
    static Parcel* RespParcelTransfer(int response, Parcel* p, int*err);
    static Parcel* UnsolParcelTransfer(int *unsol, Parcel* p);
    static void checkTableIndex();
};
#endif
