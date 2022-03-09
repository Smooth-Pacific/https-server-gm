#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include "spdlog/spdlog.h"

namespace Logger {
    void setup_logger(std::string file_path);    // inputs settings for spdlog library
}

#endif