/*
 * This struct represents the IP 3 tuple(s). TS 24.526 Section 4.4.2
 */
package vendor.mediatek.hardware.mtkradioex.data;

@VintfStability
@JavaDerive(toString=true)
parcelable IpDescriptors {
    @nullable String ipv4;
    @nullable String maskV4;
    @nullable String ipv6;
    int prefixLength;
    int port;
    int portStartRange;
    int portEndRange;
    int protocolId;
}
