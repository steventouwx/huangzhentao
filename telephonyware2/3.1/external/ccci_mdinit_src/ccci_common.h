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


#ifndef __CCCI_SRV_COMMON_H__
#define __CCCI_SRV_COMMON_H__

#include "ccci_log.h"
#include <stdbool.h>
#include "platform/mdinit_relate.h"

#define DB_OPT_FTRACE   (0)

#define SBP_FLAG_UPDATED 1 // updated, need not send to md, 0: the latest setting, need update
// SBP: Single Binary Platfor, used for modem customization
typedef struct
{
    unsigned int md_sbp_code;     // bit defined by modem;
    unsigned int flag_updated;    // 0: not updated, needn't send to md, 1:updated, need send to md
    unsigned int md2_sbp_code;     // bit defined by modem 2;
    unsigned int flag2_updated;    // 0: not updated, needn't send to md, 1:updated, need send to md
    unsigned int md3_sbp_code;     // bit defined by modem 3;
    unsigned int flag3_updated;    // 0: not updated, needn't send to md, 1:updated, need send to md
} MD_SBP_Struct;

#define CFG_FILE_MD_SBP_CONFIG_SIZE    sizeof(MD_SBP_Struct)
#define CFG_FILE_MD_SBP_CONFIG_TOTAL   1


extern int curr_md_id;
extern int  system_ch_handle;
extern const char md_img_folder[32];
extern const char md_img_cip_folder[32];

int is_factory_mode(void);
int is_meta_mode(void);

int is_current_md_en(int md_id);
int get_ccci_drv_ver(void);
int kernel_setting_prepare(void);
void depends_so_prepare(void);
void depends_so_free(void);
int show_aee_system_exception(const char *module, const char *path, unsigned int flag, const char* msg, ...);
int check_lk_load_md_status(int md_id);
//int check_decrypt_ready(void);
int md_image_exist_check(int fd,int md_id);
//int wait_for_property(const char *name, const char *desired_value, int waitmsec);
unsigned int str2uint(const char *str);
int executeCmd(char * cmd, bool print);

int main_v1(int md_id, int sub_ver);
int main_v2(int md_id, int sub_ver);
int md_image_dep_check(int md_id);
void write_ccci_dump(const char* cmd, ...);

#endif
