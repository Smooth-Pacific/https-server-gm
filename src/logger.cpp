#include "../include/logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"


// Used to get log ids
//Logger::counter = 0;

std::string Logger::gen_log_id() {
    std::lock_guard<std::mutex> lock(m);
    return "," + std::to_string(counter++);
}


void Logger::setup_logger(std::string file_path, char log_level) {
    enum spdlog::level::level_enum level;
    switch(log_level) {
        case 't':
            level = spdlog::level::trace;
            break;
        case 'd':
            level = spdlog::level::debug;
            break;
        case 'i':
            level = spdlog::level::info;
            break;
        case 'w':
            level = spdlog::level::warn;
            break;
        case 'e':
            level = spdlog::level::err;
            break;
        case 'c':
            level = spdlog::level::critical;
            break;
        case 'o':
            level = spdlog::level::off;
            break;
        default:
            level = spdlog::level::trace;
            break;
    }

    // Console sink settings
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(level);
    console_sink->set_pattern("[%^--%L--%$] [thread %t] %v");

    // File sink settings
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_path, true);
    file_sink->set_level(level);
    // date, time, log level, message (URI, response size, status code)
    file_sink->set_pattern("%D,%H:%M:%S,%l,%v");

    // Establishes both sinks as logger
    auto  logger =  std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({console_sink, file_sink}));
    logger->set_level(level);

    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::trace);
}