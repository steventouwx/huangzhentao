// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.sim;
import vendor.mediatek.hardware.mtkradioex.sim.SimTypeInfo;
import vendor.mediatek.hardware.mtkradioex.sim.VsimOperationEvent;

/**
 * Interface declaring unsolicited indications to subsidylock.
 */
@VintfStability
interface IMtkRadioExSimIndication {

    /**
     * Indicates when SIM issue a BIP proactive command to applications
     *
     * @param type Type of radio indication
     * @param cmd SAT/USAT BIP proactive represented as byte array starting with command tag.
     * Refer ETSI TS 102.223 section 9.4 for BIP related command types
     */
    oneway void bipProactiveCommand(in android.hardware.radio.RadioIndicationType type,
        in String cmd);

    /**
     * Indicates a change of iccid
     *
     * @param type Type of radio indication
     * @param iccid Iccid of SIM
     */
    oneway void iccidChanged(in android.hardware.radio.RadioIndicationType type, in String iccid);

    /**
     * [SIM] Indicate card detected
     * @param type Type of radio indication
     *
     */
    oneway void onCardDetectedInd(in android.hardware.radio.RadioIndicationType type);

    /**
     * [SIM] IMEI lock Indication
     * @param info Response info struct containing response type, serial no. and error
     * @param result Indication Data
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void onImeiLock(in android.hardware.radio.RadioIndicationType type);

    /**
     * [SIM] IMSI Refressh Done Indication
     * @param info Response info struct containing response type, serial no. and error
     * @param result Indication Data
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void onImsiRefreshDone(in android.hardware.radio.RadioIndicationType type);

    /**
     * Notification for Remote SIM Unlock event
     * @param type Type of radio indication
     * @param eventId event id
     * @param eventString event string
     */
    oneway void onRsuEvent(in android.hardware.radio.RadioIndicationType type, in int eventId,
        in String eventString);

    /**
     * [SIM] The notification of Sim Remote SIM Unlock
     * @param type Type of radio indication
     * @param eventId event id
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void onRsuSimLockEvent(in android.hardware.radio.RadioIndicationType type,
        in int eventId);

    /**
     * [SIM] Sim hot swap Indication
     * @param event hot swap event type.
     * @param info hot swap information.
     */
    oneway void onSimHotSwapInd(in android.hardware.radio.RadioIndicationType type, in int event,
        in String info);

    /**
     * Indicates the SIM power status changed
     *
     * @param type Type of radio indication
     * @param info SIM power state
     */
    oneway void onSimPowerChangedInd(in android.hardware.radio.RadioIndicationType type,
        in int[] info);

    /**
     * Indicates when need reset stk menu
     * URC:RIL_UNSOL_STK_SETUP_MENU_RESET
     *
     * @param type Type of radio indication
     */
    oneway void onStkMenuReset(in android.hardware.radio.RadioIndicationType type);

    /**
     * [SIM] Virtual SIM On/Off status Indication
     * @param type Type of radio indication
     * @param simInserted Indication Data
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void onVirtualSimStatusChanged(in android.hardware.radio.RadioIndicationType type,
        in int simInserted);

    /**
     * Send raw data to external sim manager.
     *
     * @param type Type of radio indication
     * @param event vsim data payload
     */
    oneway void onVsimEventIndication(in android.hardware.radio.RadioIndicationType type,
        in VsimOperationEvent event);

    /**
     * Indicates phb state changes.
     *
     * @param type Type of radio indication
     * @param isPhbReady New phb state
     */
    oneway void phbReadyNotification(in android.hardware.radio.RadioIndicationType type,
        in int isPhbReady);

    /**
     * Indicates the SIM ME LOCK slot lock information changed
     *
     * @param type Type of radio indication
     * @param info SIM ME LOCK slot lock infomation
     */
    oneway void smlSlotLockInfoChangedInd(in android.hardware.radio.RadioIndicationType type,
        in int[] info);

    /**
     * Indicates the SIM Type info change
     *
     * @param type Type of radio indication
     * @param SimTypeInfo the sim type information.
     */
    oneway void simTypeInfoChangedInd(in android.hardware.radio.RadioIndicationType type, in SimTypeInfo typeInfo);
}
