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
import vendor.mediatek.hardware.mtkradioex.dch.DcConfig;
import vendor.mediatek.hardware.mtkradioex.dch.DcConfigLocal;
import vendor.mediatek.hardware.mtkradioex.dch.DcMapDelStruct;
import vendor.mediatek.hardware.mtkradioex.dch.IMtkRadioExDcIndication;
import vendor.mediatek.hardware.mtkradioex.dch.IMtkRadioExDcResponse;

/**
 * Interface declaring functions to data channel requests.
 */
@VintfStability
oneway interface IMtkRadioExDc {

    /**
     * [DC] set capability information to modem
     * @param serial Serial number of request.
     * @param callId Identify the related call session
     * @param result 0: Fail, 1: Success
     * @param data capability data structure DcConfigLocal, max size MAX_BDC_CONFIG_NUM.
     * @param clientId currently only support CLIENT_DCH = 9
     *
     */
    void dcBootstrapCfgQueryCmdRsp(in int serial, in byte callId, in byte result, in DcConfigLocal[] dataList, in int clientId);

    /**
     * [DC] set DC capability information to modem
     * @param serial Serial number of request.
     * @param callId Identify the related call session
     * @param data DcConfig structure array, MAX_ADC_CONFIG_NUM.
     * @param clientId currently only support CLIENT_DCH = 9
     *
     */
    void dcCreateSipRequest(in int serial, in byte callId, in DcConfig[] dataList, in int clientId);

    /**
     * [DC]set DC capability information to modem
     * @param serial Serial number of request.
     * @param callId Identify the related call session
     * @param data DcConfig structure array, MAX_ADC_CONFIG_NUM.
     * @param clientId currently only support CLIENT_DCH = 9
     *
     */
    void dcModifySipRequest(in int serial, in byte callId, in DcConfig[] dataList, in int clientId);

    /**
     * [DC]set DC capability information to modem
     * @param serial Serial number of request.
     * @param callId Identify the related call session
     * @param closeAllStream 0: No, 1: Yes
     * @param delList DcMapDelStruct structure array, MAX_DC_DCMAP_NUM.
     * @param clientId currently only support CLIENT_DCH = 9
      *
     */
    void dcCloseSipRequest(in int serial, in byte callId, in byte closeAllStream, in DcMapDelStruct[] delList, in int clientId);

    /**
     * [DC] set DC capability information to modem
     * @param serial Serial number of request.
     * @param callId Identify the related call session
     * @param result 0: Fail, 1: Success
     * @param cause 0: None, X: Reserved, rild is uint16_t
     * @param dataList DcConfig structure array, MAX_ADC_CONFIG_NUM.
     * @param clientId currently only support CLIENT_DCH = 9
     *
     */
    void dcCreateCmdRsp(in int serial, in byte callId, in byte result, in int cause, in DcConfig[] dataList, in int clientId);

    /**
     * When response type received from a radio indication or radio response is
     * RadioIndicationType:UNSOLICITED_ACK_EXP or RadioResponseType:SOLICITED_ACK_EXP respectively,
     * acknowledge the receipt of those messages by sending responseAcknowledgement().
     *
     */
    void responseAcknowledgementMtk();

   /**
     * Set response functions for data channel radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsDc(in IMtkRadioExDcResponse radioResponse,
            in IMtkRadioExDcIndication radioIndication);

    /**
     * [DCH] This command is uesed to set ifup packet route information
     * @param serial Serial number of request.
     * @param clientId currently only support CLIENT_DCH = 9
     *
     */
    void dchDataActPacketRouteRequest(in int serial, in int clientId);

    /**
     * [DCH] This command is uesed to set ifup packet route information.
     * @param serial Serial number of request.
     * @param clientId currently only support CLIENT_DCH = 9
     *
     */
    void dchDataIfupPacketRouteRequest(in int serial, in int clientId);

    /**
     * [DCH] [REQ]Used to trigger MD start DC data path config.
     * @param serial Serial number of request.
     * @param clientId currently only support CLIENT_DCH = 9
     *
     */
    void dchDataPathConfigSyncRequest(in int serial, in int clientId);

    /**
     * [DCH] send create data channel information to modem
     * @param serial Serial number of request.
     * @param callId Identify the related call session
     * @param xmlStrData xml format String
     * @param clientId currently only support CLIENT_DCH = 9
     *
     */
    void dchCreateDataChannelRequest(in int serial, in int callId, in String xmlStrData, in int clientId);

    /**
     * [DCH] send close data channel information to modem
     * @param serial Serial number of request.
     * @param callId Identify the related call session
     * @param xmlStrData xml format String
     * @param clientId currently only support CLIENT_DCH = 9
     *
     */
    void dchCloseDataChannelRequest(in int serial, in int callId, in String xmlStrData, in int clientId);

   /**
     * Set response functions for data channel radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsDchInternal(in IMtkRadioExDcResponse radioResponse,
            in IMtkRadioExDcIndication radioIndication);
}
