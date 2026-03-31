#include "ZCLibLog/logger_sync.hpp"
#include "ZCLibLog/logger_async.hpp"

#include "ZCLibLog/formatters/vformat.hpp"
#include "ZCLibLog/executors/cstdio.hpp"
#include "ZCLibLog/executors/iostream.hpp"

#include <windows.h>

ZCLibLog::LoggerSync<ZCLibLog::formatters::vformat> Logger{"MainLogger"};

inline ZCLibLog::executor& popup() {
    using namespace ZCLibLog;
    static executor inst = [](ELString msg, ELogLevel lv) {
        UINT type = MB_OK;
        switch(lv) {
            case LogLevel_WARN:  type = MB_ICONWARNING | MB_OK; break;
            case LogLevel_ERROR: type = MB_ICONERROR   | MB_OK; break;
            case LogLevel_FATAL: type = MB_ICONSTOP    | MB_OK; break;
            default:                                           return;
        }

        MessageBox(NULL, msg.c_str(), "Custom Windows Log Executor", type);
    };
    return inst;
}

#undef ERROR

int main() {
    Logger.bind_executor(ZCLibLog::executors::cstdio());
    Logger.bind_executor(popup());

    Logger.WARN("Hello {}!", ZCLibLog::PROJECT_NAME);



    return 0;
}
