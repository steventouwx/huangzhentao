// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.data;

import vendor.mediatek.hardware.mtkradioex.data.Qos;
import vendor.mediatek.hardware.mtkradioex.data.Tft;

@VintfStability
parcelable DedicateDataCall {
    int ddcId;
    int interfaceId;
    int primaryCid;
    int cid;
    int active;
    int signalingFlag;
    int bearerId;
    int failCause;
    int hasQos;
    Qos qos;
    int hasTft;
    Tft tft;
    int hasPcscf;
    String pcscf;
}

