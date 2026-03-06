// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.tbox;

import  vendor.mediatek.hardware.tbox.OperatorInfo;

@VintfStability
parcelable OperatorInfoWithAct {
    vendor.mediatek.hardware.tbox.OperatorInfo base;
    String lac;
    String act;
}

