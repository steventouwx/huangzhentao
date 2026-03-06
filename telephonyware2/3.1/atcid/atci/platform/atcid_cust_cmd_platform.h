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

#ifndef ATCID_CUST_CMD_PLATFORM_H
#define ATCID_CUST_CMD_PLATFORM_H

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/
/* Wi-Fi TEST MODE */
typedef enum _tagATCI_WIFI_MODE {
    ATCI_WIFI_MODE_BY_API_CONTROL   = 0,
    ATCI_WIFI_MODE_CW_ONLY          = 1,
    ATCI_WIFI_MODE_80211A_ONLY      = 2,
    ATCI_WIFI_MODE_80211B_ONLY      = 3,
    ATCI_WIFI_MODE_80211G_ONLY      = 4,
    ATCI_WIFI_MODE_80211N_ONLY      = 5,
    ATCI_WIFI_MODE_80211AC_ONLY     = 6,
    ATCI_WIFI_MODE_NUM              = 7
} ATCI_WIFI_MODE, *P_ATCI_WIFI_MODE;

/* RF Band */
typedef enum _tagATCI_WIFI_RFBAND {
    ATCI_WIFI_RFBAND_2G4  = 0,
    ATCI_WIFI_RFBAND_5G   = 1,
    ATCI_WIFI_RFBAND_NUM  = 2,
} ATCI_WIFI_RFBAND, *P_ATCI_WIFI_RFBAND;

/* Guard Interval Type */
typedef enum _tagATCI_WIFI_GI_TYPE {
    ATCI_WIFI_GI_TYPE_NORMAL_GI = 0,
    ATCI_WIFI_GI_TYPE_SHORT_GI  = 1
} ATCI_WIFI_GI_TYPE, *P_ATCI_WIFI_GI_TYPE;

/* Preamble */
typedef enum _tagATCI_WIFI_PREAMBLE {
    ATCI_WIFI_PREAMBLE_NORMAL = 0,
    ATCI_WIFI_PREAMBLE_CCK_SHORT,
    ATCI_WIFI_PREAMBLE_11N_MM,
    ATCI_WIFI_PREAMBLE_11N_GF,
    ATCI_WIFI_PREAMBLE_11AC,
    ATCI_WIFI_PREAMBLE_NUM
} ATCI_WIFI_PREAMBLE_T;

/* Channel Bandwidth Type */
typedef enum _tagATCI_WIFI_CHANNEL_BANDWIDTH {
    ATCI_WIFI_CH_BW_20MHZ = 0,
    ATCI_WIFI_CH_BW_40MHZ,
    ATCI_WIFI_CH_BW_80MHZ,
    ATCI_WIFI_CH_BW_160MHZ,
    ATCI_WIFI_CH_BW_NUM
}ATCI_WIFI_CHANNEL_BANDWIDTH, *P_ATCI_WIFI_CHANNEL_BANDWIDTH;

/* Primary channel offset (in unit of 20MHZ) */
typedef enum _tagATCI_WIFI_PRI_CHANNEL_SETTING {
    ATCI_WIFI_PRI_CH_SETTING_0 = 0,
    ATCI_WIFI_PRI_CH_SETTING_1,
    ATCI_WIFI_PRI_CH_SETTING_2,
    ATCI_WIFI_PRI_CH_SETTING_3,
    ATCI_WIFI_PRI_CH_SETTING_4,
    ATCI_WIFI_PRI_CH_SETTING_5,
    ATCI_WIFI_PRI_CH_SETTING_6,
    ATCI_WIFI_PRI_CH_SETTING_7,
    ATCI_WIFI_PRI_CH_SETTING_NUM
}ATCI_WIFI_PRI_CHANNEL_SETTING, *P_ATCI_WIFI_PRI_CHANNEL_SETTING;

/* WIFI_TEST_TxDataRate */
typedef enum _tagATCI_WIFI_TX_DATA_RATE {
    ATCI_WIFI_RATE_AUTO          = -1,
    ATCI_WIFI_RATE_CCK_1MBPS     = 0,
    ATCI_WIFI_RATE_CCK_2MBPS     = 1,
    ATCI_WIFI_RATE_CCK_5_5MBPS   = 2,
    ATCI_WIFI_RATE_CCK_11MBPS    = 3,
    ATCI_WIFI_RATE_OFDM_6MBPS    = 4,
    ATCI_WIFI_RATE_OFDM_9MBPS    = 5,
    ATCI_WIFI_RATE_OFDM_12MBPS   = 6,
    ATCI_WIFI_RATE_OFDM_18MBPS   = 7,
    ATCI_WIFI_RATE_OFDM_24MBPS   = 8,
    ATCI_WIFI_RATE_OFDM_36MBPS   = 9,
    ATCI_WIFI_RATE_OFDM_48MBPS   = 10,
    ATCI_WIFI_RATE_OFDM_54MBPS   = 11,
} ATCI_WIFI_TX_DATA_RATE, *P_ATCI_WIFI_TX_DATA_RATE;
/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/
#define BAND_2G4 2407000
#define BAND_4G 4000000
#define BAND_5G 5000000
#define CHNL_FREQ_2G(n) (((n) == 14) ? 2484000 : (BAND_2G4 + 5000 * (n)))
#define CHNL_FREQ_4G(n) ((BAND_4G + 5000 * (n)))
#define CHNL_FREQ_5G(n) ((BAND_5G + 5000 * (n)))
#define FREQ_CHNL(_freq, _band) (((_freq == 2484000) ? 14 : (((_freq) - (_band)) / 5000)))
/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/
ATRESPONSE_t pas_wienable_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wimode_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wiband_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wifreq_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_widatarate_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wipow_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_witxpow_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_witx_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wirx_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wirpckg_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wirssi_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wigi_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wipreamble_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_witxpktlen_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_witxpktcnt_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_witxpktinterval_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wichbandwidth_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_widatabandwidth_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wiprimarychset_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_witxdatarate_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wirxstart_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_witestset_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_witestmode_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_witx2_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wirx2_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wimac_handler(char* cmdline, ATOP_t opType, char* response);
ATRESPONSE_t pas_wimacck_handler(char* cmdline, ATOP_t opType, char* response);
#endif
