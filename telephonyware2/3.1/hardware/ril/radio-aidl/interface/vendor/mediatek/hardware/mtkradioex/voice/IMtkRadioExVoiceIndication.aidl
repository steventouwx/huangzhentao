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

package vendor.mediatek.hardware.mtkradioex.voice;

import android.hardware.radio.RadioResponseInfo;
import vendor.mediatek.hardware.mtkradioex.voice.CallInfoType;
import vendor.mediatek.hardware.mtkradioex.voice.CfuStatusNotification;
import vendor.mediatek.hardware.mtkradioex.voice.CipherNotification;
import vendor.mediatek.hardware.mtkradioex.voice.CrssNotification;
import vendor.mediatek.hardware.mtkradioex.voice.IncomingCallNotification;

/**
 * Interface declaring unsolicited radio indications for voice APIs.
 */
@VintfStability
interface IMtkRadioExVoiceIndication {
    /**
     * Incoming Call Additional Information
     *
     * @param type Type of radio indication
     * @param ciType call addition info type
     * @param info incoming call additional info
     */
    oneway void callAdditionalInfoInd(in android.hardware.radio.RadioIndicationType type,
        in CallInfoType ciType, in String[] info);

    /**
     * Reports cdma call is accepted.
     * URC: RIL_UNSOL_CDMA_CALL_ACCEPTED
     *
     * @param type Type of radio indication
     */
    oneway void cdmaCallAccepted(in android.hardware.radio.RadioIndicationType type);

    /**
     * Notify application to show or hide CFU (Call Forwarding Uncondictional) icon
     *
     * @param type Type of radio indication
     * @param cfuStatus CfuStatusNotification as defined in types.hal
     */
    oneway void cfuStatusNotify(in android.hardware.radio.RadioIndicationType type,
        in CfuStatusNotification cfuStatus);

    /**
     * Reports ciphering indicator information
     *
     * @param type Type of radio indication
     * @param cipherNotify CipherNotification as defined in types.hal
     */
    oneway void cipherIndication(in android.hardware.radio.RadioIndicationType type,
        in CipherNotification cipherNotify);

    /**
     * Indicates the orders of participants call ids when IMS conference SRVCC.
     *
     * @param type Type of radio indication
     * @param callIds the order of calls id
     */
    oneway void confSRVCC(in android.hardware.radio.RadioIndicationType type, in int[] callIds);

    /**
     * Reports CRSS related supplementary service notification from the network.
     * URC: RIL_UNSOL_CRSS_NOTIFICATION
     *
     * @param type Type of radio indication
     * @param crssNotify CrssNotification as defined in types.hal
     */
    oneway void crssIndication(in android.hardware.radio.RadioIndicationType type,
        in CrssNotification crssNotify);

    /**
     * Reports ECC numbers.
     *
     * @param type Type of radio indication
     * @param ecc_list_with_card ECC numbers when card inserted
     * @param ecc_list_no_card ECC numbers when card not inserted
     */
    oneway void eccNumIndication(in android.hardware.radio.RadioIndicationType type,
        in String ecc_list_with_card, in String ecc_list_no_card);

    /**
     * Reports incoming call indication from the network.
     * URC: RIL_UNSOL_INCOMING_CALL_INDICATION
     *
     * @param type Type of radio indication
     * @param inCallNotify IncomingCallNotification as defined in types.hal
     */
    oneway void incomingCallIndication(in android.hardware.radio.RadioIndicationType type,
        in IncomingCallNotification inCallNotify);


    /**
     * Reports supplementary service related notification from the network.
     * The CSSU code is extended per MTK spec.
     *
     * @param type Type of radio indication
     * @param suppSvc SuppSvcNotification as defined in types.hal
     */
    oneway void suppSvcNotifyEx(in android.hardware.radio.RadioIndicationType type,
        in android.hardware.radio.network.SuppSvcNotification suppSvc);
}
