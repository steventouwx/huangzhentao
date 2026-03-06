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

package vendor.mediatek.hardware.mtkradioex.rcs;

import vendor.mediatek.hardware.mtkradioex.rcs.IMtkRadioExRcsResponse;
import vendor.mediatek.hardware.mtkradioex.rcs.IMtkRadioExRcsIndication;

/**
 * Interface declaring functions to rcs requests.
 */
@VintfStability
oneway interface IMtkRadioExRcs {
    /**
     * Notify RCS ROI status to Modem.
     *
     * @param serial Serial number of request.
     * @param status RCS ROI status.
     * @param clientId client Id.
     *
     */
    void switchRcsRoiStatus(in int serial, in boolean status, in int clientId);

    /**
     * Update RCS capabilities to Modem.
     *
     * @param serial Serial number of request.
     * @param mode update mode.
     * @param featureTags RCS feature tags.
     * @param clientId client Id.
     *
     */
    void updateRcsCapabilities(in int serial, in int mode, in String featureTags, in int clientId);

    /**
     * Update RCS Session info to Modem.
     *
     * @param serial Serial number of request.
     * @param count session count.
     * @param clientId client Id.
     *
     */
    void updateRcsSessionInfo(in int serial, in int count, in int clientId);

    /**
     * Query user agent
     * @param serial Serial number of request.
     * @param clientId client Id.
     *
     */
    void getUserAgent(in int serial, in int clientId);

    /**
     * When response type received from a radio indication or radio response is
     * RadioIndicationType:UNSOLICITED_ACK_EXP or RadioResponseType:SOLICITED_ACK_EXP respectively,
     * acknowledge the receipt of those messages by sending responseAcknowledgement().
     *
     */
    void responseAcknowledgementMtk();

    /*
     * Set response functions for Rcs radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsRcs(in IMtkRadioExRcsResponse radioResponse,
        in IMtkRadioExRcsIndication radioIndication);

    /*
     * Notify RCS service is enabled, must be called
     * before any RCS operation performed.
     *
     * @param serial Serial number of request.
     * @param clientId Client ID.
     *
     */
    void startRcsService(int serial, int clientId);

    /**
     * Notify RCS service is disabled, after that
     * all RCS related operation will become not available.
     *
     * @param serial Serial number of request.
     * @param clientId Client ID.
     *
     */
    void stopRcsService(int serial, int clientId);

    /**
     * Request current RCS registration info.
     *
     * @param serial Serial number of request.
     * @param clientId Client ID.
     *
     */
    void requestRegistrationInfo(int serial, int clientId);

    /**
     * Set enable/disable IMS deregistration indication and
     * IMS deregistration postponed.
     *
     * @param serial Serial number of request.
     * @param enabled true to enable and false to disable.
     * @param clientId client ID.
     *
     */
    void setImsDeregistrationIndicationEnabled(int serial, boolean enabled, int clientId);

    /**
     * Notify IMS to resume performing deregistration.
     *
     * @param serial Serial number of request.
     * @param clientId Client ID.
     *
     */
    void resumeImsDeregistration(int serial, int clientId);

    /**
     * Trigger IMS registration full restoration.
     *
     * @param serial Serial number of request.
     * @param clientId Client ID.
     *
     */
    void triggerRestoration(int serial, int clientId);

    /**
     * Send SIP message.
     *
     * @param serial Serial number of request.
     * @param requestId Request ID for SIP data.
     * @param connId Connection ID for SIP data.
     * @param msgData Message data to be sent.
     * @param clientId Client ID.
     *
     */
    void sendSipMessage(int serial, int requestId, int connId, in byte[] msgData, int clientId);
}
