// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.network;

import android.hardware.radio.network.OperatorInfo;

@VintfStability
parcelable OperatorInfoWithAct {
    android.hardware.radio.network.OperatorInfo base;
    String lac;
    String act;
}

