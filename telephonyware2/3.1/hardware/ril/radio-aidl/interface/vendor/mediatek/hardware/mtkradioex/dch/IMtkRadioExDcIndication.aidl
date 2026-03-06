/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2023. All rights reserved.
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

package vendor.mediatek.hardware.mtkradioex.dch;

import android.hardware.radio.RadioIndicationType;
import vendor.mediatek.hardware.mtkradioex.dch.DcConfig;

/**
 * Interface declaring unsolicited data channel indications.
 */
@VintfStability
interface IMtkRadioExDcIndication {

    /**
     * [DC] modem is ready to receive DC capability.
     * @param type Type of radio indication
     * @param callId Identify the related call session
     * @param operatorId operator Id, rild is uint16_t
     */
    oneway void dcBootstrapCfgQueryCmd(in RadioIndicationType type, in byte callId, in int operatorId);

    /**
     * [DC] send the initial DC configurations related to a call to DC manager.
     * @param type Type of radio indication
     * @param callId Identify the related call session
     * @param ifName if name, Used by AP DC manager to create IMS socket(Note: MAX_DC_IF_NAME_LENGTH = 16)
     * @param configList Indication data, max size MAX_DC_CONFIG_NUM
     */
    oneway void dcConfigInitInd(in RadioIndicationType type, in byte callId, in String ifName, in DcConfig[] configList);

    /**
     * [DC] update the DC configurations related to an existing call to DC manager.
     * @param type Type of radio indication
     * @param callId Identify the related call session
     * @param name if name
     * @param configList Indication data, max size MAX_DC_CONFIG_NUM
     */
    oneway void dcConfigUpdateInd(in RadioIndicationType type, in byte callId, in String ifName, in DcConfig[] configList);

    /**
     * [DC] Notify DC manager to deinit all the DC streams related with specific call.
     * @param type Type of radio indication
     * @param callId Identify the related call session
     */
    oneway void dcConfigDeinitInd(in RadioIndicationType type, in byte callId);

    /**
     * [DC] This primitive is used to indicate the DC manager that remote side has requested for creating new dcmap stream.
     * @param callId Identify the related call session
     * @param configList Indication data, max size MAX_ADC_CONFIG_NUM
     */
    oneway void dcCreateCmd(in RadioIndicationType type, in byte callId, in DcConfig[] configList);

    /**
     * [DCH] This command is used to ifup packet route ind.
     * @param type Type of radio indication
     * @param indData Data include net type, nw itf id, hw upload q
     *
     */
    oneway void dchDataIfupPacketRouteInd(in RadioIndicationType type, in int[] indData);

    /**
     * [DCH] Connect data channel indication from modem
     * @param type Type of radio indication
     * @param callId Identify the related call session
     * @param xmlStrData xml format String
     *
     */
    oneway void dchConnectDataChannelInd(in RadioIndicationType type, in int callId, in String xmlStrData);

    /**
     * [DCH] Close data channel indication from modem
     * @param type Type of radio indication
     * @param callId Identify the related call session
     * @param xmlStrData xml format String
     *
     */
    oneway void dchCloseDataChannelInd(in RadioIndicationType type, in int callId, in String xmlStrData);

    /**
     * [DCH] Create data channel indication from modem
     * @param type Type of radio indication
     * @param callId Identify the related call session
     * @param xmlStrData xml format String
     *
     */
    oneway void dchCreateDataChannelInd(in RadioIndicationType type, in int callId, in String xmlStrData);
}