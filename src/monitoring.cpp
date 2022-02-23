#include "../include/monitoring.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>



std::queue<std::string> Monitoring::backlog;
std::mutex Monitoring::m;

Monitoring::Monitoring(std::string filename): filename(filename) {}

void Monitoring::start() {
    std::cout << "Log Started" << std::endl;

    std::ofstream file(filename);

    while(true) {
        Monitoring::m.lock();
        if(!Monitoring::backlog.empty()) {
            std::cout << Monitoring::backlog.front() << std::endl;
            file << Monitoring::backlog.front() << std::endl;
            //file.flush();
            Monitoring::backlog.pop();
        }
        Monitoring::m.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //sleeps for half second
    }

    file.close();
}

void Monitoring::add_log(std::string message) {
    std::lock_guard<std::mutex> lk(Monitoring::m);

    backlog.push(message);
}