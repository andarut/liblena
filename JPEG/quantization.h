#ifndef QUANTIZATION_H
#define QUANTIZATION_H

#include "utils.hpp"
#include "types.hpp"
#include "globals.h"

/* TODO: create quantization matrices for all qualities? */

inline auto Q_50 = RawChannelData<u8>(8, 8, {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
});

RawChannelData<u8> create_quantization(u8 quality);

RawChannelData<s16> encode_quantization(RawChannelData<s16> data, u8 quality);

RawChannelData<s16> decode_quantization(RawChannelData<s16> data, u8 quality);

#endif // QUANTIZATION_H