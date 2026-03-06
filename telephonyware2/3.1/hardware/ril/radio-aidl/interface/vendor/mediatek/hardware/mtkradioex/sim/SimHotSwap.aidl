// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.sim;

@VintfStability
@Backing(type="int")
enum SimHotSwap {
    SIM_HOTSWAP_PLUG_IN,
    SIM_HOTSWAP_PLUG_OUT,
    SIM_HOTSWAP_RECOVERY,
    SIM_HOTSWAP_MISSING,
    SIM_HOTSWAP_TRAY_PLUG_IN,
    SIM_HOTSWAP_TRAY_PLUG_OUT,
    SIM_HOTSWAP_COMMONSLOT_NO_CHANGED,
}
