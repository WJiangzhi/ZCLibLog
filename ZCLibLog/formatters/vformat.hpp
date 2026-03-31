//
// Created by TingIAAI on 2026/4/4.
//

#ifndef ZCLIBLOG_FORMATTERS_VFORMAT_HPP
#define ZCLIBLOG_FORMATTERS_VFORMAT_HPP

#include <format>
#include <chrono>
#include "../inside/logger_types.hpp"

// NOLINTNEXTLINE
namespace ZCLibLog::formatters {
    class vformat {
    public:
        template <typename Fmt, typename... Args>
        static std::string do_format(FLogPack pack, Fmt fmt, Args&&... args) {
            std::string f_msg;
            try {
                f_msg = std::vformat(fmt, std::make_format_args(std::forward<Args&&>(args)...));
            } catch (const std::format_error&) {
                return {};
            }

            const auto tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(pack.time));

            const char* level_str;
            switch (pack.level) {
                case LogLevel_TRACE: level_str = "[TRACE]";
                    break;
                case LogLevel_DEBUG: level_str = "[DEBUG]";
                    break;
                case LogLevel_INFO: level_str = "[INFO]";
                    break;
                case LogLevel_WARN: level_str = "[WARN]";
                    break;
                case LogLevel_ERROR: level_str = "[ERROR]";
                    break;
                case LogLevel_FATAL: level_str = "[FATAL]";
                    break;
                default: level_str = "[INFO]";
                    break;
            }

            return std::format(
                "{:%Y-%m-%d %H:%M:%S} [{}] {} {}",
                std::chrono::time_point_cast<std::chrono::milliseconds>(tp),
                *pack.module,
                level_str,
                f_msg
            );
        }
    };
}


#endif // ZCLIBLOG_FORMATTERS_VFORMAT_HPP
