#ifndef LOGGER_H
#define LOGGER_H

#include <cstdio>
#include <cstdarg>

#ifndef NDEBUG
namespace Logger {

inline void log_info(const char* fmt, ...) {
    std::printf("\033[32m[INFO] ");
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
    std::printf("\033[0m\n");
}

inline void log_warning(const char* fmt, ...) {
    std::printf("\033[33m[WARNING] ");
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
    std::printf("\033[0m\n");
}

inline void log_error(const char* fmt, ...) {
    std::printf("\033[31m[ERROR] ");
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
    std::printf("\033[0m\n");
}

} // namespace Logger
#else
namespace Logger {
    inline void log_info(const char*, ...) { }
    inline void log_warning(const char*, ...) { }
    inline void log_error(const char*, ...) { }
}
#endif  // NDEBUG

#endif  // LOGGER_H
