/*
 * Copyright (c) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RIL_SERVICE_H
#define RIL_SERVICE_H

#include <telephony/mtk_ril.h>
#include <ril_internal.h>

namespace radio {
void registerService(RIL_RadioFunctions *callbacks, android::CommandInfo *commands);

void acknowledgeRequest(unsigned int slotId, int serial);

pthread_rwlock_t * getRadioServiceRwlock(int slotId);

void lockRadioServiceRlock(pthread_rwlock_t *radioServiceRwlockPtr, int slotId);

void unlockRadioServiceRlock(pthread_rwlock_t *radioServiceRwlockPtr, int slotId);

void lockRadioServiceWlock(pthread_rwlock_t *radioServiceRwlockPtr, int slotId);

void unlockRadioServiceWlock(pthread_rwlock_t *radioServiceRwlockPtr, int slotId);

int isTidInRLock(unsigned int slotId, pthread_t tid);

int isTidInWLock(unsigned int slotId, pthread_t tid);

void initReadLockOwnerListMutex(void);

void initWriteLockOwnerListMutex(void);

}   // namespace radio

#endif  // RIL_SERVICE_H
