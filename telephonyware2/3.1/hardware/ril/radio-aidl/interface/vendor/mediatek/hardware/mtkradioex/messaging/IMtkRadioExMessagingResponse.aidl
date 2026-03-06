// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.messaging;

import android.hardware.radio.RadioResponseInfo;
import vendor.mediatek.hardware.mtkradioex.messaging.SmsMemStatus;
import vendor.mediatek.hardware.mtkradioex.messaging.SmsParams;

/**
 * Interface declaring response functions to subsidylock requests.
 */
@VintfStability
interface IMtkRadioExMessagingResponse {

    /**
     * The response of getGsmBroadcastActivation
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param gsm cell broadcase setting is on or off
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void getGsmBroadcastActivationRsp(in android.hardware.radio.RadioResponseInfo info,
        in int active);

    /**
     * The response of getGsmBroadcastLangs
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param GSM cell broadcast language setting
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void getGsmBroadcastLangsResponse(in android.hardware.radio.RadioResponseInfo info,
        in String langs);

    /**
     * The response of getSmsMemStatus
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param status The number of used and total. refer to the structure SmsMemStatus
     *               defined in type.hal
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void getSmsMemStatusResponse(in android.hardware.radio.RadioResponseInfo info,
        in SmsMemStatus status);

    /**
     * The response of getSmsParameters
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param param Sms parameters(pid, vp, format, dcs). refer to the structure SmsParams
     *              defined in type.hal
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void getSmsParametersResponse(in android.hardware.radio.RadioResponseInfo info,
        in SmsParams param);

    /**
     * The response function of getSmsRuimMemoryStatus.
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param memStatus The memory status of SMS in RUIM, the status includes the total number
     *                  and the used number
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:SIM_ABSENT
     *   RadioError:SYSTEM_ERR
     */
    oneway void getSmsRuimMemoryStatusResponse(in android.hardware.radio.RadioResponseInfo info,
        in SmsMemStatus memStatus);

    /**
     * The response of removeCbMsg
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void removeCbMsgResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of setEtws
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void setEtwsResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of setGsmBroadcastLangs
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void setGsmBroadcastLangsResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * The response of setSmsParameters
     */
    oneway void setSmsParametersResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of exitSCBM
     */
    oneway void exitSCBMResponse(in android.hardware.radio.RadioResponseInfo info);
}
