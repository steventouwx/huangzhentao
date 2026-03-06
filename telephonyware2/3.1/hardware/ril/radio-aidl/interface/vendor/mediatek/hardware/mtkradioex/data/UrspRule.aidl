/*
 * customized UrspRule
 * the only difference with AOSP is that it uses customized TrafficDescriptor
 */

package vendor.mediatek.hardware.mtkradioex.data;

import android.hardware.radio.data.RouteSelectionDescriptor;
import vendor.mediatek.hardware.mtkradioex.data.TrafficDescriptor;

/**
 * This struct represents a single URSP rule as defined in 3GPP TS 24.526.
 */
@VintfStability
@JavaDerive(toString=true)
parcelable UrspRule {
    /**
     * Precedence value in the range of 0 to 255. Higher value has lower precedence.
     */
    int precedence;
    /**
     * Used as a matcher for network requests.
     */
    TrafficDescriptor[] trafficDescriptors;
    /**
     * List of routes (connection parameters) that must be used for requests matching a
     * trafficDescriptor.
     */
    RouteSelectionDescriptor[] routeSelectionDescriptor;
}
