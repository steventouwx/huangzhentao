// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.data;

// import android.hardware.radio.ApnTypes;

@VintfStability
@Backing(type="int")
enum MtkApnTypes {
    // Not expanding values from android.hardware.radio@1.5::ApnTypes. See '-e' argument.
    VSIM = 1 << 12,
    BIP = 1 << 13,
    RCS = 1 << 15,
  //  MTKALL = ALL | MCX | XCAP | RCS | BIP | VSIM,
}
