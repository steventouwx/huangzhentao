// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.network;

@VintfStability
parcelable SignalStrengthWithWcdmaEcio {
    int gsm_signalStrength;
    int gsm_bitErrorRate;
    int wcdma_rscp;
    int wcdma_ecio;
    int cdma_dbm;
    int cdma_ecio;
    int evdo_dbm;
    int evdo_ecio;
    int evdo_signalNoiseRatio;
    int lte_signalStrength;
    int lte_rsrp;
    int lte_rsrq;
    int lte_rssnr;
    int lte_cqi;
    int tdscdma_rscp;
}

