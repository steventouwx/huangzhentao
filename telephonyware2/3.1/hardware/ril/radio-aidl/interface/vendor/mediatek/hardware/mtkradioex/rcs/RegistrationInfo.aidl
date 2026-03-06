package vendor.mediatek.hardware.mtkradioex.rcs;

import vendor.mediatek.hardware.mtkradioex.rcs.RegMode;
import vendor.mediatek.hardware.mtkradioex.rcs.RegState;
import vendor.mediatek.hardware.mtkradioex.rcs.TransportProtocol;
import vendor.mediatek.hardware.mtkradioex.rcs.IpProtocol;

@VintfStability
parcelable RegistrationInfo {
    RegMode regMode;
    RegState regState;
    int rcsMode;
    long rcsFeatures;
    String localAddress;
    int localPort;
    String remoteAddress;
    int remotePort;
    String homeDomain;
    TransportProtocol transport;
    IpProtocol ipVersion;
    String IMPU;
    String IMPI;
    String userAgent;
    int ipSecLocalPort;
    int ipSecRemotePort;
    String securityVerifyHeader;
    String pAssociatedUri;
    String instanceId;
    String serviceRoute;
    String pAccessNetworkInfo;
    String regTimeStamp;
}
