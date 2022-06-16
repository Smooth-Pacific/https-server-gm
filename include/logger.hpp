#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include "spdlog/spdlog.h"
#include <mutex>

namespace Logger {
    static unsigned long counter = 0;
    static std::mutex m;
    std::string gen_log_id();

    void setup_logger(std::string file_path, char log_level);    // inputs settings for spdlog library
}

#endif