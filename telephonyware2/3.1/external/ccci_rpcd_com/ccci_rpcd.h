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


#ifndef __CCCI_RPCD_H__
#define __CCCI_RPCD_H__

#define CCCI_RPC_MAX_BUFFERS        (2)
#define RPC_API_RESP_ID            0xFFFF0000
#define RPC_PARAM_ERROR            -2


typedef enum
{
    /* 0x3001 ~ 0x3012 are reserved for tc1 */

    IPC_RPC_QUERY_AP_SYS_PROPERTY = 0x400F,
    IPC_RPC_SAR_TABLE_IDX_QUERY_OP = 0x4010,
    IPC_RPC_AMMS_DRDI_CONTROL = 0x4014,
    IPC_RPC_SAVE_MD_CAPID = 0x4015,
} RPC_CCCI_OP_ID;

typedef struct
{
    unsigned length;
    unsigned index;
}rpc_stream_msg_t;


typedef struct
{
    unsigned int rpc_ops_id;
    unsigned char     buffer[0];
} rpc_stream_buffer_t;

typedef struct RPC_PACKET_INFO_STRUCT
{
    unsigned int Length;
    void         *pData;
} RPC_PACKET_INFO;

typedef struct
{
    unsigned int data[2];
    unsigned int channel;
    unsigned int reserved;
} CCCI_BUFF_T;

typedef struct
{
    CCCI_BUFF_T header;
    rpc_stream_buffer_t payload;
}STREAM_DATA;

typedef enum {
    RPC_BUFF_IDLE = 0,   // current port is not waiting for more data
    RPC_BUFF_WAIT        // current port is waiting for more data to come in
} RPC_BUFF_STATUS;
#define RPC_REQ_BUFFER_MUN        5
#define RPC_BUFFER_SLOT_NUM 5
#define MAX_RPC_PKT_BYTE (3584-128)
#define MAX_RPC_BUF_BYTE 4096
#define CCCI_RPC_REQ_SEND_AGAIN 0x80000000
#define CCCI_RPC_PEER_REQ_SEND_AGAIN(_p) (((((CCCI_BUFF_T*)(_p))->data[0] & CCCI_RPC_REQ_SEND_AGAIN) != 0)? 1: 0)

typedef struct
{
    void*           pRpcBuf;
    unsigned char   rpc_buff_state[RPC_BUFFER_SLOT_NUM];
    unsigned int    rpc_buff_offset[RPC_BUFFER_SLOT_NUM];
    int             bNonAck;
}RPC_INFO;


/* for amms drdi new feature */
#define DRDI_SET_NUM_TOTAL_SET 15

enum AMMS_STATUS {
	AMMS_STATUS_INVALID = -2,
	AMMS_STATUS_FAIL = -1,
	AMMS_STATUS_SUCCESS = 0,
};

enum AMMS_CMD {
	AMMS_CMD_INIT = 1,
	AMMS_CMD_DRDI_COPY,
};

enum AMMS_VER {
	AMMS_V1_2 = 0,
	AMMS_V3 = 3,
};

enum COPY_STA {
	COPY_FAIL = -1,
	COPY_GOING = 0,
	COPY_DONE = 1,
};

struct amms_address_range {
	unsigned int start_address; /* flash */
	unsigned int length; /* copy length */
};

struct amms_copy_range {
	unsigned int src_offset; /* flash */
	unsigned int dst_offset; /* bank4 */
	unsigned int length; /* copy length */
};

struct amms_init_request_cmd {
	unsigned char version; /*enum AMMS_VERSION*/
	unsigned char set_total_num;
	unsigned char padding[2];
	struct amms_address_range drdi_info[DRDI_SET_NUM_TOTAL_SET];
};

struct amms_init_cmd_reply {
	unsigned char version; /*enum AMMS_VERSION*/
	char copy_status; /*drdi data copy flag*/
	char drdi_info_status; /*drdi set info error check*/
	unsigned char padding;
};

struct amms_drdi_cpy_cmd {
	char last_cpy; /*The last copy flag*/
	unsigned char padding[3];
	struct amms_copy_range copy_info[DRDI_SET_NUM_TOTAL_SET];
};

struct amms_drdi_cpy_cmd_reply {
	char last_cpy_status;
	unsigned char padding[3];
};

struct amms_msg_request {
	unsigned char cmd; /*AMMS cmd index*/
	unsigned char seq_id; /*md control and +1 by every request*/
	unsigned char padding[2]; /*padding to align 4 bytes*/
	union {
		struct amms_init_request_cmd init_request_cmd;/*struct*/
		struct amms_drdi_cpy_cmd drdi_cpy_cmd;
	}u_req;
};

struct amms_msg_response {
	unsigned char status; /*enum AMMS_STATUS*/
	unsigned char seq_id; /*the same with the one request*/
	unsigned char padding[2]; /*padding to align 4 bytes*/
	union {
		struct amms_init_cmd_reply init_reply;
		struct amms_drdi_cpy_cmd_reply cpy_reply;
	}u_resp;
};

/* for MD CAPID AND AAC */
struct Md_Rpc_Capid_Aac {
	unsigned int capid; /*capid*/
	unsigned int aac;   /*aac*/
};

/* extern API */
extern int find_image_from_pt(const char *img_name, int *offset, int *img_size);
extern void AB_image_get(char *buf);

#endif // __CCCI_RPC_H__
