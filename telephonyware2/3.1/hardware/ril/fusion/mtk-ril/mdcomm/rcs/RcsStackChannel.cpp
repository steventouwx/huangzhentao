#include "RfxLog.h"
#include "RcsCapabilityRule.h"
#include "RcsStackChannel.h"
#include <inttypes.h>

namespace vendor::rmc::rcs {
    RcsStackChannel::RcsStackChannel(int32_t slotId, ChannelType type) : m_slot(slotId), m_type(type) {
        m_account = m_slot * MAX_SIM_SUPPORTED;
        m_is_bind = false;
    }

    int RcsStackChannel::create(uint32_t connInfo, char *userInfo, const char *associated) {

        if (m_type == ChannelType::DATA_CHANNEL
            && (connInfo == 0 || userInfo == 0 || associated == 0)) {
            return -1;
        }

        ChannelInfo info;
        int packedLen = 0;
        RcsCapabilityRule *rule = 0;

        if (m_type == ChannelType::STATE_CHANNEL) {
            info.uaType = (UA_RCS | UA_MAGIC_NUMBER);
            info.connInfo = 0;
        } else {
            rule = new RcsCapabilityRule();

            rule->setLevel0(userInfo, associated);
            rule->addLevel2(METHOD_MESSAGE, HDR_CONTENT_TYPE, "application/rcs");
            rule->addLevel2(METHOD_NOTIFY, HDR_EVENT, "presence");
            rule->setLevel3("cpm");
            int len = rule->packAll();
            if (len <= 0) {
                delete rule;
                rule = 0;
            } else {
                packedLen = len;
            }

            info.uaType = (UA_RCS_SIP | UA_MAGIC_NUMBER);
            info.connInfo = connInfo;
        }

        int total = sizeof(ChannelData) + packedLen + sizeof(ChannelInfo);
        m_raw_data = (uint8_t *)malloc(total);
        ChannelData *chnlData = (ChannelData *)m_raw_data;

        chnlData->reqId = REQ_CREATE;
        chnlData->flag = 0;
        chnlData->account = m_account;
        chnlData->dataLen = packedLen + sizeof(ChannelInfo);
        if (m_type == ChannelType::STATE_CHANNEL) {
            chnlData->type = UA_RCS;
        } else {
            chnlData->type = UA_RCS_SIP;
        }
        memcpy(chnlData->data, (uint8_t *)&info, sizeof(info));
        if (rule) {
            memcpy(&(chnlData->data[sizeof(info)]), rule->getPackedData(), packedLen);
            delete rule;
        }
        return total;
    }

    int RcsStackChannel::bind() {
        RegItem item;
        ChannelData *chnlData = 0;

        memset(&item, 0, sizeof(item));
        item.account = m_account;

        int total = sizeof(*chnlData) + sizeof(item);
        m_raw_data = (uint8_t *)malloc(total);
        chnlData = (ChannelData *)m_raw_data;

        chnlData->reqId = REQ_REG_ADD_BIND;
        chnlData->flag = 0;
        chnlData->account = m_account;
        chnlData->dataLen = sizeof(item);
        if (m_type == ChannelType::STATE_CHANNEL) {
            chnlData->type = UA_RCS;
        } else {
            chnlData->type = UA_RCS_SIP;
        }

        memcpy(chnlData->data, (uint8_t *)&item, sizeof(item));

        m_is_bind = true;

        return total;
    }

    int RcsStackChannel::unbind() {
        RegItem item;
        ChannelData *chnlData = 0;

        if (!m_is_bind) {
            return -1;
        }

        memset(&item, 0, sizeof(item));
        item.account = m_account;

        int total = sizeof(ChannelData) + sizeof(item);
        m_raw_data = (uint8_t *)malloc(total);
        chnlData = (ChannelData *)m_raw_data;

        chnlData->reqId = REQ_REG_DEL_BIND;
        chnlData->flag = 0;
        chnlData->account = m_account;
        chnlData->dataLen = sizeof(item);
        if (m_type == ChannelType::STATE_CHANNEL) {
            chnlData->type = UA_RCS;
        } else {
            chnlData->type = UA_RCS_SIP;
        }
        memcpy(chnlData->data, (uint8_t *)&item, sizeof(item));

        m_is_bind = false;

        return total;
    }

    int RcsStackChannel::destroy() {
        ChannelData *chnlData = 0;

        int total = sizeof(*chnlData);
        m_raw_data = (uint8_t *)malloc(total);
        chnlData = (ChannelData *)m_raw_data;

        chnlData->reqId = REQ_DESTROY;
        chnlData->flag = 0;
        chnlData->account = m_account;
        chnlData->dataLen = 0;
        if (m_type == ChannelType::STATE_CHANNEL) {
            chnlData->type = UA_RCS;
        } else {
            chnlData->type = UA_RCS_SIP;
        }

        return total;
    }

    int RcsStackChannel::getRegState() {
        ChannelData *chnlData = 0;
        RegItem item;

        if (!m_is_bind) {
            return -1;
        }

        memset(&item, 0, sizeof(item));
        item.account = m_account;

        int total = sizeof(*chnlData) + sizeof(item);
        m_raw_data = (uint8_t *)malloc(total);
        chnlData = (ChannelData *)m_raw_data;

        chnlData->reqId = REQ_REG_GET_STATE;
        chnlData->flag = 0;
        chnlData->account = m_account;
        chnlData->dataLen = sizeof(item);
        if (m_type == ChannelType::STATE_CHANNEL) {
            chnlData->type = UA_RCS;
        } else {
            chnlData->type = UA_RCS_SIP;
        }
        memcpy(chnlData->data, (uint8_t *)&item, sizeof(item));

        return total;
    }

    int RcsStackChannel::setDeregIndEnabled(bool enable) {
        ChannelData *chnlData = 0;
        UaState uaState;

        if (!m_is_bind) {
            return -1;
        }

        memset(&uaState, 0, sizeof(uaState));
        uaState.account = 0; // Always set 0 here, no use for stack?
        uaState.type = UA_RCS | UA_MAGIC_NUMBER;
        uaState.state = enable ? 1 : 0;

        int total = sizeof(*chnlData) + sizeof(uaState);
        m_raw_data = (uint8_t *)malloc(total);
        chnlData = (ChannelData *)m_raw_data;

        chnlData->reqId = IND_REG_ENABLE_DEREG;
        chnlData->flag = 0;
        chnlData->account = m_account;
        chnlData->dataLen = sizeof(uaState);
        if (m_type == ChannelType::STATE_CHANNEL) {
            chnlData->type = UA_RCS;
        } else {
            chnlData->type = UA_RCS_SIP;
        }
        memcpy(chnlData->data, (uint8_t *)&uaState, sizeof(uaState));

        return total;
    }

    int RcsStackChannel::resumeDeregistration() {
        ChannelData *chnlData = 0;
        int32_t account = m_account;

        if (!m_is_bind) {
            return -1;
        }

        int total = sizeof(*chnlData) + sizeof(account);
        m_raw_data = (uint8_t *)malloc(total);
        chnlData = (ChannelData *)m_raw_data;

        chnlData->reqId = CNF_REG_RESUME_DEREG;
        chnlData->flag = 0;
        chnlData->account = m_account;
        chnlData->dataLen = sizeof(account);
        chnlData->type = UA_RCS;
        memcpy(chnlData->data, (uint8_t *)&account, sizeof(account));

        return total;
    }

    int RcsStackChannel::restore() {
        ChannelData *chnlData = 0;
        RegItem item;

        if (!m_is_bind) {
            return -1;
        }

        memset(&item, 0, sizeof(item));
        item.account = m_account;

        int total = sizeof(*chnlData) + sizeof(item);
        m_raw_data = (uint8_t *)malloc(total);
        chnlData = (ChannelData *)m_raw_data;

        chnlData->reqId = REQ_REG_RESTORE;
        chnlData->flag = 0;
        chnlData->account = m_account;
        chnlData->dataLen = sizeof(item);
        chnlData->type = UA_RCS;
        memcpy(chnlData->data, (uint8_t *)&item, sizeof(item));

        return total;
    }

    int RcsStackChannel::sendSipMessage(
        int32_t reqId, int32_t connId, const uint8_t *messageData, int32_t messageLen) {
        ChannelData *chnlData = 0;
        SipInfo sipInfo;

        memset(&sipInfo, 0, sizeof(sipInfo));
        sipInfo.connId = connId;
        sipInfo.reqId = reqId;
        sipInfo.length = messageLen;

        int total = sizeof(*chnlData) + sizeof(sipInfo) + messageLen;
        int offset = 0;
        m_raw_data = (uint8_t *)malloc(total);
        chnlData = (ChannelData *)m_raw_data;

        chnlData->reqId = REQ_SEND_SIP;
        chnlData->flag = 0;
        chnlData->account = m_account;
        chnlData->dataLen = sizeof(sipInfo) + messageLen;
        chnlData->type = UA_RCS;

        memcpy((void *)chnlData->data, (void *)&sipInfo, sizeof(sipInfo));
        offset += sizeof(sipInfo);
        memcpy((void *)&chnlData->data[offset], (void *)messageData, messageLen);

        return total;
    }
} //namespace: rmc
