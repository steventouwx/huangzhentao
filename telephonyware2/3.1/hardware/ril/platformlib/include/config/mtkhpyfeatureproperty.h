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
/* MediaTek Inc. (C) 2018. All rights reserved.
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

#ifndef MTKHPYFEATUREPROPERTY_H
#define MTKHPYFEATUREPROPERTY_H

//Black list for preventing misuse the api: rfx_property_get or mtk_property_get
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
"persist.vendor.radio.", //prefix
"vendor.gsm.", //prefix
"vendor.ril.", //prefix
"persist.vendor.ss.", //prefix
"persist.vendor.pco5.radio.ctrl",
"persist.vendor.mtk_usp_md_sbp_code",
"persist.vendor.mtk_usp_switch_mode",
"persist.vendor.operator.optr",
"persist.vendor.operator.seg",
"persist.vendor.operator.spec",
"persist.vendor.operator.subid",
"persist.vendor.ims_support",
"ro.vendor.md_mims_support",
"persist.vendor.gsm.sim.slot.lock.policy",
//"persist.vendor.ims.simulate" //no module set
//"vendor.ril.active.md"
//"persist.vendor.radio.ia"
//"vendor.ril.iccid.sim"
//"vendor.ril.uim.subscriberid"
//"persist.vendor.radio.external.sim"
//"persist.vendor.radio.vsim.timeout"
//"vendor.gsm.external.sim.enabled"
//"vendor.gsm.external.sim.inserted"
//"vendor.gsm.external.sim.timeout"
//"vendor.gsm.modem.vsim.capability"
//"vendor.gsm.prefered.aka.sim.slot"
//"vendor.gsm.prefered.rsim.slot"
#endif //MTK_HYP_DUAL_TELE_SUPPORT

#endif // MTKHPYFEATUREPROPERTY_H