// SPDX-License-Identifier: MediaTekProprietary
#define LOG_TAG "SIGNAL_STRENGTH"

#include "SignalStrength.h"
#include "mtk_log.h"

SignalStrength::SignalStrength(){
}


SignalStrength::SignalStrength(int gsmSignalStrength, int gsmBitErrorRate,
        int cdmaDbm, int cdmaEcio,
        int evdoDbm, int evdoEcio, int evdoSnr,
        int lteSignalStrength, int lteRsrp, int lteRsrq, int lteRssnr, int lteCqi,
        int tdScdmaRscp, int wcdmaSignalStrength, int wcdmaRscpAsu,
        int csiRsrp, int csiRsrq, int csiSinr,
        int ssRsrp, int ssRsrq, int ssSinr){
    mGsmSignalStrength = gsmSignalStrength;
    mGsmBitErrorRate = gsmBitErrorRate;
    mCdmaDbm = cdmaDbm;
    mCdmaEcio = cdmaEcio;
    mEvdoDbm = evdoDbm;
    mEvdoEcio = evdoEcio;
    mEvdoSnr = evdoSnr;
    mLteSignalStrength = lteSignalStrength;
    mLteRsrp = lteRsrp;
    mLteRsrq = lteRsrq;
    mLteRssnr = lteRssnr;
    mLteCqi = lteCqi;
    mTdScdmaRscp = tdScdmaRscp;
    mWcdmaSignalStrength = wcdmaSignalStrength;
    mWcdmaRscpAsu = wcdmaRscpAsu;
    mWcdmaRscp = wcdmaRscpAsu - 120;
    mCsiRsrp = csiRsrp;
    mCsiRsrq = csiRsrq;
    mCsiSinr = csiSinr;
    mSsRsrp = ssRsrp;
    mSsRsrq = ssRsrq;
    mSsSinr = ssSinr;

    mUseOnlyRsrpForLteLevel = true;
    mWcdmaDefaultSignalMeasurement = MEASUMENT_TYPE_RSCP;
    mLteRsrpBoost = 0;
    mLteRsrpThresholds[0] = -128; /* SIGNAL_STRENGTH_POOR */
    mLteRsrpThresholds[1] = -118; /* SIGNAL_STRENGTH_MODERATE */
    mLteRsrpThresholds[2] = -108; /* SIGNAL_STRENGTH_GOOD */
    mLteRsrpThresholds[3] = -98; /* SIGNAL_STRENGTH_GREAT */
    mWcdmaRscpThresholds[0] = -115; /* SIGNAL_STRENGTH_POOR */
    mWcdmaRscpThresholds[1] = -105; /* SIGNAL_STRENGTH_MODERATE */
    mWcdmaRscpThresholds[2] = -95; /* SIGNAL_STRENGTH_GOOD */
    mWcdmaRscpThresholds[3] = -85; /* SIGNAL_STRENGTH_GREAT */
    mSsRsrpThreholds[0] = -110;
    mSsRsrpThreholds[1] = -90;
    mSsRsrpThreholds[2] = -80;
    mSsRsrpThreholds[3] = -65;
    mSsRsrqThreholds[0] = -16;
    mSsRsrqThreholds[1] = -12;
    mSsRsrqThreholds[2] = -9;
    mSsRsrqThreholds[3] = -6;
    mSsSinrThreholds[0] = -5;
    mSsSinrThreholds[1] = 5;
    mSsSinrThreholds[2] = 15;
    mSsSinrThreholds[3] = 30;
    mParameterUseForNrLevel = USE_SSRSRP;
}

SignalStrength::~SignalStrength() {
}

void SignalStrength::init(){
    mGsmSignalStrength = 0;
    mGsmBitErrorRate = 0;
    mCdmaDbm = 0;
    mCdmaEcio = 0;
    mEvdoDbm = 0;
    mEvdoEcio = 0;
    mEvdoSnr = 0;
    mLteSignalStrength = 0;
    mLteRsrp = 0;
    mLteRsrq = 0;
    mLteRssnr = 0;
    mLteCqi = 0;
    mTdScdmaRscp = 0;
    mWcdmaSignalStrength = 0;
    mWcdmaRscpAsu = 0;
    mWcdmaRscp = 0;
    mCsiRsrp = 0;
    mCsiRsrq = 0;
    mCsiSinr = 0;
    mSsRsrp = 0;
    mSsRsrq = 0;
    mSsSinr = 0;

    mUseOnlyRsrpForLteLevel = true;
    mWcdmaDefaultSignalMeasurement = MEASUMENT_TYPE_RSCP;
    mLteRsrpBoost = 0;
    mLteRsrpThresholds[0] = -128; /* SIGNAL_STRENGTH_POOR */
    mLteRsrpThresholds[1] = -118; /* SIGNAL_STRENGTH_MODERATE */
    mLteRsrpThresholds[2] = -108; /* SIGNAL_STRENGTH_GOOD */
    mLteRsrpThresholds[3] = -98; /* SIGNAL_STRENGTH_GREAT */
    mWcdmaRscpThresholds[0] = -115; /* SIGNAL_STRENGTH_POOR */
    mWcdmaRscpThresholds[1] = -105; /* SIGNAL_STRENGTH_MODERATE */
    mWcdmaRscpThresholds[2] = -95; /* SIGNAL_STRENGTH_GOOD */
    mWcdmaRscpThresholds[3] = -85; /* SIGNAL_STRENGTH_GREAT */
    mSsRsrpThreholds[0] = -110;
    mSsRsrpThreholds[1] = -90;
    mSsRsrpThreholds[2] = -80;
    mSsRsrpThreholds[3] = -65;
    mSsRsrqThreholds[0] = -16;
    mSsRsrqThreholds[1] = -12;
    mSsRsrqThreholds[2] = -9;
    mSsRsrqThreholds[3] = -6;
    mSsSinrThreholds[0] = -5;
    mSsSinrThreholds[1] = 5;
    mSsSinrThreholds[2] = 15;
    mSsSinrThreholds[3] = 30;
    mParameterUseForNrLevel = USE_SSRSRP;
}

void SignalStrength::update(int gsmSignalStrength, int gsmBitErrorRate,
        int cdmaDbm, int cdmaEcio,
        int evdoDbm, int evdoEcio, int evdoSnr,
        int lteSignalStrength, int lteRsrp, int lteRsrq, int lteRssnr, int lteCqi,
        int tdScdmaRscp, int wcdmaSignalStrength, int wcdmaRscpAsu,
        int csiRsrp, int csiRsrq, int csiSinr,
        int ssRsrp, int ssRsrq, int ssSinr){
    mGsmSignalStrength = gsmSignalStrength;
    mGsmBitErrorRate = gsmBitErrorRate;
    mCdmaDbm = cdmaDbm;
    mCdmaEcio = cdmaEcio;
    mEvdoDbm = evdoDbm;
    mEvdoEcio = evdoEcio;
    mEvdoSnr = evdoSnr;
    mLteSignalStrength = lteSignalStrength;
    mLteRsrp = lteRsrp;
    mLteRsrq = lteRsrq;
    mLteRssnr = lteRssnr;
    mLteCqi = lteCqi;
    mTdScdmaRscp = tdScdmaRscp;
    mWcdmaSignalStrength = wcdmaSignalStrength;
    mWcdmaRscpAsu = wcdmaRscpAsu;
    mWcdmaRscp = wcdmaRscpAsu - 120;
    mCsiRsrp = csiRsrp;
    mCsiRsrq = csiRsrq;
    mCsiSinr = csiSinr;
    mSsRsrp = ssRsrp;
    mSsRsrq = ssRsrq;
    mSsSinr = ssSinr;

}

void SignalStrength::validateInput() {
    // TS 27.007 8.5
    mGsmSignalStrength = mGsmSignalStrength >= 0 ? mGsmSignalStrength : 99;
    mWcdmaSignalStrength = (mWcdmaSignalStrength >= 0) ? mWcdmaSignalStrength : 99;
    mLteSignalStrength = (mLteSignalStrength >= 0) ? mLteSignalStrength : 99;
    // BER no change;

    // WCDMA RSCP valid values are -120 through -24 as defined in TS 27.007 8.69
    // but are reported in ASU which is 0 through 96, so we do the conversion here
    mWcdmaRscpAsu =
            ((mWcdmaRscpAsu - 120 >= MIN_WCDMA_RSCP) && (mWcdmaRscpAsu - 120 <= MAX_WCDMA_RSCP))
            ? mWcdmaRscpAsu : 255;
    mWcdmaRscp = ((mWcdmaRscp >= MIN_WCDMA_RSCP) && (mWcdmaRscp <= MAX_WCDMA_RSCP))
            ? mWcdmaRscp : INVALID;

    mCdmaDbm = mCdmaDbm > 0 ? -mCdmaDbm : -120;
    mCdmaEcio = (mCdmaEcio >= 0) ? -mCdmaEcio : -160;

    mEvdoDbm = (mEvdoDbm > 0) ? -mEvdoDbm : -120;
    mEvdoEcio = (mEvdoEcio >= 0) ? -mEvdoEcio : -160;
    mEvdoSnr = ((mEvdoSnr >= 0) && (mEvdoSnr <= 8)) ? mEvdoSnr : -1;

    // TS 36.214 Physical Layer Section 5.1.3, TS 36.331 RRC
    mLteRsrp = ((-mLteRsrp >= MIN_LTE_RSRP) && (-mLteRsrp <= MAX_LTE_RSRP)) ? -mLteRsrp
                            : SignalStrength::INVALID;
    mLteRsrq = ((mLteRsrq >= 3) && (mLteRsrq <= 20)) ? -mLteRsrq : INVALID;
    mLteRssnr = ((mLteRssnr >= -200) && (mLteRssnr <= 300)) ? mLteRssnr
            : INVALID;

    mTdScdmaRscp = ((mTdScdmaRscp >= 0) && (mTdScdmaRscp <= 96))
            ? (mTdScdmaRscp - 120) : SignalStrength::INVALID;

    mCsiRsrp = ((-mCsiRsrp >= MIN_LTE_RSRP) && (-mCsiRsrp <= MAX_LTE_RSRP)) ? -mCsiRsrp
                            : SignalStrength::INVALID;
    mCsiRsrq = ((mCsiRsrq >= 3) && (mCsiRsrq <= 20)) ? -mCsiRsrq : INVALID;
    mCsiSinr = ((mCsiSinr >= -23) && (mCsiSinr <= 23)) ? mCsiSinr
            : INVALID;
    mSsRsrp = ((-mSsRsrp >= MIN_LTE_RSRP) && (-mSsRsrp <= MAX_LTE_RSRP)) ? -mSsRsrp
                            : SignalStrength::INVALID;
    mSsRsrq = ((mSsRsrq >= 3) && (mSsRsrq <= 20)) ? -mSsRsrq : INVALID;
    mSsSinr = ((mSsSinr >= -23) && (mSsSinr <= 40)) ? mLteRssnr
            : INVALID;
}

int SignalStrength::getLevel() {
    return getGsmRelatedSignalStrength();
}

int SignalStrength::getAsuLevel() {
    int asuLevel = 0;
    if (mIsGsm) {
        if (mLteAsPrimaryInNrNsa && mLteRsrp != INVALID) {
            asuLevel = getLteAsuLevel();
        } else if (mSsRsrp != INVALID) {
            asuLevel = getNrAsuLevel();
        }  else if (mLteRsrp != INVALID) {
            asuLevel = getLteAsuLevel();
        }else if (mTdScdmaRscp != INVALID) {
            asuLevel = getTdScdmaAsuLevel();
        } else if (mWcdmaRscp != INVALID) {
            asuLevel = getWcdmaAsuLevel();
        } else {
            asuLevel = getGsmAsuLevel();
        }
    }
    if (DBG) ALOGD("getAsuLevel=", asuLevel);
    return asuLevel;
}

int SignalStrength::getGsmRelatedSignalStrength() {
    int level = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;
    if(mLteAsPrimaryInNrNsa) {
        level = getLteLevel();
    }

    if (level == SIGNAL_STRENGTH_NONE_OR_UNKNOWN) {
        level = getNrLevel();
        if (level == SIGNAL_STRENGTH_NONE_OR_UNKNOWN) {
            if (!mLteAsPrimaryInNrNsa) {
                level = getLteLevel();
            }
            if (level == SIGNAL_STRENGTH_NONE_OR_UNKNOWN) {
                level = getTdScdmaLevel();
                if (level == SIGNAL_STRENGTH_NONE_OR_UNKNOWN) {
                    level = getWcdmaLevel();
                    if (level == SIGNAL_STRENGTH_NONE_OR_UNKNOWN) {
                        level = getGsmLevel();
                    }
                }
            }
        }
    }
    return level;
}

int SignalStrength::getNrDbm() {
    return mSsRsrp;
}

int SignalStrength::getLteDbm() {
    return mLteRsrp;
}

bool SignalStrength::useOnlyRsrpForLteLevel() {
    return mUseOnlyRsrpForLteLevel;
}

int SignalStrength::updateLevelWithMeasure(int measure, int* thresholds) {
    int level;
    if (measure == INVALID) {
        level = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;
    } else if (measure > thresholds[3]) {
        level = SIGNAL_STRENGTH_GREAT;
    } else if (measure > thresholds[2]) {
        level = SIGNAL_STRENGTH_GOOD;
    } else if (measure > thresholds[1]) {
        level = SIGNAL_STRENGTH_MODERATE;
    }  else if (measure > thresholds[0]) {
        level = SIGNAL_STRENGTH_POOR;
    } else {
        level = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;
    }
    return level;
}

bool SignalStrength::isLevelForParameter(int parameterType) {
    return (parameterType & mParameterUseForNrLevel) == parameterType;
}

int SignalStrength::min(int v1, int v2) {
    return v1 < v2 ? v1 : v2;
}

int SignalStrength::getNrLevel() {
    int ssRsrpLevel = INVALID;
    int ssRsrqLevel = INVALID;
    int ssSinrLevel = INVALID;
    if(isLevelForParameter(USE_SSRSRP)) {
        ssRsrpLevel = updateLevelWithMeasure(mSsRsrp, mSsRsrpThreholds);
    }
    if(isLevelForParameter(USE_SSRSRQ)) {
        ssRsrqLevel = updateLevelWithMeasure(mSsRsrq, mSsRsrqThreholds);
    }
    if(isLevelForParameter(USE_SSSINR)) {
        ssSinrLevel = updateLevelWithMeasure(mSsSinr, mSsSinrThreholds);
    }
    return min (min(ssRsrpLevel,ssRsrqLevel),ssSinrLevel);
}

int SignalStrength::getNrAsuLevel() {
    int asuLevel;
    int nrDbm = getNrDbm();
    if (nrDbm == INVALID) {
        asuLevel = 99;
    } else if (nrDbm <= -140) {
        asuLevel = 0;
    } else if (nrDbm >= -43) {
        asuLevel = 97;
    } else {
        asuLevel = nrDbm + 140;
    }
    return asuLevel;
}

/**
 * Get LTE as level 0..4
 *
 * @hide
 */
int SignalStrength::getLteLevel() {
    /*
     * TS 36.214 Physical Layer Section 5.1.3
     * TS 36.331 RRC
     *
     * RSSI = received signal + noise
     * RSRP = reference signal dBm
     * RSRQ = quality of signal dB = Number of Resource blocks*RSRP/RSSI
     * SNR = gain = signal/noise ratio = -10log P1/P2 dB
     */
    int rssiIconLevel = SIGNAL_STRENGTH_NONE_OR_UNKNOWN, rsrpIconLevel = -1, snrIconLevel = -1;

    if (mLteRsrp > MAX_LTE_RSRP || mLteRsrp < MIN_LTE_RSRP) {
        if (mLteRsrp != INVALID) {
            ALOGD("getLteLevel - invalid lte rsrp: mLteRsrp=%d", mLteRsrp);
        }
    } else if (mLteRsrp >= (mLteRsrpThresholds[3] - mLteRsrpBoost)) {
        rsrpIconLevel = SIGNAL_STRENGTH_GREAT;
    } else if (mLteRsrp >= (mLteRsrpThresholds[2] - mLteRsrpBoost)) {
        rsrpIconLevel = SIGNAL_STRENGTH_GOOD;
    } else if (mLteRsrp >= (mLteRsrpThresholds[1] - mLteRsrpBoost)) {
        rsrpIconLevel = SIGNAL_STRENGTH_MODERATE;
    } else if (mLteRsrp >= (mLteRsrpThresholds[0] - mLteRsrpBoost)) {
        rsrpIconLevel = SIGNAL_STRENGTH_POOR;
    } else {
        rsrpIconLevel = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;
    }

    if (useOnlyRsrpForLteLevel()) {
        ALOGD("useOnlyRsrpForLteLevel:getLTELevel - rsrp = %d,rsrpIconLevel: %d", mLteRsrp, rsrpIconLevel);
        if (rsrpIconLevel != -1) {
            return rsrpIconLevel;
        }
    }

    /*
     * Values are -200 dB to +300 (SNR*10dB) RS_SNR >= 13.0 dB =>4 bars 4.5
     * dB <= RS_SNR < 13.0 dB => 3 bars 1.0 dB <= RS_SNR < 4.5 dB => 2 bars
     * -3.0 dB <= RS_SNR < 1.0 dB 1 bar RS_SNR < -3.0 dB/No Service Antenna
     * Icon Only
     */
    if (mLteRssnr > 300) snrIconLevel = -1;
    else if (mLteRssnr >= 130) snrIconLevel = SIGNAL_STRENGTH_GREAT;
    else if (mLteRssnr >= 45) snrIconLevel = SIGNAL_STRENGTH_GOOD;
    else if (mLteRssnr >= 10) snrIconLevel = SIGNAL_STRENGTH_MODERATE;
    else if (mLteRssnr >= -30) snrIconLevel = SIGNAL_STRENGTH_POOR;
    else if (mLteRssnr >= -200)
        snrIconLevel = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;

    if (DBG)
        ALOGD("getLTELevel - rsrp: %d, snr: %d rsrpIconLevel: %d snrIconLevel: %d lteRsrpBoost:%d",
            mLteRsrp,mLteRssnr,rsrpIconLevel,snrIconLevel,mLteRsrpBoost);

    /* Choose a measurement type to use for notification */
    if (snrIconLevel != -1 && rsrpIconLevel != -1) {
        /*
         * The number of bars displayed shall be the smaller of the bars
         * associated with LTE RSRP and the bars associated with the LTE
         * RS_SNR
         */
        return (rsrpIconLevel < snrIconLevel ? rsrpIconLevel : snrIconLevel);
    }

    if (snrIconLevel != -1) return snrIconLevel;

    if (rsrpIconLevel != -1) return rsrpIconLevel;

    /* Valid values are (0-63, 99) as defined in TS 36.331 */
    // TODO the range here is probably supposed to be (0..31, 99). It's unclear if anyone relies
    // on the current incorrect range check, so this will be fixed in a future release with more
    // soak time
    if (mLteSignalStrength > 63) rssiIconLevel = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;
    else if (mLteSignalStrength >= 12) rssiIconLevel = SIGNAL_STRENGTH_GREAT;
    else if (mLteSignalStrength >= 8) rssiIconLevel = SIGNAL_STRENGTH_GOOD;
    else if (mLteSignalStrength >= 5) rssiIconLevel = SIGNAL_STRENGTH_MODERATE;
    else if (mLteSignalStrength >= 0) rssiIconLevel = SIGNAL_STRENGTH_POOR;

    if (DBG)
        ALOGD("getLteLevel - rssi: %d rssiIconLevel: %d", mLteSignalStrength, rssiIconLevel);
    return rssiIconLevel;

}


int SignalStrength::getLteAsuLevel() {
    int lteAsuLevel;
    int lteDbm = getLteDbm();
    /*
     * 3GPP 27.007 (Ver 10.3.0) Sec 8.69
     * 0   -140 dBm or less
     * 1   -139 dBm
     * 2...96  -138... -44 dBm
     * 97  -43 dBm or greater
     * 255 not known or not detectable
     */
    /*
     * validateInput will always give a valid range between -140 t0 -44 as
     * per ril.h. so RSRP >= -43 & <-140 will fall under asu level 255
     * and not 97 or 0
     */
    if (lteDbm == INVALID) lteAsuLevel = 255;
    else lteAsuLevel = lteDbm + 140;
    if (DBG) ALOGD("Lte Asu level: %d" ,lteAsuLevel);
    return lteAsuLevel;
}

int SignalStrength::getTdScdmaDbm() {
    return mTdScdmaRscp;
}

int SignalStrength::getTdScdmaLevel() {
    int tdScdmaDbm = getTdScdmaDbm();
    int level;

    if ((tdScdmaDbm > -25) || (tdScdmaDbm == INVALID))
            level = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;
    else if (tdScdmaDbm >= -49) level = SIGNAL_STRENGTH_GREAT;
    else if (tdScdmaDbm >= -73) level = SIGNAL_STRENGTH_GOOD;
    else if (tdScdmaDbm >= -97) level = SIGNAL_STRENGTH_MODERATE;
    else if (tdScdmaDbm >= -110) level = SIGNAL_STRENGTH_POOR;
    else level = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;

    if (DBG) ALOGD("getTdScdmaLevel = %d ", level);
    return level;
}

int SignalStrength::getTdScdmaAsuLevel() {
    int tdScdmaDbm = getTdScdmaDbm();
    int tdScdmaAsuLevel;

    if (tdScdmaDbm == INVALID) tdScdmaAsuLevel = 255;
    else tdScdmaAsuLevel = tdScdmaDbm + 120;
    if (DBG) ALOGD("TD-SCDMA Asu level: %d", tdScdmaAsuLevel);
    return tdScdmaAsuLevel;
}

int SignalStrength::getWcdmaLevel() {
    int level = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;

    if (mWcdmaDefaultSignalMeasurement == "") {
         ALOGD( "getWcdmaLevel - WCDMA default signal measurement is invalid.");
         return level;
    }

    if (mWcdmaDefaultSignalMeasurement == MEASUMENT_TYPE_RSCP) {
    // RSCP valid values are (-120 through -24) as defined in TS 27.007 8.69
        if (mWcdmaRscp < MIN_WCDMA_RSCP || mWcdmaRscp > MAX_WCDMA_RSCP) {
                if (mWcdmaRscp != INVALID) {
                    ALOGD("getWcdmaLevel - invalid WCDMA RSCP: mWcdmaRscp="
                    , mWcdmaRscp);
                }
            } else if (mWcdmaRscp >= mWcdmaRscpThresholds[3]) {
                level = SIGNAL_STRENGTH_GREAT;
            } else if (mWcdmaRscp >= mWcdmaRscpThresholds[2]) {
                level = SIGNAL_STRENGTH_GOOD;
            } else if (mWcdmaRscp >= mWcdmaRscpThresholds[1]) {
                level = SIGNAL_STRENGTH_MODERATE;
            } else if (mWcdmaRscp >= mWcdmaRscpThresholds[0]) {
                level = SIGNAL_STRENGTH_POOR;
            }
            if (DBG) ALOGD("getWcdmaLevel= %d WcdmaRscp= %d" , level , mWcdmaRscp);
    }else {
        // RSSI valid values are (0..31) as defined in TS 27.007 8.5
        if (mWcdmaSignalStrength < 0 || mWcdmaSignalStrength > 31) {
            if (mWcdmaSignalStrength != 99) {
                ALOGD("getWcdmaLevel - invalid WCDMA RSSI: mWcdmaSignalStrength="
                , mWcdmaSignalStrength);
            }
        } else if (mWcdmaSignalStrength >= 18) {
            level = SIGNAL_STRENGTH_GREAT;
        } else if (mWcdmaSignalStrength >= 13) {
            level = SIGNAL_STRENGTH_GOOD;
        } else if (mWcdmaSignalStrength >= 8) {
            level = SIGNAL_STRENGTH_MODERATE;
        } else if (mWcdmaSignalStrength >= 3) {
            level = SIGNAL_STRENGTH_POOR;
        }
        if (DBG) ALOGD("getWcdmaLevel= %d, WcdmaSignalStrength= %d" , level ,
            mWcdmaSignalStrength);
    }
    return level;
}

int SignalStrength::getWcdmaAsuLevel() {
    /*
     * 3GPP 27.007 (Ver 10.3.0) Sec 8.69
     * 0      -120 dBm or less
     * 1      -119 dBm
     * 2...95 -118... -25 dBm
     * 96     -24 dBm or greater
     * 255    not known or not detectable
     */
    int wcdmaDbm = getWcdmaDbm();
    int wcdmaAsuLevel = 255;
    // validateInput will always give a valid range between -120 to -24 as per ril.h. so RSCP
    // outside range is already set to INVALID
    if (wcdmaDbm == INVALID) wcdmaAsuLevel =  255;
    else wcdmaAsuLevel = wcdmaDbm + 120;
    if (DBG) ALOGD("Wcdma Asu level: %d", wcdmaAsuLevel);
    return wcdmaAsuLevel;
}

int SignalStrength::getWcdmaDbm() {
    return mWcdmaRscp;
}

int SignalStrength::getGsmSignalStrength() {
    return mGsmSignalStrength;
}

int SignalStrength::getGsmLevel() {
    int level;

    // ASU ranges from 0 to 31 - TS 27.007 Sec 8.5
    // asu = 0 (-113dB or less) is very weak
    // signal, its better to show 0 bars to the user in such cases.
    // asu = 99 is a special case, where the signal strength is unknown.
    int asu = getGsmSignalStrength();
    if (asu <= 2 || asu == 99) level = SIGNAL_STRENGTH_NONE_OR_UNKNOWN;
    else if (asu >= 12) level = SIGNAL_STRENGTH_GREAT;
    else if (asu >= 8)  level = SIGNAL_STRENGTH_GOOD;
    else if (asu >= 5)  level = SIGNAL_STRENGTH_MODERATE;
    else level = SIGNAL_STRENGTH_POOR;
    if (DBG) ALOGD("getGsmLevel= %d", level);
    return level;
}

int SignalStrength::getGsmAsuLevel() {
    // ASU ranges from 0 to 31 - TS 27.007 Sec 8.5
    // asu = 0 (-113dB or less) is very weak
    // signal, its better to show 0 bars to the user in such cases.
    // asu = 99 is a special case, where the signal strength is unknown.
    int level = getGsmSignalStrength();
    if (DBG) ALOGD("getGsmAsuLevel=%d", level);
    return level;
}

