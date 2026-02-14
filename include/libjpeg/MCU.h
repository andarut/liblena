#ifndef MCU_H
#define MCU_H

#include "Utils.hpp"
#include "Logger.hpp"
#include "ImageCh.h"

// typedef ImageCh<s16> MCU_ch;
// typedef std::array<ImageCh<s16>, 3> MCU;
// typedef std::vector<MCU> MCUs;

/* TODO: expance MCU if there is not enought data (like 1x1 image) */

/*
Supported modes:
- 8x8
- 16x8
- 16x16
*/
struct MCUMode {
    u64 width, height;
};

template <typename T>
std::vector<ImageCh<T>> splitIntoMCUs(ImageCh<T> ch, MCUMode mode) {

    // g_timers.start("MCUs");

    u64 blocks_count = (ch.width() * ch.height()) / (mode.width * mode.height);
    
    std::vector<ImageCh<T>> _MCUs;
    _MCUs.reserve(blocks_count);

    u64 read_i = 0, read_j = 0;
    for (u64 i = 0; i < blocks_count; i++) {
        
        ImageCh<T> MCU(mode.width, mode.height);

        for (u64 write_i = 0; write_i < mode.height; write_i++) {
            for (u64 write_j = 0; write_j < mode.width; write_j++) {
                MCU(write_i, write_j) = ch(read_i+write_i, read_j+write_j);
            }
        }

        _MCUs.push_back(MCU);

        read_j += mode.width;
        if (read_j >= ch.width()) {
            read_j = 0;
            read_i += mode.height;
        }
    }

    INFO("ch size = %llu x %llu\n", ch.width(), ch.height());
    INFO("MCUs size = %llu x %llu\n", mode.width, mode.height);
    INFO("MCUs count = %lld\n", blocks_count);

    // g_timers.end("MCUs");

    // u64 duration = g_timers.duration("MCUs");

    // INFO("MCUs duration = %llu ms\n", duration);

    INFO("BEFORE MCUs\n");
    print("", ch);

    INFO("AFTER MCUs\n");
    print("", _MCUs[0]);

    return _MCUs;
}

// template <typename T>
// ImageCh<T> MCUs(std::vector<ImageCh<T>> MCUs, MCUMode mode, u64 width, u64 height) {
//     ImageCh<T> decoded_data(width, height);
//     decoded_data.resize(width, height);

//     u64 write_i = 0, write_j = 0;
//     for (auto& MCU : MCUs) {
//         INFO("NEW MCU\n");
//         INFO("write_i = %d\n", write_i);
//         INFO("write_j = %d\n", write_j);
//         for (u64 read_i = 0; read_i < mode.height; read_i++) {
//             for (u64 read_j = 0; read_j < mode.width; read_j++) {
//                 decoded_data(write_i+read_i, write_j+read_j) = MCU(read_i, read_j);
//             }
//         }
//         write_j += mode.width;
//         if (write_j >= decoded_data.width()) {
//             write_j = 0;
//             write_i += mode.height;
//         }
//     }

//     /* TODO: add non-block's pixels techniques */

//     return decoded_data;
// }


#endif // MCU_H