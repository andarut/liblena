#ifndef PRINT_H
#define PRINT_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"

#define PRINT(fmt, ...) log<LogLevel::INFO >("", "", "", fmt, ##__VA_ARGS__)

void print_rle(const std::vector<s16> RLE_data) {
    INFO("");
    for (u64 i = 0; i < RLE_data.size(); i++)
        printf("%d ", RLE_data[i]);
    printf("\n");
}

void print_zigzag(const std::vector<s16> zigzag) {
    for (u64 i = 0; i < 8; i++) {
        INFO("");
        for (u64 j = 0; j < 8; j++) {
            printf("%d ", zigzag[i*8+j]);
        }
        printf("\n");
    }
}

template <typename T>
void print_ch(const ImageChannel<T>& ch) {
    for (u64 i = 0; i < ch.height(); i++) {
        INFO("");
        for (u64 j = 0; j < ch.width(); j++) {
            if (std::is_same<T, f64>()) printf("%6.02f ", (f64)ch(i, j));
            else printf("%3d ", ch(i, j));
        }
        printf("\n");
    }
}

#endif // PRINT_H