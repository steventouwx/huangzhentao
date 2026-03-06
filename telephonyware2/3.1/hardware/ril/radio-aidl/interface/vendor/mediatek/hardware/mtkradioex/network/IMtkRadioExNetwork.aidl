// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.network;

import android.hardware.radio.RadioResponseInfo;
import vendor.mediatek.hardware.mtkradioex.network.IMtkRadioExNetworkResponse;
import vendor.mediatek.hardware.mtkradioex.network.IMtkRadioExNetworkIndication;

import vendor.mediatek.hardware.mtkradioex.smartratswitch.ISmartRatSwitchRadioResponse;
import vendor.mediatek.hardware.mtkradioex.smartratswitch.ISmartRatSwitchRadioIndication;

/**
 * Interface declaring response functions to subsidylock requests.
 */
@VintfStability
oneway interface IMtkRadioExNetwork {
    /**
     * Abort Femtocell List
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.abortFemtocellListResponse
     */
    void abortFemtocellList(in int serial, in int clientId);

    /**
     * cancel scan available networks
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.cancelAvailableNetworksResponse()
     */
    void cancelAvailableNetworks(in int serial, in int clientId);

    /**
     * Set A2 config offset to MD.
     *
     * @param serial Serial number of socket command
     * @param offset
     * @param threshBound
     */
    void cfgA2offset(in int serial, in int offset, in int threshBound, in int clientId);

    /**
     * Set B2 config offset to MD.
     *
     * @param serial Serial number of socket command
     * @param offset
     * @param threshBound
     */
    void cfgB1offset(in int serial, in int offset, in int threshBound, in int clientId);


    /**
     * Clear LTE Available file.
     *
     * @param serial Serail Number.
     *
     * Response function is IMtkRadioExResponse.clearLteAvailableFileResponse().
     */
    void clearLteAvailableFile(in int serial, in int clientId);

    /**
     * set SCG communication to MD.
     *
     * @param serial Serial number of socket command
     * @param deactivate
     * @param allowSCGAdd
     */
    void deactivateNrScgCommunication(in int serial, in boolean deactivate,
        in boolean allowSCGAdd, in int clientId);

    /**
     * set Carrier Aggregation Mode
     *
     * @param serial Serail Number
     * @param mode:
     *    mode = 0, turn off CA
     *    mode = 1, turn on CA
     * @param option:
     *    option = 0, LTE CA
     *    option = 1, SA CA
     *    option = 2, NSA CA
     * @param linkType:
     *    linkType = 0, ca link type download
     *    linkType = 1, ca link type upload
     *    linkType = 2, ca link type both download and upload
     *
     * Response function is IMtkRadioExResponse.setCarrierAggregationModeResponse()
     */
    void setCarrierAggregationMode(in int serial, in int mode, in int option, in int linkType, in int clientId);


    /**
     * Return true on success; false on any failure.
     *
     * @param enable: enable/disable CA+ bandwidth filter
     *
     * Response function is IRadioResponse.enableCAPlusBandWidthFilterResponse()
     */
    void enableCAPlusBandWidthFilter(in int serial, in boolean enable, in int clientId);

    /**
     * enable T1/T2/T3 time to MD to disable NR capability if SCG faiure.
     *
     * @param serial Serial number of socket command
     * @param T1
     * @param T2
     * @param T3
     */
    void enableSCGfailure(in int serial, in boolean enable, in int T1, in int P1,
        in int T2, in int clientId);

    /**
     * Get 4x4Mimo enabled
     *
     * @param serial Serail Number
     *
     * Response function is IMtkRadioExResponse.get4x4MimoEnabled()
     */
    void get4x4MimoEnabled(in int serial, in int clientId);


    /**
     * Get current band mode.
     *
     * @param serial Serail Number.
     *
     * Response function is IMtkRadioExResponse.getAllBandModeResponse().
     */
    void getAllBandMode(in int serial, in int clientId);

    /**
     * Get detected pseudo cell
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getApcInfoResponse
     */
    void getApcInfo(in int serial, in int clientId);

    /**
     * Scans for available networks with ACT
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getAvailableNetworksWithActResponse()
     */
    void getAvailableNetworksWithAct(in int serial, in int clientId);


    /**
     * Get current band mode.
     *
     * @param serial Serail Number.
     *
     * Response function is IMtkRadioExResponse.getBandModeResponse().
     */
    void getBandMode(in int serial, in int clientId);

    /**
     * Get band priority list
     *
     * @param serial Serail Number
     *
     * Response function is IMtkRadioExResponse.getBandPriorityListResponse()
     */
    void getBandPriorityList(in int serial, in int clientId);

    /**
     * Get ca link capability list by linkType
     *
     * @param serial Serail Number
     * @param linkType:
     *    linkType = 0, ca link type download
     *    linkType = 1, ca link type upload
     *    linkType = 2, ca link type both download and upload
     *
     * Response function is IMtkRadioExResponse.getCALinkCapabilityListResponse()
     */
    void getCALinkCapabilityList(in int serial, in int linkType, in int clientId);

    /**
     * Get ca enable status by linkType
     *
     * @param serial Serail Number
     * @param bandsCombo bandsCombo
     * @param linkType:
     *    linkType = 0, ca link type download
     *    linkType = 1, ca link type upload
     *    linkType = 2, ca link type both download and upload
     *
     * Response function is IMtkRadioExResponse.getCALinkEnableStatusResponse()
     */
    void getCALinkEnableStatus(in int serial, in String bandsCombo, in int linkType, in int clientId);

    /**
     * Get current CA band mode.
     *
     * @param serial Serail Number.
     * @param primaryBandId primary band id.
     *
     * Response function is IMtkRadioExResponse.getCaBandModeResponse().
     */
    void getCaBandMode(in int serial, in int primaryBandId, in int clientId);

    /**
     * Get Camped femto cell info.
     *
     * @param serial Serail Number.
     *
     * Response function is IMtkRadioExResponse.getCampedFemtoCellInfoResponse().
     */
    void getCampedFemtoCellInfo(in int serial, in int clientId);

    /**
     * Get Preferred Operator List
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getCurrentPOLListResponse
     */
    void getCurrentPOLList(in int serial, in int clientId);


    /**
     * get SCG communication state.
     *
     * @param serial Serial number of socket command
     *
     * Response function is IMtkRadioExResponse.getDeactivateNrScgCommunicationResponse
     */
    void getDeactivateNrScgCommunication(in int serial, in int clientId);

    /**
     * Get Disable 2G
     * @param serial Serial number of request.
     *
     * Response function is IRadioResponse.getDisable2GResponse
     */
    void getDisable2G(in int serial, in int clientId);

    /**
     * Get Femtocell List
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getFemtocellListResponse
     */
    void getFemtocellList(in int serial, in int clientId);


    /**
     * Query function for IWlan registration state.
     * @param serial Serial number of request.
     *
     */
    void getIWlanRegistrationState(in int serial, in int clientId);

    /**
     * Get LTE available or unavailable 1xRTT Cell list.
     *
     * @param serial Serail Number.
     * @param available true: available, false: unavailable.
     *
     * Response function is IMtkRadioExResponse.getLte1xRttCellListResponse().
     */
    void getLte1xRttCellList(in int serial, in boolean available, in int clientId);

    /**
     * Get LTE BSR Timer.
     *
     * @param serial Serail Number.
     *
     * Response function is IMtkRadioExResponse.getLteBsrTimerResponse().
     */
    void getLteBsrTimer(in int serial, in int clientId);

    /**
     * Get lte related information
     *
     * @param serial Serail Number
     *
     * Response function is IMtkRadioExResponse.getLteDataResponse()
     */
    void getLteData(in int serial, in int clientId);

    /**
     * Get lte rrc(radio resource control) state
     *
     * @param serial Serail Number
     *
     * Response function is IMtkRadioExResponse.getLteRRCStateResponse()
     */
    void getLteRRCState(in int serial, in int clientId);

    /**
     * Query lte release version.
     *
     * @param serial Serial number of request.
     *
     * Response callback is IMtkRadioExResponse.getLteReleaseVersionResponse()
     */
    void getLteReleaseVersion(in int serial, in int clientId);

    /**
     * Get LTE scan duration.
     *
     * @param serial Serial number of request.
     * @param duration LTE scan duration in sec.
     *
     * Response function is IMtkRadioExResponse.getLteScanDurationResponse().
     *
     */
    void getLteScanDuration(in int serial, in int clientId);

    /**
     * Get Preferred Operator List Capability
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getPOLCapabilityResponse
     */
    void getPOLCapability(in int serial, in int clientId);


    /**
     * Return string value indicating the operator name of MCCMNC
     *
     * @param serial Serial number of request
     *
     * Response function is IRadioResponse.getPlmnNameFromSE13TableResponse()
     */
    void getPlmnNameFromSE13Table(in int serial, in int mcc, in int mnc, in int clientId);

    /**
     * Get QAM enabled status.
     *
     * @param serial Serial number of request.
     * @param ulOrDl Uplink or downlink, true for uplink, false for downlink.
     *
     * Response function is IMtkRadioExResponse.getQamEnabledResponse().
     *
     */
    void getQamEnabled(in int serial, in boolean ulOrDl, in int clientId);

    /**
     * Get the roaming enabling.
     *
     * @param serial Serial number of request.
     * @param phoneId the id of the phone.
     *
     * Response function is IMtkRadioExResponse.getRoamingEnableResponse
     */
    void getRoamingEnable(in int serial, in int phoneId, in int clientId);

    /**
     * get signal strength with wcdma ecio
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getSignalStrengthWithWcdmaEcioResponse()
     */
    void getSignalStrengthWithWcdmaEcio(in int serial, in int clientId);

    /**
     * Get smart rat switch
     *
     * @param serial Serial number of request
     * @param index switch mode
     */
    void getSmartRatSwitch(in int serial, in int mode, in int clientId);

   /**
     * Get suggested PLMN list for VSIM
     *
     * @param rat 0: GSM, 1: UMTS, 3: LTE
     * @param num maximum number of plmn in a result
     * @param timer maximum search time
     *
     * Response function is IMtkRadioExRadioResponse.getSuggestedPlmnListResponse
     */
    void getSuggestedPlmnList(in int serial, in int rat, in int num, in int timer, in int clientId);

    /**
     * Get TOE info
     *
     * @param serial Serail Number
     *
     * Response function is IMtkRadioExResponse.getTOEInfoResponse()
     */
    void getTOEInfo(in int serial, in int clientId);

    /**
     * Get transmission mode 9 enabled status.
     *
     * @param serial Serial number of request.
     * @param fddOrTdd FDD or TDD, true for FDD, false for TDD.
     * @param enabled TM9 enabled status, true for enabled, false for disabled.
     *
     * Response function is IMtkRadioExResponse.getTm9EnabledResponse().
     *
     */
    void getTm9Enabled(in int serial, in boolean fddOrTdd, in int clientId);


    /**
     * Query femtoCell system selection mode
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.queryFemtoCellSystemSelectionModeResponse
     */
    void queryFemtoCellSystemSelectionMode(in int serial, in int clientId);

    /**
     * Select femtocell
     *
     * @param serial Serial number of request.
     * @param operatorNumeric of femtocell
     * @param act preferred radio access type
     * @param csgId femtocell id
     *
     * Response function is IMtkRadioExResponse.selectFemtocellResponse
     */
    void selectFemtocell(in int serial, in String operatorNumeric, in String act,
        in String csgId, in int clientId);
    /**
     * Set 4x4Mimo enabled
     *
     * @param serial Serail Number
     * @param enabled_bitmask bitmask for 4x4Mimo
     *
     * Response function is IMtkRadioExResponse.set4x4MimoEnabled()
     */
    void set4x4MimoEnabled(in int serial, in int enabled_bitmask, in int clientId);

    /**
     * Set APC mode
     *
     * @param serial Serial number of request.
     * @param mode APC mode.
     * @param reportMode URC report on or off.
     * @param interVal URC report interval.
     *
     * Response function is IMtkRadioExResponse.setApcModeResponse
     */
    void setApcMode(in int serial, in int mode, in int reportMode, in int interval, in int clientId);

    /**
     * Set band priority list
     *
     * @param serial Serail Number
     * @param bandPriList band id array in priority order, maximum numbe of bands that can be set is 15.
     *
     * Response function is IMtkRadioExResponse.setBandPriorityListResponse()
     */
    void setBandPriorityList(in int serial, in int[] bandPriList, in int clientId);

    /**
     * Set backbround search sleep timer.
     *
     * @param serial Serial number of socket command
     * @param sleep_delta_duration
     */
    void setBgsrchDeltaSleepTimer(in int serial, in int sleepDuration, in int clientId);

    /**
     * Set ca enable status by linkType
     *
     * @param serial Serail Number
     * @param status enable/disable
     * @param bandsCombo bandsCombo
     * @param linkType:
     *    linkType = 0, ca link type download
     *    linkType = 1, ca link type upload
     *    linkType = 2, ca link type both download and upload
     *
     * Response function is IMtkRadioExResponse.setCALinkEnableStatusResponse()
     */
    void setCALinkEnableStatus(in int serial, in boolean status, in String bandsCombo,
        in int linkType, in int clientId);

    /**
     * Set Disable 2G
     * @param serial Serial number of request.
     * @param mode on or off
     *
     * Response function is IRadioResponse.setDisable2GResponse
     */
    void setDisable2G(in int serial, in boolean mode, in int clientId);

    /**
     * Set femto Cell system selection mode
     *
     * @param serial Serial number of request.
     * @param mode of femtocell selection
     *
     * Response function is IMtkRadioExResponse.setFemtoCellSystemSelectionModeResponse
     */
    void setFemtoCellSystemSelectionMode(in int serial, in int mode, in int clientId);

    /**
     * Set lte band config
     *
     * @param serial Serail Number
     * @param bandId bandId which starts from 1
     * @param status enable/disable
     *
     * Response function is IMtkRadioExResponse.setLteBandEnableStatusResponse()
     */
    void setLteBandEnableStatus(in int serial, in int bandId, in boolean status, in int clientId);

    /**
     * Set LTE BSR Timer.
     *
     * @param serial Serail Number.
     * @param timer The BSR timer to set.
     *
     * Response function is IMtkRadioExResponse.SetLteBsrTimerResponse().
     */
    void setLteBsrTimer(in int serial, in int timer, in int clientId);

    /**
     * Requests to set lte release version.
     *
     * @param serial Serial number of request.
     * @param mode setting mode of lte release version.
     *
     * Response callback is IMtkRadioExResponse.setLteReleaseVersionResponse()
     */
    void setLteReleaseVersion(in int serial, in int mode, in int clientId);

    /**
     * Set LTE scan duration.
     *
     * @param serial Serial number of request.
     * @param duration LTE scan duration in sec.
     *
     * Response function is IMtkRadioExResponse.setLteScanDurationResponse().
     *
     */
    void setLteScanDuration(in int serial, in int duration, in int clientId);

    /**
     * Set NR capability.
     *
     * @param serial Serial number of socket command
     * @param option: 1: LTE ONLY(disable NSA+SA); 2: SA without LTE 3: SA with LTE; 5: NSA ONLY; 7: NSA+SA
     */
    void setNROption(in int serial, in int option, in int clientId);


    /**
     * Manually select a specified network with Act.
     * This request must not respond until the new operator is selected and registered.
     *
     * @param serial Serial number of request.
     * @param operatorNumeric string specifying MCCMNC of network to select (eg "310170")
     * @param act string specifying act of network to select (eg "7" is LTE)
     * @param mode string specifying semi auto mode or not
     *
     * Response function is IMtkRadioExResponse.setNetworkSelectionModeManualWithActResponse()
     */
    void setNetworkSelectionModeManualWithAct(in int serial, in String operatorNumeric,
        in String act, in String mode, in int clientId);

    /**
     * Set all band mode.
     *
     * @param serial Serail Number.
     * @param saEnable bands enabled for SA.
     * @param saDisable bands disabled for SA.
     * @param nsaEnable bands enabled for NSA.
     * @param nsaDisable bands disabled for NSA.
     *
     * Response function is IMtkRadioExResponse.setNrBandModeResponse().
     */
    void setNrBandMode(in int serial, in int[] saEnable, in int[] saDisable,
        in int[] nsaEnable, in int[] nsaDisable, in int clientId);

    /**
     * Set Preferred Operator List
     *
     * @param serial Serial number of request.
     * @param index index of POL
     * @param numeric operator's mccmnc
     * @param nAct preferred radio access type
     *
     * Response function is IMtkRadioExResponse.setPOLEntryResponse
     */
    void setPOLEntry(in int serial, in int index, in String numeric, in int nAct, in int clientId);

    /**
     * Enable/Disable QAM status.
     *
     * @param serial Serial number of request.
     * @param ulOrDl Uplink or downlink, true for uplink, false for downlink.
     * @param enabled QAM status, true for enabled, false for disabled.
     *
     * Response function is IMtkRadioExResponse.setQamEnabledResponse().
     *
     */
    void setQamEnabled(in int serial, in boolean ulOrDl, in boolean enabled, in int clientId);


    /**
     * Set search rat.
     *
     * @param serial Serial number of socket command
     * @param rat_array
     * @param size for rat array
     */
    void setSearchRat(in int serial, in int[] rat, in int clientId);

    /**
     * Set search stored frenquency information.
     *
     * @param serial Serial number of socket command
     * @param operation
     * @param plmn_id
     * @param rat
     * @param freq_num
     * @param freq
     */
    void setSearchStoredFreqInfo(in int serial, in int operation, in int plmn_id,
        in int rat, in int[] freq, in int clientId);

    /**
     * Notify SST registration infromation to MD.
     *
     * @param serial Serial number of socket command
     * @param phoneId SIM index
     * @param voiceRegState Voice registration state
     * @param dataRegState Data registration state
     * @param voiceRoamingType Voice roaming type
     * @param dataRoamingType Data romaing type
     * @param rilVoiceRegState RIL voice registration state
     * @param rilDataRegState RIL data registration state
     */
    void setServiceStateToModem(in int serial, in int voiceRegState, in int dataRegState,
        in int voiceRoamingType, in int dataRoamingType, in int rilVoiceRegState,
        in int rilDataRegState, in int clientId);

    /**
     * Enable/Disable transmission mode 9 status.
     *
     * @param serial Serial number of request.
     * @param fddOrTdd FDD or TDD, true for FDD, false for TDD.
     * @param enabled TM9 status, true for enabled, false for disabled.
     *
     * Response function is IMtkRadioExResponse.setTm9EnabledResponse().
     *
     */
    void setTm9Enabled(in int serial, in boolean fddOrTdd, in boolean enabled, in int clientId);

    /**
     * Smart rat switch
     *
     * @param serial Serial number of request
     * @param index switch mode
     * @param index rat type
     */
    void smartRatSwitch(in int serial, in int mode, in int rat, in int clientId);

    /**
     * Set the roaming enabling.
     *
     * @param serial Serial number of request.
     * @param config the configuration.
     *  config[0]: phone id
     *  config[1]: international_voice_text_roaming (0,1)
     *  config[2]: international_data_roaming (0,1)
     *  config[3]: domestic_voice_text_roaming (0,1)
     *  config[4]: domestic_data_roaming (0,1)
     *  config[5]: domestic_LTE_data_roaming (1)
     *
     * Response function is IMtkRadioExResponse.setRoamingEnableResponse
     */
    void setRoamingEnable(in int serial, in int[] config, in int clientId);

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
    void setResponseFunctionsMtk(in IMtkRadioExNetworkResponse radioResponse,
        in IMtkRadioExNetworkIndication radioIndication);

    /**
     * Set response functions for telephony smart switch radio requests & radio indications.
     *
     * @param radioResponse Object containing smart switch response functions
     * @param radioIndication Object containing smart switch  indications
     */
   void setResponseFunctionsSmartRatSwitch(in ISmartRatSwitchRadioResponse radioResponse,
       in ISmartRatSwitchRadioIndication radioIndication);

    /**
     * This command is used to acquire NITZ information (UTC time and time zone).
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getNitzTimeResponse
     */
    void getNitzTime(in int serial, in int clientId);
}
