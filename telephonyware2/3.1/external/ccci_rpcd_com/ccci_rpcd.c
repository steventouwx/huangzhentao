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


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/vfs.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <wchar.h>
#include <linux/ioctl.h>
#include <ctype.h>
#include <signal.h>
//#include "sysenv_utils.h"

//#include <prop/properties.h>
#include "hardware/ccci_intf.h"
//#include <hardware_legacy/power.h>
#include <assert.h>
//#define RPC_WAKE_LOCK_NAME "ccci_rpc"
//#define RPC_WAKE_LOCK() acquire_wake_lock(PARTIAL_WAKE_LOCK, RPC_WAKE_LOCK_NAME)
//#define RPC_WAKE_UNLOCK() release_wake_lock(RPC_WAKE_LOCK_NAME)

//#include <cutils/log.h>
#include "platform/ccci_rpcd_platform.h"
#include "ccci_rpcd.h"


//#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "ccci_rpcd",__VA_ARGS__)
//#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "ccci_rpcd",__VA_ARGS__)
//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , "ccci_rpcd",__VA_ARGS__)
//#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , "ccci_rpcd",__VA_ARGS__)
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "ccci_rpcd",__VA_ARGS__)

static int          stream_support = 0;
static int          DeviceFd = 0;
static unsigned int RPC_MAX_BUF_SIZE = 2048;
static unsigned int RPC_MAX_ARG_NUM = 6;
static int          md_id = 0;
static RPC_INFO     g_RpcInfo;


static int RPC_GetPackInfo(RPC_PACKET_INFO* pPackInfo, unsigned char* pData)
{
    unsigned int PackNum = *((unsigned int*)pData);
    unsigned int Index = 0;
    unsigned int i;

    if(PackNum > RPC_MAX_ARG_NUM)
        return false;

    Index = sizeof(unsigned int);
    for(i = 0; i < PackNum; i++)
    {
        pPackInfo[i].Length = *((unsigned int*)(pData + Index));
        Index += sizeof(unsigned int);
        pPackInfo[i].pData = (pData + Index);
        //4 byte alignment
        Index += ((pPackInfo[i].Length+3)>>2)<<2;
    }

    if(Index > RPC_MAX_BUF_SIZE)
        return false;

    return true;
}

// 5 = CCCI header + Operation ID
unsigned int g_bak[5];

/*
 * @brief Prepare a packet buffer for sending to MD
 * @param
 *     pData [in] A pointer to argument data for sending
 *     data_to_send [in] Size in bytes of argument data to send
 *     ccci_src [in] The pointer to the CCCI header for every sub-packet
 *     op_id [in] Operation ID currently used
 *     again [in] A flag means if we need to set "send again indicator"
 * @return
 *     On success, a pointer to arguments data is returned.
 *     On error, NULL is returned.
 */
void *RPC_PreparePktEx(unsigned char *pData, unsigned int data_to_send, CCCI_BUFF_T *ccci_src, unsigned int op_id, unsigned int again)
{
    char *bak_ptr = NULL;
    STREAM_DATA *stream = NULL;

    assert(data_to_send <= MAX_RPC_PKT_BYTE);
    assert(pData != NULL && ccci_src != NULL);
    assert(pData - (sizeof(CCCI_BUFF_T) + sizeof(unsigned int)) >= ccci_src);
        assert(sizeof(g_bak) == (sizeof(CCCI_BUFF_T) + sizeof(unsigned int)));
    // move pointer forward to fill in CCCI header, this will replace orignal data there, so we backup them first
    bak_ptr = (char *)(pData - (sizeof(CCCI_BUFF_T) + sizeof(unsigned int)));
    // backup partial data
    memcpy((void*)g_bak, bak_ptr, sizeof(g_bak));
    stream = (STREAM_DATA *)bak_ptr;
    // copy CCCI header from the very fist header of all sub-packets
    if (again)
        stream->header.data[0] = ccci_src->data[0] | CCCI_RPC_REQ_SEND_AGAIN;
    else
        stream->header.data[0] = ccci_src->data[0] & ~CCCI_RPC_REQ_SEND_AGAIN;
    stream->header.data[1] = data_to_send + sizeof(CCCI_BUFF_T) + sizeof(unsigned int);;
    stream->header.channel = ccci_src->channel;
    stream->header.reserved = ccci_src->reserved;
    stream->payload.rpc_ops_id = op_id;

    //LOGD("RPC_PreparePktEx() CCCI_H(0x%X)(0x%X)(0x%X)(0x%X), OP ID = 0x%X",
    //     stream->header.data[0], stream->header.data[1], stream->header.channel, stream->header.reserved,
    //     stream->payload.OperateID);

    return (void*)stream;
}

/*
 * @brief Determine the prepare data has done
 * @param
 *     pStream [in] A pointer returned from RPC_PreparePktEx()
 * @return
 *     None
 */
void RPC_PreparePktDone(void *pStream)
{
    assert(pStream != NULL);
    // Restore backuped data
    memcpy(pStream, (void*)g_bak, sizeof(g_bak));
}

static bool RPC_WriteToMD(int DeviceFd, int BufIndex, RPC_PACKET_INFO* pPacketSrc, unsigned int PacketNum)
{
    bool bRet = false;
    int ret = 0;
    unsigned char* pData = NULL;
    unsigned int DataLength = 0, AlignLength;
    unsigned int i;
    rpc_stream_buffer_t *pRpcBuf = NULL;
    rpc_stream_msg_t stream_msg;

    int data_len = 0;
    int data_sent = 0;
    STREAM_DATA *buffer_slot = NULL;
    CCCI_BUFF_T *ccci_h = NULL;
    void *pkt_ptr = NULL;
    int pkt_size = 0;
    int data_to_send = 0;
    if(!stream_support) {
        pRpcBuf = (rpc_stream_buffer_t *)((char *)g_RpcInfo.pRpcBuf + (RPC_MAX_BUF_SIZE + sizeof(rpc_stream_msg_t))*BufIndex);
    } else {
        buffer_slot = (STREAM_DATA *)((char *)g_RpcInfo.pRpcBuf + (RPC_MAX_BUF_SIZE + sizeof(STREAM_DATA))*BufIndex);
        pRpcBuf = &buffer_slot->payload;
        DataLength += sizeof(CCCI_BUFF_T);
        DataLength += sizeof(unsigned int); // size of operate ID field
    }
    pRpcBuf->rpc_ops_id = RPC_API_RESP_ID | pRpcBuf->rpc_ops_id;
    pData = (unsigned char* )pRpcBuf->buffer;

    *((unsigned int*)pData) = PacketNum;

    pData += sizeof(unsigned int);
    DataLength += sizeof(unsigned int);

    for(i = 0; i < PacketNum; i++)
    {
        if((DataLength + 2*sizeof(unsigned int) + pPacketSrc[i].Length) > RPC_MAX_BUF_SIZE)
        {
            LOGE("RPCD_WriteToMD: Stream buffer full!!\r\n");
            goto _Exit;
        }
        *((unsigned int*)pData) = pPacketSrc[i].Length;
        pData += sizeof(unsigned int);
        DataLength += sizeof(unsigned int);

        //4 byte aligned
        AlignLength = ((pPacketSrc[i].Length + 3) >> 2) << 2;
        DataLength += AlignLength;

        if(pData != pPacketSrc[i].pData)
            memcpy(pData, pPacketSrc[i].pData, pPacketSrc[i].Length);

        pData += AlignLength;
    }

    stream_msg.length = DataLength;
    stream_msg.index = BufIndex;

    if(!stream_support) {
        msync(pRpcBuf, RPC_MAX_BUF_SIZE, MS_SYNC);
        ret = ioctl(DeviceFd, CCCI_RPC_IOCTL_SEND, &stream_msg);
        if(ret < 0) {
            LOGE("WriteToMD: [error]fail send RPC stream: %d \n", errno);
            return bRet;
        }
    } else {
        // data length excluding CCCI header and OP ID
        data_len = DataLength - sizeof(CCCI_BUFF_T) - sizeof(unsigned int);
        ccci_h = &buffer_slot->header;
                ccci_h->channel++; //Rx->Tx

        /* No fragment is needed */
        if (data_len <= MAX_RPC_PKT_BYTE) {
            pData = (unsigned char *)buffer_slot;
            // Clear "send again indicator"
            ccci_h->data[0] = ccci_h->data[0] & ~CCCI_RPC_REQ_SEND_AGAIN;
                        ccci_h->data[1] = DataLength;
            ret = write(DeviceFd, pData, DataLength);
            if (ret != (int)DataLength) {
                LOGE("Failed to write only one RPC packet(%d)!! (%d/%d)\n", DataLength, ret, errno);
                return bRet;
            }
            //LOGD("Write %d bytes to slot %d, CCCI_H(0x%X)(0x%X)(0x%X)(0x%X)\n",
                 //ret, BufIndex, ccci_h->data[0], ccci_h->data[1], ccci_h->channel, ccci_h->reserved);
        } else {
            /* Data fragment is needed */
            //LOGD("Big packet, need fragment.");
            pData = (unsigned char *)(&buffer_slot->payload.buffer);
            while ((unsigned int)(data_sent + sizeof(CCCI_BUFF_T) + sizeof(unsigned int)) < DataLength) {
                /* Moret than 2 packets to send */
                /* Each packet includes CCCI header, OP id, and data */
                if ((data_len - data_sent) > MAX_RPC_PKT_BYTE) {
                    data_to_send = MAX_RPC_PKT_BYTE;
                    pkt_ptr = RPC_PreparePktEx(pData, data_to_send, ccci_h, pRpcBuf->rpc_ops_id, 1);
                } else {
                    /* The last packet */
                    data_to_send = data_len - data_sent;
                    pkt_ptr = RPC_PreparePktEx(pData, data_to_send, ccci_h, pRpcBuf->rpc_ops_id, 0);
                }
                // Add CCCI header and operation ID size to packet size, be aware of that OP_ID is not cosindered as payload, so not counted in MAX_RPC_PKT_BYTE
                pkt_size = data_to_send + sizeof(CCCI_BUFF_T) + sizeof(unsigned int);
                // write size = data + CCCI header + OP ID
                ret = write(DeviceFd, pkt_ptr, pkt_size);
                if (ret != pkt_size) {
                    LOGE("Failed to write RPC packet !! (%d)\n", errno);
                    break;
                } else {
                    CCCI_BUFF_T *dst_ccci_h = (CCCI_BUFF_T *)pkt_ptr;
                    LOGD("Write %d bytes to slot %d, CCCI_H(0x%X)(0x%X)(0x%X)(0x%X)\n",
                         ret, BufIndex,
                         dst_ccci_h->data[0], dst_ccci_h->data[1], dst_ccci_h->channel, dst_ccci_h->reserved);
                }
                RPC_PreparePktDone(pkt_ptr);
                data_sent += data_to_send;
                pData += data_to_send;
            };
        }
        if (ret < 0) {
            LOGE("WriteToMD: [error]fail send RPC stream: %d \n", ret);
            return bRet;
        }
        //LOGD("write to MD %d\n", DataLength);
    }
    bRet = true;

_Exit:
    return bRet;
}

/***************************************************************************
 * Support lib Section
 ***************************************************************************/

/* tc1 lib --------------------------------------------------------------------------------------- */
#define TC1_SUPPORT_LIB_PATH    "/vendor/lib/libccci_tc1_srv.so"
static void *tc1_support_lib = NULL;
static int tc1_lib_ready = 0;
int (*tc1_rpc_srv_entry)(int ops_id, RPC_PACKET_INFO *packet_info, int *packet_num, int buf[]);
int (*tc_lib_init)(void);

int load_support_lib(void)
{
    const char *error = NULL;

    /* Tc1 section */
    tc1_support_lib = dlopen(TC1_SUPPORT_LIB_PATH, RTLD_NOW);
    if(NULL == tc1_support_lib){
        LOGD("TC1 lib not support for current project!(%s)\n", dlerror());
        return -1;
    }
    error = dlerror(); /* to clear previous error msg */
    tc_lib_init = dlsym(tc1_support_lib, "tc1_lib_init");
    error = dlerror();
    if (NULL != error) {
        LOGE("Load tc1_lib_init api fail!!(%s)\n", error);
        dlclose(tc1_support_lib);
        return -1;
    }
    tc1_rpc_srv_entry = dlsym(tc1_support_lib, "tc1_rpc_srv_entry");
    error = dlerror();
    if (NULL != error) {
        LOGE("Load tc1_rpc_srv_entry api fail!!(%s)\n", error);
        dlclose(tc1_support_lib);
        return -1;
    }
    if (tc_lib_init() < 0) {
        LOGE("tc1_rpc_lib init fail!!\n");
        dlclose(tc1_support_lib);
        return -1;
    }
    tc1_lib_ready = 1;

    return 0;
}

static int tc1_srv_check(int ops_id, RPC_PACKET_INFO *packet_info, int *packet_num, int buf[])
{
    if (tc1_lib_ready)
        return tc1_rpc_srv_entry(ops_id, packet_info, packet_num, buf);

    return -1;
}


static int exit_signal = 0;
void signal_treatment(int param)
{
    /*
     * this signal catching design does NOT work...
     * set property ctl.stop will send SIGKILL to ccci_rpcd(check service_stop_or_reset() in init.c),
     * but SIGKILL is not catchable.
     * kill pid will send SIGTERM to ccci_rpcd, we can catch this signal, but the process is just
     * terminated, and no time for us to check exit_signal in main().
     * per system team's comment, kernel will free all resource (memory get from malloc, etc.),
     * so we do NOT need to take care of these.
     */
    LOGD("signal number=%d\n", param);
    switch (param) {
    case SIGPIPE:
    case SIGHUP:
    case SIGINT:
    case SIGTERM:
    case SIGUSR1:
    case SIGUSR2:
    case SIGALRM:
    case SIGKILL:
    default:
        exit_signal = param;
        break;
    }
}

/* amms new feature */
static unsigned char *drdi_addr;
static int drdi_img_offset, drdi_img_size;
static int drdi_info_ready = -1;
static unsigned int set_total_num;
#define IMG_NODE_SIZE        2
static const char *mdimg_node[IMG_NODE_SIZE] = {
    "/dev/disk/by-partlabel/modem",
    "/dev/disk/by-partlabel/md1img"
};

/* bank4 smem size */
#define BANK4_DRDI_SMEM_SIZE (64*1024)

/* copy status */
static unsigned int all_set_copy_done = COPY_FAIL;

/* find md1drdi img offset and size, mmap 512KB memory */
static int ccci_prepare_drdi_info()
{
    const char *img_name = "md1drdi";
    int md_img_fd = -1;

    /* find offset of md1drdi in md image */
    md_img_fd = find_image_from_pt(img_name, &drdi_img_offset, &drdi_img_size);
    if (md_img_fd < 0) {
        LOGE("not find %s in partition\n", img_name);
        return -1;
    }
    if ((drdi_img_size > 512*1024*1024) || (drdi_img_size <= 0)) {
        LOGE("MD image size abnormal %d\n", drdi_img_size);
        return -2;
    }
    LOGD("find img %s (size 0x%x) at 0x%x in partition\n", img_name, drdi_img_size, drdi_img_offset);

    if (DeviceFd < 0) {
        LOGE("%s:DeviceFd(%d) error\n", __func__, DeviceFd);
        return -ENODEV;
    }

    drdi_addr = (unsigned char *)mmap(NULL, BANK4_DRDI_SMEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, DeviceFd, 0);
    if (drdi_addr == MAP_FAILED) {
        LOGE("%s:mmap failed, errno=%d\n", __func__, errno);
        return -EFAULT;
    }
    LOGD("%s:drdi_addr=0x%p\n", __func__, drdi_addr);

    drdi_info_ready = 0;
    return 0;
}

static int amms_cmd_init_handler(struct amms_msg_request *request,
    struct amms_msg_response *reply)
{
    unsigned int i;

    if (!request || !reply) {
        LOGE("%s: invalid input or output\n", __func__);
        return -1;
    }
    if (drdi_info_ready != 0) {
        LOGE("%s: prepare drdi_info fail\n", __func__);
        goto fail;
    }

    reply->seq_id = request->seq_id;
    LOGD("%s:reply seq_id(%d)\n", __func__, reply->seq_id);

    /* check version valid */
    if (request->u_req.init_request_cmd.version == AMMS_V3)
        reply->u_resp.init_reply.version = AMMS_V3;
    else {
        LOGE("%s:version(%d) error, goto fail\n",
            __func__, request->u_req.init_request_cmd.version);
        goto fail;
    }

    /* get set_total_num */
    if (request->u_req.init_request_cmd.set_total_num > DRDI_SET_NUM_TOTAL_SET) {
        LOGE("%s:set_total_num error\n", __func__);
        goto fail;
    }
    set_total_num = request->u_req.init_request_cmd.set_total_num;

    for (i = 0; i < set_total_num; i++) {
        /* check start_addr or length exceed */
        if (request->u_req.init_request_cmd.drdi_info[i].start_address + request->u_req.init_request_cmd.drdi_info[i].length > drdi_img_size) {
            LOGE("%s: cmd invalid: start_addr=0x%x, size=0x%x\n",
                __func__, request->u_req.init_request_cmd.drdi_info[i].start_address, request->u_req.init_request_cmd.drdi_info[i].length);
            reply->u_resp.init_reply.drdi_info_status = -1;
            reply->status = AMMS_STATUS_INVALID;
            return -1;
        }
    }

    /* first boot and copy success --> status no need */
    reply->u_resp.init_reply.drdi_info_status = 0;
    reply->status = AMMS_STATUS_SUCCESS;

    all_set_copy_done = COPY_FAIL;
    reply->u_resp.init_reply.copy_status = 0;

    LOGD("stats=%d, seqid=%d, coypstat=%d, ver=%d,drdiinfostat=%d\n",
        reply->status, reply->seq_id, reply->u_resp.init_reply.copy_status,
        reply->u_resp.init_reply.version, reply->u_resp.init_reply.drdi_info_status);

    return 0;

fail:
    reply->status = AMMS_STATUS_FAIL;
    reply->u_resp.init_reply.version = AMMS_V3;
    reply->u_resp.init_reply.copy_status = 0;
    reply->u_resp.init_reply.drdi_info_status = -1;
    LOGE("%s:init cmd fail\n", __func__);
    return -1;
}

static int amms_cmd_drdi_copy_handler(struct amms_msg_request *request,
    struct amms_msg_response *reply)
{
    int ret, i, has_write = 0;
    int drdi_fd = -1;
    char partition_path[128] = { 0 };
    char buf[128] = { 0 };

    if (!request || !reply) {
        LOGE("%s: invalid input or output\n", __func__);
        return -1;
    }

    reply->seq_id = request->seq_id;
    //LOGD("%s:reply seq_id(%d)\n", __func__, reply->seq_id);

    for (i = 0; i < set_total_num; i++) {
        if (request->u_req.drdi_cpy_cmd.copy_info[i].length == 0)
            continue;
        if (request->u_req.drdi_cpy_cmd.copy_info[i].length > BANK4_DRDI_SMEM_SIZE ||
            request->u_req.drdi_cpy_cmd.copy_info[i].src_offset + request->u_req.drdi_cpy_cmd.copy_info[i].length > drdi_img_size) {
            LOGD("set(%d) error,src=0x%x,dst=0x%x,len=0x%x\n", i, request->u_req.drdi_cpy_cmd.copy_info[i].src_offset,
                request->u_req.drdi_cpy_cmd.copy_info[i].dst_offset, request->u_req.drdi_cpy_cmd.copy_info[i].length);
            goto fail;
        }
    }

    if (all_set_copy_done == COPY_DONE) {
        LOGD("%s:already copy success, no need copy\n", __func__);
        reply->status = AMMS_STATUS_SUCCESS;
        return 0;
    }

    AB_image_get(buf);

    for (i = 0; i < IMG_NODE_SIZE; i++) {
        has_write = snprintf(partition_path, 128, "%s%s", mdimg_node[i], buf);
        if (has_write < 0 || has_write >= 128) {
            LOGE("[%s] %d:get mdimg node[%d] fail,has_write = %d\n",
                __func__, __LINE__, i, has_write);
            continue;
        }
        drdi_fd = open(partition_path, O_RDONLY);
        if (drdi_fd < 0) {
            LOGE("%s:open drdi img(%s) fail\n", __func__, partition_path);
            continue;
        } else
            break;
    }
    if ( i >= IMG_NODE_SIZE) {
        LOGE("[%s] %d:open drdi imgfail, cmd exit\n", __func__, __LINE__);
        goto fail;
    }

    reply->u_resp.cpy_reply.last_cpy_status =
        request->u_req.drdi_cpy_cmd.last_cpy == 1 ? 0 : -1;

    for (i = 0; i < set_total_num; i++) {
        if (request->u_req.drdi_cpy_cmd.copy_info[i].length == 0)
            continue;
        if (lseek(drdi_fd, (off_t)request->u_req.drdi_cpy_cmd.copy_info[i].src_offset + drdi_img_offset, SEEK_SET) != -1) {
                ret = read(drdi_fd, drdi_addr + request->u_req.drdi_cpy_cmd.copy_info[i].dst_offset,
                    (size_t)request->u_req.drdi_cpy_cmd.copy_info[i].length);
                if (ret < 0) {
                    LOGE("read set(%d)fail,err=%d,ret=%d,src=0x%x,dst=0x%x,len=0x%x\n", i, errno, ret, request->u_req.drdi_cpy_cmd.copy_info[i].src_offset,
                        request->u_req.drdi_cpy_cmd.copy_info[i].dst_offset, request->u_req.drdi_cpy_cmd.copy_info[i].length);
                    close(drdi_fd);
                    goto fail;
                }
                all_set_copy_done = request->u_req.drdi_cpy_cmd.last_cpy == 1 ? COPY_DONE : COPY_GOING;
                LOGD("copy set(%d) from 0x%x to 0x%x,len=0x%x success\n", i, request->u_req.drdi_cpy_cmd.copy_info[i].src_offset,
                    (unsigned int)(drdi_addr + request->u_req.drdi_cpy_cmd.copy_info[i].dst_offset), request->u_req.drdi_cpy_cmd.copy_info[i].length);
        } else {
            all_set_copy_done = COPY_FAIL;
            LOGE("lseek set(%d) fail,err=%d,src=0x%x,dst=0x%x,len=0x%x\n", i, errno, request->u_req.drdi_cpy_cmd.copy_info[i].src_offset,
                request->u_req.drdi_cpy_cmd.copy_info[i].dst_offset, request->u_req.drdi_cpy_cmd.copy_info[i].length);
            close(drdi_fd);
            goto fail;
        }
    }

    /* lseek fail, copy fail or all copy_info.length==0, must reply fail and close fd */
    reply->status = (all_set_copy_done == COPY_FAIL) ? AMMS_STATUS_FAIL : AMMS_STATUS_SUCCESS;
    close(drdi_fd);

    return 0;

fail:
    all_set_copy_done = COPY_FAIL;
    LOGE("%s:copy fail\n", __func__);
    reply->status = AMMS_STATUS_FAIL;
    return -1;
}

int ccci_rpc_amms_drdi_ctl(struct amms_msg_request *request,
    struct amms_msg_response *reply)
{
    int ret;

    switch (request->cmd) {
    case AMMS_CMD_INIT:
        ret = amms_cmd_init_handler(request, reply);
        break;
    case AMMS_CMD_DRDI_COPY:
        ret = amms_cmd_drdi_copy_handler(request, reply);
        break;
    default:
        ret = -1;
        reply->status = AMMS_STATUS_FAIL;
        reply->seq_id = request->seq_id;
        LOGE("%s:%d unknown cmd:%u\n", __func__, __LINE__, request->cmd);
        break;
    }

    //LOGD("%s:req:%u done, ret=%d\n", __func__, request->cmd, ret);

    return ret;
}

#define CCCI_SIG "CCCIRPC"
#define MISC_PART "dev/block/by-name/para"
#define MISC_PART_DATA_OFFSET (0x28000)

struct ccci_rpc_s {
    char sign[8]; // "CCCIRPC"
    unsigned int md_capid;
    unsigned int md_aac;
    char sign_1[8];  //"CCCIRPC"
};

int ccci_rpc_mdcap_aac_ctl(struct Md_Rpc_Capid_Aac *input,
        int *reply)
{
    int ret, fd;
    struct ccci_rpc_s ccci_data = {0};

    if (input == NULL || reply == NULL)
        return -1;

    *reply = -1;

    strncpy(ccci_data.sign, CCCI_SIG, sizeof(CCCI_SIG));
    strncpy(ccci_data.sign_1, CCCI_SIG, sizeof(CCCI_SIG));

    ccci_data.md_capid = input->capid;
    ccci_data.md_aac = input->aac;
    LOGD("%s:md_capid 0x%x md_aac 0x%x\n", __func__, ccci_data.md_capid, ccci_data.md_aac);

    fd = open(MISC_PART, O_RDWR);
    if (fd < 0) {
        LOGE("failed to get device path by mount point %s fd=%d", MISC_PART, fd);
        return -1;
    }

    if (lseek(fd, MISC_PART_DATA_OFFSET, SEEK_SET) != MISC_PART_DATA_OFFSET) {
        LOGE("seek to %ld fail: %s\n", MISC_PART_DATA_OFFSET, strerror(errno));
        goto fail;
    }

    ret = write(fd, (char *)&ccci_data, sizeof(struct ccci_rpc_s));
    if (ret < 0 || ret != sizeof(struct ccci_rpc_s)) {
        LOGE("write fail ret=%d, err=%s\n", ret, strerror(errno));
        goto fail;
    }

    *reply = 0;

fail:
    close(fd);

    return 0;
}

int main(int argc, char *argv[])
{
    int ReqBufIndex = 0;
    rpc_stream_buffer_t *pRpcBuf = NULL;
    int PacketNum = 0;
    int RetVal;
    char dev_node[32];
    RPC_PACKET_INFO *PackInfo = NULL;
    CCCI_BUFF_T *ccci_h = NULL;
    char pkt_buff[MAX_RPC_BUF_BYTE] = {0};
    STREAM_DATA *stream = NULL; // data packet received from MD
    STREAM_DATA *buffer_slot = NULL; // local buffer slot
    char *p_rpc_buff = NULL;
    char property_val[PROPERTY_VALUE_MAX] = {0};
    int tmp_buff[2];
    int sar_tbl_idx = 0;
    unsigned int cpy_size = 0;
    struct amms_msg_request *input = NULL;
    struct amms_msg_response *output = NULL;
    struct Md_Rpc_Capid_Aac *md_cap_aac = NULL;
    int md_cap_aac_ret = -1;

    LOGD("ccci_rpcd Ver:v2.00, CCCI Ver:%d", ccci_get_version());
    //Check if input parameter is valid
    if(argc != 2) {
        md_id = 0;
        LOGE("[Warning]Parameter number not correct,use old version!\n");
        snprintf(dev_node, 32, "/dev/ccci_rpc");
    } else {
        if(strcmp(argv[1],"0")==0) {
            snprintf(dev_node, 32, "%s", ccci_get_node_name(USR_CCCI_RPC, MD_SYS1));
            md_id = 0;
        } else if(strcmp(argv[1],"1")==0) {
            snprintf(dev_node, 32, "%s", ccci_get_node_name(USR_CCCI_RPC, MD_SYS2));
            md_id =1;
        } else if(strcmp(argv[1],"4")==0) {
            snprintf(dev_node, 32, "%s", ccci_get_node_name(USR_CCCI_RPC, MD_SYS5));
            md_id =4;
        } else {
            LOGD("Invalid md sys id(%d)!\n", md_id);
            return -1;
        }
    }
    if(md_id==0 || md_id==1) {
        if(ccci_get_version() == ECCCI || ccci_get_version() == EDSDA || ccci_get_version() == ECCCI_FSM)
            stream_support = 1;
        else
            stream_support = 0;
    } else if(md_id == 4) {
        stream_support = 1;
    }

    DeviceFd = open(dev_node, O_RDWR);
    if(DeviceFd == -1)
    {
        LOGE("Main: open ccci_rpc fail\r\n");
        return -1;
    }
    LOGD("%s:dev_node=%s,devicefd= %d\n", __func__, dev_node, DeviceFd);

    if(!stream_support) {
        g_RpcInfo.pRpcBuf = mmap(NULL, sizeof(rpc_stream_buffer_t), PROT_READ | PROT_WRITE, MAP_SHARED, DeviceFd, 0);
    } else {
        int alloc_length = (sizeof(STREAM_DATA) + RPC_MAX_BUF_SIZE) * RPC_BUFFER_SLOT_NUM;
        g_RpcInfo.pRpcBuf = malloc(alloc_length);
        if(g_RpcInfo.pRpcBuf  == NULL)
        {
            LOGE("Main: malloc buffer fail\r\n");
            return -1;
        }
        memset(g_RpcInfo.pRpcBuf, 0, alloc_length);
    }
    PackInfo = malloc(sizeof(RPC_PACKET_INFO) * RPC_MAX_ARG_NUM);
    if (PackInfo == NULL) {
        LOGE("Main: PackInfo fail\r\n");
        return -1;
    }

    /* move from while loop, only malloc once */
    output = malloc(sizeof(struct amms_msg_response));
    if (output == NULL) {
        LOGE("Main: amms_msg_response fail\r\n");
        free(PackInfo);
        return -1;
    }

    LOGD("register signal hadler\n");
    if(signal(SIGHUP, signal_treatment)==SIG_ERR)
        LOGE("can't catch SIGHUP\n");
    if(signal(SIGPIPE, signal_treatment)==SIG_ERR)
        LOGE("can't catch SIGPIPE\n");
    if(signal(SIGINT, signal_treatment)==SIG_ERR)
        LOGE("can't catch SIGINT\n");
    if(signal(SIGUSR1, signal_treatment)==SIG_ERR)
        LOGE("can't catch SIGUSR1\n");
    if(signal(SIGUSR2, signal_treatment)==SIG_ERR)
        LOGE("can't catch SIGUSR2\n");
    if(signal(SIGTERM, signal_treatment)==SIG_ERR)
        LOGE("can't catch SIGTERM\n");
    if(signal(SIGALRM, signal_treatment)==SIG_ERR)
        LOGE("can't catch SIGALRM\n");

    load_support_lib();

    LOGD("%s:start find md1drdi image\n", __func__);
    ccci_prepare_drdi_info();

    while(exit_signal == 0)
    {
        PacketNum = 0;
retry:
        if(!stream_support) {
            ReqBufIndex = ioctl(DeviceFd, CCCI_RPC_IOCTL_GET_INDEX, 0);
            RPC_WAKE_LOCK();

            if(ReqBufIndex < 0 || ReqBufIndex > RPC_REQ_BUFFER_MUN)
            {
                LOGE("Main: [error]fail get CCCI_RPC buffer index: %d \n", errno);
                RetVal = RPC_PARAM_ERROR;
                PackInfo[PacketNum].Length = sizeof(unsigned int);
                PackInfo[PacketNum++].pData = (void*) &RetVal;
                goto _Next;
            }

            pRpcBuf = (rpc_stream_buffer_t *)((char *)g_RpcInfo.pRpcBuf + (RPC_MAX_BUF_SIZE + sizeof(rpc_stream_buffer_t))*ReqBufIndex);
        } else {
            while (1) {
                memset(pkt_buff, 0, MAX_RPC_BUF_BYTE);
                // add an extra integer as MD consider OP_ID as not part of the "payload"
                RetVal = read(DeviceFd, pkt_buff, (MAX_RPC_PKT_BYTE+sizeof(CCCI_BUFF_T)+sizeof(unsigned int)));
                if (RetVal <= 0) {
                    LOGE("Failed to read from RPC device (%d) !! errno = %d", RetVal, errno);
                    goto retry;
                }

                //LOGD("Read %d bytes from RPC device", RetVal);

        RPC_WAKE_LOCK();
                stream = (STREAM_DATA *)pkt_buff;
                ccci_h = (CCCI_BUFF_T *)&stream->header;
                ReqBufIndex = ccci_h->reserved;
                if (ReqBufIndex >= RPC_BUFFER_SLOT_NUM || ReqBufIndex < 0){
                    LOGD("invalid ReqBufIndex:%d\n", ReqBufIndex);
                    RPC_WAKE_UNLOCK();
                    free(PackInfo);
                    free(output);
                    return -1;
                }

                //LOGD("Read %d bytes from slot %d, CCCI_H(0x%X)(0x%X)(0x%X)(0x%X)",
                     //RetVal, ReqBufIndex, ccci_h->data[0], ccci_h->data[1], ccci_h->channel, ccci_h->reserved);

                buffer_slot = (STREAM_DATA *)((char *)g_RpcInfo.pRpcBuf + (RPC_MAX_BUF_SIZE + sizeof(STREAM_DATA))*ReqBufIndex);
                p_rpc_buff = (char *)buffer_slot;
                /******************************************
                 *
                 *  FSM description for re-sent mechanism
                 *   (ccci_rpc_buff_state == CCCI_RPC_BUFF_IDLE) ==> initial status & end status
                 *   (ccci_rpc_buff_state == CCCI_RPC_BUFF_WAIT) ==> need to receive again
                 *
                 ******************************************/
                if (!CCCI_RPC_PEER_REQ_SEND_AGAIN(ccci_h)) {
                    if (g_RpcInfo.rpc_buff_state[ReqBufIndex] == RPC_BUFF_IDLE) {
                        /* copy data memory and CCCI header */
                        LOGD("0 Read %d bytes from slot %d, CCCI_H(0x%X)(0x%X)(0x%X)(0x%X), copy from %p to %p(%p, %p)\n",
                             RetVal, ReqBufIndex, ccci_h->data[0], ccci_h->data[1], ccci_h->channel,
                             ccci_h->reserved, ccci_h, p_rpc_buff, g_RpcInfo, g_RpcInfo.pRpcBuf);
                        memcpy(p_rpc_buff, ccci_h, ccci_h->data[1]);
                        /* don't need to update FS_Address */
                    } else if (g_RpcInfo.rpc_buff_state[ReqBufIndex] == RPC_BUFF_WAIT) {
                        /* copy data memory and NULL, excluding CCCI header, OP id */
                        if (ccci_h->data[1] < (sizeof(CCCI_BUFF_T) + sizeof(unsigned int))) {
                            /* data send from modem abnormal */
                            assert(0);
                        } else {
                            cpy_size = ccci_h->data[1] - sizeof(CCCI_BUFF_T) - sizeof(unsigned int);
                            LOGD("0 Read %d bytes from slot %d, CCCI_H(0x%X)(0x%X)(0x%X)(0x%X), copy from %p/%p to %p(%p, %p)\n",
                                 RetVal, ReqBufIndex, ccci_h->data[0], ccci_h->data[1], ccci_h->channel,
                                 ccci_h->reserved, ccci_h, stream->payload.buffer, p_rpc_buff, g_RpcInfo, g_RpcInfo.pRpcBuf);

                            memcpy(p_rpc_buff + g_RpcInfo.rpc_buff_offset[ReqBufIndex],
                                 stream->payload.buffer, cpy_size);
                            /* update CCCI header info */
                            memcpy(p_rpc_buff, ccci_h, sizeof(CCCI_BUFF_T));
                        }
                    } else {
                        /* No such rpc_buff_state state */
                        assert(0);
                    }
                    g_RpcInfo.rpc_buff_state[ReqBufIndex] = RPC_BUFF_IDLE;
                    g_RpcInfo.rpc_buff_offset[ReqBufIndex] = 0;
                } else {
                    if (g_RpcInfo.rpc_buff_state[ReqBufIndex] == RPC_BUFF_IDLE) {
                        /* only "OP id" and "data" size and "CCCI header" */
                        unsigned int length = ccci_h->data[1];
                        LOGD("1 Read %d bytes from slot %d, CCCI_H(0x%X)(0x%X)(0x%X)(0x%X), copy from %p to %p(%p, %p)\n",
                             RetVal, ReqBufIndex, ccci_h->data[0], ccci_h->data[1], ccci_h->channel,
                             ccci_h->reserved, ccci_h, p_rpc_buff, g_RpcInfo, g_RpcInfo.pRpcBuf);

                        memcpy(p_rpc_buff, ccci_h, length);
                        g_RpcInfo.rpc_buff_offset[ReqBufIndex] += length;
                    } else if (g_RpcInfo.rpc_buff_state[ReqBufIndex] == RPC_BUFF_WAIT) {
                        /* only "data" size, excluding CCCI header and OP id */
                        unsigned int length = ccci_h->data[1] - sizeof(CCCI_BUFF_T) - sizeof(unsigned int);
                        LOGD("1 Read %d bytes from slot %d, CCCI_H(0x%X)(0x%X)(0x%X)(0x%X), copy from %p/%p to %p(%p, %p)\n",
                             RetVal, ReqBufIndex, ccci_h->data[0], ccci_h->data[1], ccci_h->channel,
                             ccci_h->reserved, ccci_h, stream->payload.buffer, p_rpc_buff, g_RpcInfo, g_RpcInfo.pRpcBuf);

                        memcpy(p_rpc_buff + g_RpcInfo.rpc_buff_offset[ReqBufIndex],
                             stream->payload.buffer,
                             length);    /* CCCI_HEADER + RPC_OP_ID */
                        g_RpcInfo.rpc_buff_offset[ReqBufIndex] += length;
                    } else {
                        /* No such ccci_rpc_buff_state state */
                        assert(0);
                    }
                    g_RpcInfo.rpc_buff_state[ReqBufIndex] = RPC_BUFF_WAIT;
                }
                if (g_RpcInfo.rpc_buff_state[ReqBufIndex] == RPC_BUFF_IDLE)
                    break;
                RPC_WAKE_UNLOCK();
            }
            pRpcBuf = &buffer_slot->payload;
        }
        //LOGD("Main: operation ID = %x\n", pRpcBuf->OperateID);
        if(!RPC_GetPackInfo(PackInfo, pRpcBuf->buffer))
        {
            LOGE("Main: Fail to get packet info!! \r\n");
            RetVal = RPC_PARAM_ERROR;
            PackInfo[PacketNum].Length = sizeof(unsigned int);
            PackInfo[PacketNum++].pData = (void*) &RetVal;
            goto _Next;
        }

        switch(pRpcBuf->rpc_ops_id)
        {
        case IPC_RPC_QUERY_AP_SYS_PROPERTY:
            {
            int property_len;
            char *property_name = (char*)PackInfo[0].pData;

            property_name[PackInfo[0].Length] = 0;
            RetVal = mtk_property_get(property_name, property_val, NULL);
            LOGD("Main: IPC_RPC_QUERY_AP_SYS_PROPERTY, key<%s>, value<%s>, %d\n", property_name, property_val, RetVal);
            property_len = strlen(property_val) + 1;
            PackInfo[PacketNum].Length = sizeof(int);
            PackInfo[PacketNum++].pData = (void*) &RetVal;
            PackInfo[PacketNum].Length = property_len;
            PackInfo[PacketNum++].pData = &property_val;
            }
            break;
        case IPC_RPC_SAR_TABLE_IDX_QUERY_OP:
            {
            RetVal = mtk_sar_table_id_get(&sar_tbl_idx);
            if (!RetVal)
                LOGE("Main: IPC_RPC_SAR_TABLE_IDX_QUERY_OP:mtk_sar_table_id_get fail\n");
            LOGD("Main: IPC_RPC_SAR_TABLE_IDX_QUERY_OP, value: %d, ret: %d\n",sar_tbl_idx, RetVal);
            PackInfo[PacketNum].Length = sizeof(int);
            PackInfo[PacketNum++].pData = (void*) &RetVal;
            PackInfo[PacketNum].Length = sizeof(int);
            PackInfo[PacketNum++].pData = (void*) &sar_tbl_idx;
            }
            break;
        case IPC_RPC_AMMS_DRDI_CONTROL:
            {
            unsigned int pkt_size;

            memset(output, 0x0, sizeof(struct amms_msg_response));
            pkt_size = PackInfo[0].Length;

            if (pkt_size == sizeof(struct amms_msg_request)) {
                input = (struct amms_msg_request *)(PackInfo[0].pData);
                RetVal = ccci_rpc_amms_drdi_ctl(input, output);
            } else {
                LOGE("Main:can't recognize rpc pkt size:%d!\n", pkt_size);
                RetVal = -1;
            }
            PackInfo[PacketNum].Length = sizeof(int);
            PackInfo[PacketNum++].pData = (void*) &RetVal;
            PackInfo[PacketNum].Length = sizeof(struct amms_msg_response);
            PackInfo[PacketNum++].pData = output;
            }
        break;
        case IPC_RPC_SAVE_MD_CAPID:
            {
            unsigned int pkt_size;

            pkt_size = PackInfo[0].Length;

            if (pkt_size == sizeof(struct Md_Rpc_Capid_Aac)) {
                md_cap_aac = (struct Md_Rpc_Capid_Aac *)(PackInfo[0].pData);
                RetVal = ccci_rpc_mdcap_aac_ctl(md_cap_aac, &md_cap_aac_ret);
            } else {
                LOGE("Main:can't recognize rpc pkt size:%d!\n", pkt_size);
                RetVal = -1;
            }
            PackInfo[PacketNum].Length = sizeof(int);
            PackInfo[PacketNum++].pData = (void*) &RetVal;
            PackInfo[PacketNum].Length = sizeof(int);
            PackInfo[PacketNum++].pData = (void*) &md_cap_aac_ret;
            }
        break;
        default:
            if (tc1_srv_check(pRpcBuf->rpc_ops_id, PackInfo, &PacketNum, tmp_buff) == 0)
                break;
            else {
                LOGE("Main: Unknow RPC Operation ID (0x%x)\n", pRpcBuf->rpc_ops_id);
                RetVal = RPC_PARAM_ERROR;
                PackInfo[PacketNum].Length = sizeof(int);
                PackInfo[PacketNum++].pData = (void*) &RetVal;
                break;
            }
        }
_Next:
        if(!RPC_WriteToMD(DeviceFd, ReqBufIndex, PackInfo, PacketNum))
        {
            LOGE("Main: fail to write packet!!\r\n");
//            return -1;
        }
        RPC_WAKE_UNLOCK();
    }
    LOGD("ccci_rpcd exit, free buffer\n");
    close(DeviceFd);
    free(PackInfo);
    if(stream_support)
        free(g_RpcInfo.pRpcBuf);
    dlclose(tc1_support_lib);
    if (output != NULL)
        free(output);
    return 0;
}
