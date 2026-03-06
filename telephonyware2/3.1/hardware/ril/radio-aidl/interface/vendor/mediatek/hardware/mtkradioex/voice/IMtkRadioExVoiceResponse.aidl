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

/**
 * Interface declaring response functions to solicited radio requests for voice APIs
 */
@VintfStability
interface IMtkRadioExVoiceResponse {
    /**
     * Hang up all response
     * Response of RIL request "RIL_REQUEST_HANGUP_ALL"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void hangupAllResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Hang up with specific reason to the network response
     * Response of RIL request "RIL_REQUEST_HANGUP_WITH_REASON"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void hangupWithReasonResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response of RIL request "RIL_REQUEST_GET_CALL_SUB_ADDRESS"
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param enable the current enable/disable status.
     */
    oneway void getCallSubAddressResponse(in android.hardware.radio.RadioResponseInfo info,
        in int enable);

    /**
     * The response of Query COLP (Connected Line Identification Presentation)
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param n Parameter sets/shows the result code presentation status to the TE.
     *        "0" means disabled, "1" means enabled.
     * @param m Parameter shows the subscriber COLP service status in the network
     *        "0" means not provisioned, "1" means provisioned in permanent mode
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:NO_MEMORY
     *   RadioError:MODEM_ERR
     *   RadioError:INTERNAL_ERR
     *   RadioError:FDN_CHECK_FAILURE
     *   RadioError:SYSTEM_ERR
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getColpResponse(in android.hardware.radio.RadioResponseInfo info, in int n,
        in int m);

    /**
     * The response of Query COLR (Connected Line Identification Restriction)
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param n COLR status in network. "0" means not provisioned, "1" means provisioned,
     *        "2" means unknown
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:NO_MEMORY
     *   RadioError:MODEM_ERR
     *   RadioError:INTERNAL_ERR
     *   RadioError:FDN_CHECK_FAILURE
     *   RadioError:SYSTEM_ERR
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getColrResponse(in android.hardware.radio.RadioResponseInfo info, in int n);

    /**
     * Response of getEccNum
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void getEccNumResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param callForwardInfoExs points to a vector of CallForwardInfoEx, one for
     *        each distinct registered phone number.
     *
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:NO_MEMORY
     *   RadioError:SYSTEM_ERR
     *   RadioError:MODEM_ERR
     *   RadioError:INTERNAL_ERR
     *   RadioError:FDN_CHECK_FAILURE
     *   RadioError:REQUEST_NOT_SUPPORTED
     *   RadioError:SYSTEM_ERR
     */
    oneway void queryCallForwardInTimeSlotStatusResponse(
        in android.hardware.radio.RadioResponseInfo info,
        in CallForwardInfoEx[] callForwardInfoExs);

    /**
     * Response for reset SS.
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void resetSuppServResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of Query CNAP (Calling Name Presentation)
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param n Integer type (parameter sets/shows the result code presentation status to the TE)
     *        "0" means disabled, "1" means enabled.
     * @param m Integer type (parameter shows the subscriber CNAP service status in the network)
     *        "0" means not provisioned, "1" means provisioned, "2" means unknown
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:NO_MEMORY
     *   RadioError:MODEM_ERR
     *   RadioError:INTERNAL_ERR
     *   RadioError:FDN_CHECK_FAILURE
     *   RadioError:SYSTEM_ERR
     *   RadioError:GENERIC_FAILURE
     */
    oneway void sendCnapResponse(in android.hardware.radio.RadioResponseInfo info, in int n,
        in int m);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:NO_MEMORY
     *   RadioError:SYSTEM_ERR
     *   RadioError:MODEM_ERR
     *   RadioError:INTERNAL_ERR
     *   RadioError:INVALID_STATE
     *   RadioError:FDN_CHECK_FAILURE
     *   RadioError:REQUEST_NOT_SUPPORTED
     *   RadioError:INVALID_MODEM_STATE
     */
    oneway void setCallForwardInTimeSlotResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * Proprietary incoming call handling.
     * Response of RIL request "RIL_REQUEST_SET_CALL_INDICATION"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setCallIndicationResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response of RIL request "RIL_REQUEST_SET_CALL_SUB_ADDRESS"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setCallSubAddressResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * Response for set call valid.
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setCallValidTimerResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of Set CLIP (Calling Line Identification Presentation)
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:SYSTEM_ERR
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setClipResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of Set COLP
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:SYSTEM_ERR
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setColpResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of Set COLR
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:SYSTEM_ERR
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setColrResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response E911 set Emergency Mode
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setEccModeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response of setEccNum
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setEccNumResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * Response for set gwsd mode.
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setGwsdModeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for set ignore same phone number interval.
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setIgnoreSameNumberIntervalResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for set keep alive by ip data.
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setKeepAliveByIpDataResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for set keep alive by PDCP.
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setKeepAliveByPDCPCtrlPDUResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for set SS property status.
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setSuppServPropertyResponse(in android.hardware.radio.RadioResponseInfo info);

}
