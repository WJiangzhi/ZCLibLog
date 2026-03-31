//
// Created by wanjiangzhi on 2026/3/30.
//

#ifndef ZCLIBLOG_LOGGER_CLASSES_HPP
#define ZCLIBLOG_LOGGER_CLASSES_HPP

#include <cstdint>
#include <string>
#include <tuple>
#include <utility>
#include <thread>
#include <memory>
#include <functional>

namespace ZCLibLog {
    using LogLevel = uint16_t;
    enum LogLevel_ : LogLevel {
        LogLevel_ALL   = 0,

        LogLevel_TRACE = 1,
        LogLevel_DEBUG = 2,
        LogLevel_INFO  = 3,
        LogLevel_WARN  = 4,
        LogLevel_ERROR = 5,
        LogLevel_FATAL = 6,

        LogLevel_NONE  = std::numeric_limits<LogLevel>::max()
    };
    struct LogPack {
        const std::string* module = {};
        uint64_t time = {};
        LogLevel level = {};
    };
    template<typename... Args>
    class ArgPack {
    public:
        explicit ArgPack(Args... args)
            : data_(std::forward<Args>(args)...) {}

        template<typename Func>
        void apply(Func&& func) const {
            apply_impl(std::forward<Func>(func),
                       std::index_sequence_for<Args...>{});
        }
    private:
        std::tuple<Args...> data_;

        template<typename Func, std::size_t... I>
        void apply_impl(Func&& func, std::index_sequence<I...>) const {
            func(std::get<I>(data_)...);
        }
    };

    template<typename... Args>
    ArgPack<Args...> make_argpack(Args&&... args) {
        return ArgPack<Args...>(std::forward<Args>(args)...);
    }

    using ELogLevel = const LogLevel; // 执行器接收的等级
    using ELString = const std::string&; // 执行器接受的字符串

    using executor = std::function<void(ELString, ELogLevel)>;

    using FRString = std::string; // 格式化输出
    using FLString = const char* const; // 格式化接受的格式
    using FLogPack = const LogPack&; // 格式化接受的数据包

    template<typename... Args>
    using formatter = std::function<FRString(FLogPack, FLString, Args&&...)>;
}

#endif //ZCLIBLOG_LOGGER_CLASSES_HPP
