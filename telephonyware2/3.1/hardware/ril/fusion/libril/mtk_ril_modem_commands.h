/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2022. All rights reserved.
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
{RIL_REQUEST_RESTART_RILD, mtkRadioExModem::restartRILDResponse},
{RIL_REQUEST_SEND_SAR_IND, mtkRadioExModem::sendSarIndicatorResponse},
{RIL_REQUEST_MODEM_POWERON, mtkRadioExModem::setModemPowerResponse},
{RIL_REQUEST_MODEM_POWEROFF, mtkRadioExModem::setModemPowerResponse},
{RIL_REQUEST_SET_TRM, mtkRadioExModem::setTrmResponse},
{RIL_REQUEST_SET_TX_POWER, mtkRadioExModem::setTxPowerResponse},
{RIL_REQUEST_SET_TX_POWER_STATUS, mtkRadioExModem::setTxPowerStatusResponse},
{RIL_REQUEST_SWITCH_MODE_FOR_ECC, mtkRadioExModem::triggerModeSwitchByEccResponse},
{RIL_REQUEST_SEND_WIFI_ENABLED, mtkRadioExModem::sendWifiEnabledResponse},
{RIL_REQUEST_SEND_WIFI_ASSOCIATED, mtkRadioExModem::sendWifiAssociatedResponse},
{RIL_REQUEST_SEND_WIFI_IP_ADDRESS, mtkRadioExModem::sendWifiIpAddressResponse},
{RIL_REQUEST_IWLAN_REGISTER_CELLULAR_QUALITY_REPORT, mtkRadioExModem::registerCellQltyReportResponse},
{RIL_REQUEST_MODIFY_MODEM_TYPE, mtkRadioExModem::modifyModemTypeResponse},
{RIL_REQUEST_GET_ECHOLOCATE_METRICS, mtkRadioExModem::getEngineeringModeInfoResponse},
{RIL_REQUEST_OEM_HOOK_ATCI_INTERNAL, mtkRadioExModem::sendAtciResponse},
{RIL_REQUEST_ROUTE_CERTIFICATE, mtkRadioExModem::routeCertificateResponse},
{RIL_REQUEST_ROUTE_AUTH, mtkRadioExModem::routeAuthMessageResponse},
{RIL_REQUEST_ENABLE_CAPABILITY, mtkRadioExModem::enableCapabilityResponse},
{RIL_REQUEST_ABORT_CERTIFICATE, mtkRadioExModem::abortCertificateResponse},
{RIL_REQUEST_RUN_GBA, mtkRadioExModem::runGbaAuthenticationResponse},
{RIL_REQUEST_SYNC_APP_EVENT_STATUS, mtkRadioExModem::syncAppEventStatusResponse},
