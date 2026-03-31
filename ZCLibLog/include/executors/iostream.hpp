//
// Created by wanjiangzhi on 2026/3/30.
//

#ifndef ZCLIBLOG_EXECUTORS_IOSTREAM_HPP
#define ZCLIBLOG_EXECUTORS_IOSTREAM_HPP

#include <iostream>
#include "../logger_classes.hpp"

namespace ZCLibLog {
    namespace executors {
        executor iostream = [](ELString msg, ELogLevel lv) {
            if (lv >= LogLevel_ERROR)
                std::cerr << msg << std::endl;
            else if (lv >= LogLevel_INFO)
                std::cout << msg << std::endl;
            else
                std::clog << msg << std::endl;
        };
    }
}

#endif //ZCLIBLOG_EXECUTORS_IOSTREAM_HPP
