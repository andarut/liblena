#ifndef TIMER_H
#define TIMER_H

#include "Utils.hpp"

#include <chrono>
#include <cstdint>
using u64 = std::uint64_t;

struct Timer {
    u64 duration = 0;
    std::chrono::high_resolution_clock::time_point time_point;

    void start() {
        time_point = std::chrono::high_resolution_clock::now();
    }

    void end() {
        auto now = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                       now - time_point
                   ).count();
    }
};


#endif // TIMER_H