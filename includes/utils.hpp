#pragma once

#include <cstdint>
#include <fstream>
#include <mutex>
#include <memory>

enum class Level : std::uint8_t {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class QuarkLogger {
private:
    static std::unique_ptr<QuarkLogger> m_instance;
    static std::mutex m_mutex;
    std::ofstream log_file;

    static auto get_time() -> std::string;
    static auto level_to_string(Level level) -> std::string;
    void log(Level level, const std::string &msg);

public:
    QuarkLogger();
    ~QuarkLogger();

    QuarkLogger(QuarkLogger &other) = delete;
    void operator=(const QuarkLogger&) = delete;
    QuarkLogger(QuarkLogger&& other) = delete;
    auto operator=(QuarkLogger&& other) -> QuarkLogger& = delete;

    static auto get_instance() -> QuarkLogger*;
    void info(const std::string &msg);
    void debug(const std::string &msg);
    void warn(const std::string &msg);
    void error(const std::string &msg);
};
