#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "RfxRootController.h"
#include "RfxLog.h"
#include "RfxStringsData.h"
#include "RfxXmlParser.h"
#include "rfx_properties.h"
#include "RfxRilUtils.h"

#include "RtcImsConferenceController.h"
#include "RtcImsVideoRingtoneHandler.h"

#define RFX_LOG_TAG "RtcImsVideoRingtoneHandler"

const string RtcImsVideoRingtoneHandler::TAG_NEXT_LINE("<ascii_10>");
const string RtcImsVideoRingtoneHandler::TAG_RETURN("<ascii_13>");
const string RtcImsVideoRingtoneHandler::TAG_DOUBLE_QUOTE("<ascii_34>");

RtcImsVideoRingtoneHandler::RtcImsVideoRingtoneHandler(int slot){
    mSlot = slot;
    RFX_LOG_D(RFX_LOG_TAG, "RtcImsVideoRingtoneHandler()");
}

RtcImsVideoRingtoneHandler::~RtcImsVideoRingtoneHandler() {
}

//Handle IMS Dialog event package raw data.
void RtcImsVideoRingtoneHandler::handleImsVideoRingtoneMessage(const sp<RfxMessage>& message) {
    RFX_LOG_D(RFX_LOG_TAG, "handleImsVideoRingtoneMessage");
    /*
     * +EIMSEVTPKG: <call_id>,<type>,<urc_index>,<total_urc_count>,<data>
     * <call_id>:  0~255
     * <type>: 1 = Conference Event Package; 2 = Dialog Event Package; 3 = Message Waiting Event Package; 100 = VideoRingtone Event Package
     * <urc_index>: 1~255, the index of URC part
     * <total_urc_count>: 1~255
     * <data>: xml raw data, max length = 1950
     */

    RfxStringsData* data = (RfxStringsData*)message->getData();
    char** params = (char**)data->getData();
    int callId = atoi(params[0]);
    int msgType = atoi(params[1]);
    int index = atoi(params[2]);
    int count = atoi(params[3]);
    char* rawData = params[4];

    if (rawData == NULL) {
        return;
    }

    bool isFirstPkt = (index == 1);
    mVRTepData = concatData(isFirstPkt, mVRTepData, rawData);
    if (index != count) {
        //do nothing
        return;
    }
    mVRTepData = recoverDataFromAsciiTag(mVRTepData);
    if (mVRTepData.empty()) {
        RFX_LOG_D(RFX_LOG_TAG, "Failed to handleImsVideoRingToneMessage due to data is empty");
        return;
    }

    sp<RfxXmlParser> parser = new RfxXmlParser();

    RFX_LOG_D(RFX_LOG_TAG, "start to new VRTMessageHandler");
    VRTMessageHandler* xmlData = new VRTMessageHandler();
    RFX_LOG_D(RFX_LOG_TAG, "VRTMessageHandler constructor done");

    if (xmlData == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "can't create xmlData object.");
        return;
    }

    RFX_LOG_D(RFX_LOG_TAG, "start to parse xml");
    parser->parse(xmlData, mVRTepData);

    RFX_LOG_D(RFX_LOG_TAG, "parse xml done");

    string content = xmlData->getDigits();

    RFX_LOG_D(RFX_LOG_TAG, "getDigits done");

    int contentLen = strlen(content.c_str());

    RFX_LOG_D(RFX_LOG_TAG, "start copy to VRTInfo");

    char VRTInfo[256] = {0};

    if (contentLen >= 256 ) {
        RFX_LOG_E(RFX_LOG_TAG, "content too long");
        return;
    }
    strncpy(VRTInfo, content.c_str(), contentLen);

    RFX_LOG_D(RFX_LOG_TAG, "callId: %d, msgType: %d, VRTInfo: %s", callId, msgType, VRTInfo);

    const int maxLen = 5;

    char **urcData = (char **) calloc(maxLen, sizeof(char *));
    RFX_ASSERT(urcData != NULL);

    urcData[0] = strdup(params[0]); //call id
    urcData[1] = strdup(params[1]);  //type
    urcData[2] = strdup(VRTInfo);  //event info

    sp<RfxMessage> msg = RfxMessage::obtainUrc(mSlot, RFX_MSG_UNSOL_VIDEO_RINGTONE_EVENT_IND,
            RfxStringsData(urcData, maxLen));


    RfxRootController *root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    RtcImsConferenceController *ctrl =
            (RtcImsConferenceController *)root->findController(mSlot,
                    RFX_OBJ_CLASS_INFO(RtcImsConferenceController));
    ctrl->responseToRilj(msg);

    for (int i = 0; i < maxLen; i++) {
        if (urcData[i] != NULL) {
            free(urcData[i]);
        }
    }
    free(urcData);

    delete xmlData;
    xmlData = NULL;
}

string RtcImsVideoRingtoneHandler::concatData(int isFirst, string origData, string appendData) {
    if (isFirst) {
        return appendData;
    }
    return origData + appendData;
}

string RtcImsVideoRingtoneHandler::recoverDataFromAsciiTag(string data) {
    data = replaceAll(data, TAG_RETURN, "\r");
    data = replaceAll(data, TAG_DOUBLE_QUOTE, "\"");
    data = replaceAll(data, TAG_NEXT_LINE, "\n");
    return data;
}

string RtcImsVideoRingtoneHandler::replaceAll(string &str, const string &old_value, const string &new_value) {
    while(true) {
        string::size_type pos(0);
        if((pos=str.find(old_value)) != string::npos) {
            str.replace(pos, old_value.length(), new_value);
        } else {
            break;
        }
    }
    return str;
}

//============================================================


const string VRTMessageHandler::MSML("msml");
const string VRTMessageHandler::DIALOG_START("dialogstart");
const string VRTMessageHandler::DTMF_GEN("dtmfgen");
const string VRTMessageHandler::DTMF_GEN_EXIT("dtmfgenexit");
const string VRTMessageHandler::VERSION("version");
const string VRTMessageHandler::SEND("send");
const string VRTMessageHandler::ID("id");
const string VRTMessageHandler::STATE("state");
const string VRTMessageHandler::NAME("name");
const string VRTMessageHandler::DIGITS("digits");
const string VRTMessageHandler::EVENT("event");
const string VRTMessageHandler::TARGET("target");
const string VRTMessageHandler::TYPE("type");
const string VRTMessageHandler::DUR("dur");
const string VRTMessageHandler::LEVEL("level");
const string VRTMessageHandler::NAME_LIST("namelist");


VRTMessageHandler::VRTMessageHandler() :
    mDigits(""), mVersion(-1), mVRTepState(VRTEP_STATE_UNKNOWN) {
}

VRTMessageHandler::~VRTMessageHandler() {
}

/**
 * To retrieve Digits info.
 * @return Digits
 */
string VRTMessageHandler::getDigits() {
    return mDigits;
}

/**
 * To get version
 * @return the msml version
 */
int VRTMessageHandler::getVersion() const {
    return mVersion;
}

void VRTMessageHandler::startElement(string nodeName, string nodeValue, string attributeName,
        string attributeValue) {
    RFX_UNUSED(nodeValue);
    if (nodeName == MSML) {
        if (attributeName == VERSION) {
            mVersion = atoi(attributeValue.c_str());
            RFX_LOG_D(RFX_LOG_TAG, ":startElement version: %d", mVersion);
        }
    } else if (nodeName == DTMF_GEN) {
        if (attributeName == DIGITS) {
            mDigits = attributeValue;
            RFX_LOG_D(RFX_LOG_TAG, ":startElement mDigits: %s", mDigits.c_str());
        }
    }
}

void VRTMessageHandler::endElement(string nodeName) {
    if (nodeName == DTMF_GEN) {
        RFX_LOG_D(RFX_LOG_TAG, "endElement end DTMF_GEN");
    }
}

