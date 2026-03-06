/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2022. All rights reserved.
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

package vendor.mediatek.hardware.mtkradioex.dch;

import vendor.mediatek.hardware.mtkradioex.dch.DcMapStruct;

/**
 * Structure of initial local data parameters (defined in RFC 8864)
 */
@VintfStability
parcelable DcConfigLocal {
    int    local_udp_port;  //rild uint16_t
    int    local_sctp_port; //rild uint16_t
    byte   index; //Index of the Config
    byte   dcmap_count; //The number of valid dcmap elements
    byte   local_dtls_setup; //Indicates the role when creating DTLS association, usually in SDP offer it’s "activepass" or "active", in SDP answer it’s "passive"
    String local_tls_id; //MAX_DC_TLS_ID_LENGTH, DTLS association ID, tls-id-value = 20*(tls-id-char)
    int    fp_hash_func; //rild uint16_t, Could be sha-256, sha-384, sha-512, normally we use sha-256 /* dc_fp_hash_func_enum */
    String local_fp_str; //MAX_DC_FINGERPRINT_LENGTH, "Secure Hash Standard (SHS)" (reference in RFC 8122) mentioned that "The message digests range in length from 160 to 512 bits, depending on the algorithm"
    int    local_max_message_size; //The maximum SCTP size peer could receive according to SDP negotiation
    int    data_b_as; //Bandwidth for this DC stream
    String qos_hint; //MAX_DC_QOS_HINT_LENGTH, TS 26.114 e.g.: loss=0.000002,latency=1000
    DcMapStruct[] dcmap; // max size MAX_DC_DCMAP_NUM
}
