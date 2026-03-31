//
// Created by TingIAAI on 2026/3/30.
//

#ifndef ZCLIBLOG_FORMATTERS_STRINGSTREAM_HPP
#define ZCLIBLOG_FORMATTERS_STRINGSTREAM_HPP

#include <ctime>
#include <sstream>
#include <iomanip>
#include "../logger_classes.hpp"

namespace ZCLibLog {
    namespace formatters {
        template<typename... Args>
        FRString stringstream(FLogPack pack, FLString fmt, Args&&... args) {
            thread_local char buffer[4096];
            int len = std::snprintf(buffer, sizeof(buffer), fmt, std::forward<Args>(args)...);
            if (len < 0) return {};
            if (len >= static_cast<int>(sizeof(buffer))) len = sizeof(buffer) - 1;

            const std::string f_msg(buffer, len);

            std::time_t t = static_cast<std::time_t>(pack.time / 1000);
            const int ms = pack.time % 1000;
            std::tm tm{};

            #if defined(_WIN32)
            localtime_s(&tm, &t);
            #elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
            localtime_r(&t, &tm);
            #else
            tm = *std::localtime(&t);
            #endif

            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
                << '.' << std::setfill('0') << std::setw(3) << ms
                << " [" << *pack.module << "]";

            switch (pack.level) {
                case LogLevel_TRACE: oss << " [TRACE] "; break;
                case LogLevel_DEBUG: oss << " [DEBUG] "; break;
                case LogLevel_INFO:  oss << " [INFO] "; break;
                case LogLevel_WARN:  oss << " [WARN] "; break;
                case LogLevel_ERROR: oss << " [ERROR] "; break;
                case LogLevel_FATAL: oss << " [FATAL] "; break;
                default:             oss << " [INFO] "; break;
            }

            oss << f_msg;

            return oss.str();
        }
    }
}

#endif // ZCLIBLOG_FORMATTERS_STRINGSTREAM_HPP