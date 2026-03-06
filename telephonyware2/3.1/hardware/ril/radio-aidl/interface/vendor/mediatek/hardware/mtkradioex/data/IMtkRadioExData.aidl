// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.data;

import android.hardware.radio.AccessNetwork;
import android.hardware.radio.data.DataProfileInfo;
import android.hardware.radio.data.DataRequestReason;
import android.hardware.radio.data.LinkAddress;
import android.hardware.radio.data.SliceInfo;

import vendor.mediatek.hardware.mtkradioex.data.IMtkRadioExDataResponse;
import vendor.mediatek.hardware.mtkradioex.data.IMtkRadioExDataIndication;
import vendor.mediatek.hardware.mtkradioex.data.TrafficDescriptor;

/**
 * Interface declaring unsolicited indications to subsidylock.
 */
@VintfStability
oneway interface IMtkRadioExData {
    /**
     * Do ps/cs attach.
     *
     * @param serial ril-request serial
     * @param type ps attach or ps&cs attach
     */
    void dataConnectionAttach(in int serial, in int type, in int clientId);

    /**
     * Do ps/cs detach.
     *
     * @param serial ril-request serial
     * @param type ps detach or ps&cs detach
     */
    void dataConnectionDetach(in int serial, in int type, in int clientId);

    /**
     * Enable the indication event of DSDA mode changed
     *
     * @param serial Serial number of request
     * @param enable Enable or disable
     *
     * Response function is IMtkRadioExRadioResponse.enableDsdaIndicationResponse
     */
    void enableDsdaIndication(in int serial, in boolean enable, in int clientId);

    /**
     * Get the current DSDA status
     *
     * @param serial Serial number of request
     *
     * Response function is IMtkRadioExRadioResponse.getDsdaStatusResponse
     */
    void getDsdaStatus(in int serial, in int clientId);

    /**
     * Cleanup all connections.
     *
     * @param serial ril-request serial
     */
    void resetAllConnections(in int serial, in int clientId);

    /**
     * M: Data Framework - Data Retry enhancement
     * Reset the modem's maintained retry count for the apn name
     *
     * @param serial Serial number of request.
     * @param apn the associated apn name.
     *
     *  Response function is IMtkRadioExResponse.resetMdDataRetryCountResponse
     */
    void resetMdDataRetryCount(in int serial, in String apn, in int clientId);

    /**
     * Set Fast dormancy configuration.
     *
     * @param serial ril-request serial
     * @param mode fastdormancy mode
     * @param param1 screen status
     * @param param2 timer value
     */
    void setFdMode(in int serial, in int mode, in int param1, in int param2, in int clientId);

    /**
     * M: Data Framework - common part enhancement
     * Sync data settings to modem
     *
     * @param serial Serial number of request.
     * @param settings contain the status of data setting, the status of data roaming setting,
     *        and the selection of the default data SIM consecutively.
     *
     *  Response function is IMtkRadioExResponse.syncDataSettingsToMdResponse
     */
    void syncDataSettingsToMd(in int serial, in int[] settings, in int clientId);


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
    void setResponseFunctionsMtk(in IMtkRadioExDataResponse radioResponse,
        in IMtkRadioExDataIndication radioIndication);

    /**
     * Set response functions for mtk assist requests.
     *
     * @param radioResponse Object containing response functions
     */
    void setResponseFunctionsAssist(in vendor.mediatek.hardware.mtkradioex.assist.IAssistRadioResponse radioResponse);

    /**
     * Setup data call customization version. Add extra parameters.
     *
     * @param trafficDescriptor Customized traffic descriptor
     * @param verifyDescriptor Customized verify descriptor
     * @param responseMode 0 control return response
     *
     * Response function is controlled by responseMode:
     *     0 return by IRadioDataResponse.setupDataCallResponse
     *     1 return by IMtkRadioExDataResponse.setupDataCallResponseSlice
     */
    void setupDataCallSlice(in int serial, in AccessNetwork accessNetwork,
            in DataProfileInfo dataProfileInfo, in boolean roamingAllowed,
            in DataRequestReason reason, in LinkAddress[] addresses, in String[] dnses,
            in int pduSessionId, in @nullable SliceInfo sliceInfo,
            in boolean matchAllRuleAllowed,
            in @nullable TrafficDescriptor trafficDescriptor,
            in @nullable String verifyDescriptor, in int responseMode,
            in int clientId);
}
