#ifndef MCUS_H
#define MCUS_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"

/*
Supported modes:
- 8x8
- 16x8
- 16x16
*/
struct MCUMode {
    u64 width, height;
};

namespace enc {

template <typename T>
std::vector<ImageChannel<T>> MCUs(ImageChannel<T> ch, MCUMode mode) {

    g_timers.start("MCUs");

    u64 blocks_count = (ch.width() * ch.height()) / (mode.width * mode.height);
    
    std::vector<ImageChannel<T>> _MCUs;
    _MCUs.reserve(blocks_count);

    u64 read_i = 0, read_j = 0;
    for (u64 i = 0; i < blocks_count; i++) {
        
        ImageChannel<T> MCU(mode.width, mode.height);
        MCU.resize(mode.width, mode.height);

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

    g_timers.end("MCUs");

    u64 duration = g_timers.duration("MCUs");

    INFO("MCUs duration = %llu ms\n", duration);

    return _MCUs;
}

} // namespace enc

namespace dec {

template <typename T>
ImageChannel<T> MCUs(std::vector<ImageChannel<T>> MCUs, MCUMode mode) {
    ImageChannel<T> decoded_data(16, 16);
    decoded_data.resize(16, 16);

    u64 write_i = 0, write_j = 0;
    for (auto& MCU : MCUs) {
        INFO("NEW MCU\n");
        INFO("write_i = %d\n", write_i);
        INFO("write_j = %d\n", write_j);
        for (u64 read_i = 0; read_i < mode.height; read_i++) {
            for (u64 read_j = 0; read_j < mode.width; read_j++) {
                decoded_data(write_i+read_i, write_j+read_j) = MCU(read_i, read_j);
            }
        }
        write_j += mode.width;
        if (write_j >= decoded_data.width()) {
            write_j = 0;
            write_i += mode.height;
        }
    }

    /* TODO: add non-block's pixels techniques */

    return decoded_data;
}

} // namespace dec

#endif // MCUS_H