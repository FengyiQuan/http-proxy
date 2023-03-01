

#ifndef logger_hpp
#define logger_hpp

#include <iostream>  // std::cout
#include <thread>    // std::thread
#include <mutex>     // std::mutex, std::lock_guard
#include <stdexcept> // std::logic_error
#include <fstream>
#include <algorithm>

extern std::mutex mtx;
// std::ofstream logFile("/var/log/erss/proxy.log");
extern std::ofstream logFile;

void LOG(std::string log);
// void LOG_Request(int id, std::string request, std::string server);
// void LOG_Requesting(int id, std::string request, std::string server);
// void LOG_Received(int id, std::string response, std::string server);
std::string now();

#endif /* logger_hpp */
