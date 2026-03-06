// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.modem;
import vendor.mediatek.hardware.mtkradioex.modem.IMtkRadioExModemResponse;
import vendor.mediatek.hardware.mtkradioex.modem.IMtkRadioExModemIndication;
import vendor.mediatek.hardware.mtkradioex.cap.IMtkRadioExCapRadioResponse;
import vendor.mediatek.hardware.mtkradioex.atci.IAtciResponse;
import vendor.mediatek.hardware.mtkradioex.atci.IAtciIndication;

/**
 * 
 */
@VintfStability
oneway interface IMtkRadioExModem {

    /**
     * Cancel certificate
     *
     * @param uid Caller process id
     */
    void abortCertificate(in int serial, in int uid, in int clientId);

    /**
     * Enable feature capability
     *
     * @param id Capability name
     * @param uid Caller process id
     * @param toActive Enable or disable
     */
    void enableCapability(in int serial, in String id, in int uid, in int toActive, in int clientId);

    /**
     * Query function for echolocate metrics.
     * @param serial Serial number of request.
     * @param index Request type of Echolocate metrics.
     *
     */
    void getEngineeringModeInfo(in int serial, in int index, in int clientId);

    /**
     * modify modem.
     *
     * @param serial Serial number of request.
     * @param applyType store/reload type
     * @param modemType which world mode is modified when reboot.
     */
    void modifyModemType(in int serial, in int applyType, in int modemType, in int clientId);


    /**
     * Requests to restart rild and modem
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.restartRILDResponse()
     *
     */
    void restartRILD(in int serial, in int clientId);

    /**
     * Route authentication information to modem
     *
     * @param uid Caller process id
     * @param msg Authentication message
     */
    void routeAuthMessage(in int serial, in int uid, in byte[] msg, in int clientId);

    /**
     * Route certificate information to modem
     *
     * @param uid Caller process id
     * @param cert Certificate
     * @param msg encrypted message
     */
    void routeCertificate(in int serial, in int uid, in byte[] cert, in byte[] msg, in int clientId);

    /**
     * This request passes raw byte arrays from ATCI.
     * Response function is IAtciResponse.sendAtciResponse()
     */
    void sendAtciRequest(in int serial, in byte[] data, in int clientId);

    /**
     * Send eMBMS Command String
     *
     * @param serial Serial number of request.
     * @param data command string
     *
     * Response function is IMtkRadioExResponse.sendEmbmsAtCommandResponse
     */
    void sendEmbmsAtCommand(in int serial, in String data, in int clientId);

    /**
     * This request passes raw byte arrays between framework and vendor code.
     * It's used to send AT command to modem for internal testing.
     *
     * @param serial Serial number of request.
     * @param data data passed as raw bytes
     *
     * Response function is IMtkRadioExResponse.sendRequestRawResponse()
     */
    void sendRequestRaw(in int serial, in byte[] data, in int clientId);

    /**
     * This request passes strings between framework and vendor code.
     * It's used to send AT command to modem for internal testing.
     *
     * @param serial Serial number of request.
     * @param data data passed as strings
     *
     * Response function is IMtkRadioExResponse.sendRequestStringsResponse()
     */
    void sendRequestStrings(in int serial, in String[] data, in int clientId);

    /**
     * Send sar indicator to modem.
     *
     * @parma serial Serail Number
     * @parma sar_cmd_type Indicator feature type.
     *                   Value: 0 AT+ERFIDX
     *                          1 AT+ERFTX
     * @param sar_parameter The parameter of SAR Command.
     *
     */
    void sendSarIndicator(in int serial, in int sar_cmd_type, in String sar_parameter, in int clientId);


    /**
     * set Max UL Speed to MD.
     *
     * @param serial Serial number of socket command
     * @param ulSpeed Max UL Speed
     */
    void setMaxUlSpeed(in int serial, in int ulSpeed, in int clientId);

    /**
     * Set Modem power
     *
     * @param serial Serial number of request.
     * @param isOn desired modem power.
     *
     * Response function is IMtkRadioExResponse.setsetModemPower
     */
    void setModemPower(in int serial, in boolean isOn, in int clientId);

   /**
     * Reset modem.
     *
     * @param serial Serial number of request
     * @param mode 1: framework just want an exception from modem;
     *             2: framework want reset modem.
     */
    void setTrm(in int serial, in int mode, in int clientId);

    /**
     * Set power limitation.
     *
     * @param serial Serial number of socket command
     * @param limitpower
     */
    void setTxPower(in int serial, in int limitpower, in int clientId);

    /**
     * Disable/enable report TXPWRSTUS URC
     *
     * @param serial Serial number of request.
     * @param 0 disable; 1 enable.
     *
     * Response function is IMtkRadioExResponse.setTxPowerStatusResponse
     */
    void setTxPowerStatus(in int serial, in int mode, in int clientId);

    /**
     * This request is used to set vendor setting
     *
     * @param serial Serial number of request.
     * @param setting setting name
     * @param value setting value
     *
     * Response function is IMtkRadioExResponse.setVendorSettingResponse()
     */
    void setVendorSetting(in int serial, in int setting, in String value, in int clientId);


    /**
     * Trigger mode switch by Ecc
     *
     * @param serial Serial number of request.
     * @param mode the target mode.
     *
     * Response function is IMtkRadioExResponse.triggerModeSwitchByEccResponse
     */
    void triggerModeSwitchByEcc(in int serial, in int mode, in int clientId);

    /**
     * Run Gba Authentication
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.runGbaAuthenticationResponse()
     */
    void runGbaAuthentication(in int serial, in String nafFqdn,
        in String nafSecureProtocolId, in boolean forceRun, in int netId, in int clientId);

    /**
     * Notify Wi-Fi association information to MD.
     *
     * @param serial Serial number of socket command
     * @param ifName Wi-Fi interface name
     * @param associated Wi-Fi association status
     * @param ssid Wi-Fi AP SSID
     * @param apMac Wi-Fi AP MAC address
     * @param mtuSize mtu size
     * @param ueMac UE MAC address
     */
    void sendWifiAssociated(in int serial, in String ifName, in int associated,
        in String ssid, in String apMac, in int mtuSize, in String ueMac, in int clientId);

    /**
     * Notify Wi-Fi enable/disable status to MD.
     *
     * @param serial Serial number of socket command
     * @param ifName Wi-Fi interface name
     * @param isEnabled Wi-Fi enable/disable status
     */
    void sendWifiEnabled(in int serial, in String ifName, in int isWifiEnabled, in int clientId);

    /**
     * Notify Wi-Fi IP address information to MD.
     *
     * @param serial Serial number of socket command
     * @param ifName Wi-Fi interface name
     * @param ipv4Addr IPV4 address
     * @param ipv6Addr IPV6 address
     * @param ipv4PrefixLen IPV4 prefix length
     * @param ipv6PrefixLen IPV6 prefix length
     * @param ipv4Gateway IPV4 gateway address
     * @param ipv6Gateway IPV6 gateway address
     * @param dnsCount DNS server count
     * @param dnsServers DNS server addresses string
     */
    void sendWifiIpAddress(in int serial, in String ifName, in String ipv4Addr,
        in String ipv6Addr, in int ipv4PrefixLen, in int ipv6PrefixLen, in String ipv4Gateway,
        in String ipv6Gateway, in int dnsCount, in String dnsServers, in int clientId);

    /**
     * Register Cellular Quality Report.
     *
     * @param serial Serial number of socket command
     * @param registerQuality enable/disable
     * @param type cellular quality type
     * @param thresholdValues Threshold values
     * @param time trigger peroid
     */
    void registerCellQltyReport(in int serial, in String registerQuality, in String type,
        in String thresholdValues, in String triggerTime, in int clientId);

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
    void setResponseFunctionsMtk(in IMtkRadioExModemResponse radioResponse,
        in IMtkRadioExModemIndication radioIndication);

    /**
     * Set IMS client response functions for mtk modem requests & modem indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsMtkIms(in IMtkRadioExModemResponse radioResponse,
        in IMtkRadioExModemIndication radioIndication);

    /**
     * Set response functions for capctrl requests.
     *
     * @param capRadioResponse Object containing response functions
     */
    void setResponseFunctionsCap(in IMtkRadioExCapRadioResponse capRadioResponse);

    /**
     * Set response functions for atci requests.
     *
     * @param atciResponseParam for atci response function
     * @param atciIndicationParam for atci indication function
     */
    void setResponseFunctionsForAtci(in IAtciResponse atciResponseParam, in IAtciIndication atciIndicationParam);

    /**
     * Set response functions for mtk engineermode radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsEm(in vendor.mediatek.hardware.mtkradioex.em.IEmRadioResponse radioResponse, in vendor.mediatek.hardware.mtkradioex.em.IEmRadioIndication radioIndication);

    /**
     * Set response functions for GBA requests.
     *
     * @param radioResponse for gba response function
     */
    void setResponseFunctionsGba(in IMtkRadioExModemResponse radioResponse);

    /**
     * Set response functions for mtk assist requests.
     *
     * @param radioResponse Object containing response functions
     */
    void setResponseFunctionsAssist(in vendor.mediatek.hardware.mtkradioex.assist.IAssistModemResponse radioResponse);

    /**
     * Sync some upper-layer events, such as VoIP call status,  to the modem so that the
     * modem can implement enhancements like power saving.
     *
     * @param serial ril-request serial
     * @param event  specific event type
     * @param status event status
     */
    void syncAppEventStatus(in int serial, in int event, in int status, in int clientId);
}
