// SPDX-License-Identifier: MediaTekProprietary
/*
 * DcFailCause.h
 *
 *  Created on: Apr 28, 2017
 *      Author: mtk54458
 */

#ifndef DCFAIL_H_
#define DCFAIL_H_

#include "../include/telephony/ril.h"
#include "Common.h"

class DcFail {
public:
    DcFail();
    DcFail(int dcFailCause);
    bool isPermanentFail();

    RIL_DataCallFailCause mDcFailCause;
};

#endif /* DCFAIL_H_ */
