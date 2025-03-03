#ifndef CATHEDRAL_LOGGING_H
#define CATHEDRAL_LOGGING_H

#include <iostream>
#include <fstream>
#include <string>

namespace Cathedral {
    enum class LogLevel { INFO, WARNING, ERROR };

    class Logger {
    public:
        static void Log(const std::string& message, LogLevel level = LogLevel::INFO);
        static void SetLogFile(const std::string& filename);

    private:
        static std::ofstream logFile;
        static std::string LogLevelToString(LogLevel level);
    };
}

#endif // CATHEDRAL_LOGGING_H
