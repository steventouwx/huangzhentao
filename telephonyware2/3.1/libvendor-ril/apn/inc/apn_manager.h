// SPDX-License-Identifier: MediaTekProprietary
/*
 * apn_manager.h
 *
 *  Created on: Nov 3, 2016
 *      Author: mtk54498
 */

#ifndef INC_APN_MANAGER_H_
#define INC_APN_MANAGER_H_

#include <pthread.h>
#include "apn_interface.h"
#include <sqlite3.h>
#include <string>
#include <libxml/tree.h>
#include "libxml/parser.h"
using std::string;

class apn_manager {
private:
    static const char* CREATE_TABLE_FORMAT;
    static const char* INSERT_TABLE_FORMAT;
    int mLoadState;
    pthread_t mSyncThread;
    sqlite3* mDb;
    apn_manager();
    apn_callback mStateCallback;
    inline bool handleSettingProp(apn_record_t*, xmlNode*, int collum);
    inline void pushSettingPropValue(apn_record_t*, int collum,
            const char* value);
    void insertAPNDb(apn_list_t* list);
    int isReady();
    int hasDbCreated(const char* db_path);
    apn_list_t* parseRes(const char* filename, const char* tag);
    void insertApnSettingXml(apn_list_t* apn_list, xmlNode* node);
    bool checkFile_exists(const char *filename);
public:
    static const char* DB_FILE_NAME;
    static const char* DB_FOLDER_NAME;
    static const char* TABLE_NAME;
    static const char* CONFIG_APNS_PATH;
    static const char* PARSE_TAG;
    static apn_manager* getInstance(apn_callback func);
    static apn_manager* forTestOnly();
    static int destroyInstance();
    ~apn_manager();
    void addStateListener(apn_callback func);
    void loadApnSettings(apn_list_t* apn_list, const char* tag,
            xmlNode* element);
    void notifyApnState(int state, void* data = NULL, bool remove = false);
    int initDb(const char* db_path);
    apnErrorCode doInsert(const apn_record_t* record);
    apn_list_t* doQuery(const apn_record_t* record);
    apnErrorCode doUpdate(const apn_record_t* record);
    apnErrorCode doDelete(const int id);
    apnErrorCode doDeleteInt(int id);
    apnErrorCode doDelete(const apn_record_t *record);
    apnErrorCode doCmd(apnSqlCmd cmd, const apn_record_t* record, void** data = NULL);

};

#endif /* INC_APN_MANAGER_H_ */
