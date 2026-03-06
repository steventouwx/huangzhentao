/*
 * TrafficDescriptor with more parameters.
 */
package vendor.mediatek.hardware.mtkradioex.data;

import android.hardware.radio.data.OsAppId;

import vendor.mediatek.hardware.mtkradioex.data.IpDescriptors;

@VintfStability
@JavaDerive(toString=true)
parcelable TrafficDescriptor {
    /**
     * DNN stands for Data Network Name and represents an APN as defined in 3GPP TS 23.003.
     */
    @nullable String dnn;
    /**
     * Indicates the OsId + OsAppId (used as category in Android).
     */
    @nullable OsAppId osAppId;
    /**
     * This struct represents the IP 3 tuple(s). TS 24.526 Section 4.4.2
     */
    @nullable IpDescriptors ipDescriptors;
    /**
     * This represents an optional domainDescriptors. TS 24.526 Section 4.4.2
     */
    @nullable String domainDescriptors;
    /**
     * This safe_union represents an optional connectionCapabilities. TS 24.526 Section 4.4.2
     */
    @nullable String connectionCapabilities;
}
