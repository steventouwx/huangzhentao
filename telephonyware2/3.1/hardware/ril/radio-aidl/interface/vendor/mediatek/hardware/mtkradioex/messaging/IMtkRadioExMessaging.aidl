// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.messaging;

import android.hardware.radio.RadioResponseInfo;
import vendor.mediatek.hardware.mtkradioex.messaging.SmsParams;
import vendor.mediatek.hardware.mtkradioex.messaging.IMtkRadioExMessagingResponse;
import vendor.mediatek.hardware.mtkradioex.messaging.IMtkRadioExMessagingIndication;


/**
 * Interface declaring response functions to subsidylock requests.
 */
@VintfStability
oneway interface IMtkRadioExMessaging {

    /**
     * Get GSM cell broadcast activation
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getGsmBroadcastActivationRsp
     */
    void getGsmBroadcastActivation(in int serial, in int clientId);

    /**
     * Get GSM cell broadcast language
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getGsmBroadcastLangsResponse
     */
    void getGsmBroadcastLangs(in int serial, in int clientId);

    /**
     * Get SMS memory status
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getSmsMemStatusResponse
     */
    void getSmsMemStatus(in int serial, in int clientId);

    /**
     * Get SMS parameters
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getSmsParametersResponse
     */
    void getSmsParameters(in int serial, in int clientId);

    /**
     * Get the memory status of the SMS in RUIM.
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getSmsRuimMemoryStatusResponse()
     */
    void getSmsRuimMemoryStatus(in int serial, in int clientId);

    /**
     * Remove cell broadcast message id
     *
     * @param serial Serial number of request.
     * @param channelId removed channel id
     * @param serialId removed serial id
     *
     * Response function is IMtkRadioExResponse.removeCbMsgResponse
     */
    void removeCbMsg(in int serial, in int channelId, in int serialId, in int clientId);


    /**
     * Set Earthquake and Tsunami Warning System on/off
     *
     * @param serial Serial number of request.
     * @param mode identifies to turn on or off
     *
     * Response function is IMtkRadioExResponse.setEtwsResponse
     */
    void setEtws(in int serial, in int mode, in int clientId);


    /**
     * Set GSM cell broadcast language
     *
     * @param serial Serial number of request.
     * @param langs language setting string
     *
     * Response function is IMtkRadioExResponse.setGsmBroadcastLangsResponse
     */
    void setGsmBroadcastLangs(in int serial, in String langs, in int clientId);

    /**
     * Set SMS parameters
     *
     * @param serial Serial number of request.
     * @param message Sms parameters(pid, vp, format, dcs) refer to the structure SmsParams
     *                 defined in type.hal
     *
     * Response function is IMtkRadioExResponse.setSmsParametersResponse
     */
    void setSmsParameters(in int serial, in SmsParams message, in int clientId);

    /**
     * Exit SCBM
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.exitSCBMResponse
     */
    void exitSCBM(in int serial, in int clientId);

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
    void setResponseFunctionsMtk(in IMtkRadioExMessagingResponse radioResponse,
        in IMtkRadioExMessagingIndication radioIndication);
}
