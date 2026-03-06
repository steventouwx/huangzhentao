/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2017. All rights reserved.
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
 * File name:  rfxOpControllerFactory.cpp
 * Author: Weilun Liu(MTK02772)
 * Description:
 *  Implementation of OP controller factory.
 */

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxRootController.h"
#include "RfxSlotRootController.h"

#include "RfxRilAdapter.h"
#include "RfxLog.h"
#include "RfxMipcUtils.h"
#include "RtmDataController.h"
#include "RtmNwController.h"

#include "RfxControllerFactory.h"
#include "RfxController.h"
#include "RfxRootController.h"
#include "RfxSlotRootController.h"
#include "ims/RtmImsController.h"
#include "ims/config/RtmImsConfigController.h"
#include "ims/RtmImsConferenceController.h"

#include "RtmWpController.h"
#include "cc/RtmCallController.h"
#include "ecall/RtmEcallController.h"
#include "capabilityswitch/RtmCapabilityGetController.h"
#include "capabilityswitch/RtmCapabilitySwitchChecker.h"
#include "capabilityswitch/RtmCapabilitySwitchController.h"
#include "sim/RtmCommSimController.h"
#include "sim/RtmCommSimRadioConfigController.h"
#include "ss/RtmSuppServController.h"
#include "oem/RtmOemController.h"
#include "phb/RtmPhbController.h"
#include "phb/RtmPhbSimIoController.h"
#include "phb/RtmPhbInModemController.h"
#include "cc/RtmEccNumberController.h"

// power-on
#include "power/RtmRadioController.h"
#include "power/RtmModemController.h"
// STK
#include "cat/RtmCatController.h"
// M: MwiService
#include "mwi/RtmMobileWifiController.h"
#include "carrierconfig/RtmCarrierConfigController.h"
#include "client/RtcRilClientController.h"
#include "sms/RtmImsSmsController.h"
#include "sms/RtmSmsNSlotController.h"
#include "sms/RtmSCBMController.h"
// ATCI
#include "atci/RtmAtciController.h"
// VT
#include "vt/RtmVtController.h"
//RTT
#include "ims/RtmImsRttController.h"
// GWSD
#include "gwsd/RtmGwsdController.h"

// VSIM
#include "vsim/RtmVsimController.h"

//SBP
#include "sbp/RtmSbpController.h"

// Echolocate
#include "el/RtmElController.h"

//RCS
#include "rcs/RtmRcsController.h"

#ifdef TBOX
#include "TelematicsController.h"
#endif


/*****************************************************************************
 * Define
 *****************************************************************************/

#define RFX_CONTROLLER_CREATION_ENTRY(_className) \
        {_className::createInstance, _className::getClassInfoInstance, false}

/*****************************************************************************
 * Class RfxMipcUtils
 *****************************************************************************/

extern const rfx_ctrler_factory_init s_pre_non_slot_controllers[] = {
};

extern const int s_num_pre_non_slot_controller = sizeof(s_pre_non_slot_controllers) / sizeof(rfx_ctrler_factory_init);

extern const rfx_ctrler_factory_init s_slot_controllers[] = {
    RFX_CONTROLLER_CREATION_ENTRY(RtmNwController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmDataController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmRadioController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmCommSimController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmSuppServController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmEccNumberController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmCatController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmImsController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmImsConfigController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmOemController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmCallController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmEcallController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmImsConferenceController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmMobileWifiController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmCarrierConfigController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmImsSmsController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmSCBMController),
    // ATCI
    RFX_CONTROLLER_CREATION_ENTRY(RtmAtciController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmPhbController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmVtController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmPhbSimIoController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmPhbInModemController),
    // RTT
    RFX_CONTROLLER_CREATION_ENTRY(RtmImsRttController),
    // GWSD
    RFX_CONTROLLER_CREATION_ENTRY(RtmGwsdController),
    // VSIM
    RFX_CONTROLLER_CREATION_ENTRY(RtmVsimController),
    //RCS
    RFX_CONTROLLER_CREATION_ENTRY(RtmRcsController),
    // Echolocate
    RFX_CONTROLLER_CREATION_ENTRY(RtmElController),
#ifdef TBOX
    RFX_CONTROLLER_CREATION_ENTRY(TelematicsController),
#endif

};

extern const int s_num_slot_controller = sizeof(s_slot_controllers) / sizeof(rfx_ctrler_factory_init);

extern const rfx_ctrler_factory_init s_non_slot_controllers[] = {
    RFX_CONTROLLER_CREATION_ENTRY(RtmWpController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmCapabilityGetController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmCapabilitySwitchChecker),
    RFX_CONTROLLER_CREATION_ENTRY(RtmCapabilitySwitchController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmModemController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcRilClientController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmCommSimRadioConfigController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmSmsNSlotController),
    RFX_CONTROLLER_CREATION_ENTRY(RtmSbpController),
};
extern const int s_num_non_slot_controller = sizeof(s_non_slot_controllers) / sizeof(rfx_ctrler_factory_init);
