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

#ifndef ATCI_PQ_CMD_H
#define ATCI_PQ_CMD_H

#include "atcid_util.h"

#include <linux/ioctl.h>

#define PANEL_MAX 4
enum PQ_DC_index_t {
  BlackEffectEnable = 0,
  WhiteEffectEnable,
  StrongBlackEffect,
  StrongWhiteEffect,
  AdaptiveBlackEffect,
  AdaptiveWhiteEffect,
  ScenceChangeOnceEn,
  ScenceChangeControlEn,
  ScenceChangeControl,
  ScenceChangeTh1,
  ScenceChangeTh2,
  ScenceChangeTh3,
  ContentSmooth1,
  ContentSmooth2,
  ContentSmooth3,
  MiddleRegionGain1,
  MiddleRegionGain2,
  BlackRegionGain1,
  BlackRegionGain2,
  BlackRegionRange,
  BlackEffectLevel,
  BlackEffectParam1,
  BlackEffectParam2,
  BlackEffectParam3,
  BlackEffectParam4,
  WhiteRegionGain1,
  WhiteRegionGain2,
  WhiteRegionRange,
  WhiteEffectLevel,
  WhiteEffectParam1,
  WhiteEffectParam2,
  WhiteEffectParam3,
  WhiteEffectParam4,
  ContrastAdjust1,
  ContrastAdjust2,
  DCChangeSpeedLevel,
  ProtectRegionEffect,
  DCChangeSpeedLevel2,
  ProtectRegionWeight,
  DCEnable,
  DarkSceneTh,
  DarkSceneSlope,
  DarkDCGain,
  DarkACGain,
  BinomialTh,
  BinomialSlope,
  BinomialDCGain,
  BinomialACGain,
  BinomialTarRange,
  bIIRCurveDiffSumTh,
  bIIRCurveDiffMaxTh,
  bGlobalPQEn,
  bHistAvoidFlatBgEn,
  PQDC_INDEX_MAX
};

#define DISP_COLOR_SWREG_START              (0xFFFF0000)
#define DISP_COLOR_SWREG_COLOR_BASE         (DISP_COLOR_SWREG_START)  /* 0xFFFF0000 */
#define DISP_COLOR_SWREG_TDSHP_BASE         (DISP_COLOR_SWREG_COLOR_BASE + 0x1000)  /* 0xFFFF1000 */
#define DISP_COLOR_SWREG_TDSHP_REG_BASE     (DISP_COLOR_SWREG_COLOR_BASE + 0x1001)  /* 0xFFFF1001 */
#define DISP_COLOR_SWREG_PQDC_BASE          (DISP_COLOR_SWREG_TDSHP_BASE + 0x1000)  /* 0xFFFF2000 */
#define DISP_COLOR_SWREG_PQDS_BASE          (DISP_COLOR_SWREG_PQDC_BASE + 0x1000) /* 0xFFFF3000 */
#define DISP_COLOR_SWREG_MDP_COLOR_BASE     (DISP_COLOR_SWREG_PQDS_BASE + 0x1000) /* 0xFFFF4000 */
#define DISP_COLOR_SWREG_END                (DISP_COLOR_SWREG_MDP_COLOR_BASE + 0x1000)  /* 0xFFFF5000 */

#define SWREG_TDSHP_TUNING_MODE             (DISP_COLOR_SWREG_TDSHP_BASE + 0x0000)
#define SWREG_TDSHP_REG_GAIN_MID            (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_GAIN_MID)
#define SWREG_TDSHP_REG_GAIN_HIGH           (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_GAIN_HIGH)
#define SWREG_TDSHP_REG_COR_GAIN            (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_COR_GAIN)
#define SWREG_TDSHP_REG_COR_THR             (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_COR_THR)
#define SWREG_TDSHP_REG_COR_ZERO            (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_COR_ZERO)
#define SWREG_TDSHP_REG_GAIN                (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_GAIN)
#define SWREG_TDSHP_REG_COR_VALUE           (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_COR_VALUE)

#define SWREG_TDSHP_REG_HIGH_COR_ZERO       (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_HIGH_COR_ZERO)
#define SWREG_TDSHP_REG_HIGH_COR_THR        (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_HIGH_COR_THR)
#define SWREG_TDSHP_REG_HIGH_COR_GAIN       (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_HIGH_COR_GAIN)
#define SWREG_TDSHP_REG_HIGH_COR_VALUE      (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_HIGH_COR_VALUE)
#define SWREG_TDSHP_REG_MID_COR_ZERO        (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_MID_COR_ZERO)
#define SWREG_TDSHP_REG_MID_COR_THR         (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_MID_COR_THR)
#define SWREG_TDSHP_REG_MID_COR_GAIN        (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_MID_COR_GAIN)
#define SWREG_TDSHP_REG_MID_COR_VALUE       (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_MID_COR_VALUE)
#define SWREG_TDSHP_REG_EDF_EDGE_GAIN       (DISP_COLOR_SWREG_TDSHP_REG_BASE + EDF_EDGE_GAIN)
#define SWREG_TDSHP_REG_EDF_FLAT_TH         (DISP_COLOR_SWREG_TDSHP_REG_BASE + EDF_FLAT_TH)
#define SWREG_TDSHP_REG_EDF_DETAIL_RISE_TH  (DISP_COLOR_SWREG_TDSHP_REG_BASE + EDF_DETAIL_RISE_TH)
#define SWREG_TDSHP_REG_CLIP_RATIO          (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_CLIP_RATIO)
#define SWREG_TDSHP_REG_CLIP_THR            (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_CLIP_THR)

#define SWREG_TDSHP_REG_END                 (DISP_COLOR_SWREG_TDSHP_REG_BASE + TDSHP_REG_INDEX_MAX)

#define SWREG_BLACK_EFFECT_ENABLE      (DISP_COLOR_SWREG_PQDC_BASE + BlackEffectEnable)
#define SWREG_WHITE_EFFECT_ENABLE      (DISP_COLOR_SWREG_PQDC_BASE + WhiteEffectEnable)
#define SWREG_STRONG_BLACK_EFFECT      (DISP_COLOR_SWREG_PQDC_BASE + StrongBlackEffect)
#define SWREG_STRONG_WHITE_EFFECT      (DISP_COLOR_SWREG_PQDC_BASE + StrongWhiteEffect)
#define SWREG_ADAPTIVE_BLACK_EFFECT    (DISP_COLOR_SWREG_PQDC_BASE + AdaptiveBlackEffect)
#define SWREG_ADAPTIVE_WHITE_EFFECT    (DISP_COLOR_SWREG_PQDC_BASE + AdaptiveWhiteEffect)
#define SWREG_SCENCE_CHANGE_ONCE_EN    (DISP_COLOR_SWREG_PQDC_BASE + ScenceChangeOnceEn)
#define SWREG_SCENCE_CHANGE_CONTROL_EN (DISP_COLOR_SWREG_PQDC_BASE + ScenceChangeControlEn)
#define SWREG_SCENCE_CHANGE_CONTROL    (DISP_COLOR_SWREG_PQDC_BASE + ScenceChangeControl)
#define SWREG_SCENCE_CHANGE_TH1        (DISP_COLOR_SWREG_PQDC_BASE + ScenceChangeTh1)
#define SWREG_SCENCE_CHANGE_TH2        (DISP_COLOR_SWREG_PQDC_BASE + ScenceChangeTh2)
#define SWREG_SCENCE_CHANGE_TH3        (DISP_COLOR_SWREG_PQDC_BASE + ScenceChangeTh3)
#define SWREG_CONTENT_SMOOTH1          (DISP_COLOR_SWREG_PQDC_BASE + ContentSmooth1)
#define SWREG_CONTENT_SMOOTH2          (DISP_COLOR_SWREG_PQDC_BASE + ContentSmooth2)
#define SWREG_CONTENT_SMOOTH3          (DISP_COLOR_SWREG_PQDC_BASE + ContentSmooth3)
#define SWREG_MIDDLE_REGION_GAIN1      (DISP_COLOR_SWREG_PQDC_BASE + MiddleRegionGain1)
#define SWREG_MIDDLE_REGION_GAIN2      (DISP_COLOR_SWREG_PQDC_BASE + MiddleRegionGain2)
#define SWREG_BLACK_REGION_GAIN1       (DISP_COLOR_SWREG_PQDC_BASE + BlackRegionGain1)
#define SWREG_BLACK_REGION_GAIN2       (DISP_COLOR_SWREG_PQDC_BASE + BlackRegionGain2)
#define SWREG_BLACK_REGION_RANGE       (DISP_COLOR_SWREG_PQDC_BASE + BlackRegionRange)
#define SWREG_BLACK_EFFECT_LEVEL       (DISP_COLOR_SWREG_PQDC_BASE + BlackEffectLevel)
#define SWREG_BLACK_EFFECT_PARAM1      (DISP_COLOR_SWREG_PQDC_BASE + BlackEffectParam1)
#define SWREG_BLACK_EFFECT_PARAM2      (DISP_COLOR_SWREG_PQDC_BASE + BlackEffectParam2)
#define SWREG_BLACK_EFFECT_PARAM3      (DISP_COLOR_SWREG_PQDC_BASE + BlackEffectParam3)
#define SWREG_BLACK_EFFECT_PARAM4      (DISP_COLOR_SWREG_PQDC_BASE + BlackEffectParam4)
#define SWREG_WHITE_REGION_GAIN1       (DISP_COLOR_SWREG_PQDC_BASE + WhiteRegionGain1)
#define SWREG_WHITE_REGION_GAIN2       (DISP_COLOR_SWREG_PQDC_BASE + WhiteRegionGain2)
#define SWREG_WHITE_REGION_RANGE       (DISP_COLOR_SWREG_PQDC_BASE + WhiteRegionRange)
#define SWREG_WHITE_EFFECT_LEVEL       (DISP_COLOR_SWREG_PQDC_BASE + WhiteEffectLevel)
#define SWREG_WHITE_EFFECT_PARAM1      (DISP_COLOR_SWREG_PQDC_BASE + WhiteEffectParam1)
#define SWREG_WHITE_EFFECT_PARAM2      (DISP_COLOR_SWREG_PQDC_BASE + WhiteEffectParam2)
#define SWREG_WHITE_EFFECT_PARAM3      (DISP_COLOR_SWREG_PQDC_BASE + WhiteEffectParam3)
#define SWREG_WHITE_EFFECT_PARAM4      (DISP_COLOR_SWREG_PQDC_BASE + WhiteEffectParam4)
#define SWREG_CONTRAST_ADJUST1         (DISP_COLOR_SWREG_PQDC_BASE + ContrastAdjust1)
#define SWREG_CONTRAST_ADJUST2         (DISP_COLOR_SWREG_PQDC_BASE + ContrastAdjust2)
#define SWREG_DC_CHANGE_SPEED_LEVEL    (DISP_COLOR_SWREG_PQDC_BASE + DCChangeSpeedLevel)
#define SWREG_PROTECT_REGION_EFFECT    (DISP_COLOR_SWREG_PQDC_BASE + ProtectRegionEffect)
#define SWREG_DC_CHANGE_SPEED_LEVEL2   (DISP_COLOR_SWREG_PQDC_BASE + DCChangeSpeedLevel2)
#define SWREG_PROTECT_REGION_WEIGHT    (DISP_COLOR_SWREG_PQDC_BASE + ProtectRegionWeight)
#define SWREG_DC_ENABLE                (DISP_COLOR_SWREG_PQDC_BASE + DCEnable)
#define SWREG_DC_END                   (DISP_COLOR_SWREG_PQDC_BASE + PQDC_INDEX_MAX)


#define SWREG_PQDS_DS_EN                    (DISP_COLOR_SWREG_PQDS_BASE + PQDS_DS_en)
#define SWREG_PQDS_UP_SLOPE                 (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUpSlope)
#define SWREG_PQDS_UP_THR                   (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUpThreshold)
#define SWREG_PQDS_DOWN_SLOPE               (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iDownSlope)
#define SWREG_PQDS_DOWN_THR                 (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iDownThreshold)
#define SWREG_PQDS_ISO_EN                   (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iISO_en)
#define SWREG_PQDS_ISO_THR1                 (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iISO_thr1)
#define SWREG_PQDS_ISO_THR0                 (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iISO_thr0)
#define SWREG_PQDS_ISO_THR3                 (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iISO_thr3)
#define SWREG_PQDS_ISO_THR2                 (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iISO_thr2)
#define SWREG_PQDS_ISO_IIR                  (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iISO_IIR_alpha)
#define SWREG_PQDS_COR_ZERO_2               (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iCorZero_clip2)
#define SWREG_PQDS_COR_ZERO_1               (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iCorZero_clip1)
#define SWREG_PQDS_COR_ZERO_0               (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iCorZero_clip0)
#define SWREG_PQDS_COR_THR_2                (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iCorThr_clip2)
#define SWREG_PQDS_COR_THR_1                (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iCorThr_clip1)
#define SWREG_PQDS_COR_THR_0                (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iCorThr_clip0)
#define SWREG_PQDS_COR_GAIN_2               (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iCorGain_clip2)
#define SWREG_PQDS_COR_GAIN_1               (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iCorGain_clip1)
#define SWREG_PQDS_COR_GAIN_0               (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iCorGain_clip0)
#define SWREG_PQDS_GAIN_2                   (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iGain_clip2)
#define SWREG_PQDS_GAIN_1                   (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iGain_clip1)
#define SWREG_PQDS_GAIN_0                   (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iGain_clip0)

#define SWREG_PQDS_HIGH_COR_ZERO_2          (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iHighCorZero_clip2)
#define SWREG_PQDS_HIGH_COR_ZERO_1          (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iHighCorZero_clip1)
#define SWREG_PQDS_HIGH_COR_ZERO_0          (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iHighCorZero_clip0)
#define SWREG_PQDS_HIGH_COR_THR_2           (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iHighCorThr_clip2)
#define SWREG_PQDS_HIGH_COR_THR_1           (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iHighCorThr_clip1)
#define SWREG_PQDS_HIGH_COR_THR_0           (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iHighCorThr_clip0)
#define SWREG_PQDS_HIGH_COR_GAIN_2          (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iHighCorGain_clip2)
#define SWREG_PQDS_HIGH_COR_GAIN_1          (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iHighCorGain_clip1)
#define SWREG_PQDS_HIGH_COR_GAIN_0          (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iHighCorGain_clip0)

#define SWREG_PQDS_MID_COR_ZERO_2           (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iMidCorZero_clip2)
#define SWREG_PQDS_MID_COR_ZERO_1           (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iMidCorZero_clip1)
#define SWREG_PQDS_MID_COR_ZERO_0           (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iMidCorZero_clip0)
#define SWREG_PQDS_MID_COR_THR_2            (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iMidCorThr_clip2)
#define SWREG_PQDS_MID_COR_THR_1            (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iMidCorThr_clip1)
#define SWREG_PQDS_MID_COR_THR_0            (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iMidCorThr_clip0)
#define SWREG_PQDS_MID_COR_GAIN_2           (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iMidCorGain_clip2)
#define SWREG_PQDS_MID_COR_GAIN_1           (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iMidCorGain_clip1)
#define SWREG_PQDS_MID_COR_GAIN_0           (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iMidCorGain_clip0)

#define SWREG_PQDS_EDF_FLAT_TH_2            (DISP_COLOR_SWREG_PQDS_BASE + PQDS_i_edf_flat_th_clip2)
#define SWREG_PQDS_EDF_FLAT_TH_1            (DISP_COLOR_SWREG_PQDS_BASE + PQDS_i_edf_flat_th_clip1)
#define SWREG_PQDS_EDF_FLAT_TH_0            (DISP_COLOR_SWREG_PQDS_BASE + PQDS_i_edf_flat_th_clip0)
#define SWREG_PQDS_EDF_DETAIL_RISE_TH_2     (DISP_COLOR_SWREG_PQDS_BASE + PQDS_i_edf_detail_rise_th_clip2)
#define SWREG_PQDS_EDF_DETAIL_RISE_TH_1     (DISP_COLOR_SWREG_PQDS_BASE + PQDS_i_edf_detail_rise_th_clip1)
#define SWREG_PQDS_EDF_DETAIL_RISE_TH_0     (DISP_COLOR_SWREG_PQDS_BASE + PQDS_i_edf_detail_rise_th_clip0)

//UltraResolution registers
#define SWREG_PQDS_ULTRARES_EN              (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_en)
#define SWREG_PQDS_ULTRARES_RATIO_THR_2     (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_ratio_thr2)
#define SWREG_PQDS_ULTRARES_RATIO_THR_1     (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_ratio_thr1)
#define SWREG_PQDS_ULTRARES_RATIO_THR_0     (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_ratio_thr0)
#define SWREG_PQDS_ULTRARES_GAIN_MID_1      (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_gain_mid_clip1)
#define SWREG_PQDS_ULTRARES_GAIN_MID_0      (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_gain_mid_clip0)
#define SWREG_PQDS_ULTRARES_GAIN_HIGH_1     (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_gain_high_clip1)
#define SWREG_PQDS_ULTRARES_GAIN_HIGH_0     (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_gain_high_clip0)
#define SWREG_PQDS_ULTRARES_EDF_EDGE_GAIN_1 (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_edf_edge_gain_clip1)
#define SWREG_PQDS_ULTRARES_EDF_EDGE_GAIN_0 (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_edf_edge_gain_clip0)
#define SWREG_PQDS_ULTRARES_CLIP_THR_1      (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_clip_thr_clip1)
#define SWREG_PQDS_ULTRARES_CLIP_THR_0      (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_clip_thr_clip0)
#define SWREG_PQDS_ULTRARES_CLIP_RATIO_1    (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_clip_ratio_clip1)
#define SWREG_PQDS_ULTRARES_CLIP_RATIO_0    (DISP_COLOR_SWREG_PQDS_BASE + PQDS_iUltraRes_clip_ratio_clip0)

#define SWREG_PQDS_END                      (DISP_COLOR_SWREG_PQDS_BASE + PQDS_INDEX_MAX)

typedef enum {
    PQDS_DS_en = 0,
    PQDS_iUpSlope,
    PQDS_iUpThreshold,
    PQDS_iDownSlope,
    PQDS_iDownThreshold,
    PQDS_iISO_en,
    PQDS_iISO_thr1,
    PQDS_iISO_thr0,
    PQDS_iISO_thr3,
    PQDS_iISO_thr2,
    PQDS_iISO_IIR_alpha,
    PQDS_iCorZero_clip2,
    PQDS_iCorZero_clip1,
    PQDS_iCorZero_clip0,
    PQDS_iCorThr_clip2,
    PQDS_iCorThr_clip1,
    PQDS_iCorThr_clip0,
    PQDS_iCorGain_clip2,
    PQDS_iCorGain_clip1,
    PQDS_iCorGain_clip0,
    PQDS_iGain_clip2,
    PQDS_iGain_clip1,
    PQDS_iGain_clip0,

    PQDS_iHighCorZero_clip2,
    PQDS_iHighCorZero_clip1,
    PQDS_iHighCorZero_clip0,
    PQDS_iHighCorThr_clip2,
    PQDS_iHighCorThr_clip1,
    PQDS_iHighCorThr_clip0,
    PQDS_iHighCorGain_clip2,
    PQDS_iHighCorGain_clip1,
    PQDS_iHighCorGain_clip0,

    PQDS_iMidCorZero_clip2,
    PQDS_iMidCorZero_clip1,
    PQDS_iMidCorZero_clip0,
    PQDS_iMidCorThr_clip2,
    PQDS_iMidCorThr_clip1,
    PQDS_iMidCorThr_clip0,
    PQDS_iMidCorGain_clip2,
    PQDS_iMidCorGain_clip1,
    PQDS_iMidCorGain_clip0,

    PQDS_i_edf_flat_th_clip2,
    PQDS_i_edf_flat_th_clip1,
    PQDS_i_edf_flat_th_clip0,
    PQDS_i_edf_detail_rise_th_clip2,
    PQDS_i_edf_detail_rise_th_clip1,
    PQDS_i_edf_detail_rise_th_clip0,

    //UltraResolution registers
    PQDS_iUltraRes_en,
    PQDS_iUltraRes_ratio_thr2,
    PQDS_iUltraRes_ratio_thr1,
    PQDS_iUltraRes_ratio_thr0,
    PQDS_iUltraRes_gain_mid_clip1,
    PQDS_iUltraRes_gain_mid_clip0,
    PQDS_iUltraRes_gain_high_clip1,
    PQDS_iUltraRes_gain_high_clip0,
    PQDS_iUltraRes_edf_edge_gain_clip1,
    PQDS_iUltraRes_edf_edge_gain_clip0,
    PQDS_iUltraRes_clip_thr_clip1,
    PQDS_iUltraRes_clip_thr_clip0,
    PQDS_iUltraRes_clip_ratio_clip1,
    PQDS_iUltraRes_clip_ratio_clip0,
    PQDS_iUltraRes_edf_detail_gain_clip1,
    PQDS_iUltraRes_edf_detail_gain_clip0,
    PQDS_iUltraRes_edf_flat_gain_clip1,
    PQDS_iUltraRes_edf_flat_gain_clip0,

    PQDS_INDEX_MAX
} PQ_DS_EX_index_t;


enum PQ_AT_ACTIONID
{
    HW_READ = 0,
    HW_WRITE = 1,
    PQ_SW_READ = 2,
    PQ_SW_WRITE = 3,
    AAL_SW_READ = 4,
    AAL_SW_WRITE = 5,
    IDLEMGR_CONTROL = 6,
    PANEL_SEL = 7
};

enum IoctlRequest {
    IOCTL_WRITE_REG,
    IOCTL_READ_REG,
    IOCTL_MUTEX_CONTROL,
    IOCTL_WRITE_SW_REG,
    IOCTL_READ_SW_REG,
};


//PQ_COLOR_READ_REG
struct DISP_READ_REG {
    unsigned int reg;
    unsigned int val;
    unsigned int mask;
};

struct DISP_WRITE_REG {
    unsigned int reg;
    unsigned int val;
    unsigned int mask;
};


enum
{
    MOD_DISPLAY =               0x0000,
    MOD_DYNAMIC_SHARPNESS =     0x0002,
    MOD_TDSHP_REG =             0x0003,
    MOD_ULTRARESOLUTION =       0x0004,
    MOD_DYNAMIC_CONTRAST =      0x0007,
    MOD_BLUE_LIGHT_ALGO =       0x0010,
    MOD_BLUE_LIGHT_INPUT =      0x0011,
    MOD_BLUE_LIGHT_OUTPUT =     0x0012,
    MOD_CHAMELEON_ALGO =        0x0020,
    MOD_CHAMELEON_INPUT =       0x0021,
    MOD_CHAMELEON_OUTPUT =      0x0022,
    MOD_TRANSITION_ALGO =       0x0030,
    MOD_TRANSITION_INPUT =      0x0031,
    MOD_TRANSITION_OUTPUT =     0x0032,
    MOD_DS_SWREG =              0x0110,
    MOD_DS_INPUT =              0x0111,
    MOD_DS_OUTPUT =             0x0112,
    MOD_DS_REGION_OUTPUT =       0x0113,
    MOD_DC_SWREG =              0x0120,
    MOD_DC_INPUT =              0x0121,
    MOD_DC_OUTPUT =             0x0122,
    MOD_RSZ_SWREG =             0x0130,
    MOD_RSZ_INPUT =             0x0131,
    MOD_RSZ_OUTPUT =            0x0132,
    MOD_COLOR_SWREG =           0x0140,
    MOD_COLOR_INPUT =           0x0141,
    MOD_COLOR_OUTPUT =          0x0142,
    MOD_HDR_SWREG =             0x0150,
    MOD_HDR_INPUT =             0x0151,
    MOD_HDR_OUTPUT =            0x0152,
    MOD_HDR10PLUS_SWREG =       0x0153,
    MOD_HDR10PLUS_INPUT =       0x0154,
    MOD_HDR10PLUS_OUTPUT =      0x0155,
    MOD_HDRVIVID_SWREG =        0x0156,
    MOD_HDRVIVID_INPUT =        0x0157,
    MOD_HDRVIVID_OUTPUT =       0x0158,
    MOD_CCORR_SWREG =           0x0160,
    MOD_CCORR_INPUT =           0x0161,
    MOD_CCORR_OUTPUT =          0x0162,
    MOD_DRE_SWREG =             0x0170,
    MOD_DRE_INPUT =             0x0171,
    MOD_DRE_OUTPUT =            0x0172,
    MOD_HFG_INPUT =             0x0173,
    MOD_HFG_OUTPUT =            0x0174,
    MDP_DRE_READONLYREG =       0x0175,
    MOD_DISP_DRE_SWREG =        0x0180,
    MOD_DISP_DRE_INPUT =        0x0181,
    MOD_DISP_DRE_OUTPUT =       0x0182,
    MOD_DISP_DRE_SWREG_RO =     0x0183,
    MOD_SDR2HDR_INPUT =         0x0190,
    MOD_SDR2HDR_OUTPUT =        0x0191,
    MOD_SDR2HDR_SWREG =         0x0192,
    MOD_RGB_GAIN =              0x0200,
    MOD_TRS_SWREG =             0x0210,
    MOD_DISP_AIBLD_SWREG =      0x0211,
    MOD_DISP_ODDMR_SWREG =      0x0212,
    MOD_DISP_CLARITY_SWREG =    0x0213,
    MOD_DISP_AAL_SWREG =        0x0214,
    MOD_DISP_ESS20_SWREG =      0x0215,
    MOD_DISP_DRECOLOR_SWREG =   0x0216,
    MOD_REGION_INPUT =          0x0220,
    MOD_REGION_OUTPUT =         0x0221,
    MOD_REGION_SWREG =          0x0222,
    MOD_CLARITY_SWREG =         0x0230,
    MOD_SEG_SWREG =             0x0240,
    MOD_REGION_COLOR_SWREG =    0x0250,
    MOD_C3D_SWREG =             0x0260,
};

typedef enum {
    TDSHP_GAIN_MID = 0,
    TDSHP_GAIN_HIGH,
    TDSHP_COR_GAIN,
    TDSHP_COR_THR,
    TDSHP_COR_ZERO,
    TDSHP_GAIN,
    TDSHP_COR_VALUE,

    TDSHP_HIGH_COR_ZERO,
    TDSHP_HIGH_COR_THR,
    TDSHP_HIGH_COR_GAIN,
    TDSHP_HIGH_COR_VALUE,
    TDSHP_MID_COR_ZERO,
    TDSHP_MID_COR_THR,
    TDSHP_MID_COR_GAIN,
    TDSHP_MID_COR_VALUE,
    EDF_EDGE_GAIN,
    EDF_FLAT_TH,
    EDF_DETAIL_RISE_TH,

    TDSHP_CLIP_RATIO,
    TDSHP_CLIP_THR,

    TDSHP_REG_BASE_SEL,
    TDSHP_REG_INDEX_MAX
} MDP_TDSHP_REG_EX_index_t;


typedef struct DispPQIoctlParams {
    enum IoctlRequest request;
    int reg;
    int val;
    int mask;
} DispPQIoctlParams;

enum Result {
    OK,
    NOT_INITIALIZED,
    INVALID_ARGUMENTS,
    INVALID_STATE,
    NOT_SUPPORTED,
};

typedef struct ExecIoctl_return {
    int retval;
    DispPQIoctlParams retParam;
} execIoctl_return;

typedef struct GetTuning_return {
    int retval;
    int value;
} getTuning_return;

typedef struct GetTDSHP_return {
    int retval;
    int TDSHPFlag;
} getTdshp_return;


int pq_cmd_handler(char* cmdline, ATOP_t at_op, char* response);
#endif
