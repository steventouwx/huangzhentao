// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.ecall;

import vendor.mediatek.hardware.mtkradioex.ecall.ECallSetMSD;
import vendor.mediatek.hardware.mtkradioex.ecall.ECallSetNum;
import vendor.mediatek.hardware.mtkradioex.ecall.ECallReqMsg;
import vendor.mediatek.hardware.mtkradioex.ecall.ECallCategory;
import vendor.mediatek.hardware.mtkradioex.ecall.ECallDomain;
import vendor.mediatek.hardware.mtkradioex.ecall.ECallVariant;
import vendor.mediatek.hardware.mtkradioex.ecall.ECallPRI;
import vendor.mediatek.hardware.mtkradioex.ecall.ECallTime;

import vendor.mediatek.hardware.mtkradioex.ecall.IMtkRadioExEcallResponse;
import vendor.mediatek.hardware.mtkradioex.ecall.IMtkRadioExEcallIndication;

/**
 * Interface declaring unsolicited indications to subsidylock.
 */
@VintfStability
interface IMtkRadioExEcall {
    /**
     * Reset IVS
     * Request of RIL request "RIL_REQUEST_ECALL_RESET_IVS"
     *
     * @param serial Serial number of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.resetIvsResponse()
     */
    void resetIvs(in int serial, in int clientId);

    /**
     * Set MSD
     * Request of RIL request "RIL_REQUEST_ECALL_SET_MSD"
     *
     * @param serial Serial number of request
     * @param msd msd info of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setMSDResponse()
     */
    void setMSD(in int serial, in ECallSetMSD msd, in int clientId);

    /**
     * Set test number response
     * Request of RIL request "RIL_REQUEST_ECALL_SET_TEST_NUM"
     *
     * @param serial Serial number of request
     * @param testNum test number info of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setTestNumResponse()
     */
    void setTestNum(in int serial, in ECallSetNum testNum, in int clientId);

    /**
     * Set reconf number
     * Request of RIL request "RIL_REQUEST_ECALL_SET_RECONF_NUM"
     *
     * @param serial Serial number of request
     * @param reconfNum reconf number info of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setReconfNumResponse()
     */
    void setReconfNum(in int serial, in ECallSetNum reconfNum, in int clientId);

    /**
     * Make fast ecall
     * Request of RIL request "RIL_REQUEST_ECALL_FAST_MAKE_ECALL"
     *
     * @param serial Serial number of request
     * @param reconfNum reconf number info of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.makeFastEcallResponse()
     */
    void makeFastEcall(in int serial, in ECallReqMsg msg, in int clientId);

    /**
     * Set ecall priority
     * Request of RIL request "RIL_REQUEST_ECALL_SET_PRI"
     *
     * @param serial Serial number of request
     * @param pri priority info of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setEmsdPriResponse()
     */
    void setEmsdPri(in int serial, in ECallPRI pri, in int clientId);

    /**
     * Set T10 timer
     * Request of RIL request "RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME"
     *
     * @param serial Serial number of request
     * @param time timer info of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setNadDeregTimeResponse()
     */
    void setNadDeregTime(in int serial, in ECallTime time, in int clientId);

    /**
     * Set ecall only mode response
     * Request of RIL request "RIL_REQUEST_ECALL_SET_OPRT_MODE"
     *
     * @param serial Serial number of request
     * @param mode mode of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.setOprtModeResponse()
     */
    void setOprtMode(in int serial, in int mode, in int clientId);

    /**
     * Get ecall only mode response
     * Response of RIL request "RIL_REQUEST_ECALL_GET_OPRT_MODE"
     *
     * @param serial Serial number of request
     * @param clientId ril service client Id
     *
     * Response function is IMtkRadioExResponse.getOprtModeResponse()
     */
    void getOprtMode(in int serial, in int clientId);

    /**
     * Set response functions for mtk radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsMtk(in IMtkRadioExEcallResponse radioResponse,
        in IMtkRadioExEcallIndication radioIndication);
}
