#ifndef __TIME_HPP__
#define __TIME_HPP__

#include <chrono>

class Timer {
public:
    std::chrono::system_clock::time_point start;
    Timer();
    void restart();
    void print();
};

#endif
