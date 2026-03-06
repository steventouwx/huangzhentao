// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.network;

import android.hardware.radio.RadioResponseInfo;
import vendor.mediatek.hardware.mtkradioex.network.SignalStrengthWithWcdmaEcio;

/**
 * Interface declaring response functions to subsidylock requests.
 */
@VintfStability
interface IMtkRadioExNetworkIndication {
    /**
     * Indicates of signal strength witch wcdma ecio
     * @param type Type of radio indication
     * @param signalStrength signal strength with wcdma ecio
     */
    oneway void currentSignalStrengthWithWcdmaEcioInd(
        in android.hardware.radio.RadioIndicationType type,
        in SignalStrengthWithWcdmaEcio signalStrength);

    /**
     * Notify Band Information
     *
     * @param type Type of radio indication
     * @param state band information
     */
    oneway void networkBandInfoInd(in android.hardware.radio.RadioIndicationType type,
        in int[] state);

    /**
     * Indicates of network info
     *
     * @param type Type of radio indication
     * @param networkinfo network info
     */
    oneway void networkInfoInd(in android.hardware.radio.RadioIndicationType type,
        in String[] networkinfo);


    /**
     * Notification for NR CA Band change
     *
     * @param type Type of radio indication
     * @param band SA bands
     */
    oneway void nrCaBandChangeInd(in android.hardware.radio.RadioIndicationType type,
        in int[] bands);

    /**
     * Indicates when NR 5G system information changed.
     *
     * @param type Type of radio indication
     * @param nrSysInfos NR 5G System information
     */
    oneway void nrSysInfoInd(in android.hardware.radio.RadioIndicationType type,
        in int[] nrSysInfos);

    /**
     * Notification for 5GUW Info
     *
     * @param type Type of radio indication
     * @param data is 5GUW info.
     * data[0]: display_5GUW. 0: don't display; 1: display.
     * data[1]: on_n77_band. 0: not on n77 band; 1: on n77 band.
     * data[2]: on_fr2_band. 0: not on fr2 band; 1: on fr2 band.
     * data[3]: 5guw_allowed. 0: not allowed; 1: allowed.
     */
    oneway void on5GUWInfoInd(in android.hardware.radio.RadioIndicationType type, in int[] data);


    /**
     * Indicates when current resident network mccmnc changes
     * URC: RIL_UNSOL_MCCMNC_CHANGED
     *
     * @param type Type of radio indication
     * @param mccmnc current resident network mcc & mnc
     */
    oneway void onMccMncChanged(in android.hardware.radio.RadioIndicationType type,
        in String mccmnc);

    /**
     * Notification for network config info
     *
     * @param type Type of radio indication
     * @param mimo MIMO 4x4
     * @param qam_256 256 QAM
     * @param qam_ul64 64 QAM UL
     */
    oneway void onNwCfgInfoInd(in android.hardware.radio.RadioIndicationType type, in boolean mimo,
        in boolean qam_256, in boolean qam_ul64);


    /**
     * Notification for rrc connection state change
     *
     * @param type Type of radio indication
     * @param rat Rat type
     * @param state Rrc connection state
     */
    oneway void onNwRrcStateInd(in android.hardware.radio.RadioIndicationType type, in int rat,
        in int state);

    /**
     * Indicates of pseudo cell info
     * URC: RIL_UNSOL_PSEUDO_CELL_INFO
     *
     * @param type Type of radio indication
     * @param cellInfo pseudo cell info
     */
    oneway void onPseudoCellInfoInd(in android.hardware.radio.RadioIndicationType type,
        in int[] cellInfo);


    /**
     * Indicates when CS state changes.
     *
     * @param type Type of radio indication
     * @param Info register state
     * @param Info lac
     * @param Info ci
     * @param Info Act
     * @param Info reject cause
     * @param Info cdma network existence
     */
    oneway void responseCsNetworkStateChangeInd(in android.hardware.radio.RadioIndicationType type,
        in String[] state);

    /**
     * Indicates the femtocell information.
     *
     * @param information of femtocell
     */
    oneway void responseFemtocellInfo(in android.hardware.radio.RadioIndicationType type,
        in String[] info);

    /**
     * Indicates LTE network info.
     *
     * @param type Type of radio indication
     * @param lteBand Serving LTE band
     */
    oneway void responseLteNetworkInfo(in android.hardware.radio.RadioIndicationType type,
        in int lteBand);

    /**
     * Indicates for modulation info
     *
     * @param type Type of radio indication
     * @param info modulation
     */
    oneway void responseModulationInfoInd(in android.hardware.radio.RadioIndicationType type,
        in int[] modulation);

    /**
     * Indicates for RAU/TAU enevt
     *
     * @param type Type of radio indication
     * @param info Event
     */
    oneway void responseNetworkEventInd(in android.hardware.radio.RadioIndicationType type,
        in int[] event);

    /**
     * Indicates when PS state changes.
     *
     * @param type Type of radio indication
     * @param Info register state
     * @param Info mcc/mnc
     * @param Info Act
     */
    oneway void responsePsNetworkStateChangeInd(in android.hardware.radio.RadioIndicationType type,
        in int[] state);


    /**
     * Notification for sib16 time Info
     *
     * @param type Type of radio indication
     * @param sib16Time is sib16 time.
     * @param receivedTime is sib16 time received time.
     */
    oneway void sib16TimeInfoInd(in android.hardware.radio.RadioIndicationType type,
        in String sib16Time, in long receivedTime);


    /**
     * Notification for TOE Info
     *
     * @param type Type of radio indication
     * @param longName is long NITZ oprator name if received.
     * @param shortName is short NITZ oprator name if received.
     * @param longName is 5 or 6 digit numeric code (mcc + mnc) if received.
     */
    oneway void toeInfoInd(in android.hardware.radio.RadioIndicationType type, in String longName,
        in String shortName, in String numeric);

    /**
     * Notification for IWlan registration state change
     *
     * @param type Type of radio indication
     * @param state IWlan registration state
     */
    oneway void iwlanRegistrationStateInd(in android.hardware.radio.RadioIndicationType type,
        in int state);
}
