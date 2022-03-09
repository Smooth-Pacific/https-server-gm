#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>


size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp){
   return size * nmemb;
}

int curl_server(){
    CURLcode ret;
    CURL *hnd;

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(hnd, CURLOPT_URL, "https://localhost:8081/hello/");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERPWD, "myuser:password");
    curl_easy_setopt(hnd, CURLOPT_HTTPAUTH, (long)CURLAUTH_DIGEST);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.71.1");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CAINFO, "/usr/local/share/ca-certificates/root-ca.crt");
    curl_easy_setopt(hnd, CURLOPT_SSH_KNOWNHOSTS, "/home/dev/.ssh/known_hosts");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_data);       // write output to nothing


    ret = curl_easy_perform(hnd);
    long response_code;
    curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(hnd);
    hnd = NULL;

    return (int)response_code;
}

void proccess_chunk(std::atomic<int>& passed, const unsigned long chunk_size, const int delay) {
    for(int i = 0; i < chunk_size; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(curl_server() == 200)
            passed++;
    }
}


int main() {
    int num_runs = 10;
    int total_requests = 0;
    float total_passed = 0.;
    float total_time = 0.;

    for(int run = 0; run <= num_runs; ++run) {
        int delay = (10 - run);

        const unsigned long num_threads = 6;
        const unsigned long requests = 600;
        const unsigned long chunk_size = requests / num_threads;
        std::atomic<int> passed = 0;

        auto start = std::chrono::steady_clock::now();

        std::vector<std::thread> threads(num_threads);
        for(int i = 0; i < num_threads; i++) {
            threads.at(i) = std::thread(proccess_chunk, std::ref(passed), chunk_size, delay);
        }

        for(auto& t : threads)
            t.join();
        
        auto end = std::chrono::steady_clock::now();
        float time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

        total_passed += passed;
        total_requests += requests;
        total_time += time_elapsed;

        // std::cout << "Current delay: " << std::to_string(delay) << " ms" << std::endl;
        // std::cout << "Total requests made: " << requests << std::endl;
        // std::cout << "Average percentage of passed requests: " << ((float)passed / (float)requests) * 100 << "%"<< std::endl;
        // std::cout << "Rate of requests: " << ((float)requests/time_elapsed) * 1000 << " r/s" << std::endl;
        // std::cout << std::endl << std::endl;
    }


    std::cout << "=========================================" << std::endl;
    std::cout << "Total requests made: " << total_requests << std::endl;
    std::cout << "Average percentage of passed requests: " << ((float)total_passed / (float)total_requests) * 100 << "%"<< std::endl;
    std::cout << "Rate of requests: " << ((float)total_requests/total_time) * 1000 << " r/s" << std::endl;
}