// SPDX-License-Identifier: MediaTekProprietary
/*
 * sample.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: mtk54498
 */

#include <apn_interface.h>
#include <stdio.h>
#include <unistd.h>

void callback(int ret, void* data) {
    if (ret == APN_READY) {
        apn_record_t* record = apn_malloc_record();
        apn_build_cRecord(record, APN_PROP_MCC, "202");
        apn_build_iRecord(record, APN_PROP_iMNC, "01");
        apn_list_t* list = 0;
        apn_query_db(record, &list);
        apn_record_t* instance = list->top;
        while(instance != NULL) {
            apn_dump_record(instance);
            instance = instance->next;
        }
        // free all the malloc memory , in case of memory leak
        apn_free_record(record);
        apn_free_list(list);
        apn_destroy();
        printf("apn loaded finished\n");
        exit(0);
    }
}

int main() {
    apn_init(callback);
    printf("job finished!\n");
    pause();
    return 0;
}
