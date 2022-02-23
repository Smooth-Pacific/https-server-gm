#include <iostream>
#include <httpserver.hpp>
#include "../include/configs.h"
//#include "../include/monitoring.h"
#include "hello_world.cpp"

void start_monitoring(Monitoring &m) {
    m.start();
}

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
        .max_threads(c.get_max_threads())
        .digest_auth();

    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr);

    std::cout << "\nSERVER START" << std::endl;
    std::cout << c.toString() << std::endl;
    std::cout << "Ongoing Status: " << std::endl;

    Monitoring m("log.txt");

    std::thread monitor_thread(start_monitoring, std::ref(m));
    ws.start(true);

    return 0;
}


// Call with this command within the container
// curl -XGET -v http://localhost:8001/hello
// curl -XGET -v -k https://localhost:8001/hello
// curl -XGET -v --digest --user myuser:password https://localhost:8080/hello
// curl -v --cacert /usr/local/share/ca-certificates/root-ca.crt --digest --user myuser:password https://localhost:8080/hello