/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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
/*
 * File name:  rfx_controller_factory.cpp
 * Author: Jun Liu (MTK80064)
 * Description:
 *  Implementation of controller factory.
 */

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxControllerFactory.h"
#include "RfxController.h"
#include "RfxRootController.h"
#include "RfxSlotRootController.h"
#include "RfxOpUtils.h"
#include "RfxLog.h"
#include "RfxRilAdapter.h"
#include "mtk_properties.h"
#include "RfxRilUtils.h"
//#include "el/RtcElController.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
#define RFX_LOG_TAG "RfxContFactory"

#define RFX_CONTROLLER_CREATION_ENTRY(_className) \
        {_className::createInstance, _className::getClassInfoInstance, false}

Vector<rfx_ctrler_factory_init> *RfxControllerFactory::sPreNonSlotControllerList = NULL;
Vector<rfx_ctrler_factory_init> *RfxControllerFactory::sPreNonSlotOpControllerList = NULL;
Vector<rfx_ctrler_factory_init> *RfxControllerFactory::sSlotControllerList = NULL;
Vector<rfx_ctrler_factory_init> *RfxControllerFactory::sSlotOpControllerList = NULL;
Vector<rfx_ctrler_factory_init> *RfxControllerFactory::sNonSlotControllerList = NULL;
Vector<rfx_ctrler_factory_init> *RfxControllerFactory::sNonSlotOpControllerList = NULL;

extern const rfx_ctrler_factory_init s_pre_non_slot_controllers[];
extern const rfx_ctrler_factory_init s_slot_controllers[];
extern const rfx_ctrler_factory_init s_non_slot_controllers[];

extern const int s_num_pre_non_slot_controller;
extern const int s_num_slot_controller;
extern const int s_num_non_slot_controller;
/*****************************************************************************
 * Class RfxControllerFactory
 *****************************************************************************/

void RfxControllerFactory::createControllers() {
    RfxRootController *root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    int numOfOm = 0;
    int numOfOp = 0;
    int numOfNonSlotOm = 0;
    int numOfNonSlotOp = 0;

    // TODO
    //RFX_OBJ_GET_INSTANCE(RfxRilAdapter)->initSocketNotifyByFwk();

    RfxSlotRootController *non_slot_controller;

    // create a special slot root controller as non-slot based controllers parent
    RFX_OBJ_CREATE_EX(non_slot_controller, RfxSlotRootController, root, (RFX_SLOT_ID_UNKNOWN));
    root->setSlotRootController(RFX_SLOT_ID_UNKNOWN, non_slot_controller);

    // init controller list vector
    sPreNonSlotControllerList = new Vector<rfx_ctrler_factory_init>();
    sPreNonSlotOpControllerList = new Vector<rfx_ctrler_factory_init>();
    sSlotControllerList = new Vector<rfx_ctrler_factory_init>();
    sSlotOpControllerList = new Vector<rfx_ctrler_factory_init>();
    sNonSlotControllerList = new Vector<rfx_ctrler_factory_init>();
    sNonSlotOpControllerList = new Vector<rfx_ctrler_factory_init>();

    // int numOfPreOm = sizeof(s_pre_non_slot_controllers)/sizeof(rfx_ctrler_factory_init);
    int numOfPreOm = s_num_pre_non_slot_controller;
    for (unsigned int i = 0; i < numOfPreOm; i++) {
        sPreNonSlotControllerList->add(s_pre_non_slot_controllers[i]);
    }

    const rfx_ctrler_factory_init *preControllerList =
            RfxOpUtils::getPreNonSlotOpCtlerList();
    int numOfPreOp = RfxOpUtils::getNumOfPreNonSlotOpCtler();
    if (preControllerList != NULL) {
        for (unsigned int i = 0; i < numOfPreOp; i++) {
            sPreNonSlotOpControllerList->add(preControllerList[i]);
        }
    }

    // numOfOm = sizeof(s_slot_controllers)/sizeof(rfx_ctrler_factory_init);
    numOfOm = s_num_slot_controller;
    for (unsigned int i = 0; i < numOfOm; i++) {
        sSlotControllerList->add(s_slot_controllers[i]);
    }


    // numOfNonSlotOm = sizeof(s_non_slot_controllers)/sizeof(rfx_ctrler_factory_init);
    numOfNonSlotOm = s_num_non_slot_controller;
    for (unsigned int i = 0; i < numOfNonSlotOm; i++) {
        sNonSlotControllerList->add(s_non_slot_controllers[i]);
    }

    const rfx_ctrler_factory_init *controllerList =
            RfxOpUtils::getSlotOpCtlerList();
    numOfOp = RfxOpUtils::getNumOfSlotOpCtler();
    if (controllerList != NULL) {
        for (unsigned int i = 0; i < numOfOp; i++) {
           sSlotOpControllerList->add(controllerList[i]);
       }
    }

    const rfx_ctrler_factory_init *nonSlotControllerList =
            RfxOpUtils::getNonSlotOpCtlerList();
    numOfNonSlotOp = RfxOpUtils::getNumOfNonSlotOpCtler();
    if (nonSlotControllerList != NULL) {
        for (unsigned int i = 0; i < numOfNonSlotOp; i++) {
            sNonSlotOpControllerList->add(nonSlotControllerList[i]);
        }
    }

    RFX_LOG_D(RFX_LOG_TAG,
        "numOfOmSlot:%d, numofOpSlot:%d, numOfNonSlotOm: %d,\
numOfNonSlotOp: %d", numOfOm, numOfOp, numOfNonSlotOm, numOfNonSlotOp);

    // Replace OM controller with OP controller extending from OM controller
    mergeControllerFactorList();


    // create some non-slot based controllers firstly
    createControllerInternal(sPreNonSlotControllerList, numOfPreOm, non_slot_controller);
    createControllerInternal(sPreNonSlotOpControllerList, numOfPreOp, non_slot_controller);

    // create slot controllers
    RfxSlotRootController *slot_controller;
    for (int i = 0; i < RFX_SLOT_COUNT; i++) {
        RFX_OBJ_CREATE_EX(slot_controller, RfxSlotRootController, root, (i));
        root->setSlotRootController(i, slot_controller);
        createControllerInternal(sSlotControllerList, numOfOm, slot_controller);
        createControllerInternal(sSlotOpControllerList, numOfOp, slot_controller);
    }

    // create non-slot controllers
    createControllerInternal(sNonSlotControllerList, numOfNonSlotOm, non_slot_controller);
    createControllerInternal(sNonSlotOpControllerList, numOfNonSlotOp, non_slot_controller);

    delete sPreNonSlotControllerList;
    delete sPreNonSlotOpControllerList;
    delete sSlotControllerList;
    delete sSlotOpControllerList;
    delete sNonSlotControllerList;
    delete sNonSlotOpControllerList;
}

void RfxControllerFactory::createControllerInternal(
    const Vector<rfx_ctrler_factory_init> * controller_list,
    int length, RfxObject *parent) {

    rfx_ctrler_factory_init ctrlerInit;
    for (int i = 0; i < length; i++) {
        ctrlerInit = (rfx_ctrler_factory_init)(controller_list->itemAt(i));
        RfxClassInfo *classInfo = ctrlerInit.getControClassInfo();
        if (ctrlerInit.skip== false) {
            RFX_LOG_D(RFX_LOG_TAG, "createControllerInternalEx init %s",
                    classInfo->getClassName());
            const RfxCreateControllerFuncptr ptr =
                    ctrlerInit.getCreateControllerFuncptr;
            if (ptr != NULL) {
                ptr(parent);
            }
        }
    }
}

void RfxControllerFactory::mergeControllerFactorList() {
    // Merge slot controller, replace platform controller with op controller extends from it
    int NumOfPreOmContro = sPreNonSlotControllerList->size();
    int NumOfPreOpContro = sPreNonSlotOpControllerList->size();
    for (int i = 0; i < NumOfPreOmContro; i++) {
        rfx_ctrler_factory_init parent = sPreNonSlotControllerList->itemAt(i);
        RfxClassInfo *parentClassInfo = parent.getControClassInfo();
        for (int j=0; j < NumOfPreOpContro; j++) {
            rfx_ctrler_factory_init opController = sPreNonSlotOpControllerList->itemAt(j);
            RfxClassInfo *classInfo = opController.getControClassInfo();
            if (classInfo->isKindOf(parentClassInfo)) {
                sPreNonSlotControllerList->replaceAt(opController, i);
                opController.skip = true;
                sPreNonSlotOpControllerList->replaceAt(opController, j);
                RFX_LOG_I(RFX_LOG_TAG, "%s merged", parentClassInfo->getClassName());
                break;
            }
        }
    }

    int NumOfOmContro = sSlotControllerList->size();
    int NumOfOpContro = sSlotOpControllerList->size();
    for (int i = 0; i < NumOfOmContro; i++) {
        rfx_ctrler_factory_init parent = sSlotControllerList->itemAt(i);
        RfxClassInfo *parentClassInfo = parent.getControClassInfo();
        for (int j=0; j < NumOfOpContro; j++) {
            rfx_ctrler_factory_init opController = sSlotOpControllerList->itemAt(j);
            RfxClassInfo *classInfo = opController.getControClassInfo();
            if (classInfo->isKindOf(parentClassInfo)) {
                sSlotControllerList->replaceAt(opController, i);
                opController.skip = true;
                sSlotOpControllerList->replaceAt(opController, j);
                RFX_LOG_I(RFX_LOG_TAG, "%s merged %s",
                        parentClassInfo->getClassName(), classInfo->getClassName());
                break;
            }
        }
    }

    // Merge non-slot controller, replace platform controller with op controller extends from it
    int NumOfNonOmContro = sNonSlotControllerList->size();
    int NumOfNonOpContro = sNonSlotOpControllerList->size();
    for (int i = 0; i < NumOfNonOmContro; i++) {
        rfx_ctrler_factory_init parent = sNonSlotControllerList->itemAt(i);
        RfxClassInfo *parentClassInfo = parent.getControClassInfo();
        for (int j=0; j < NumOfNonOpContro; j++) {
            rfx_ctrler_factory_init opController = sNonSlotOpControllerList->itemAt(j);
            RfxClassInfo *classInfo = opController.getControClassInfo();
            if (classInfo->isKindOf(parentClassInfo)) {
                sNonSlotControllerList->replaceAt(opController, i);
                opController.skip = true;
                sNonSlotOpControllerList->replaceAt(opController, j);
                RFX_LOG_I(RFX_LOG_TAG, "%s merged", parentClassInfo->getClassName());
                break;
            }
        }
    }
}
