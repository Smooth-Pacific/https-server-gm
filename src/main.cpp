#include <iostream>
#include <httpserver.hpp>
#include "headers/configs.h"


using namespace httpserver;


class hello_world_resource : public http_resource {
public:
    const std::shared_ptr<http_response> render(const http_request&) {
        std::cout << "SERVER CALL RECEIEVED" << std::endl;
        return std::shared_ptr<http_response>(new string_response("Hello, World!\n"));
    }
};

int main() {
    Config c;

    httpserver::webserver ws = httpserver::create_webserver(c.get_port())
        .use_ssl()
        .https_mem_key(c.get_mem_key_path())
        .https_mem_cert(c.get_mem_cert_path())
        .max_connections(c.get_max_connections())
        .connection_timeout(c.get_timeout())
        .memory_limit(c.get_memory_limit())
        .use_dual_stack() //could check fo ipv4/ipv6 here
        .start_method(httpserver::http::http_utils::INTERNAL_SELECT) //default value
        .max_threads(c.get_max_threads());

    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr);

    ws.start(true);

    return 0;
}


// Call with this command within the container
// curl -XGET -v http://localhost:8001/hello
// curl -XGET -v -k https://localhost:8001/hello