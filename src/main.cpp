#include <iostream>
#include <httpserver.hpp>
#include "../include/configs.hpp"
#include "../include/resources.hpp"
#include "../include/logger.hpp"


std::string data::user = "";
std::string data::pass = "";

//void setup_logger(std::string f);

int main(int argc, char** argv) {
    // Load configs for server
    Config c(argc, argv);

    // Set logger settings
    try {
        Logger::setup_logger("logs/server.log", c.get_log_level());
    }

    catch(const spdlog::spdlog_ex &ex) {
       std::cerr << "Log init failed: " << ex.what() << std::endl;
       exit(1);
    }

    // load user info
    data::user = c.get_digest_user();
    data::pass = c.get_digest_pass();


    auto setup = httpserver::create_webserver(c.get_port())
        .use_ssl()
        .https_mem_key(c.get_mem_key_path())
        .https_mem_cert(c.get_mem_cert_path())
        .max_connections(c.get_max_connections())
        .connection_timeout(c.get_timeout())
        .memory_limit(c.get_memory_limit())
        .start_method(httpserver::http::http_utils::INTERNAL_SELECT) //default value
        .max_threads(c.get_max_threads())
        .digest_auth();
        //file_upload_target()
        //file_upload_dir("/server_files");

    if(c.get_iPv4() && c.get_iPv6()) {
        setup.use_dual_stack();
    }

    else if(!c.get_iPv4() && c.get_iPv6()) {
        setup.use_ipv6();
    }


    httpserver::webserver ws = setup;
    c.log_configs();

    // Declare resources
    hello_world_resource    hwr;
    transaction_resource    tr;
    log_resource            lg;

    // Register resources
    ws.register_resource("/hello", &hwr);
    ws.register_resource("/transaction/{arg1}", &tr);
    ws.register_resource("/log/{arg1}", &lg);

    
    ws.start(true);

    spdlog::shutdown();
    //monitor_thread.join();
    return 0;
}