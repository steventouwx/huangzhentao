/*
 * Copyright (C) 2023 The Android Open Source Project
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

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.IVoiceCallCallback;
import vendor.mediatek.hardware.voice.IVoiceDtmfCallback;
import vendor.mediatek.hardware.voice.IVoiceEcallEventCallback;
import vendor.mediatek.hardware.voice.IVoiceEcallStatusCallback;
import vendor.mediatek.hardware.voice.IVoiceServiceErrorCallback;
import vendor.mediatek.hardware.voice.VoiceEcallConfig;
import vendor.mediatek.hardware.voice.VoiceEcallInfo;
import vendor.mediatek.hardware.voice.VoiceEcallState;
import vendor.mediatek.hardware.voice.VoiceForwardingCondition;
import vendor.mediatek.hardware.voice.VoiceForwardingStatus;
import vendor.mediatek.hardware.voice.VoiceRecordArray;

@VintfStability
interface IVoiceService {
    int svVoiceInit();

    int svVoiceDeinit();

    int svVoiceDial(in String num, in int len, out int id);

    int svVoiceHangupAll();

    int svVoiceAnswer(in int id);

    int svVoiceHangup(in int id);

    int svVoiceHold(in int id);

    int svVoiceUnhold(in int id);

    int svVoiceGetRecords(out VoiceRecordArray records);

    int svVoiceForwarding(in int reg, in VoiceForwardingCondition cond, in String num, in int len);

    int svVoiceGetForwardingStatus(in VoiceForwardingCondition cond, out VoiceForwardingStatus status);

    int svVoiceWaiting(in int enable);

    int svVoiceGetWaitingStatus(out int enabled);

    int svVoiceAutoanswer(in int enable, in int sec);

    int svVoiceSendDtmfChar(in int id, in String c);

    int svVoiceSetCallCb(in IVoiceCallCallback cb);

    int svVoiceSetDtmfCb(in IVoiceDtmfCallback cb);

    int svVoiceEcallDial(in VoiceEcallInfo info);

    int svVoiceEcallHangup();

    int svVoiceEcallUpdateMsd(in byte[] msd, in int msdLen);

    int svVoiceEcallPushMsd(out VoiceEcallState state);

    int svVoiceEcallGetConfig(out VoiceEcallConfig config);

    int svVoiceEcallSetConfig(in int item, in VoiceEcallConfig config);

    int svVoiceEcallSetEventCb(in IVoiceEcallEventCallback cb);

    int svVoiceEcallSetStatusCb(in IVoiceEcallStatusCallback cb);

    int svVoiceSetServiceErrorCb(in IVoiceServiceErrorCallback cb);
}
