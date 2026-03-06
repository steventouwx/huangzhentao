// SPDX-License-Identifier: MediaTekProprietary
/*
 * LogUtils.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: mtk54498
 */

#include <LogUtils.h>
#include <iostream>
#include <stdarg.h>
#include <sstream>
#include <string>
#include <cstring>
#include <assert.h>
#include <stdio.h>

using namespace std;
using logutils::SLOG;

#define LOG_BUF_SIZE 1024

const char* SLOG::SEPERATE = ": ";

void SLOG::D(const char* tag, const char* fmt, ...) {
    va_list vargs;
    va_start(vargs, fmt);
    printfLog(tag, "d", fmt, vargs);
    va_end(vargs);
}

void SLOG::E(const char* tag, const char* fmt, ...) {
    va_list vargs;
    va_start(vargs, fmt);
    printfLog(tag, "e", fmt, vargs);
    va_end(vargs);
}

void SLOG::FATAL(const char* tag, const char* fmt, ...) {
    va_list vargs;
    va_start(vargs, fmt);
    printfLog(tag, "e", fmt, vargs);
    va_end(vargs);
    //assert(0);
}

#ifdef TAGLOG_SUPPORT

//#include <log/log.h>
#include <android/log.h>
#include <log/log.h>
void SLOG::printfLog(const char* tag, const char* debug, const char* fmt,
        va_list args) {
    char buf[LOG_BUF_SIZE]= {'\0'};
    int ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, args);
    if (ret >= 0 && ret < LOG_BUF_SIZE)
        __android_log_buf_write(LOG_ID_SYSTEM, ANDROID_LOG_DEBUG, tag, buf);
}

#else

void SLOG::printfLog(const char* tag, const char* debug, const char* fmt,
        va_list args) {
    char buf[LOG_BUF_SIZE] = { '\0' };
    vsnprintf(buf, LOG_BUF_SIZE, fmt, args);
    std::cout << tag << "/" << debug << SEPERATE << buf << std::endl;
}
#endif
