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
#ifndef YOCTO_SMS_PDU_H
#define YOCTO_SMS_PDU_H

/*
 * Unix/Linux
 */
typedef char kal_int8;

typedef unsigned char kal_uint8;

typedef short kal_int16;

typedef unsigned short kal_uint16;

typedef int kal_int32;

typedef unsigned int kal_uint32;

/**/
typedef long long kal_int64;

/**/
typedef unsigned long long kal_uint64;

typedef char kal_char;

/* portable wide character for unicode character set */
typedef unsigned short          kal_wchar;

typedef kal_uint32 kal_time_t;


typedef unsigned long   UTF32;

typedef unsigned short  UTF16;

typedef unsigned short  UCS2;

typedef unsigned char   UTF8;

#define REPLACEMENT_CHAR    (UTF32)0x0000FFFD
#define MAX_UNI_BMP         (UTF32)0x0000FFFF
#define MAX_UNI_UCS2        MAX_UNI_BMP
#define MAX_UTF16           (UTF32)0x0010FFFF
#define MAX_UTF32           (UTF32)0x7FFFFFFF
#define MAX_LEGAL_UTF32     (UTF32)0x0010FFFF

#define STATUS_SUCCESS          0
#define STATUS_MEM_EXHAUSTED    (-0xC0000)
#define STATUS_ILLEGAL_UTF8     (-0xC0001)
#define STATUS_ILLEGAL_UTF16    (-0xC0002)
#define STATUS_ILLEGAL_UTF32    (-0xC0003)
#define STATUS_ILLEGAL_UCS2     (-0xC0004)
#define STATUS_NULL_POINTER     (-0x00001)

#define MDAPI_TIME_STR_SIZE         22
#define MDAPI_PHONE_NUMBER_SIZE     32
#define MDAPI_MAX_PDU_SIZE          512
#define MAX_CONCATENATED_MSG 32

#define MAX_OUT_SIZE 512
#define MAX_PDU_SIZE 512

// TODO: why ??
#define MAX_7BIT_MSG_LEN    153
#define MAX_UCS2_MSG_LEN    67
#define MAX_8BIT_MSG_LEN    268 //134*2


#define SMS_DELIVER         0x00
#define SMS_DELIVER_REPORT  0x00
#define SMS_SUBMIT          0x01
#define SMS_SUBMIT_REPORT   0x01
#define SMS_STATUS_REPORT   0x02
#define SMS_STATUS_COMMAND  0x02

#define TRUE    1
#define FALSE   0

#define SUR_HIGH_START  (UTF32)0xD800
#define SUR_HIGH_END    (UTF32)0xDBFF
#define SUR_LOW_START   (UTF32)0xDC00
#define SUR_LOW_END     (UTF32)0xDFFF

enum SMS_VPF_E
{
    SMS_VPF_NO_PRESENT  = 0x00,
    SMS_VPF_RESORVED    = 0x01,
    SMS_VPF_RELATIVE    = 0x02,
    SMS_VPF_ABSOLUTE    = 0x03,
    SMS_VPF_INVALID     = 0xFF,
};


enum SMS_ADDR_NUM_PLAN
{
    SMS_NP_UNKOWN       = 0x00,
    SMS_NP_ISDNTEL      = 0x01,
    SMS_NP_DATA         = 0x03,
    SMS_NP_TELIX        = 0x04,
    SMS_NP_NATIONAL     = 0x08,
    SMS_NP_PRIVATE      = 0x09,
    SMS_NP_RESERVED     = 0xFF,
};

enum SMS_ADDR_TYPE
{
    SMS_TON_UNKNOWN         = 0,
    SMS_TON_INTERNATIONAL   = 1,
    SMS_TON_NATIONAL        = 2,
    SMS_TON_NETWORKSPECIFIC = 3,
    SMS_TON_SUBSCRIBER      = 4,
    SMS_TON_ALPHANUMERIC    = 5,
    SMS_TON_ABBREVIATED     = 6,
    SMS_TON_RESERVED        = 7
};

enum SMS_ADDR_ENCODE_TYPE
{
    SMS_ENCODE_SCA      = 0,
    SMS_ENCODE_OADA     = 1,
    SMS_ENCODE_INVALID  = 0xFF,
};

enum MDAPI_RET_e {
    MDAPI_RET_SUCCESS       = 0,
    MDAPI_RET_ERROR         = 1,
    MDAPI_RET_TIMEOUT       = 2,
    MDAPI_RET_NOT_SUPPORT   = 3,
};

enum MDAPI_SMS_CHARSET_E
{
    MDAPI_SMS_CHARSET_GSM_7BIT  = 0x00000000,
    MDAPI_SMS_CHARSET_GSM_8BIT  = 0x00000001,
    MDAPI_SMS_CHARSET_UCS2      = 0x00000002,
    MDAPI_SMS_CHARSET_INVALID,
};

enum MDAPI_SMS_POSITION_E
{
    MDAPI_SMS_POS_INBOX     = 0x00000001,
    MDAPI_SMS_POS_SENTBOX   = 0x00000002,
    MDAPI_SMS_POS_DRAFBOX   = 0x00000003,
    MDAPI_SMS_POS_OUTBOX    = 0x00000004,
    MDAPI_SMS_POS_INVALID,
};

/* */
enum  MDAPI_SMS_MSGTYPE_E
{
    MDAPI_SMS_NORMAL_MSG    = 0x00000000,
    MDAPI_SMS_MSG_REPORT    = 0x00000001,
    MDAPI_SMS_MMS_ALERT     = 0x00000002,
    MDAPI_SMS_VOICE_MAIL    = 0x00000003,
    MDAPI_SMS_MSGTYPE_INVALID,
};

typedef struct _mdapi_sms_setting {
    kal_int8 rd;    /* reject duplicate */
    kal_int8 vpf;   /* validity peroid format */
    kal_int8 srr;   /* status report request */
    kal_int8 rp;    /* replay path */
    kal_int32 validity_period;   /* validity peroid */
} mdapi_sms_settings_t;

typedef struct _mdapi_sms_record {
    kal_int32 msg_id;
    kal_int32 is_read;
    kal_int32 position;
    kal_int32 result;
    kal_int32 msg_type;
    kal_int32 sms_class;
    kal_int32 total_pack;
    kal_int32 curr_pack;
    kal_int32 ref_num;
    kal_int32 msg_location;
    kal_int16 charset;
    kal_char  time[MDAPI_TIME_STR_SIZE];            /*YYYY-MM-DD HH:MM:SS*/
    kal_char  phone_number[MDAPI_PHONE_NUMBER_SIZE];
    kal_char  *msg_content;
} mdapi_sms_record_t;

typedef struct _smsbuf {
    kal_uint8 *curbyte;
    kal_uint8 *finalbyte;
    kal_uint8 *septet_start;
    kal_uint8 curbit;
    kal_uint8 smsbuf[512];
} smsbuf_t;

static const kal_int32 UTF16_HALF_SHIFT  = 10; /* used for shifting by 10 bits */

static const UTF32 UTF16_HALF_BASE = 0x0010000UL;
static const UTF32 UTF16_HALF_MASK = 0x3FFUL;

static const unsigned char utf8_tailing_bytes[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

static const UTF32 utf8_offsets[6] = {
    0x00000000UL,
    0x00003080UL,
    0x000E2080UL,
    0x03C82080UL,
    0xFA082080UL,
    0x80282080UL
};

static const UTF8 first_byte_mark[7] = {
    0x00,
    0x00,
    0xC0,
    0xE0,
    0xF0,
    0xF8,
    0xFC
};

//ISO 8859 Latin-1 / GSM 7 bit undefined char
#define NOP  ('_')

static kal_uint8 gsm_to_latin1_table[] =
{
  //0x00 '@',  -,  '$',  -,   -,   -,   -,   -,
    '@', 163, '$', 165, 232, 233, 249, 236,
  //0x08  -,   -,  LF,   -,   -,   CR,  -,   -,
    242, 199,  10, 216, 248,  13, 197, 229,
  //0x10  -,  '_',  -,   -,   -,   -,   -,   -,
    NOP, '_', NOP, NOP, NOP, NOP, NOP, NOP,
  //0x18  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, 198, 230, 223, 201,
  //0x20 ' ', '!', '"', '#', '?,  '%', '&', ''',
    ' ', '!', '"', '#', 164, '%', '&', '\'',
  //0x28 '(', ')', '*', '+', ',', '-', '.', '/',
    '(', ')', '*', '+', ',', '-', '.', '/',
  //0x30 '0', '1', '2', '3', '4', '5', '6', '7',
    '0', '1', '2', '3', '4', '5', '6', '7',
  //0x38 '8', '9', ':', ';', '<', '=', '>', '?',
    '8', '9', ':', ';', '<', '=', '>', '?',
  //0x40  -,  'A', 'B', 'C', 'D', 'E', 'F', 'G',
    161, 'A', 'B', 'C', 'D', 'E', 'F', 'G',
  //0x48 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
  //0x50 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
  //0x58 'X', 'Y', 'Z',  -,   -,   -,   -,   -,
    'X', 'Y', 'Z', 196, 214, 209, 220, 167,
  //0x60  -,  'a', 'b', 'c', 'd', 'e', 'f', 'g',
    191, 'a', 'b', 'c', 'd', 'e', 'f', 'g',
  //0x68 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  //0x70 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
  //0x78 'x', 'y', 'z',  -,   -,   -,   -,   -,
    'x', 'y', 'z', 228, 246, 241, 252, 224
};

static kal_uint8 latin1_to_gsm_table[] =
{
  //0x00  -,   -,   -,  -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
  //0x08  -,   -,   LF,  -,   -,   CR,  -,   -,
    NOP, NOP,  10, NOP, NOP,  13, NOP, NOP,
  //0x10  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
  //0x18  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
  //0x20 ' ', '!', '"', '#',  -,  '%', '&', ''',
    ' ', '!', '"', '#', 0x2, '%', '&', '\'',
  //0x28 '(', ')', '*', '+', ',', '-', '.', '/',
    '(', ')', '*', '+', ',', '-', '.', '/',
  //0x30 '0', '1', '2', '3', '4', '5', '6', '7',
    '0', '1', '2', '3', '4', '5', '6', '7',
  //0x38 '8', '9', ':', ';', '<', '=', '>', '?',
    '8', '9', ':', ';', '<', '=', '>', '?',
  //0x40  -,  'A', 'B', 'C', 'D', 'E', 'F', 'G',
    0x0, 'A', 'B', 'C', 'D', 'E', 'F', 'G',
  //0x48 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
  //0x50 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
  //0x58 'X', 'Y', 'Z',  -,   -,   -,   -,   -,
    'X', 'Y', 'Z', NOP, NOP, NOP, NOP,0x11,
  //0x60  -,  'a', 'b', 'c', 'd', 'e', 'f', 'g',
    NOP, 'a', 'b', 'c', 'd', 'e', 'f', 'g',
  //0x68 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  //0x70 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
  //0x78 'x', 'y', 'z',  -,   -,   -,   -,   -,
    'x', 'y', 'z', NOP, NOP, NOP, NOP, NOP,
  //0x80  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
  //0x88  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
  //0x90  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
  //0x98  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
  //0xA0  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, 0x40,NOP, 0x1, 0x24,0x3, NOP,0x5F,
  //0xA8  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
  //0xB0  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
  //0xB8  -,   -,   -,   -,   -,   -,   -,   -,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP,0x60,
  //0xC0 'A', 'A', 'A', 'A',  -,   -,   -,   -,
    'A', 'A', 'A', 'A', 0x5B,0xE, 0x1C,0x9,
  //0xC8 'E',  -,  'E', 'E', 'I', 'I', 'I', 'I',
    'E',0x1F, 'E', 'E', 'I', 'I', 'I', 'I',
  //0xD0  -,  -,   'O', 'O', 'O', 'O',  -,   -,
    NOP,0x5D, 'O', 'O', 'O', 'O',0x5C, NOP,
  //0xD8  -,  'U', 'U', 'U',  -,   -,  'Y',  -,
    0x0B,'U', 'U', 'U',0x5E, 'Y', NOP,0x1E,
  //0xE0  -,  'a', 'a', 'a',  -,   -,   -,   -,
    0x7F,'a', 'a', 'a',0x7B, 0xF,0x1D, 0x9,
  //0xE8  -,   -,  'e', 'e',  -,  'i', 'i', 'i',
    0x4, 0x5, 'e', 'e', 0x7, 'i', 'i', 'i',
  //0xF0  -,   -,   -,  'o', 'o', 'o',  -,   -,
    NOP,0x7D, 0x8, 'o', 'o', 'o',0x7C, NOP,
  //0xF8  -,   -,  'u', 'u',  -,  'y',  -,  'y',
    0xC, 0x6, 'u', 'u',0x7E, 'y', NOP, 'y'
};
typedef struct latin1_to_gsm_extable {
    kal_int8 symbol;
    kal_uint8 value;
} Latin1_to_Gsm_ExTable;
static Latin1_to_Gsm_ExTable latin1_to_gsm_tableEx[]=
{
    {'^', 0x14,},
    {'{', 0x28,},
    {'}', 0x29,},
    {'\\',0x2f,},
    {'[', 0x3C,},
    {'~', 0x3d,},
    {']', 0x3e,},
    {'|', 0x40,},
    {NULL,NULL,},
};

/* boolean representation */
typedef enum
{
    /* FALSE value */
    KAL_FALSE,
    /* TRUE value */
    KAL_TRUE
} kal_bool;

#ifdef __cplusplus
extern "C" {
#endif

kal_int32 kal_utf8_to_utf16(UTF16 *dest, const UTF8 *src, kal_int32 size);

kal_int32 kal_utf8_to_utf32(UTF32 *dest, const UTF8 *src, kal_int32 size);

kal_int32 kal_utf8_to_ucs2(UCS2 *dest, const UTF8 *src, kal_int32 size);

kal_int32 kal_utf16_to_utf8(UTF8 *dest, const UTF16 *src, kal_int32 size);

kal_int32 kal_utf32_to_utf8(UTF8 *dest, const UTF32 *src, kal_int32 size);

kal_int32 kal_ucs2_to_utf8(UTF8 *dest, const UCS2 *src, kal_int32 size);

kal_int32 kal_ext_ascii_to_utf8(UTF8 *dest, const kal_uint8 *src, kal_int32 size);

kal_int32 is_utf8_sequence(const UTF8 *start, const UTF8 *end);

int smsPduEncode(const char *smsc, const char *da_num, const char *msg, int charset,
                        char *smsc_pdu, char **pdu);

int smsPduDecode(const char *pdu_str, kal_int32 pdu_len,
                        char *da_num, char *smsc, char *msg, int *charset, int *ref_num, int *cur_pack , int *total_pack );
kal_int32 _mdapi_sms_get_msg_num(const char *msg, int charset, kal_int32 *msg_num, kal_int32 *msg_len);

#ifdef __cplusplus
}
#endif

#endif
