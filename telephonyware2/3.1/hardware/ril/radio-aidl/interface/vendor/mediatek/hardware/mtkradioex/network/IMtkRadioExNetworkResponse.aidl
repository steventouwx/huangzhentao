// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.network;
import vendor.mediatek.hardware.mtkradioex.network.BandModeInfo;
import vendor.mediatek.hardware.mtkradioex.network.OperatorInfoWithAct;
import vendor.mediatek.hardware.mtkradioex.network.Lte1xRttCellInfo;
import vendor.mediatek.hardware.mtkradioex.network.LteData;
import vendor.mediatek.hardware.mtkradioex.network.SignalStrengthWithWcdmaEcio;

/**
 * Interface declaring unsolicited indications to subsidylock.
 */
@VintfStability
interface IMtkRadioExNetworkResponse {
    /**
     * The response of abortFemtocellList
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void abortFemtocellListResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void cancelAvailableNetworksResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void cfgA2offsetResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void cfgB1offsetResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of clearLteAvailableFile.
     *
     * @param info Response info struct containing response type, serial no. and error.
     *
     */
    oneway void clearLteAvailableFileResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void deactivateNrScgCommunicationResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of setCarrierAggregationMode
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setCarrierAggregationModeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
    oneway void enableCAPlusBandWidthFilterResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void enableSCGfailureResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of get4x4MimoEnabled
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param enabled_bitmask bitmask for 4x4Mimo.
     */
    oneway void get4x4MimoEnabledResponse(in android.hardware.radio.RadioResponseInfo info,
        in int enabled_bitmask);

    /**
     * The response of getAllBandMode.
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param data All bands about GSM, UMTS, LTE and NR, Refer to @BandModeInfo.
     */
    oneway void getAllBandModeResponse(in android.hardware.radio.RadioResponseInfo info,
        in BandModeInfo data);

    /**
     * Response for getApcInfo request.
     */
    oneway void getApcInfoResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] cellInfo);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param networkInfos List of network operator information as OperatorInfoWithAct defined in
     *         types.hal
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void getAvailableNetworksWithActResponse(
        in android.hardware.radio.RadioResponseInfo info,
        in OperatorInfoWithAct[] networkInfosWithAct);

    /**
     * The response of getBandMode.
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param data  The data to response. There are 8 int32_t values:
     *    data[0]: gsmBand  bit1: EGSM900; bit3: DCS1800; bit4: PCS1900; bit7: GSM850.
     *    data[1]: umtsBand please see AT doc for detail.
     *    data[2]: lteBand1to32 lte band bit 1 to bit 32. 0x000001 means bit 1 is enable while 0x80000000 means bit 32 is enable.
     *    data[3]: lteBand33to64 lte band bit 33 to bit 64. 0x000001 means bit 33 is enable while 0x80000000 means bit 64 is enable.
     *
     */
    oneway void getBandModeResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] data);

    /**
     * The response of getBandPriorityList
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param bandPriList band id array in priority order
     */
    oneway void getBandPriorityListResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] bandPriList);

    /**
     * The response of getCALinkCapabilityList
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param linkCapabilityList a string[] without duplicate element, each element is a string.
     */
    oneway void getCALinkCapabilityListResponse(in android.hardware.radio.RadioResponseInfo info,
        in String[] linkCapabilityList);

    /**
     * The response of getCALinkEnableStatus
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param status true if the combo is enabled.
     */
    oneway void getCALinkEnableStatusResponse(in android.hardware.radio.RadioResponseInfo info,
        in boolean status);

    /**
     * The response of getCaBandMode.
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param data The data to response. There are 8 int32_t values:
     *    data[0]: bit1-bit32; data[1]: bit33-bit64; data[2]: bit65-bit96; data[3]: bit97-bit128;
     *    data[4]: bit129-bit160; data[5]: bit161-bit192; data[6]: bit193-bit224; data[7]: bit225-bit256;
     *
     */
    oneway void getCaBandModeResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] data);

    /**
     * The response of  getCampedFemtoCellInfo.
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param femto a list contain the camped femto cell info.
     */
    oneway void getCampedFemtoCellInfoResponse(in android.hardware.radio.RadioResponseInfo info,
        in String[] femto);

    /**
     * The response of getCurrentPOLList
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param polList the SIM/USIM preferred operator list
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void getCurrentPOLListResponse(in android.hardware.radio.RadioResponseInfo responseInfo,
        in String[] polList);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void getDeactivateNrScgCommunicationResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo, in int deactivate,
        in int allowSCGAdd);

    /**
     * The response of getDisable2G
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param mode Disable2G is enabled when it is true.
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void getDisable2GResponse(in android.hardware.radio.RadioResponseInfo responseInfo,
        in int mode);


    /**
     * The response of getFemtocellList
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param femto List from network
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void getFemtocellListResponse(in android.hardware.radio.RadioResponseInfo responseInfo,
        in String[] femtoList);

    /**
     * The response of getIWlanRegistrationState.
     *
     * @param responseInfo struct containing response type, serial no, and error.
     * @param state        the IWlan registration state.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:REQUEST_NOT_SUPPORTED
     *   RadioError:INTERNAL_ERR
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getIWlanRegistrationStateResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo, in int state);

    /**
     * The response of getLteAvailableFiles.
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param list  the list of Lte1xRttCellInfo.
     *
     */
    oneway void getLte1xRttCellListResponse(in android.hardware.radio.RadioResponseInfo info,
        in Lte1xRttCellInfo[] list);

    /**
     * The response of getLteBsrTimer.
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param timer LTE BSR Timer.
     *
     */
    oneway void getLteBsrTimerResponse(in android.hardware.radio.RadioResponseInfo info,
        in int timer);

    /**
     * The response of getLteData
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param data lte related information.
     */
    oneway void getLteDataResponse(in android.hardware.radio.RadioResponseInfo info,
        in LteData data);

    /**
     * The response of getLteRRCState
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param state lte rrc state.
     *   state = 0x00, lte rrc state idle
     *   state = 0x01, lte rrc state scanning
     *   state = 0x02, lte rrc state connected
     *   state = 0x03, lte rrc state disconnected
     *   state = 0xFF, lte rrc state error
     */
    oneway void getLteRRCStateResponse(in android.hardware.radio.RadioResponseInfo info,
        in int state);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param mode setting mode of lte release version.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getLteReleaseVersionResponse(in android.hardware.radio.RadioResponseInfo info,
        in int mode);

    /**
     * The response of getLteScanResponse.
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param duration LTE scan duration in sec.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getLteScanDurationResponse(in android.hardware.radio.RadioResponseInfo info,
        in int duration);

    /**
     * The response of getPOLCapability
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param polCapability the order number of operator in the SIM/USIM preferred operator list
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void getPOLCapabilityResponse(in android.hardware.radio.RadioResponseInfo responseInfo,
        in int[] polCapability);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param string containing operator name
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
    oneway void getPlmnNameFromSE13TableResponse(in android.hardware.radio.RadioResponseInfo info,
        in String name);

    /**
     * The response of getQamEnabled.
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param ulOrDl Uplink or downlink, true for uplink, false for downlink.
     * @param enabled QAM enabled status.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
    oneway void getQamEnabledResponse(in android.hardware.radio.RadioResponseInfo info,
        in boolean ulOrDl, in boolean enabled);

    /**
     * Response for getRoamingEnable request.
     *
     * @param responseInfo Response info struct containing response type, serial no. and error
     * @param data         the the configuration of roaming
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getRoamingEnableResponse(in android.hardware.radio.RadioResponseInfo responseInfo,
        in int[] data);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param SignalStrength with wcdma ecio
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void getSignalStrengthWithWcdmaEcioResponse(
        in android.hardware.radio.RadioResponseInfo info,
        in SignalStrengthWithWcdmaEcio signalStrength);

    /**
     * The response of getSuggestedPlmnList
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param plmnList the suggested PLMN list
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void getSuggestedPlmnListResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo, in String[] plmnList);

    /**
     * The response of getTOEInfo
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param longName is long NITZ oprator name or empty string if received.
     * @param shortName is short NITZ oprator name or empty string if received.
     * @param longName is 5 or 6 digit numeric code (mcc + mnc) or empty string if received.
     */
    oneway void getTOEInfoResponse(in android.hardware.radio.RadioResponseInfo info,
        in String longName, in String shortName, in String numeric);

    /**
     * The response of getTm9Enabled.
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param fddOrTdd FDD or TDD, true for FDD, false for TDD.
     * @param enabled TM9 enabled status.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getTm9EnabledResponse(in android.hardware.radio.RadioResponseInfo info,
        in boolean fddOrTdd, in boolean enabled);


    /**
     * The response of queryFemtoCellSystemSelectionMode
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param femtocell system selection mode
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void queryFemtoCellSystemSelectionModeResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo, in int mode);


    /**
     * The response of selectFemtocell
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void selectFemtocellResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of set4x4MimoEnabled
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void set4x4MimoEnabledResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for setApcMode request.
     */
    oneway void setApcModeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of setBandPriorityList
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setBandPriorityListResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void setBgsrchDeltaSleepTimerResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of setCALinkEnableStatus
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setCALinkEnableStatusResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * The response of setDisable2G
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void setDisable2GResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of setFemtoCellSystemSelectionMode
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void setFemtoCellSystemSelectionModeResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of setLteBandEnableStatus
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setLteBandEnableStatusResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of setLteBsrTimer.
     *
     * @param info Response info struct containing response type, serial no. and error.
     *
     */
    oneway void setLteBsrTimerResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setLteReleaseVersionResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of setLteScanDuration.
     *
     * @param info Response info struct containing response type, serial no. and error.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setLteScanDurationResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void setNROptionResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void setNetworkSelectionModeManualWithActResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of setNrBandMode.
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setNrBandModeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of setPOLEntryResponse
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void setPOLEntryResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of setQamEnabled.
     *
     * @param info Response info struct containing response type, serial no. and error.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
    oneway void setQamEnabledResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void setSearchRatResponse(in android.hardware.radio.RadioResponseInfo responseInfo);


    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void setSearchStoredFreqInfoResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void setServiceStateToModemResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of setTm9Enabled.
     *
     * @param info Response info struct containing response type, serial no. and error.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setTm9EnabledResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for setRoamingEnable request.
     *
     * @param responseInfo Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
   oneway void setRoamingEnableResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for getNitzTime request.
     *
     * @param responseInfo Response info struct containing response type, serial no. and error
     * @param nitzTime NITZ time string in the form "yy/mm/dd,hh:mm:ss(+/-)tz,dt"
     * @param tz_valid Whether time_zone_offset_minutes in INFO field is valid; 0: invalid; 1: valid
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
   oneway void getNitzTimeResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo, in String nitzTime, int tz_valid);
}
