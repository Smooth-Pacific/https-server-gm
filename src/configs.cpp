#include "../include/configs.h"
#include <string>

Config::Config() {
    load();
}


// Loads settings from environment variables or sets to defaults
void Config::load() {
    // Load from the environment variables
    port = getenv("PORT") 
        ? static_cast<int>(std::stoul(getenv("PORT"))) 
        : 8080;

    max_connections = getenv("MAX_CONNECTIONS") 
        ? static_cast<int>(std::stoi(getenv("MAX_CONNECTIONS"))) 
        : 6;
        
    timeout = getenv("TIMEOUT") 
        ? static_cast<int>(std::stoi(getenv("TIMEOUT"))) 
        : 100;
    
    memory_limit = getenv("MEMORY_LIMIT") 
        ? static_cast<int>(std::stoi(getenv("MEMORY_LIMIT"))) 
        : 30000;

    max_threads = getenv("MAX_THREADS") 
        ? static_cast<int>(std::stoi(getenv("MAX_THREADS"))) 
        : 4;
    
    // iPv4 = getenv("IPV4") 
    //     ? ( static_cast<std::string>(getenv("IPV4")) == "true" 
    //             ? true
    //             : false )
    //     : true;
    
    // iPv6 = getenv("IPV6") 
    //     ? ( static_cast<std::string>(getenv("IPV6")) == "true" 
    //             ? true
    //             : false )
    //     : true;
    
    mem_key_path = getenv("HTTPS_MEM_KEY_PATH") 
        ? static_cast<std::string>(getenv("HTTPS_MEM_KEY_PATH")) 
        : "/ca/server-ca/private/server-ca.key";
    
    mem_cert_path = getenv("HTTPS_MEM_CERT_PATH") 
        ? static_cast<std::string>(getenv("HTTPS_MEM_CERT_PATH")) 
        : "/ca/server-ca/server-ca.crt";
}


// Print config settings
std::string Config::toString() {
    return "Port Number: " + std::to_string(port) + "\n"
        + "Timeout: " + std::to_string(timeout) + "\n"
        + "Maximum Connections: " + std::to_string(max_connections) + "\n"
        + "Max Threads: " + std::to_string(max_threads) + "\n";
}


// Getters
unsigned int Config::get_port() { return port; }
int Config::get_max_connections() { return max_connections; }
int Config::get_timeout() { return timeout; }
int Config::get_memory_limit() { return memory_limit; }
int  Config::get_max_threads() {
    if (max_threads > 4) {
        return max_threads;
    }
    
    else {
        return 1; 
    }
}
// bool Config::get_iPv4() { return iPv4; }
// bool Config::get_iPv6() { return iPv6; }
std::string Config::get_mem_key_path() { return mem_key_path; }
std::string Config::get_mem_cert_path() { return mem_cert_path; }