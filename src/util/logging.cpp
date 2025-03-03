#include "util/logging.h"

namespace Cathedral {

    std::ofstream Logger::logFile;

    void Logger::SetLogFile(const std::string& filename) {
        logFile.open(filename, std::ios::out | std::ios::app);
        if (!logFile) {
            std::cerr << "[ERROR] Failed to open log file: " << filename << std::endl;
        }
    }

    std::string Logger::LogLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    void Logger::Log(const std::string& message, LogLevel level) {
        std::string output = "[" + LogLevelToString(level) + "] " + message;

        // Print to console
        std::cout << output << std::endl;

        // Write to log file if open
        if (logFile.is_open()) {
            logFile << output << std::endl;
        }
    }
}
