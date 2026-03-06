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
{RIL_REQUEST_GET_COLP, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_COLP, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_COLR, tranferRequestRaw, tranferResponseRaw},
//{RIL_REQUEST_GET_CCM, tranferRequestRaw, tranferResponseRaw},
#if 0
{RIL_REQUEST_GET_ACM, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_ACMMAX, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_PPU_AND_CURRENCY, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_ACMMAX, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_RESET_ACM, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_PPU_AND_CURRENCY, tranferRequestRaw, tranferResponseRaw},
#endif
{RIL_REQUEST_MODEM_POWEROFF, tranferRequestRaw, tranferResponseRaw},
//{RIL_REQUEST_DUAL_SIM_MODE_SWITCH, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_QUERY_PHB_STORAGE_INFO, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_WRITE_PHB_ENTRY, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_READ_PHB_ENTRY, tranferRequestRaw, tranferResponseRaw},
#if 0
{RIL_REQUEST_SET_GPRS_CONNECT_TYPE, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_GPRS_TRANSFER_TYPE, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_MOBILEREVISION_AND_IMEI, tranferRequestRaw, tranferResponseRaw},
#endif
{RIL_REQUEST_QUERY_SIM_NETWORK_LOCK, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_SIM_NETWORK_LOCK, tranferRequestRaw, tranferResponseRaw},
#if 0
{RIL_REQUEST_SET_SCRI, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_BTSIM_CONNECT, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_BTSIM_DISCONNECT_OR_POWEROFF, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_BTSIM_POWERON_OR_RESETSIM, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_BTSIM_TRANSFERAPDU, tranferRequestRaw, tranferResponseRaw},
#endif
{RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SIM_GET_ICCID,tranferRequestRaw, tranferResponseRaw},
#if 0
{RIL_REQUEST_USIM_AUTHENTICATION, tranferRequestRaw, tranferResponseRaw},
#endif
{RIL_REQUEST_MODEM_POWERON,tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_SMS_SIM_MEM_STATUS, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_PHONE_CAPABILITY, tranferRequestRaw, tranferResponseRaw},
//{RIL_REQUEST_SET_PHONE_CAPABILITY, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_POL_CAPABILITY, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_POL_LIST, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_POL_ENTRY, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_QUERY_UPB_CAPABILITY, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_EDIT_UPB_ENTRY, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_DELETE_UPB_ENTRY, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_READ_UPB_GAS_LIST, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_READ_UPB_GRP, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_WRITE_UPB_GRP, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_TRM, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_PHB_STRING_LENGTH, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_PHB_MEM_STORAGE, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_PHB_MEM_STORAGE, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_READ_PHB_ENTRY_EXT, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_WRITE_PHB_ENTRY_EXT, tranferRequestRaw , tranferResponseRaw},
{RIL_REQUEST_GET_SMS_PARAMS, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_SMS_PARAMS, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SIM_GET_ATR, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_ETWS, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_FD_MODE, tranferRequestRaw, tranferResponseRaw},
/// M: [C2K][IR] Support SVLTE IR feature. @{
{RIL_REQUEST_RESUME_REGISTRATION, tranferRequestRaw, tranferResponseRaw},
/// M: [C2K][IR] Support SVLTE IR feature. @}

{RIL_REQUEST_GET_FEMTOCELL_LIST, tranferRequestRaw, tranferResponseRaw},
// Femtocell (CSG) : abort command shall be sent in differenent channel
{RIL_REQUEST_ABORT_FEMTOCELL_LIST, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SELECT_FEMTOCELL, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS , tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_HANGUP_ALL, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_FORCE_RELEASE_CALL, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_CALL_INDICATION, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_EMERGENCY_DIAL, tranferRequestRaw, tranferResponseRaw},
//{RIL_REQUEST_SET_ECC_SERVICE_CATEGORY, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_ECC_LIST, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GENERAL_SIM_AUTH, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_IMS_ENABLE, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_IMSCFG, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT , tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SEND_CNAP, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_CLIP, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_REMOVE_CB_MESSAGE, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_DIAL_WITH_SIP_URI, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_RESUME_CALL, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_REMOVE_RESTRICT_EUTRAN_MODE, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_VT_DIAL, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_VOICE_ACCEPT, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_REPLACE_VT_CALL, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_CONFERENCE_DIAL, tranferRequestRaw, tranferResponseRaw},
/// M: IMS feature. @{
{RIL_REQUEST_HOLD_CALL, tranferRequestRaw, tranferResponseRaw},
/// @}
{RIL_REQUEST_VIDEO_CALL_ACCEPT, tranferRequestRaw, tranferResponseRaw},
/// M: USSI RIL request commands @{
{RIL_REQUEST_SEND_USSI, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_CANCEL_USSI, tranferRequestRaw, tranferResponseRaw},
/// @}

/// M: set Ims capability to MD @{
{RIL_REQUEST_SET_VOLTE_ENABLE, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_WFC_ENABLE, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_SET_IMS_VIDEO_ENABLE, tranferRequestRaw, tranferResponseRaw},
/// @}
/// M: ViLTE. @{
{RIL_REQUEST_VT_DIAL_WITH_SIP_URI, tranferRequestRaw, tranferResponseRaw},
/// @}
/// M: IMS Deregistration request. @{
{RIL_REQUEST_IMS_DEREG_NOTIFICATION, tranferRequestRaw, tranferResponseRaw},
/// @}
{RIL_REQUEST_SWITCH_MODE_FOR_ECC, tranferRequestRaw, tranferResponseRaw},
// M: VzW
//{RIL_REQUEST_SYNC_APN_TABLE, tranferRequestRaw, tranferResponseRaw},
// M: VzW
{RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD, tranferRequestRaw, tranferResponseRaw},
//{RIL_REQUEST_START_KEEPALIVE_PRO, tranferRequestRaw, tranferResponseRaw},
//{RIL_REQUEST_STOP_KEEPALIVE_PRO, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_NITZ, tranferRequestRaw, tranferResponseRaw},
{RIL_REQUEST_GET_IMSCFG, tranferRequestRaw, tranferResponseRaw},