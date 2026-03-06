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
// MTK-START: SIM
{RIL_REQUEST_SIM_GET_ATR, mtkRadioExSim::getATRResponse},
{RIL_REQUEST_SIM_GET_ICCID, mtkRadioExSim::getIccidResponse},
{RIL_REQUEST_GET_SIM_TYPE_INFO, mtkRadioExSim::getSimTypeInfoResponse},
{RIL_REQUEST_SWITCH_SIM_TYPE, mtkRadioExSim::switchSimTypeResponse},
{RIL_REQUEST_SET_SIM_POWER, mtkRadioExSim::setSimPowerResponse},
{RIL_REQUEST_ACTIVATE_UICC_CARD, mtkRadioExSim::activateUiccCardRsp},
{RIL_REQUEST_DEACTIVATE_UICC_CARD, mtkRadioExSim::deactivateUiccCardRsp},
{RIL_REQUEST_GET_CURRENT_UICC_CARD_PROVISIONING_STATUS, mtkRadioExSim::getCurrentUiccCardProvisioningStatusRsp},
// MTK-END
// STK
{RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM_WITH_RESULT_CODE, mtkRadioExSim::handleStkCallSetupRequestFromSimWithResCodeResponse},
// PHB START
// MTK-START: SIM GBA
{RIL_REQUEST_GENERAL_SIM_AUTH, radioSim::iccIOForAppResponse},
// MTK-END
{RIL_REQUEST_QUERY_PHB_STORAGE_INFO, mtkRadioExSim::queryPhbStorageInfoResponse},
{RIL_REQUEST_WRITE_PHB_ENTRY, mtkRadioExSim::writePhbEntryResponse},
{RIL_REQUEST_READ_PHB_ENTRY, mtkRadioExSim::readPhbEntryResponse},
{RIL_REQUEST_QUERY_UPB_CAPABILITY, mtkRadioExSim::queryUPBCapabilityResponse},
{RIL_REQUEST_EDIT_UPB_ENTRY, mtkRadioExSim::editUPBEntryResponse},
{RIL_REQUEST_DELETE_UPB_ENTRY, mtkRadioExSim::deleteUPBEntryResponse},
{RIL_REQUEST_READ_UPB_GAS_LIST, mtkRadioExSim::readUPBGasListResponse},
{RIL_REQUEST_READ_UPB_GRP, mtkRadioExSim::readUPBGrpEntryResponse},
{RIL_REQUEST_WRITE_UPB_GRP, mtkRadioExSim::writeUPBGrpEntryResponse},
{RIL_REQUEST_GET_PHB_STRING_LENGTH, mtkRadioExSim::getPhoneBookStringsLengthResponse},
{RIL_REQUEST_GET_PHB_MEM_STORAGE, mtkRadioExSim::getPhoneBookMemStorageResponse},
{RIL_REQUEST_SET_PHB_MEM_STORAGE, mtkRadioExSim::setPhoneBookMemStorageResponse},
{RIL_REQUEST_READ_PHB_ENTRY_EXT, mtkRadioExSim::readPhoneBookEntryExtResponse},
{RIL_REQUEST_WRITE_PHB_ENTRY_EXT, mtkRadioExSim::writePhoneBookEntryExtResponse},
{RIL_REQUEST_QUERY_UPB_AVAILABLE, mtkRadioExSim::queryUPBAvailableResponse},
{RIL_REQUEST_READ_EMAIL_ENTRY, mtkRadioExSim::readUPBEmailEntryResponse},
{RIL_REQUEST_READ_SNE_ENTRY, mtkRadioExSim::readUPBSneEntryResponse},
{RIL_REQUEST_READ_ANR_ENTRY, mtkRadioExSim::readUPBAnrEntryResponse},
{RIL_REQUEST_READ_UPB_AAS_LIST, mtkRadioExSim::readUPBAasListResponse},
{RIL_REQUEST_SET_PHONEBOOK_READY, mtkRadioExSim::setPhonebookReadyResponse},
// PHB END
// MTK-START: SIM ME LOCK
{RIL_REQUEST_QUERY_SIM_NETWORK_LOCK, mtkRadioExSim::queryNetworkLockResponse},
{RIL_REQUEST_SET_SIM_NETWORK_LOCK, mtkRadioExSim::setNetworkLockResponse},
{RIL_REQUEST_ENTER_DEPERSONALIZATION, mtkRadioExSim::supplyDepersonalizationResponse},
// External SIM [START]
{RIL_REQUEST_VSIM_NOTIFICATION, mtkRadioExSim::vsimNotificationResponse},
{RIL_REQUEST_VSIM_OPERATION, mtkRadioExSim::vsimOperationResponse},
// External SIM [END]
/// M: SIM SLOT LOCK @{
{RIL_REQUEST_ENTER_DEVICE_NETWORK_DEPERSONALIZATION, mtkRadioExSim::supplyDeviceNetworkDepersonalizationResponse},
{RIL_REQUEST_SML_RSU_REQUEST, mtkRadioExSim::sendRsuRequestResponse},
///@}

