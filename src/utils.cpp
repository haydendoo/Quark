#include "utils.hpp"

#include <iostream>
#include <ios>
#include <chrono>
#include <ctime>
#include <memory>
#include <mutex>
#include <string>

std::unique_ptr<QuarkLogger> QuarkLogger::m_instance = nullptr;
std::mutex QuarkLogger::m_mutex;

QuarkLogger::QuarkLogger() {
    log_file.open("quark.log", std::ios::app);
    if (!log_file.is_open()) {
        throw std::ios_base::failure("Failed to open log file");
    }
}

QuarkLogger::~QuarkLogger() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

auto QuarkLogger::get_time() -> std::string {
    auto now = std::chrono::system_clock::now();
    const std::time_t local_time = std::chrono::system_clock::to_time_t(now);

    std::string result = std::ctime(&local_time);
    result.pop_back();
    return result;
}

auto QuarkLogger::level_to_string(Level level) -> std::string {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void QuarkLogger::log(Level level, const std::string& msg) {
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (log_file.is_open()) {
        log_file << "[" << get_time() << "] - [" << level_to_string(level) << "] " << msg << '\n';
    }
    std::cout << "[" << get_time() << "] - [" << level_to_string(level) << "] " << msg << '\n';
}

void QuarkLogger::info(const std::string& msg) {
    log(Level::INFO, msg);
}

void QuarkLogger::debug(const std::string& msg) {
    log(Level::DEBUG, msg);
}

void QuarkLogger::warn(const std::string& msg) {
    log(Level::WARNING, msg);
}

void QuarkLogger::error(const std::string& msg) {
    log(Level::ERROR, msg);
}

auto QuarkLogger::get_instance() -> QuarkLogger* {
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_instance == nullptr) {
        m_instance = std::make_unique<QuarkLogger>();
    }
    return m_instance.get();
}
