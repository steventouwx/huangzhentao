// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.data;
import vendor.mediatek.hardware.mtkradioex.data.DedicateDataCall;
import vendor.mediatek.hardware.mtkradioex.data.PlmnMvnoInfo;
import vendor.mediatek.hardware.mtkradioex.data.PcoDataAttachedInfo;
import vendor.mediatek.hardware.mtkradioex.data.UrspRule;

/**
 * Interface declaring response functions to subsidylock requests.
 */
@VintfStability
interface IMtkRadioExDataIndication {
    /**
     * Notify application for the dedicated bearer data.
     *
     * @param type Type of radio indication
     * @param DedicateDataCall as defined in types.hal
     */
    oneway void dedicatedBearerActivationInd(in android.hardware.radio.RadioIndicationType type,
        in DedicateDataCall ddcData);

    /**
     * Notify application for the dedicated bearer data.
     *
     * @param type Type of radio indication
     * @param cid Bearer Id
     */
    oneway void dedicatedBearerDeactivationInd(in android.hardware.radio.RadioIndicationType type,
       in int cid);

    oneway void dedicatedBearerModificationInd(in android.hardware.radio.RadioIndicationType type,
           in DedicateDataCall ddcData);

    /**
     * Notify Mobile Data Usage Information
     *
     * @param type Type of radio indication
     * @param data <TxBytes><TxPackets><RxBytes><RxPackets>
     */
    oneway void mobileDataUsageInd(in android.hardware.radio.RadioIndicationType type,
       in int[] data);

    /**
     * DSDA changed indication
     *
     * @param type Type of radio indication
     * @param mode The DSDA mode
     */
    oneway void onDsdaChangedInd(in android.hardware.radio.RadioIndicationType type, in int mode);

    /**
     * Indicates that modem has reset the data retry count
     *
     * @param type Type of radio indication
     */
    oneway void onMdDataRetryCountReset(in android.hardware.radio.RadioIndicationType type);

    /**
     * Notify PLMN Data Information
     *
     * @param type Type of radio indication
     * @param PlmnMvnoInfo as defined in types.hal
     */
    oneway void onPlmnDataInd(in android.hardware.radio.RadioIndicationType type,
        in PlmnMvnoInfo plmnMvnoInfo);

    /**
     * Notify application for the PCO data after data attached.
     *
     * @param type Type of radio indication
     * @param PcoDataAttachedInfo as defined in types.hal
     */
    oneway void pcoDataAfterAttached(in android.hardware.radio.RadioIndicationType type,
        in PcoDataAttachedInfo pcoData);

    /**
     * Notify the network reject cause
     *
     * @param type Type of radio indication
     * @param data indicates the reject cause
     */
    oneway void networkRejectCauseInd(in android.hardware.radio.RadioIndicationType type,
        in int[] data);

    /**
     * Network Limit State
     *
     * @param type Type of radio indication
     * @param state the network limit state
     */
    oneway void onNwLimitInd(in android.hardware.radio.RadioIndicationType type, in int[] state);

    /**
     * Notify Qualified Network Types Changed
     *
     * @param type Type of radio indication
     * @param data apn type and qualified network types
     */
    oneway void qualifiedNetworkTypesChangedInd(in android.hardware.radio.RadioIndicationType type,
        in int[] data);

    /**
     * Notify ursp change with raw strings
     *
     * @param slotIndex Slot id start from 0
     * @param type Currently always 0
     * @param originalUrsp Ursp raw data strings
     * @param urspRules Ursp rules with customized traffic descriptors
     */
    oneway void postUrspRule(in int slotIndex, in android.hardware.radio.RadioIndicationType type,
        in String originalUrsp, in UrspRule[] urspRules);
}
