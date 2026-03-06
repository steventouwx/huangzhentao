// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.data;

@VintfStability
parcelable PktFilter {
    int id;
    int precedence;
    int direction;
    int networkPfIdentifier;
    int bitmap;
    String address;
    String mask;
    int protocolNextHeader;
    int localPortLow;
    int localPortHigh;
    int remotePortLow;
    int remotePortHigh;
    int spi;
    int tos;
    int tosMask;
    int flowLabel;
}

