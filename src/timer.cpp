#include "timer.hpp"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

Timer::Timer() {
    restart();
}

void Timer::restart() {
    start = system_clock::now();
}

void Timer::print() {
    auto end = system_clock::now();
    auto dur = end - start;
    cout << "Time:" << duration_cast<seconds>(dur).count();
    cout << "." << duration_cast<milliseconds>(dur).count() % 1000;
    cout << "[s]\n";
}
