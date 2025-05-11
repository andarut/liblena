#ifndef DOWNSAMPLING_H
#define DOWNSAMPLING_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"

/*
Supported mods are:
- 4:4:4
- 4:2:2
- 4:1:1
- 4:4:0
- 4:2:0
- 4:1:0
- 4:4:1
- 4:2:1
*/
struct DownsamplingMode {
    u64 J, a, b;
};

namespace enc {

template <typename T>
ImageChannel<T> downsampling(const ImageChannel<T> &ch, const DownsamplingMode &mode) {

    g_timers.start("downsampling");

    u64 block_w = mode.J;
    u64 block_h = 2;

    INFO("downsampling mode = %lld:%lld:%lld\n", mode.J, mode.a, mode.b);

    ImageChannel<T> downsampled_ch(ch.width(), ch.height());

    u64 write_i = 0;
    for (u64 read_i = 0; read_i < ch.height(); read_i++) {
        u64 write_j = 0;
        u64 samples_count = (read_i % 2 == 0) ? mode.a : mode.b;
        if (read_i % 2 != 0 && mode.b == 0) continue;
        for (u64 read_j = 0; read_j < ch.width(); read_j++) {
            if ((read_j % mode.J) % samples_count != 0 && samples_count != mode.J) continue;
            downsampled_ch.push_back(ch(read_i, read_j));
            write_j++;
            if (samples_count == 1) read_j+=block_w-1;
        }
        if (read_i == 0) downsampled_ch.set_width(downsampled_ch.size());
        write_i++;
    }
    downsampled_ch.set_height(write_i);

    g_timers.end("downsampling");

    INFO("downsampled from %llu x %llu -> %llu x %llu\n", \
        ch.width(), ch.height(), downsampled_ch.width(), downsampled_ch.height());

    INFO("downsampling duration = %llu ms\n", g_timers.duration("encoding"));

    return downsampled_ch;
}

} // namespace enc

namespace dec {

/* TODO: think about passing down original sizes */
template <typename T>
ImageChannel<T> downsampling(const ImageChannel<T> &ch, const DownsamplingMode &mode) {
    INFO("mode = %lld:%lld:%lld\n", mode.J, mode.a, mode.b);

    ImageChannel<T> decoded_data(ch.width(), ch.height());
    decoded_data.resize(ch.width(), ch.height());


    u64 read_i = 0, read_j = 0;

    u64 read_index = 0;

    for (u64 write_i = 0; write_i < decoded_data.height(); write_i++) {
        u64 a_or_b = (write_i % 2 == 0) ? mode.a : mode.b;
        if (a_or_b == 0) {
            // printf("COPY %lld -> %lld\n", write_i-1, write_i);
            for (u64 write_j = 0; write_j < decoded_data.width(); write_j++) {
                decoded_data(write_i, write_j) = decoded_data(write_i-1, write_j);
            }
            continue;
        }
        for (u64 write_j = 0; write_j < decoded_data.width(); write_j++) {
            if (read_index >= ch.size()) {
                read_index = ch.size() - 1;
            }
            T element = ch(0, read_index);

            for (u64 copy_j = 0; copy_j < mode.J/a_or_b; copy_j++) {
                // printf("write to (%lld, %lld) = %lld\n", write_i, write_j, element);
                if (write_j == decoded_data.width()) break;
                decoded_data(write_i, write_j) = element;
                write_j++;
            }
            write_j--;

            read_index++;   
        }
    }

    return decoded_data;
}

} // namespace dec

#endif // DOWNSAMPLING_H