// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.sim;

import android.hardware.radio.RadioResponseInfo;
import vendor.mediatek.hardware.mtkradioex.sim.PhbMemStorageResponse;
import vendor.mediatek.hardware.mtkradioex.sim.PhbEntryStructure;
import vendor.mediatek.hardware.mtkradioex.sim.PhbEntryExt;
import vendor.mediatek.hardware.mtkradioex.sim.SimTypeInfo;
import vendor.mediatek.hardware.mtkradioex.sim.VsimEvent;
import vendor.mediatek.hardware.mtkradioex.sim.VsimOperationEvent;


/**
 * Interface declaring response functions to subsidylock requests.
 */
@VintfStability
interface IMtkRadioExSimResponse {

    /**
     * The response of getIccid
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param response Response string of getIccidResponse.
     */
    oneway void getIccidResponse(in android.hardware.radio.RadioResponseInfo info, in String response);

    /**
     * The Response of activateUiccCard
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param simPowerOnOffResponse "0" means SUCCESS, "-1" means GENERIC_FAILURE,
     *              "-3" means REQUEST_IN_PROGRESS
     *
     * Valid errors returned:
     *   RadioError:RIL_E_SUCCESS
     *   RadioError:RIL_E_REQUEST_RATE_LIMITED
     *   RadioError:RIL_E_OPERATION_NOT_ALLOWED
     *   RadioError:SIM_ABSENT
     */
    oneway void activateUiccCardRsp(in android.hardware.radio.RadioResponseInfo info,
        in int simPowerOnOffResponse);

    /**
     * The Response of deactivateUiccCard
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param simPowerOnOffResponse "0" means SUCCESS, "-1" means GENERIC_FAILURE,
     *              "-3" means REQUEST_IN_PROGRESS
     *
     * Valid errors returned:
     *   RadioError:RIL_E_SUCCESS
     *   RadioError:RIL_E_REQUEST_RATE_LIMITED
     *   RadioError:RIL_E_OPERATION_NOT_ALLOWED
     *   RadioError:SIM_ABSENT
     */
    oneway void deactivateUiccCardRsp(in android.hardware.radio.RadioResponseInfo info,
        in int simPowerOnOffResponse);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void deleteUPBEntryResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     *   RadioError:OEM_ERROR_2 (TEXT_STRING_TOO_LONG)
     *   RadioError:OEM_ERROR_3 (SIM_MEM_FULL)
     */
    oneway void editUPBEntryResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of getATRResponse
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param response Response string of getATRResponse.
     */
    oneway void getATRResponse(in android.hardware.radio.RadioResponseInfo info,
        in String response);

    /**
     * The Response of getCurrentUiccCardProvisioningStatus
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param simPowerOnOffStatus "0" means INACTIVE, "1" means ACTIVE
     *
     * Valid errors returned:
     *   RadioError:RIL_E_SUCCESS
     */
    oneway void getCurrentUiccCardProvisioningStatusRsp(
        in android.hardware.radio.RadioResponseInfo info, in int simPowerOnOffStatus);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param phbMemStorage Current phonebook storage info (<STORAGE>, <used>, <total>)
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getPhoneBookMemStorageResponse(in android.hardware.radio.RadioResponseInfo info,
        in PhbMemStorageResponse phbMemStorage);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param stringLengthInfo is an int[5]
     *        stringLengthInfo[0] is maximum supported length of the number
     *        stringLengthInfo[1] is maximum supported length of the alphaId
     *        stringLengthInfo[2] is maximum supported length of the GAS
     *        stringLengthInfo[3] is maximum supported length of the SNE
     *        stringLengthInfo[4] is maximum supported length of the Email
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getPhoneBookStringsLengthResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] stringLengthInfo);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OPERATION_NOT_ALLOWED
     *   RadioError:GENERIC_FAILURE
     */
    oneway void handleStkCallSetupRequestFromSimWithResCodeResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of queryNetworkLock
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void queryNetworkLockResponse(in android.hardware.radio.RadioResponseInfo info,
        in int catagory, in int state, in int retry_cnt, in int autolock_cnt, in int num_set,
        in int total_set, in int key_state);

    /**
     * The response of setNetworkLock
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setNetworkLockResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param storageInfo is an int[4]
     *        storageInfo[0] is number of current used entries
     *        storageInfo[1] is number of total entries in the storage
     *        storageInfo[2] is maximum supported length of the number
     *        storageInfo[3] is maximum supported length of the alphaId
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void queryPhbStorageInfoResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] storageInfo);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param upbAvailable is an int[3]
     *        upbAvailable[0] is <M_NUM>, Max number of entries
     *        upbAvailable[1] is <A_NUM>, Available number of entries
     *        upbAvailable[2] is <L_XXX>, Max support length
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void queryUPBAvailableResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] upbAvailable);


    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param upbCapability <N_ANR>,<N_EMAIL>,<N_SNE>,<N_AAS>,<L_AAS>,<N_GAS>,<L_GAS>,<N_GRP>
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void queryUPBCapabilityResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] upbCapability);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param phbEntries PHB entries in an array of PhbEntryStructure.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void readPhbEntryResponse(in android.hardware.radio.RadioResponseInfo info,
        in PhbEntryStructure[] phbEntries);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param phbEntryExts PHB entries with some UPB fields value
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void readPhoneBookEntryExtResponse(in android.hardware.radio.RadioResponseInfo info,
        in PhbEntryExt[] phbEntryExts);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param aasList A AAS string list
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void readUPBAasListResponse(in android.hardware.radio.RadioResponseInfo info,
        in String[] aasList);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param anrs an ANR contained in PhbEntryStructure array
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void readUPBAnrEntryResponse(in android.hardware.radio.RadioResponseInfo info,
        in PhbEntryStructure[] anrs);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param email Email string
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void readUPBEmailEntryResponse(in android.hardware.radio.RadioResponseInfo info,
        in String email);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param gasList A GAS string list
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void readUPBGasListResponse(in android.hardware.radio.RadioResponseInfo info,
        in String[] gasList);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param grpEntries A Group id list of the ADN
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void readUPBGrpEntryResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] grpEntries);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param sne SNE string (need to be decoded)
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void readUPBSneEntryResponse(in android.hardware.radio.RadioResponseInfo info,
        in String sne);

    /**
     * The response of sendRsuRequest.
     *
     * responseInfo Response info struct containing response type, serial no. and error
     */
    oneway void sendRsuRequestResponse(in android.hardware.radio.RadioResponseInfo info,
        in vendor.mediatek.hardware.mtkradioex.rsu.RsuResponseInfo rri);


    /**
     * The response of sendVsimNotification
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param data data returned
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OEM_ERROR_X
     */
    oneway void vsimNotificationResponse(in android.hardware.radio.RadioResponseInfo info,
        in VsimEvent event);

    /**
     * The response of sendVsimOperation
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param data data returned
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OEM_ERROR_X
     */
    oneway void vsimOperationResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setPhoneBookMemStorageResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for set phonebook ready request.
     */
    oneway void setPhonebookReadyResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * The response of setSimPowerResponse
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setSimPowerResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     *   RadioError:SIM_PIN2
     *   RadioError:SIM_PUK2
     *   RadioError:OEM_ERROR_1 (DIAL_STRING_TOO_LONG)
     *   RadioError:OEM_ERROR_2 (TEXT_STRING_TOO_LONG)
     *   RadioError:OEM_ERROR_3 (SIM_MEM_FULL)
     */
    oneway void writePhbEntryResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     *   RadioError:SIM_PIN2
     *   RadioError:SIM_PUK2
     *   RadioError:OEM_ERROR_1 (DIAL_STRING_TOO_LONG)
     *   RadioError:OEM_ERROR_2 (TEXT_STRING_TOO_LONG)
     *   RadioError:OEM_ERROR_3 (SIM_MEM_FULL)
     */
    oneway void writePhoneBookEntryExtResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void writeUPBGrpEntryResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param remainingRetries Number of retries remaining, must be equal to -1 if unknown.
     *
     */
    oneway void supplyDepersonalizationResponse(in android.hardware.radio.RadioResponseInfo info,
        in int remainingRetries);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param remainingRetries Number of retries remaining, must be equal to -1 if unknown.
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:PASSWORD_INCORRECT
     *   RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void supplyDeviceNetworkDepersonalizationResponse(
        in android.hardware.radio.RadioResponseInfo info, in int remainingRetries);

    /**
     * The response of getIccid
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param response Response SimTypeInfo of getIccCardTypeInfo.
     */
    oneway void getSimTypeInfoResponse(in android.hardware.radio.RadioResponseInfo info, in SimTypeInfo typeInfo);

    /**
     * The response of switchIccCardType
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void switchSimTypeResponse(in android.hardware.radio.RadioResponseInfo info);

}
