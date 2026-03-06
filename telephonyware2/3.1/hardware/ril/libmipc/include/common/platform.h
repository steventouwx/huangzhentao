/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  Copyright (c) [2020], MediaTek Inc. All rights reserved.
*  This software/firmware and related documentation ("MediaTek Software") are
*  protected under relevant copyright laws.
*
*  The information contained herein is confidential and proprietary to
*  MediaTek Inc. and/or its licensors. Except as otherwise provided in the
*  applicable licensing terms with MediaTek Inc. and/or its licensors, any
*  reproduction, modification, use or disclosure of MediaTek Software, and
*  information contained herein, in whole or in part, shall be strictly
*  prohibited.
*****************************************************************************/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stdint.h>
#include <limits.h>

#if (__WORDSIZE == 64)
#define BUILD_64 (1)
#endif

//we don't use long double, so the largest base data type size will be 8 bytes
//aligned to 2^3 = 8
#define ALIGN_BITS (3)
#define ALIGN_SIZE (1 << ALIGN_BITS)
#define ALIGN(x) ((((x) + (ALIGN_SIZE - 1)) >> ALIGN_BITS) << ALIGN_BITS)

#if (BUILD_64 == 1)
#define POINTER_TO_UINT(x) ((unsigned long long)(x))
#else
#define POINTER_TO_UINT(x) ((unsigned long)(x))
#endif

#ifndef offsetof
#define offsetof(t, m) ((size_t)&((t *)0)->m)
#endif
#include "platform_mtk.h"

#endif
