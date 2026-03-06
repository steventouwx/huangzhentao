
#include "RfxSimTypeInfoData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSimTypeInfoData);

RfxSimTypeInfoData::RfxSimTypeInfoData(void *_data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_SimTypeInfo *pSimTypeInfo = (RIL_SimTypeInfo*)_data;
        RIL_SimTypeInfo *pData = (RIL_SimTypeInfo *)calloc(1, sizeof(RIL_SimTypeInfo));
        RFX_ASSERT(pData != NULL);

        pData->isSupport = pSimTypeInfo->isSupport;
        pData->simType = pSimTypeInfo->simType;
        pData->actionSource = pSimTypeInfo->actionSource;

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSimTypeInfoData::~RfxSimTypeInfoData() {
    // free memory
    if (m_data != NULL) {
        free(m_data);
        m_data = NULL;
    }
}
