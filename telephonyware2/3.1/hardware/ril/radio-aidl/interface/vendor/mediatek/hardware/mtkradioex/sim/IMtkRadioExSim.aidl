// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.sim;
import vendor.mediatek.hardware.mtkradioex.rsu.IRsuRadioResponse;
import vendor.mediatek.hardware.mtkradioex.rsu.IRsuRadioIndication;
import vendor.mediatek.hardware.mtkradioex.se.ISERadioResponse;
import vendor.mediatek.hardware.mtkradioex.se.ISERadioIndication;
import vendor.mediatek.hardware.mtkradioex.sim.SimAuthStructure;
import vendor.mediatek.hardware.mtkradioex.sim.PhbEntryStructure;
import vendor.mediatek.hardware.mtkradioex.sim.PhbEntryExt;
import vendor.mediatek.hardware.mtkradioex.sim.IMtkRadioExSimResponse;
import vendor.mediatek.hardware.mtkradioex.sim.IMtkRadioExSimIndication;

@VintfStability
oneway interface IMtkRadioExSim {


    /**
     * Get Iccid
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getIccidResponse
     */
    void getIccid(in int serial, in int clientId);

    /**
     * Activate uicc card
     *
     * @param serial Serial number of request.
     *
     * Response function is IRadioResponse.activateUiccCardRsp()
     */
    void activateUiccCard(in int serial, in int clientId);

    /**
     * Deactivate uicc card
     *
     * @param serial Serial number of request.
     *
     * Response function is IRadioResponse.deactivateUiccCardRsp()
     */
    void deactivateUiccCard(in int serial, in int clientId);

    /**
     * Delete a USIM PHB field's entry.
     *
     * @param serial Serial number of request.
     * @param entryType must be 0(ANR), 1(EMAIL), 2(SNE), 3(AAS), or 4(GAS)
     * @param adnIndex ADN index
     * @param entryIndex The i-th EF_(EMAIL/ANR/SNE)
     *
     * Response function is IMtkRadioExResponse.deleteUPBEntryResponse()
     */
    void deleteUPBEntry(in int serial, in int entryType, in int adnIndex,
        in int entryIndex, in int clientId);

    /**
     * Request to do general sim authentication using the given parameters.
     *
     * @param serial Serial number of request.
     * @param simAuth A SIM Auth strucutre, SimAuthStructure as defined in types.hal
     *
     * Response function is IMtkRadioExResponse.writePhoneBookEntryExtResponse()
     */
    void doGeneralSimAuthentication(in int serial, in SimAuthStructure simAuth, in int clientId);

    /**
     * Update a USIM PHB field's entry.
     *
     * @param serial Serial number of request.
     * @param data may be int[4] or int[6](only when entryType is 0),
     *        which contains:
     *        entryType must be 0(ANR), 1(EMAIL), 2(SNE), 3(AAS), or 4(GAS)
     *        adnIndex ADN index
     *        entryIndex the i-th EF_(EMAIL/ANR/SNE)
     *        strVal is the value string to be updated
     *        tonForNum TON for ANR
     *        aasAnrIndex AAS index of the ANR
     *
     * Response function is IMtkRadioExResponse.editUPBEntryResponse()
     */
    void editUPBEntry(in int serial, in String[] data, in int clientId);

    /**
     * Get ATR
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getATRResponse
     */
    void getATR(in int serial, in int clientId);


    /**
     * Get current uicc card provisioning status
     *
     * @param serial Serial number of request.
     *
     * Response function is IRadioResponse.getCurrentUiccCardProvisioningStatusRsp()
     */
    void getCurrentUiccCardProvisioningStatus(in int serial, in int clientId);

    /**
     * Get phonebook storage info.
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getPhoneBookMemStorageResponse()
     */
    void getPhoneBookMemStorage(in int serial, in int clientId);

    /**
     * Get maximum length of some phonebook fields.
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getPhoneBookStringsLengthResponse()
     */
    void getPhoneBookStringsLength(in int serial, in int clientId);

    /**
     * When STK application gets stkCallSetup(), the call actually has
     * been initialized by mobile device already. (We could see the call has been in the 'call
     * list') So, STK application needs to accept/reject the call according to user
     * operations.
     *
     * @param serial Serial number of request.
     * @param resultCode 1:accept the call setup,0:reject the call setup,
     *         0x20:TERMINAL_CRNTLY_UNABLE_TO_PROCESS
     *         0x21:NETWORK_CRNTLY_UNABLE_TO_PROCESS
     *
     * Response callback is IMtkRadioExResponse.handleStkCallSetupRequestFromSimWithResCodeResponse()
     */
    void handleStkCallSetupRequestFromSimWithResCode(in int serial, in int resultCode, in int clientId);

    /**
     * Request to query NetworkLock using the given category.
     *
     * @param serial Serial number of request.
     * @param category the lock category
     *
     * Response function is IMtkRadioExResponse.queryNetworkLockResponse()
     */
    void queryNetworkLock(in int serial, in int category, in int clientId);

    /**
     * Request the information of the given storage type.
     *
     * @param serial Serial number of request.
     * @param type The type of the storage, refer to PHB_XDN defined in the RilConstants
     *
     * Response function is IMtkRadioExResponse.queryPhbStorageInfoResponse()
     */
    void queryPhbStorageInfo(in int serial, in int type, in int clientId);

    /**
     * Query info of the EF_EMAIL/EF_ANR/EF_Sne.
     *
     * @param serial Serial number of request.
     * @param eftype 0:EF_ANR, 1:EF_EMAIL, 2: EF_SNE
     * @param fileIndex the i-th EF_EMAIL/EF_ANR/EF_SNE (1-based)
     *
     * Response function is IMtkRadioExResponse.queryUPBAvailableResponse()
     */
    void queryUPBAvailable(in int serial, in int eftype, in int fileIndex, in int clientId);

   /**
     * Request read PHB entries from the given storage.
     *
     * @param serial Serial number of request.
     * @param type The type of the storage, refer to PHB_* defined in the RilConstants
     * @param bIndex The begin index of the entries to be read
     * @param eIndex The end index of the entries to be read, note that the (eIndex - bIndex +1)
     *               should not exceed the value RilConstants.PHB_MAX_ENTRY
     *
     * Response function is IMtkRadioExResponse.readPhbEntryResponse()
     */
    void readPhbEntry(in int serial, in int type, in int bIndex, in int eIndex, in int clientId);

    /**
     * Request read PHB entries with some UPB fields value.
     *
     * @param serial Serial number of request.
     * @param index1 The begin index of the entries to be read
     * @param index2 The end index of the entries to be read, note that the (index2 - index1 +1)
     *               should not exceed the value RilConstants.PHB_MAX_ENTRY
     *
     * Response function is IMtkRadioExResponse.readPhoneBookEntryExtResponse()
     */
    void readPhoneBookEntryExt(in int serial, in int index1, in int index2, in int clientId);

    /**
     * Read AAS entry by giving range.
     *
     * @param serial Serial number of request.
     * @param startIndex AAS index start to read
     * @param endIndex AAS index end to read
     *
     * Response function is IMtkRadioExResponse.readUPBAasListResponse()
     */
    void readUPBAasList(in int serial, in int startIndex, in int endIndex, in int clientId);

    /**
     * Read a ANR entry by ADN index.
     *
     * @param serial Serial number of request.
     * @param adnIndex ADN index
     * @param fileIndex the i-th EF_ANR (1-based)
     *
     * Response function is IMtkRadioExResponse.readUPBAnrEntryResponse()
     */
    void readUPBAnrEntry(in int serial, in int adnIndex, in int fileIndex, in int clientId);

    /**
     * Read a Email entry by ADN index.
     *
     * @param serial Serial number of request.
     * @param adnIndex ADN index
     * @param fileIndex the i-th EF_EMAIL (1-based)
     *
     * Response function is IMtkRadioExResponse.readUPBEmailEntryResponse()
     */
    void readUPBEmailEntry(in int serial, in int adnIndex, in int fileIndex, in int clientId);

    /**
     * Read GAS entry by giving range.
     *
     * @param serial Serial number of request.
     * @param startIndex GAS index start to read
     * @param endIndex GAS index end to read
     *
     * Response function is IMtkRadioExResponse.readUPBGasListResponse()
     */
    void readUPBGasList(in int serial, in int startIndex, in int endIndex, in int clientId);

    /**
     * Read a GRP entry by ADN index.
     *
     * @param serial Serial number of request.
     * @param adnIndex ADN index
     *
     * Response function is IMtkRadioExResponse.readUPBGrpEntryResponse()
     */
    void readUPBGrpEntry(in int serial, in int adnIndex, in int clientId);

    /**
     * Read a SNE entry by ADN index.
     *
     * @param serial Serial number of request.
     * @param adnIndex ADN index
     * @param fileIndex the i-th EF_SNE (1-based)
     *
     * Response function is IMtkRadioExResponse.readUPBSneEntryResponse()
     */
    void readUPBSneEntry(in int serial, in int adnIndex, in int fileIndex, in int clientId);

    /**
     * Send Remote SIM Unlock request.
     *
     * @param serial Serial number of request.
     * @param rsuRequestInfo The request.
     */
    void sendRsuRequest(in int serial,
            in vendor.mediatek.hardware.mtkradioex.rsu.RsuRequestInfo rri, in int clientId);

    /**
     * Request to send vsim notification to modem.
     *
     * @param serial Serial number of request.
     * @param transactionId Serial number of request
     * @param eventId event id to indicated the notification
     * @param simType sim type according to event id
     *
     * Response function is IVsimResponse.vsimNotificationResponse()
     */
    void sendVsimNotification(in int serial, in int transactionId, in int eventId,
        in int simType, in int clientId);

    /**
     * Request to send vsim operation to modem.
     *
     * @param serial Serial number of request.
     * @param transactionId Serial number of request
     * @param eventId event id to indicated the deital operation
     * @param result the apdu execute result
     * @param dataLength apdu raw data length
     * @param data apdu raw data
     *
     * Response function is IVsimResponse.vsimOperationResponse()
     */
    void sendVsimOperation(in int serial, in int transactionId, in int eventId,
        in int result, in int dataLength, in byte[] data, in int clientId);


    /**
     * Request to query NetworkLock using the given category.
     *
     * @param serial Serial number of request.
     * @param category the lock category
     *
     * Response function is IMtkRadioExResponse.setNetworkLockResponse()
     */
    void setNetworkLock(in int serial, in int category, in int lockop, in String password,
        in String data_imsi, in String gid1, in String gid2, in int clientId);

    /**
     * Set phonebook storage to given type.
     *
     * @param serial Serial number of request.
     * @param storage The type of the storage
     * @param password PIN2 for FDN
     *
     * Response function is IMtkRadioExResponse.setPhoneBookMemStorageResponse()
     */
    void setPhoneBookMemStorage(in int serial, in String storage, in String password, in int clientId);

    /**
     * Set phonebook ready state.
     *
     * @param serial Serial number of request.
     * @param ready which SIM phonebook ready state.
     */
    void setPhonebookReady(in int serial, in int ready, in int clientId);

    /**
     * Set SIM Power
     *
     * @param serial Serial number of request.
     * @param mode Set SIM power mode.
     *
     * Response function is IMtkRadioExResponse.setSimPowerResponse
     */
    void setSimPower(in int serial, in int mode, in int clientId);

    /**
     * Request update a PHB entry using the given PhbEntry.
     *
     * @param serial Serial number of request.
     * @param phbEntry A PHB entry strucutre, PhbEntryStructure as defined in types.hal
     *          when one of the following occurs, it means delete the entry.
     *          1. phbEntry.number is NULL
     *          2. phbEntry.number is empty and phbEntry.ton = 0x91
     *          3. phbEntry.alphaId is NULL
     *          4. both phbEntry.number and phbEntry.alphaId are empty.
     *
     * Response function is IMtkRadioExResponse.writePhbEntryResponse()
     */
    void writePhbEntry(in int serial, in PhbEntryStructure phbEntry, in int clientId);

    /**
     * Request update a PHB entry using the given PhbEntryExt.
     *
     * @param serial Serial number of request.
     * @param phbEntryExt A extended PHB entry strucutre, PhbEntryExt as defined in types.hal
     *
     * Response function is IMtkRadioExResponse.writePhoneBookEntryExtResponse()
     */
    void writePhoneBookEntryExt(in int serial, in PhbEntryExt phbEntryExt, in int clientId);

    /**
     * Update a GRP entry by ADN index.
     *
     * @param serial Serial number of request.
     * @param adnIndex ADN index
     * @param grpIds Group id list to be updated
     *
     * Response function is IMtkRadioExResponse.writeUPBGrpEntryResponse()
     */
    void writeUPBGrpEntry(in int serial, in int adnIndex, in int[] grpIds, in int clientId);

    /**
     * Query capability of USIM PHB.
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.queryUPBCapabilityResponse()
     */
    void queryUPBCapability(in int serial, in int clientId);

    /**
     * Request the given type of network personalization be deactivated
     *
     * @param serial Serial number of request.
     * @param netPin Network depersonlization code
     * @param type the given depersonlization type
     *
     * Response function is IMtkRadioExResponse.supplyDepersonalizationResponse()
     *
     */
    void supplyDepersonalization(in int serial, in String netPin, in int type, in int clientId);

    /**
     * Requests that unlock device
     *
     * @param serial Serial number of request.
     * @param pwd the password to unlock device
     *
     * Response function is IMtkRadioExResponse.supplyDeviceNetworkDepersonalizationResponse()
     *
     */
    void supplyDeviceNetworkDepersonalization(in int serial, in String pwd, in int clientId);


    /**
     * When response type received from a radio indication or radio response is
     * RadioIndicationType:UNSOLICITED_ACK_EXP or RadioResponseType:SOLICITED_ACK_EXP respectively,
     * acknowledge the receipt of those messages by sending responseAcknowledgement().
     *
     */
    void responseAcknowledgementMtk();

    /**
     * Set response functions for mtk radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsMtk(in IMtkRadioExSimResponse radioResponse,
        in IMtkRadioExSimIndication radioIndication);

    /**
     * Set response functions for rsu radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsRsu(
        in IRsuRadioResponse radioResponse,
        in IRsuRadioIndication radioIndication);

    /**
     * Set response functions for ISecureElement radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsSE(
        in ISERadioResponse radioResponse,
        in ISERadioIndication radioIndication);

    /**
     * Get Sim type information
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.getIccCardTypeInfoResponse
     */
    void getSimTypeInfo(in int serial, in int clientId);

    /**
     * Switch Sim type
     *
     * @param serial Serial number of request.
     *
     * Response function is IMtkRadioExResponse.switchIccCardTypeResponse
     */
    void switchSimType(in int serial, in int mode, in int clientId);
}
