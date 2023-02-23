

#ifndef logger_hpp
#define logger_hpp

#include <iostream>  // std::cout
#include <thread>    // std::thread
#include <mutex>     // std::mutex, std::lock_guard
#include <stdexcept> // std::logic_error
#include <fstream>

extern std::mutex mtx;
// std::ofstream logFile("/var/log/erss/proxy.log");
extern std::ofstream logFile;

void print_thread_id(std::string log);

#endif /* logger_hpp */
