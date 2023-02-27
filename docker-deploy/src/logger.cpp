#include "logger.hpp"

std::mutex mtx;
// std::ofstream logFile("./log.log");
std::ofstream logFile("/var/log/erss/proxy.log");
void LOG(std::string log)
{
    try
    {
        // using a local lock_guard to lock mtx guarantees unlocking on destruction / exception:
        std::lock_guard<std::mutex> lock(mtx);
        logFile << log << std::endl;
        // print_even(id);
    }
    catch (std::logic_error &)
    {
        std::cout << "[exception caught]\n";
    }
}
std::string now()
{
    std::time_t now = std::time(0);
    std::tm *now_tm = std::gmtime(&now);
    char *t = asctime(now_tm);
    std::string t_str(t);
    t_str.erase(std::remove(t_str.begin(), t_str.end(), '\n'), t_str.cend());
    return t_str;
}
