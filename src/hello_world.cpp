#include <iostream>
#include <thread>
#include <httpserver.hpp>
#include "../include/monitoring.h"


#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"


class hello_world_resource : public httpserver::http_resource {
public:
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
        // load digest info
        std::string user = getenv("DIGEST_USER") 
            ? static_cast<std::string>(getenv("DIGEST_USER")) 
            : std::string("");

        std::string pass = getenv("DIGEST_PASS") 
            ? static_cast<std::string>(getenv("DIGEST_PASS")) 
            : std::string("");

        if(req.get_digested_user() == "") {
            return std::shared_ptr<httpserver::digest_auth_fail_response>(
                new httpserver::digest_auth_fail_response("FAIL: No User Found\n", user, MY_OPAQUE, true)
            );
        }

        else {
            bool reload_nonce = false;

            if(!req.check_digest_auth(user, pass, 300, &reload_nonce)) {
                Monitoring::add_log("LOG: FAIL - Digest Authentication Fail");
                return std::shared_ptr<httpserver::digest_auth_fail_response>(
                    new httpserver::digest_auth_fail_response("FAIL: Fail to authenticate\n", user, MY_OPAQUE, reload_nonce)
                );
            }
        }

        Monitoring::add_log("LOG: SUCCESS - Hello World Call");
        return std::shared_ptr<httpserver::string_response>(
            new httpserver::string_response("SUCCESS: Hello, World!\n", 200, "text/plain")
        );
    }
};