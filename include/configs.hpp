#ifndef CONFIGS_HEADER
#define CONFIGS_HEADER

#include <string>
#include "../include/popl.hpp"


class Config {
private:
    unsigned int port;
    int max_connections;
    int timeout;
    int memory_limit;
    int max_threads;
    bool iPv4;
    bool iPv6;
    char log_level;
    std::string mem_cert_path;
    std::string mem_key_path;
    std::string digest_user;
    std::string digest_pass;

    void parse_settings(int argc, char** argv);
    void env_load();
    bool verify_settings();
    bool file_found(std::string path);
    void print_usage(std::string prog, popl::OptionParser op);


public:
    Config(int argc, char** argv);
    void log_configs();

    unsigned int get_port();
    int     get_max_connections();
    int     get_timeout();
    int     get_memory_limit();
    int     get_max_threads();
    bool    get_iPv4();
    bool    get_iPv6();
    char    get_log_level();
    std::string get_mem_cert_path();
    std::string get_mem_key_path();
    std::string get_digest_user();
    std::string get_digest_pass();
};

#endif