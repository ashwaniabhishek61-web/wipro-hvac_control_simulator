#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger {
public:
    // Returns the singleton instance of the Logger
    static Logger& getInstance();

    // Initialize the logger with the file path
    bool init(const std::string& logFilePath = "logs/simulator.log");

    // Write a log entry with timestamp and log level
    void log(const std::string& level, const std::string& message);

    // Convenience logging methods
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);

    // Display the last N log entries in the console
    void displayRecentLogs(int count = 10);

    // Get current process ID
    int getProcessId() const;

    // Destructor ensures file buffer is flushed and closed cleanly
    ~Logger();

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string currentTimestamp() const;

    std::string filePath;
    std::ofstream logFile;
};

#endif // LOGGER_H
