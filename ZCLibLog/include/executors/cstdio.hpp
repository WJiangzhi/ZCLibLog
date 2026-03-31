//
// Created by wanjiangzhi on 2026/3/30.
//

#ifndef ZCLIBLOG_EXECUTORS_CSTDIO_HPP
#define ZCLIBLOG_EXECUTORS_CSTDIO_HPP

#include <cstdio>
#include "../logger_classes.hpp"

namespace ZCLibLog {
    namespace executors {
        executor cstdio = [](ELString msg, ELogLevel lv) {
            if (lv >= LogLevel_ERROR)
                fputs(msg.c_str(), stderr);
            else
                fputs(msg.c_str(), stdout);
        };
    }
}

#endif //ZCLIBLOG_EXECUTORS_CSTDIO_HPP
