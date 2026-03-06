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
import vendor.mediatek.hardware.mtkradioex.voice.CallForwardInfoEx;
import vendor.mediatek.hardware.mtkradioex.voice.IMtkRadioExVoiceResponse;
import vendor.mediatek.hardware.mtkradioex.voice.IMtkRadioExVoiceIndication;

/**
 * Interface declaring unsolicited indications to subsidylock.
 */
@VintfStability
oneway interface IMtkRadioExVoice {
    /**
     * Hang up all
     * After this HANGUP request returns, Radio must show the connection is NOT
     * active anymore in next getCurrentCalls() query.
     *
     * @param serial Serial number of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.hangupAllResponse()
     *
     */
    void hangupAll(in int serial, in int clientId);

    /**
     * Hang up with specific reason to the network
     * After this hangupWithReason request returns, Radio must show the connection is NOT
     * active anymore in next getCurrentCalls() query.
     *
     * @param serial Serial number of request
     * @param callId  call index
     * @param reason  disconnect reason
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.hangupWithReasonResponse()
     *
     */
    void hangupWithReason(in int serial, in int callId, in int reason, in int clientId);

    /**
     * Get call sub address feature enable or disable from modem.
     *
     * @parma serial Serail Number
     * @param clientId ril service client Id
     *
     */
    void getCallSubAddress(in int serial, in int clientId);

    /**
     * Query COLP (Connected Line Identification Presentation), for MMI code *#76*
     *
     * @param serial Serial number of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.getColpResponse
     */
    void getColp(in int serial, in int clientId);

    /**
     * Query COLR (Connected Line Identification Restriction), for MMI code *#77#
     *
     * @param serial Serial number of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.getColrResponse
     */
    void getColr(in int serial, in int clientId);

    /**
     * Get ECC numbers.
     *
     * @param serial Serial number of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.getEccNumResponse()
     *
     */
    void getEccNum(in int serial, in int clientId);

    /**
     * Request call forward in time slot status.
     *
     * @param serial Serial number of request
     * @param callInfoEx CallForwardInfoEx
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.queryCallForwardInTimeSlotStatusResponse()
     */
    void queryCallForwardInTimeSlotStatus(in int serial,
            in CallForwardInfoEx callInfoEx, in int clientId);

    /**
     * Reset supplementary service parameter.
     *
     * @param serial Serail Number
     * @param clientId ril service client Id
     */
    void resetSuppServ(in int serial, in int clientId);

    /**
     * Query CNAP (Calling Name Presentation), for MMI code *#300#
     *
     * @param serial Serial number of request
     * @param cnapssMessage CNAP message.
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.sendCnapResponse
     */
    void sendCnap(in int serial, in String cnapssMessage, in int clientId);


    /**
     * Change call barring facility password (Checked By Network)
     *
     * @param serial Serial number of request
     * @param facility facility string code from TS 27.007 7.4 (eg "AO" for BAOC)
     * @param oldPassword old password
     * @param newPassword new password
     * @param cfmPassword confirm password
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setBarringPasswordResponse()
     */
    void setBarringPasswordCheckedByNW(in int serial, in String facility,
        in String oldPassword, in String newPassword, in String cfmPassword, in int clientId);



    /**
     * Configure call forward rule in time slot
     *
     * @param serial Serial number of request
     * @param callInfoEx CallForwardInfoEx
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setCallForwardInTimeSlotResponse()
     */
    void setCallForwardInTimeSlot(in int serial, in CallForwardInfoEx callInfoEx, in int clientId);

    /**
     * Proprietary incoming call handling.
     * RIL Request: "RIL_REQUEST_SET_CALL_INDICATION"
     *
     * @param serial Serial number of request
     * @param mode allow/disallow for the call
     * @param callId id of the call
     * @param seqNumber sequence number of the call
     * @cause reason to set the mode
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setCallIndicationResponse()
     */
    void setCallIndication(in int serial, in int mode, in int callId, in int seqNumber,
        in int cause, in int clientId);

    /**
     * Set call sub address feature enable or disable to modem.
     *
     * @parma serial Serail Number
     * @param enable enable sub address or not
     * @param clientId ril service client Id
     *
     */
    void setCallSubAddress(in int serial, in boolean enable, in int clientId);

    /**
     * Set timer for call valid
     *
     * @param serial Serial number of request
     * @param timer 1 ~ 255sec(default 7sec)
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExRadioResponse.setCallValidTimerResponse
     */
    void setCallValidTimer(in int serial, in int timer, in int clientId);

    /**
     * Set CLIP (Calling Line Identification Presentation), for MMI code *30# and #30#
     *
     * @param serial Serial number of request
     * @param clipEnable CLIP mode to set
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setCLIPResponse
     */
    void setClip(in int serial, in int clipEnable, in int clientId);

    /**
     * Set COLP
     *
     * @param serial Serial number of request
     * @param colpEnable COLP mode to set
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setCOLPResponse
     */
    void setColp(in int serial, in int colpEnable, in int clientId);

    /**
     * Set COLR
     *
     * @param serial Serial number of request
     * @param colrEnable COLR mode to set
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setCOLRResponse
     */
    void setColr(in int serial, in int colrEnable, in int clientId);

    /**
     * E911 Request current status
     * RIL Request: RIL_REQUEST_CURRENT_STATUS
     *
     * @param serial Serail Number
     * @param number Emergency number
     * @param enable enable/disable
     * @param airplaneMode airplane mode (0: off, 1: on)
     * @param imsReg IMS registration state
     * @param clientId ril service client Id
     */
    void setEccMode(in int serial, in String number, in int enable, in int airplaneMode,
        in int imsReg, in int clientId);

    /**
     * Set ECC numbers.
     *
     * @param serial Serial number of request
     * @param ecc_list_with_card ECC numbers when card inserted
     * @param ecc_list_no_card ECC numbers when card not inserted
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setEccNumResponse()
     *
     */
    void setEccNum(in int serial, in String ecc_list_with_card,
        in String ecc_list_no_card, in int clientId);

    /**
     * Set GWSD Mode
     *
     * @param serial Serial number of request
     * @param data String[3]
     *        which contains:
     *        mode 1: User selection, 2: Auto reject
     *        keepAliveMode 301: control pdu, 307: ip data
     *        keepAliveCycle 16bit: druation, 4bits: period
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExRadioResponse.setGwsdModeResponse
     */
    void setGwsdMode(in int serial, in String[] data, in int clientId);

    /**
     * Set interval for ignore same phone number
     *
     * @param serial Serial number of request
     * @param interval 1 ~ 255sec(default 3sec)
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExRadioResponse.setUserResponseTimerResponse
     */
    void setIgnoreSameNumberInterval(in int serial, in int interval, in int clientId);

    /**
     * Set keep alive by ip data
     *
     * @param serial Serial number of request
     * @param config duration XXXX
     *               peroid   XX
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExRadioResponse.setKeepAlivesByIpDataResponse
     */
    void setKeepAliveByIpData(in int serial, in String config, in int clientId);

    /**
     * Set keep alive by PDCD control PDU
     *
     * @param serial Serial number of request
     * @param config duration XXXX
     *               peroid   XX
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExRadioResponse.setKeepAliveByPDCPCtrlPDUResponse
     */
    void setKeepAliveByPDCPCtrlPDU(in int serial, in String config, in int clientId);

    /**
     * Set Supplementary Service Property
     *
     * @param serial Serail Number
     * @param name Property Name
     * @param value Property Value
     * @param clientId ril service client Id
     */
    void setSuppServProperty(in int serial, in String name, in String value, in int clientId);

    /**
     * When response type received from a radio indication or radio response is
     * RadioIndicationType:UNSOLICITED_ACK_EXP or RadioResponseType:SOLICITED_ACK_EXP respectively,
     * acknowledge the receipt of those messages by sending responseAcknowledgement().
     *
     */
    void responseAcknowledgementMtk();

    /**
     * Set response functions for mtk radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsMtk(in IMtkRadioExVoiceResponse radioResponse,
        in IMtkRadioExVoiceIndication radioIndication);

    /**
     * Set response functions for mtk_ims radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsMtkIms(in IMtkRadioExVoiceResponse radioResponse,
        in IMtkRadioExVoiceIndication radioIndication);
}
