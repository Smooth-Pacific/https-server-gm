#ifndef CONFIGS_HEADER
#define CONFIGS_HEADER

#include <string>
#include "spdlog/spdlog.h"


class Config {
private:
    unsigned int port;
    int max_connections;
    int timeout;
    int memory_limit;
    int max_threads;
    // bool iPv4;
    // bool iPv6;
    std::string mem_key_path;
    std::string mem_cert_path;

public:
    Config();
    void load();
    void log_configs();

    unsigned int     get_port();
    int     get_max_connections();
    int     get_timeout();
    int     get_memory_limit();
    int     get_max_threads();
    // bool    get_iPv4();
    // bool    get_iPv6();
    std::string get_mem_key_path();
    std::string get_mem_cert_path();
};

#endif