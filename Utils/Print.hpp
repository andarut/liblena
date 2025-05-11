#ifndef PRINT_H
#define PRINT_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"

#define PRINT(fmt, ...) log<LogLevel::INFO >("", "", "", fmt, ##__VA_ARGS__)

template <typename T>
void print_ch(const ImageChannel<T>& ch) {
    for (u64 i = 0; i < ch.height(); i++) {
        for (u64 j = 0; j < ch.width(); j++) {
            if (std::is_same<T, f64>()) printf("%3f ", (f64)ch(i, j));
            else printf("%3d ", ch(i, j));
        }
        printf("\n");
    }
}

#endif // PRINT_H