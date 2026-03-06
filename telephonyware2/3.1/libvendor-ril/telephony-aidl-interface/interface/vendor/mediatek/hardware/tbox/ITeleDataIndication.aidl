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
 
package vendor.mediatek.hardware.tbox;

import vendor.mediatek.hardware.tbox.SetupDataCallResult;
import vendor.mediatek.hardware.tbox.KeepaliveStatus;
/*
 * Interface declaring data indications.
 */
@VintfStability
interface ITeleDataIndication {
    /**
     * Indicate the change of data call list
     *
     * @param dcList Type of radio indication
     */
    oneway void dataStatusChanged(in SetupDataCallResult dc);

    /**
     * Indicates a status update for a particular Keepalive session. This must include a handle for
     * a previous session and should include a status update regarding the state of a keepalive.
     * Unsolicited keepalive status reports should never be PENDING as unsolicited status should
     * only be sent when known.
     *
     * @param status Status information for a Keepalive session
     */
    oneway void keepaliveStatus(in KeepaliveStatus status);
}
