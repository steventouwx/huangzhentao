/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2022. All rights reserved.
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
{RIL_REQUEST_GET_HARDWARE_CONFIG, radioModem::getHardwareConfigResponse},
{RIL_REQUEST_RADIO_POWER, radioModem::setRadioPowerResponse},
{RIL_REQUEST_ENABLE_MODEM, radioModem::enableModemResponse},
{RIL_REQUEST_BASEBAND_VERSION, radioModem::getBasebandVersionResponse},
{RIL_REQUEST_DEVICE_IDENTITY, radioModem::getDeviceIdentityResponse},
{RIL_REQUEST_GET_ACTIVITY_INFO, radioModem::getModemActivityInfoResponse},
{RIL_REQUEST_GET_RADIO_CAPABILITY, radioModem::getRadioCapabilityResponse},
{RIL_REQUEST_NV_READ_ITEM, radioModem::nvReadItemResponse},
{RIL_REQUEST_NV_RESET_CONFIG, radioModem::nvResetConfigResponse},
{RIL_REQUEST_NV_WRITE_CDMA_PRL, radioModem::nvWriteCdmaPrlResponse},
{RIL_REQUEST_NV_WRITE_ITEM, radioModem::nvWriteItemResponse},
{RIL_REQUEST_SHUTDOWN, radioModem::requestShutdownResponse},
{RIL_REQUEST_SEND_DEVICE_STATE, radioModem::sendDeviceStateResponse},
{RIL_REQUEST_SET_RADIO_CAPABILITY, radioModem::setRadioCapabilityResponse},
{RIL_REQUEST_DEVICE_IMEI, radioModem::getImeiResponse},
{RIL_REQUEST_OEM_HOOK_RAW, mtkRadioExModem::sendRequestRawResponse},
{RIL_REQUEST_OEM_HOOK_STRINGS, mtkRadioExModem::sendRequestStringsResponse},
