//
// Created by wanjiangzhi on 2026/4/4.
//

#ifndef ZCLIBLOG_ANDROID_LOG_HPP
#define ZCLIBLOG_ANDROID_LOG_HPP

//
// Created by TingIAAI on 2026/4/4.
//

#ifndef ZCLIBLOG_FORMATTERS_ANDROID_LOG_HPP
#define ZCLIBLOG_FORMATTERS_ANDROID_LOG_HPP

#include <ctime>
#include <cstdio>
#include <array>
#include "../inside/logger_types.hpp"

// NOLINTNEXTLINE
namespace ZCLibLog {
    namespace formatters {
        class android_log {
        public:
            template<typename... Args>
            static std::string do_format(FLogPack pack, const char* fmt, Args&&... args) {
                std::string f_msg;
                if (sizeof...(args) == 0) {
                    f_msg = fmt;
                }
                else {
                    thread_local std::array<char, 4096> buffer;
                    int len = std::snprintf(buffer.data(), buffer.size(), fmt, std::forward<Args>(args)...);
                    if (len < 0) return {};
                    if (len >= static_cast<int>(buffer.size())) len = buffer.size() - 1;
                    f_msg = std::string(buffer.data(), len);
                }

                auto t = static_cast<std::time_t>(pack.time / 1000);
                const auto ms = static_cast<int>(pack.time % 1000);
                std::tm tm{};

                localtime_r(&t, &tm);

                thread_local std::array<char, 64> time_buf;
                std::strftime(time_buf.data(), time_buf.size(), "%Y-%m-%d %H:%M:%S", &tm);

                thread_local std::array<char, 80> ms_time;
                std::snprintf(ms_time.data(), ms_time.size(), "%s.%03d", time_buf.data(), ms);

                len = std::snprintf(buffer.data(), buffer.size(),
                                    "%s [%s] %s\n",
                                    ms_time.data(), pack.module->c_str(), f_msg.c_str());
                if (len < 0) return {};
                if (len >= static_cast<int>(buffer.size())) len = buffer.size() - 1;

                return {buffer.data(), buffer.data() + len};
            }
        };
    }
}

#endif // ZCLIBLOG_FORMATTERS_ANDROID_LOG_HPP

#endif //ZCLIBLOG_ANDROID_LOG_HPP
