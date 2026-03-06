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
{RIL_REQUEST_GET_SIM_STATUS, radioSim::getIccCardStatusResponse},
{RIL_REQUEST_ENTER_SIM_PIN, radioSim::supplyIccPinForAppResponse},
{RIL_REQUEST_ENTER_SIM_PUK, radioSim::supplyIccPukForAppResponse},
{RIL_REQUEST_ENTER_SIM_PIN2, radioSim::supplyIccPin2ForAppResponse},
{RIL_REQUEST_ENTER_SIM_PUK2, radioSim::supplyIccPuk2ForAppResponse},
{RIL_REQUEST_CHANGE_SIM_PIN, radioSim::changeIccPinForAppResponse},
{RIL_REQUEST_CHANGE_SIM_PIN2, radioSim::changeIccPin2ForAppResponse},
{RIL_REQUEST_GET_IMSI, radioSim::getIMSIForAppResponse},
{RIL_REQUEST_SIM_IO, radioSim::iccIOForAppResponse},
{RIL_REQUEST_QUERY_FACILITY_LOCK, radioSim::getFacilityLockForAppResponse},
{RIL_REQUEST_SET_FACILITY_LOCK, radioSim::setFacilityLockForAppResponse},
{RIL_REQUEST_STK_GET_PROFILE, NULL},
{RIL_REQUEST_STK_SET_PROFILE, NULL},
{RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND, radioSim::sendEnvelopeResponse},
{RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE, radioSim::sendTerminalResponseToSimResponse},
{RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING, radioSim::reportStkServiceIsRunningResponse},
{RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE, radioSim::getCdmaSubscriptionSourceResponse},
{RIL_REQUEST_ISIM_AUTHENTICATION, radioSim::requestIsimAuthenticationResponse},
{RIL_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS, radioSim::sendEnvelopeWithStatusResponse},
{RIL_REQUEST_SIM_TRANSMIT_APDU_BASIC, radioSim::iccTransmitApduBasicChannelResponse},
{RIL_REQUEST_SIM_OPEN_CHANNEL, radioSim::iccOpenLogicalChannelResponse},
{RIL_REQUEST_SIM_CLOSE_CHANNEL, radioSim::iccCloseLogicalChannelResponse},
{RIL_REQUEST_SIM_CLOSE_CHANNEL_WITH_SESSION, radioSim::iccCloseLogicalChannelWithSessionInfoResponse},
{RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL, radioSim::iccTransmitApduLogicalChannelResponse},
{RIL_REQUEST_SET_UICC_SUBSCRIPTION, radioSim::setUiccSubscriptionResponse},
{RIL_REQUEST_SIM_AUTHENTICATION, radioSim::requestIccSimAuthenticationResponse},
{RIL_REQUEST_CDMA_SUBSCRIPTION, radioSim::getCdmaSubscriptionResponse},
{RIL_REQUEST_ENTER_SIM_DEPERSONALIZATION, radioSim::supplySimDepersonalizationResponse},
{RIL_REQUEST_SET_SIM_CARD_POWER, radioSim::setSimCardPowerResponse},
{RIL_REQUEST_ENABLE_UICC_APPLICATIONS, radioSim::enableUiccApplicationsResponse},
{RIL_REQUEST_ARE_UICC_APPLICATIONS_ENABLED, radioSim::areUiccApplicationsEnabledResponse},
{RIL_REQUEST_GET_SIM_PHONEBOOK_RECORDS, radioSim::getSimPhonebookRecordsResponse},
{RIL_REQUEST_GET_SIM_PHONEBOOK_CAPACITY, radioSim::getSimPhonebookCapacityResponse},
{RIL_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS, radioSim::updateSimPhonebookRecordsResponse},
{RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE, radioSim::setCdmaSubscriptionSourceResponse},
{RIL_REQUEST_SET_CARRIER_INFO_IMSI_ENCRYPTION, radioSim::setCarrierInfoForImsiEncryptionResponse},
{RIL_REQUEST_SET_ALLOWED_CARRIERS, radioSim::setAllowedCarriersResponse},
{RIL_REQUEST_GET_ALLOWED_CARRIERS, radioSim::getAllowedCarriersResponse},
