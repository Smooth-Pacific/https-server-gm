#include "../include/configs.hpp"
#include "spdlog/spdlog.h"
#include <fstream>

Config::Config(int argc, char** argv): 
    port(8081), 
    max_connections(6), 
    timeout(100),
    memory_limit(30000),
    max_threads(4),
    iPv4(true),
    iPv6(false),
    log_level('t'),
    mem_cert_path("/ca/server-ca/server-ca.crt"),
    mem_key_path("/ca/server-ca/private/server-ca.key"),
    digest_user("myuser"),
    digest_pass("password")
{ parse_settings(argc, argv); }


// Parses settings from user input
void Config::parse_settings(int argc, char** argv) {
    popl::OptionParser parser("Options");

    auto help = parser.add<popl::Switch>("h", "help", "show program options");
    auto env  = parser.add<popl::Switch>("e", "env", "use environment variables");
    auto port = parser.add<popl::Value<int>>("p", "port", "use designated port number");
    auto conn = parser.add<popl::Value<int>>("c", "connections", "maximum connections");
    auto time = parser.add<popl::Value<int>>("t", "timeout", "inactivity time for connection timeout");
    auto thrd = parser.add<popl::Value<int>>("T", "threads", "maximum server thread count");
    auto ipv4 = parser.add<popl::Switch>("4", "ipv4", "use ipv4 connections");
    auto ipv6 = parser.add<popl::Switch>("6", "ipv6", "use ipv6 connections");
    auto logs = parser.add<popl::Value<std::string>>("l", "logs", "log reporting level");
    auto cert = parser.add<popl::Value<std::string>>("C", "cert", "location of certificate");
    auto key  = parser.add<popl::Value<std::string>>("K", "key", "location of private key");
    auto user = parser.add<popl::Value<std::string>>("U", "user", "user for digest authentication");
    auto pass = parser.add<popl::Value<std::string>>("P", "pass", "password for digest authentication");

    try {
        parser.parse(argc, argv);
    }

    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        print_usage(argv[0], parser);
    }

    catch(...) {
        std::cout << "error with settings" << std::endl << std::endl;
        print_usage(argv[0], parser);
    }

    if(help->is_set()) {
        print_usage(argv[0], parser);
    }

    try {
        if(env->is_set())
            env_load();
    }

    catch(...) {
        std::cout << "error with settings" << std::endl << std::endl;
        print_usage(argv[0], parser);
    }

    if(port->is_set())
        this->port = port->value();

    if(conn->is_set())
        this->max_connections = conn->value();

    if(time->is_set())
        this->timeout = time->value();

    if(thrd->is_set())
        this->max_threads = thrd->value();

    this->iPv4 = ipv4->is_set();
    this->iPv6 = ipv6->is_set();

    if(logs->is_set())
        this->log_level = logs->value()[0];

    if(cert->is_set())
        this->mem_cert_path = cert->value();

    if(key->is_set())
        this->mem_key_path = key->value();

    if(user->is_set())
        this->digest_user = user->value();
    
    if(pass->is_set())
        this->digest_pass = pass->value();


    if(!verify_settings())
        print_usage(argv[0], parser);
}


// Loads settings from environment variables
void Config::env_load() {
    // Load from the environment variables
    if(getenv("PORT"))
        port = static_cast<int>(std::stoul(getenv("PORT")));

    if(getenv("MAX_CONNECTIONS")) 
        max_connections = static_cast<int>(std::stoi(getenv("MAX_CONNECTIONS"))) ;
        
    if(getenv("TIMEOUT"))
        timeout = static_cast<int>(std::stoi(getenv("TIMEOUT"))) ;
    
    if(getenv("MEMORY_LIMIT"))
        memory_limit = static_cast<int>(std::stoi(getenv("MEMORY_LIMIT")));

    if(getenv("MAX_THREADS"))
        max_threads = static_cast<int>(std::stoi(getenv("MAX_THREADS")));
    
    if(getenv("IPV4"))
        iPv4 = static_cast<std::string>(getenv("IPV4")) == "true" ? true : false;
    
    if(getenv("IPV6"))
        iPv6 = static_cast<std::string>(getenv("IPV6")) == "true" ? true : false;

    if(getenv("LOG_LEVEL"))
        log_level = static_cast<std::string>(getenv("LOG_LEVEL"))[0];

    if(getenv("HTTPS_MEM_KEY_PATH"))
        mem_key_path = static_cast<std::string>(getenv("HTTPS_MEM_KEY_PATH"));
    
    if(getenv("HTTPS_MEM_CERT_PATH"))
        mem_cert_path = static_cast<std::string>(getenv("HTTPS_MEM_CERT_PATH"));

    if(getenv("DIGEST_USER"))
        digest_user = static_cast<std::string>(getenv("HTTPS_MEM_CERT_PATH"));

    if(getenv("DIGEST_PASS"))
        digest_pass = static_cast<std::string>(getenv("HTTPS_MEM_CERT_PATH"));
}


// Verifies settings are in valid ranges
bool Config::verify_settings() {
    bool passing = true;

    if(port < 0 || port > 65535) {
        std::cout << "Port number out of range(0 - 65'535)" << std::endl;
        passing = false;
    }

    if(max_connections < 0 || max_connections >= 1000) {
        std::cout << "Max connections out of range(1 - 1'000)" << std::endl;
        passing = false;
    }

    if(timeout < 0 || timeout >= 360) {
        std::cout << "Timeout out of range(1 - 360)" << std::endl;
        passing = false;
    }

    switch(log_level) {
    case 't':
    case 'd':
    case 'i':
    case 'w':
    case 'e':
    case 'c':
    case 'o':
        break;
    default:
        std::cout << "Log level invalid [t,d,i,w,e,c,o]" << std::endl;
        passing = false;
    }

    if(!file_found(mem_key_path)) {
        std::cout << "File not found at " << mem_key_path << std::endl;
        passing = false;
    }

    if(!file_found(mem_cert_path)) {
        std::cout << "File not found at " << mem_cert_path << std::endl;
        passing = false;
    }

    if(digest_user == "") {
        std::cout << "No digest user given" << std::endl;
        passing = false;
    }

    if(digest_pass == "") {
        std::cout << "No digest password given" << std::endl;
        passing = false;
    }

    return passing;
}


// Checks if file exists
bool Config::file_found(std::string path) {
    std::ifstream f(path.c_str());
    return f.good();
}


// Prints out usage from OptionParser object
void Config::print_usage(std::string prog, popl::OptionParser op) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << prog << " [OPTIONS]" << std::endl << std::endl;
    std::cout << op << std::endl;
    std::exit(0);
}


// Logs startup info
void Config::log_configs() {
    spdlog::info("Startup");
    spdlog::info("Config, Port Number: " + std::to_string(port));
    spdlog::info("Config, Timeout: " + std::to_string(timeout));
    spdlog::info("Config, Maximum Connections: " + std::to_string(max_connections));
    spdlog::info("Config, Max Threads: " + std::to_string(max_threads));
}


// Getters
unsigned int Config::get_port() { return port; }
int Config::get_max_connections() { return max_connections; }
int Config::get_timeout() { return timeout; }
int Config::get_memory_limit() { return memory_limit; }
int  Config::get_max_threads() { return max_threads; }
bool Config::get_iPv4() { return iPv4; }
bool Config::get_iPv6() { return iPv6; }
char Config::get_log_level() { return log_level; }
std::string Config::get_mem_cert_path() { return mem_cert_path; }
std::string Config::get_mem_key_path()  { return mem_key_path; }
std::string Config::get_digest_user()   { return digest_user; }
std::string Config::get_digest_pass()   { return digest_pass; }