// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <malloc.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <mtk_log.h>

#include "sms_pdu.h"

#if DEBUG
#define RLOGE(x...) printf(x);printf("\n")
#define RLOGD(x...) printf(x);printf("\n")
#define RLOGW(x...) printf(x);printf("\n")
#define RLOGE(x...) printf(x);printf("\n")
#endif

#undef LOG_TAG
#define LOG_TAG "DEMO_PDU"
void hexString_To_Bytes(char *in, int inLength, char *out);
struct timeb {
    time_t time;
    unsigned short millitm;
    short timezone;
    short dstflag;
};

static inline int ftime(struct timeb *tp) {
    const unsigned int ONE_MS_IN_NS = 100000;
    struct timespec ts;

    int err = clock_gettime(CLOCK_REALTIME, &ts);
    if (err)
        return -1;

    tp->time = ts.tv_sec;
    tp->millitm = ts.tv_nsec / ONE_MS_IN_NS;
    return 0;
}


/* RFC3629 chapter 4. Syntax of UTF-8 Byte Sequences */
static kal_int32 is_legal_utf8(const UTF8 *start, kal_int32 len)
{
    UTF8 tmp = 0;
    const UTF8 *ptr = start + len;

    switch (len) {
        default:
            return FALSE;

        case 4:
            tmp = *--ptr;
            if (tmp < 0x80 || tmp > 0xBF) {
                return FALSE;
            }

        case 3:
            tmp = *--ptr;
            if (tmp < 0x80 || tmp > 0xBF) {
                return FALSE;
            }

        case 2:
            tmp = *--ptr;
            if (tmp > 0xBF) {
                return FALSE;
            }

            switch (*start) {
                case 0xE0:
                    if (tmp < 0xA0) {
                        return FALSE;
                    }
                    break;

                case 0xED:
                    if (tmp > 0x9F) {
                        return FALSE;
                    }
                    break;

                case 0xF0:
                    if (tmp < 0x90) {
                        return FALSE;
                    }
                    break;

                case 0xF4:
                    if (tmp > 0x8F) {
                        return FALSE;
                    }
                    break;

                default:
                    if (tmp < 0x80) {
                        return FALSE;
                    }
                    break;
            }

        case 1:
            if (*start >= 0x80 && *start < 0xC2) {
                return FALSE;
            }
    }

    if (*start > 0xF4) {
        return FALSE;
    }

    return TRUE;
}

kal_int32 is_utf8_sequence(const UTF8 *start, const UTF8 *end)
{
    kal_int32 len = utf8_tailing_bytes[*start] + 1;

    if (start + len > end) {
        return FALSE;
    }

    return is_legal_utf8(start, len);
}

kal_int32 kal_utf8_to_utf16(UTF16 *dest,
                            const UTF8 *src,
                            kal_int32 size)
{
    kal_int32 result = STATUS_SUCCESS;
    const UTF8 *start = src;
    const UTF8 *end = src;
    UTF16 *target = dest;

    while(*end) {
        end++;
    }

    while (start < end) {
        unsigned long tmp = 0;
        unsigned int extra_bytes = utf8_tailing_bytes[*start];
        //UTF8 *next = start + extra_bytes + 1;

        if (target && target >= dest + size) {
            result = STATUS_MEM_EXHAUSTED;
            break;
        }

        if (start + extra_bytes >= end) {
            result = STATUS_MEM_EXHAUSTED;
            break;
        }

        if (!is_legal_utf8(start, extra_bytes + 1)) {
            result = STATUS_ILLEGAL_UTF8;
            break;
        }

        switch (extra_bytes) {
            case 5:
                tmp += *start++;
                tmp <<= 6; /* illegal UTF8 */
            case 4:
                tmp += *start++;
                tmp <<= 6; /* illegal UTF8 */
            case 3:
                tmp += *start++;
                tmp <<= 6;
            case 2:
                tmp += *start++;
                tmp <<= 6;
            case 1:
                tmp += *start++;
                tmp <<= 6;
            case 0:
                tmp += *start++;
        }

        tmp -= utf8_offsets[extra_bytes];

        if (tmp <= MAX_UNI_BMP) {
            if (tmp >= SUR_HIGH_START && tmp <= SUR_LOW_END) {
                result = STATUS_ILLEGAL_UTF8;
                break;
            } else {
                if (target) {
                    *target++ = (UTF16)tmp;
                }

                result++;
            }
        } else if (tmp > MAX_UTF16) {
            result = STATUS_ILLEGAL_UTF8;
            break;
        } else {
            if (target && target + 1 >= dest + size) {
                result = STATUS_MEM_EXHAUSTED;
                break;
            }

            tmp -= UTF16_HALF_BASE;

            if (target) {
                *target++ = (UTF16)((tmp >> UTF16_HALF_SHIFT) + SUR_HIGH_START);
                *target++ = (UTF16)((tmp & UTF16_HALF_MASK) + SUR_LOW_START);
            }

            result += 2;
        }
    }

    if (target) {
        *target = 0;
    }

    return result;
}

kal_int32 kal_utf8_to_utf32(UTF32 *dest, const UTF8 *src, kal_int32 size)
{
    kal_int32 result = STATUS_SUCCESS;
    const UTF8 *start = src;
    const UTF8 *end = src;
    UTF32 *target = dest;

    if (!src) { return STATUS_NULL_POINTER; }

    while(*end) {
        end++;
    }

    while (start < end) {
        unsigned long tmp = 0;
        unsigned int extra_bytes = utf8_tailing_bytes[*start];
        //UTF8 *next = start + extra_bytes + 1;

        if (target && target >= dest + size) {
            result = STATUS_MEM_EXHAUSTED;
            break;
        }

        if (start + extra_bytes >= end) {
            result = STATUS_MEM_EXHAUSTED;
            break;
        }

        if (!is_legal_utf8(start, extra_bytes + 1)) {
            result = STATUS_ILLEGAL_UTF8;
            break;
        }

        switch (extra_bytes) {
            case 5:
                tmp += *start++;
                tmp <<= 6; /* illegal UTF8 */
            case 4:
                tmp += *start++;
                tmp <<= 6; /* illegal UTF8 */
            case 3:
                tmp += *start++;
                tmp <<= 6;
            case 2:
                tmp += *start++;
                tmp <<= 6;
            case 1:
                tmp += *start++;
                tmp <<= 6;
            case 0:
                tmp += *start++;
        }

        tmp -= utf8_offsets[extra_bytes];

        if (tmp <= MAX_LEGAL_UTF32) {
            if (tmp >= SUR_HIGH_START && tmp <= SUR_LOW_END) {
                result = STATUS_ILLEGAL_UTF8;
                break;
            } else {
                if (target) {
                    *target++ = tmp;
                }
                result++;
            }
        } else {
            result = STATUS_ILLEGAL_UTF8;
            break;
        }
    }

    if (target) {
        *target = 0;
    }

    return result;
}

kal_int32 kal_utf8_to_ucs2(UCS2 *dest, const UTF8 *src, kal_int32 size)
{
    kal_int32 result = STATUS_SUCCESS;
    const UTF8 *start = src;
    const UTF8 *end = src;
    UTF16 *target = dest;

    if (!src) { return STATUS_NULL_POINTER; }
    while(*end) {
        end++;
    }

    while (start < end) {
        unsigned long tmp = 0;
        unsigned int extra_bytes = utf8_tailing_bytes[*start];
        //UTF8 *next = start + extra_bytes + 1;

        if (target && target > dest + size) {
            result = STATUS_MEM_EXHAUSTED;
            break;
        }

        if (start + extra_bytes >= end) {
            result = STATUS_MEM_EXHAUSTED;
            break;
        }

        if (!is_legal_utf8(start, extra_bytes + 1)) {
            result = STATUS_ILLEGAL_UTF8;
            break;
        }

        switch (extra_bytes) {
            case 5:
                tmp += *start++;
                tmp <<= 6; /* illegal UTF8 */
            case 4:
                tmp += *start++;
                tmp <<= 6; /* illegal UTF8 */
            case 3:
                tmp += *start++;
                tmp <<= 6;
            case 2:
                tmp += *start++;
                tmp <<= 6;
            case 1:
                tmp += *start++;
                tmp <<= 6;
            case 0:
                tmp += *start++;
        }
        tmp -= utf8_offsets[extra_bytes];

        if (tmp <= MAX_UNI_BMP) {
            if (tmp >= SUR_HIGH_START && tmp <= SUR_LOW_END) {
                result = STATUS_ILLEGAL_UTF8;
                break;
            } else {
                if (target) {
                    *target++ = (UTF16)tmp;
                }
                result++;
            }
        } else {
            result = STATUS_ILLEGAL_UTF8;
            break;
        }
    }

    if (target) {
        *target = 0;
    }

    return result;
}

kal_int32 kal_utf16_to_utf8(UTF8 *dest, const UTF16 *src, kal_int32 size)
{
    kal_int32 result = STATUS_SUCCESS;
    const UTF16 *start = src;
    const UTF16 *end = src;
    const UTF32 TAIL_MASK = 0xBF;
    const UTF32 TAIL_MARK = 0x80;

    UTF8 *target = dest;

    if (!src) { return STATUS_NULL_POINTER; }
    while (*end) {
        end++;
    }

    while (start < end) {
        UTF32 tmp = 0;
        unsigned long bytes = 0;

        tmp = *start++;
        //RLOGD("tmp = [%X]\n", tmp);
        /* */
        if (tmp >= SUR_HIGH_START && tmp <= SUR_HIGH_END) {
            if (start < end) {
                UTF32 tmp2 = *start;
                if (tmp2 >= SUR_LOW_START && tmp2 <= SUR_LOW_END) {
                    tmp = ((tmp - SUR_HIGH_START) << UTF16_HALF_SHIFT)
                        + (tmp2 - SUR_LOW_START) + UTF16_HALF_BASE;
                    start++;
                } else {
                    result = STATUS_ILLEGAL_UTF16;
                    break;
                }
            } else {
                result = STATUS_MEM_EXHAUSTED;
                break;
            }
        } else if (tmp >= SUR_LOW_START && tmp <= SUR_LOW_END) {
            result = STATUS_ILLEGAL_UTF16;
            break;
        }

        if (tmp < (UTF32)0x80) {
            bytes = 1;
        } else if (tmp < (UTF32)0x800) {
            bytes = 2;
        } else if (tmp < (UTF32)0x10000) {
            bytes = 3;
        } else if (tmp < (UTF32)0x110000) {
            bytes = 4;
        } else {
            result = STATUS_ILLEGAL_UTF16;
            break;
        }

        if (target && size > 0) {
            size -= bytes;
            if (0 >= size) {
                break;
            }

            switch (bytes) {
                case 4:
                    *(target + 3) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 3:
                    *(target + 2) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 2:
                    *(target + 1) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 1:
                    *target = (UTF8)(tmp | first_byte_mark[bytes]);
            }
            target += bytes;
        }
        result += bytes;
    }

    if (target)
        *target = 0;

    return result;
}

kal_int32 kal_utf32_to_utf8(UTF8 *dest, const UTF32 *src, kal_int32 size)
{
    kal_int32 result = STATUS_SUCCESS;
    const UTF32 *start = src, *end = src;
    UTF8 *target = dest;
    const UTF32 TAIL_MASK = 0xBF;
    const UTF32 TAIL_MARK = 0x80;

    if (!src) { return STATUS_NULL_POINTER; }
    while (*end) {
        end++;
    }

    while (start < end) {
        UTF32 tmp = 0;
        unsigned long bytes = 0;

        tmp = *start++;
        /* UTF16 surrogate values are illegal in UTF32*/
        if (tmp >= SUR_HIGH_START && tmp <= SUR_LOW_END) {
            result = STATUS_ILLEGAL_UTF32;
            break;
        }

        if (tmp < (UTF32)0x80) {
            bytes = 1;
        } else if (tmp < (UTF32)0x800) {
            bytes = 2;
        } else if (tmp < (UTF32)0x10000) {
            bytes = 3;
        } else if (tmp < (UTF32)0x110000) {
            bytes = 4;
        } else {
            result = STATUS_ILLEGAL_UTF16;
            break;
        }

        if (target && size) {
            size -= bytes;
            if (size <= 0) {
                break;
            }

            switch (bytes) {
                case 4:
                    *(target + 3) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 3:
                    *(target + 2) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 2:
                    *(target + 1) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 1:
                    *target = (UTF8)(tmp | first_byte_mark[bytes]);
            }
            target += bytes;
        }
        result += bytes;
    }

    if (target) {
        *target = 0;
    }

    return result;
}

kal_int32 kal_ucs2_to_utf8(UTF8 *dest, const UCS2 *src, kal_int32 size)
{
    kal_int32 result = STATUS_SUCCESS;
    const UCS2 *start = src, *end = src;
    const UTF32 TAIL_MASK = 0xBF;
    const UTF32 TAIL_MARK = 0x80;
    UTF8 *target = dest;

    if (!src) { return STATUS_NULL_POINTER; }
    while (*end) {
        end++;
    }

    while (start < end) {
        UTF32 tmp = 0;
        unsigned long bytes = 0;

        tmp = *start++;
        /* */
        if (tmp >= SUR_HIGH_START && tmp <= SUR_LOW_END) {
            result = STATUS_ILLEGAL_UCS2;
            break;
        }

        if (tmp < (UTF32)0x80) {
            bytes = 1;
        } else if (tmp < (UTF32)0x800) {
            bytes = 2;
        } else if (tmp < (UTF32)0x10000) {
            bytes = 3;
        } else if (tmp < (UTF32)0x110000) {
            bytes = 4;
        } else {
            result = STATUS_ILLEGAL_UCS2;
            break;
        }

        if (target && size > 0) {
            size -= bytes;
            if (size <= 0) {
                break;
            }

            switch (bytes) {
                case 4:
                    *(target + 3) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 3:
                    *(target + 2) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 2:
                    *(target + 1) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 1:
                    *target = (UTF8)(tmp | first_byte_mark[bytes]);
            }
            target += bytes;
        }
        result += bytes;
    }

    if (target) {
        *target = 0;
    }

    return result;
}

kal_int32 kal_ext_ascii_to_utf8(UTF8 *dest, const kal_uint8 *src, kal_int32 size)
{
    kal_int32 result = STATUS_SUCCESS;
    const kal_uint8 *start = src, *end = src;
    const UTF32 TAIL_MASK = 0xBF;
    const UTF32 TAIL_MARK = 0x80;
    UTF8 *target = dest;

    if (!src) { return STATUS_NULL_POINTER; }
    while (*end) {
        end++;
    }

    while (start < end) {
        UTF32 tmp = 0;
        unsigned long bytes = 0;

        tmp = *start++;

        if (tmp < (UTF32)0x80) {
            bytes = 1;
        } else {
            bytes = 2;
        }

        if (target && size > 0) {
            size -= bytes;
            if (size <= 0) {
                break;
            }

            switch (bytes) {
                case 2:
                    *(target + 1) = (UTF8)((tmp | TAIL_MARK) & TAIL_MASK);
                    tmp >>= 6;
                case 1:
                    *target = (UTF8)(tmp | first_byte_mark[bytes]);
            }
            target += bytes;
        }
        result += bytes;
    }

    if (target) {
        *target = 0;
    }

    return result;
}

static kal_int32 _mdapi_sms_set_timestamp(mdapi_sms_record_t *sms)
{
    struct timeb tp;
    struct tm *p;
    kal_int32 y = 0;

    // Initialize the timeb structure using ftime
    if (ftime(&tp) != 0) {
        return MDAPI_RET_ERROR; // Return error if ftime fails
    }

    sms->time[6] = tp.timezone;

    p = localtime(&tp.time);
    if (p == NULL) {
        return MDAPI_RET_ERROR;
    }

    y = 1900 + p->tm_year;
    sms->time[0] = y % 100;
    sms->time[1] = 1 + p->tm_mon;
    sms->time[2] = p->tm_mday;
    sms->time[3] = p->tm_hour;
    sms->time[4] = p->tm_min;
    sms->time[5] = p->tm_sec;

    return MDAPI_RET_SUCCESS;
}

static unsigned char
internal_mdapi_hex2int(char *s) {
    int ret = 0;
    int len = 2, i = 0;
    while (i < len) {
        if (s[i] == 0) {
            return -1;
        } else if (s[i] >= 'a' && s[i] <= 'f') {
            ret = (s[i] - 'a' + 10) + (ret << 4);
        } else if (s[i] >= 'A' && s[i] <= 'F') {
            ret = (s[i] - 'A' + 10) + (ret << 4);
        } else if (s[i] >= '0' && s[i] <= '9') {
            ret = (s[i] - '0') + (ret << 4);
        } else {
            return -1;
        }
        i++;
    }
    return ret;
}

kal_int32 _mdapi_sms_7bit_encode(kal_int8 *mesg, kal_int32 len, kal_int8 *output, kal_int32 size)
{
    kal_int32 i = 0;
    kal_int8 *ptr = mesg;
    kal_int32 out_len = 0;
    kal_uint8 curbyte = 0;
    kal_uint8 curbit = 0;
    kal_uint8 c = 0;

    output[0] = '\0';
    while(*ptr && len) {
        c = latin1_to_gsm_table[(kal_uint8)*ptr++];
        len--;
        for (i = 0; i < 7; i++) {
            if(((1 << i) & c) != 0) {
                curbyte |= ((char)1 << curbit);
            }

            curbit++;
            if(curbit == 8) {
                curbit = 0;
                out_len += snprintf(output + out_len, size - out_len - 1, "%02X", curbyte);
                curbyte = 0;
            }
        }
    }

    if (curbit > 0) {
        out_len += snprintf(output + out_len, size - out_len - 1, "%02X", curbyte);
    }

    return out_len;
}

int internal_mdapi_sms_7bit_decode(char * message, char * output, int output_size, int padding_bits, int octect)
{
    int i = 0, len = 0;
    char * ptr = message;
    int output_len = 0;
    int cur_val = 0;
    int val = 0;
    int last_val = 0;//used to save last 1 octet
    int offset;

    if( padding_bits < 0||padding_bits > 6 )
    {
        return 0;
    }
    //Calc how many octets are there
    len = strlen(message) >> 1;

    for(i = 0; i < len - 1; i++){
        offset = i % 0x7;
        //Add padding bit, realign to septets.
        cur_val = ((internal_mdapi_hex2int(ptr)>>padding_bits)&0xFF)+((internal_mdapi_hex2int(ptr+2)<<(8-padding_bits))&0xFF);
        val = ((cur_val << offset) & 0x7F)+((last_val >> (8 - offset))&0xFF);
        last_val = cur_val;
        //printf("val raw = 0x%X, val = 0x%X\n", val, gsm_to_latin1_table[(kal_uint8)(val & 0xFF)]);
        output[output_len++] = (char)gsm_to_latin1_table[(kal_uint8)(val & 0xFF)];
        if (output_len == output_size - 1) {
            output[output_len] = 0;
            return output_len;
        }

        if(offset == 6){
            val = ((cur_val>>1)&0x7F) ;
            //printf("val raw = 0x%X, val = 0x%X\n", val, gsm_to_latin1_table[(kal_uint8)(val & 0xFF)]);
            output[output_len++] = (char)gsm_to_latin1_table[(kal_uint8)(val & 0xFF)];
            if (output_len == output_size - 1) {
                output[output_len] = 0;
                return output_len;
            }
        }
        ptr += 2;
    }

    /* decode the last octet */
    cur_val = (internal_mdapi_hex2int(ptr) >> padding_bits) & 0xFF;
    offset = i % 7;
    val = ((cur_val << offset) & 0x7F)+((last_val >> (8 - offset))&0xFF);
    //printf("val raw = 0x%X, val = 0x%X\n", val, gsm_to_latin1_table[(kal_uint8)(val & 0xFF)]);
    output[output_len++] = (char)gsm_to_latin1_table[(kal_uint8)(val & 0xFF)];
    if (output_len == output_size - 1) {
        output[output_len] = 0;
        return output_len;
    }

    //printf("output = [%s], output_len = [%d]\n", output, output_len);
    if(offset == 6){
        val = ((cur_val >> 1) & 0x7F);
        //printf("val raw = 0x%X, val = 0x%X\n", val, gsm_to_latin1_table[(kal_uint8)(val & 0xFF)]);
        if (val || octect) {
            output[output_len++] = (char)gsm_to_latin1_table[(kal_uint8)(val & 0xFF)];
            if (output_len == output_size - 1) {
                output[output_len] = 0;
                return output_len;
            }
        }
    }
    output[output_len] = 0;

    return output_len;
}

int internal_mdapi_address_decode(char *number, char *output, int output_size, int type) {
    // 81 90 21 43 65 87 --> 0912345678
    RLOGD("%s, %s, %d", __FILE__, __FUNCTION__, __LINE__);

    int len = 0;
    int output_len = 0;
    int val = 0;
    char *ptr = number;
    int padding_bit = 0;//To identify the number is even or odd
    //RLOGD("before internal_mdapi_hex2int(ptr), ptr :%s",ptr);
    // Length
    len = internal_mdapi_hex2int(ptr);
    //RLOGD("after internal_mdapi_hex2int(ptr), ptr :%s",ptr);
    ptr += 2;
    //RLOGD("after +=2, ptr :%s",ptr);

    if (len == 0) {
        return 2;
    } else if (len < 0) {
        return -1;
    }

    if (type) {
        len = (len) << 1;
    } else {
        len += 2;
    }

    RLOGD("Address length = %d ", len);

    // Type-of-address
    val = internal_mdapi_hex2int(ptr);
    ptr += 2;
    len -= 2;
    if (val == 0x91) {
        // international number
        output_len += snprintf(output+output_len, output_size-output_len-1, "+");
    } else if (val == 0x81) {
        // national number
    } else if ((val & 0x50) == 0x50 ){
        // alphabet number
    } else {
        RLOGD("Invalid type-of-address : %02X " , val);
    }

    // decode alphabet number
    if (0x50 == (val & 0x50)) {
        kal_int32 octect = (((len * 4) / 7 ) % 8 == 0) ? 1 : 0;
        kal_char * tmp = (kal_char * )malloc(len + 1);
        if (!tmp) {
            RLOGD("Invalid type-of-address : %02X ", val);
            return -1;
        }
        memcpy(tmp, ptr, len);
        tmp[len] = 0;
        output_len += internal_mdapi_sms_7bit_decode(tmp, output, output_size - output_len - 1, 0, octect);
        free(tmp);
        tmp = NULL;
    } else {
        do {
            if (len > 1) {
                if (*ptr == 'F') {
                    output_len += snprintf(output + output_len, output_size - output_len - 1, "%c", *(ptr + 1));
                } else {
                    output_len += snprintf(output + output_len, output_size - output_len - 1, "%c%c", *(ptr + 1), *(ptr));
                }

                len -= 2;
                ptr += 2;
            } else {
                output_len += snprintf(output + output_len, output_size - output_len - 1, "%c", *(ptr + 1));
                len -= 1;
                ptr += 1;
                padding_bit = 1;
            }
        } while (len > 0);
    }

    return (ptr-number+padding_bit);
}

void _smsbuf_byte_align(smsbuf_t *buf)
{
    if (buf->curbyte > buf->finalbyte) {
        return;
    }
    if (buf->curbit != 0) {
        buf->curbit = 0;
        buf->curbyte++;
    }
}

void _smsbuf_init(smsbuf_t *buf)
{
    buf->curbyte = buf->smsbuf;
    buf->curbit = 0;
    memset(buf->smsbuf, 0, sizeof(buf->smsbuf));
    buf->finalbyte = buf->curbyte + sizeof(buf->smsbuf);
}

void _smsbuf_set1bit(smsbuf_t *buf, kal_uint8 b)
{
    if (buf->curbyte > buf->finalbyte)
        return;

    if (b != 0)
        *buf->curbyte |= (1 << buf->curbit);

    buf->curbit++;
    if (buf->curbit == 8) {
        buf->curbit = 0;
        buf->curbyte++;
    }
}

void _smsbuf_set_multi_bits(smsbuf_t *buf, kal_uint32 value, kal_uint8 bit_num)
{
    kal_int32 i = 0;

    for (i = 0; i < bit_num; i++) {
        _smsbuf_set1bit(buf, (value & (1 << i)) != 0);
    }
}

void _smsbuf_set_octet(smsbuf_t *buf, kal_uint8 c)
{
    _smsbuf_byte_align(buf);
    *(buf->curbyte) = c;
    buf->curbyte++;
}

void _smsbuf_septet_align(smsbuf_t *buf)
{
    if (buf->curbyte > buf->finalbyte)
        return;
    while (((buf->curbyte - buf->septet_start) * 8 + buf->curbit) % 7 != 0)
        _smsbuf_set1bit(buf, 0);
}

void _smsbuf_set_septet_start(smsbuf_t *buf)
{
    _smsbuf_byte_align(buf);
    buf->septet_start = buf->curbyte;
}
kal_uint8 lookup_latin1_to_gsm_ex_table(kal_int8 character)
{
    int i;
    for( i=0; latin1_to_gsm_tableEx[i].symbol != NULL; i++) {
        if (character == latin1_to_gsm_tableEx[i].symbol)
            return latin1_to_gsm_tableEx[i].value;
    }
    return NULL;
}
void _smsbuf_set_string(smsbuf_t *buf, kal_int8 *str, kal_int32 size)
{
    kal_int32 i = 0;
    _smsbuf_septet_align(buf);

    //RLOGD("%s, %s, %d. string is:[%s],size is:[%d]", __FILE__, __FUNCTION__, __LINE__, (char*)str, size);
    while (*str && size) {
        /* change the iso8859latin1 charactor to gsm support charactor */
        //RLOGD("%s, %s, %d. character is:[%d]-[0x%x]", __FILE__, __FUNCTION__, __LINE__, (int)*str,*str);
        kal_uint8 c = lookup_latin1_to_gsm_ex_table(*str);
        if(c != NULL)
        {
            //RLOGD("extent table!");
            for (i = 0; i < 7; i++) {
                _smsbuf_set1bit(buf, ((1 << i) & 0x1b) != 0);
            }
            size--;
        } else {
            c = latin1_to_gsm_table[(kal_uint8)*str];
        }
        //RLOGD("%s, %s, %d. character is:[%d]-[0x%x]", __FILE__, __FUNCTION__, __LINE__, (int)c,c);
        str++;
        size--;
        for (i = 0; i < 7; i++) {
            _smsbuf_set1bit(buf, ((1 << i) & c) != 0);
            //KAL_RLOG(KAL_LOG_INFO, LOG_TAG, "curbit = [%d] curbyte=[%X]", buf->curbit, *buf->curbyte);
        }
        //KAL_RLOG(KAL_LOG_INFO, LOG_TAG, "size = [%d] c = [%c]", size, c);
    }
}

void _smsbuf_set_addr(smsbuf_t *buf, kal_int8 *str)
{
    _smsbuf_byte_align(buf);

    while (*str) {
        kal_uint8 c = *str;

        if (*str == '*') {
            c = 0xa;
        } else if (*str == '#') {
            c = 0xb;
        } else if (*str == 'p') {
            c = 0xc;
        } else if (*str == 'w') {
            c = 0xd;
        } else if (*str == '+') {
            c = 0xe;
        } else {
            c = *str - '0';
        }

        if (buf->curbit == 0) {
            *buf->curbyte = c;
            buf->curbit = 4;
        } else {
            *buf->curbyte |= (c << 4);
            buf->curbyte++;
            buf->curbit = 0;
        }

        str++;
    }

    if (buf->curbit == 4) {
        *buf->curbyte |= 0xF0;
        buf->curbyte++;
        buf->curbit = 0;
    }
}

kal_int32 _smsbuf_hex_string(smsbuf_t *buf, kal_int8 *output, kal_int32 size)
{
    kal_int32 len = 0;
    kal_uint8 *str = buf->smsbuf;

    _smsbuf_byte_align(buf);
    while (str != buf->curbyte && (size - len) > 3) {
        len += snprintf(output + len, size - len, "%02X", *str);
        str++;
    }

    return len;
}

kal_int32 _mdapi_sms_encode_addr(smsbuf_t *buf, kal_int8 *smsc, kal_int32 type)
{
    kal_int32 len = strlen(smsc);
    kal_int8 *str = smsc;

    kal_uint8 addr_plane = SMS_NP_ISDNTEL;
    kal_uint8 addr_type = SMS_TON_UNKNOWN;

    if (len <= 0) {
        /* use default sca for sms message */
        _smsbuf_set_octet(buf, 0);
        return 1;;
    }

    if (len > 255) {
        RLOGD("input number is too long. len[%d] smsc[%s] ", len, smsc);
        return 0;
    }

    /* check the type of address */
    while (*str) {
        if (!isdigit(*str) && *str != '+' && *str != '*'
            && *str != '#' && *str != 'p' && *str != 'w')
            addr_type = SMS_TON_ALPHANUMERIC;
        str++;
    }

    if (smsc[0] == '+' && addr_type != SMS_TON_ALPHANUMERIC) {
        addr_type = SMS_TON_INTERNATIONAL;
        /* */
        str = &smsc[1];
        len--;
    } else {
        str = smsc;
    }

    /* set len */
    if (type == SMS_ENCODE_SCA)
        _smsbuf_set_octet(buf, len / 2 + len % 2 + 1);
    else {
        _smsbuf_set_octet(buf, len);
    }

    RLOGD("%02X, %d ", *(buf->curbyte), buf->curbyte - buf->smsbuf);

    _smsbuf_set_multi_bits(buf, addr_plane, 4);
    _smsbuf_set_multi_bits(buf, addr_type, 3);
    _smsbuf_set1bit(buf, 1);

    RLOGD("%02X, %d ", *(buf->curbyte), buf->curbyte - buf->smsbuf);

    if (addr_type == SMS_TON_ALPHANUMERIC) {
        _smsbuf_set_septet_start(buf);
        _smsbuf_set_string(buf, str, len);
    } else {
        _smsbuf_set_addr(buf, str);
    }

    _smsbuf_byte_align(buf);

    return buf->curbyte - buf->smsbuf;
}

kal_int32 _mdapi_sms_encode_pdu(mdapi_sms_record_t *sms,
                            kal_int8 *smsc,
                            mdapi_sms_settings_t *settings,
                            kal_char *start,
                            kal_int32 send_size,
                            kal_uint8 *udh,
                            kal_char *output,
                            kal_int32 out_len,
                            kal_int32 *sca_out_len)
{
    smsbuf_t smsbuf;
    kal_int32 udh_len = 0;
    kal_int32 len = 0;
    kal_int32 sca_len = 0;

    if (udh) {
        udh_len = udh[0];
    } else {
        udh_len = 0;
    }

    memset(&smsbuf, 0, sizeof(smsbuf));
    _smsbuf_init(&smsbuf);

    /* SMSC */
    sca_len = _mdapi_sms_encode_addr(&smsbuf, smsc, SMS_ENCODE_SCA);

    /* Encode PDU Type */
    {
        _smsbuf_byte_align(&smsbuf);

        /* Message Type Indicator */
        _smsbuf_set_multi_bits(&smsbuf, SMS_SUBMIT, 2);
        //KAL_RLOG(KAL_LOG_INFO, LOG_TAG, "curbit = [%d] curbyte=[%02X]", smsbuf.curbit, *smsbuf.curbyte);
        /* Reject Duplicate */
        _smsbuf_set1bit(&smsbuf, 0);
        //KAL_RLOG(KAL_LOG_INFO, LOG_TAG, "curbit = [%d] curbyte=[%02X]", smsbuf.curbit, *smsbuf.curbyte);
        /* Validity Period Format */
        _smsbuf_set_multi_bits(&smsbuf, settings->vpf, 2);
        //KAL_RLOG(KAL_LOG_INFO, LOG_TAG, "curbit = [%d] curbyte=[%02X]", smsbuf.curbit, *smsbuf.curbyte);

        /* Status Report Request */
        if (settings->srr)
            _smsbuf_set1bit(&smsbuf, 1);
        else
            _smsbuf_set1bit(&smsbuf, 0);
        //KAL_RLOG(KAL_LOG_INFO, LOG_TAG, "curbit = [%d] curbyte=[%02X]", smsbuf.curbit, *smsbuf.curbyte);

        /* User Data Header Indicator */
        if (udh_len)
            _smsbuf_set1bit(&smsbuf, 1);
        else
            _smsbuf_set1bit(&smsbuf, 0);

        //KAL_RLOG(KAL_LOG_INFO, LOG_TAG, "curbit = [%d] curbyte=[%02X]", smsbuf.curbit, *smsbuf.curbyte);

        /* Replay Path */
        _smsbuf_set1bit(&smsbuf, settings->rp);

        //KAL_RLOG(KAL_LOG_INFO, LOG_TAG, "curbit = [%d] curbyte=[%02X]", smsbuf.curbit, *smsbuf.curbyte);
    }

    /* Encode MR */
    {
        /* Message Reference Count, Long Message should use this */
        _smsbuf_set_octet(&smsbuf, 0);
    }

    /* Encode OA/DA */
    _mdapi_sms_encode_addr(&smsbuf, sms->phone_number, SMS_ENCODE_OADA);

    /* Encode Protocol Identifier */
    _smsbuf_set_octet(&smsbuf, 0x00);

    /* Encode DCS */
    {
        /* Bit 0,1, Class */
        _smsbuf_set_multi_bits(&smsbuf, sms->sms_class, 2);
        /* Bit 2,3, Class */
        _smsbuf_set_multi_bits(&smsbuf, sms->charset, 2);
        /* Bit 4 */
        _smsbuf_set1bit(&smsbuf, 0);
        /* Bit 5 */
        /* No text compressed */
        _smsbuf_set1bit(&smsbuf, 0);
        /* Bit 6, 7 */
        _smsbuf_set_multi_bits(&smsbuf, 0, 0);
    }
    //_smsbuf_byte_align(&smsbuf);

    /* Validity Period */
    {
        if (settings->vpf == SMS_VPF_ABSOLUTE) {
            // TODO: add absolute validity period

        } else if (settings->vpf == SMS_VPF_RELATIVE) {
            // TODO: add validity period support
            kal_uint8 validity_period = 0xAA;
            _smsbuf_set_octet(&smsbuf, validity_period);
        }
    }

    /* User Data Header */
    {
        if (udh_len) {
            kal_int32 i = 0;
            if (sms->charset == MDAPI_SMS_CHARSET_UCS2) {
                _smsbuf_set_octet(&smsbuf, 1 + udh_len + send_size);
            } else if (sms->charset == MDAPI_SMS_CHARSET_GSM_7BIT) {
                _smsbuf_set_octet(&smsbuf, (((1 + udh_len) * 8 + 6) / 7) + send_size);
            } else if (sms->charset == MDAPI_SMS_CHARSET_GSM_8BIT) {
                _smsbuf_set_octet(&smsbuf, 1 + udh_len + send_size/2);
            }

            /* set the start byte of septet(7bits) align */
            _smsbuf_set_septet_start(&smsbuf);
            for (i = 0; i < udh_len + 1; i++) {
                _smsbuf_set_octet(&smsbuf, udh[i]);
            }
        } else {
            if(sms->charset == MDAPI_SMS_CHARSET_GSM_8BIT)
            {
                _smsbuf_set_octet(&smsbuf, send_size/2);
            }
            else
            {
                _smsbuf_set_octet(&smsbuf, send_size);
            }

            /* set the start byte of septet(7bits) align */
            _smsbuf_set_septet_start(&smsbuf);
        }
    }

    /* Get the Hex String */
    len = _smsbuf_hex_string(&smsbuf, output, out_len);
    RLOGD("encoded pdu = (%s), len = (%d) ", output, len);

    /* Set User Data */
    if (sms->charset == MDAPI_SMS_CHARSET_GSM_7BIT) {
        //_sms_iso8859latin1_to_gsm(start, gsm, *size + 1);
        _smsbuf_set_string(&smsbuf, start, send_size);
    } else if(sms->charset == MDAPI_SMS_CHARSET_GSM_8BIT) {
        kal_int32 i = 0;
        char outhex[MAX_PDU_SIZE] = {0};
        hexString_To_Bytes(start, send_size, outhex);
        for (i = 0; i < send_size/2; i++) {
            _smsbuf_set_octet(&smsbuf, outhex[i]);
        }
    } else {
        kal_int32 i = 0;
        for (i = 0; i < send_size; i += 2) {
            kal_uint16 tmp = 0;
            kal_uint8 *ptr = (kal_uint8 *)&tmp;

            *ptr = *(start + i);
            *(ptr + 1) = *(start + i + 1);
            tmp = htons(tmp);
            _smsbuf_set_octet(&smsbuf, *(ptr));
            _smsbuf_set_octet(&smsbuf, *(ptr + 1));
        }
    }

    /* Get the Hex String */
    len = _smsbuf_hex_string(&smsbuf, output, out_len);
    *sca_out_len = sca_len * 2;
    RLOGD("start = [%p] size = [%d] encoded pdu = (%s), len = (%d)", start, send_size, output, len);

    return len;
}

int handleDecodePduType(mdapi_sms_record_t *&sms, kal_int8 *&content, kal_int8 *&ptr,
               kal_int32 &udh) {
  kal_int32 pdu_type = internal_mdapi_hex2int(ptr);
  if ((pdu_type & 0x03) == SMS_DELIVER) {
    sms->msg_type = MDAPI_SMS_NORMAL_MSG;
  } else if ((pdu_type & 0x03) == SMS_STATUS_REPORT) {
    sms->msg_type = MDAPI_SMS_MSG_REPORT;
  } else {
    RLOGD("unkown PDU type = %02X(%02X), content = %s", pdu_type,
          pdu_type & 0x03, content);
     return MDAPI_RET_ERROR;
  }

  if ((pdu_type & 0x40) == 0x40) {
    udh = 1; /* find UDH header */
  }

  RLOGD("PDU Type = [%d]", pdu_type);
  return MDAPI_RET_SUCCESS;
}

static kal_int32 _mdapi_sms_decode_pdu(mdapi_sms_record_t *sms, kal_int8 *smsc,
                                       kal_int8 *content,
                                       kal_int32 content_size,
                                       kal_int32 *record_size) {

  kal_int32 ret = 0;

  kal_int8 *ptr = content;
  kal_int32 udh = 0;

  /*
   * Service Centre address informaction element
   */
  RLOGD("%s, %s, %d, ptr_len: %d, ptr: %s", __FILE__, __FUNCTION__, __LINE__,
        content_size, ptr);
  ret = internal_mdapi_address_decode(ptr, smsc, 512, 1);
  if (ret <= 0) {
    RLOGW("can't get SMSC address");
    return MDAPI_RET_ERROR;
  }
  ptr += ret;
  RLOGD("SMSC = [%s]", smsc);

  /* Protocol Data Unit Type(PDU Type) */
  {
    if (handleDecodePduType(sms, content, ptr, udh) == MDAPI_RET_ERROR) {
            RLOGD("can't get PDU type");
            return MDAPI_RET_ERROR;
    }
  }
  ptr += 2;

  /* decode originator address(OA) destination address(DA)*/
  ret = internal_mdapi_address_decode(ptr, sms->phone_number,
                                      sizeof(sms->phone_number), 0);
  if (ret <= 0) {
    RLOGD("can't get sender address");
    return MDAPI_RET_ERROR;
  }
  ptr += ret;
  RLOGD("sender = [%s]", sms->phone_number);

  /* protocol identifiler(PID) */
  ptr += 2;

  /* Data Coding Scheme (DCS) */
  {
    // kal_int32 text_compressed = 0;
    kal_int32 dcs = 0;

    dcs = internal_mdapi_hex2int(ptr);
    if (dcs < 0) {
      RLOGW("can't get the DCS");
      return MDAPI_RET_ERROR;
    }

    if ((dcs & 0xC0) == 0x00) {
      /* General Data Coding indication */
      if (dcs & 0x20) {
        // text_compressed = 1;
      }

      if (dcs & 0x10) {
        sms->sms_class = dcs & 0xF;
      }

      if (dcs & 0x04) {
        sms->charset = MDAPI_SMS_CHARSET_GSM_8BIT;

      } else if (dcs & 0x08) {
        sms->charset = MDAPI_SMS_CHARSET_UCS2;
      } else {
        sms->charset = MDAPI_SMS_CHARSET_GSM_7BIT;
      }
    } else {
      RLOGD("un-supported dcs type ");
      return MDAPI_RET_ERROR;
    }
    ptr += 2;
  }

  /* Time */
  {
    kal_int32 i = 0;
    kal_int8 tmp[4] = {0};
    kal_uint8 time[7] = {0};

    for (i = 0; i < 7; i++) {
      tmp[0] = *(ptr + 1);
      tmp[1] = *ptr;
      tmp[2] = 0;
      time[i] = strtoul(tmp, 0, 10);
      ptr += 2;
    }

    if (time[0] < 80) {
      time[0] += 100;
    }

    int result =
        snprintf(sms->time, sizeof(sms->time), "%04u-%02u-%02u %02u:%02u:%02u",
                 time[0] + 1900, time[1], time[2], time[3], time[4], time[5]);

    if (result < 0) {
      // Handle error in snprintf
      RLOGE("Failed to format sms time");
      return MDAPI_RET_ERROR;
    } else if (result >= sizeof(sms->time)) {
      // Handle buffer truncation
      RLOGE("sms time string was truncated");
      return MDAPI_RET_ERROR;
    }
    RLOGD("sms time = [%s]", sms->time);
  }
  {
    kal_int32 i = 0;
    kal_int32 udh_len = 0;
    kal_int32 data_len = 0;
    kal_int32 buf_len = 0;
    kal_int32 octect = 0;

    data_len = internal_mdapi_hex2int(ptr);
    ptr += 2;

    /* parse UDH header, to parse log SMS header */
    if (udh) {
      kal_int8 *ptr_udh = NULL;
      udh_len = internal_mdapi_hex2int(ptr);
      ptr += 2;
      ptr_udh = ptr;
      while (i < udh_len) {
        kal_int32 iei = 0;
        kal_int32 iei_len = 0;

        iei = internal_mdapi_hex2int(ptr);
        ptr += 2;
        iei_len = internal_mdapi_hex2int(ptr);
        ptr += 2;

        if (iei != 0x00 && iei != 0x08) {
          ptr += (iei_len) * 2;
          i += 2 + iei_len;
          continue;
        }

        if (iei == 0x00) {
          sms->ref_num = internal_mdapi_hex2int(ptr);
          ptr += 2;
        } else {
          sms->ref_num = ((internal_mdapi_hex2int(ptr) & 0xFF) << 8) |
                         (internal_mdapi_hex2int(ptr + 2) & 0xFF);
          ptr += 4;
        }

        sms->total_pack = internal_mdapi_hex2int(ptr);
        sms->curr_pack = internal_mdapi_hex2int(ptr + 2);
        break;
      }

      ptr = ptr_udh + (udh_len) * 2;
    }

    RLOGD("sms->charset = [%d] \n", sms->charset);
    switch (sms->charset) {
    case MDAPI_SMS_CHARSET_GSM_7BIT:
      octect = (data_len % 8 == 0) ? 1 : 0;
      // decode 7bit
      if (1 == udh) {
        kal_int32 padding_bits = ((udh_len + 1) * 8) % 7;
        if (padding_bits) {
          padding_bits = 7 - padding_bits;
        }
        data_len = internal_mdapi_sms_7bit_decode(
            ptr, sms->msg_content, content_size, padding_bits, octect);
      } else {
        data_len = internal_mdapi_sms_7bit_decode(ptr, sms->msg_content,
                                                  content_size, 0, octect);
      }

      *(sms->msg_content + data_len++) = '\0';
      *(sms->msg_content + data_len++) = '\0';

      *record_size = data_len;
      break;

    case MDAPI_SMS_CHARSET_GSM_8BIT:
      sms->msg_content = ptr;
      content_size = strlen(sms->msg_content);
      break;

    case MDAPI_SMS_CHARSET_UCS2:
      buf_len = 0;

      if (1 == udh) {
        data_len -= (udh_len + 1);
      }

      sms->msg_content[0] = '\0';

      for (i = 0; i < data_len; i = i + 2) {
        UTF16 tmp[2] = {0, 0};
        kal_uint8 *tmp_ptr = (kal_uint8 *)tmp;

        tmp_ptr[0] = internal_mdapi_hex2int(ptr);
        ptr += 2;
        tmp_ptr[1] = internal_mdapi_hex2int(ptr);
        ptr += 2;
        tmp[0] = ntohs(tmp[0]);
        // KAL_RLOG(KAL_LOG_INFO, LOG_TAG, "tmp = [%04X]", tmp);

        buf_len += kal_utf16_to_utf8((UTF8 *)(sms->msg_content + buf_len), tmp,
                                     *record_size - buf_len);
        if (*record_size - buf_len < 0) {
          return MDAPI_RET_ERROR;
        }
      }

      *(sms->msg_content + buf_len++) = '\0';
      *(sms->msg_content + buf_len++) = '\0';

      *record_size = buf_len;

      break;

    default:
      return MDAPI_RET_ERROR;
    }
  }

  sms->position = MDAPI_SMS_POS_INBOX;
  return MDAPI_RET_SUCCESS;
}

kal_int32 _mdapi_sms_get_msg_num(const char *msg, int charset, kal_int32 *msg_num, kal_int32 *msg_len)
{
    RLOGD("%s, %s, %d, send sms content = [%s]",__FILE__, __FUNCTION__, __LINE__, msg);
    *msg_num = 0;
    *msg_len = 0;
    kal_int32 max_len = 0;

    if (charset == MDAPI_SMS_CHARSET_GSM_7BIT) {
        *msg_len = strlen(msg);

        //special char
        kal_int32 extenSize =0;
        kal_char* point = (kal_char*)msg;
        kal_int32 size = *msg_len;
        while (*point && size) {
            kal_uint8 c = lookup_latin1_to_gsm_ex_table((kal_int8)*point);
            if(c != NULL)
                extenSize++;
            point++;
            size--;
        }
        //special char
        if (*msg_len + extenSize > 160) {
            *msg_num = (*msg_len + extenSize + MAX_7BIT_MSG_LEN - 1) / MAX_7BIT_MSG_LEN;
            max_len = MAX_7BIT_MSG_LEN;
        } else {
            *msg_num = 1;
        }

        if (*msg_num > MAX_CONCATENATED_MSG) {
            RLOGW("message is too long. msg_len[%d], msg_num[%d]", *msg_len, *msg_num);
            return MDAPI_RET_ERROR;
        }
    RLOGD("%s, %s, %d, 7bit msg_len = [%d] ,msg_num=[%d]", __FILE__, __FUNCTION__, __LINE__, *msg_len, *msg_num);
    } else if (charset == MDAPI_SMS_CHARSET_UCS2) {
        UTF16 *dest = NULL;
        *msg_len = kal_utf8_to_utf16(NULL, (const UTF8 *)msg, 0);
        if (*msg_len > 70) {
            *msg_num = (*msg_len + MAX_UCS2_MSG_LEN - 1) / MAX_UCS2_MSG_LEN;
            max_len = MAX_UCS2_MSG_LEN * 2;
        } else {
            *msg_num = 1;
        }

        if (*msg_num > MAX_CONCATENATED_MSG) {
            RLOGW("message is too long. msg_len[%d], msg_num[%d]", msg_len, msg_num);
            return MDAPI_RET_ERROR;
        }
    } else if (charset == MDAPI_SMS_CHARSET_GSM_8BIT) {
        *msg_len = strlen(msg);
        if(*msg_len > (140 * 2))
        {
            *msg_num = (*msg_len + MAX_8BIT_MSG_LEN - 1) / MAX_8BIT_MSG_LEN;
            max_len = MAX_8BIT_MSG_LEN;

            if (*msg_num > MAX_CONCATENATED_MSG) {
                RLOGW("message is too long. msg_len[%d], msg_num[%d]", *msg_len, *msg_num);
                return MDAPI_RET_ERROR;
            }
        }
        else {
            *msg_num = 1;
        }
    } else{
        RLOGW("Not support charset");
        return MDAPI_RET_ERROR;
    }

    return MDAPI_RET_SUCCESS;
}

void handleGsm7BitCharset(kal_int32 &msg_num, kal_int32 &msg_len, kal_int32 &max_len,
               mdapi_sms_record_t *&sms) {
  msg_len = strlen(sms->msg_content);

  // for special char
  kal_int32 extenTotalSize = 0;
  kal_int32 i = 0;
  kal_char *point = sms->msg_content;
  RLOGD("XXX msg len %d \n", msg_len);
  kal_int32 size = msg_len;
  while (*point && size) {
    kal_uint8 c = lookup_latin1_to_gsm_ex_table((kal_int8)*point);
    if (c != NULL) {
      extenTotalSize++;
    }
    point++;
    size--;
  }
  // for specail char
  msg_len += extenTotalSize;
  RLOGD("XXX msg_len %d extenTotalSize %d \n", msg_len, extenTotalSize);
  if (msg_len > 160) {
    msg_num = (msg_len + MAX_7BIT_MSG_LEN - 1) / MAX_7BIT_MSG_LEN;
    max_len = MAX_7BIT_MSG_LEN;
  } else {
    msg_num = 1;
  }
}

int smsPduEncode(const char *smsc, const char *da_num, const char *msg,
                 int charset, char *smsc_pdu, char **pdu) {
  mdapi_sms_record_t record;
  mdapi_sms_settings_t sms_settings;
  kal_int32 status = MDAPI_RET_ERROR;
  kal_int32 msg_num = 0;
  kal_int32 msg_len = 0;
  kal_int32 max_len = 0;
  kal_char *msg_content = NULL;
  kal_int32 out_len = 0;
  kal_int8 output[MAX_PDU_SIZE] = {0};
  kal_int32 sca_out_len;
  mdapi_sms_record_t *sms = &record;

  memset(sms, 0, sizeof(mdapi_sms_record_t));

  int ret =
      snprintf(sms->phone_number, sizeof(sms->phone_number), "%s", da_num);
  if (ret < 0) {
    // Handle encoding error
    RLOGE("%s: snprintf failed with ret=%d", __func__, ret);
    return MDAPI_RET_ERROR;
  } else if (ret >= sizeof(sms->phone_number)) {
    // Handle truncation, the output was truncated
    RLOGE("%s: snprintf truncated with ret=%d", __func__, ret);
    return MDAPI_RET_ERROR;
  }
  sms->msg_content = (kal_char *)msg;
  sms->charset = charset;

  sms_settings.rd = 1;
  sms_settings.vpf = SMS_VPF_RELATIVE;
  sms_settings.srr = 1;
  sms_settings.rp = 0;
  sms_settings.validity_period = 0;

  if (sms->charset == MDAPI_SMS_CHARSET_GSM_7BIT) {
    handleGsm7BitCharset(msg_num, msg_len, max_len, sms);

    if (msg_num > MAX_CONCATENATED_MSG) {
        RLOGW("message is too long. msg_len[%d], msg_num[%d]", msg_len,
        msg_num); return MDAPI_RET_ERROR;
    }
    RLOGD("7bit msg_len = [%d] ,msg_num=[%d]", msg_len, msg_num);
    msg_content = sms->msg_content;
  } else if (sms->charset == MDAPI_SMS_CHARSET_UCS2) {
    UTF16 *dest = NULL;
    msg_len = kal_utf8_to_utf16(NULL, (const UTF8 *)sms->msg_content, 0);
    if (msg_len > 70) {
      msg_num = (msg_len + MAX_UCS2_MSG_LEN - 1) / MAX_UCS2_MSG_LEN;
      max_len = MAX_UCS2_MSG_LEN * 2;
    } else {
      msg_num = 1;
    }

    if (msg_num > MAX_CONCATENATED_MSG) {
      RLOGW("message is too long. msg_len[%d], msg_num[%d]", msg_len, msg_num);
      return MDAPI_RET_ERROR;
    }

    msg_content = (kal_char *)malloc((msg_len + 1) * sizeof(UTF16));
    dest = (UTF16 *)msg_content;
    msg_len = kal_utf8_to_utf16(dest, (const UTF8 *)sms->msg_content,
                                (msg_len + 1) * sizeof(UTF16));
    if (msg_len <= 0) {
      free(msg_content);
      msg_content = NULL;
      return MDAPI_RET_ERROR;
    }
    msg_len *= 2;
    RLOGD("ucs2 msg_len = [%d] ,msg_num=[%d] ", msg_len, msg_num);
  } else if (sms->charset == MDAPI_SMS_CHARSET_GSM_8BIT) {
    msg_len = strlen(sms->msg_content);
    msg_content = sms->msg_content;
    if (msg_len > (140 * 2)) {
      msg_num = (msg_len + MAX_8BIT_MSG_LEN - 1) / MAX_8BIT_MSG_LEN;
      max_len = MAX_8BIT_MSG_LEN;
    } else {
      msg_num = 1;
    }

    if (msg_num > MAX_CONCATENATED_MSG) {
      RLOGW("message is too long. msg_len[%d], msg_num[%d]", msg_len, msg_num);
      return MDAPI_RET_ERROR;
    }
    RLOGD("8bit msg_len = [%d] ,msg_num=[%d] ", msg_len, msg_num);
  } else {
    RLOGW("Not support charset");
    return MDAPI_RET_ERROR;
  }

  // set sms record
  _mdapi_sms_set_timestamp(sms);
  if (msg_num == 1) {
    out_len = _mdapi_sms_encode_pdu(sms, (kal_int8 *)smsc, &sms_settings,
                                    msg_content, msg_len, NULL, output,
                                    sizeof(output), &sca_out_len);
    memcpy(smsc_pdu, output, sca_out_len);
    // RLOGD("%s, %s, %d, returned encoded smsc_pdu:%s", __FILE__, __FUNCTION__,
    // __LINE__, smsc_pdu); RLOGD("%s, %s, %d, output + sca_out_len:%s, out_len
    // - sca_out_len: %d", __FILE__, __FUNCTION__, __LINE__, output +
    // sca_out_len, out_len - sca_out_len);

    // RLOGD("%s, %s, %d, pdu:%s sizeof(pdu[0])=%d", __FILE__, __FUNCTION__,
    // __LINE__, pdu[0], sizeof(pdu[0]));

    memset(pdu[0], 0, MAX_PDU_SIZE);
    RLOGD("%s, %s, %d, pdu:%s", __FILE__, __FUNCTION__, __LINE__, pdu[0]);
    strncpy(pdu[0], output + sca_out_len, out_len - sca_out_len);
    RLOGD("%s, %s, %d, returned encoded pdu:%s\n, len=%d", __FILE__,
          __FUNCTION__, __LINE__, pdu[0], strlen(pdu[0]));
  } else {
    // send long sms
    kal_int32 index = 0;
    kal_int32 offset = 0;
    static kal_uint8 concat_msgid;
    concat_msgid += (rand() + 1) % 256;
    RLOGD("start send one long msg, total has %d part msg", msg_num);
    for (index = 0; index < msg_num; index++) {
      kal_uint8 udh[] = {
          5, 0, 3, concat_msgid, (kal_uint8)msg_num, (kal_uint8)(index + 1)};
      kal_uint32 size = 0;
      kal_int8 *start = NULL;
      size = msg_len > max_len ? max_len : msg_len;
      msg_len -= size;
      start = msg_content + offset;
      int exterSize = 0;
      if (sms->charset == MDAPI_SMS_CHARSET_GSM_7BIT) {
        char *point = start;
        int calsize = size;
        while (*point && calsize) {
          kal_uint8 c = lookup_latin1_to_gsm_ex_table((kal_int8)*point);
          if (c != NULL) {
            exterSize++;
            calsize--;
          }
          point++;
          calsize--;
        }
      }
      offset = offset + size - exterSize;
      // calculate offset
      RLOGD(" msg_len %d size %d offset %d exterSize %d", msg_len, size, offset,
            exterSize);
      out_len = _mdapi_sms_encode_pdu(sms, (kal_int8 *)smsc, &sms_settings,
                                      start, size, udh, output, sizeof(output),
                                      &sca_out_len);
      memcpy(smsc_pdu, output, sca_out_len);
      memset(pdu[index], 0, MAX_PDU_SIZE);
      memcpy(pdu[index], output + sca_out_len, out_len - sca_out_len);
    }
  }
  if (sms->charset == MDAPI_SMS_CHARSET_UCS2) {
    if (msg_content != NULL)
      free(msg_content);
  }
  return MDAPI_RET_SUCCESS;
}

int smsPduDecode(const char *pdu_str, int pdu_len,
                        char *da_num, char *smsc, char *msg, int *charset, int *ref_num, int *cur_pack , int *total_pack)
{
    kal_char msg_tmp[MAX_PDU_SIZE] = {0};
    mdapi_sms_record_t record;

    kal_int32 status = MDAPI_RET_ERROR;
    kal_int32 out_len = 210;
    RLOGD("%s, %s, %d, pdu_len: %d, pdu_str: %s", __FILE__,__FUNCTION__, __LINE__, pdu_len, pdu_str);
    record.ref_num = -1;
    record.curr_pack = -1;
    record.total_pack = -1;

    record.msg_content = msg_tmp;

    status = _mdapi_sms_decode_pdu(&record, smsc, (kal_int8 *)pdu_str,
                            pdu_len, &out_len);
    if(status == MDAPI_RET_SUCCESS) {
        memcpy(da_num, record.phone_number, strlen(record.phone_number));
        memcpy(msg, record.msg_content, strlen(record.msg_content));
        *charset = record.charset;
        *charset = record.charset;
        *ref_num = record.ref_num;
        *cur_pack = record.curr_pack;
        *total_pack = record.total_pack;
    } else {
        RLOGW("PDU decode error");
    }

    return status;
}

int hexChar_To_Int(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');
    if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);

    return 0;
}
void hexString_To_Bytes(char *in, int inLength, char *out)
{
    int i;

    if (in == NULL || out == NULL)
    {
        return;
    }

    for (i = 0 ; i < inLength ; i += 2)
    {
        out[i/2] = (char)((hexChar_To_Int(in[i]) << 4)
                           | hexChar_To_Int(in[i+1]));
    }
}
