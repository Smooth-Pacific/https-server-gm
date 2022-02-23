#include <thread>
#include <mutex>
#include <queue>


class Monitoring {
private:
    std::string filename;
    static std::queue<std::string> backlog;

public:
    Monitoring(std::string filename);

    void start();

    static std::mutex m;
    static void add_log(std::string);
};