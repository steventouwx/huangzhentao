/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2023. All rights reserved.
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

/**
 * Structure of dcmap parameters acquired by SDP negotiation
 */
@VintfStability
parcelable DcMapStruct {
    int    dcmap_stream_id; //rild uint16_t, indicates the SCTP stream identifier within the SCTP association used to form the data channel.dcmap-stream-id = 1*5DIGIT
    int    priority; //rild uint16_t, indicates the data channel's priority relative to the priorities of other data channels, which may additionally exist over the same SCTP association. The 'priority' parameter maps to the 'Priority' parameter defined in [RFC8832].
    byte   dc_config_index;  //Mapping to related DC config
    byte   ordered;  //The 'ordered' parameter with value "true" indicates that the receiver will dispatch DATA chunks in the data channel to the upper layer while preserving the order. The 'ordered' parameter is optional and takes two values -- "true" for ordered delivery and "false" for unordered delivery -- with "true" as the default value. This parameter maps to the ordered or unordered data channel types as defined in [RFC8832].
    int    max_retr;   //indicates that the data channel is partially reliable. The 'max-retr' parameter indicates the maximal number of times a user message will be retransmitted. This parameter maps to the 'Number of RTX' parameter defined in [RFC8832]
    int    max_time;   //indicates that the data channel is partially reliable. A user message will no longer be transmitted or retransmitted after a specified lifetime, given in milliseconds, in the 'max-time' parameter. This parameter maps to the 'Lifetime in ms' parameter defined in [RFC8832].
    String label;  //MAX_DCMAP_LABEL_LENGTH, The 'label' parameter indicates the name of the channel. It represents a label that can be used to distinguish, in the context of the WebRTC API. This parameter maps to the 'Label' parameter defined in [RFC8832] (Note: string length is a variable Length according to RFC 8832, we just give it a length with 128 and should be enough)
    String subprotocol; //MAX_DCMAP_SUBPROTOCOL_LENGTH, The 'subprotocol' parameter indicates which protocol the client expects to exchange via the channel. This parameter maps to the 'Protocol' parameter defined in [RFC8832] (Note: string length is a variable Length according to RFC 8832, we just give it a length with 32 and should be enough)
}