#include <iostream>
#include <httpserver.hpp>

using namespace httpserver;


class hello_world_resource : public http_resource {
public:
    const std::shared_ptr<http_response> render(const http_request&) {
        std::cout << "SERVER CALL RECEIEVED" << std::endl;
        return std::shared_ptr<http_response>(new string_response("Hello, World!\n"));
    }
};

int main() {
    httpserver::webserver ws = httpserver::create_webserver(8001)
        .use_ssl()
        .https_mem_key("/ca/server-ca/private/server-ca.key")
        .https_mem_cert("/ca/server-ca/server-ca.crt");

    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr);

    ws.start(true);

    return 0;
}


// Call with this command within the container
// curl -XGET -v http://localhost:8001/hello
// curl -XGET -v -k https://localhost:8001/hello