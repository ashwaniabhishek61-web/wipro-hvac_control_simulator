#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <vector>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
    #include <process.h>
    #define GET_PID() _getpid()
#else
    #include <unistd.h>
    #define GET_PID() getpid()
#endif

Logger::Logger() : filePath("logs/simulator.log") {
    init(filePath);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.flush();
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

bool Logger::init(const std::string& path) {
    filePath = path;
    
    // Open in append mode so past history is preserved
    logFile.open(filePath.c_str(), std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "[ERROR] Failed to open log file at: " << filePath << std::endl;
        return false;
    }

    // Write session start marker
    logFile << "\n======================================================\n";
    logFile << "[" << currentTimestamp() << "] [PID: " << GET_PID() 
            << "] [INFO] Smart Home Simulator session initialized.\n";
    logFile << "======================================================\n";
    logFile.flush();
    return true;
}

int Logger::getProcessId() const {
    return GET_PID();
}

std::string Logger::currentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* timeInfo = std::localtime(&now_time);

    std::ostringstream oss;
    if (timeInfo) {
        oss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    } else {
        oss << "2026-01-01 00:00:00";
    }
    return oss.str();
}

void Logger::log(const std::string& level, const std::string& message) {
    if (!logFile.is_open()) return;

    std::string timestamp = currentTimestamp();
    int pid = GET_PID();

    logFile << "[" << timestamp << "] [PID: " << pid << "] [" 
            << level << "] " << message << "\n";
    logFile.flush(); // Flush buffer to ensure instant disk write in case of abrupt shutdown
}

void Logger::info(const std::string& message) {
    log("INFO", message);
}

void Logger::warn(const std::string& message) {
    log("WARN", message);
}

void Logger::error(const std::string& message) {
    log("ERROR", message);
}

void Logger::displayRecentLogs(int count) {
    std::ifstream reader(filePath.c_str());
    if (!reader.is_open()) {
        std::cout << "No log file found at: " << filePath << "\n";
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(reader, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    reader.close();

    std::cout << "\n--- Recent Log Events (Last " << count << ") ---\n";
    int start = 0;
    if (static_cast<int>(lines.size()) > count) {
        start = static_cast<int>(lines.size()) - count;
    }

    for (size_t i = start; i < lines.size(); ++i) {
        std::cout << lines[i] << "\n";
    }
    std::cout << "-------------------------------------------\n";
}
