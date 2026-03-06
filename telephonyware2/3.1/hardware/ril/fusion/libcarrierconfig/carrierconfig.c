/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2018. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#undef LOG_TAG
#define LOG_TAG "CarrierCfg"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <RfxStatusDefs.h>

#include "carrierconfig.h"

const static CarrierConfigValue carierConfigData[] = {
   { MCC_MNC_SEPARATOR, "00101" },  /* TEST SIM */
   #include <config_data/carrier_config_001_01.h>
   {MCC_MNC_SEPARATOR, "20201" },
   #include <config_data/carrier_config_202_01.h>
   {MCC_MNC_SEPARATOR, "20202" },
   #include <config_data/carrier_config_202_02.h>
   {MCC_MNC_SEPARATOR, "20205" },
   #include <config_data/carrier_config_202_05.h>
   {MCC_MNC_SEPARATOR, "20209" },
   #include <config_data/carrier_config_202_09.h>
   {MCC_MNC_SEPARATOR, "20210" },
   #include <config_data/carrier_config_202_10.h>
   {MCC_MNC_SEPARATOR, "20402" },
   #include <config_data/carrier_config_204_02.h>
   {MCC_MNC_SEPARATOR, "20403" }, /*Voiceworks Netherlands*/
   #include <config_data/carrier_config_204_03.h>
   {MCC_MNC_SEPARATOR, "20404" },
   #include <config_data/carrier_config_204_04.h>
   {MCC_MNC_SEPARATOR, "204069" },
   #include <config_data/carrier_config_204_069.h>
   {MCC_MNC_SEPARATOR, "20407" },
   #include <config_data/carrier_config_204_07.h>
   {MCC_MNC_SEPARATOR, "20408" },
   #include <config_data/carrier_config_204_08.h>
   {MCC_MNC_SEPARATOR, "20416" },
   #include <config_data/carrier_config_204_16.h>
   {MCC_MNC_SEPARATOR, "20417" }, /*Lebara Mobile Netherlands*/
   #include <config_data/carrier_config_204_17.h>
   {MCC_MNC_SEPARATOR, "20420" },
   #include <config_data/carrier_config_204_20.h>
   {MCC_MNC_SEPARATOR, "20469" },
   #include <config_data/carrier_config_204_69.h>
   {MCC_MNC_SEPARATOR, "20601" },
   #include <config_data/carrier_config_206_01.h>
   {MCC_MNC_SEPARATOR, "20605" },
   #include <config_data/carrier_config_206_05.h>
   {MCC_MNC_SEPARATOR, "20608" },
   #include <config_data/carrier_config_206_08.h>
   {MCC_MNC_SEPARATOR, "20610" },
   #include <config_data/carrier_config_206_10.h>
   {MCC_MNC_SEPARATOR, "20612" }, /*Digi Belgium*/
   #include <config_data/carrier_config_206_12.h>
   {MCC_MNC_SEPARATOR, "20620" },
   #include <config_data/carrier_config_206_20.h>
   {MCC_MNC_SEPARATOR, "20801" }, /*Orange France*/
   #include <config_data/carrier_config_208_01.h>
   {MCC_MNC_SEPARATOR, "20802" },
   #include <config_data/carrier_config_208_02.h>
   {MCC_MNC_SEPARATOR, "20809" },
   #include <config_data/carrier_config_208_09.h>
   {MCC_MNC_SEPARATOR, "20810" },
   #include <config_data/carrier_config_208_10.h>
   {MCC_MNC_SEPARATOR, "20811" },
   #include <config_data/carrier_config_208_11.h>
   {MCC_MNC_SEPARATOR, "20813" },
   #include <config_data/carrier_config_208_13.h>
   {MCC_MNC_SEPARATOR, "20815" },/*Free France*/
   #include <config_data/carrier_config_208_15.h>
   {MCC_MNC_SEPARATOR, "20816" },/*Free France*/
   #include <config_data/carrier_config_208_16.h>
   {MCC_MNC_SEPARATOR, "20820" },
   #include <config_data/carrier_config_208_20.h>
   {MCC_MNC_SEPARATOR, "20821" },
   #include <config_data/carrier_config_208_21.h>
   {MCC_MNC_SEPARATOR, "20826" },
   #include <config_data/carrier_config_208_26.h>
   {MCC_MNC_SEPARATOR, "20827" },/*Coriolis France*/
   #include <config_data/carrier_config_208_27.h>
   {MCC_MNC_SEPARATOR, "20832" }, /*Orange France lab*/
   #include <config_data/carrier_config_208_32.h>
   {MCC_MNC_SEPARATOR, "20829" },
   #include <config_data/carrier_config_208_29.h>
   {MCC_MNC_SEPARATOR, "20835" },/*Free France*/
   #include <config_data/carrier_config_208_35.h>
   {MCC_MNC_SEPARATOR, "20838" },/*Lebara France*/
   #include <config_data/carrier_config_208_38.h>
   {MCC_MNC_SEPARATOR, "20844" }, /*Orange France lab*/
   #include <config_data/carrier_config_208_44.h>
   {MCC_MNC_SEPARATOR, "208500" }, /*EDF France*/
   #include <config_data/carrier_config_208_500.h>
   {MCC_MNC_SEPARATOR, "20888" },
   #include <config_data/carrier_config_208_88.h>
   {MCC_MNC_SEPARATOR, "20889" }, /*Hub One France*/
   #include <config_data/carrier_config_208_89.h>
   {MCC_MNC_SEPARATOR, "20891" },
   #include <config_data/carrier_config_208_91.h>
   {MCC_MNC_SEPARATOR, "21210" },    /*Monaco Telecom*/
   #include <config_data/carrier_config_212_10.h>
   {MCC_MNC_SEPARATOR, "21303" }, /*andorra telecom Andorra*/
   #include <config_data/carrier_config_213_03.h>
   {MCC_MNC_SEPARATOR, "21401" },
   #include <config_data/carrier_config_214_01.h>
   {MCC_MNC_SEPARATOR, "21403" },
   #include <config_data/carrier_config_214_03.h>
   {MCC_MNC_SEPARATOR, "21404" },     /*Yoigo Spain*/
   #include <config_data/carrier_config_214_04.h>
   {MCC_MNC_SEPARATOR, "21405" },  /*Telefonica Spain*/
   #include <config_data/carrier_config_214_05.h>
   {MCC_MNC_SEPARATOR, "21406" },
   #include <config_data/carrier_config_214_06.h>
   {MCC_MNC_SEPARATOR, "21407" },  /*Telefonica Spain*/
   #include <config_data/carrier_config_214_07.h>
   {MCC_MNC_SEPARATOR, "21409" },
   #include <config_data/carrier_config_214_09.h>
   {MCC_MNC_SEPARATOR, "21411" },
   #include <config_data/carrier_config_214_11.h>
   {MCC_MNC_SEPARATOR, "21422" },     /*Digi Spain*/
   #include <config_data/carrier_config_214_22.h>
   {MCC_MNC_SEPARATOR, "21438" },  /*Telefonica Spain*/
   #include <config_data/carrier_config_214_38.h>
   {MCC_MNC_SEPARATOR, "21601" },
   #include <config_data/carrier_config_216_01.h>
   {MCC_MNC_SEPARATOR, "21603" },
   #include <config_data/carrier_config_216_03.h>
   {MCC_MNC_SEPARATOR, "21630" },
   #include <config_data/carrier_config_216_30.h>
   {MCC_MNC_SEPARATOR, "21670" },   /* Vodafone Hungary */
   #include <config_data/carrier_config_216_70.h>
   {MCC_MNC_SEPARATOR, "21805" },   /* mtel/Bosnia and Herzegovina */
   #include <config_data/carrier_config_218_05.h>
   {MCC_MNC_SEPARATOR, "21890" },   /* BH Mobile/Bosnia and Herzegovina */
   #include <config_data/carrier_config_218_90.h>
   { MCC_MNC_SEPARATOR, "21901" },  /* TMO Croatia */
   #include <config_data/carrier_config_219_01.h>
   { MCC_MNC_SEPARATOR, "21902" },  /* Telemach Croatia */
   #include <config_data/carrier_config_219_02.h>
   {MCC_MNC_SEPARATOR, "21910" },  /* A1 Croatia */
   #include <config_data/carrier_config_219_10.h>
   {MCC_MNC_SEPARATOR, "21920" },  /* TMO Croatia */
   #include <config_data/carrier_config_219_20.h>
   {MCC_MNC_SEPARATOR, "22001" },   /* Telenor Serbia */
   #include <config_data/carrier_config_220_01.h>
   {MCC_MNC_SEPARATOR, "22003" },   /* MTS Serbia */
   #include <config_data/carrier_config_220_03.h>
   {MCC_MNC_SEPARATOR, "22005" },   /* VIP Serbia */
   #include <config_data/carrier_config_220_05.h>
   {MCC_MNC_SEPARATOR, "22201" },   /* TIM Italy */
   #include <config_data/carrier_config_222_01.h>
   {MCC_MNC_SEPARATOR, "22206" },
   #include <config_data/carrier_config_222_06.h>
   {MCC_MNC_SEPARATOR, "22207" },  /* Kena Mobile Italy */
   #include <config_data/carrier_config_222_07.h>
   {MCC_MNC_SEPARATOR, "22208" },    /*Fastweb Italy*/
   #include <config_data/carrier_config_222_08.h>
   {MCC_MNC_SEPARATOR, "22210" },
   #include <config_data/carrier_config_222_10.h>
   {MCC_MNC_SEPARATOR, "22233" },  /* PosteMobile Italy */
   #include <config_data/carrier_config_222_33.h>
   {MCC_MNC_SEPARATOR, "22235" },  /* Lycamobile Italy */
   #include <config_data/carrier_config_222_35.h>
   {MCC_MNC_SEPARATOR, "22236" },  /* Digi Italy */
   #include <config_data/carrier_config_222_36.h>
   {MCC_MNC_SEPARATOR, "22249" },  /* Vianova Italy */
   #include <config_data/carrier_config_222_49.h>
   {MCC_MNC_SEPARATOR, "22250" },  /* Iliad Italy */
   #include <config_data/carrier_config_222_50.h>
   {MCC_MNC_SEPARATOR, "22253" },  /* Coopvoce Italy */
   #include <config_data/carrier_config_222_53.h>
   {MCC_MNC_SEPARATOR, "22254" },  /* Plintron Italy */
   #include <config_data/carrier_config_222_54.h>
   {MCC_MNC_SEPARATOR, "22256" },  /* Spusu Italy */
   #include <config_data/carrier_config_222_56.h>
   {MCC_MNC_SEPARATOR, "22288" },  /* WindTre Italy */
   #include <config_data/carrier_config_222_88.h>
   {MCC_MNC_SEPARATOR, "22299" },  /* WindTre Italy */
   #include <config_data/carrier_config_222_99.h>
   {MCC_MNC_SEPARATOR, "22601" },  /* Vodafone Romamia */
   #include <config_data/carrier_config_226_01.h>
   {MCC_MNC_SEPARATOR, "22603" },  /* Telekom Romamia */
   #include <config_data/carrier_config_226_03.h>
   {MCC_MNC_SEPARATOR, "22605" },
   #include <config_data/carrier_config_226_05.h>
   {MCC_MNC_SEPARATOR, "22606" },  /* Telekom Romamia */
   #include <config_data/carrier_config_226_06.h>
   {MCC_MNC_SEPARATOR, "22610" },
   #include <config_data/carrier_config_226_10.h>
   {MCC_MNC_SEPARATOR, "22801" },
   #include <config_data/carrier_config_228_01.h>
   {MCC_MNC_SEPARATOR, "22802" },
   #include <config_data/carrier_config_228_02.h>
   {MCC_MNC_SEPARATOR, "22803" }, /* Salt Switzerland */
   #include <config_data/carrier_config_228_03.h>
   {MCC_MNC_SEPARATOR, "22853" }, /* UPC Switzerland */
   #include <config_data/carrier_config_228_53.h>
   {MCC_MNC_SEPARATOR, "22871" }, /* Spusu Switzerland */
   #include <config_data/carrier_config_228_71.h>
   {MCC_MNC_SEPARATOR, "23001" },
   #include <config_data/carrier_config_230_01.h>
   {MCC_MNC_SEPARATOR, "23002" },
   #include <config_data/carrier_config_230_02.h>
   {MCC_MNC_SEPARATOR, "23003" },
   #include <config_data/carrier_config_230_03.h>
   {MCC_MNC_SEPARATOR, "23099" },
   #include <config_data/carrier_config_230_99.h>
   { MCC_MNC_SEPARATOR, "23101" },
   #include <config_data/carrier_config_231_01.h>
   {MCC_MNC_SEPARATOR, "23102" },
   #include <config_data/carrier_config_231_02.h>
   {MCC_MNC_SEPARATOR, "23103" }, /* 4ka Slovakia */
   #include <config_data/carrier_config_231_03.h>
   {MCC_MNC_SEPARATOR, "23104" },
   #include <config_data/carrier_config_231_04.h>
   {MCC_MNC_SEPARATOR, "23106" },
   #include <config_data/carrier_config_231_06.h>
   {MCC_MNC_SEPARATOR, "23112" },
   #include <config_data/carrier_config_231_12.h>
   {MCC_MNC_SEPARATOR, "23201" },  /* A1 Austria */
   #include <config_data/carrier_config_232_01.h>
   {MCC_MNC_SEPARATOR, "23202" },  /* A1 Austria */
   #include <config_data/carrier_config_232_02.h>
   {MCC_MNC_SEPARATOR, "23203" },  /* TMOEU Austria */
   #include <config_data/carrier_config_232_03.h>
   {MCC_MNC_SEPARATOR, "23205" },
   #include <config_data/carrier_config_232_05.h>
   {MCC_MNC_SEPARATOR, "23207" },  /* TMOEU Austria */
   #include <config_data/carrier_config_232_07.h>
   {MCC_MNC_SEPARATOR, "23208" },
   #include <config_data/carrier_config_232_08.h>
   {MCC_MNC_SEPARATOR, "23209" },  /* A1 Austria */
   #include <config_data/carrier_config_232_09.h>
   {MCC_MNC_SEPARATOR, "23210" },
   #include <config_data/carrier_config_232_10.h>
   {MCC_MNC_SEPARATOR, "23211" },  /* Bob Austria */
   #include <config_data/carrier_config_232_11.h>
   {MCC_MNC_SEPARATOR, "23212" },  /* Yesss Austria */
   #include <config_data/carrier_config_232_12.h>
   {MCC_MNC_SEPARATOR, "23214" },
   #include <config_data/carrier_config_232_14.h>
   {MCC_MNC_SEPARATOR, "23217" },  /* Spusu Austria */
   #include <config_data/carrier_config_232_17.h>
   {MCC_MNC_SEPARATOR, "23410" },
   #include <config_data/carrier_config_234_10.h>
   {MCC_MNC_SEPARATOR, "23415" },
   #include <config_data/carrier_config_234_15.h>
   {MCC_MNC_SEPARATOR, "23420" },
   #include <config_data/carrier_config_234_20.h>
   {MCC_MNC_SEPARATOR, "23430" },
   #include <config_data/carrier_config_234_30.h>
   {MCC_MNC_SEPARATOR, "23433" },
   #include <config_data/carrier_config_234_33.h>
   {MCC_MNC_SEPARATOR, "23408" },
   #include <config_data/carrier_config_234_08.h>
   {MCC_MNC_SEPARATOR, "23427" },
   #include <config_data/carrier_config_234_27.h>
   {MCC_MNC_SEPARATOR, "23431" },
   #include <config_data/carrier_config_234_31.h>
   {MCC_MNC_SEPARATOR, "23432" },
   #include <config_data/carrier_config_234_32.h>
   {MCC_MNC_SEPARATOR, "23434" },
   #include <config_data/carrier_config_234_34.h>
   {MCC_MNC_SEPARATOR, "23438" },   /*Virgin UK*/
   #include <config_data/carrier_config_234_38.h>
   {MCC_MNC_SEPARATOR, "23440" },   /*Spusu UK*/
   #include <config_data/carrier_config_234_40.h>
   {MCC_MNC_SEPARATOR, "23455" },   /*Sure UK*/
   #include <config_data/carrier_config_234_55.h>
   {MCC_MNC_SEPARATOR, "23457" }, /*Sky UK*/
   #include <config_data/carrier_config_234_57.h>
   {MCC_MNC_SEPARATOR, "23458" },   /*Manx Telecom UK*/
   #include <config_data/carrier_config_234_58.h>
   {MCC_MNC_SEPARATOR, "23470" },
   #include <config_data/carrier_config_234_70.h>
   {MCC_MNC_SEPARATOR, "23486" },
   #include <config_data/carrier_config_234_86.h>
   {MCC_MNC_SEPARATOR, "23487" },   /*Lebara Mobile UK*/
   #include <config_data/carrier_config_234_87.h>
   {MCC_MNC_SEPARATOR, "23491" },
   #include <config_data/carrier_config_234_91.h>
   {MCC_MNC_SEPARATOR, "23502" },
   #include <config_data/carrier_config_235_02.h>
   {MCC_MNC_SEPARATOR, "23591" },
   #include <config_data/carrier_config_235_91.h>
   {MCC_MNC_SEPARATOR, "23594" },
   #include <config_data/carrier_config_235_94.h>
   {MCC_MNC_SEPARATOR, "23801" }, /* TDC Denmark */
   #include <config_data/carrier_config_238_01.h>
   {MCC_MNC_SEPARATOR, "23802" },
   #include <config_data/carrier_config_238_02.h>
   {MCC_MNC_SEPARATOR, "23806" },
   #include <config_data/carrier_config_238_06.h>
   {MCC_MNC_SEPARATOR, "23810" }, /* TDC Denmark */
   #include <config_data/carrier_config_238_10.h>
   {MCC_MNC_SEPARATOR, "23820" },
   #include <config_data/carrier_config_238_20.h>
   {MCC_MNC_SEPARATOR, "23830" },
   #include <config_data/carrier_config_238_30.h>
   {MCC_MNC_SEPARATOR, "23877" },
   #include <config_data/carrier_config_238_77.h>
   {MCC_MNC_SEPARATOR, "24001" },
   #include <config_data/carrier_config_240_01.h>
   {MCC_MNC_SEPARATOR, "24002" },
   #include <config_data/carrier_config_240_02.h>
   {MCC_MNC_SEPARATOR, "24005" },
   #include <config_data/carrier_config_240_05.h>
   {MCC_MNC_SEPARATOR, "24006" },/* Vimla Sweden */
   #include <config_data/carrier_config_240_06.h>
   {MCC_MNC_SEPARATOR, "24007" },
   #include <config_data/carrier_config_240_07.h>
   {MCC_MNC_SEPARATOR, "24008" },
   #include <config_data/carrier_config_240_08.h>
   {MCC_MNC_SEPARATOR, "24099" },
   #include <config_data/carrier_config_240_99.h>
   {MCC_MNC_SEPARATOR, "240680" },
   #include <config_data/carrier_config_240_680.h>
   {MCC_MNC_SEPARATOR, "240681" },
   #include <config_data/carrier_config_240_681.h>
   {MCC_MNC_SEPARATOR, "24201" },
   #include <config_data/carrier_config_242_01.h>
   {MCC_MNC_SEPARATOR, "24202" },
   #include <config_data/carrier_config_242_02.h>
   {MCC_MNC_SEPARATOR, "24205" },
   #include <config_data/carrier_config_242_05.h>
   {MCC_MNC_SEPARATOR, "24206" }, /* Ice Norway */
   #include <config_data/carrier_config_242_06.h>
   {MCC_MNC_SEPARATOR, "24214" }, /* Ice Norway */
   #include <config_data/carrier_config_242_14.h>
   {MCC_MNC_SEPARATOR, "24215" }, /* Telavox Norway */
   #include <config_data/carrier_config_242_15.h>
   {MCC_MNC_SEPARATOR, "24405" },
   #include <config_data/carrier_config_244_05.h>
   {MCC_MNC_SEPARATOR, "24407" },
   #include <config_data/carrier_config_244_07.h>
   {MCC_MNC_SEPARATOR, "24408" },
   #include <config_data/carrier_config_244_08.h>
   {MCC_MNC_SEPARATOR, "24409" },
   #include <config_data/carrier_config_244_09.h>
   {MCC_MNC_SEPARATOR, "24412" },
   #include <config_data/carrier_config_244_12.h>
   {MCC_MNC_SEPARATOR, "24413" },
   #include <config_data/carrier_config_244_13.h>
   {MCC_MNC_SEPARATOR, "24421" },
   #include <config_data/carrier_config_244_21.h>
   {MCC_MNC_SEPARATOR, "24427" },
   #include <config_data/carrier_config_244_27.h>
   {MCC_MNC_SEPARATOR, "24436" },
   #include <config_data/carrier_config_244_36.h>
   {MCC_MNC_SEPARATOR, "24438" },
   #include <config_data/carrier_config_244_38.h>
   {MCC_MNC_SEPARATOR, "24439" },
   #include <config_data/carrier_config_244_39.h>
   {MCC_MNC_SEPARATOR, "24440" },
   #include <config_data/carrier_config_244_40.h>
   {MCC_MNC_SEPARATOR, "24441" },
   #include <config_data/carrier_config_244_41.h>
   {MCC_MNC_SEPARATOR, "24491" },
   #include <config_data/carrier_config_244_91.h>
   {MCC_MNC_SEPARATOR, "24601" },/* Telia Lithuania */
   #include <config_data/carrier_config_246_01.h>
   {MCC_MNC_SEPARATOR, "24602" },/* Bite Lithuania */
   #include <config_data/carrier_config_246_02.h>
   {MCC_MNC_SEPARATOR, "24603" },/* TELE2 Lithuania */
   #include <config_data/carrier_config_246_03.h>
   {MCC_MNC_SEPARATOR, "24681" },
   #include <config_data/carrier_config_246_81.h>
   {MCC_MNC_SEPARATOR, "24701" },/* LMT Latvia */
   #include <config_data/carrier_config_247_01.h>
   {MCC_MNC_SEPARATOR, "24702" },/* TELE2 Latvia */
   #include <config_data/carrier_config_247_02.h>
   {MCC_MNC_SEPARATOR, "24705" },/* Bite Latvia */
   #include <config_data/carrier_config_247_05.h>
   {MCC_MNC_SEPARATOR, "24710" },/* LMT Latvia */
   #include <config_data/carrier_config_247_10.h>
   {MCC_MNC_SEPARATOR, "24801" },/* Telia Estonia */
   #include <config_data/carrier_config_248_01.h>
   {MCC_MNC_SEPARATOR, "24802" },/* Elisa Estonia */
   #include <config_data/carrier_config_248_02.h>
   {MCC_MNC_SEPARATOR, "24803" },/* TELE2 Estonia */
   #include <config_data/carrier_config_248_03.h>
   {MCC_MNC_SEPARATOR, "25001" },
   #include <config_data/carrier_config_250_01.h>
   {MCC_MNC_SEPARATOR, "25002" },
   #include <config_data/carrier_config_250_02.h>
   {MCC_MNC_SEPARATOR, "25004" },
   #include <config_data/carrier_config_250_04.h>
   {MCC_MNC_SEPARATOR, "25005" },
   #include <config_data/carrier_config_250_05.h>
   {MCC_MNC_SEPARATOR, "25010" },
   #include <config_data/carrier_config_250_10.h>
   {MCC_MNC_SEPARATOR, "25011" },
   #include <config_data/carrier_config_250_11.h>
   {MCC_MNC_SEPARATOR, "25013" },
   #include <config_data/carrier_config_250_13.h>
   {MCC_MNC_SEPARATOR, "25020" },
   #include <config_data/carrier_config_250_20.h>
   {MCC_MNC_SEPARATOR, "25030" },
   #include <config_data/carrier_config_250_30.h>
   {MCC_MNC_SEPARATOR, "25035" },
   #include <config_data/carrier_config_250_35.h>
   {MCC_MNC_SEPARATOR, "25039" },
   #include <config_data/carrier_config_250_39.h>
   {MCC_MNC_SEPARATOR, "25050" },
   #include <config_data/carrier_config_250_50.h>
   {MCC_MNC_SEPARATOR, "25092" },
   #include <config_data/carrier_config_250_92.h>
   {MCC_MNC_SEPARATOR, "25093" },
   #include <config_data/carrier_config_250_93.h>
   {MCC_MNC_SEPARATOR, "25099" },
   #include <config_data/carrier_config_250_99.h>
   {MCC_MNC_SEPARATOR, "25501" },/* Vodafone Ukraine */
   #include <config_data/carrier_config_255_01.h>
   {MCC_MNC_SEPARATOR, "25503" },
   #include <config_data/carrier_config_255_03.h>
   {MCC_MNC_SEPARATOR, "25506" },/* Lifecell Ukraine */
   #include <config_data/carrier_config_255_06.h>
   {MCC_MNC_SEPARATOR, "25701" },
   #include <config_data/carrier_config_257_01.h>
   {MCC_MNC_SEPARATOR, "25702" },
   #include <config_data/carrier_config_257_02.h>
   {MCC_MNC_SEPARATOR, "25901" },/* Orange Moldova */
   #include <config_data/carrier_config_259_01.h>
   {MCC_MNC_SEPARATOR, "25902" },/* Moldcell Moldova */
   #include <config_data/carrier_config_259_02.h>
   {MCC_MNC_SEPARATOR, "25915" },
   #include <config_data/carrier_config_259_15.h>
   {MCC_MNC_SEPARATOR, "26001" },
   #include <config_data/carrier_config_260_01.h>
   {MCC_MNC_SEPARATOR, "26002" },
   #include <config_data/carrier_config_260_02.h>
   {MCC_MNC_SEPARATOR, "26003" },
   #include <config_data/carrier_config_260_03.h>
   {MCC_MNC_SEPARATOR, "26006" },
   #include <config_data/carrier_config_260_06.h>
   {MCC_MNC_SEPARATOR, "26098" },
   #include <config_data/carrier_config_260_98.h>
   {MCC_MNC_SEPARATOR, "26034" },
   #include <config_data/carrier_config_260_34.h>
   {MCC_MNC_SEPARATOR, "26201" }, /*TMO EU Germany*/
   #include <config_data/carrier_config_262_01.h>
   {MCC_MNC_SEPARATOR, "26202" },
   #include <config_data/carrier_config_262_02.h>
   {MCC_MNC_SEPARATOR, "26203" },
   #include <config_data/carrier_config_262_03.h>
   {MCC_MNC_SEPARATOR, "26204" },
   #include <config_data/carrier_config_262_04.h>
   {MCC_MNC_SEPARATOR, "26206" }, /*TMO EU Germany*/
   #include <config_data/carrier_config_262_06.h>
   {MCC_MNC_SEPARATOR, "26207" },
   #include <config_data/carrier_config_262_07.h>
   {MCC_MNC_SEPARATOR, "26208" },
   #include <config_data/carrier_config_262_08.h>
   {MCC_MNC_SEPARATOR, "262080" },
   #include <config_data/carrier_config_262_080.h>
   {MCC_MNC_SEPARATOR, "26209" },
   #include <config_data/carrier_config_262_09.h>
   {MCC_MNC_SEPARATOR, "26211" },
   #include <config_data/carrier_config_262_11.h>
   {MCC_MNC_SEPARATOR, "26221" },/* Spusu Germany */
   #include <config_data/carrier_config_262_21.h>
   {MCC_MNC_SEPARATOR, "26223" },/* 1&1 Germany */
   #include <config_data/carrier_config_262_23.h>
   {MCC_MNC_SEPARATOR, "26277" },
   #include <config_data/carrier_config_262_77.h>
   {MCC_MNC_SEPARATOR, "26280" },
   #include <config_data/carrier_config_262_80.h>
   {MCC_MNC_SEPARATOR, "26288" },
   #include <config_data/carrier_config_262_88.h>
   {MCC_MNC_SEPARATOR, "26295" }, /*Ericsson MFI Lab for NVIOT*/
   #include <config_data/carrier_config_262_95.h>
   {MCC_MNC_SEPARATOR, "26298" }, /*DTAG Germany*/
   #include <config_data/carrier_config_262_98.h>
   {MCC_MNC_SEPARATOR, "26299" },
   #include <config_data/carrier_config_262_99.h>
   {MCC_MNC_SEPARATOR, "26601" }, /*Gibtelecom Gibraltar*/
   #include <config_data/carrier_config_266_01.h>
   {MCC_MNC_SEPARATOR, "26801" },
   #include <config_data/carrier_config_268_01.h>
   {MCC_MNC_SEPARATOR, "26802" }, /*Digi Portugal*/
   #include <config_data/carrier_config_268_02.h>
   {MCC_MNC_SEPARATOR, "26803" }, /*NOS Portugal*/
   #include <config_data/carrier_config_268_03.h>
   {MCC_MNC_SEPARATOR, "26806" },
   #include <config_data/carrier_config_268_06.h>
   {MCC_MNC_SEPARATOR, "26889" },
   #include <config_data/carrier_config_268_89.h>
   {MCC_MNC_SEPARATOR, "26893" }, /*NOS Portugal*/
   #include <config_data/carrier_config_268_93.h>
   {MCC_MNC_SEPARATOR, "26897" }, /*NOS Portugal*/
   #include <config_data/carrier_config_268_97.h>
   {MCC_MNC_SEPARATOR, "26898" }, /*NOS Portugal*/
   #include <config_data/carrier_config_268_98.h>
   {MCC_MNC_SEPARATOR, "26899" }, /*NOS Portugal*/
   #include <config_data/carrier_config_268_99.h>
   {MCC_MNC_SEPARATOR, "27001" },
   #include <config_data/carrier_config_270_01.h>
   {MCC_MNC_SEPARATOR, "27077" },
   #include <config_data/carrier_config_270_77.h>
   {MCC_MNC_SEPARATOR, "27099" },/* Orange Luxembourg */
   #include <config_data/carrier_config_270_99.h>
   {MCC_MNC_SEPARATOR, "27201" },
   #include <config_data/carrier_config_272_01.h>
   {MCC_MNC_SEPARATOR, "27202" },
   #include <config_data/carrier_config_272_02.h>
   {MCC_MNC_SEPARATOR, "27203" }, /* Eir Ireland */
   #include <config_data/carrier_config_272_03.h>
   {MCC_MNC_SEPARATOR, "27205" },
   #include <config_data/carrier_config_272_05.h>
   {MCC_MNC_SEPARATOR, "27401" }, /* Siminn Iceland */
   #include <config_data/carrier_config_274_01.h>
   {MCC_MNC_SEPARATOR, "27402" }, /* Vodafone Iceland */
   #include <config_data/carrier_config_274_02.h>
   {MCC_MNC_SEPARATOR, "27411" }, /* Nova Iceland */
   #include <config_data/carrier_config_274_11.h>
   {MCC_MNC_SEPARATOR, "27601" },
   #include <config_data/carrier_config_276_01.h>
   {MCC_MNC_SEPARATOR, "27602" },
   #include <config_data/carrier_config_276_02.h>
   {MCC_MNC_SEPARATOR, "27801" },
   #include <config_data/carrier_config_278_01.h>
   {MCC_MNC_SEPARATOR, "28001" }, /* CYTA Cyprus */
   #include <config_data/carrier_config_280_01.h>
   {MCC_MNC_SEPARATOR, "28010" }, /* Epic Cyprus */
   #include <config_data/carrier_config_280_10.h>
   {MCC_MNC_SEPARATOR, "28020" }, /* PrimeTel Cyprus */
   #include <config_data/carrier_config_280_20.h>
   {MCC_MNC_SEPARATOR, "28202" }, /* Magti Georgia */
   #include <config_data/carrier_config_282_02.h>
   {MCC_MNC_SEPARATOR, "28305" }, /* MTS Armenia */
   #include <config_data/carrier_config_283_05.h>
   {MCC_MNC_SEPARATOR, "28401" },
   #include <config_data/carrier_config_284_01.h>
   {MCC_MNC_SEPARATOR, "28403" },
   #include <config_data/carrier_config_284_03.h>
   {MCC_MNC_SEPARATOR, "28405" },
   #include <config_data/carrier_config_284_05.h>
   {MCC_MNC_SEPARATOR, "286003" },
   #include <config_data/carrier_config_286_003.h>
   {MCC_MNC_SEPARATOR, "28601" },
   #include <config_data/carrier_config_286_01.h>
   {MCC_MNC_SEPARATOR, "28602" },
   #include <config_data/carrier_config_286_02.h>
   {MCC_MNC_SEPARATOR, "28603" },
   #include <config_data/carrier_config_286_03.h>
   {MCC_MNC_SEPARATOR, "28801" }, /* Foroya Tele */
   #include <config_data/carrier_config_288_01.h>
   {MCC_MNC_SEPARATOR, "28808" },
   #include <config_data/carrier_config_288_08.h>
   {MCC_MNC_SEPARATOR, "29001" },
   #include <config_data/carrier_config_290_01.h>
   {MCC_MNC_SEPARATOR, "29340" },/* A1 Slovenia */
   #include <config_data/carrier_config_293_40.h>
   {MCC_MNC_SEPARATOR, "29341" },/* Telekom Slovenia */
   #include <config_data/carrier_config_293_41.h>
   {MCC_MNC_SEPARATOR, "29364" },/* T2 Slovenia */
   #include <config_data/carrier_config_293_64.h>
   {MCC_MNC_SEPARATOR, "29370" },/* Telemach Slovenia */
   #include <config_data/carrier_config_293_70.h>
   {MCC_MNC_SEPARATOR, "29401" },/* Telekom Macedonia */
   #include <config_data/carrier_config_294_01.h>
   {MCC_MNC_SEPARATOR, "29403" },
   #include <config_data/carrier_config_294_03.h>
   {MCC_MNC_SEPARATOR, "29501" },
   #include <config_data/carrier_config_295_01.h>
   {MCC_MNC_SEPARATOR, "29505" },/* A1 Liechtenstein */
   #include <config_data/carrier_config_295_05.h>
   {MCC_MNC_SEPARATOR, "29701" },
   #include <config_data/carrier_config_297_01.h>
   {MCC_MNC_SEPARATOR, "29702" },/* TMO EU Montenegro */
   #include <config_data/carrier_config_297_02.h>
   {MCC_MNC_SEPARATOR, "29703" },/* mtel Montenegro */
   #include <config_data/carrier_config_297_03.h>
   {MCC_MNC_SEPARATOR, "302220" }, /* Telus */
   #include <config_data/carrier_config_302_220.h>
   {MCC_MNC_SEPARATOR, "302221" }, /* Telus */
   #include <config_data/carrier_config_302_221.h>
   {MCC_MNC_SEPARATOR, "302222" },
   #include <config_data/carrier_config_302_222.h>
   {MCC_MNC_SEPARATOR, "302320" }, /* Chatr/Mobicity (Rogers MVNO) */
   #include <config_data/carrier_config_302_320.h>
   {MCC_MNC_SEPARATOR, "302270" },
   #include <config_data/carrier_config_302_270.h>
   {MCC_MNC_SEPARATOR, "302370" },
   #include <config_data/carrier_config_302_370.h>
   {MCC_MNC_SEPARATOR, "302490" },
   #include <config_data/carrier_config_302_490.h>
   {MCC_MNC_SEPARATOR, "302500" },
   #include <config_data/carrier_config_302_500.h>
   {MCC_MNC_SEPARATOR, "302510" },
   #include <config_data/carrier_config_302_510.h>
   {MCC_MNC_SEPARATOR, "302520" },
   #include <config_data/carrier_config_302_520.h>
   {MCC_MNC_SEPARATOR, "302610" },
   #include <config_data/carrier_config_302_610.h>
   {MCC_MNC_SEPARATOR, "302630" },
   #include <config_data/carrier_config_302_630.h>
   {MCC_MNC_SEPARATOR, "302640" },
   #include <config_data/carrier_config_302_640.h>
   {MCC_MNC_SEPARATOR, "302690" },
   #include <config_data/carrier_config_302_690.h>
   {MCC_MNC_SEPARATOR, "302720" },
   #include <config_data/carrier_config_302_720.h>
   {MCC_MNC_SEPARATOR, "302760" }, /* Telus */
   #include <config_data/carrier_config_302_760.h>
   {MCC_MNC_SEPARATOR, "302780" },
   #include <config_data/carrier_config_302_780.h>
   {MCC_MNC_SEPARATOR, "302990" },
   #include <config_data/carrier_config_302_990.h>
   {MCC_MNC_SEPARATOR, "30803" }, /* Orange-SPM Telecom */
   #include <config_data/carrier_config_308_03.h>
   {MCC_MNC_SEPARATOR, "31000" },
   #include <config_data/carrier_config_310_00.h>
   {MCC_MNC_SEPARATOR, "310000" },
   #include <config_data/carrier_config_310_000.h>
   {MCC_MNC_SEPARATOR, "310003" },
   #include <config_data/carrier_config_310_003.h>
   {MCC_MNC_SEPARATOR, "310004" },
   #include <config_data/carrier_config_310_004.h>
   {MCC_MNC_SEPARATOR, "310005" },
   #include <config_data/carrier_config_310_005.h>
   {MCC_MNC_SEPARATOR, "310006" },
   #include <config_data/carrier_config_310_006.h>
   {MCC_MNC_SEPARATOR, "310008" },/* For Verizon dual SIM*/
   #include <config_data/carrier_config_310_008.h>
   {MCC_MNC_SEPARATOR, "310009" },
   #include <config_data/carrier_config_310_009.h>
   {MCC_MNC_SEPARATOR, "310010" },
   #include <config_data/carrier_config_310_010.h>
   {MCC_MNC_SEPARATOR, "310020" },/*Union NA*/
   #include <config_data/carrier_config_310_020.h>
   {MCC_MNC_SEPARATOR, "31031" },
   #include <config_data/carrier_config_310_31.h>
   {MCC_MNC_SEPARATOR, "31032" },
   #include <config_data/carrier_config_310_32.h>
   {MCC_MNC_SEPARATOR, "310012" },
   #include <config_data/carrier_config_310_012.h>
   {MCC_MNC_SEPARATOR, "310013" },
   #include <config_data/carrier_config_310_013.h>
   {MCC_MNC_SEPARATOR, "310028" }, /* For Verizon Test SIM*/
   #include <config_data/carrier_config_310_028.h>
   {MCC_MNC_SEPARATOR, "310030" },
   #include <config_data/carrier_config_310_030.h>
   {MCC_MNC_SEPARATOR, "310070" },
   #include <config_data/carrier_config_310_070.h>
   {MCC_MNC_SEPARATOR, "310090" },
   #include <config_data/carrier_config_310_090.h>
   {MCC_MNC_SEPARATOR, "310120" },
   #include <config_data/carrier_config_310_120.h>
   {MCC_MNC_SEPARATOR, "310130" },
   #include <config_data/carrier_config_310_130.h>
   {MCC_MNC_SEPARATOR, "310150" },
   #include <config_data/carrier_config_310_150.h>
   { MCC_MNC_SEPARATOR, "310160" },  /* TMO US */
   #include <config_data/carrier_config_310_160.h>
   {MCC_MNC_SEPARATOR, "310170" },
   #include <config_data/carrier_config_310_170.h>
   {MCC_MNC_SEPARATOR, "310180" },
   #include <config_data/carrier_config_310_180.h>
   { MCC_MNC_SEPARATOR, "310200" },  /* TMO US */
   #include <config_data/carrier_config_310_200.h>
   { MCC_MNC_SEPARATOR, "310210" },  /* TMO US */
   #include <config_data/carrier_config_310_210.h>
   { MCC_MNC_SEPARATOR, "310220" },  /* TMO US */
   #include <config_data/carrier_config_310_220.h>
   { MCC_MNC_SEPARATOR, "310230" },  /* TMO US */
   #include <config_data/carrier_config_310_230.h>
   { MCC_MNC_SEPARATOR, "310240" },  /* TMO US */
   #include <config_data/carrier_config_310_240.h>
   { MCC_MNC_SEPARATOR, "310250" },  /* TMO US */
   #include <config_data/carrier_config_310_250.h>
   { MCC_MNC_SEPARATOR, "310260" },  /* TMO US */
   #include <config_data/carrier_config_310_260.h>
   { MCC_MNC_SEPARATOR, "310270" },  /* TMO US */
   #include <config_data/carrier_config_310_270.h>
   {MCC_MNC_SEPARATOR, "310280" },
   #include <config_data/carrier_config_310_280.h>
   {MCC_MNC_SEPARATOR, "310300" },
   #include <config_data/carrier_config_310_300.h>
   { MCC_MNC_SEPARATOR, "310310" },  /* TMO US */
   #include <config_data/carrier_config_310_310.h>
   { MCC_MNC_SEPARATOR, "310330" },
   #include <config_data/carrier_config_310_330.h>
   { MCC_MNC_SEPARATOR, "310490" },  /* TMO US */
   #include <config_data/carrier_config_310_490.h>
   {MCC_MNC_SEPARATOR, "310350" },
   #include <config_data/carrier_config_310_350.h>
   {MCC_MNC_SEPARATOR, "310380" },
   #include <config_data/carrier_config_310_380.h>
   { MCC_MNC_SEPARATOR, "310370" },
   #include <config_data/carrier_config_310_370.h>
   {MCC_MNC_SEPARATOR, "310410" },
   #include <config_data/carrier_config_310_410.h>
   {MCC_MNC_SEPARATOR, "310450" },
   #include <config_data/carrier_config_310_450.h>
   {MCC_MNC_SEPARATOR, "310470" },
   #include <config_data/carrier_config_310_470.h>
   {MCC_MNC_SEPARATOR, "310530" },
   #include <config_data/carrier_config_310_530.h>
   {MCC_MNC_SEPARATOR, "310560" },
   #include <config_data/carrier_config_310_560.h>
   { MCC_MNC_SEPARATOR, "310580" },  /* TMO US */
   #include <config_data/carrier_config_310_580.h>
   {MCC_MNC_SEPARATOR, "310590" }, /* VZW 310590~310599 use the same config with 310590 */
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310591" },
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310592" },
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310593" },
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310594" },
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310595" },
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310596" },
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310597" },
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310598" },
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310599" },
   #include <config_data/carrier_config_310_590.h>
   {MCC_MNC_SEPARATOR, "310630" },/*Thumb cellular NA*/
   #include <config_data/carrier_config_310_630.h>
   { MCC_MNC_SEPARATOR, "310660" },  /* TMO US */
   #include <config_data/carrier_config_310_660.h>
   {MCC_MNC_SEPARATOR, "310640" },
   #include <config_data/carrier_config_310_640.h>
   {MCC_MNC_SEPARATOR, "310680" },
   #include <config_data/carrier_config_310_680.h>
   {MCC_MNC_SEPARATOR, "310710" },/*ASTAC NA*/
   #include <config_data/carrier_config_310_710.h>
   {MCC_MNC_SEPARATOR, "310820" },
   #include <config_data/carrier_config_310_820.h>
   {MCC_MNC_SEPARATOR, "310890" },
   #include <config_data/carrier_config_310_890.h>
   {MCC_MNC_SEPARATOR, "310910" },
   #include <config_data/carrier_config_310_910.h>
   {MCC_MNC_SEPARATOR, "310950" },  /* ATT lab */
   #include <config_data/carrier_config_310_950.h>
   {MCC_MNC_SEPARATOR, "310990" },  /*  Evolve Broadband NA */
   #include <config_data/carrier_config_310_990.h>
   { MCC_MNC_SEPARATOR, "310800" },  /* TMO US */
   #include <config_data/carrier_config_310_800.h>
   { MCC_MNC_SEPARATOR, "311040" },
   #include <config_data/carrier_config_311_040.h>
   { MCC_MNC_SEPARATOR, "311110" },
   #include <config_data/carrier_config_311_110.h>
   { MCC_MNC_SEPARATOR, "311480" },
   #include <config_data/carrier_config_311_480.h>
   { MCC_MNC_SEPARATOR, "46001" },   /*CMCC*/
   #include <config_data/carrier_config_460_01.h>
   { MCC_MNC_SEPARATOR, "46002" },   /*CU*/
   #include <config_data/carrier_config_460_02.h>
   { MCC_MNC_SEPARATOR, "46601" },   /*FET*/
   #include <config_data/carrier_config_466_01.h>
   { MCC_MNC_SEPARATOR, "46692" },  /* CHT */
   #include <config_data/carrier_config_466_92.h>
   {MCC_MNC_SEPARATOR, "311012" },
   #include <config_data/carrier_config_311_012.h>
   {MCC_MNC_SEPARATOR, "311180" },
   #include <config_data/carrier_config_311_180.h>
   {MCC_MNC_SEPARATOR, "31100" },
   #include <config_data/carrier_config_311_00.h>
   {MCC_MNC_SEPARATOR, "311003" },
   #include <config_data/carrier_config_311_003.h>
   {MCC_MNC_SEPARATOR, "311050" }, /*Thumb cellular NA*/
   #include <config_data/carrier_config_311_050.h>
   {MCC_MNC_SEPARATOR, "311220" },
   #include <config_data/carrier_config_311_220.h>
   {MCC_MNC_SEPARATOR, "311221" },
   #include <config_data/carrier_config_311_221.h>
   {MCC_MNC_SEPARATOR, "311222" },
   #include <config_data/carrier_config_311_222.h>
   {MCC_MNC_SEPARATOR, "311223" },
   #include <config_data/carrier_config_311_223.h>
   {MCC_MNC_SEPARATOR, "311224" },
   #include <config_data/carrier_config_311_224.h>
   {MCC_MNC_SEPARATOR, "311225" },
   #include <config_data/carrier_config_311_225.h>
   {MCC_MNC_SEPARATOR, "311226" },
   #include <config_data/carrier_config_311_226.h>
   {MCC_MNC_SEPARATOR, "311227" },
   #include <config_data/carrier_config_311_227.h>
   {MCC_MNC_SEPARATOR, "311228" },
   #include <config_data/carrier_config_311_228.h>
   {MCC_MNC_SEPARATOR, "311229" },
   #include <config_data/carrier_config_311_229.h>
   {MCC_MNC_SEPARATOR, "311270" },
   #include <config_data/carrier_config_311_270.h>
   {MCC_MNC_SEPARATOR, "311271" },
   #include <config_data/carrier_config_311_271.h>
   {MCC_MNC_SEPARATOR, "311272" },
   #include <config_data/carrier_config_311_272.h>
   {MCC_MNC_SEPARATOR, "311273" },
   #include <config_data/carrier_config_311_273.h>
   {MCC_MNC_SEPARATOR, "311274" },
   #include <config_data/carrier_config_311_274.h>
   {MCC_MNC_SEPARATOR, "311275" },
   #include <config_data/carrier_config_311_275.h>
   {MCC_MNC_SEPARATOR, "311276" },
   #include <config_data/carrier_config_311_276.h>
   {MCC_MNC_SEPARATOR, "311277" },
   #include <config_data/carrier_config_311_277.h>
   {MCC_MNC_SEPARATOR, "311278" },
   #include <config_data/carrier_config_311_278.h>
   {MCC_MNC_SEPARATOR, "311279" },
   #include <config_data/carrier_config_311_279.h>
   {MCC_MNC_SEPARATOR, "311280" },
   #include <config_data/carrier_config_311_280.h>
   {MCC_MNC_SEPARATOR, "311281" },
   #include <config_data/carrier_config_311_281.h>
   {MCC_MNC_SEPARATOR, "311282" },
   #include <config_data/carrier_config_311_282.h>
   {MCC_MNC_SEPARATOR, "311283" },
   #include <config_data/carrier_config_311_283.h>
   {MCC_MNC_SEPARATOR, "311284" },
   #include <config_data/carrier_config_311_284.h>
   {MCC_MNC_SEPARATOR, "311285" },
   #include <config_data/carrier_config_311_285.h>
   {MCC_MNC_SEPARATOR, "311286" },
   #include <config_data/carrier_config_311_286.h>
   {MCC_MNC_SEPARATOR, "311287" },
   #include <config_data/carrier_config_311_287.h>
   {MCC_MNC_SEPARATOR, "311288" },
   #include <config_data/carrier_config_311_288.h>
   {MCC_MNC_SEPARATOR, "311289" },
   #include <config_data/carrier_config_311_289.h>
   {MCC_MNC_SEPARATOR, "311390" },/* For Verizon Test SIM */
   #include <config_data/carrier_config_311_390.h>
   {MCC_MNC_SEPARATOR, "311370" },
   #include <config_data/carrier_config_311_370.h>
   {MCC_MNC_SEPARATOR, "311481" },
   #include <config_data/carrier_config_311_481.h>
   {MCC_MNC_SEPARATOR, "311482" },
   #include <config_data/carrier_config_311_482.h>
   {MCC_MNC_SEPARATOR, "311483" },
   #include <config_data/carrier_config_311_483.h>
   {MCC_MNC_SEPARATOR, "311484" },
   #include <config_data/carrier_config_311_484.h>
   {MCC_MNC_SEPARATOR, "311485" },
   #include <config_data/carrier_config_311_485.h>
   {MCC_MNC_SEPARATOR, "311486" },
   #include <config_data/carrier_config_311_486.h>
   {MCC_MNC_SEPARATOR, "311487" },
   #include <config_data/carrier_config_311_487.h>
   {MCC_MNC_SEPARATOR, "311488" },
   #include <config_data/carrier_config_311_488.h>
   {MCC_MNC_SEPARATOR, "311489" },
   #include <config_data/carrier_config_311_489.h>
   {MCC_MNC_SEPARATOR, "311490" },
   #include <config_data/carrier_config_311_490.h>
   {MCC_MNC_SEPARATOR, "311530" },/*ASTAC NA */
   #include <config_data/carrier_config_311_530.h>
   {MCC_MNC_SEPARATOR, "311580" },
   #include <config_data/carrier_config_311_580.h>
   {MCC_MNC_SEPARATOR, "311581" },
   #include <config_data/carrier_config_311_581.h>
   {MCC_MNC_SEPARATOR, "311582" },
   #include <config_data/carrier_config_311_582.h>
   {MCC_MNC_SEPARATOR, "311583" },
   #include <config_data/carrier_config_311_583.h>
   {MCC_MNC_SEPARATOR, "311584" },
   #include <config_data/carrier_config_311_584.h>
   {MCC_MNC_SEPARATOR, "311585" },
   #include <config_data/carrier_config_311_585.h>
   {MCC_MNC_SEPARATOR, "311586" },
   #include <config_data/carrier_config_311_586.h>
   {MCC_MNC_SEPARATOR, "311587" },
   #include <config_data/carrier_config_311_587.h>
   {MCC_MNC_SEPARATOR, "311588" },
   #include <config_data/carrier_config_311_588.h>
   {MCC_MNC_SEPARATOR, "311589" },
   #include <config_data/carrier_config_311_589.h>
   {MCC_MNC_SEPARATOR, "311590" },
   #include <config_data/carrier_config_311_590.h>
   {MCC_MNC_SEPARATOR, "311830" },/*Thumb cellular NA*/
   #include <config_data/carrier_config_311_830.h>
   {MCC_MNC_SEPARATOR, "311870" },
   #include <config_data/carrier_config_311_870.h>
   {MCC_MNC_SEPARATOR, "311880" },
   #include <config_data/carrier_config_311_880.h>
   {MCC_MNC_SEPARATOR, "311910" }, /*MobileNation NA*/
   #include <config_data/carrier_config_311_910.h>
   {MCC_MNC_SEPARATOR, "311940" },
   #include <config_data/carrier_config_311_940.h>
   {MCC_MNC_SEPARATOR, "312010" },
   #include <config_data/carrier_config_312_010.h>
   {MCC_MNC_SEPARATOR, "312190" },
   #include <config_data/carrier_config_312_190.h>
   {MCC_MNC_SEPARATOR, "312250" },
   #include <config_data/carrier_config_312_250.h>
   {MCC_MNC_SEPARATOR, "312420" },
   #include <config_data/carrier_config_312_420.h>
   {MCC_MNC_SEPARATOR, "312530" },
   #include <config_data/carrier_config_312_530.h>
   {MCC_MNC_SEPARATOR, "312670" },
   #include <config_data/carrier_config_312_670.h>
   {MCC_MNC_SEPARATOR, "312720" }, /*Southern LINC NA*/
   #include <config_data/carrier_config_312_720.h>
   {MCC_MNC_SEPARATOR, "312770" },
   #include <config_data/carrier_config_312_770.h>
   {MCC_MNC_SEPARATOR, "313100" },
   #include <config_data/carrier_config_313_100.h>
   {MCC_MNC_SEPARATOR, "313390" }, /* Altice US*/
   #include <config_data/carrier_config_313_390.h>
   {MCC_MNC_SEPARATOR, "313070" },
   #include <config_data/carrier_config_313_070.h>
   {MCC_MNC_SEPARATOR, "313100" },
   #include <config_data/carrier_config_313_100.h>
   {MCC_MNC_SEPARATOR, "313110" },
   #include <config_data/carrier_config_313_110.h>
   {MCC_MNC_SEPARATOR, "313111" },
   #include <config_data/carrier_config_313_111.h>
   {MCC_MNC_SEPARATOR, "313112" },
   #include <config_data/carrier_config_313_112.h>
   {MCC_MNC_SEPARATOR, "313113" },
   #include <config_data/carrier_config_313_113.h>
   {MCC_MNC_SEPARATOR, "313114" },
   #include <config_data/carrier_config_313_114.h>
   {MCC_MNC_SEPARATOR, "313115" },
   #include <config_data/carrier_config_313_115.h>
   {MCC_MNC_SEPARATOR, "313116" },
   #include <config_data/carrier_config_313_116.h>
   {MCC_MNC_SEPARATOR, "313117" },
   #include <config_data/carrier_config_313_117.h>
   {MCC_MNC_SEPARATOR, "313118" },
   #include <config_data/carrier_config_313_118.h>
   {MCC_MNC_SEPARATOR, "313119" },
   #include <config_data/carrier_config_313_119.h>
   {MCC_MNC_SEPARATOR, "313120" },
   #include <config_data/carrier_config_313_120.h>
   {MCC_MNC_SEPARATOR, "313121" },
   #include <config_data/carrier_config_313_121.h>
   {MCC_MNC_SEPARATOR, "313122" },
   #include <config_data/carrier_config_313_122.h>
   {MCC_MNC_SEPARATOR, "313123" },
   #include <config_data/carrier_config_313_123.h>
   {MCC_MNC_SEPARATOR, "313124" },
   #include <config_data/carrier_config_313_124.h>
   {MCC_MNC_SEPARATOR, "313125" },
   #include <config_data/carrier_config_313_125.h>
   {MCC_MNC_SEPARATOR, "313126" },
   #include <config_data/carrier_config_313_126.h>
   {MCC_MNC_SEPARATOR, "313127" },
   #include <config_data/carrier_config_313_127.h>
   {MCC_MNC_SEPARATOR, "313128" },
   #include <config_data/carrier_config_313_128.h>
   {MCC_MNC_SEPARATOR, "313129" },
   #include <config_data/carrier_config_313_129.h>
   {MCC_MNC_SEPARATOR, "313131" },
   #include <config_data/carrier_config_313_131.h>
   {MCC_MNC_SEPARATOR, "313132" },
   #include <config_data/carrier_config_313_132.h>
   {MCC_MNC_SEPARATOR, "313133" },
   #include <config_data/carrier_config_313_133.h>
   {MCC_MNC_SEPARATOR, "313134" },
   #include <config_data/carrier_config_313_134.h>
   {MCC_MNC_SEPARATOR, "313135" },
   #include <config_data/carrier_config_313_135.h>
   {MCC_MNC_SEPARATOR, "313136" },
   #include <config_data/carrier_config_313_136.h>
   {MCC_MNC_SEPARATOR, "313137" },
   #include <config_data/carrier_config_313_137.h>
   {MCC_MNC_SEPARATOR, "313138" },
   #include <config_data/carrier_config_313_138.h>
   {MCC_MNC_SEPARATOR, "313139" },
   #include <config_data/carrier_config_313_139.h>
   {MCC_MNC_SEPARATOR, "313141" },
   #include <config_data/carrier_config_313_141.h>
   {MCC_MNC_SEPARATOR, "313142" },
   #include <config_data/carrier_config_313_142.h>
   {MCC_MNC_SEPARATOR, "313143" },
   #include <config_data/carrier_config_313_143.h>
   {MCC_MNC_SEPARATOR, "313144" },
   #include <config_data/carrier_config_313_144.h>
   {MCC_MNC_SEPARATOR, "313145" },
   #include <config_data/carrier_config_313_145.h>
   {MCC_MNC_SEPARATOR, "313146" },
   #include <config_data/carrier_config_313_146.h>
   {MCC_MNC_SEPARATOR, "313147" },
   #include <config_data/carrier_config_313_147.h>
   {MCC_MNC_SEPARATOR, "313148" },
   #include <config_data/carrier_config_313_148.h>
   {MCC_MNC_SEPARATOR, "313149" },
   #include <config_data/carrier_config_313_149.h>
   {MCC_MNC_SEPARATOR, "313150" },
   #include <config_data/carrier_config_313_150.h>
   {MCC_MNC_SEPARATOR, "313151" },
   #include <config_data/carrier_config_313_151.h>
   {MCC_MNC_SEPARATOR, "313152" },
   #include <config_data/carrier_config_313_152.h>
   {MCC_MNC_SEPARATOR, "313153" },
   #include <config_data/carrier_config_313_153.h>
   {MCC_MNC_SEPARATOR, "313154" },
   #include <config_data/carrier_config_313_154.h>
   {MCC_MNC_SEPARATOR, "313155" },
   #include <config_data/carrier_config_313_155.h>
   {MCC_MNC_SEPARATOR, "313156" },
   #include <config_data/carrier_config_313_156.h>
   {MCC_MNC_SEPARATOR, "313157" },
   #include <config_data/carrier_config_313_157.h>
   {MCC_MNC_SEPARATOR, "313158" },
   #include <config_data/carrier_config_313_158.h>
   {MCC_MNC_SEPARATOR, "313159" },
   #include <config_data/carrier_config_313_159.h>
   {MCC_MNC_SEPARATOR, "313160" },
   #include <config_data/carrier_config_313_160.h>
   {MCC_MNC_SEPARATOR, "313161" },
   #include <config_data/carrier_config_313_161.h>
   {MCC_MNC_SEPARATOR, "313162" },
   #include <config_data/carrier_config_313_162.h>
   {MCC_MNC_SEPARATOR, "313163" },
   #include <config_data/carrier_config_313_163.h>
   {MCC_MNC_SEPARATOR, "313164" },
   #include <config_data/carrier_config_313_164.h>
   {MCC_MNC_SEPARATOR, "313165" },
   #include <config_data/carrier_config_313_165.h>
   {MCC_MNC_SEPARATOR, "313166" },
   #include <config_data/carrier_config_313_166.h>
   {MCC_MNC_SEPARATOR, "313167" },
   #include <config_data/carrier_config_313_167.h>
   {MCC_MNC_SEPARATOR, "313168" },
   #include <config_data/carrier_config_313_168.h>
   {MCC_MNC_SEPARATOR, "313169" },
   #include <config_data/carrier_config_313_169.h>
   {MCC_MNC_SEPARATOR, "313170" },
   #include <config_data/carrier_config_313_170.h>
   {MCC_MNC_SEPARATOR, "313171" },
   #include <config_data/carrier_config_313_171.h>
   {MCC_MNC_SEPARATOR, "313172" },
   #include <config_data/carrier_config_313_172.h>
   {MCC_MNC_SEPARATOR, "313173" },
   #include <config_data/carrier_config_313_173.h>
   {MCC_MNC_SEPARATOR, "313174" },
   #include <config_data/carrier_config_313_174.h>
   {MCC_MNC_SEPARATOR, "313175" },
   #include <config_data/carrier_config_313_175.h>
   {MCC_MNC_SEPARATOR, "313176" },
   #include <config_data/carrier_config_313_176.h>
   {MCC_MNC_SEPARATOR, "313177" },
   #include <config_data/carrier_config_313_177.h>
   {MCC_MNC_SEPARATOR, "313178" },
   #include <config_data/carrier_config_313_178.h>
   {MCC_MNC_SEPARATOR, "313179" },
   #include <config_data/carrier_config_313_179.h>
   {MCC_MNC_SEPARATOR, "313180" },
   #include <config_data/carrier_config_313_180.h>
   {MCC_MNC_SEPARATOR, "313181" },
   #include <config_data/carrier_config_313_181.h>
   {MCC_MNC_SEPARATOR, "313182" },
   #include <config_data/carrier_config_313_182.h>
   {MCC_MNC_SEPARATOR, "313183" },
   #include <config_data/carrier_config_313_183.h>
   {MCC_MNC_SEPARATOR, "313184" },
   #include <config_data/carrier_config_313_184.h>
   {MCC_MNC_SEPARATOR, "313185" },
   #include <config_data/carrier_config_313_185.h>
   {MCC_MNC_SEPARATOR, "313186" },
   #include <config_data/carrier_config_313_186.h>
   {MCC_MNC_SEPARATOR, "313187" },
   #include <config_data/carrier_config_313_187.h>
   {MCC_MNC_SEPARATOR, "313188" },
   #include <config_data/carrier_config_313_188.h>
   {MCC_MNC_SEPARATOR, "313189" },
   #include <config_data/carrier_config_313_189.h>
   {MCC_MNC_SEPARATOR, "313190" },
   #include <config_data/carrier_config_313_190.h>
   {MCC_MNC_SEPARATOR, "313340" },
   #include <config_data/carrier_config_313_340.h>
   {MCC_MNC_SEPARATOR, "313790" },/*LLA*/
   #include <config_data/carrier_config_313_790.h>
   {MCC_MNC_SEPARATOR, "313450" },/*Spectrum*/
   #include <config_data/carrier_config_313_450.h>
   {MCC_MNC_SEPARATOR, "313590" },/*Southern LINC NA*/
   #include <config_data/carrier_config_313_590.h>
   {MCC_MNC_SEPARATOR, "314020" },/*Spectrum*/
   #include <config_data/carrier_config_314_020.h>
   {MCC_MNC_SEPARATOR, "314020" },
   #include <config_data/carrier_config_314_020.h>
   {MCC_MNC_SEPARATOR, "314090" },/*Southern LINC NA */
   #include <config_data/carrier_config_314_090.h>
   {MCC_MNC_SEPARATOR, "314200" },
   #include <config_data/carrier_config_314_200.h>
   {MCC_MNC_SEPARATOR, "330110" },
   #include <config_data/carrier_config_330_110.h>
   {MCC_MNC_SEPARATOR, "334003" },
   #include <config_data/carrier_config_334_003.h>
   {MCC_MNC_SEPARATOR, "33402" },
   #include <config_data/carrier_config_334_02.h>
   {MCC_MNC_SEPARATOR, "334020" },
   #include <config_data/carrier_config_334_020.h>
   {MCC_MNC_SEPARATOR, "33403" },
   #include <config_data/carrier_config_334_03.h>
   {MCC_MNC_SEPARATOR, "334030" },
   #include <config_data/carrier_config_334_030.h>
   {MCC_MNC_SEPARATOR, "334050" },
   #include <config_data/carrier_config_334_050.h>
   {MCC_MNC_SEPARATOR, "334090" },
   #include <config_data/carrier_config_334_090.h>
   {MCC_MNC_SEPARATOR, "334130" },
   #include <config_data/carrier_config_334_130.h>
   {MCC_MNC_SEPARATOR, "334140" },
   #include <config_data/carrier_config_334_140.h>
   {MCC_MNC_SEPARATOR, "338180" },  /* FLOW Jamaica */
   #include <config_data/carrier_config_338_180.h>
   {MCC_MNC_SEPARATOR, "34001" },
   #include <config_data/carrier_config_340_01.h>
   {MCC_MNC_SEPARATOR, "342600" },  /* FLOW Barbados */
   #include <config_data/carrier_config_342_600.h>
   {MCC_MNC_SEPARATOR, "34420" },
   #include <config_data/carrier_config_344_20.h>
   {MCC_MNC_SEPARATOR, "362251" },
   #include <config_data/carrier_config_362_251.h>
   {MCC_MNC_SEPARATOR, "36439" },  /* BTC Bahamas */
   #include <config_data/carrier_config_364_39.h>
   {MCC_MNC_SEPARATOR, "36449" },  /* Aliv/Bahamas */
   #include <config_data/carrier_config_364_49.h>
   {MCC_MNC_SEPARATOR, "37001" },
   #include <config_data/carrier_config_370_01.h>
   {MCC_MNC_SEPARATOR, "37002" },
   #include <config_data/carrier_config_370_02.h>
   {MCC_MNC_SEPARATOR, "37203" },
   #include <config_data/carrier_config_372_03.h>
   {MCC_MNC_SEPARATOR, "40001" },  /* Azercell Azerbaijan */
   #include <config_data/carrier_config_400_01.h>
   {MCC_MNC_SEPARATOR, "40002" },  /* Bakcell Azerbaijan */
   #include <config_data/carrier_config_400_02.h>
   {MCC_MNC_SEPARATOR, "40004" },  /* Nar Azerbaijan */
   #include <config_data/carrier_config_400_04.h>
   {MCC_MNC_SEPARATOR, "40101" },  /* Beeline/Kazakhstan */
   #include <config_data/carrier_config_401_01.h>
   {MCC_MNC_SEPARATOR, "40107" },  /* Alter Kazakhstan */
   #include <config_data/carrier_config_401_07.h>
   {MCC_MNC_SEPARATOR, "40177" },  /* Tele2 Kazakhstan */
   #include <config_data/carrier_config_401_77.h>
   {MCC_MNC_SEPARATOR, "40211" },  /* B-Mobile Bhutan */
   #include <config_data/carrier_config_402_11.h>
   {MCC_MNC_SEPARATOR, "40277" },
   #include <config_data/carrier_config_402_77.h>
   {MCC_MNC_SEPARATOR, "40401" },
   #include <config_data/carrier_config_404_01.h>
   {MCC_MNC_SEPARATOR, "40402" },
   #include <config_data/carrier_config_404_02.h>
   {MCC_MNC_SEPARATOR, "40403" },
   #include <config_data/carrier_config_404_03.h>
   {MCC_MNC_SEPARATOR, "40404" },
   #include <config_data/carrier_config_404_04.h>
   {MCC_MNC_SEPARATOR, "40405" },
   #include <config_data/carrier_config_404_05.h>
   {MCC_MNC_SEPARATOR, "40407" },
   #include <config_data/carrier_config_404_07.h>
   {MCC_MNC_SEPARATOR, "40410" },
   #include <config_data/carrier_config_404_10.h>
   {MCC_MNC_SEPARATOR, "40411" },
   #include <config_data/carrier_config_404_11.h>
   {MCC_MNC_SEPARATOR, "40412" },
   #include <config_data/carrier_config_404_12.h>
   {MCC_MNC_SEPARATOR, "40413" },
   #include <config_data/carrier_config_404_13.h>
   {MCC_MNC_SEPARATOR, "40414" },
   #include <config_data/carrier_config_404_14.h>
   {MCC_MNC_SEPARATOR, "40415" },
   #include <config_data/carrier_config_404_15.h>
   {MCC_MNC_SEPARATOR, "40416" },
   #include <config_data/carrier_config_404_16.h>
   {MCC_MNC_SEPARATOR, "40419" },
   #include <config_data/carrier_config_404_19.h>
   {MCC_MNC_SEPARATOR, "40420" },
   #include <config_data/carrier_config_404_20.h>
   {MCC_MNC_SEPARATOR, "40422" },
   #include <config_data/carrier_config_404_22.h>
   {MCC_MNC_SEPARATOR, "40424" },
   #include <config_data/carrier_config_404_24.h>
   {MCC_MNC_SEPARATOR, "40427" },
   #include <config_data/carrier_config_404_27.h>
   {MCC_MNC_SEPARATOR, "40430" },
   #include <config_data/carrier_config_404_30.h>
   {MCC_MNC_SEPARATOR, "40431" },
   #include <config_data/carrier_config_404_31.h>
   {MCC_MNC_SEPARATOR, "40434" },
   #include <config_data/carrier_config_404_34.h>
   {MCC_MNC_SEPARATOR, "40438" },
   #include <config_data/carrier_config_404_38.h>
   {MCC_MNC_SEPARATOR, "40451" },
   #include <config_data/carrier_config_404_51.h>
   {MCC_MNC_SEPARATOR, "40453" },
   #include <config_data/carrier_config_404_53.h>
   {MCC_MNC_SEPARATOR, "40454" },
   #include <config_data/carrier_config_404_54.h>
   {MCC_MNC_SEPARATOR, "40455" },
   #include <config_data/carrier_config_404_55.h>
   {MCC_MNC_SEPARATOR, "40457" },
   #include <config_data/carrier_config_404_57.h>
   {MCC_MNC_SEPARATOR, "40458" },
   #include <config_data/carrier_config_404_58.h>
   {MCC_MNC_SEPARATOR, "40459" },
   #include <config_data/carrier_config_404_59.h>
   {MCC_MNC_SEPARATOR, "40462" },
   #include <config_data/carrier_config_404_62.h>
   {MCC_MNC_SEPARATOR, "40464" },
   #include <config_data/carrier_config_404_64.h>
   {MCC_MNC_SEPARATOR, "40466" },
   #include <config_data/carrier_config_404_66.h>
   {MCC_MNC_SEPARATOR, "40471" },
   #include <config_data/carrier_config_404_71.h>
   {MCC_MNC_SEPARATOR, "40472" },
   #include <config_data/carrier_config_404_72.h>
   {MCC_MNC_SEPARATOR, "40473" },
   #include <config_data/carrier_config_404_73.h>
   {MCC_MNC_SEPARATOR, "40474" },
   #include <config_data/carrier_config_404_74.h>
   {MCC_MNC_SEPARATOR, "40475" },
   #include <config_data/carrier_config_404_75.h>
   {MCC_MNC_SEPARATOR, "40476" },
   #include <config_data/carrier_config_404_76.h>
   {MCC_MNC_SEPARATOR, "40477" },
   #include <config_data/carrier_config_404_77.h>
   {MCC_MNC_SEPARATOR, "40479" },
   #include <config_data/carrier_config_404_79.h>
   {MCC_MNC_SEPARATOR, "40480" },
   #include <config_data/carrier_config_404_80.h>
   {MCC_MNC_SEPARATOR, "40481" },
   #include <config_data/carrier_config_404_81.h>
   {MCC_MNC_SEPARATOR, "40440" },
   #include <config_data/carrier_config_404_40.h>
   {MCC_MNC_SEPARATOR, "40443" },
   #include <config_data/carrier_config_404_43.h>
   {MCC_MNC_SEPARATOR, "40444" },
   #include <config_data/carrier_config_404_44.h>
   {MCC_MNC_SEPARATOR, "40445" },
   #include <config_data/carrier_config_404_45.h>
   {MCC_MNC_SEPARATOR, "40446" },
   #include <config_data/carrier_config_404_46.h>
   {MCC_MNC_SEPARATOR, "40449" },
   #include <config_data/carrier_config_404_49.h>
   {MCC_MNC_SEPARATOR, "40456" },
   #include <config_data/carrier_config_404_56.h>
   {MCC_MNC_SEPARATOR, "40460" },
   #include <config_data/carrier_config_404_60.h>
   {MCC_MNC_SEPARATOR, "40470" },
   #include <config_data/carrier_config_404_70.h>
   {MCC_MNC_SEPARATOR, "40478" },
   #include <config_data/carrier_config_404_78.h>
   {MCC_MNC_SEPARATOR, "40482" },
   #include <config_data/carrier_config_404_82.h>
   {MCC_MNC_SEPARATOR, "40484" },
   #include <config_data/carrier_config_404_84.h>
   {MCC_MNC_SEPARATOR, "40486" },
   #include <config_data/carrier_config_404_86.h>
   {MCC_MNC_SEPARATOR, "40487" },
   #include <config_data/carrier_config_404_87.h>
   {MCC_MNC_SEPARATOR, "40488" },
   #include <config_data/carrier_config_404_88.h>
   {MCC_MNC_SEPARATOR, "40489" },
   #include <config_data/carrier_config_404_89.h>
   {MCC_MNC_SEPARATOR, "40490" },
   #include <config_data/carrier_config_404_90.h>
   {MCC_MNC_SEPARATOR, "40492" },
   #include <config_data/carrier_config_404_92.h>
   {MCC_MNC_SEPARATOR, "40493" },
   #include <config_data/carrier_config_404_93.h>
   {MCC_MNC_SEPARATOR, "40494" },
   #include <config_data/carrier_config_404_94.h>
   {MCC_MNC_SEPARATOR, "40495" },
   #include <config_data/carrier_config_404_95.h>
   {MCC_MNC_SEPARATOR, "40496" },
   #include <config_data/carrier_config_404_96.h>
   {MCC_MNC_SEPARATOR, "40497" },
   #include <config_data/carrier_config_404_97.h>
   {MCC_MNC_SEPARATOR, "40498" },
   #include <config_data/carrier_config_404_98.h>
   {MCC_MNC_SEPARATOR, "40525" },
   #include <config_data/carrier_config_405_25.h>
   {MCC_MNC_SEPARATOR, "40527" },
   #include <config_data/carrier_config_405_27.h>
   {MCC_MNC_SEPARATOR, "40530" },
   #include <config_data/carrier_config_405_30.h>
   {MCC_MNC_SEPARATOR, "40531" },
   #include <config_data/carrier_config_405_31.h>
   {MCC_MNC_SEPARATOR, "40532" },
   #include <config_data/carrier_config_405_32.h>
   {MCC_MNC_SEPARATOR, "40534" },
   #include <config_data/carrier_config_405_34.h>
   {MCC_MNC_SEPARATOR, "40535" },
   #include <config_data/carrier_config_405_35.h>
   {MCC_MNC_SEPARATOR, "40536" },
   #include <config_data/carrier_config_405_36.h>
   {MCC_MNC_SEPARATOR, "40537" },
   #include <config_data/carrier_config_405_37.h>
   {MCC_MNC_SEPARATOR, "40538" },
   #include <config_data/carrier_config_405_38.h>
   {MCC_MNC_SEPARATOR, "40539" },
   #include <config_data/carrier_config_405_39.h>
   {MCC_MNC_SEPARATOR, "40541" },
   #include <config_data/carrier_config_405_41.h>
   {MCC_MNC_SEPARATOR, "40542" },
   #include <config_data/carrier_config_405_42.h>
   {MCC_MNC_SEPARATOR, "40543" },
   #include <config_data/carrier_config_405_43.h>
   {MCC_MNC_SEPARATOR, "40544" },
   #include <config_data/carrier_config_405_44.h>
   {MCC_MNC_SEPARATOR, "40545" },
   #include <config_data/carrier_config_405_45.h>
   {MCC_MNC_SEPARATOR, "40546" },
   #include <config_data/carrier_config_405_46.h>
   {MCC_MNC_SEPARATOR, "40547" },
   #include <config_data/carrier_config_405_47.h>
   {MCC_MNC_SEPARATOR, "405030" },
   #include <config_data/carrier_config_405_030.h>
   {MCC_MNC_SEPARATOR, "405035" },
   #include <config_data/carrier_config_405_035.h>
   {MCC_MNC_SEPARATOR, "405036" },
   #include <config_data/carrier_config_405_036.h>
   {MCC_MNC_SEPARATOR, "405037" },
   #include <config_data/carrier_config_405_037.h>
   {MCC_MNC_SEPARATOR, "405038" },
   #include <config_data/carrier_config_405_038.h>
   {MCC_MNC_SEPARATOR, "405039" },
   #include <config_data/carrier_config_405_039.h>
   {MCC_MNC_SEPARATOR, "405044" },
   #include <config_data/carrier_config_405_044.h>
   {MCC_MNC_SEPARATOR, "40551" },
   #include <config_data/carrier_config_405_51.h>
   {MCC_MNC_SEPARATOR, "40552" },
   #include <config_data/carrier_config_405_52.h>
   {MCC_MNC_SEPARATOR, "40553" },
   #include <config_data/carrier_config_405_53.h>
   {MCC_MNC_SEPARATOR, "40554" },
   #include <config_data/carrier_config_405_54.h>
   {MCC_MNC_SEPARATOR, "40555" },
   #include <config_data/carrier_config_405_55.h>
   {MCC_MNC_SEPARATOR, "40556" },
   #include <config_data/carrier_config_405_56.h>
   {MCC_MNC_SEPARATOR, "40566" },
   #include <config_data/carrier_config_405_66.h>
   {MCC_MNC_SEPARATOR, "40567" },
   #include <config_data/carrier_config_405_67.h>
   {MCC_MNC_SEPARATOR, "40570" },
   #include <config_data/carrier_config_405_70.h>
   {MCC_MNC_SEPARATOR, "405750" },
   #include <config_data/carrier_config_405_750.h>
   {MCC_MNC_SEPARATOR, "405751" },
   #include <config_data/carrier_config_405_751.h>
   {MCC_MNC_SEPARATOR, "405752" },
   #include <config_data/carrier_config_405_752.h>
   {MCC_MNC_SEPARATOR, "405753" },
   #include <config_data/carrier_config_405_753.h>
   {MCC_MNC_SEPARATOR, "405754" },
   #include <config_data/carrier_config_405_754.h>
   {MCC_MNC_SEPARATOR, "405755" },
   #include <config_data/carrier_config_405_755.h>
   {MCC_MNC_SEPARATOR, "405756" },
   #include <config_data/carrier_config_405_756.h>
   {MCC_MNC_SEPARATOR, "405799" },
   #include <config_data/carrier_config_405_799.h>
   {MCC_MNC_SEPARATOR, "405818" },
   #include <config_data/carrier_config_405_818.h>
   {MCC_MNC_SEPARATOR, "405819" },
   #include <config_data/carrier_config_405_819.h>
   {MCC_MNC_SEPARATOR, "405840" },
   #include <config_data/carrier_config_405_840.h>
   {MCC_MNC_SEPARATOR, "405845" },
   #include <config_data/carrier_config_405_845.h>
   {MCC_MNC_SEPARATOR, "405846" },
   #include <config_data/carrier_config_405_846.h>
   {MCC_MNC_SEPARATOR, "405848" },
   #include <config_data/carrier_config_405_848.h>
   {MCC_MNC_SEPARATOR, "405849" },
   #include <config_data/carrier_config_405_849.h>
   {MCC_MNC_SEPARATOR, "405850" },
   #include <config_data/carrier_config_405_850.h>
   {MCC_MNC_SEPARATOR, "405852" },
   #include <config_data/carrier_config_405_852.h>
   {MCC_MNC_SEPARATOR, "405853" },
   #include <config_data/carrier_config_405_853.h>
   {MCC_MNC_SEPARATOR, "405854" },
   #include <config_data/carrier_config_405_854.h>
   {MCC_MNC_SEPARATOR, "405855" },
   #include <config_data/carrier_config_405_855.h>
   {MCC_MNC_SEPARATOR, "405856" },
   #include <config_data/carrier_config_405_856.h>
   {MCC_MNC_SEPARATOR, "405857" },
   #include <config_data/carrier_config_405_857.h>
   {MCC_MNC_SEPARATOR, "405858" },
   #include <config_data/carrier_config_405_858.h>
   {MCC_MNC_SEPARATOR, "405859" },
   #include <config_data/carrier_config_405_859.h>
   {MCC_MNC_SEPARATOR, "405860" },
   #include <config_data/carrier_config_405_860.h>
   {MCC_MNC_SEPARATOR, "405861" },
   #include <config_data/carrier_config_405_861.h>
   {MCC_MNC_SEPARATOR, "405862" },
   #include <config_data/carrier_config_405_862.h>
   {MCC_MNC_SEPARATOR, "405863" },
   #include <config_data/carrier_config_405_863.h>
   {MCC_MNC_SEPARATOR, "405864" },
   #include <config_data/carrier_config_405_864.h>
   {MCC_MNC_SEPARATOR, "405865" },
   #include <config_data/carrier_config_405_865.h>
   {MCC_MNC_SEPARATOR, "405866" },
   #include <config_data/carrier_config_405_866.h>
   {MCC_MNC_SEPARATOR, "405867" },
   #include <config_data/carrier_config_405_867.h>
   {MCC_MNC_SEPARATOR, "405868" },
   #include <config_data/carrier_config_405_868.h>
   {MCC_MNC_SEPARATOR, "405869" },
   #include <config_data/carrier_config_405_869.h>
   {MCC_MNC_SEPARATOR, "40587" },
   #include <config_data/carrier_config_405_87.h>
   {MCC_MNC_SEPARATOR, "405870" },
   #include <config_data/carrier_config_405_870.h>
   {MCC_MNC_SEPARATOR, "405871" },
   #include <config_data/carrier_config_405_871.h>
   {MCC_MNC_SEPARATOR, "405872" },
   #include <config_data/carrier_config_405_872.h>
   {MCC_MNC_SEPARATOR, "405873" },
   #include <config_data/carrier_config_405_873.h>
   {MCC_MNC_SEPARATOR, "405874" },
   #include <config_data/carrier_config_405_874.h>
   {MCC_MNC_SEPARATOR, "405876" },
   #include <config_data/carrier_config_405_876.h>
   {MCC_MNC_SEPARATOR, "405879" },
   #include <config_data/carrier_config_405_879.h>
   {MCC_MNC_SEPARATOR, "405927" },
   #include <config_data/carrier_config_405_927.h>
   {MCC_MNC_SEPARATOR, "405929" },
   #include <config_data/carrier_config_405_929.h>
   {MCC_MNC_SEPARATOR, "41001" },
   #include <config_data/carrier_config_410_01.h>
   {MCC_MNC_SEPARATOR, "41003" },
   #include <config_data/carrier_config_410_03.h>
   {MCC_MNC_SEPARATOR, "41004" },
   #include <config_data/carrier_config_410_04.h>
  {MCC_MNC_SEPARATOR, "41006" },
   #include <config_data/carrier_config_410_06.h>
   {MCC_MNC_SEPARATOR, "41007" },
   #include <config_data/carrier_config_410_07.h>
   {MCC_MNC_SEPARATOR, "41301" },
   #include <config_data/carrier_config_413_01.h>
   {MCC_MNC_SEPARATOR, "41302" },
   #include <config_data/carrier_config_413_02.h>
   {MCC_MNC_SEPARATOR, "41305" },
   #include <config_data/carrier_config_413_05.h>
   {MCC_MNC_SEPARATOR, "41400" }, /*MPT Myanmar*/
   #include <config_data/carrier_config_414_00.h>
   {MCC_MNC_SEPARATOR, "414006" },
   #include <config_data/carrier_config_414_006.h>
   {MCC_MNC_SEPARATOR, "41401" }, /*MPT Myanmar*/
   #include <config_data/carrier_config_414_01.h>
   {MCC_MNC_SEPARATOR, "41402" }, /*MPT Myanmar*/
   #include <config_data/carrier_config_414_02.h>
   {MCC_MNC_SEPARATOR, "41403" }, /*MECTEL Myanmar*/
   #include <config_data/carrier_config_414_03.h>
   {MCC_MNC_SEPARATOR, "41404" }, /*MPT Myanmar*/
   #include <config_data/carrier_config_414_04.h>
   {MCC_MNC_SEPARATOR, "41405" }, /*Ooredoo Myanmar*/
   #include <config_data/carrier_config_414_05.h>
   {MCC_MNC_SEPARATOR, "41406" },
   #include <config_data/carrier_config_414_06.h>
   {MCC_MNC_SEPARATOR, "41409" },
   #include <config_data/carrier_config_414_09.h>
   {MCC_MNC_SEPARATOR, "41501" },
   #include <config_data/carrier_config_415_01.h>
   {MCC_MNC_SEPARATOR, "41601" }, /*Zain Jordan*/
   #include <config_data/carrier_config_416_01.h>
   {MCC_MNC_SEPARATOR, "41603" }, /*Umniah Jordan*/
   #include <config_data/carrier_config_416_03.h>
   {MCC_MNC_SEPARATOR, "41677" }, /*Orange Jordan*/
   #include <config_data/carrier_config_416_77.h>
   {MCC_MNC_SEPARATOR, "41805" }, /* AsiaCell Iraq */
   #include <config_data/carrier_config_418_05.h>
   {MCC_MNC_SEPARATOR, "41820" }, /* Zain Iraq */
   #include <config_data/carrier_config_418_20.h>
   {MCC_MNC_SEPARATOR, "41830" }, /* Zain Iraq */
   #include <config_data/carrier_config_418_30.h>
   {MCC_MNC_SEPARATOR, "41866" },
   #include <config_data/carrier_config_418_66.h>
   {MCC_MNC_SEPARATOR, "41902" }, /* Zain Kuwait */
   #include <config_data/carrier_config_419_02.h>
   {MCC_MNC_SEPARATOR, "41903" },
   #include <config_data/carrier_config_419_03.h>
   {MCC_MNC_SEPARATOR, "41904" }, /* Viva(STC) Kuwait */
   #include <config_data/carrier_config_419_04.h>
   {MCC_MNC_SEPARATOR, "41909" },
   #include <config_data/carrier_config_419_09.h>
   {MCC_MNC_SEPARATOR, "42001" },
   #include <config_data/carrier_config_420_01.h>
   {MCC_MNC_SEPARATOR, "42003" },
   #include <config_data/carrier_config_420_03.h>
   {MCC_MNC_SEPARATOR, "42004" },
   #include <config_data/carrier_config_420_04.h>
   {MCC_MNC_SEPARATOR, "42005" }, /* Virgin Saudi Arabia */
   #include <config_data/carrier_config_420_05.h>
   {MCC_MNC_SEPARATOR, "42006" }, /* Lebara Mobile Saudi Arabia */
   #include <config_data/carrier_config_420_06.h>
   {MCC_MNC_SEPARATOR, "42009" },
   #include <config_data/carrier_config_420_09.h>
   {MCC_MNC_SEPARATOR, "42010" }, /* Redbull Saudi Arabia */
   #include <config_data/carrier_config_420_10.h>
   {MCC_MNC_SEPARATOR, "42103" }, /*Yemen Mobile Yemen*/
   #include <config_data/carrier_config_421_03.h>
   {MCC_MNC_SEPARATOR, "42111" }, /*Yemen Mobile Yemen*/
   #include <config_data/carrier_config_421_11.h>
   {MCC_MNC_SEPARATOR, "42202" }, /*Omantel Oman*/
   #include <config_data/carrier_config_422_02.h>
   {MCC_MNC_SEPARATOR, "42203" }, /*Ooredoo Oman*/
   #include <config_data/carrier_config_422_03.h>
   {MCC_MNC_SEPARATOR, "42206" }, /*Vodafone Oman*/
   #include <config_data/carrier_config_422_06.h>
   {MCC_MNC_SEPARATOR, "42402" },
   #include <config_data/carrier_config_424_02.h>
   {MCC_MNC_SEPARATOR, "42403" },
   #include <config_data/carrier_config_424_03.h>
   {MCC_MNC_SEPARATOR, "42410" }, /*ATLAS TELECOM UAE*/
   #include <config_data/carrier_config_424_10.h>
   {MCC_MNC_SEPARATOR, "42411" }, /*Nedaa UAE*/
   #include <config_data/carrier_config_424_11.h>
   {MCC_MNC_SEPARATOR, "42501" },
   #include <config_data/carrier_config_425_01.h>
   {MCC_MNC_SEPARATOR, "42502" },
   #include <config_data/carrier_config_425_02.h>
   {MCC_MNC_SEPARATOR, "42503" },
   #include <config_data/carrier_config_425_03.h>
   {MCC_MNC_SEPARATOR, "42507" },
   #include <config_data/carrier_config_425_07.h>
   {MCC_MNC_SEPARATOR, "42509" },
   #include <config_data/carrier_config_425_09.h>
   {MCC_MNC_SEPARATOR, "42510" },
   #include <config_data/carrier_config_425_10.h>
   {MCC_MNC_SEPARATOR, "42519" }, /*019 Mobile Israel*/
   #include <config_data/carrier_config_425_19.h>
   {MCC_MNC_SEPARATOR, "42526" }, /*Annatel Israel*/
   #include <config_data/carrier_config_425_26.h>
   {MCC_MNC_SEPARATOR, "42601" }, /*Batelco Bahrain*/
   #include <config_data/carrier_config_426_01.h>
   {MCC_MNC_SEPARATOR, "42602" }, /* Zain Bahrain */
   #include <config_data/carrier_config_426_02.h>
   {MCC_MNC_SEPARATOR, "42604" }, /* Viva(STC) Bahrain */
   #include <config_data/carrier_config_426_04.h>
   {MCC_MNC_SEPARATOR, "42701" },
   #include <config_data/carrier_config_427_01.h> /* Ooredoo Qatar */
   {MCC_MNC_SEPARATOR, "42702" },
   #include <config_data/carrier_config_427_02.h> /* Vodafone Qatar */
   {MCC_MNC_SEPARATOR, "42707" },
   #include <config_data/carrier_config_427_07.h> /* Vodafone Qatar */
   {MCC_MNC_SEPARATOR, "42899" },  /* Mobicom Mongolia */
   #include <config_data/carrier_config_428_99.h>
   {MCC_MNC_SEPARATOR, "42901" },  /* NTC Nepal */
   #include <config_data/carrier_config_429_01.h>
   {MCC_MNC_SEPARATOR, "42902" },  /* Ncell Nepal */
   #include <config_data/carrier_config_429_02.h>
   {MCC_MNC_SEPARATOR, "43404" },  /* Beeline Uzbekistan */
   #include <config_data/carrier_config_434_04.h>
   {MCC_MNC_SEPARATOR, "43405" },  /* Ucell Uzbekistan */
   #include <config_data/carrier_config_434_05.h>
   {MCC_MNC_SEPARATOR, "43407" },  /* Mobiuz Uzbekistan */
   #include <config_data/carrier_config_434_07.h>
   {MCC_MNC_SEPARATOR, "44000" },
   #include <config_data/carrier_config_440_00.h>
   {MCC_MNC_SEPARATOR, "44007" },
   #include <config_data/carrier_config_440_07.h>
   {MCC_MNC_SEPARATOR, "44008" },
   #include <config_data/carrier_config_440_08.h>
   {MCC_MNC_SEPARATOR, "44010" },
   #include <config_data/carrier_config_440_10.h>
   {MCC_MNC_SEPARATOR, "44011" },
   #include <config_data/carrier_config_440_11.h> /* Rakuten Japan */
   {MCC_MNC_SEPARATOR, "44020" },
   #include <config_data/carrier_config_440_20.h>
   {MCC_MNC_SEPARATOR, "44050" },
   #include <config_data/carrier_config_440_50.h>
   {MCC_MNC_SEPARATOR, "44051" },
   #include <config_data/carrier_config_440_51.h>
   {MCC_MNC_SEPARATOR, "44052" },
   #include <config_data/carrier_config_440_52.h>
   {MCC_MNC_SEPARATOR, "44053" },
   #include <config_data/carrier_config_440_53.h>
   {MCC_MNC_SEPARATOR, "44054" },
   #include <config_data/carrier_config_440_54.h>
   {MCC_MNC_SEPARATOR, "44055" },
   #include <config_data/carrier_config_440_55.h>
   {MCC_MNC_SEPARATOR, "44056" },
   #include <config_data/carrier_config_440_56.h>
   {MCC_MNC_SEPARATOR, "44070" },
   #include <config_data/carrier_config_440_70.h>
   {MCC_MNC_SEPARATOR, "44071" },
   #include <config_data/carrier_config_440_71.h>
   {MCC_MNC_SEPARATOR, "44072" },
   #include <config_data/carrier_config_440_72.h>
   {MCC_MNC_SEPARATOR, "44073" },
   #include <config_data/carrier_config_440_73.h>
   {MCC_MNC_SEPARATOR, "44074" },
   #include <config_data/carrier_config_440_74.h>
   {MCC_MNC_SEPARATOR, "44075" },
   #include <config_data/carrier_config_440_75.h>
   {MCC_MNC_SEPARATOR, "44076" },
   #include <config_data/carrier_config_440_76.h>
   {MCC_MNC_SEPARATOR, "44077" },
   #include <config_data/carrier_config_440_77.h>
   {MCC_MNC_SEPARATOR, "44078" },
   #include <config_data/carrier_config_440_78.h>
   {MCC_MNC_SEPARATOR, "44079" },
   #include <config_data/carrier_config_440_79.h>
   {MCC_MNC_SEPARATOR, "44088" },
   #include <config_data/carrier_config_440_88.h>
   {MCC_MNC_SEPARATOR, "44089" },
   #include <config_data/carrier_config_440_89.h>
   {MCC_MNC_SEPARATOR, "44170" },
   #include <config_data/carrier_config_441_70.h>
   {MCC_MNC_SEPARATOR, "45005" },
   #include <config_data/carrier_config_450_05.h>
   {MCC_MNC_SEPARATOR, "45002" },
   #include <config_data/carrier_config_450_02.h>
   {MCC_MNC_SEPARATOR, "45004" },
   #include <config_data/carrier_config_450_04.h>
   {MCC_MNC_SEPARATOR, "45006" },
   #include <config_data/carrier_config_450_06.h>
   {MCC_MNC_SEPARATOR, "45008" },
   #include <config_data/carrier_config_450_08.h>
   {MCC_MNC_SEPARATOR, "45011" },
   #include <config_data/carrier_config_450_11.h>
   {MCC_MNC_SEPARATOR, "45012" },
   #include <config_data/carrier_config_450_12.h>
   {MCC_MNC_SEPARATOR, "45201" }, /* MobiFone Vietnam */
   #include <config_data/carrier_config_452_01.h>
   {MCC_MNC_SEPARATOR, "45202" }, /* Vinaphone Vietnam */
   #include <config_data/carrier_config_452_02.h>
   {MCC_MNC_SEPARATOR, "45204" }, /* Viettel Vietnam */
   #include <config_data/carrier_config_452_04.h>
   {MCC_MNC_SEPARATOR, "45205" },
   #include <config_data/carrier_config_452_05.h>
   {MCC_MNC_SEPARATOR, "45400" },
   #include <config_data/carrier_config_454_00.h>
   {MCC_MNC_SEPARATOR, "45402" },
   #include <config_data/carrier_config_454_02.h>
   {MCC_MNC_SEPARATOR, "45403" },
   #include <config_data/carrier_config_454_03.h>
   {MCC_MNC_SEPARATOR, "45404" },
   #include <config_data/carrier_config_454_04.h>
   {MCC_MNC_SEPARATOR, "45405" },
   #include <config_data/carrier_config_454_05.h>
   {MCC_MNC_SEPARATOR, "45406" },
   #include <config_data/carrier_config_454_06.h>
   {MCC_MNC_SEPARATOR, "45407" }, /* CUHK HongKong */
   #include <config_data/carrier_config_454_07.h>
   {MCC_MNC_SEPARATOR, "45410" },
   #include <config_data/carrier_config_454_10.h>
   {MCC_MNC_SEPARATOR, "45412" },
   #include <config_data/carrier_config_454_12.h>
   {MCC_MNC_SEPARATOR, "45413" },
   #include <config_data/carrier_config_454_13.h>
   {MCC_MNC_SEPARATOR, "45414" },
   #include <config_data/carrier_config_454_14.h>
   {MCC_MNC_SEPARATOR, "45415" },
   #include <config_data/carrier_config_454_15.h>
   {MCC_MNC_SEPARATOR, "45416" },
   #include <config_data/carrier_config_454_16.h>
   {MCC_MNC_SEPARATOR, "45417" },
   #include <config_data/carrier_config_454_17.h>
   {MCC_MNC_SEPARATOR, "45418" },
   #include <config_data/carrier_config_454_18.h>
   {MCC_MNC_SEPARATOR, "45419" },
   #include <config_data/carrier_config_454_19.h>
   {MCC_MNC_SEPARATOR, "45420" },
   #include <config_data/carrier_config_454_20.h>
   {MCC_MNC_SEPARATOR, "45429" },
   #include <config_data/carrier_config_454_29.h>
   {MCC_MNC_SEPARATOR, "45430" },
   #include <config_data/carrier_config_454_30.h>
   {MCC_MNC_SEPARATOR, "45431" },  /*CTExcel HK*/
   #include <config_data/carrier_config_454_31.h>
   {MCC_MNC_SEPARATOR, "45501" },    /*CTM*/
   #include <config_data/carrier_config_455_01.h>
   {MCC_MNC_SEPARATOR, "45502" },
   #include <config_data/carrier_config_455_02.h>
   {MCC_MNC_SEPARATOR, "45503" },    /*THREE*/
   #include <config_data/carrier_config_455_03.h>
   {MCC_MNC_SEPARATOR, "45504" },    /*CTM*/
   #include <config_data/carrier_config_455_04.h>
   {MCC_MNC_SEPARATOR, "45505" },    /*THREE*/
   #include <config_data/carrier_config_455_05.h>
   {MCC_MNC_SEPARATOR, "45507" },
   #include <config_data/carrier_config_455_07.h>
   {MCC_MNC_SEPARATOR, "45601" },
   #include <config_data/carrier_config_456_01.h>
   {MCC_MNC_SEPARATOR, "45606" },
   #include <config_data/carrier_config_456_06.h>
   {MCC_MNC_SEPARATOR, "45608" }, /*Metfone Cambodia*/
   #include <config_data/carrier_config_456_08.h>
   {MCC_MNC_SEPARATOR, "45611" },
   #include <config_data/carrier_config_456_11.h>
   {MCC_MNC_SEPARATOR, "45701" }, /*LaoTel Laos*/
   #include <config_data/carrier_config_457_01.h>
   {MCC_MNC_SEPARATOR, "45703" }, /*Unitel Laos*/
   #include <config_data/carrier_config_457_03.h>
   {MCC_MNC_SEPARATOR, "45707" }, /*Best Telecom Laos*/
   #include <config_data/carrier_config_457_07.h>
   {MCC_MNC_SEPARATOR, "46000" },
   #include <config_data/carrier_config_460_00.h>
   {MCC_MNC_SEPARATOR, "46003" },
   #include <config_data/carrier_config_460_03.h>
   {MCC_MNC_SEPARATOR, "46004" },
   #include <config_data/carrier_config_460_04.h>
   {MCC_MNC_SEPARATOR, "46006" },
   #include <config_data/carrier_config_460_06.h>
   {MCC_MNC_SEPARATOR, "46007" },
   #include <config_data/carrier_config_460_07.h>
   {MCC_MNC_SEPARATOR, "46008" },
   #include <config_data/carrier_config_460_08.h>
   {MCC_MNC_SEPARATOR, "46009" },
   #include <config_data/carrier_config_460_09.h>
   {MCC_MNC_SEPARATOR, "46011" },
   #include <config_data/carrier_config_460_11.h>
   {MCC_MNC_SEPARATOR, "46012" },
   #include <config_data/carrier_config_460_12.h>
   {MCC_MNC_SEPARATOR, "46013" },
   #include <config_data/carrier_config_460_13.h>
   {MCC_MNC_SEPARATOR, "46015" },
   #include <config_data/carrier_config_460_15.h>
   {MCC_MNC_SEPARATOR, "46019" },
   #include <config_data/carrier_config_460_19.h>
   {MCC_MNC_SEPARATOR, "46020" },
   #include <config_data/carrier_config_460_20.h>
   {MCC_MNC_SEPARATOR, "46030" },
   #include <config_data/carrier_config_460_30.h>
   {MCC_MNC_SEPARATOR, "46070" },
   #include <config_data/carrier_config_460_70.h>
   {MCC_MNC_SEPARATOR, "46096" },
   #include <config_data/carrier_config_460_96.h>
   {MCC_MNC_SEPARATOR, "46002" },
   #include <config_data/carrier_config_460_02.h>
   {MCC_MNC_SEPARATOR, "46003" },
   #include <config_data/carrier_config_460_03.h>
   {MCC_MNC_SEPARATOR, "46605" },
   #include <config_data/carrier_config_466_05.h>
   {MCC_MNC_SEPARATOR, "46606" },
   #include <config_data/carrier_config_466_06.h>
   {MCC_MNC_SEPARATOR, "46607" },
   #include <config_data/carrier_config_466_07.h>
   {MCC_MNC_SEPARATOR, "46611" },
   #include <config_data/carrier_config_466_11.h>
   {MCC_MNC_SEPARATOR, "46688" },
   #include <config_data/carrier_config_466_88.h>
   {MCC_MNC_SEPARATOR, "46689" },
   #include <config_data/carrier_config_466_89.h>
   {MCC_MNC_SEPARATOR, "46697" },
   #include <config_data/carrier_config_466_97.h>
   {MCC_MNC_SEPARATOR, "46699" },
   #include <config_data/carrier_config_466_99.h>
   {MCC_MNC_SEPARATOR, "47001" },
   #include <config_data/carrier_config_470_01.h>
   {MCC_MNC_SEPARATOR, "47002" },
   #include <config_data/carrier_config_470_02.h>
   {MCC_MNC_SEPARATOR, "47007" },
   #include <config_data/carrier_config_470_07.h>
   {MCC_MNC_SEPARATOR, "47202" }, /*Ooredoo Maldives*/
   #include <config_data/carrier_config_472_02.h>
   {MCC_MNC_SEPARATOR, "50208" },
   #include <config_data/carrier_config_502_08.h>
   {MCC_MNC_SEPARATOR, "50212" },
   #include <config_data/carrier_config_502_12.h>
   {MCC_MNC_SEPARATOR, "50213" },
   #include <config_data/carrier_config_502_13.h>
   {MCC_MNC_SEPARATOR, "502152" },
   #include <config_data/carrier_config_502_152.h>
   {MCC_MNC_SEPARATOR, "502153" },
   #include <config_data/carrier_config_502_153.h>
   {MCC_MNC_SEPARATOR, "50216" },
   #include <config_data/carrier_config_502_16.h>
   {MCC_MNC_SEPARATOR, "50218" },
   #include <config_data/carrier_config_502_18.h>
   {MCC_MNC_SEPARATOR, "50219" },
   #include <config_data/carrier_config_502_19.h>
   {MCC_MNC_SEPARATOR, "50501" },
   #include <config_data/carrier_config_505_01.h>
   {MCC_MNC_SEPARATOR, "50502" },
   #include <config_data/carrier_config_505_02.h>
   {MCC_MNC_SEPARATOR, "50503" },
   #include <config_data/carrier_config_505_03.h>
   {MCC_MNC_SEPARATOR, "50506" },
   #include <config_data/carrier_config_505_06.h>
   {MCC_MNC_SEPARATOR, "50511" },
   #include <config_data/carrier_config_505_11.h>
   {MCC_MNC_SEPARATOR, "50571" },
   #include <config_data/carrier_config_505_71.h>
   {MCC_MNC_SEPARATOR, "50572" },
   #include <config_data/carrier_config_505_72.h>
   {MCC_MNC_SEPARATOR, "51001" },
   #include <config_data/carrier_config_510_01.h>
   {MCC_MNC_SEPARATOR, "51009" },
   #include <config_data/carrier_config_510_09.h>
   {MCC_MNC_SEPARATOR, "51010" },
   #include <config_data/carrier_config_510_10.h>
   {MCC_MNC_SEPARATOR, "51011" }, /* XL Indonesia */
   #include <config_data/carrier_config_510_11.h>
   {MCC_MNC_SEPARATOR, "51021" },
   #include <config_data/carrier_config_510_21.h>
   {MCC_MNC_SEPARATOR, "51028" },
   #include <config_data/carrier_config_510_28.h>
   {MCC_MNC_SEPARATOR, "51089" }, /* 3 Indonesia */
   #include <config_data/carrier_config_510_89.h>
   {MCC_MNC_SEPARATOR, "51502" }, /* Globe Philippines */
   #include <config_data/carrier_config_515_02.h>
   {MCC_MNC_SEPARATOR, "51503" },
   #include <config_data/carrier_config_515_03.h>
   {MCC_MNC_SEPARATOR, "51566" }, /* DITO Philippines */
   #include <config_data/carrier_config_515_66.h>
   {MCC_MNC_SEPARATOR, "52000" },
   #include <config_data/carrier_config_520_00.h>
   {MCC_MNC_SEPARATOR, "52001" },
   #include <config_data/carrier_config_520_01.h>
   {MCC_MNC_SEPARATOR, "52003" },/* AIS Thailand */
   #include <config_data/carrier_config_520_03.h>
   {MCC_MNC_SEPARATOR, "52004" },
   #include <config_data/carrier_config_520_04.h>
   {MCC_MNC_SEPARATOR, "52005" }, /* DTAC Thailand */
   #include <config_data/carrier_config_520_05.h>
   {MCC_MNC_SEPARATOR, "52017" },
   #include <config_data/carrier_config_520_17.h>
   {MCC_MNC_SEPARATOR, "52018" }, /* DTAC Thailand */
   #include <config_data/carrier_config_520_18.h>
   {MCC_MNC_SEPARATOR, "52047" }, /* DTAC Thailand */
   #include <config_data/carrier_config_520_47.h>
   {MCC_MNC_SEPARATOR, "52018" },
   #include <config_data/carrier_config_520_18.h>
   {MCC_MNC_SEPARATOR, "52023" },
   #include <config_data/carrier_config_520_23.h>
   {MCC_MNC_SEPARATOR, "52501" },
   #include <config_data/carrier_config_525_01.h>
   {MCC_MNC_SEPARATOR, "52502" },
   #include <config_data/carrier_config_525_02.h>
   {MCC_MNC_SEPARATOR, "52503" },
   #include <config_data/carrier_config_525_03.h>
   {MCC_MNC_SEPARATOR, "52505" },
   #include <config_data/carrier_config_525_05.h>
   {MCC_MNC_SEPARATOR, "52506" },
   #include <config_data/carrier_config_525_06.h>
   {MCC_MNC_SEPARATOR, "52507" },
   #include <config_data/carrier_config_525_07.h>
   {MCC_MNC_SEPARATOR, "52510" },
   #include <config_data/carrier_config_525_10.h>
   {MCC_MNC_SEPARATOR, "52801" }, /*imagine Brunei*/
   #include <config_data/carrier_config_528_01.h>
   {MCC_MNC_SEPARATOR, "52802" }, /*Progresif Brunei*/
   #include <config_data/carrier_config_528_02.h>
   {MCC_MNC_SEPARATOR, "52803" }, /*UNN Brunei*/
   #include <config_data/carrier_config_528_03.h>
   {MCC_MNC_SEPARATOR, "52811" }, /*DST Brunei*/
   #include <config_data/carrier_config_528_11.h>
   {MCC_MNC_SEPARATOR, "53001" },
   #include <config_data/carrier_config_530_01.h>
   {MCC_MNC_SEPARATOR, "53004" },
   #include <config_data/carrier_config_530_04.h>
   {MCC_MNC_SEPARATOR, "53005" },
   #include <config_data/carrier_config_530_05.h>
   {MCC_MNC_SEPARATOR, "53024" },
   #include <config_data/carrier_config_530_24.h>
   {MCC_MNC_SEPARATOR, "53099" },
   #include <config_data/carrier_config_530_99.h>
   {MCC_MNC_SEPARATOR, "53704" }, /*Vodafone Papus New Guinea*/
   #include <config_data/carrier_config_537_04.h>
   {MCC_MNC_SEPARATOR, "54201" }, /*Vodafone Fiji*/
   #include <config_data/carrier_config_542_01.h>
   {MCC_MNC_SEPARATOR, "54705" }, /*VITI French Polynesia*/
   #include <config_data/carrier_config_547_05.h>
   {MCC_MNC_SEPARATOR, "55299" }, /*PMCI Palau*/
   #include <config_data/carrier_config_552_99.h>
   {MCC_MNC_SEPARATOR, "60201" },
   #include <config_data/carrier_config_602_01.h>
   {MCC_MNC_SEPARATOR, "60202" },
   #include <config_data/carrier_config_602_02.h>
   {MCC_MNC_SEPARATOR, "60203" },
   #include <config_data/carrier_config_602_03.h>
   {MCC_MNC_SEPARATOR, "60204" },
   #include <config_data/carrier_config_602_04.h>
   {MCC_MNC_SEPARATOR, "60210" }, /*MOD Egypt*/
   #include <config_data/carrier_config_602_10.h>
   {MCC_MNC_SEPARATOR, "60211" }, /*NPSN Egypt*/
   #include <config_data/carrier_config_602_11.h>
   {MCC_MNC_SEPARATOR, "60303" }, /*Ooredoo Algeria*/
   #include <config_data/carrier_config_603_03.h>
   {MCC_MNC_SEPARATOR, "60400" }, /*Orange Morocco*/
   #include <config_data/carrier_config_604_00.h>
   {MCC_MNC_SEPARATOR, "60401" }, /*Maroc Morocco*/
   #include <config_data/carrier_config_604_01.h>
   {MCC_MNC_SEPARATOR, "60402" }, /*INWI Morocco*/
   #include <config_data/carrier_config_604_02.h>
   {MCC_MNC_SEPARATOR, "60501" }, /*Orange Tunisia*/
   #include <config_data/carrier_config_605_01.h>
   {MCC_MNC_SEPARATOR, "60502" }, /*Tunisie Telecom Tunisia*/
   #include <config_data/carrier_config_605_02.h>
   {MCC_MNC_SEPARATOR, "60503" }, /*Ooredoo Tunisia*/
   #include <config_data/carrier_config_605_03.h>
   {MCC_MNC_SEPARATOR, "60603" }, /*LTT Libya*/
   #include <config_data/carrier_config_606_03.h>
   {MCC_MNC_SEPARATOR, "60801" }, /*Orange Senegal*/
   #include <config_data/carrier_config_608_01.h>
   {MCC_MNC_SEPARATOR, "61002" }, /*Orange Mali*/
   #include <config_data/carrier_config_610_02.h>
   {MCC_MNC_SEPARATOR, "61203" }, /*Orange Cote dlvoire*/
   #include <config_data/carrier_config_612_03.h>
   {MCC_MNC_SEPARATOR, "61701" }, /*my.t Mauritius*/
   #include <config_data/carrier_config_617_01.h>
   {MCC_MNC_SEPARATOR, "61710" }, /*Emtel Mauritius*/
   #include <config_data/carrier_config_617_10.h>
   {MCC_MNC_SEPARATOR, "61820" }, /*LTC MOBILE Liberia*/
   #include <config_data/carrier_config_618_20.h>
   {MCC_MNC_SEPARATOR, "62001" }, /*MTN Ghana*/
   #include <config_data/carrier_config_620_01.h>
   {MCC_MNC_SEPARATOR, "62120" }, /*Airtel Nigeria*/
   #include <config_data/carrier_config_621_20.h>
   {MCC_MNC_SEPARATOR, "62124" }, /*Airtel Spectranet*/
   #include <config_data/carrier_config_621_24.h>
   {MCC_MNC_SEPARATOR, "62127" },
   #include <config_data/carrier_config_621_27.h>
   {MCC_MNC_SEPARATOR, "62130" }, /*MTN Nigeria*/
   #include <config_data/carrier_config_621_30.h>
   {MCC_MNC_SEPARATOR, "62140" },
   #include <config_data/carrier_config_621_40.h>
   {MCC_MNC_SEPARATOR, "62160" }, /*9mobile Nigeria*/
   #include <config_data/carrier_config_621_60.h>
   {MCC_MNC_SEPARATOR, "62170" }, /*Mcom Nigeria*/
   #include <config_data/carrier_config_621_70.h>
   {MCC_MNC_SEPARATOR, "63001" }, /*Vodafone Democratic Republic of the Congo*/
   #include <config_data/carrier_config_630_01.h>
   {MCC_MNC_SEPARATOR, "63002" }, /*Airtel Democratic Republic of the Congo*/
   #include <config_data/carrier_config_630_02.h>
   {MCC_MNC_SEPARATOR, "63102" }, /*Unitel Angola*/
   #include <config_data/carrier_config_631_02.h>
   {MCC_MNC_SEPARATOR, "63517" }, /*KTRN Rwanda*/
   #include <config_data/carrier_config_635_17.h>
   {MCC_MNC_SEPARATOR, "63601" }, /*Ethio Telecom Ethiopia*/
   #include <config_data/carrier_config_636_01.h>
   {MCC_MNC_SEPARATOR, "63602" }, /*Safaricom Ethiopia*/
   #include <config_data/carrier_config_636_02.h>
   {MCC_MNC_SEPARATOR, "639010" },
   #include <config_data/carrier_config_639_010.h>
   {MCC_MNC_SEPARATOR, "63902" },  /*Safaricom*/
   #include <config_data/carrier_config_639_02.h>
   {MCC_MNC_SEPARATOR, "63903" },  /*Airtel Kenya*/
   #include <config_data/carrier_config_639_03.h>
   {MCC_MNC_SEPARATOR, "63907" },
   #include <config_data/carrier_config_639_07.h>
   {MCC_MNC_SEPARATOR, "63910" },
   #include <config_data/carrier_config_639_10.h>
   {MCC_MNC_SEPARATOR, "63911" },  /*Jambotel Kenya*/
   #include <config_data/carrier_config_639_11.h>
   {MCC_MNC_SEPARATOR, "64004" },   /*Vodacom Tanzania*/
   #include <config_data/carrier_config_640_04.h>
   {MCC_MNC_SEPARATOR, "64005" },   /*Airtel Tanzania*/
   #include <config_data/carrier_config_640_05.h>
   {MCC_MNC_SEPARATOR, "64011" },
   #include <config_data/carrier_config_640_11.h>
   {MCC_MNC_SEPARATOR, "64101" },   /*Airtel Uganda*/
   #include <config_data/carrier_config_641_01.h>
   {MCC_MNC_SEPARATOR, "64110" },   /*MTN Uganda*/
   #include <config_data/carrier_config_641_10.h>
   {MCC_MNC_SEPARATOR, "64122" },   /*Airtel Uganda*/
   #include <config_data/carrier_config_641_22.h>
   {MCC_MNC_SEPARATOR, "64133" },
   #include <config_data/carrier_config_641_33.h>
   {MCC_MNC_SEPARATOR, "64304" },
   #include <config_data/carrier_config_643_04.h>
   {MCC_MNC_SEPARATOR, "64501" },   /*Airtel Zambia*/
   #include <config_data/carrier_config_645_01.h>
   {MCC_MNC_SEPARATOR, "64502" },   /*MTN Zambia*/
   #include <config_data/carrier_config_645_02.h>
   {MCC_MNC_SEPARATOR, "64505" },
   #include <config_data/carrier_config_645_05.h>
   {MCC_MNC_SEPARATOR, "64604" },   /*Telma Madagascar*/
   #include <config_data/carrier_config_646_04.h>
   {MCC_MNC_SEPARATOR, "64700" }, /*Orange Reunion Mayotte*/
   #include <config_data/carrier_config_647_00.h>
   {MCC_MNC_SEPARATOR, "64704" }, /*Zeop French Reunion*/
   #include <config_data/carrier_config_647_04.h>
   {MCC_MNC_SEPARATOR, "64710" },
   #include <config_data/carrier_config_647_10.h>
   {MCC_MNC_SEPARATOR, "65101" }, /*Vodafone Lesotho*/
   #include <config_data/carrier_config_651_01.h>
   {MCC_MNC_SEPARATOR, "65201" }, /*Mascom Botswana*/
   #include <config_data/carrier_config_652_01.h>
   {MCC_MNC_SEPARATOR, "65302" },
   #include <config_data/carrier_config_653_02.h>
   {MCC_MNC_SEPARATOR, "65501" },
   #include <config_data/carrier_config_655_01.h>
   {MCC_MNC_SEPARATOR, "65502" },
   #include <config_data/carrier_config_655_02.h>
   {MCC_MNC_SEPARATOR, "65507" },
   #include <config_data/carrier_config_655_07.h>
   {MCC_MNC_SEPARATOR, "65510" },
   #include <config_data/carrier_config_655_10.h>
   {MCC_MNC_SEPARATOR, "65519" },
   #include <config_data/carrier_config_655_19.h>
   {MCC_MNC_SEPARATOR, "65538" },
   #include <config_data/carrier_config_655_38.h>
   {MCC_MNC_SEPARATOR, "65573" },
   #include <config_data/carrier_config_655_73.h>
   {MCC_MNC_SEPARATOR, "65574" },
   #include <config_data/carrier_config_655_74.h>
   {MCC_MNC_SEPARATOR, "70401" },
   #include <config_data/carrier_config_704_01.h>
   {MCC_MNC_SEPARATOR, "70402" },
   #include <config_data/carrier_config_704_02.h>
   {MCC_MNC_SEPARATOR, "70601" },
   #include <config_data/carrier_config_706_01.h>
   {MCC_MNC_SEPARATOR, "708001" },
   #include <config_data/carrier_config_708_001.h>
   {MCC_MNC_SEPARATOR, "708002" }, /* Tigo Honduras */
   #include <config_data/carrier_config_708_002.h>
   {MCC_MNC_SEPARATOR, "70802" }, /* Tigo Honduras */
   #include <config_data/carrier_config_708_02.h>
   {MCC_MNC_SEPARATOR, "71021" },
   #include <config_data/carrier_config_710_21.h>
   {MCC_MNC_SEPARATOR, "710300" }, /* Tigo Nicaragua */
   #include <config_data/carrier_config_710_300.h>
   {MCC_MNC_SEPARATOR, "71201" },
   #include <config_data/carrier_config_712_01.h>
   {MCC_MNC_SEPARATOR, "71203" },
   #include <config_data/carrier_config_712_03.h>
   {MCC_MNC_SEPARATOR, "71204" }, /* Liberty Costa Rica */
   #include <config_data/carrier_config_712_04.h>
   {MCC_MNC_SEPARATOR, "71401" },
   #include <config_data/carrier_config_714_01.h>
   {MCC_MNC_SEPARATOR, "71402" }, /* Tigo Panama */
   #include <config_data/carrier_config_714_02.h>
   {MCC_MNC_SEPARATOR, "71403" },
   #include <config_data/carrier_config_714_03.h>
   {MCC_MNC_SEPARATOR, "714010" },
   #include <config_data/carrier_config_714_010.h>
   {MCC_MNC_SEPARATOR, "714020" }, /* Tigo Panama */
   #include <config_data/carrier_config_714_020.h>
   {MCC_MNC_SEPARATOR, "716006" },
   #include <config_data/carrier_config_716_006.h>
   {MCC_MNC_SEPARATOR, "71606" },
   #include <config_data/carrier_config_716_06.h>
   {MCC_MNC_SEPARATOR, "71610" },
   #include <config_data/carrier_config_716_10.h>
   {MCC_MNC_SEPARATOR, "71615" }, /* Bitel Peru */
   #include <config_data/carrier_config_716_15.h>
   {MCC_MNC_SEPARATOR, "71617" },
   #include <config_data/carrier_config_716_17.h>
   {MCC_MNC_SEPARATOR, "72207" },
   #include <config_data/carrier_config_722_07.h>
   {MCC_MNC_SEPARATOR, "722310" },
   #include <config_data/carrier_config_722_310.h>
   {MCC_MNC_SEPARATOR, "722320" },
   #include <config_data/carrier_config_722_320.h>
   {MCC_MNC_SEPARATOR, "722330" },
   #include <config_data/carrier_config_722_330.h>
   {MCC_MNC_SEPARATOR, "72234" },
   #include <config_data/carrier_config_722_34.h>
   {MCC_MNC_SEPARATOR, "72402" },
   #include <config_data/carrier_config_724_02.h>
   {MCC_MNC_SEPARATOR, "72403" },
   #include <config_data/carrier_config_724_03.h>
   {MCC_MNC_SEPARATOR, "72404" },
   #include <config_data/carrier_config_724_04.h>
   {MCC_MNC_SEPARATOR, "72405" },
   #include <config_data/carrier_config_724_05.h>
   {MCC_MNC_SEPARATOR, "72406" },
   #include <config_data/carrier_config_724_06.h>
   {MCC_MNC_SEPARATOR, "72410" },
   #include <config_data/carrier_config_724_10.h>
   {MCC_MNC_SEPARATOR, "72411" },
   #include <config_data/carrier_config_724_11.h>
   {MCC_MNC_SEPARATOR, "72417" }, /* Surf Brazil */
   #include <config_data/carrier_config_724_17.h>
   {MCC_MNC_SEPARATOR, "72423" },
   #include <config_data/carrier_config_724_23.h>
   {MCC_MNC_SEPARATOR, "72426" }, /* america.net Brazil */
   #include <config_data/carrier_config_724_26.h>
   {MCC_MNC_SEPARATOR, "72429" }, /* Unifique Brazil */
   #include <config_data/carrier_config_724_29.h>
   {MCC_MNC_SEPARATOR, "72432" }, /* Algar Telecom Brazil */
   #include <config_data/carrier_config_724_32.h>
   {MCC_MNC_SEPARATOR, "72433" }, /* Algar Telecom Brazil */
   #include <config_data/carrier_config_724_33.h>
   {MCC_MNC_SEPARATOR, "72434" }, /* Algar Telecom Brazil */
   #include <config_data/carrier_config_724_34.h>
   {MCC_MNC_SEPARATOR, "72454" },
   #include <config_data/carrier_config_724_54.h>
   {MCC_MNC_SEPARATOR, "72477" }, /* brisanet Brazil */
   #include <config_data/carrier_config_724_77.h>
   {MCC_MNC_SEPARATOR, "72480" },
   #include <config_data/carrier_config_724_80.h>
   {MCC_MNC_SEPARATOR, "730003" },
   #include <config_data/carrier_config_730_003.h>
   {MCC_MNC_SEPARATOR, "73001" },
   #include <config_data/carrier_config_730_01.h>
   {MCC_MNC_SEPARATOR, "73002" },
   #include <config_data/carrier_config_730_02.h>
   {MCC_MNC_SEPARATOR, "73003" },
   #include <config_data/carrier_config_730_03.h>
   {MCC_MNC_SEPARATOR, "73007" },
   #include <config_data/carrier_config_730_07.h>
   {MCC_MNC_SEPARATOR, "73009" },
   #include <config_data/carrier_config_730_09.h>
   {MCC_MNC_SEPARATOR, "730010" },
   #include <config_data/carrier_config_730_010.h>
   {MCC_MNC_SEPARATOR, "73010" },
   #include <config_data/carrier_config_730_10.h>
   {MCC_MNC_SEPARATOR, "732101" },
   #include <config_data/carrier_config_732_101.h>
   {MCC_MNC_SEPARATOR, "732103" },
   #include <config_data/carrier_config_732_103.h>
   {MCC_MNC_SEPARATOR, "732111" },
   #include <config_data/carrier_config_732_111.h>
   {MCC_MNC_SEPARATOR, "732123" },
   #include <config_data/carrier_config_732_123.h>
   {MCC_MNC_SEPARATOR, "732130" },
   #include <config_data/carrier_config_732_130.h>
   {MCC_MNC_SEPARATOR, "732360" },
   #include <config_data/carrier_config_732_360.h>
   {MCC_MNC_SEPARATOR, "73402" }, /* Digitel Venezuela */
   #include <config_data/carrier_config_734_02.h>
   {MCC_MNC_SEPARATOR, "73404" },
   #include <config_data/carrier_config_734_04.h>
   {MCC_MNC_SEPARATOR, "73602" },
   #include <config_data/carrier_config_736_02.h>
   {MCC_MNC_SEPARATOR, "73603" }, /* Tigo Bolivia */
   #include <config_data/carrier_config_736_03.h>
   {MCC_MNC_SEPARATOR, "73800" }, /* E-Networks Guyana */
   #include <config_data/carrier_config_738_00.h>
   {MCC_MNC_SEPARATOR, "738040" }, /* E-Networks Guyana */
   #include <config_data/carrier_config_738_040.h>
   {MCC_MNC_SEPARATOR, "73840" }, /* E-Networks Guyana */
   #include <config_data/carrier_config_738_40.h>
   {MCC_MNC_SEPARATOR, "74000" },
   #include <config_data/carrier_config_740_00.h>
   {MCC_MNC_SEPARATOR, "74001" },
   #include <config_data/carrier_config_740_01.h>
   {MCC_MNC_SEPARATOR, "74002" },
   #include <config_data/carrier_config_740_02.h>
   {MCC_MNC_SEPARATOR, "74402" },
   #include <config_data/carrier_config_744_02.h>
   {MCC_MNC_SEPARATOR, "74405" }, /* Personal Paraguay */
   #include <config_data/carrier_config_744_05.h>
   {MCC_MNC_SEPARATOR, "74801" }, /* Antel/Uruguay */
   #include <config_data/carrier_config_748_01.h>
   {MCC_MNC_SEPARATOR, "74807" },
   #include <config_data/carrier_config_748_07.h>
   {MCC_MNC_SEPARATOR, "74810" },
   #include <config_data/carrier_config_748_10.h>
   {MCC_MNC_SEPARATOR, "748010" },
   #include <config_data/carrier_config_748_010.h>
   {MCC_MNC_SEPARATOR, "90128" },
   #include <config_data/carrier_config_901_28.h>

   /* End indication */
   { MCC_MNC_END_TAG, NULL },
};

/**
  * Return the start index of CarrierConfigValue array for specific MCC/MNC
  * @param mccmnc  mobile country code and mobile network code string
  * @retrun the start index if found else return -1
  */
int getStartIndex(const char* mccmnc) {
    int i = 0, found = 0;

    while (carierConfigData[i].key != MCC_MNC_END_TAG) {
        if ((carierConfigData[i].key == MCC_MNC_SEPARATOR) &&
             (!strcmp(carierConfigData[i].value, mccmnc))) {
            found = 1;
            break;
        }
        i++;
    }

    return (found == 1) ? i : -1;
}


/**
 * Retrieve the count of keys for specific MCC/MNC
 * @param mccmnc  mobile country code and mobile network code string
 * @retrun the count of keys
 */
unsigned int getKeyCount(const char* mccmnc) {
    int i = 0, count = 0;

    i = getStartIndex(mccmnc);

    if (i >= 0) {
        while ((carierConfigData[++i].key != MCC_MNC_SEPARATOR) &&
                     (carierConfigData[i].key != MCC_MNC_END_TAG)) {
            count++;
        }
    }

    return count;
}

/**
 * Retrieve all the values of corresponding MCC/MNC
 * @param mccmnc  mobile country code and mobile network code string
 * @retrun the array of values and the count of values if MCC/MNC can be found, else return 0
 */
int getValuesByMccMnc(const char* mccmnc, CarrierConfigValue *data) {
    int i = 0, count = 0, len;

    i = getStartIndex(mccmnc);
    if (i >= 0) {
        while ((carierConfigData[++i].key != MCC_MNC_SEPARATOR) &&
                     (carierConfigData[i].key != MCC_MNC_END_TAG)) {
            len = strlen(carierConfigData[i].value);
            data[count].key = carierConfigData[i].key;
            data[count].value = (char*)calloc(len + 1, sizeof(char));
            strncpy(data[count].value, carierConfigData[i].value, len);
            count++;
        }
    }

    return count;
}

