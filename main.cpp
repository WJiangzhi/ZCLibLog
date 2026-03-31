#include <iostream>

#include "ZCLibLog/include/logger.hpp"
#include "ZCLibLog/include/executors/cstdio.hpp"
#include "ZCLibLog/include/formatters/csnprintf.hpp"

int main() {
    ZCLibLog::LoggerSync Logger("MyLogger");
    Logger.bind_executor(ZCLibLog::executors::cstdio);
    Logger.bind_formatter(std::move(ZCLibLog::formatters::csnprintf<>));

    Logger.ERROR("ERROR %s", 123);

    return 0;
}
