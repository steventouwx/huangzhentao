/* SPDX-License-Identifier: MediaTekProprietary */

#include <string>
#include <map>
#include <queue>
#include <pthread.h>
#include <prop/properties.h>
#include <mtk_log.h>
#include "property_map.h"
#include <fstream>
#include <cstring>
#include <unistd.h>

std::map<std::string, std::string> taskMap;
pthread_mutex_t mapMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditionVar = PTHREAD_COND_INITIALIZER;
static bool threadStarted = false;

#undef LOG_TAG
#define LOG_TAG "MTK_PROPERTY"

extern "C" bool isAndroidSuspended() {
    std::string pm_state;
    std::ifstream pm_state_file("/sys/guest_os/android/pm_state");

    if (!pm_state_file.is_open()) {
        return false;
    }
    std::getline(pm_state_file, pm_state);
    pm_state_file.close();
    return pm_state == "suspend";
}


void* thread_function(void* arg) {
    while (true) {
        std::pair<std::string, std::string> task;

        pthread_mutex_lock(&mapMutex);

        while (taskMap.empty()) {
            pthread_cond_wait(&conditionVar, &mapMutex);
        }
        auto it = taskMap.begin();
        task = *it;
        taskMap.erase(it);

        pthread_mutex_unlock(&mapMutex);

        int max_retries = 120;
        int retry_count = 0;
        while (retry_count < max_retries) {
            // char property_value[PROPERTY_VALUE_MAX] = { 0 };
            // memset(property_value, 0, sizeof(property_value));
            // property_get(task.first.c_str(), property_value, "0");
            int ret = property_set_only_android(task.first.c_str(), task.second.c_str(), 0);
            if(ret < 0) {
                mtkLogE(LOG_TAG, "property_set_only_android failed, key = %s, value = %s, count=%d",
                task.first.c_str(), task.second.c_str(), retry_count);
                retry_count++;
                sleep(1);
            } else {
                //mtkLogD(LOG_TAG, "property_set_only_android successed, key = %s, value = %s, count=%d",
                //task.first.c_str(), task.second.c_str(), retry_count);
                break;
            }

            if (retry_count == max_retries) {
                mtkLogE(LOG_TAG, "Failed to property_set_only_android fter %d retries\n", max_retries);
                break;
            }
        }
    }
}


extern "C" void set_property(const char *key, const char *value) {
    //mtkLogD(LOG_TAG, "set_property start");

    pthread_mutex_lock(&mapMutex);

    taskMap[std::string(key)] = std::string(value);
    pthread_cond_signal(&conditionVar);

    pthread_mutex_unlock(&mapMutex);

    if (!threadStarted) {
        pthread_mutex_lock(&mapMutex);
        if (!threadStarted) {
            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            mtkLogD(LOG_TAG, "create the worker thread");
            if (pthread_create(&tid, &attr, &thread_function, NULL) == 0) {
                //pthread_detach(tid);
                threadStarted = true;
            } else {
                mtkLogE(LOG_TAG, "Failed to create the worker thread");
            }
        }
        pthread_mutex_unlock(&mapMutex);
    }
}