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

import vendor.mediatek.hardware.tbox.MTK_CellInfo;
import vendor.mediatek.hardware.tbox.MTK_SignalStrength;
import vendor.mediatek.hardware.tbox.SmsInfo;
import vendor.mediatek.hardware.tbox.VcallInformation;
import vendor.mediatek.hardware.tbox.VcallMsg;
import vendor.mediatek.hardware.tbox.VcallState;
import vendor.mediatek.hardware.tbox.EcallMsg;
import vendor.mediatek.hardware.tbox.CellInfo;
import vendor.mediatek.hardware.tbox.RadioState;

/*
 * Interface declaring data indications.
 */
@VintfStability
interface ITeleServiceIndication {
    oneway void SmsInfoReceived(in SmsInfo msg);

    oneway void cellInfoListInd(in CellInfo[] records);

    oneway void notifySignalStrength(in MTK_SignalStrength signalStrength);

    oneway void vcallInformationInd(in VcallInformation info);

    oneway void vcallMsgInd(in VcallMsg msg);

    oneway void vcallStateInd(in VcallState status);

    oneway void ecallMsgInd(in EcallMsg msg);
    /**
     * Indicates when PS state changes.
     *
     * @param Info register state
     * @param Info mcc/mnc
     * @param Info Act
     */
    oneway void responsePsNetworkStateChangeInd(in int[] state);
    /**
     * Indicates when CS state changes.
     *
     * @param Info register state
     * @param Info lac, Location area code.
     * @param Info ci, Cell id, 32 bit for non-5G and 36 bit for 5G
     * @param Info Act, Radio access technology
     * @param Info reject cause,  The cause of the failed registration.
     * @param Info cdma network existence, Indicate if nw exist, only valid when eact belongs to C2K.
     */
    oneway void responseCsNetworkStateChangeInd(in String[] state);

    oneway void responseCardStateChangedInd();
    /**
     * Indicates when radio has received a NITZ time message.
     *
     * @param type Type of radio indication
     * @param nitzTime NITZ time string in the form "yy/mm/dd,hh:mm:ss(+/-)tz,dt"
     * @param receivedTimeMs time (in milliseconds since boot) at which RIL sent the NITZ time to
     *        the framework
     * @param ageMs time in milliseconds indicating how long NITZ was cached in RIL and modem.
     *        This must track true age and therefore must be calculated using clocks that
     *        include the time spend in sleep / low power states. If it can not be guaranteed,
     *        there must not be any caching done at the modem and should fill in 0 for ageMs
     */
    void nitzTimeReceived(in String nitzTime, in long receivedTimeMs, in long ageMs);
    /**
     * Indicates when new SMS is received.
     *
     * @param pdu PDU of SMS-DELIVER represented as byte array.
     *        The PDU starts with the SMSC address per TS 27.005 (+CMT:)
     */
    void newSms(in byte[] pdu);
    /**
     * Indicates when IMS registration state has changed. To get IMS registration state and IMS SMS
     * format, callee needs to invoke getImsRegState().
     */
    void imsNetworkStateChanged();
    /**
     * Indicates when voice or data network state changed. Callee must invoke
     * ITeleService.getVoiceRegistrationState(), ITeleService.getDataRegistrationState(), and
     * ITeleService.getOperator()
     */
    void networkStateChanged();
    /**
     * Indicates when radio state changes.
     *
     * @param type Type of radio indication
     * @param radioState Current radio state
     */
    void radioStateChanged(in RadioState radioState);

}
