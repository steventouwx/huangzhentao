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
#include "data/RtcDataAllowController.h"
#include "data/RtcDataController.h"

#include "RfxControllerFactory.h"
#include "RfxController.h"
#include "RfxRootController.h"
#include "RfxSlotRootController.h"
#include "ims/RtcImsController.h"
#include "ims/config/RtcImsConfigController.h"
#include "ims/RtcImsConferenceController.h"

#include "RtcWpController.h"
#include "nw/RtcRatSwitchController.h"
#include "nw/RtcNetworkController.h"
#include "nw/RtcNrSwitchController.h"
#include "cc/RtcCallController.h"
#include "capabilityswitch/RtcCapabilityGetController.h"
#include "capabilityswitch/RtcCapabilitySwitchChecker.h"
#include "capabilityswitch/RtcCapabilitySwitchController.h"
#include "sim/RtcCommSimController.h"
#include "sim/RtcCommSimRadioConfigController.h"
#include "ss/RtcSuppServController.h"
#include "oem/RtcOemController.h"
#include "phb/RtcPhbController.h"
#include "phb/RtcPhbSimIoController.h"
#include "phb/RtcPhbInModemController.h"
#include "cc/RtcEccNumberController.h"

// power-on
#include "power/RtcRadioController.h"
#include "power/RtcModemController.h"
#include "modecontroller/RtcModeSwitchController.h"
// STK
#include "cat/RtcCatController.h"
// M: MwiService
#include "mwi/RtcMobileWifiController.h"
#include "carrierconfig/RtcCarrierConfigController.h"
#include "client/RtcRilClientController.h"
#include "sms/RtcImsSmsController.h"
#include "sms/RtcSmsNSlotController.h"
#include "sms/RtcSCBMController.h"
// ATCI
#include "atci/RtcAtciController.h"
// VT
#include "vt/RtcVtController.h"
//RTT
#include "ims/RtcImsRttController.h"
// GWSD
#include "gwsd/RtcGwsdController.h"

// VSIM
#include "vsim/RtcVsimController.h"

//SBP
#include "sbp/RtcSbpController.h"
// Echolocate
#include "el/RtcElController.h"

//RCS
#include "rcs/RtcRcsController.h"

#ifdef TBOX_ECALL
//ecall
#include "cc/RtcEcallController.h"
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
//    RFX_CONTROLLER_CREATION_ENTRY(RtcNwController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcDataController),
//  RFX_CONTROLLER_CREATION_ENTRY(RfxHelloController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcRadioController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcRatSwitchController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcNetworkController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcCommSimController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcSuppServController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcEccNumberController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcCatController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcImsController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcImsConfigController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcOemController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcCallController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcImsConferenceController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcMobileWifiController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcCarrierConfigController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcImsSmsController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcSCBMController),
    // ATCI
    RFX_CONTROLLER_CREATION_ENTRY(RtcAtciController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcPhbController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcVtController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcPhbSimIoController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcPhbInModemController),
    // RTT
    RFX_CONTROLLER_CREATION_ENTRY(RtcImsRttController),
    // GWSD
    RFX_CONTROLLER_CREATION_ENTRY(RtcGwsdController),
    // VSIM
    RFX_CONTROLLER_CREATION_ENTRY(RtcVsimController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcRcsController),
    // Echolocate
    RFX_CONTROLLER_CREATION_ENTRY(RtcElController),
#ifdef TBOX_ECALL
    // ecall
    RFX_CONTROLLER_CREATION_ENTRY(RtcEcallController),
#endif
};

extern const int s_num_slot_controller = sizeof(s_slot_controllers) / sizeof(rfx_ctrler_factory_init);

extern const rfx_ctrler_factory_init s_non_slot_controllers[] = {
    RFX_CONTROLLER_CREATION_ENTRY(RtcWpController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcCapabilityGetController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcCapabilitySwitchChecker),
    RFX_CONTROLLER_CREATION_ENTRY(RtcCapabilitySwitchController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcDataAllowController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcModeSwitchController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcModemController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcRilClientController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcCommSimRadioConfigController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcSmsNSlotController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcNrSwitchController),
    RFX_CONTROLLER_CREATION_ENTRY(RtcSbpController),
};

extern const int s_num_non_slot_controller = sizeof(s_non_slot_controllers) / sizeof(rfx_ctrler_factory_init);
