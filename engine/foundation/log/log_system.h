#pragma once

#include <cstdint>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace muggle
{

extern class LogSystem* gLoggerSystem;

class LogSystem {
public:
    enum class LogLevel : uint8_t
    {
        debug,
        info,
        warn,
        error,
        fatal
    };

public:
    LogSystem();
    ~LogSystem();

    template<typename... TARGS>
    static void log(LogLevel level, TARGS&&... args)
    {
        switch (level)
        {
            case LogLevel::debug:
                gLoggerSystem->logger_->debug(std::forward<TARGS>(args)...);
                break;
            case LogLevel::info:
                gLoggerSystem->logger_->info(std::forward<TARGS>(args)...);
                break;
            case LogLevel::warn:
                gLoggerSystem->logger_->warn(std::forward<TARGS>(args)...);
                break;
            case LogLevel::error:
                gLoggerSystem->logger_->error(std::forward<TARGS>(args)...);
                break;
            case LogLevel::fatal:
                gLoggerSystem->logger_->critical(std::forward<TARGS>(args)...);
                fatalCallback(std::forward<TARGS>(args)...);
                break;
            default:
                break;
        }
    }

    template<typename... TARGS>
    static void fatalCallback(TARGS&&... args)
    {
        const std::string format_str = fmt::format(std::forward<TARGS>(args)...);
        throw std::runtime_error(format_str);
    }

private:
    std::shared_ptr<spdlog::logger> logger_;
};

#define LOG_DEBUG(...) muggle::LogSystem::log(muggle::LogSystem::LogLevel::debug, ##__VA_ARGS__);

#define LOG_INFO(...) muggle::LogSystem::log(muggle::LogSystem::LogLevel::info, ##__VA_ARGS__);

#define LOG_WARN(...) muggle::LogSystem::log(muggle::LogSystem::LogLevel::warn, ##__VA_ARGS__);

#define LOG_ERROR(...) muggle::LogSystem::log(muggle::LogSystem::LogLevel::error, ##__VA_ARGS__);

#define LOG_FATAL(...) muggle::LogSystem::log(muggle::LogSystem::LogLevel::fatal, ##__VA_ARGS__);

} // namespace muggle