#include <iostream>
#include <httpserver.hpp>

#include "../include/configs.hpp"
//#include "../include/monitoring.h"
#include "hello_world.cpp"
#include "../include/logger.hpp"

//void start_monitoring(Monitoring &m) {
//    m.start();
//}

void setup_logger(std::string f);

int main() {
    // Load configs for server
    Config c;

    // Set logger settings
    try {
        Logger::setup_logger("logs/file_sink.log");
    }

    catch(const spdlog::spdlog_ex &ex) {
        std::cerr << "Log init failed: " << ex.what() << std::endl;
        exit(1);
    }


    httpserver::webserver ws = httpserver::create_webserver(c.get_port())
        .use_ssl()
        .https_mem_key(c.get_mem_key_path())
        .https_mem_cert(c.get_mem_cert_path())
        .max_connections(c.get_max_connections())
        .connection_timeout(c.get_timeout())
        .memory_limit(c.get_memory_limit())
        .use_dual_stack() //could check fo ipv4/ipv6 here
        .start_method(httpserver::http::http_utils::INTERNAL_SELECT) //default value
        .max_threads(c.get_max_threads())
        .digest_auth();

    spdlog::info("Server started with settings:");
    c.log_configs();

    //Monitoring m("log.txt");
    //std::thread monitor_thread(start_monitoring, std::ref(m));


    // Declare resources
    hello_world_resource hwr;


    // Register resources
    ws.register_resource("/hello", &hwr);

    
    ws.start(true);
    spdlog::info("log working 2");

    //monitor_thread.join();
    return 0;
}