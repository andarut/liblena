#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <chrono>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <thread>

#include "logging.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

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

/* TODO: use function name as prefix to timer name ?? */
/* TODO: use format timer name ?? */
struct Timers {

    std::map<const char *, Timer> timers;
    
    Timers() = default;

    void start(const char *timer_name) {
        auto timer = Timer();
        timer.start();
        timers.insert({timer_name, timer});
    }

    void end(const char *timer_name) {
        timers[timer_name].end();
    }

    u64 duration(const char *timer_name) {
        return timers[timer_name].duration;
    }
};

inline Timers g_timers;

#endif // UTILS_H