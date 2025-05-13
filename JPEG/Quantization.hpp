#ifndef QUANTIZATION_H
#define QUANTIZATION_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"
#include "Print.hpp"

/* TODO: create quantization matrices for all qualities? */

/* Table K.1 */
inline auto LUMINANCE_Q_50 = ImageChannel<u8>(8, 8, {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
});

/* Table K.2 */
inline auto CHROMINANCE_Q_50 = ImageChannel<u8>(8, 8, {
    17,	18,	24,	47,	99,	99,	99,	99,
    18,	21,	26,	66,	99,	99,	99,	99,
    24,	26,	56,	99,	99,	99,	99,	99,
    47,	66,	99,	99,	99,	99,	99,	99,
    99,	99,	99,	99,	99,	99,	99,	99,
    99,	99,	99,	99,	99,	99,	99,	99,
    99,	99,	99,	99,	99,	99,	99,	99,
    99,	99,	99,	99,	99,	99,	99,	99,
});

inline ImageChannel<u8> LUMINANCE_Q_N(u8 quality) {
    assert(quality >= 1 && quality <= 100);
    u64 S = (quality < 50) ? (5000 / quality) : (200 - 2*quality);

    auto Q = ImageChannel<u8>(8, 8);
    Q.resize(8, 8);
    for (u64 i = 0; i < 8; i++)
        for (u64 j = 0; j < 8; j++)
            Q(i, j) = std::clamp((S*LUMINANCE_Q_50(i, j)+50)/100, (u64)1, (u64)255);

    return Q;
}

inline ImageChannel<u8> CHROMINANCE_Q_N(u8 quality) {
    assert(quality >= 1 && quality <= 100);
    u64 S = (quality < 50) ? (5000 / quality) : (200 - 2*quality);

    auto Q = ImageChannel<u8>(8, 8);
    Q.resize(8, 8);
    for (u64 i = 0; i < 8; i++)
        for (u64 j = 0; j < 8; j++)
            Q(i, j) = std::clamp((S*CHROMINANCE_Q_50(i, j)+50)/100, (u64)1, (u64)255);

    return Q;
}

namespace enc {

inline ImageChannel<s16> luminance_quantization(ImageChannel<f64> ch, u8 quality) {
    assert(ch.width() == 8);
    assert(ch.height() == 8);
    auto Q = LUMINANCE_Q_N(quality);
    // printf("quantization table = ");
    // print_ch(Q);

    auto quantizated_ch = ImageChannel<s16>(8, 8);
    quantizated_ch.resize(8, 8);

    for (u64 i = 0; i < 8; i++)
        for (u64 j = 0; j < 8; j++) {
            quantizated_ch(i, j) = std::round((f64)(ch(i, j))/Q(i, j));
        }

    return quantizated_ch;
}

inline ImageChannel<s16> chrominance_quantization(ImageChannel<f64> ch, u8 quality) {
    assert(ch.width() == 8);
    assert(ch.height() == 8);
    auto Q = CHROMINANCE_Q_N(quality);
    // printf("quantization table = ");
    // print_ch(Q);

    auto quantizated_ch = ImageChannel<s16>(8, 8);
    quantizated_ch.resize(8, 8);

    for (u64 i = 0; i < 8; i++)
        for (u64 j = 0; j < 8; j++) {
            quantizated_ch(i, j) = std::round((f64)(ch(i, j))/Q(i, j));
        }

    return quantizated_ch;
}

inline std::vector<ImageChannel<s16>> luminance_quantization(const std::vector<ImageChannel<f64>>& _MCUs, u8 quality) {
    g_timers.start("quantization MCUs");

    std::vector<ImageChannel<s16>> quantization_MCUs(_MCUs.size());
    for (u64 i = 0; i < _MCUs.size(); i++) {
        quantization_MCUs[i].resize(_MCUs[i].width(), _MCUs[i].height());
        quantization_MCUs[i] = enc::luminance_quantization(_MCUs[i], quality);
    }
    g_timers.end("quantization MCUs");

    u64 duration = g_timers.duration("quantization MCUs");

    INFO("quantization MCUs for %llu MCUs duration = %llu ms\n", _MCUs.size(), duration);

    return quantization_MCUs;
}

inline std::vector<ImageChannel<s16>> chrominance_quantization(const std::vector<ImageChannel<f64>>& _MCUs, u8 quality) {
    g_timers.start("quantization MCUs");

    std::vector<ImageChannel<s16>> quantization_MCUs(_MCUs.size());
    for (u64 i = 0; i < _MCUs.size(); i++) {
        quantization_MCUs[i].resize(_MCUs[i].width(), _MCUs[i].height());
        quantization_MCUs[i] = enc::chrominance_quantization(_MCUs[i], quality);
    }
    g_timers.end("quantization MCUs");

    u64 duration = g_timers.duration("quantization MCUs");

    INFO("quantization MCUs for %llu MCUs duration = %llu ms\n", _MCUs.size(), duration);

    return quantization_MCUs;
}

} // namespace enc

namespace dec {

inline ImageChannel<s16> luminance_quantization(ImageChannel<s16> ch, u8 quality) {
    auto Q = LUMINANCE_Q_N(quality);

    auto decoded_data = ImageChannel<s16>(ch.width(), ch.height());
    decoded_data.resize(ch.width(), ch.height());

    for (u64 i = 0; i < ch.height(); i++)
        for (u64 j = 0; j < ch.width(); j++) {
            decoded_data(i, j) = std::round((f64)(ch(i, j))*Q(i, j));
        }

    return decoded_data;
}

inline ImageChannel<s16> chrominance_quantization(ImageChannel<s16> ch, u8 quality) {
    auto Q = CHROMINANCE_Q_N(quality);

    auto decoded_data = ImageChannel<s16>(ch.width(), ch.height());
    decoded_data.resize(ch.width(), ch.height());

    for (u64 i = 0; i < ch.height(); i++)
        for (u64 j = 0; j < ch.width(); j++) {
            decoded_data(i, j) = std::round((f64)(ch(i, j))*Q(i, j));
        }

    return decoded_data;
}

inline std::vector<ImageChannel<s16>> luminance_quantization(const std::vector<ImageChannel<s16>>& _MCUs, u8 quality) {
    g_timers.start("dequantization MCUs");

    std::vector<ImageChannel<s16>> quantization_MCUs(_MCUs.size());
    for (u64 i = 0; i < _MCUs.size(); i++) {
        quantization_MCUs[i].resize(_MCUs[i].width(), _MCUs[i].height());
        quantization_MCUs[i] = dec::luminance_quantization(_MCUs[i], quality);
    }
    g_timers.end("dequantization MCUs");

    u64 duration = g_timers.duration("dequantization MCUs");

    INFO("dequantization MCUs for %llu MCUs duration = %llu ms\n", _MCUs.size(), duration);

    return quantization_MCUs;
}

inline std::vector<ImageChannel<s16>> chrominance_quantization(const std::vector<ImageChannel<s16>>& _MCUs, u8 quality) {
    g_timers.start("dequantization MCUs");

    std::vector<ImageChannel<s16>> quantization_MCUs(_MCUs.size());
    for (u64 i = 0; i < _MCUs.size(); i++) {
        quantization_MCUs[i].resize(_MCUs[i].width(), _MCUs[i].height());
        quantization_MCUs[i] = dec::chrominance_quantization(_MCUs[i], quality);
    }
    g_timers.end("dequantization MCUs");

    u64 duration = g_timers.duration("dequantization MCUs");

    INFO("dequantization MCUs for %llu MCUs duration = %llu ms\n", _MCUs.size(), duration);

    return quantization_MCUs;
}

} // namespace dec

#endif // QUANTIZATION_H