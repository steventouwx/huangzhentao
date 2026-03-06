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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
//#include <android/log.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include "ccci_log.h"
#include "platform/mdinit_ccci_log.h"

#define CCCI_PRINT_BUF_SIZE        4096

void ccci_log_print(int lvl, int md_id, const char *fmt, ...)
{
    va_list args;
    char *temp_log;
    int ret;

    temp_log = (char*)malloc(CCCI_PRINT_BUF_SIZE);
    if (temp_log == NULL)
        return;

    memset(temp_log, 0, CCCI_PRINT_BUF_SIZE);
    ret = snprintf(temp_log, CCCI_PRINT_BUF_SIZE, "(%d):", md_id);
    if (ret <= 0)
        goto _Exit;

    //(args, fmt);
    ret = vsnprintf(&temp_log[ret], CCCI_PRINT_BUF_SIZE - ret, fmt, args);
    if (ret < 0 || ret >= (CCCI_PRINT_BUF_SIZE - ret)) {
        //va_end(args);
        goto _Exit;
    }
    //va_end(args);

    /*switch(lvl) {
    case LV_V:
        LOG_PRINT_V(temp_log, NULL);
        break;
    case LV_D:
        LOG_PRINT_D(temp_log, NULL);
        break;
    case LV_I:
        LOG_PRINT_I(temp_log, NULL);
        break;
    case LV_W:
        LOG_PRINT_W(temp_log, NULL);
        break;
    case LV_E:
        LOG_PRINT_E(temp_log, NULL);
        break;
    default:
        LOG_PRINT_D(temp_log, NULL);
        break;
    }*/

_Exit:
    free(temp_log);
}


