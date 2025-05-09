#ifndef QUANTIZATION_H
#define QUANTIZATION_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"

/* TODO: create quantization matrices for all qualities? */

inline auto Q_50 = ImageChannel<u8>(8, 8, {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
});

ImageChannel<u8> Q_N(u8 quality) {
    assert(quality >= 1 && quality <= 100);
    u64 S = (quality < 50) ? (5000 / quality) : (200 - 2*quality);

    auto Q = ImageChannel<u8>(8, 8);
    Q.resize(8, 8);
    for (u64 i = 0; i < 8; i++)
        for (u64 j = 0; j < 8; j++)
            Q(i, j) = std::clamp((S*Q_50(i, j)+50)/100, (u64)1, (u64)255);

    return Q;
}

ImageChannel<s16> quantization(ImageChannel<f64> ch, u8 quality) {
    assert(ch.width() == 8);
    assert(ch.height() == 8);
    auto Q = Q_N(quality);

    auto quantizated_ch = ImageChannel<s16>(8, 8);
    quantizated_ch.resize(8, 8);

    for (u64 i = 0; i < 8; i++)
        for (u64 j = 0; j < 8; j++) {
            quantizated_ch(i, j) = std::round((f64)(ch(i, j))/Q(i, j));
        }

    return quantizated_ch;
}

std::vector<ImageChannel<s16>> quantization(const std::vector<ImageChannel<f64>>& _MCUs, u8 quality) {
    g_timers.start("quantization MCUs");

    std::vector<ImageChannel<s16>> quantization_MCUs(_MCUs.size());
    for (u64 i = 0; i < _MCUs.size(); i++) {
        quantization_MCUs[i].resize(_MCUs[i].width(), _MCUs[i].height());
        quantization_MCUs[i] = quantization(_MCUs[i], quality);
    }
    g_timers.end("quantization MCUs");

    u64 duration = g_timers.duration("quantization MCUs");

    INFO("quantization MCUs for %llu MCUs duration = %llu ms\n", _MCUs.size(), duration);

    return quantization_MCUs;
}


// RawChannelData<s16> decode_quantization(RawChannelData<s16> data, u8 quality) {
//     auto Q = create_quantization(quality);

//     auto decoded_data = RawChannelData<s16>(data.width, data.height);

//     for (u64 i = 0; i < data.height; i++)
//         for (u64 j = 0; j < data.width; j++) {
//             decoded_data(i, j) = std::round((f64)(data(i, j))*Q(i, j));
//         }

//     return decoded_data;
// }

#endif // QUANTIZATION_H