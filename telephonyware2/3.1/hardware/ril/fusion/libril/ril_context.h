/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2023. All rights reserved.
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
#ifndef __RIL_CONTEXT__
#define __RIL_CONTEXT__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "telephony/ril.h"
#include "ril_internal.h"

/*****************************************************************************
 * Name space
 *****************************************************************************/
using android::ClientType;

/*****************************************************************************
 * Interface IRilContext
 *****************************************************************************/
class IRilContext {
public:
    virtual ~IRilContext() {}
    virtual int plusCounterRadioNumOne(unsigned int slotId) = 0;
    virtual int plusCounterMtkRadioNumOne(unsigned int slotId) = 0;
    virtual unsigned int setAospResponseNumberToOne(unsigned int index, int slotId) = 0;
    virtual unsigned int setMtkResponseNumberToOne(unsigned int index, int slotId) = 0;
    virtual void onNewCommandConnect(RIL_SOCKET_ID socket_id, ClientType clientType) = 0;
    virtual unsigned int checkIfSetAllAospResponseDone(int slotId) = 0;
    virtual unsigned int checkIfSetAllMtkResponseDone(int slotId) = 0;
};

/*****************************************************************************
 * class RilContext
 *****************************************************************************/
class RilContext : public IRilContext {
public:
    int plusCounterRadioNumOne(unsigned int slotId);
    int plusCounterMtkRadioNumOne(unsigned int slotId);
    unsigned int setAospResponseNumberToOne(unsigned int index, int slotId);
    unsigned int setMtkResponseNumberToOne(unsigned int index, int slotId);
    void onNewCommandConnect(RIL_SOCKET_ID socket_id, ClientType clientType);
    unsigned int checkIfSetAllAospResponseDone(int slotId);
    unsigned int checkIfSetAllMtkResponseDone(int slotId);
};

/*****************************************************************************
 * Global Variable Declarations
 *****************************************************************************/
extern IRilContext *rilContext;

#endif /* __RIL_CONTEXT__ */