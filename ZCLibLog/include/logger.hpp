//
// Created by wanjiangzhi on 2026/3/30.
//

#ifndef ZCLIBLOG_LOGGER_HPP
#define ZCLIBLOG_LOGGER_HPP

#include <vector>
#include <algorithm>
#include <memory>

#include "logger_classes.hpp"

namespace ZCLibLog {
    template<typename... Args>
    class LoggerSync {
    protected:
        using executor_pair = std::pair<size_t, executor>;

        std::string m_name;
        LogLevel m_level;
        size_t m_nextID{};

        formatter<Args&&...> m_formatter;
        std::vector<executor_pair> m_executors;
    public:
        const std::string& name() const noexcept {
            return m_name;
        }
        const LogLevel& level() const noexcept {
            return m_level;
        }

        void bind_formatter(formatter<>&& formatter) {
            m_formatter = formatter;
        }

        size_t bind_executor(executor ex) {
            m_executors.emplace_back(m_nextID, std::move(ex));
            return m_nextID++;
        }

        void debind_executor(size_t id) {
            m_executors.erase(
                std::remove_if(m_executors.begin(), m_executors.end(),
                               [id](const executor_pair& p){ return p.first == id; }),
                m_executors.end());
        }

        void clear_executors() {
            m_executors.clear();
            m_nextID = {};
        }

        explicit LoggerSync(std::string name, LogLevel = LogLevel_ALL) : m_name(std::move(name)), m_level(LogLevel_ALL) {}

        class Tag {
        protected:
            const LoggerSync* const m_logger{};
            const LogLevel m_level{};
        public:
            const LogLevel& level() const noexcept {
                return m_level;
            }

            Tag(const LoggerSync* const logger, const LogLevel level) : m_logger(logger), m_level(level) {}

            void operator()(FLString fmt, Args&&... args) const {
                const auto now = std::chrono::system_clock::now();
                const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now.time_since_epoch()
                ).count();

                LogPack p;
                p.module = &m_logger->name();
                p.level = m_level;
                p.time = ms;

                FRString Formatted;

                if (m_logger->m_formatter) {
                    Formatted = m_logger->m_formatter(p, fmt, args...);
                }

                if (!Formatted.empty()) {
                   for (const auto& the_executor_pair : m_logger->m_executors) {
                       the_executor_pair.second(Formatted, level());
                   }
                }
            }
        };

        Tag ALL{this, LogLevel_ALL};
        Tag TRACE{this, LogLevel_TRACE};
        Tag DEBUG{this, LogLevel_DEBUG};
        Tag INFO{this, LogLevel_INFO};
        Tag WARN{this, LogLevel_WARN};
        Tag ERROR{this, LogLevel_ERROR};
        Tag FATAL{this, LogLevel_FATAL};
        Tag NONE{this, LogLevel_NONE};
    };
}

#define ZCLibLog_Sync template<typename... Args> ZCLibLog::LoggerSync<Args&&...>

#endif //ZCLIBLOG_LOGGER_HPP
