#include <iostream>
#include <thread>
#include <chrono>
#include <httpserver.hpp>
#include "spdlog/spdlog.h"
//#include "../include/monitoring.h"


#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"


class hello_world_resource : public httpserver::http_resource {
public:
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
        auto start = std::chrono::steady_clock::now();

        // load digest info
        std::string user = getenv("DIGEST_USER") 
            ? static_cast<std::string>(getenv("DIGEST_USER")) 
            : std::string("myuser");

        std::string pass = getenv("DIGEST_PASS") 
            ? static_cast<std::string>(getenv("DIGEST_PASS")) 
            : std::string("password");

        if(req.get_digested_user() == "") {
            return std::shared_ptr<httpserver::digest_auth_fail_response>(
                new httpserver::digest_auth_fail_response("FAIL: No User Found\n", user, MY_OPAQUE, true)
            );
        }

        else {
            bool reload_nonce = false;

            if(!req.check_digest_auth(user, pass, 300, &reload_nonce)) {
                spdlog::warn("Hello resource - FAIL to authenticate");
                return std::shared_ptr<httpserver::digest_auth_fail_response>(
                    new httpserver::digest_auth_fail_response("FAIL: Fail to authenticate\n", user, MY_OPAQUE, reload_nonce)
                );
            }
        }


        spdlog::info("Hello resource - SUCCESS");
        auto end = std::chrono::steady_clock::now();
        std::string latency = std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        spdlog::info("Hello resource - Latency: " + latency + " µs");
        return std::shared_ptr<httpserver::string_response>(
            new httpserver::string_response("SUCCESS: Hello, World!\n", 200, "text/plain")
        );
    }
};

class slow_resource : public httpserver::http_resource {
public:
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
        auto start = std::chrono::steady_clock::now();

        // load digest info
        std::string user = getenv("DIGEST_USER") 
            ? static_cast<std::string>(getenv("DIGEST_USER")) 
            : std::string("myuser");

        std::string pass = getenv("DIGEST_PASS") 
            ? static_cast<std::string>(getenv("DIGEST_PASS")) 
            : std::string("password");

        // SLEEPS THREAD FOR 1 SECOND FOR TESTING PURPOSES
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));



        if(req.get_digested_user() == "") {
            return std::shared_ptr<httpserver::digest_auth_fail_response>(
                new httpserver::digest_auth_fail_response("FAIL: No User Found\n", user, MY_OPAQUE, true)
            );
        }

        else {
            bool reload_nonce = false;

            if(!req.check_digest_auth(user, pass, 300, &reload_nonce)) {
                spdlog::warn("Slow resource - FAIL to authenticate");
                return std::shared_ptr<httpserver::digest_auth_fail_response>(
                    new httpserver::digest_auth_fail_response("FAIL: Fail to authenticate\n", user, MY_OPAQUE, reload_nonce)
                );
            }
        }


        spdlog::info("Slow resource - SUCCESS");

        auto end = std::chrono::steady_clock::now();
        std::string latency = std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        spdlog::info("Slow resource - Latency: " + latency + " µs");
        return std::shared_ptr<httpserver::string_response>(
            new httpserver::string_response("SUCCESS: Hello, World!\n", 200, "text/plain")
        );
    }
};