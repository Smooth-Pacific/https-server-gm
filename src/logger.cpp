#include "../include/logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"


void Logger::setup_logger(std::string file_path) {
    // Console sink settings
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("[%^--%L--%$] [thread %t] %v");

    // File sink settings
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_path);
    file_sink->set_level(spdlog::level::trace);
    file_sink->set_pattern("[%H:%M:%S %z] [%^--%L--%$] [thread %t] %v");

    // Establishes both sinks as logger
    auto  logger =  std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({console_sink, file_sink}));
    logger->set_level(spdlog::level::trace);

    spdlog::set_default_logger(std::move(logger));
}