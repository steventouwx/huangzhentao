// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.data;

import vendor.mediatek.hardware.mtkradioex.data.PktFilter;
import vendor.mediatek.hardware.mtkradioex.data.TftParameter;

@VintfStability
parcelable Tft {
    int operation;
    int pfNumber;
    PktFilter[] pfList;
    TftParameter tftParameter;
}

