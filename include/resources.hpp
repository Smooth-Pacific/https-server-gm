#ifndef RESOURCES_HEADER
#define RESOURCES_HEADER

#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <httpserver.hpp>
#include "spdlog/spdlog.h"
#include "file_signer.hpp"
#include "logger.hpp"

#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"


class data {
public:
    static std::string user;
    static std::string pass;

    static long long int payload_size(std::string file_name) {
        long long int size = 0;
        std::ifstream file{file_name};
        if (file.is_open()) {
            file.seekg(0, file.end);
            size = file.tellg();
        }

        file.close();
        return size;
    }
};


class hello_world_resource : public httpserver::http_resource {
public:
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
        auto start = std::chrono::steady_clock::now();

        auto res = std::shared_ptr<httpserver::string_response>(new httpserver::string_response("Hello, World!\n", 200, "text/plain"));
        std::string uri = "https://" + req.get_header("Host") + "/hello";

        auto end = std::chrono::steady_clock::now();
        std::string latency = "," + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        spdlog::warn(uri + ",0," + std::to_string(res->get_response_code()) + latency + Logger::gen_log_id());
        return res;
    }
};


class transaction_resource : public httpserver::http_resource {
public:
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
        auto start = std::chrono::steady_clock::now();

        std::string arg = req.get_arg("arg1");
        std::string uri = "https://" + req.get_header("Host") + "/transaction/" + arg;
        std::string path = "../https-data-handler-gm/cache/";
        std::string file_name = "";

        // Check User Digest AUthentication
        if(req.get_digested_user() == "") {
            auto res = std::shared_ptr<httpserver::digest_auth_fail_response>(
                new httpserver::digest_auth_fail_response("No user found\n", data::user, MY_OPAQUE, true)
            );
            // because it reloads, reports error incorrectly here
            //spdlog::error(uri + ",0," + std::to_string(res->get_response_code()));
            return res;
        }

        else {
            bool reload_nonce = false;

            if(!req.check_digest_auth(data::user, data::pass, 300, &reload_nonce)) {
                auto res = std::shared_ptr<httpserver::digest_auth_fail_response>(
                    new httpserver::digest_auth_fail_response("Fail to authenticate\n", data::user, MY_OPAQUE, reload_nonce)
                );

                auto end = std::chrono::steady_clock::now();
                std::string latency = "," + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
                spdlog::error(uri + ",0," + std::to_string(res->get_response_code()) + latency + Logger::gen_log_id());
                return res;
            }
        }


        // Transaction types
        if (arg == "largest")
            file_name = "largest_transactions_10";
        else if (arg == "merchant")
            file_name = "largest_transactions_by_merchant_5";
        else if (arg == "fraud")
            file_name = "percentage_of_fraud_by_year";
        else if (arg == "types")
            file_name = "transaction_types";
        else if (arg == "insufficient")
            file_name = "insufficient_balance";
        else if (arg == "state")
            file_name = "transactions_by_state";
        else if (arg == "zipcode")
            file_name = "most_transactions_by_zip_5";
        else if (arg == "lowest_online")
            file_name = "bottom_online_transaction_months";
        else if (arg == "city_with_online")
            file_name = "top_cities_with_online";
        else if (arg == "most_transactions_by_city")
            file_name = "most_transactions_by_city";
        else if (arg == "deposits")
            file_name = "all_deposits";
        else if (arg == "bottom_online_transaction_months")
            file_name = "bottom_online_transaction_months";
        else if (arg == "top_merchant_insufficient_balance")
            file_name = "top_merchant_insufficient_balance";
        else {
            auto res = std::shared_ptr<httpserver::string_response>(
                new httpserver::string_response("Fail to find resource\n", 404, "text/plain")
            );

            auto end = std::chrono::steady_clock::now();
            std::string latency = "," + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
            spdlog::error(uri + ",0," + std::to_string(res->get_response_code()) + latency + Logger::gen_log_id());
            return res;
        }

        std::string out_file = path + file_name + "-s.xml";
        file_signer::signXML((path + file_name + ".xml").c_str(), out_file.c_str());

        auto res = std::shared_ptr<httpserver::file_response>(
            new httpserver::file_response(out_file, 200, "application/xml")
        );

        auto end = std::chrono::steady_clock::now();
        std::string latency = "," + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        spdlog::warn(uri + "," + std::to_string(data::payload_size(out_file)) + "," + std::to_string(res->get_response_code()) + latency + Logger::gen_log_id());
        return res;
    }
};



class log_resource : public httpserver::http_resource {
public:
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
        auto start = std::chrono::steady_clock::now();

        std::string arg = req.get_arg("arg1");
        std::string uri = "https://" + req.get_header("Host") + "/log/" + arg;
        std::string path = "../https-log-analytics-gm/cache/";
        std::string file_name = "";

        // Check User Digest AUthentication
        if(req.get_digested_user() == "") {
            auto res = std::shared_ptr<httpserver::digest_auth_fail_response>(
                new httpserver::digest_auth_fail_response("No user found\n", data::user, MY_OPAQUE, true)
            );
            // because it reloads, reports error incorrectly here
            //spdlog::error(uri + ",0," + std::to_string(res->get_response_code()));
            return res;
        }

        else {
            bool reload_nonce = false;

            if(!req.check_digest_auth(data::user, data::pass, 300, &reload_nonce)) {
                auto res = std::shared_ptr<httpserver::digest_auth_fail_response>(
                    new httpserver::digest_auth_fail_response("Fail to authenticate\n", data::user, MY_OPAQUE, reload_nonce)
                );

                auto end = std::chrono::steady_clock::now();
                std::string latency = "," + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
                spdlog::error(uri + ",0," + std::to_string(res->get_response_code()) + latency + Logger::gen_log_id());
                return res;
            }
        }


        // Transaction types
        if (arg == "failed_requests")
            file_name = "failed_requests";
        else if (arg == "status_code_distribution")
            file_name = "status_code_distribution";
        else if (arg == "aggregate_payload")
            file_name = "aggregate_payload";
        else if (arg == "average_latency_by_uri")
            file_name = "average_latency_by_uri";
        else if (arg == "success_failure_frequency_by_uri")
            file_name = "success_failure_frequency_by_uri";
        else if (arg == "total_response_payload_by_uri")
            file_name = "total_response_payload_by_uri";
        else if (arg == "error_types")
            file_name = "error_types.xml";
        else if (arg == "success_percentage")
            file_name = "success_percentage.xml";
        else if (arg == "most_frequent_uri")
            file_name = "most_frequent_uri.xml";
        else {
            auto res = std::shared_ptr<httpserver::string_response>(
                new httpserver::string_response("Fail to find resource\n", 404, "text/plain")
            );

            auto end = std::chrono::steady_clock::now();
            std::string latency = "," + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
            spdlog::error(uri + ",0," + std::to_string(res->get_response_code()) + latency + Logger::gen_log_id());
            return res;
        }

        std::string out_file = path + file_name + "-s.xml";
        file_signer::signXML((path + file_name + ".xml").c_str(), out_file.c_str());

        auto res = std::shared_ptr<httpserver::file_response>(
            new httpserver::file_response(out_file, 200, "application/xml")
        );

        auto end = std::chrono::steady_clock::now();
        std::string latency = "," + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        spdlog::warn(uri + "," + std::to_string(data::payload_size(out_file)) + "," + std::to_string(res->get_response_code()) + latency + Logger::gen_log_id());
        return res;
    }
};

#endif