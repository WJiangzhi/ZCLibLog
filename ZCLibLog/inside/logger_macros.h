//
// Created by wanjiangzhi on 2026/4/4.
//

#ifndef ZCLIBLOG_LOGGER_MACROS_H
#define ZCLIBLOG_LOGGER_MACROS_H

#if __cplusplus >= 201703L
    #define ZCLibLog_NODISCARD [[nodiscard]]
#else
    #define ZCLibLog_NODISCARD
#endif

#endif //ZCLIBLOG_LOGGER_MACROS_H
