/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/syslog.h>
#include "atcid_mlclient_dispatch.h"
#include "ML_Vcall.h"
#include "ML_Data.h"
#include "property/mtk_properties.h"

#define ATCI_SIM "persist.vendor.service.atci.sim"

// Define a structure to hold the parsed results
typedef struct ParsedResult {
    int type;
    char *number;
} ParsedResult;

// Function to free allocated resources in ParsedResult
void freeParsedResult(ParsedResult *result) {
    if (result != NULL) {
        if(result->number != NULL) {
            free(result->number);  // Free the string if it was allocated
        }
        free(result);          // Free the structure itself
    }
}

// Function to parse the input string and return a ParsedResult structure
ParsedResult *parseString(const char *input) {
    // Allocate memory for the result structure
    ParsedResult *result = malloc(sizeof(ParsedResult));
    if (result == NULL) {
        LOGATCI(LOG_ERR, "Memory allocation failed\n");
        return NULL;
    }
    result->type = 0;  // Initialize the number to 0
    result->number = NULL;

    // Check if the first character is a digit
    if (isdigit(input[0])) {
        // Make a copy of the input string to use with strtok (which modifies the string)
        char *inputCopy = strdup(input);
        char *token = strtok(inputCopy, ",");
        if (token != NULL) {
            // Convert the token to an integer
            result->type = atoi(token);

            // Get the next token, which should be the string enclosed in quotes
            token = strtok(NULL, ",");
            if (token != NULL && token[0] == '"' && token[strlen(token) - 1] == '"') {
                // Allocate memory for the string and remove the quotes
                result->number = strndup(token + 1, strlen(token) - 2);
            }
        }
        free(inputCopy);  // Free the copy of the input string
    } else {
        // The input is just a string enclosed in quotes, remove the quotes
        if (input[0] == '"' && input[strlen(input) - 1] == '"') {
            result->number = strndup(input + 1, strlen(input) - 2);
        }
    }

    return result;
}

char* removeSemicolon(const char* input) {
    // Check if the input is NULL or empty
    if (input == NULL || *input == '\0') {
        return NULL;
    }

    size_t length = strlen(input);
    // Allocate memory for the new string (excluding the semicolon and including the null terminator)
    char* result = (char*)malloc(length); // 'length' already includes space for null terminator

    if (result == NULL) {
        // Memory allocation failed
        LOGATCI(LOG_ERR, "Memory allocation failed\n");
        return NULL;
    }

    // Copy the input to the result, excluding the semicolon
    strncpy(result, input, length - 1);
    // Ensure the string is null-terminated
    result[length - 1] = '\0';

    return result;
}

int atd_cmd_handler(char* cmdline, ATOP_t at_op, char* response, char* prefix) {
    LOGATCI(LOG_INFO, "atd_cmd_handler: %s,%s", cmdline, prefix);
    char cmd[MAX_AT_CMD] = {0};
    memset(cmd, 0, sizeof(cmd));
    get_substring(cmdline, strlen(prefix), strlen(cmdline) - strlen(prefix), cmd);
    if (strlen(cmd) > 0) {
        LOGATCI(LOG_INFO, "atd_cmd_handler: number=%s", cmd);
        char* number = removeSemicolon(cmd);

        if (number != NULL) {
            LOGATCI(LOG_INFO, "number: %s\n", number);
            int old_slot = ML_GetVcallDefaultSim();
            char atci_sim[MTK_PROPERTY_VALUE_MAX] = {0};
            mtk_property_get(ATCI_SIM, atci_sim, "0");
            int slotId = atoi(atci_sim);
            ML_SetVcallDefaultSim(slotId);
            int ret = ML_VcallStart(number);
            ML_SetVcallDefaultSim(old_slot);
            if (ret != 0) {
                free(number);
                sprintf(response, "\r\nERROR\r\n");
                return AT_ERROR;
            } else {
                sprintf(response, "\r\nOK\r\n");
            }
            free(number);  // Don't forget to free the allocated memory!
        } else {
            LOGATCI(LOG_ERR, "atd_cmd_handler: number is null");
            sprintf(response, "\r\nERROR\r\n");
            return AT_ERROR;
        }

    } else {
        sprintf(response, "\r\nERROR\r\n");
        return AT_ERROR;
    }
    ALOGI("atd_cmd_handler %s", response);
    return AT_OK;
}

int dial_cmd_handler(char* cmdline, ATOP_t at_op, char* response, char* prefix) {
    LOGATCI(LOG_INFO, "dial_cmd_handler: %s,%s", cmdline, prefix);
    if (strlen(cmdline) > 0) {
        LOGATCI(LOG_INFO, "dial_cmd_handler: number=%s", cmdline);
        ParsedResult* result = parseString(cmdline);
        if (result != NULL) {
            if(result->number == NULL) {
                freeParsedResult(result);
                sprintf(response, "\r\nERROR\r\n");
                return AT_ERROR;
            }
            LOGATCI(LOG_INFO, "dial_cmd_handler, type: %d, number: %s\n", result->type, result->number);
            int old_slot = ML_GetVcallDefaultSim();
            char atci_sim[MTK_PROPERTY_VALUE_MAX] = {0};
            mtk_property_get(ATCI_SIM, atci_sim, "0");
            int slotId = atoi(atci_sim);
            ML_SetVcallDefaultSim(slotId);
            int ret = ML_VcallCliStart(result->number, result->type);
            ML_SetVcallDefaultSim(old_slot);
            if (ret != 0) {
                freeParsedResult(result);
                sprintf(response, "\r\nERROR\r\n");
                return AT_ERROR;
            } else {
                sprintf(response, "\r\nOK\r\n");
            }
            freeParsedResult(result);
        } else {
            LOGATCI(LOG_INFO, "dial_cmd_handler: format fail");
            sprintf(response, "\r\nERROR\r\n");
            return AT_ERROR;
        }
    } else {
        sprintf(response, "\r\nERROR\r\n");
        return AT_ERROR;
    }
    ALOGI("dial_cmd_handler %s", response);
    return AT_OK;
}

int mute_cmd_handler(char* cmdline, ATOP_t at_op, char* response, char* prefix){
    LOGATCI(LOG_INFO, "mute_cmd_handler no implication: %s,%s", cmdline,prefix);
    sprintf(response,"\r\nNot Implement\r\n");
    return AT_NOT_IMPL;
}

int conference_cmd_handler(char* cmdline, ATOP_t at_op, char* response, char* prefix) {
    LOGATCI(LOG_INFO, "conference_cmd_handler no implication: %s,%s", cmdline,prefix);
    sprintf(response,"\r\nNot Implement\r\n");
    return AT_NOT_IMPL;
}

int ss_cmd_handler(char* cmdline, ATOP_t at_op, char* response, char* prefix) {
    LOGATCI(LOG_INFO, "ss_cmd_handler no implication: %s,%s", cmdline,prefix);
    sprintf(response,"\r\nNot Implement\r\n");
    return AT_NOT_IMPL;
}

int filght_cmd_handler(char* cmdline, ATOP_t at_op, char* response, char* prefix) {
    LOGATCI(LOG_INFO, "filght_cmd_handler: %s,%s", cmdline,prefix);
    if(strlen(cmdline) > 0) {
    LOGATCI(LOG_INFO, "filght_cmd_handler: mode=%s", cmdline);
        int ret = ML_SetAirplaneMode(atoi(cmdline));
        if(ret != 0) {
            sprintf(response,"\r\nERROR\r\n");
            return AT_ERROR;
        } else {
            sprintf(response,"\r\nOK\r\n");
        }
    } else {
        sprintf(response,"\r\nERROR\r\n");
        return AT_ERROR;
    }
    ALOGI ("filght_cmd_handler %s",response);
    return AT_OK;
}

int ims_cmd_handler(char* cmdline, ATOP_t at_op, char* response, char* prefix) {
    LOGATCI(LOG_INFO, "ims_cmd_handler: %s,%s", cmdline,prefix);
    if(strlen(cmdline) > 0) {
    LOGATCI(LOG_INFO, "ims_cmd_handler: enable=%s", cmdline);
        int ret = ML_EnableIms(atoi(cmdline));
        if(ret != 0) {
            sprintf(response,"\r\nERROR\r\n");
            return AT_ERROR;
        } else {
            sprintf(response,"\r\nOK\r\n");
        }
    } else {
        sprintf(response,"\r\nERROR\r\n");
        return AT_ERROR;
    }
    ALOGI ("ims_cmd_handler %s",response);
    return AT_OK;
}

