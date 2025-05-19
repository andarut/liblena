#ifndef TIMER_H
#define TIMER_H

#include "Utils.hpp"

struct Timer {
    u64 duration;
    std::chrono::steady_clock::time_point time_point;

    Timer() = default;

    void start() {
        time_point = std::chrono::high_resolution_clock::now();
    }

    void end() {
        auto now = std::chrono::high_resolution_clock::now();
        duration =  std::chrono::duration_cast<std::chrono::milliseconds>(now - time_point).count();
    }

};

#endif // TIMER_H