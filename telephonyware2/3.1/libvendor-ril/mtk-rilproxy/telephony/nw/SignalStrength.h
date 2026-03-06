// SPDX-License-Identifier: MediaTekProprietary
#ifndef SIGNAL_STRENGTH
#define SIGNAL_STRENGTH
#include <limits.h>
#include <string>
#include <mtk_log.h>
#include <stdlib.h>
#include <utils/RefBase.h>

#define LTE_RSRP_THRESHOLDS_NUM   4
#define WCDMA_RSCP_THRESHOLDS_NUM   4
#define NR_RSRP_THRESHOLDS_NUM 4
#define NR_RSRQ_THRESHOLDS_NUM 4
#define NR_SINR_THRESHOLDS_NUM 4

using namespace android;

class SignalStrength : public RefBase{

public:
    int DBG = 1;
    static const int INVALID = INT_MAX;

    SignalStrength();

    SignalStrength(int gsmSignalStrength, int gsmBitErrorRate,
            int cdmaDbm, int cdmaEcio,
            int evdoDbm, int evdoEcio, int evdoSnr,
            int lteSignalStrength, int lteRsrp, int lteRsrq, int lteRssnr, int lteCqi,
            int tdScdmaRscp, int wcdmaSignalStrength, int wcdmaRscp,
            int csiRsrp, int csiRsrq, int csiSinr,
            int ssRsrp, int ssRsrq, int ssSinr);
    virtual ~SignalStrength();
    void init();
    void validateInput();

    int getLevel();
    int getAsuLevel();
    void update(int gsmSignalStrength, int gsmBitErrorRate,
            int cdmaDbm, int cdmaEcio,
            int evdoDbm, int evdoEcio, int evdoSnr,
            int lteSignalStrength, int lteRsrp, int lteRsrq, int lteRssnr, int lteCqi,
            int tdScdmaRscp, int wcdmaSignalStrength, int wcdmaRscp,
            int csiRsrp, int csiRsrq, int csiSinr,
            int ssRsrp, int ssRsrq, int ssSinr);
private:

    int getGsmRelatedSignalStrength();
    int getLteDbm();
    int getNrDbm();
    bool useOnlyRsrpForLteLevel();
    int updateLevelWithMeasure(int measure, int* thresholds);
    bool isLevelForParameter(int parameterType);
    int min(int v1, int v2);
    int getNrLevel();
    int getNrAsuLevel();
    int getLteLevel();
    int getLteAsuLevel();
    int getTdScdmaDbm();
    int getTdScdmaLevel();
    int getTdScdmaAsuLevel();
    int getWcdmaLevel();
    int getWcdmaAsuLevel();
    int getWcdmaDbm();
    int getGsmSignalStrength();
    int getGsmLevel();
    int getGsmAsuLevel();

private:

    std::string MEASUMENT_TYPE_RSCP = "rscp";

    int SIGNAL_STRENGTH_NONE_OR_UNKNOWN = 0;
    int SIGNAL_STRENGTH_POOR = 1;
    int SIGNAL_STRENGTH_MODERATE = 2;
    int SIGNAL_STRENGTH_GOOD = 3;
    int SIGNAL_STRENGTH_GREAT = 4;
    int MAX_LTE_RSRP = -44;
    int MIN_LTE_RSRP = -140;

    int MAX_WCDMA_RSCP = -24;
    int MIN_WCDMA_RSCP = -120;
    int USE_SSRSRP = 1 << 0;
    int USE_SSRSRQ = 1 << 1;
    int USE_SSSINR = 1 << 2;

    /** Parameters reported by the Radio */
    int mGsmSignalStrength; // Valid values are (0-31, 99) as defined in TS 27.007 8.5
    int mGsmBitErrorRate;   // bit error rate (0-7, 99) as defined in TS 27.007 8.5
    int mCdmaDbm;   // This value is the RSSI value
    int mCdmaEcio;  // This value is the Ec/Io
    int mEvdoDbm;   // This value is the EVDO RSSI value
    int mEvdoEcio;  // This value is the EVDO Ec/Io
    int mEvdoSnr;   // Valid values are 0-8.  8 is the highest signal to noise ratio
    int mLteSignalStrength;
    int mLteRsrp;
    int mLteRsrq;
    int mLteRssnr;
    int mLteCqi;
    int mTdScdmaRscp; // Valid values are -24...-120dBm or INVALID if unknown
    int mWcdmaSignalStrength;
    int mWcdmaRscpAsu;  // the WCDMA RSCP in ASU as reported from the HAL
    int mWcdmaRscp;     // the WCDMA RSCP in dBm
    int mCsiRsrp;
    int mCsiRsrq;
    int mCsiSinr;
    int mSsRsrp;
    int mSsRsrq;
    int mSsSinr;

    bool mIsGsm = true;
    bool mUseOnlyRsrpForLteLevel = false;
    bool mLteAsPrimaryInNrNsa = true;
    int mLteRsrpThresholds[LTE_RSRP_THRESHOLDS_NUM];
    int mLteRsrpBoost;

    std::string mWcdmaDefaultSignalMeasurement;
    int mWcdmaRscpThresholds[WCDMA_RSCP_THRESHOLDS_NUM];

    int mSsRsrpThreholds[NR_RSRP_THRESHOLDS_NUM];
    int mSsRsrqThreholds[NR_RSRQ_THRESHOLDS_NUM];
    int mSsSinrThreholds[NR_SINR_THRESHOLDS_NUM];
    int mParameterUseForNrLevel;
};
#endif  // RP_NW_ADAPTER
