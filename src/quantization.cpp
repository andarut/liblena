#include "quantization.h"

RawChannelData<u8> create_quantization(u8 quality) {
    assert(quality >= 1 && quality <= 100);
    u64 S = (quality < 50) ? (5000 / quality) : (200 - 2*quality);

    auto Q = RawChannelData<u8>(8, 8);
    for (u64 i = 0; i < Q.height; i++)
        for (u64 j = 0; j < Q.width; j++)
            Q(i, j) = std::clamp((S*Q_50(i, j)+50)/100, (u64)1, (u64)255);

    return Q;
}

RawChannelData<s16> encode_quantization(RawChannelData<s16> data, u8 quality) {
    auto Q = create_quantization(quality);

    auto encoded_data = RawChannelData<s16>(data.width, data.height);

    for (u64 i = 0; i < data.height; i++)
        for (u64 j = 0; j < data.width; j++) {
            encoded_data(i, j) = std::round((f64)(data(i, j))/Q(i, j));
        }

    return encoded_data;
}

RawChannelData<s16> decode_quantization(RawChannelData<s16> data, u8 quality) {
    auto Q = create_quantization(quality);

    auto decoded_data = RawChannelData<s16>(data.width, data.height);

    for (u64 i = 0; i < data.height; i++)
        for (u64 j = 0; j < data.width; j++) {
            decoded_data(i, j) = std::round((f64)(data(i, j))*Q(i, j));
        }

    return decoded_data;
}