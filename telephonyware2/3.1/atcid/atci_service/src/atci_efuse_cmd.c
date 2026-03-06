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
#include "atci_efuse_cmd.h"
#include "atci_service.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HUID_FILE_PATH "/data/vendor/thinmd/huid"

#define MAX_HRID_SIZE  256

void print_msg(char *buffer, char *msg) {
    int ret = 0;

    ret = sprintf(buffer, "%s\r\n", msg);
    if (ret < 0)
        ALOGE("sprint function issue:%d\n", errno);
}

int read_huid_from_file(char *huid) {
    FILE *fd = NULL;
    int len = 0;

    memset(huid, 0, MAX_HRID_SIZE);
    if ((fd = fopen(HUID_FILE_PATH, "r")) == NULL) {
        ALOGE("open file: %s. error:%d\n", HUID_FILE_PATH, errno);
        print_msg(huid, "File open issue\r\n");
        return -1;
    }

    len = fread(huid, sizeof(char), MAX_HRID_SIZE, fd);
    if (len < 0) {
        ALOGE("fread error: %d\n", errno);
        print_msg(huid, "File read issue\r\n");
        if (fclose(fd))
            ALOGE("fclose error:%d\n", errno);
        return -1;
    }
    ALOGD("HUID INFO:%s\n", huid);
    if (fclose(fd))
        ALOGE("fclose error:%d\n", errno);

    return 0;
}

int efuse_huid_cmd_handler(char *cmdline, ATOP_t at_op, char *response) {
    int ret = 0;
    char huid[MAX_HRID_SIZE];

    ALOGD("EFuse cmdline=%s, at_op=%d, \n", cmdline, at_op);

    switch (at_op) {
        case AT_ACTION_OP:
        case AT_READ_OP:
            ret = read_huid_from_file(huid);
            if (!ret)
                ret = sprintf(response, "HUID: %s\r\n", huid);
            else
                ret = sprintf(response, "ERROR: %s\r\n", huid);
            if (ret < 0) {
                ALOGE("Error in sprintf:%d\n", errno);
                return -1;
            }
            ALOGD("Command is executed done\n");
            return 0;
            break;

        case AT_TEST_OP:
        case AT_SET_OP:
            break;

        default:
            break;
    }

    return 0;
}
