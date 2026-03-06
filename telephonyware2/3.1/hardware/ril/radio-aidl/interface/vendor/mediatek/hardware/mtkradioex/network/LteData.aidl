// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.network;

@VintfStability
parcelable LteData {
    int state;
    int mcc;
    int mnc;
    int cellId;
    int band;
    int bandwidth;
    int ulChannel;
    int dlChannel;
    int rssi;
    int rsrq;
    int rsrp;
    int txPower;
}

