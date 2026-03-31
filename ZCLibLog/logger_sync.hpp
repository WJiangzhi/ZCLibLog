//
// Created by wanjiangzhi on 2026/3/30.
//

#ifndef ZCLIBLOG_LOGGER_SYNC_HPP
#define ZCLIBLOG_LOGGER_SYNC_HPP

#include <algorithm>
#include <vector>
#include <string>

#include "inside/logger_macros.h"
#include "inside/logger_types.hpp"

// ReSharper disable CppUnusedIncludeDirective
#include "inside/logger_constants.hpp"
// ReSharper enable CppUnusedIncludeDirective

#include "logger_configurations.h"
#if ZCLIBLOG_LOGGER_CONFIGURATIONS_DEFAULT_CSNPRINTF
#include "formatters/csnprintf.hpp"
#endif

namespace ZCLibLog {
    template <
        typename Formatter
        #if ZCLIBLOG_LOGGER_CONFIGURATIONS_DEFAULT_CSNPRINTF
        = formatters::csnprintf
        #endif
    >
    class LoggerSync {
    protected:
        using executor_pair = std::pair<size_t, executor>;

        std::string m_name;
        LogLevel m_level;
        size_t m_nextID{};

        std::vector<executor_pair> m_executors;

    public:
        ZCLibLog_NODISCARD const std::string& name() const noexcept {
            return m_name;
        }

        ZCLibLog_NODISCARD LogLevel& level() noexcept {
            return m_level;
        }

        size_t bind_executor(executor ex) {
            m_executors.emplace_back(m_nextID, std::move(ex));
            return m_nextID++;
        }

        void debind_executor(size_t id) {
            m_executors.erase(
                std::remove_if(
                    m_executors.begin(),
                    m_executors.end(),
                    [id](const executor_pair& p) { return p.first == id; }
                ),
                m_executors.end()
            );
        }

        void clear_executors() {
            m_executors.clear();
            m_nextID = {};
        }

        // ReSharper disable once CppNonExplicitConvertingConstructor
        LoggerSync(std::string name, const LogLevel level = LogLevel_ALL) : m_name(std::move(name)),
                                                                            m_level(level) {}

        class Tag {
        protected:
            const LoggerSync* const m_logger{};
            const LogLevel m_level{};

        public:
            ZCLibLog_NODISCARD const LogLevel& level() const noexcept {
                return m_level;
            }

            Tag(const LoggerSync* const logger, const LogLevel level) : m_logger(logger),
                                                                        m_level(level) {}

            template <typename Fmt, typename... Args>
            void operator()(Fmt fmt, Args&&... args) const {
                if (m_logger->m_executors.empty()) return;

                const auto now = std::chrono::system_clock::now();
                const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now.time_since_epoch()
                ).count();

                LogPack p;
                p.module = &m_logger->name();
                p.level = m_level;
                p.time = ms;

                std::string Formatted = Formatter::do_format(p, std::forward<Fmt>(fmt), std::forward<Args>(args)...);

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

#endif //ZCLIBLOG_LOGGER_SYNC_HPP
