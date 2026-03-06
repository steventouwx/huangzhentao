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
{RIL_REQUEST_SEND_SMS, radioMessaging::sendSmsResponse},
{RIL_REQUEST_SEND_SMS_EXPECT_MORE, radioMessaging::sendSMSExpectMoreResponse},
{RIL_REQUEST_SMS_ACKNOWLEDGE, radioMessaging::acknowledgeLastIncomingGsmSmsResponse},
{RIL_REQUEST_WRITE_SMS_TO_SIM, radioMessaging::writeSmsToSimResponse},
{RIL_REQUEST_DELETE_SMS_ON_SIM, radioMessaging::deleteSmsOnSimResponse},
{RIL_REQUEST_CDMA_SEND_SMS, radioMessaging::sendCdmaSmsResponse},
{RIL_REQUEST_CDMA_SEND_SMS_EXPECT_MORE, radioMessaging::sendCdmaSmsExpectMoreResponse},
{RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE, radioMessaging::acknowledgeLastIncomingCdmaSmsResponse},
{RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG, radioMessaging::getGsmBroadcastConfigResponse},
{RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG, radioMessaging::setGsmBroadcastConfigResponse},
{RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION, radioMessaging::setGsmBroadcastActivationResponse},
{RIL_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG, radioMessaging::getCdmaBroadcastConfigResponse},
{RIL_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG, radioMessaging::setCdmaBroadcastConfigResponse},
{RIL_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION, radioMessaging::setCdmaBroadcastActivationResponse},
{RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM, radioMessaging::writeSmsToRuimResponse},
{RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM, radioMessaging::deleteSmsOnRuimResponse},
{RIL_REQUEST_GET_SMSC_ADDRESS, radioMessaging::getSmscAddressResponse},
{RIL_REQUEST_SET_SMSC_ADDRESS, radioMessaging::setSmscAddressResponse},
{RIL_REQUEST_REPORT_SMS_MEMORY_STATUS, radioMessaging::reportSmsMemoryStatusResponse},
{RIL_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU, radioMessaging::acknowledgeIncomingGsmSmsWithPduResponse},
{RIL_REQUEST_IMS_SEND_SMS, radioMessaging::sendImsSmsResponse},
