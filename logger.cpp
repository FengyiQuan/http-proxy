#include "logger.hpp"

std::mutex mtx;
std::ofstream logFile("./log.log");
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
