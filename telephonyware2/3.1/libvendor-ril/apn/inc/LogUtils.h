// SPDX-License-Identifier: MediaTekProprietary
/*
 * LogUtils.h
 *
 *  Created on: Oct 14, 2016
 *      Author: mtk54498
 */
#ifndef SRC_LogUtils_H_
#define SRC_LogUtils_H_
#include <stdarg.h>
#include <assert.h>

#undef LOG_TAG
#define LOG_TAG "APN"

namespace logutils {
class SLOG {
private:
    const static char* SEPERATE;
public:
    static void printfLog(const char* tag, const char* debug, const char* fmt,
            va_list args);
    static void D(const char* tag, const char* fmt, ...);
    static void E(const char* tag, const char* fmt, ...);
    static void FATAL(const char* tag, const char* fmt, ...);
};
}

#define SLOGD(...) logutils::SLOG::D(LOG_TAG, ##__VA_ARGS__)
#define SLOGFATAL(f, a...) logutils::SLOG::FATAL(LOG_TAG, f "(%s, %d)", __FILE__, __LINE__,## a)

#endif /* SRC_LogUtils_H_ */
