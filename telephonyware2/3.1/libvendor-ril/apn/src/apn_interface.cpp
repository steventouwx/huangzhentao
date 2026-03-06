// SPDX-License-Identifier: MediaTekProprietary
/*
 * apn_interface.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: mtk54498
 */

#include <cstdlib>
#include <cstring>
#include "apn_interface.h"
#include "apn_manager.h"
#include "LogUtils.h"

int APN_DEBUG = 0;

prop_attr_t APN_PROP_ATTR[APN_PROP_iTOTAL] = {
        { APN_PROP_ID,PROP_INTEGER },
        { APN_PROP_TYPE, PROP_TEXT },
        { APN_PROP_MCC, PROP_TEXT },
        { APN_PROP_MNC, PROP_TEXT },
        {APN_PROP_CARRIER, PROP_TEXT },
        { APN_PROP_APN, PROP_TEXT },
        { APN_PROP_PORT, PROP_TEXT },
        { APN_PROP_PROXY, PROP_TEXT },
        { APN_PROP_MMSC, PROP_TEXT },
        { APN_PROP_MMSPROXY, PROP_TEXT },
        { APN_PROP_MMSPORT, PROP_TEXT },
        { APN_PROP_USER, PROP_TEXT },
        { APN_PROP_PASSWORD, PROP_TEXT },
        { APN_PROP_AUTH_TYPE, PROP_INTEGER },
        { APN_PROP_SERVER, PROP_TEXT },
        { APN_PROP_PROTOCOL, PROP_TEXT },
        { APN_PROP_ROAMING_PROTOCOL, PROP_TEXT },
        { APN_PROP_MTU, PROP_INTEGER },
        { APN_PROP_CARRIER_ENABLED, PROP_BOOLEAN },
        { APN_PROP_BEARER, PROP_INTEGER },
        { APN_PROP_BEARER_BITMASK, PROP_INTEGER },
        { APN_PROP_PROFILE_ID, PROP_INTEGER },
        { APN_PROP_MODEM_COGNITIVE, PROP_BOOLEAN },
        { APN_PROP_MAXCONNS, PROP_TEXT },
        { APN_PROP_WAITTIME, PROP_TEXT },
        { APN_PROP_MAXCONNSTIME, PROP_TEXT },
        { APN_PROP_MVNO_TYPE, PROP_TEXT },
        { APN_PROP_USER_VISIBLE, PROP_BOOLEAN },
        { APN_PROP_CURRENT, PROP_INTEGER }
};

int apn_init(apn_callback func) {
    apn_manager* instance = apn_manager::getInstance(func);
    if (instance != NULL) {
        return APN_OK;
    } else {
        return APN_ERR;
    }
}

int apn_destroy() {
    SLOGD("do apn destroy");
    return apn_manager::destroyInstance();
}

int apn_query_db(const apn_record_t* record, apn_list_t** list) {
    if (list == NULL) {
        SLOGD("apn query has no valid output!");
        return APN_ERR;
    }
    *list = apn_manager::getInstance(NULL)->doQuery(record);
    return APN_OK;
}

int apn_update_record_db(const apn_record_t * record) {
    return apn_manager::getInstance(NULL)->doUpdate(record);
}

int apn_delete_record_db(int id) {
    return apn_manager::getInstance(NULL)->doDeleteInt(id);
}

int apn_delete_db(const apn_record_t *record) {
    return apn_manager::getInstance(NULL)->doDelete(record);
}
int apn_insert_record_db(const apn_record_t * record) {
    return apn_manager::getInstance(NULL)->doInsert(record);
}
int apn_cmd_record_db(apnSqlCmd cmd, const apn_record_t* record, void ** data) {
    return apn_manager::getInstance(NULL)->doCmd(cmd, record, data);
}

void apn_free_record(apn_record_t* record) {
    if (record != NULL) {
        for (int i = 0; i < record->count; i++) {
            if(record->values[i]) {
            free(record->values[i]);
            }
        }
        delete record;
    }
}

apn_record_t* apn_malloc_record() {
    apn_record_t* instance = new apn_record_t();
    memset(instance, 0, sizeof(apn_record_t));
    return instance;
}

int apn_get_idx_by_name(const char* name) {
    if (name == NULL) {
        return APN_ERR;
    }
    int i = 0;
    for (; i < APN_PROP_iTOTAL; i++) {
        if (strcmp(APN_PROP_ATTR[i].prop_name, name) == 0) {
            if (APN_DEBUG)
                SLOGD("get idx (%d) by name(%s)", i, name);
            return i;
        }
    }
    SLOGD("apn_get_prop_attr error(%s)!", name);
    return APN_ERR;
}

const prop_attr_t* apn_get_attr_by_name(const char* name) {
    if (name == NULL) {
        return NULL;
    }
    int i = 0;
    for (; i < APN_PROP_iTOTAL; i++) {
        if (strcmp(APN_PROP_ATTR[i].prop_name, name) == 0) {
            return &APN_PROP_ATTR[i];
        }
    }
    SLOGFATAL("apn_get_prop_attr error(%s)", name);
    return NULL;
}

int apn_build_iRecord(apn_record_t* record, int col,
        const char* value) {
    if (record == NULL || NULL == value) {
        SLOGD("apn_update_record empty");
        return APN_ERR;
    }
    if (col < 0 || col > APN_PROP_iTOTAL) {
        return APN_ERR;
    }
    record->columnIdx[record->count] = col;
    record->values[record->count] = strdup(value);
    record->count++;
    SLOGD("apn update irecord column (%d:%s)", col, value);
    return APN_OK;
}

int apn_build_cRecord(apn_record_t* record, const char* prop,
        const char* value) {
    if (record == NULL || prop == NULL || NULL == value) {
        SLOGD("apn_update_record empty");
        return APN_ERR;
    }
    const prop_attr_t* attr = apn_get_attr_by_name(prop);
    if (attr == NULL) {
        return APN_ERR;
    }
    int column = apn_get_idx_by_name(attr->prop_name);
    record->columnIdx[record->count] = column;
    record->values[record->count] = strdup(value);
    record->count++;
    SLOGD("apn update record column (%s:%s)", prop, value);
    return APN_OK;
}

void apn_dump_record(const apn_record_t* record) {
    string dump("");
    for (int i = 0; i < record->count; i++) {
        if (record->values[i] != NULL) {
            dump.append(apn_get_prop_name(record->columnIdx[i])).append("/").append(
                    record->values[i]);
        }
        if (i != (record->count - 1)) {
            dump.append("; ");
        }
    }
    SLOGD("dump %s", dump.c_str());
}

const char* apn_get_prop_from_record(const apn_record_t* record, int idx) {
    if (idx < 0 || idx > APN_PROP_iTOTAL) {
        return NULL;
    }
    for (int i = 0; i < record->count; i++) {
        if (idx == record->columnIdx[i]) {
            return record->values[i];
        }
    }
    return NULL;
}

void apn_free_list(apn_list_t* list) {
    if (list != NULL) {
        apn_record_t* doFree = NULL;
        while (list->top != NULL) {
            doFree = list->top;
            list->top = doFree->next;
            list->count--;
            apn_free_record(doFree);
        }
        delete list;
    }
}

const char* apn_get_prop_name(int idx) {
    return APN_PROP_ATTR[idx].prop_name;
}

int apn_get_prop_value_type(int idx) {
    return APN_PROP_ATTR[idx].type;
}

int apn_get_prop_idx(prop_attr_t* attr) {
    if (attr == NULL) {
        return -1;
    }
    int size = (attr - &APN_PROP_ATTR[0]) % sizeof(prop_attr_t);
    if (size >= 0 && size < APN_PROP_iTOTAL) {
        return size;
    } else {
        return -1;
    }
}

const prop_attr_t* apn_get_prop_attr_by_idx(int idx) {
    if (idx < 0 || idx > APN_PROP_iTOTAL) {
        SLOGFATAL("get prop_attr_idx error(%d)", idx);
        return NULL;
    }
    return &APN_PROP_ATTR[idx];
}

int apn_get_default_profile_id(const char* apnType) {
    if (0 == strcmp(apnType, APN_TYPE_IMS)) {
        return DATA_PROFILE_IMS;
    } else if (0 == strcmp(apnType, APN_TYPE_FOTA)) {
        return DATA_PROFILE_FOTA;
    } else if (0 == strcmp(apnType, APN_TYPE_CBS)) {
        return DATA_PROFILE_CBS;
    } else if (0 == strcmp(apnType, APN_TYPE_IA)) {
        return DATA_PROFILE_DEFAULT; // DEFAULT for now
    } else if (0 == strcmp(apnType, APN_TYPE_DUN)) {
        return DATA_PROFILE_TETHERED;
    } else {
        return DATA_PROFILE_DEFAULT;
    }
}
