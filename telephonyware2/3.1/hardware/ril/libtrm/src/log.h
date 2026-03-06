// #include <syslog.h>  //openlog(), syslog(), closelog()
#include <mtk_log.h>

#define DEBUG 1

#undef LOG_TAG_TRM
#define LOG_TAG_TRM "trm_lib"

#if DEBUG
#define LOGD(...) ((void)mtkLogD(LOG_TAG_TRM, __VA_ARGS__))

#define LOGI(...) ((void)mtkLogI(LOG_TAG_TRM, __VA_ARGS__))

#define LOGW(...) ((void)mtkLogW(LOG_TAG_TRM, __VA_ARGS__))

#define LOGE(...) ((void)mtkLogE(LOG_TAG_TRM, __VA_ARGS__))
#else
#define LOGD(...)   {\
        syslog(LOG_DEBUG, ## __VA_ARGS__);}

#define LOGI(...)   {\
        syslog(LOG_INFO, ## __VA_ARGS__);}

#define LOGW(...)   {\
        syslog(LOG_WARNING, ## __VA_ARGS__);}

#define LOGE(...)   {\
        syslog(LOG_ERR, ## __VA_ARGS__);}
#endif



