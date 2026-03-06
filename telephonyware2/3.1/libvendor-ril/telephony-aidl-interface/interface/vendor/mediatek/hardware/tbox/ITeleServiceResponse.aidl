/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package vendor.mediatek.hardware.tbox;
import  vendor.mediatek.hardware.tbox.OperatorInfoWithAct;
import vendor.mediatek.hardware.tbox.SendSmsResult;
import vendor.mediatek.hardware.tbox.RegStateResult;
import vendor.mediatek.hardware.tbox.CellInfo;

/**
 * This interface is used by client  to talk to tele-fwk.
 * All the functions have minimum one parameter:
 * serial: which corresponds to serial no. of request. Serial numbers must only be memorized for the
 * duration of a method call. If clients provide colliding serials (including passing the same
 * serial to different methods), multiple responses (one for each method call) must still be served.
 */
// Interface inherits from vendor.mediatek.hardware.tbox@1.1::ITeleServiceResponse but AIDL does not support interface inheritance (methods have been flattened).
@VintfStability
interface ITeleServiceResponse {
    oneway void enableRadioResponse(in int ril_err);

    oneway void iccTransmitApduLogicalChannelResponse(in int ril_err);

    oneway void sendSmsResponse(in int ril_err);

    oneway void setPreferredNetworkTypeResponse(in int ril_err);

    /**
     * @param networkInfos List of network operator information as OperatorInfoWithAct defined in
     *         OperatorInfoWithAct.hal
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void getAvailableNetworksWithActResponse(in OperatorInfoWithAct[] networkInfosWithAct);


    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param sms Response to sms sent as defined by SendSmsResult
     *
     */
    oneway void sendSmsNoEncoderResponse(in SendSmsResult sms);
    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:REQUEST_NOT_SUPPORTED when android.hardware.telephony.radio.access is not
     *                                    defined
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:ILLEGAL_SIM_OR_ME
     *   RadioError:OPERATION_NOT_ALLOWED
     *   RadioError:NO_MEMORY
     *   RadioError:INTERNAL_ERR
     *   RadioError:SYSTEM_ERR
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:MODEM_ERR
     *   RadioError:NO_RESOURCES
     *   RadioError:CANCELLED
     *
     * Returns RadioError:ILLEGAL_SIM_OR_ME when the failure is permanent and
     * no retries needed, such as illegal SIM or ME.
     */
    void setNetworkSelectionModeAutomaticResponse(in int ril_err);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:REQUEST_NOT_SUPPORTED when android.hardware.telephony.radio.access is not
     *                                    defined
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:ILLEGAL_SIM_OR_ME
     *   RadioError:OPERATION_NOT_ALLOWED
     *   RadioError:INVALID_STATE
     *   RadioError:NO_MEMORY
     *   RadioError:INTERNAL_ERR
     *   RadioError:SYSTEM_ERR
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:MODEM_ERR
     *   RadioError:NO_RESOURCES
     *   RadioError:CANCELLED
     *
     * Returns RadioError:ILLEGAL_SIM_OR_ME when the failure is permanent and
     * no retries needed, such as illegal SIM or ME.
     */
    void setNetworkSelectionModeManualResponse(in int ril_err);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param voiceRegResponse Current Voice registration response as defined by RegStateResult
     *
     * Valid errors returned:
     *   RadioError:REQUEST_NOT_SUPPORTED when android.hardware.telephony.radio.access is not
     *                                    defined
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INTERNAL_ERR
     */
    void getVoiceRegistrationStateResponse(in RegStateResult voiceRegResponse);
	/**
     * @param info Response info struct containing response type, serial no. and error
     * @param dataRegResponse Current data registration response as defined by RegStateResult
     *
     * Valid errors returned:
     *   RadioError:REQUEST_NOT_SUPPORTED when android.hardware.telephony.radio.access is not
     *                                    defined
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INTERNAL_ERR
     *   RadioError:NOT_PROVISIONED
     */
    void getDataRegistrationStateResponse(in RegStateResult dataRegResponse);
    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param cellInfo List of current cell information known to radio
     *
     * Valid errors returned:
     *   RadioError:REQUEST_NOT_SUPPORTED when android.hardware.telephony.radio.access is not
     *                                    defined
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INTERNAL_ERR
     */
    void getCellInfoListResponse(in CellInfo[] cellInfo);
}
