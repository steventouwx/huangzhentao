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

import android.hardware.radio.RadioResponseInfo;

/**
 * Interface declaring response functions to solicited radio requests.
 */
@VintfStability
interface IMtkRadioExDcResponse {
    /**
     * @param responseInfo Response info struct containing response type, serial no. and error
     */
    oneway void dcBootstrapCfgQueryCmdRspCnf(in RadioResponseInfo responseInfo);

    /**
     * @param responseInfo Response info struct containing response type, serial no. and error
     * @param data Data include callId, result, casue
     */
    oneway void dcCreateSipResponse(in RadioResponseInfo responseInfo, in int[] resultData);

    /**
     * @param responseInfo Response info struct containing response type, serial no. and error
     * @param data Data include callId, result, casue
     */
    oneway void dcModifySipResponse(in RadioResponseInfo responseInfo, in int[] resultData);

    /**
     * @param responseInfo Response info struct containing response type, serial no. and error
     * @param data Data include callId, result, casue
     */
    oneway void dcCloseSipResponse(in RadioResponseInfo responseInfo, in int[] resultData);

    /**
     * @param responseInfo Response info struct containing response type, serial no. and error
     */
    oneway void dcCreateCmdRspCnf(in RadioResponseInfo responseInfo);

    /**
     * [DCH] it is the response of dchDataActPacketRouteRequest
     * @param responseInfo Response info struct containing response type, serial no. and error
     *
     */
    oneway void dchDataActPacketRouteResponse(in RadioResponseInfo responseInfo);

    /**
     * [DCH] it is the response of dchDataIfupPacketRouteRequest
     * @param responseInfo Response info struct containing response type, serial no. and error
     *
     */
    oneway void dchDataIfupPacketRouteResponse(in RadioResponseInfo responseInfo);

    /**
     * [DCH] [CNF]Used to provide MD configurations on DC data path to AP.
     * @param serial Serial number of request.
     * @param clientAddr client IP address, Max 16 bytes
     * @param serverAddr server IP address, Max 16 bytes
     * @param dcMss Max payload size (DC application data) for one-time transmission across MD/AP
     *
     */
    oneway void dchDataPathConfigSyncResponse(in RadioResponseInfo responseInfo, in byte[] clientAddr, in byte[] serverAddr, in int dcMss);

    /**
     * [DCH] response of create data channel
     * @param responseInfo Response info struct containing response type, serial no. and error
     * @param callId Identify the related call session
     * @param xmlStrData xml format String
     *
     */
    oneway void dchCreateDataChannelResponse(in RadioResponseInfo responseInfo, in int callId, in String xmlStrData);

    /**
     * [DCH] response of close data channel
     * @param responseInfo Response info struct containing response type, serial no. and error
     * @param callId Identify the related call session
     * @param xmlStrData xml format String
     *
     */
    oneway void dchCloseDataChannelResponse(in RadioResponseInfo responseInfo, in int callId, in String xmlStrData);
}