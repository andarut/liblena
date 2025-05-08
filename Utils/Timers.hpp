#ifndef TIMERS_H
#define TIMERS_H

#include "Utils.hpp"

#include "Timer.hpp"

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

#endif // TIMERS_H