/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include "property/mtk_properties.h"
#include <stdlib.h>
#include "WfAtCmd.h"
#include "WfHqaCmd.h"
#include "WfDev.h"

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>
#include "libnvram.h"
#include "libfile_op.h"

#include "atcid_serial.h"
#include "atcid_util.h"
#include "atcid_cust_cmd_platform.h"
#include "at_tok.h"

#define MAC_ADDR_SIZE 6

int wifiEnableState = -1;
int wifiBand = -1;
uint32_t wifiFreq = 0;
uint32_t wifiRate = 0;
uint32_t wifiGain = 0;
int wifiChannelBandwidth = -1;
int wifiDataBandwidth = -1;
int wifiGain2 = 0;
int wifiTxPacketLength = 0;
int wifiTxPacketCount = 0;
int wifiTxPacketinterval = 0;
int wifiCompen = 0;
int wifiPrimaryChannelOffset = 0;
int wifiTxDataRate = 0;

int gRateCodeWiFi = 0;

struct hqa_chip_cap cap = {0};
uint32_t g_u4DbdcBand = 0;
uint32_t g_u4WifiTestMode = 0;
uint32_t g_u4CBW = 0;
uint32_t g_u4DBW = 0;
uint32_t g_u4ChNum = 0;
uint32_t g_u4RFBand = 0;
uint32_t g_u4Rate = 0;
uint32_t g_u4GIType = 0;
uint32_t g_u4Power = 0;
uint32_t g_u4Preamble = 0;
uint32_t g_u4PktLen = 0;
uint32_t g_u4PktCnt = 0;
uint32_t g_u4PktInterval = 0;
uint32_t g_u4PriCh = 0;

extern int sendATCommandToServiceWithResult(char* line);

/*******************************************************************************
*           P R I V A T E   F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/
static bool atci_wifi_test_set_mode (ATCI_WIFI_MODE Mode);
static bool atci_wifi_test_get_mode (uint32_t *SupportedMode);
static bool atci_wifi_test_set_bandwidth (ATCI_WIFI_CHANNEL_BANDWIDTH Bandwidth);
static bool atci_wifi_test_set_frequency (uint32_t FreqInMHz);
static bool atci_wifi_test_set_rate(uint32_t Rate);
static bool atci_wifi_test_set_power(uint32_t Gain);
static bool atci_wifi_test_clear_rx_count();
static bool atci_wifi_test_get_rx_count(uint32_t *pu4GoodFrameCount, uint32_t *pu4BadFrameCount);
static bool atci_wifi_test_get_rssi(int32_t *rssi);
static bool atci_wifi_test_set_gi(ATCI_WIFI_GI_TYPE GIType);
static bool atci_wifi_test_set_preamble(ATCI_WIFI_PREAMBLE_T PreambleType);
static bool atci_wifi_test_set_pkt_len(int PktLen);
static bool atci_wifi_test_set_pkt_cnt(int PktCnt);
static bool atci_wifi_test_set_pkt_interval(int PktInterval);
static bool atci_wifi_test_set_cbw(ATCI_WIFI_CHANNEL_BANDWIDTH Cbw);
static bool atci_wifi_test_set_dbw(ATCI_WIFI_CHANNEL_BANDWIDTH Dbw);
static bool atci_wifi_test_set_pri_ch(uint8_t PriCh);
static bool atci_wifi_test_set_tx_data_rate(int TxDataRate);
static bool atci_wifi_test_get_chip_cap();
static bool atci_wifi_test_is_dbdc_always_on();
static bool atci_wifi_test_start_tx();
static bool atci_wifi_test_stop_tx();
static bool atci_wifi_test_start_rx(char *aucSrcAddr);
static bool atci_wifi_test_stop_rx();
/*******************************************************************************
*                      P U B L I C   F U N C T I O N
********************************************************************************
*/

ATRESPONSE_t pas_wienable_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, cmdID = 0;
    bool b = false;
    int r = -1;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &cmdID);

            if (err < 0) return -1;
            switch (cmdID) {
                case 0:
                    if (wifiEnableState == 1) {
                        r = sendATCommandToServiceWithResult("AT+WITOF=1");
                        if(r != -1)
                            wifiEnableState = 0;
                        else
                            return AT_ERROR;
                    } else {
#ifdef MTK_WLAN_FEATURE
                        b = WIFI_TEST_CloseDUT();
#endif
                        if (b)
                            wifiEnableState = 0;
                        else
                            return AT_ERROR;
                    }
                    return AT_OK;
                case 1:
                    r = sendATCommandToServiceWithResult("AT+WITOF=2");
                    if (r != -1)
                        wifiEnableState = 1;
                    else
                        return AT_ERROR;
                    return AT_OK;
                case 2:
#ifdef MTK_WLAN_FEATURE
                    b = WIFI_TEST_OpenDUT();
#endif
                    if (b)
                        wifiEnableState = 2;
                    else
                        return AT_ERROR;
                    return AT_OK;
            }
            break;
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiEnableState) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;

    }

    return AT_ERROR;
}

ATRESPONSE_t pas_wimode_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, cmdID = 0;
    bool b = false;
    uint32_t mode = 0;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &cmdID);

            if (err < 0) return -1;
            switch (cmdID) {
                case 0:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_mode(ATCI_WIFI_MODE_BY_API_CONTROL);
#endif
                    break;
                case 1:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_mode(ATCI_WIFI_MODE_CW_ONLY);
#endif
                    break;
                case 2:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_mode(ATCI_WIFI_MODE_80211A_ONLY);
#endif
                    break;
                case 3:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_mode(ATCI_WIFI_MODE_80211B_ONLY);
#endif
                    break;
                case 4:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_mode(ATCI_WIFI_MODE_80211G_ONLY);
#endif
                    break;
                case 5:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_mode(ATCI_WIFI_MODE_80211N_ONLY);
#endif
                    break;
                case 6:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_mode(ATCI_WIFI_MODE_80211AC_ONLY);
#endif
                    break;
            }
            if (b)
                return AT_OK;
            else
                return AT_ERROR;
        case AT_TEST_OP:
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_get_mode(&mode);
#endif
            if (sprintf(response, "%d", mode) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            break;
        default:
            break;
    }
    if (b)
        return AT_OK;
    else
        return AT_ERROR;
}

ATRESPONSE_t pas_wiband_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, cmdID = 0;
    bool b = false;
    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &cmdID);
            if (err < 0) return -1;
            switch (cmdID) {
                case 0:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_bandwidth(ATCI_WIFI_CH_BW_20MHZ);
#endif
                    if (b) {
                        if (sprintf(response, "20MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiBand = 0;
                    }
                    break;
                case 1:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_bandwidth(ATCI_WIFI_CH_BW_40MHZ);
#endif
                    if (b) {
                        if (sprintf(response, "40MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiBand = 1;
                    }
                    break;
            }
            if (b)
                return AT_OK;
            else
                return AT_ERROR;
        case AT_READ_OP:
            if (wifiBand == 0) {
                if (sprintf(response, "20MHZ") < 0) {
                    LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                    return AT_ERROR;
                }
                return AT_OK;
            } else if(wifiBand == 1) {
                if (sprintf(response, "40MHZ") < 0) {
                    LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                    return AT_ERROR;
                }
                return AT_OK;
            } else {
                return AT_ERROR;
            }
            break;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_wifreq_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0;
    bool b = false;
    uint32_t freq = 0, offset = 0;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, (int *)&freq);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_frequency(freq);
#endif
            if (b) {
                wifiFreq = freq;
                return AT_OK;
            } else {
                wifiFreq = -1;
                return AT_ERROR;
            }
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiFreq) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_widatarate_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0;
    bool b = false;
    uint32_t rate = 0;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, (int *)&rate);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_rate(rate);
#endif
            if (b) {
                wifiRate = rate;
                LOGATCI(LOG_DEBUG, "pas_widatarate_handler [rate=%d] \n", wifiRate);
                gRateCodeWiFi = wifiRate;
                return AT_OK;
            } else {
                wifiRate = -1;
                return AT_ERROR;
            }
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiRate) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_wipow_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0;
    bool b = false;
    uint32_t gain = 0;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, (int *)&gain);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_power(gain);
#endif
            if (b) {
                wifiGain = gain;
                return AT_OK;
            } else {
                wifiGain = -1;
                return AT_ERROR;
            }
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiGain) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_witxpow_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, gain = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &gain);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_power(gain);
#endif
            wifiGain2 = (b ? gain : -1);
            return (b ? AT_OK : AT_ERROR);
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiGain2) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_witx_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(response);
    int err = 0, cmd = -1;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &cmd);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            if (cmd == 0) {
                b = atci_wifi_test_stop_tx();
            } else {
                b = atci_wifi_test_start_tx();
            }
#endif
            if (b)
                return AT_OK;
            else
                return AT_ERROR;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_wirx_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(response);
    int err = 0, cmd = -1;
    bool b = false;
    char srcAddr[MAC_ADDR_LEN] = {0};
    char dstAddr[MAC_ADDR_LEN] = {0};
    int64_t tmp = 0;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &cmd);
            if (err < 0) return -1;
            switch (cmd) {
                case 0:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_start_rx(srcAddr);
#endif
                    break;
                case 1:
                    err = at_tok_nextint64(&cmdline, &tmp);
                    memcpy(srcAddr, (char *)(&tmp), MAC_ADDR_LEN);
                    if (err < 0) return -1;
                    err = at_tok_nextint64(&cmdline, &tmp);
                    memcpy(dstAddr, (char *)(&tmp), MAC_ADDR_LEN);
                    if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_start_rx(srcAddr); // Rx only filter TA aka source mac address in Tx.
#endif
                    break;
            }
        default:
            break;
    }
    if(b)
        return AT_OK;
    else
        return AT_ERROR;
}

ATRESPONSE_t pas_wirpckg_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(cmdline);
    bool b = false;
    uint32_t pu4GoodFrameCount = 0;
    uint32_t pu4BadFrameCount = 0;

    switch (opType) {
        case AT_SET_OP:
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_clear_rx_count();
#endif
            if (b) {
                return AT_OK;
            } else {
                return AT_ERROR;
            }
        case AT_READ_OP:
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_get_rx_count(&pu4GoodFrameCount, &pu4BadFrameCount);
#endif
            if (sprintf(response, "%d,%d OK",
                    pu4GoodFrameCount, pu4BadFrameCount) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            if (b)
                return AT_OK;
            else
                return AT_ERROR;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_wirssi_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(cmdline);
    int rssi = -127, rxok = -1, rxerror = -1;
    bool b = false;

    switch (opType) {
        case AT_READ_OP:
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_get_rx_count(&rxok, &rxerror);
            LOGATCI(LOG_DEBUG, "ATCI_WIFI_get_rx_count got retrun: %s", b ? "true" : "false");
            LOGATCI(LOG_DEBUG, "RX ok:%d, error:%d", rxok, rxerror);
            b = atci_wifi_test_get_rssi(&rssi);
            LOGATCI(LOG_DEBUG, "ATCI_WIFI_get_rssi got retrun: %s", b ? "true" : "false");
            LOGATCI(LOG_DEBUG, "RSSI:%d", rssi);
#endif
            if (sprintf(response, "Current RSSI:%d, RX OK/ERR = %d/%d",
                    rssi, rxok, rxerror) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return (b ? AT_OK : AT_ERROR);
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_wigi_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, guardInterval = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &guardInterval);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_gi(guardInterval);
            if (sprintf(response, "Set guard interval:%d", guardInterval) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
#endif
            return (b ? AT_OK : AT_ERROR);
        default:
            break;
    }
    return AT_ERROR;
}


ATRESPONSE_t pas_wipreamble_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, preamble = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &preamble);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_preamble(preamble);
            if (sprintf(response, "Set preamble:%d", preamble) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
#endif
            return (b ? AT_OK : AT_ERROR);
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_witxpktlen_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, txPacketLength = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &txPacketLength);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_pkt_len(txPacketLength);
#endif
            wifiTxPacketLength = (b ? txPacketLength : -1);
            return (b ? AT_OK : AT_ERROR);
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiTxPacketLength) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_witxpktcnt_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, txPacketCount = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &txPacketCount);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_pkt_cnt(txPacketCount);
#endif
            wifiTxPacketCount = (b ? txPacketCount : -1);
            return (b ? AT_OK : AT_ERROR);
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiTxPacketCount) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_witxpktinterval_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, txPacketInterval = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &txPacketInterval);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_pkt_interval(txPacketInterval);
#endif
            wifiTxPacketinterval = (b ? txPacketInterval : -1);
            return (b ? AT_OK : AT_ERROR);
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiTxPacketinterval) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_wichbandwidth_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, cmdID = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &cmdID);
            if (err < 0) return -1;
            switch (cmdID) {
                case 0:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_cbw(ATCI_WIFI_CH_BW_20MHZ);
#endif
                    if (b) {
                        if (sprintf(response,
                                "Set channel bandwidth: 20MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiChannelBandwidth = 0;
                    }
                    break;
                case 1:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_cbw(ATCI_WIFI_CH_BW_40MHZ);
#endif
                    if (b) {
                        if (sprintf(response,
                                "Set channel bandwidth: 40MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiChannelBandwidth = 1;
                    }
                    break;
                case 2:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_cbw(ATCI_WIFI_CH_BW_80MHZ);
#endif
                    if (b) {
                        if (sprintf(response,
                                "Set channel bandwidth: 80MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiChannelBandwidth = 2;
                    }
                    break;
                case 3:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_cbw(ATCI_WIFI_CH_BW_160MHZ);
#endif
                    if (b) {
                        if (sprintf(response,
                                "Set channel bandwidth: 160MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiChannelBandwidth = 3;
                    }
                    break;
            }
            return (b ? AT_OK : AT_ERROR);
        case AT_READ_OP:
            if (wifiChannelBandwidth >= 0 && wifiChannelBandwidth < ATCI_WIFI_CH_BW_NUM) {
                if (sprintf(response, "Channel bandwidth: %d",
                        wifiChannelBandwidth) < 0) {
                    LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                    return AT_ERROR;
                }
            } else {
                if (sprintf(response, "No define for channel band") < 0) {
                    LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                    return AT_ERROR;
                }
                return AT_OK;
            }
            break;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_widatabandwidth_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, cmdID = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &cmdID);
            if (err < 0) return -1;
            switch (cmdID) {
                case 0:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_dbw(ATCI_WIFI_CH_BW_20MHZ);
#endif
                    if (b) {
                        if (sprintf(response, "Set data bandwidth: 20MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiDataBandwidth= 0;
                    }
                    break;
                case 1:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_dbw(ATCI_WIFI_CH_BW_40MHZ);
#endif
                    if (b) {
                        if (sprintf(response,
                                "Set data bandwidth: 40MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiDataBandwidth = 1;
                    }
                    break;
                case 2:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_dbw(ATCI_WIFI_CH_BW_80MHZ);
#endif
                    if (b) {
                        if (sprintf(response,
                                "Set data bandwidth: 80MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiDataBandwidth = 2;
                    }
                    break;
                case 3:
#ifdef MTK_WLAN_FEATURE
                    b = atci_wifi_test_set_dbw(ATCI_WIFI_CH_BW_160MHZ);
#endif
                    if (b) {
                        if (sprintf(response,
                                "Set data bandwidth: 160MHZ") < 0) {
                            LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                            return AT_ERROR;
                        }
                        wifiDataBandwidth = 3;
                    }
                    break;
            }
            return (b ? AT_OK : AT_ERROR);
        case AT_READ_OP:
            if (wifiDataBandwidth >= 0 && wifiDataBandwidth < ATCI_WIFI_CH_BW_NUM) {
                if (sprintf(response, "Data bandwidth: %d",
                        wifiDataBandwidth) < 0) {
                    LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                    return AT_ERROR;
                }
            } else {
                if (sprintf(response, "No define for data bandwidth") < 0) {
                    LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                    return AT_ERROR;
                }
                return AT_OK;
            }
            break;
        default:
            break;
    }
    return AT_ERROR;
}


ATRESPONSE_t pas_wiprimarychset_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, primaryChannelOffset = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &primaryChannelOffset);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_pri_ch(primaryChannelOffset);
#endif
            wifiPrimaryChannelOffset = (b ? primaryChannelOffset : -1);
            return (b ? AT_OK : AT_ERROR);
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiPrimaryChannelOffset) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_witxdatarate_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, txDataRate = 0;
    bool b = false;

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &txDataRate);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            b = atci_wifi_test_set_tx_data_rate(txDataRate);
#endif
            wifiTxDataRate = (b ? txDataRate : -1);
            return (b ? AT_OK : AT_ERROR);
        case AT_READ_OP:
            if (sprintf(response, "%d", wifiTxDataRate) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return AT_OK;
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_wirxstart_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(response);
    int err = 0, cmd = -1;
    bool b = false;
    char srcAddr[MAC_ADDR_LEN] = {0};

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &cmd);
            if (err < 0) return -1;
#ifdef MTK_WLAN_FEATURE
            if (cmd == 0) {
                b = atci_wifi_test_stop_rx();
            } else {
                b = atci_wifi_test_start_rx(srcAddr);
            }
#endif
            return (b ? AT_OK : AT_ERROR);
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_witestset_handler(char* cmdline, ATOP_t opType, char* response) {
    int err = 0, index = -1, data = -1;
#ifdef MTK_WLAN_FEATURE
    int operation = -1;
#endif

    switch (opType) {
        case AT_SET_OP:
            err = at_tok_nextint(&cmdline, &index);
            if (err < 0) {
                return AT_ERROR;
            }

            err = at_tok_nextint(&cmdline, &data);
            if (err < 0) {
                return AT_ERROR;
            }
#ifdef MTK_WLAN_FEATURE
            operation = WIFI_TEST_set(index, data, NULL, NULL);
            if (sprintf(response, "Set index:%d to %d %s", index, data,
                        (operation == 0 ? "successful" : "failed")) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return (operation == 0 ? AT_OK : AT_ERROR);
#else
            LOGATCI(LOG_DEBUG,
                "AT_SET_OP failed - MTK_WLAN_FEATURE is disabled.\n");
            if (sprintf(response, "Set index:%d to %d failed", index, data) < 0)
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
            return AT_ERROR;
#endif
        case AT_READ_OP:
            err = at_tok_nextint(&cmdline, &index);
            if (err < 0) {
                return AT_ERROR;
            }
#ifdef MTK_WLAN_FEATURE
            operation = WIFI_TEST_get(index, 0, NULL, (uint32_t *)&data);
            if (sprintf(response, "Get index:%d --> data:%d %s", index, data,
                    (operation == 0 ? "successful" : "failed")) < 0) {
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
                return AT_ERROR;
            }
            return (operation == 0 ? AT_OK : AT_ERROR);
#else
            LOGATCI(LOG_DEBUG,
                "AT_READ_OP failed - MTK_WLAN_FEATURE is disabled.\n");
            if (sprintf(response, "Get index:%d --> data:%d failed",
                    index, data) < 0)
                LOGATCI(LOG_ERR, "sprintf failed: %d", __LINE__);
            return AT_ERROR;
#endif
        default:
            break;
    }
    return AT_ERROR;
}

ATRESPONSE_t pas_witestmode_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(cmdline);
    UNUSED(opType);
    UNUSED(response);
    return AT_ERROR;
}

ATRESPONSE_t pas_witx2_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(cmdline);
    UNUSED(opType);
    UNUSED(response);
    return AT_ERROR;
}

ATRESPONSE_t pas_wirx2_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(cmdline);
    UNUSED(opType);
    UNUSED(response);
    return AT_ERROR;
}

ATRESPONSE_t pas_wimac_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(cmdline);
    UNUSED(opType);
    UNUSED(response);
    return AT_ERROR;
}

ATRESPONSE_t pas_wimacck_handler(char* cmdline, ATOP_t opType, char* response) {
    UNUSED(cmdline);
    UNUSED(opType);
    UNUSED(response);
    return AT_ERROR;
}


/*******************************************************************************
*                         P R I V A T E   F U N C T I O N
********************************************************************************
*/
static bool atci_wifi_test_set_mode (ATCI_WIFI_MODE Mode) {

    if(Mode >= ATCI_WIFI_MODE_NUM) {
        return false;
    }
    else {
        g_u4WifiTestMode = Mode;
        LOGATCI(LOG_DEBUG, "Mode:%d -> WifiTestMode: %d", Mode, g_u4WifiTestMode);
        return true;
    }
}

static bool atci_wifi_test_get_mode (uint32_t *SupportedMode) {

    if(SupportedMode == NULL) {
        return false;
    }

    return true;
}

static bool atci_wifi_test_set_bandwidth (ATCI_WIFI_CHANNEL_BANDWIDTH Bandwidth) {

    if (atci_wifi_test_set_cbw(Bandwidth) == false) {
        return false;
    }
    if (atci_wifi_test_set_dbw(Bandwidth) == false) {
        return false;
    }

    return true;
}

static bool atci_wifi_test_set_frequency (uint32_t FreqInMHz) {

    if(FreqInMHz >= CHNL_FREQ_2G(1) && FreqInMHz <= CHNL_FREQ_2G(14)) { //Ch1~Ch13
        g_u4ChNum = FREQ_CHNL(FreqInMHz, BAND_2G4);
        g_u4RFBand = HQA_RFBAND_2G4;
    } else if (FreqInMHz == 3665){
        g_u4ChNum = 133;
        g_u4RFBand = HQA_RFBAND_2G4;
    } else if (FreqInMHz == 3685){
        g_u4ChNum = 137;
        g_u4RFBand = HQA_RFBAND_2G4;
    } else if(FreqInMHz >= CHNL_FREQ_4G(183) && FreqInMHz <= CHNL_FREQ_4G(196)) { //Ch183~Ch196
        g_u4ChNum = FREQ_CHNL(FreqInMHz, BAND_4G);
        g_u4RFBand = HQA_RFBAND_2G4;
    } else if(FreqInMHz >= CHNL_FREQ_5G(34) && FreqInMHz <= CHNL_FREQ_5G(165)) { //Ch34~Ch165
        g_u4ChNum = FREQ_CHNL(FreqInMHz, BAND_5G);
        g_u4RFBand = HQA_RFBAND_5G;
    } else {
        g_u4ChNum = -1;
    }

    LOGATCI(LOG_DEBUG, "Freq:%d -> ChNum:%d, RFBand: %d", FreqInMHz, g_u4ChNum, g_u4RFBand);
    return true;
}

static bool atci_wifi_test_set_rate(uint32_t Rate) {

    /* validate rate by mode/bandwidth settings */
    if(g_u4WifiTestMode == ATCI_WIFI_MODE_80211B_ONLY) {
        switch(Rate) {
        case 100:
            g_u4Rate = HQA_RATE_CCK_LP_1MBPS;
            break;

        case 200:
            g_u4Rate = HQA_RATE_CCK_LP_2MBPS;
            break;

        case 550:
            g_u4Rate = HQA_RATE_CCK_LP_5_5MBPS;
            break;

        case 1100:
            g_u4Rate = HQA_RATE_CCK_LP_11MBPS;
            break;

        default:
            return false;
        }
    }
    else if(g_u4WifiTestMode == ATCI_WIFI_MODE_80211G_ONLY || g_u4WifiTestMode == ATCI_WIFI_MODE_80211A_ONLY) {
        switch(Rate) {
        case 600:
            g_u4Rate = HQA_RATE_OFDM_6MBPS;
            break;

        case 900:
            g_u4Rate = HQA_RATE_OFDM_9MBPS;
            break;

        case 1200:
            g_u4Rate = HQA_RATE_OFDM_12MBPS;
            break;

        case 1800:
            g_u4Rate = HQA_RATE_OFDM_18MBPS;
            break;

        case 2400:
            g_u4Rate = HQA_RATE_OFDM_24MBPS;
            break;

        case 3600:
            g_u4Rate = HQA_RATE_OFDM_36MBPS;
            break;

        case 4800:
            g_u4Rate = HQA_RATE_OFDM_48MBPS;
            break;

        case 5400:
            g_u4Rate = HQA_RATE_OFDM_54MBPS;
            break;

        default:
            return false;
        }
    }
    else if(g_u4WifiTestMode == ATCI_WIFI_MODE_80211N_ONLY && g_u4CBW == HQA_BW_20) {
        switch(Rate) {
        case 650:
            g_u4Rate = HQA_RATE_MCS0;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 720:
            g_u4Rate = HQA_RATE_MCS0;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 1300:
            g_u4Rate = HQA_RATE_MCS1;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 1440:
            g_u4Rate = HQA_RATE_MCS1;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 1950:
            g_u4Rate = HQA_RATE_MCS2;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 2170:
            g_u4Rate = HQA_RATE_MCS2;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 2600:
            g_u4Rate = HQA_RATE_MCS3;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 2890:
            g_u4Rate = HQA_RATE_MCS3;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 3900:
            g_u4Rate = HQA_RATE_MCS4;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 4330:
            g_u4Rate = HQA_RATE_MCS4;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 5200:
            g_u4Rate = HQA_RATE_MCS5;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 5780:
            g_u4Rate = HQA_RATE_MCS5;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 5850:
            g_u4Rate = HQA_RATE_MCS6;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 6500:
            g_u4Rate = HQA_RATE_MCS7;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 7200:
            g_u4Rate = HQA_RATE_MCS7;
            g_u4GIType = HQA_GI_SHORT;
            break;
        default:
            return false;
        }
    }
    else if(g_u4WifiTestMode == ATCI_WIFI_MODE_80211N_ONLY && g_u4CBW == HQA_BW_40) {
        switch(Rate) {
        case 1350:
            g_u4Rate = HQA_RATE_MCS0;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 1500:
            g_u4Rate = HQA_RATE_MCS0;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 2700:
            g_u4Rate = HQA_RATE_MCS1;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 3000:
            g_u4Rate = HQA_RATE_MCS1;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 4050:
            g_u4Rate = HQA_RATE_MCS2;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 4500:
            g_u4Rate = HQA_RATE_MCS2;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 5400:
            g_u4Rate = HQA_RATE_MCS3;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 6000:
            g_u4Rate = HQA_RATE_MCS3;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 8100:
            g_u4Rate = HQA_RATE_MCS4;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 9000:
            g_u4Rate = HQA_RATE_MCS4;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 10800:
            g_u4Rate = HQA_RATE_MCS5;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 12000:
            g_u4Rate = HQA_RATE_MCS5;
            g_u4GIType = HQA_GI_SHORT;
            break;
        case 12150:
            g_u4Rate = HQA_RATE_MCS6;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 13500:
            g_u4Rate = HQA_RATE_MCS7;
            g_u4GIType = HQA_GI_NORMAL;
            break;
        case 15000:
            g_u4Rate = HQA_RATE_MCS7;
            g_u4GIType = HQA_GI_SHORT;
            break;
        default:
            return false;
        }
    }
    else {
        return false;
    }

    return true;
}

static bool atci_wifi_test_set_power(uint32_t Gain) {

    if(Gain > 6350) {
        return false;
    }

    g_u4Power = Gain / 50;

    LOGATCI(LOG_DEBUG, "Gain: %d -> g_u4Power: %d", Gain, g_u4Power);
    return true;
}

static bool atci_wifi_test_clear_rx_count() {

    struct hqa_get_rx_statistics_all_in rx_statistics_setting = {0};
    struct hqa_get_rx_statistics_all_out rx_statistics ={0};

    rx_statistics_setting.type_mask = 0xf;
    rx_statistics_setting.dbdc_band = g_u4DbdcBand;

    if (HQA_GetRxStatisticsAllExt(rx_statistics_setting, &rx_statistics) == false) {
        LOGATCI(LOG_ERR, "HQA_GetRxStatisticsAllExt fail");
        return false;
    }

    return true;
}

static bool atci_wifi_test_get_rx_count(uint32_t *pu4GoodFrameCount, uint32_t *pu4BadFrameCount) {

    struct hqa_get_rx_statistics_all_in rx_statistics_setting = {0};
    struct hqa_get_rx_statistics_all_out rx_statistics ={0};
    struct hqa_rx_stat_band_info *band_info;

    rx_statistics_setting.type_mask = 0xf;
    rx_statistics_setting.dbdc_band = g_u4DbdcBand;

    if (HQA_GetRxStatisticsAllExt(rx_statistics_setting, &rx_statistics) == false) {
        LOGATCI(LOG_ERR, "HQA_GetRxStatisticsAllExt fail");
        return false;
    }

    band_info = &rx_statistics.band_info[g_u4DbdcBand];

    *pu4GoodFrameCount = band_info->mac_fcs_ok_cnt;
    *pu4BadFrameCount = band_info->mac_rx_mdrdy_cnt - *pu4GoodFrameCount;

    LOGATCI(LOG_DEBUG, "GoodCount:%d, BadCount: %d", *pu4GoodFrameCount, *pu4BadFrameCount);

    return true;
}

static bool atci_wifi_test_get_rssi(int32_t *rssi) {

    struct hqa_get_rx_statistics_all_in rx_statistics_setting = {0};
    struct hqa_get_rx_statistics_all_out rx_statistics ={0};
    struct hqa_rx_stat_path_info *path_info;
    int8_t tmp = 0;

    rx_statistics_setting.type_mask = 0xf;
    rx_statistics_setting.dbdc_band = g_u4DbdcBand;

    if (HQA_GetRxStatisticsAllExt(rx_statistics_setting, &rx_statistics) == false) {
        LOGATCI(LOG_ERR, "HQA_GetRxStatisticsAllExt fail");
        return false;
    }

    path_info = &rx_statistics.path_info[HQA_ANT_WF0];

    tmp = (int8_t)(path_info->rssi & BITS(0,7));
    *rssi = tmp;

    LOGATCI(LOG_DEBUG, "rssi:%d\n", *rssi);

    return true;
}

static bool atci_wifi_test_set_gi(ATCI_WIFI_GI_TYPE GIType) {

    if (GIType == ATCI_WIFI_GI_TYPE_NORMAL_GI){
        g_u4GIType = HQA_GI_NORMAL;
    } else if (GIType == ATCI_WIFI_GI_TYPE_SHORT_GI) {
        g_u4GIType = HQA_GI_SHORT;
    } else {
    	return false;
    }

    LOGATCI(LOG_DEBUG, "GIType: %d -> g_u4GIType: %d", GIType, g_u4GIType);
    return true;
}

static bool atci_wifi_test_set_preamble(ATCI_WIFI_PREAMBLE_T PreambleType) {

    switch (PreambleType) {
    case ATCI_WIFI_PREAMBLE_NORMAL:
        if(g_u4WifiTestMode == ATCI_WIFI_MODE_80211B_ONLY) {
            g_u4Preamble = HQA_PREAMBLE_TYPE_CCK;
        } else {
            g_u4Preamble = HQA_PREAMBLE_TYPE_OFDM;
        }
        break;
    case ATCI_WIFI_PREAMBLE_CCK_SHORT:
        g_u4Preamble = HQA_PREAMBLE_TYPE_CCK;
        break;
    case ATCI_WIFI_PREAMBLE_11N_MM:
        g_u4Preamble = HQA_PREAMBLE_TYPE_HT_MM;
        break;
    case ATCI_WIFI_PREAMBLE_11N_GF:
        g_u4Preamble = HQA_PREAMBLE_TYPE_HT_GF;
        break;
    case ATCI_WIFI_PREAMBLE_11AC:
        g_u4Preamble = HQA_PREAMBLE_TYPE_VHT;
        break;
    default:
        return false;
    }

    LOGATCI(LOG_DEBUG, "PreambleType: %d, g_u4WifiTestMode: %d-> g_u4Preamble: %d", g_u4WifiTestMode, g_u4WifiTestMode, g_u4Preamble);
    return true;
}

static bool atci_wifi_test_set_pkt_len(int PktLen) {

    if (PktLen <= 0) {
        return false;
    }

    g_u4PktLen = PktLen;
    LOGATCI(LOG_DEBUG, "PktLen: %d -> g_u4PktLen:%d", PktLen, g_u4PktLen);

    return true;
}

static bool atci_wifi_test_set_pkt_cnt(int PktCnt) {

    if (PktCnt < 0) {
        return false;
    }

    g_u4PktCnt = PktCnt;
    LOGATCI(LOG_DEBUG, "PktCnt: %d -> g_u4PktCnt:%d", PktCnt, g_u4PktCnt);

    return true;
}

static bool atci_wifi_test_set_pkt_interval(int PktInterval) {

    if (PktInterval < 20 || PktInterval > 1000) {
        return false;
    }

    g_u4PktInterval = PktInterval;
    LOGATCI(LOG_DEBUG, "PktInterval: %d -> g_u4PktInterval:%d", PktInterval, g_u4PktInterval);

    return true;
}

static bool atci_wifi_test_set_cbw(ATCI_WIFI_CHANNEL_BANDWIDTH Cbw) {

    if (Cbw == ATCI_WIFI_CH_BW_20MHZ) {
        g_u4CBW = HQA_BW_20;
    } else if (Cbw == ATCI_WIFI_CH_BW_40MHZ) {
        g_u4CBW = HQA_BW_40;
    } else if (Cbw == ATCI_WIFI_CH_BW_80MHZ) {
        g_u4CBW = HQA_BW_80;
    } else if (Cbw == ATCI_WIFI_CH_BW_160MHZ) {
        g_u4CBW = HQA_BW_160C;
    } else {
        return false;
    }

    LOGATCI(LOG_DEBUG, "Cbw: %d -> g_u4CBW:%d", Cbw, g_u4CBW);
    return true;
}

static bool atci_wifi_test_set_dbw(ATCI_WIFI_CHANNEL_BANDWIDTH Dbw) {

    if (Dbw == ATCI_WIFI_CH_BW_20MHZ) {
        g_u4DBW = HQA_BW_20;
    } else if (Dbw == ATCI_WIFI_CH_BW_40MHZ) {
        g_u4DBW = HQA_BW_40;
    } else if (Dbw == ATCI_WIFI_CH_BW_80MHZ) {
        g_u4DBW = HQA_BW_80;
    } else if (Dbw == ATCI_WIFI_CH_BW_160MHZ) {
        g_u4DBW = HQA_BW_160C;
    } else {
        return false;
    }

    LOGATCI(LOG_DEBUG, "Dbw: %d -> g_u4DBW:%d", Dbw, g_u4DBW);
    return true;
}

static bool atci_wifi_test_set_pri_ch(uint8_t PriCh) {

    if(PriCh >= ATCI_WIFI_PRI_CH_SETTING_NUM) {
        return false;
    }

    g_u4PriCh = PriCh;

    LOGATCI(LOG_DEBUG, "PriCh: %d -> g_u4PriCh:%d", PriCh, g_u4PriCh);
    return true;
}

static bool atci_wifi_test_set_tx_data_rate(int TxDataRate) {

    if (g_u4WifiTestMode == ATCI_WIFI_MODE_80211B_ONLY) {

        switch (TxDataRate) {
        case ATCI_WIFI_RATE_CCK_1MBPS:
            g_u4Rate = HQA_RATE_CCK_LP_1MBPS;
            break;
        case ATCI_WIFI_RATE_CCK_2MBPS:
            g_u4Rate = HQA_RATE_CCK_LP_2MBPS;
            break;
        case ATCI_WIFI_RATE_CCK_5_5MBPS:
            g_u4Rate = HQA_RATE_CCK_LP_5_5MBPS;
            break;
        case ATCI_WIFI_RATE_CCK_11MBPS:
            g_u4Rate = HQA_RATE_CCK_LP_11MBPS;
            break;
        default:
            return false;
        }

    } else if (g_u4WifiTestMode == ATCI_WIFI_MODE_80211G_ONLY || g_u4WifiTestMode == ATCI_WIFI_MODE_80211A_ONLY) {

        switch (TxDataRate) {
        case ATCI_WIFI_RATE_OFDM_6MBPS:
            g_u4Rate = HQA_RATE_OFDM_6MBPS;
            break;
        case ATCI_WIFI_RATE_OFDM_9MBPS:
            g_u4Rate = HQA_RATE_OFDM_9MBPS;
            break;
        case ATCI_WIFI_RATE_OFDM_12MBPS:
            g_u4Rate = HQA_RATE_OFDM_12MBPS;
            break;
        case ATCI_WIFI_RATE_OFDM_18MBPS:
            g_u4Rate = HQA_RATE_OFDM_18MBPS;
            break;
        case ATCI_WIFI_RATE_OFDM_24MBPS:
            g_u4Rate = HQA_RATE_OFDM_24MBPS;
            break;
        case ATCI_WIFI_RATE_OFDM_36MBPS:
            g_u4Rate = HQA_RATE_OFDM_36MBPS;
            break;
        case ATCI_WIFI_RATE_OFDM_48MBPS:
            g_u4Rate = HQA_RATE_OFDM_48MBPS;
            break;
        case ATCI_WIFI_RATE_OFDM_54MBPS:
            g_u4Rate = HQA_RATE_OFDM_54MBPS;
            break;
        default:
            return false;
        }
    } else {
        g_u4Rate = TxDataRate;  //MCS rate interface is match with HQA CMD.
    }


    LOGATCI(LOG_DEBUG, "TxDataRate: %d, g_u4WifiTestMode: %d -> g_u4Rate: %d", TxDataRate, g_u4WifiTestMode, g_u4Rate);
    return true;
}

static bool atci_wifi_test_get_chip_cap() {

    static bool fgGetChipCapabilityFlag = false;

    /* avoid overmuch ioctl */
    if (fgGetChipCapabilityFlag == true) {
        return true;
    }

    if (HQA_GetChipCapability(&cap) == false) {
        LOGATCI(LOG_ERR, "HQA_GetChipCapability");
        return false;
    }

    fgGetChipCapabilityFlag = true;

    return true;
}

static bool atci_wifi_test_is_dbdc_always_on() {

    if (HQA_GetChipCapability(&cap) == false) {
        LOGATCI(LOG_ERR, "HQA_GetChipCapability");
        return false;
    }

    if (cap.ext_cap.feature1 & EXT_CAP_FEATURE1_CONNAC3) {
        return true;
    }

    return false;
}

static bool atci_wifi_test_start_tx() {

    uint8_t BroadCastAddr[MAC_ADDR_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t NullAddr[MAC_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    struct hqa_set_tx_path tx_ant_setting = {0};
    struct hqa_set_rx_path rx_ant_setting = {0};
    struct hqa_dbdc_set_channel channel_setting = {0};
    struct hqa_set_tx_content packet_content_setting = {0};
    struct hqa_dbdc_start_tx dbdc_start_tx_settings = {0};


    if (atci_wifi_test_is_dbdc_always_on() == true) { //Adapt to connac3 arch.
        if (g_u4RFBand == HQA_RFBAND_2G4) {
            g_u4DbdcBand = HQA_DBDC_BAND_0;
        } else {
            g_u4DbdcBand = HQA_DBDC_BAND_1;
        }
    }

    /* ===== Step 1 HQA_SetTxPath ====== */
    tx_ant_setting.ant_idx = BIT(HQA_ANT_WF0);
    tx_ant_setting.dbdc_band = g_u4DbdcBand;
    if (HQA_SetTxPath(tx_ant_setting) == false){
        return false;
    }

    /* ===== Step 2 HQA_SetRxPath ===== */
    rx_ant_setting.ant_idx = BIT(HQA_ANT_WF0);
    rx_ant_setting.dbdc_band = g_u4DbdcBand;
    if (HQA_SetRxPath(rx_ant_setting) == false){
        return false;
    }

    /* ===== Step 3 HQA_DBDCSetChannel ===== */
    channel_setting.dbdb_band = g_u4DbdcBand;
    channel_setting.center_channel0 = g_u4ChNum;
    channel_setting.center_channel1 = 0;
    channel_setting.cbw = g_u4CBW;
    channel_setting.dbw = g_u4DBW;
    channel_setting.primary_channnel = g_u4PriCh;
    channel_setting.reason = 0;
    channel_setting.rf_band = g_u4RFBand;
    channel_setting.out_band_freq = 0;
    if (HQA_DBDCSetChannel(channel_setting) == false) {
        return false;
    }

    /* ===== Step 4 HQA_DBDCSetTxContent ===== */
    packet_content_setting.dbdc_band = g_u4DbdcBand;
    packet_content_setting.fc = 0;
    packet_content_setting.dur = 0;
    packet_content_setting.seq = 0;
    packet_content_setting.fixed_payload = HQA_PAYLOAD_TYPE_RANDOM;
    packet_content_setting.tx_len = g_u4PktLen;
    packet_content_setting.payload_len = 0;
    memcpy(&packet_content_setting.DA, BroadCastAddr, sizeof(packet_content_setting.DA));
    memcpy(&packet_content_setting.SA, NullAddr, sizeof(packet_content_setting.SA));
    memcpy(&packet_content_setting.BSSID, NullAddr, sizeof(packet_content_setting.BSSID));
    if (HQA_DBDCSetTxContent(packet_content_setting) == false) {
        return false;
    }

    /* ===== Step 5 HQA_DBDCStartTx ===== */
    dbdc_start_tx_settings.dbdc_band = g_u4DbdcBand;
    dbdc_start_tx_settings.pkt_cnt = g_u4PktCnt;
    dbdc_start_tx_settings.preamble = g_u4Preamble;
    dbdc_start_tx_settings.rate = g_u4Rate;
    dbdc_start_tx_settings.pwr = g_u4Power;
    dbdc_start_tx_settings.stbc = 0;
    dbdc_start_tx_settings.ldpc = 0;
    dbdc_start_tx_settings.ibf = 0;
    dbdc_start_tx_settings.ebf = 0;
    dbdc_start_tx_settings.wlan_idx = 0;
    dbdc_start_tx_settings.pkt_interval = g_u4PktInterval;
    dbdc_start_tx_settings.gi = g_u4GIType;
    dbdc_start_tx_settings.tx_path = BIT(HQA_ANT_WF0);
    dbdc_start_tx_settings.nss = 1;
    dbdc_start_tx_settings.hwtx = 0;
    dbdc_start_tx_settings.puncture = 0xffff;
    if (HQA_DBDCStartTx(dbdc_start_tx_settings) == false) {
        return false;
    }

    return true;
}

static bool atci_wifi_test_stop_tx() {

    struct hqa_dbdc_stop_tx dbdc_stop_tx_settings = {0};

    dbdc_stop_tx_settings.dbdc_band = g_u4DbdcBand; // depends on start tx dbdc band.

    if (HQA_DBDCStopTx(dbdc_stop_tx_settings) == false) {
        return false;
    }

    return true;
}

static bool atci_wifi_test_start_rx(char *aucSrcAddr) {

    struct hqa_set_tx_path tx_ant_setting = {0};
    struct hqa_set_rx_path rx_ant_setting = {0};
    struct hqa_dbdc_set_channel channel_setting = {0};
    struct hqa_dbdc_start_rx dbdc_start_rx_settings = {0};

    if (!aucSrcAddr) {
        LOGATCI(LOG_DEBUG, "aucSrcAddr is Null");
        return false;
    }

    if (atci_wifi_test_is_dbdc_always_on() == true) { //Adapt to connac3 arch.
        if (g_u4RFBand == HQA_RFBAND_2G4) {
            g_u4DbdcBand = HQA_DBDC_BAND_0;
        } else {
            g_u4DbdcBand = HQA_DBDC_BAND_1;
        }
    }

    /* ===== Step 1 HQA_SetTxPath ====== */
    tx_ant_setting.ant_idx = BIT(HQA_ANT_WF0);
    tx_ant_setting.dbdc_band = g_u4DbdcBand;
    if (HQA_SetTxPath(tx_ant_setting) == false){
        return false;
    }

    /* ===== Step 2 HQA_SetRxPath ===== */
    rx_ant_setting.ant_idx = BIT(HQA_ANT_WF0);
    rx_ant_setting.dbdc_band = g_u4DbdcBand;
    if (HQA_SetRxPath(rx_ant_setting) == false){
        return false;
    }

    /* ===== Step 3 HQA_DBDCSetChannel ===== */
    channel_setting.dbdb_band = g_u4DbdcBand;
    channel_setting.center_channel0 = g_u4ChNum;
    channel_setting.center_channel1 = 0;
    channel_setting.cbw = g_u4CBW;
    channel_setting.dbw = g_u4DBW;
    channel_setting.primary_channnel = g_u4PriCh;
    channel_setting.reason = 0;
    channel_setting.rf_band = g_u4RFBand;
    channel_setting.out_band_freq = 0;
    if (HQA_DBDCSetChannel(channel_setting) == false) {
        return false;
    }

    /* ===== Step 4 HQA_DBDCStartRxExt ===== */
    dbdc_start_rx_settings.dbdc_band = g_u4DbdcBand;
    memcpy(&dbdc_start_rx_settings.own_mac, aucSrcAddr, sizeof(dbdc_start_rx_settings.own_mac));
    dbdc_start_rx_settings.rx_path = BIT(HQA_ANT_WF0);
    dbdc_start_rx_settings.sta_id = 0;
    dbdc_start_rx_settings.preamble = g_u4Preamble;
    dbdc_start_rx_settings.gi = g_u4GIType;
    dbdc_start_rx_settings.user_idx = 0;
    if (HQA_DBDCStartRxExt(dbdc_start_rx_settings) == false) {
        return false;
    }

    return true;
}

static bool atci_wifi_test_stop_rx() {

    struct hqa_dbdc_stop_rx dbdc_stop_rx_settings = {0};

    dbdc_stop_rx_settings.dbdc_band = g_u4DbdcBand; // depends on start rx dbdc band.

    if (HQA_DBDCStopRX(dbdc_stop_rx_settings) == false) {
        return false;
    }

    return true;
}