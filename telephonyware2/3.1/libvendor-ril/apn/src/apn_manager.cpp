// SPDX-License-Identifier: MediaTekProprietary
/*
 * apn_manager.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: mtk54498
 */

#include "apn_manager.h"
#include "LogUtils.h"

#include <sys/stat.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <map>
#include <cstring>
#include <cstdio>
#include <vector>
#include <sstream>
using namespace std;

static apn_manager* INSTANCE = NULL;

/** default config file path */
const char* apn_manager::CONFIG_APNS_PATH = "/system/etc/tele/apns-conf.xml";

const char* apn_manager::TABLE_NAME = "apn_table";
const char* apn_manager::PARSE_TAG = "apn";

const char* apn_manager::INSERT_TABLE_FORMAT = "INSERT INTO %s(%s) values(%s);";

const char* apn_manager::CREATE_TABLE_FORMAT = "CREATE TABLE %s"
        "(_id INTEGER PRIMARY KEY,"
APN_PROP_TYPE " TEXT DEFAULT '',"
APN_PROP_MCC " TEXT DEFAULT '',"
APN_PROP_MNC " TEXT DEFAULT '',"
APN_PROP_CARRIER " TEXT DEFAULT '', "
APN_PROP_APN " TEXT DEFAULT '',"
APN_PROP_USER " TEXT DEFAULT '',"
APN_PROP_SERVER " TEXT DEFAULT '',"
APN_PROP_PASSWORD " TEXT DEFAULT '',"
APN_PROP_PROXY " TEXT DEFAULT '',"
APN_PROP_PORT " TEXT DEFAULT '',"
APN_PROP_MMSPROXY " TEXT DEFAULT '',"
APN_PROP_MMSPORT " TEXT DEFAULT '',"
APN_PROP_MMSC " TEXT DEFAULT '',"
APN_PROP_AUTH_TYPE " INTEGER DEFAULT -1,"
APN_PROP_CURRENT " INTEGER,"
APN_PROP_PROTOCOL " TEXT DEFAULT IP,"
APN_PROP_ROAMING_PROTOCOL " TEXT DEFAULT IP,"
APN_PROP_CARRIER_ENABLED " BOOLEAN DEFAULT 1,"
APN_PROP_BEARER " INTEGER DEFAULT 0,"
APN_PROP_BEARER_BITMASK " INTEGER DEFAULT 0,"
APN_PROP_MVNO_TYPE " TEXT DEFAULT '',"
APN_PROP_PROFILE_ID " INTEGER DEFAULT 0,"
APN_PROP_MODEM_COGNITIVE " BOOLEAN DEFAULT 0,"
APN_PROP_MTU " INTEGER DEFAULT 0,"
APN_PROP_USER_VISIBLE " BOOLEAN DEFAULT 1);";

const char* apn_manager::DB_FOLDER_NAME = "/data/vendor/telephony";
const char* apn_manager::DB_FILE_NAME = "/data/vendor/telephony/apn.db";

void* threadLoop(void* arg) {
    SLOGD("begin apn loading!");
    apn_manager* instance = (apn_manager*) arg;
    if (instance != NULL) {
        if (instance->initDb(apn_manager::DB_FILE_NAME) == APN_OK) {
            //load data from db
            SLOGD("notify apn db ready");
            instance->notifyApnState(APN_READY);
        } else {
            instance->notifyApnState(APN_NOREADY);
        }
        SLOGD("apn loading done!");
    }

    return NULL;
}

int query_callback(void* params, int argc, char** argv, char** cols) {
    apn_list_t* list = (apn_list_t*) params;
    apn_record_t* record = new apn_record_t();
    memset(record, 0, sizeof(apn_record_t));
    int idx = -1;
    for (int i = 0; i < argc; i++) {
        if (argv[i] == NULL || strlen(argv[i]) == 0) {
            continue;
        }
        idx = apn_get_idx_by_name(cols[i]);
        if (idx < 0) {
            SLOGFATAL("query_callback overrid %s", cols[i]);
            continue;
        }
        record->columnIdx[record->count] = idx;
        record->values[record->count] = strdup(argv[i]);
        record->count++;
    }
    apn_record_t* pre_item = list->top;
    if (pre_item == NULL) {
        list->top = record;
    } else {
        while (pre_item->next != NULL) {
            pre_item = pre_item->next;
        }
        pre_item->next = record;
    }
    list->count++;

    while (APN_DEBUG && record != NULL) {
        apn_dump_record(record);
        record = (apn_record_t*) record->next;
    }
    SLOGD("query callback result %d", list->count);
    return 0;
}

apn_manager::apn_manager() :
        mLoadState(APN_NOREADY), mStateCallback(NULL), mSyncThread(-1), mDb(
        NULL) {
}

apn_manager::~apn_manager() {
    if (mStateCallback != NULL) {
        mStateCallback = NULL;
    }
    if (mDb != NULL) {
        sqlite3_close(mDb);
        SLOGD("apn manager destory sqlite3");
        mDb = NULL;
    }
}

int apn_manager::isReady() {
    return mLoadState == APN_READY;
}

/* record need contains _id field */
apnErrorCode apn_manager::doUpdate(const apn_record_t* record) {
    if (record == NULL || record->count == 0) {
        return APN_ERR;
    }
    int _id = -1;
    const char *idvalue = NULL;
    for (_id = 0; _id < record->count; _id++) {
        if (APN_PROP_iId == record->columnIdx[_id]) {
            idvalue = record->values[_id];
            break;
        }
    }
    if (_id == record->count) {
        SLOGD("update error without _id");
        return APN_ERR;
    }
    string sql("update ");
    sql.append(TABLE_NAME);
    for (int i = 0; i < record->count; i++) {
        if (i == _id) {
            if (i == 0 && record->count > 1) {
                sql.append(" set ");
            }
            continue;
        }
        if (i == 0) {
            sql.append(" set ");
        }
        if (APN_PROP_ATTR[record->columnIdx[i]].prop_name != NULL)
            sql.append(APN_PROP_ATTR[record->columnIdx[i]].prop_name).append(" = ");
        switch (APN_PROP_ATTR[record->columnIdx[i]].type) {
        case PROP_TEXT:
            sql.append("'").append(record->values[i]).append("'");
            break;
        case PROP_BOOLEAN:
            if (atoi(record->values[i]) == 0) {
                sql.append("0");
            } else {
                sql.append("1");
            }
            break;
        case PROP_INTEGER:
            sql.append(record->values[i]);
            break;
        }
        if (i != (record->count - 1)) {
            sql.append(",");
        }
    }
    if (idvalue != NULL)
        sql.append(" where _id = ").append(idvalue);
    sql.append(";");
    char* errmsg = NULL;
    if (sqlite3_exec(mDb, sql.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        if (errmsg != NULL) {
            SLOGFATAL("update error %s", errmsg);
            sqlite3_free(errmsg);
        }
        return APN_ERR;
    }
    return APN_OK;
}

apnErrorCode apn_manager::doDeleteInt(const int id) {
    if (id <= 0) {
        SLOGFATAL("doDeleteInt id is invalid");
        return APN_ERR;
    }
    return doDelete(id);
}
apnErrorCode apn_manager::doDelete(const int id) {
    if (id < 0) {
        return APN_ERR;
    }
    char idvalue[32] = {0};
    int ret = sprintf(idvalue, "%d", id);
    if (ret < 0) {
        SLOGFATAL("sprintf error %d", ret);
        return APN_ERR;
    }
    string sql("delete from ");
    sql.append(TABLE_NAME).append(" where _id = ").append(idvalue);
    sql.append(";");
    char* errmsg = NULL;
    if (sqlite3_exec(mDb, sql.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        SLOGD("doDelete error %s", errmsg);
        sqlite3_free(errmsg);
        return APN_ERR;
    }

    if (APN_DEBUG)
        SLOGD("doDelete %s", sql.c_str());
    return APN_OK;
}

apnErrorCode apn_manager::doCmd(apnSqlCmd cmd, const apn_record_t* record,
        void** data) {
    apnErrorCode ret = APN_OK;
    if (record == NULL || record->count == 0) {
        return APN_ERR;
    }
    int idx = 0;
    switch (cmd) {
    case CMD_INSERT:
        if (record == NULL || record->count < 1) {
            SLOGD("doCmd %d record %s", cmd,
                    record == NULL?"emty":"count is empty");
            return APN_ERR;
        }
        ret = doInsert(record);
        break;
    case CMD_DELETE:
        if (record == NULL || record->count < 1) {
            SLOGD("doCmd %d record %s", cmd,
                    record == NULL?"emty":"count is empty");
            return APN_ERR;
        }

        for (idx = 0; idx < record->count; idx++) {
            if (record->columnIdx[idx] == APN_PROP_iId) {
                break;
            }
        }
        if (idx == record->count) {
            SLOGD("doCmd delete without id");
            ret = APN_ERR;
            break;
        }
        ret = doDelete(idx);
        break;
    case CMD_QUERY:
        if (record == NULL || record->count < 1) {
            SLOGD("doCmd query record %s",
                    record == NULL?"emty":"count is empty");
            return APN_ERR;
        }
        if (data == NULL) {
            SLOGD("doCmd query without output");
            ret = APN_ERR;
            break;
        }
        *data = (void*) doQuery(record);
        if (*data == NULL) {
            ret = APN_ERR;
        }
        break;
    case CMD_UPDATE:
        if (record == NULL || record->count < 1) {
            SLOGD("doCmd query record %s",
                    record == NULL?"emty":"count is empty");
            return APN_ERR;
        }

        ret = doUpdate(record);
        break;
    }
    if (ret == APN_OK) {
        SLOGD("doCmd %d success", cmd);
    }
    return ret;
}

apnErrorCode apn_manager::doInsert(const apn_record_t* instance) {
    //insert into db
    string collumnSql;
    string valueSql;
    stringstream insertSql;
    insertSql << "INSERT INTO " << TABLE_NAME << "(";
    for (int j = 0; j < instance->count; j++) {
        const prop_attr_t* attr = apn_get_prop_attr_by_idx(
                instance->columnIdx[j]);
        if (attr == NULL) {
            SLOGFATAL("doInsert attr NULL");
            return APN_ERR;
        }
        if (attr->prop_name != NULL)
            collumnSql.append(attr->prop_name);
        switch (attr->type) {
        case PROP_TEXT:
            valueSql.append("'").append(instance->values[j]).append("'");
            break;
        case PROP_INTEGER:
            valueSql.append(instance->values[j]);
            break;
        case PROP_BOOLEAN:
            if (atoi(instance->values[j]) == 0) {
                valueSql.append("1");
            } else {
                valueSql.append("1");
            }
            break;
        }

        if (j != (instance->count - 1)) {
            collumnSql.append(",");
            valueSql.append(",");
        }
    }
    insertSql << collumnSql << ") VALUES(" << valueSql << ");";
    if (APN_DEBUG)
        SLOGD("doInsert SQL %s", insertSql.str().c_str());
    char* msgerr = NULL;
    if (sqlite3_exec(mDb, insertSql.str().c_str(), NULL, NULL,
            &msgerr) != SQLITE_OK) {
        SLOGD("doInsert error %s", msgerr);
        sqlite3_free(msgerr);
        return APN_ERR;
    } else {
        SLOGD("doInsert success");
        apn_dump_record(instance);
        return APN_OK;
    }
}
apnErrorCode apn_manager::doDelete(const apn_record_t *record) {
    string sql("delete from ");
    sql.append(TABLE_NAME);
    if (record != NULL && record->count > 0) {
        sql.append(" where ");
        for (int i = 0; i < record->count; i++) {
            const prop_attr_t *attr = apn_get_prop_attr_by_idx(
                    record->columnIdx[i]);
            if (attr == NULL) {
                SLOGD("doDelete error: attr null");
                return APN_ERR;
            }
            if (attr->prop_name != NULL)
                sql.append(attr->prop_name).append("=");
            switch (attr->type) {
            case PROP_TEXT:
                sql.append("'").append(record->values[i]).append("'");
                break;
            case PROP_INTEGER:
                sql.append(record->values[i]);
                break;
            case PROP_BOOLEAN:
                if (atoi(record->values[i]) == 0) {
                    sql.append("0");
                } else {
                    sql.append("1");
                }
                break;
            }
            if (i != (record->count - 1)) {
                sql.append(" and ");
            }
        }
    }
    sql.append(";");
    char *errMsg = NULL;
    if (APN_DEBUG)
        SLOGD("do delete (%s)", sql.c_str());
    int rc = sqlite3_exec(mDb, sql.c_str(), NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        SLOGD("do delete error %s", errMsg);
        sqlite3_free(errMsg);
        return APN_ERR;
    }
    return APN_OK;
}

apn_list_t* apn_manager::doQuery(const apn_record_t* record) {
    apn_list_t* list = new apn_list_t();
    if (list == NULL) {
        SLOGFATAL("list new NULL");
        return NULL;
    }
    memset(list, 0, sizeof(apn_list_t));
    string sql("select * from ");
    sql.append(TABLE_NAME);
    if (record != NULL && record->count > 0) {
        sql.append(" where ");
        for (int i = 0; i < record->count; i++) {
            const prop_attr_t* attr = apn_get_prop_attr_by_idx(
                    record->columnIdx[i]);
            if (attr == NULL) {
                SLOGFATAL("doQuery attr NULL");
                apn_free_list(list);
                return NULL;
            }
            if (attr->prop_name != NULL)
                sql.append(attr->prop_name).append("=");
            switch (attr->type) {
            case PROP_TEXT:
                sql.append("'").append(record->values[i]).append("'");
                break;
            case PROP_INTEGER:
                sql.append(record->values[i]);
                break;
            case PROP_BOOLEAN:
                if (atoi(record->values[i]) == 0) {
                    sql.append("0");
                } else {
                    sql.append("1");
                }
                break;
            }
            if (i != (record->count - 1)) {
                sql.append(" and ");
            }
        }
    }
    sql.append(" order by _id;");
    char* errMsg = NULL;
    if (APN_DEBUG)
        SLOGD("do query (%s)", sql.c_str());
    int rc = sqlite3_exec(mDb, sql.c_str(), query_callback, list, &errMsg);
    if (rc != SQLITE_OK) {
        SLOGD("do query error %s", errMsg);
        sqlite3_free(errMsg);
        apn_free_list(list);
        return NULL;
    }
    SLOGD("do query result %d", list->count);
    return list;
}

bool apn_manager::checkFile_exists(const char *filename) {
    struct stat buf;
    if (stat(filename, &buf) < 0) {
        if (errno == ENOENT) {
            return false;
        }
    }
    return true;
}

void apn_manager::insertAPNDb(apn_list_t* list) {
    if (mDb == NULL) {
        SLOGD("Database hasn't inited!");
        return;
    }
    string sql("insert into ");
    sql.append(TABLE_NAME);

}

void apn_manager::pushSettingPropValue(apn_record_t* record, int collumn,
        const char* value) {
    int i = record->count;
    if ((collumn >= 0 && collumn < APN_PROP_iTOTAL) && value != NULL) {
        record->columnIdx[i] = collumn;
        record->values[i] = strdup(value);
        record->count++;
        if (APN_DEBUG)
            SLOGD("handleSettingProp %s / %s",
                    apn_get_prop_name((int ) collumn), value);
    }
}

bool apn_manager::handleSettingProp(apn_record_t* record, xmlNode* node,
        int collumn) {
    xmlChar *szAttr = xmlGetProp(node,BAD_CAST apn_get_prop_name((int) collumn));
    if (xmlHasProp(node, BAD_CAST apn_get_prop_name((int) collumn))) {
        pushSettingPropValue(record, collumn, (char*) szAttr);
        if(szAttr){
            xmlFree(szAttr);
        }
        return true;
    }
    if(szAttr){
        xmlFree(szAttr);
    }
    return false;
}
void apn_manager::insertApnSettingXml(apn_list_t* apn_list, xmlNode* node) {
    if (apn_list == NULL || node == NULL) {
        SLOGD("insertApnSettingXml %s empty", node == NULL?"NODE":"apn_list");
        return;
    }
    apn_record_t* record = new apn_record_t();
    memset(record, 0, sizeof(apn_record_t));
    int isValid = false;
    for (int i = 0; i < APN_PROP_iTOTAL; i++) {
        isValid |= handleSettingProp(record, node, i);
    }
    if (isValid) {
        apn_list->count++;
        apn_record_t* index = apn_list->top;
        if (index == NULL) {
            apn_list->top = record;
        } else {
            while (index->next != NULL) {
                index = index->next;
            }
            index->next = record;
        }

    } else {
        delete record;
    }

}

void apn_manager::loadApnSettings(apn_list_t* apn_list, const char* tag,
        xmlNode* element) {
    if (element == NULL || apn_list == NULL) {
        return;
    }
    xmlNode *cur_node = NULL;
    for (cur_node = element; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE
                && xmlStrcmp(cur_node->name, BAD_CAST (tag)) == 0) {
            insertApnSettingXml(apn_list, cur_node);
        }
        loadApnSettings(apn_list, tag, cur_node->children);
    }
}

apn_list_t* apn_manager::parseRes(const char* filename, const char* tag) {
    xmlDocPtr doc = NULL;
    xmlNode *root_element = NULL;
    apn_list_t* apn_list = NULL;
    if (!checkFile_exists(filename)) {
        SLOGD("parseRes file not exist %s", filename);
        return NULL;
    }
    doc = xmlParseFile(filename);
    if (doc == NULL) {
        SLOGD("parseRes %s!", xmlGetLastError()->message);
        return NULL;
    }
    root_element = xmlDocGetRootElement(doc);
    if (root_element != NULL) {
        apn_list = new apn_list_t();
        if (apn_list == NULL) {
            SLOGD("apn_list new NULL");
            return NULL;
        }
        memset(apn_list, 0, sizeof(apn_list_t));
        loadApnSettings(apn_list, tag, root_element);
        SLOGD("parseRes numer:%d", apn_list->count);

    } else {
        SLOGD("parse file may not have root element! %s", CONFIG_APNS_PATH);
    }
    if (doc != NULL) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
    }
    return apn_list;
}

int apn_manager::hasDbCreated(const char* db_path) {
    int rc = sqlite3_open_v2(db_path, &mDb, SQLITE_OPEN_READWRITE,
    NULL);
    if (rc == SQLITE_OK) {
        SLOGD("check init success!");
        return APN_OK;
    } else {
        SLOGD("db has not create %s!", sqlite3_errmsg(mDb));
        sqlite3_close_v2(mDb);
        return APN_ERR;
    }
}

int apn_manager::initDb(const char* db_path) {
    if (db_path == NULL || strlen(db_path) == 0) {
        SLOGD("initDb error with null path!");
        return APN_ERR;
    }
    if (APN_OK == hasDbCreated(db_path)) {
        SLOGD("initDb done!");
        return APN_OK;
    }

    struct stat buffer;
    if (stat(DB_FOLDER_NAME, &buffer) != 0) {
        int status = mkdir(DB_FOLDER_NAME, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status != 0) {
            SLOGD("file:%s runtime create error(%s)!", DB_FOLDER_NAME, strerror(errno));
            return APN_ERR;
        }
    }

    int rc = sqlite3_open(db_path, &mDb);
    if (rc != SQLITE_OK) {
        SLOGD("initDb failed %s", sqlite3_errmsg(mDb));
        return APN_ERR;
    } else {
        SLOGD("create db in %s", db_path);
    }
    //create table
    char* err = NULL;
    int sqlLen = strlen(CREATE_TABLE_FORMAT) + strlen(TABLE_NAME) + 1;
    char* table_sql = new char[sqlLen];
    int ret = 0;
    memset(table_sql, 0, sqlLen);
    ret = sprintf(table_sql, CREATE_TABLE_FORMAT, TABLE_NAME);
    if (ret < 0) {
        SLOGD("initDb sprintf failed!");
        return APN_ERR;
    }
    rc = sqlite3_exec(mDb, table_sql, NULL, NULL, &err);
    delete[] table_sql;
    if (rc != SQLITE_OK) {
        SLOGD("initDb do init error %s", err);
        sqlite3_free(err);
        return APN_ERR;
    }
    // loading data from xml
    apn_list_t* apn_list = parseRes(apn_manager::CONFIG_APNS_PATH,
            apn_manager::PARSE_TAG);
    if (apn_list == NULL) {
        SLOGD("initDb parseRes failed!");
        return APN_ERR;
    }
    //insert into db
    vector<apn_record_t*> failedList;
    string collumnSql;
    string valueSql;
    stringstream insertSql;
    apn_record_t* instance = apn_list->top;
    for (int i = 0; i < apn_list->count; i++) {
        collumnSql.clear();
        valueSql.clear();
        insertSql.clear();
        insertSql.str("");
        insertSql << "INSERT INTO " << TABLE_NAME << "(";
        for (int j = 0; j < instance->count; j++) {
            const prop_attr_t* attr = apn_get_prop_attr_by_idx(
                    instance->columnIdx[j]);
            if (attr == NULL) {
                SLOGD("initDb attr NULL");
                apn_free_list(apn_list);
                return APN_ERR;
            }
            if (attr->prop_name != NULL)
                collumnSql.append(attr->prop_name);
            switch (attr->type) {
            case PROP_TEXT:
                valueSql.append("'").append(instance->values[j]).append("'");
                break;
            case PROP_INTEGER:
                valueSql.append(instance->values[j]);
                break;
            case PROP_BOOLEAN:
                if (atoi(instance->values[j]) == 0) {
                    valueSql.append("0");
                } else {
                    valueSql.append("1");
                }
                break;
            }

            if (j != (instance->count - 1)) {
                collumnSql.append(",");
                valueSql.append(",");
            }
        }
        insertSql << collumnSql << ") VALUES(" << valueSql << ");";
        if (APN_DEBUG)
            SLOGD("insert SQL is %s", insertSql.str().c_str());
        char* msgerr = NULL;
        if (sqlite3_exec(mDb, insertSql.str().c_str(), NULL, NULL,
                &msgerr) != SQLITE_OK) {
            SLOGD("insert xml data error %s", msgerr);
            sqlite3_free(msgerr);
        } else if (APN_DEBUG) {
            SLOGD("insert xml data No. %d", i + 1);
        }

        instance = (apn_record_t*) instance->next;
    }
    apn_free_list(apn_list);
    SLOGD("apn db do init done!");
    return APN_OK;
}

void apn_manager::addStateListener(apn_callback func) {
    if (func != NULL) {
        (*func)(mLoadState, NULL);
    }
    INSTANCE->mStateCallback = func;
}

apn_manager* apn_manager::forTestOnly() {
    return new apn_manager();
}

apn_manager* apn_manager::getInstance(apn_callback func) {
    if (INSTANCE == NULL) {
        INSTANCE = new apn_manager();
        if (func != NULL) {
            INSTANCE->addStateListener(func);
        }
        pthread_attr_t attr;
        pthread_attr_init (&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        int rc = pthread_create(&INSTANCE->mSyncThread, &attr, threadLoop,
                (void*) INSTANCE);
        if (rc) {
            SLOGD("start thread error %d", rc);
            assert(0);
            return NULL;
        }
                pthread_attr_destroy(&attr);
    }
    return INSTANCE;
}

int apn_manager::destroyInstance() {
    if (INSTANCE != NULL) {
        INSTANCE->notifyApnState(APN_DESTROYED, NULL, true);
        if (INSTANCE->mDb != NULL) {
            if (sqlite3_close(INSTANCE->mDb) != SQLITE_OK) {
                SLOGFATAL("destroyInstance Error!");
            }
        }
        delete INSTANCE;
        INSTANCE = NULL;
    }
    return APN_OK;
}

void apn_manager::notifyApnState(int state, void* data, bool remove) {
    SLOGD("notify State %d", state);
    apn_callback pfunc = mStateCallback;
    if (remove) {
        mStateCallback = NULL;
    }
    if (pfunc != NULL) {
        (*pfunc)(state, NULL);
    }
}

