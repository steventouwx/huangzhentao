/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2016. All rights reserved.
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
#include "PhonebookBitArray.h"

const unsigned int bitValue[32] = {
    0x80000000,
    0x40000000,
    0x20000000,
    0x10000000,
    0x08000000,
    0x04000000,
    0x02000000,
    0x01000000,
    0x00800000,
    0x00400000,
    0x00200000,
    0x00100000,
    0x00080000,
    0x00040000,
    0x00020000,
    0x00010000,
    0x00008000,
    0x00004000,
    0x00002000,
    0x00001000,
    0x00000800,
    0x00000400,
    0x00000200,
    0x00000100,
    0x00000080,
    0x00000040,
    0x00000020,
    0x00000010,
    0x00000008,
    0x00000004,
    0x00000002,
    0x00000001
};

const int bitLen =sizeof(int) * 8;

PhonebookBitArray::PhonebookBitArray(unsigned int length) {
    this->len = length;
    bit = new unsigned int[length/bitLen + (length % bitLen > 0 ? 1 : 0) + 1]();
}

PhonebookBitArray::~PhonebookBitArray() {
    delete[] bit;
}

unsigned int PhonebookBitArray::getBit(unsigned int index) {
    unsigned int data = bit[index / bitLen];
    return (data & bitValue[index % bitLen]) >> (bitLen - index % bitLen -1);
}

void PhonebookBitArray::setBit(int index, unsigned int value) {

    if (index <= 0 || index > len || (value != 0 && value != 1)) {
        return;
    }

    unsigned int data = bit[index / bitLen];
    if (value == 1) {
        bit[index / bitLen] = data | bitValue[index % bitLen];
    } else {
        bit[index / bitLen] = data &~bitValue[index % bitLen];
    }
}

unsigned int PhonebookBitArray::getLength() {
    return this->len;
}

unsigned int PhonebookBitArray::getFirstNotSetBitIndex() {
    unsigned  int i = 0;
    for (i = 1; i <= len; i++) {
        unsigned int value = getBit(i);
        if (value == 0) {
            return i;
        }
    }
    return i;
}
