

#ifndef logger_hpp
#define logger_hpp

#include <iostream>  // std::cout
#include <thread>    // std::thread
#include <mutex>     // std::mutex, std::lock_guard
#include <stdexcept> // std::logic_error
#include <fstream>

std::mutex mtx;
// std::ofstream logFile("/var/log/erss/proxy.log");
std::ofstream logFile("./log.log");

void print_thread_id(std::string log);


#endif /* logger_hpp */
