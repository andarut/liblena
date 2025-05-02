#include <gtest/gtest.h>
#include "quantization.h"

TEST(quantization, quantization_matrices) {
    ASSERT_EQ(Q_50, create_quantization(50));
}

TEST(quantization, quantization) {
    auto test_data = RawChannelData<s16>(8, 8, {
        -415, -33, -58, 35, 58, -51, -15, -12,
        5, -34, 49, 18, 27, 1, -5, 3,
        -46, 14, 80, -35, -50, 19, 7, -18,
        -53, 21, 34, -20, 2, 34, 36, 12,
        9, -2, 9, -5, -32, -15, 45, 37,
        -8, 15, -16, 7, -8, 11, 4, 7,
        19, -28, -2, -26, -2, 7, -44, -21,
        18, 25, -12, -44, 35, 48, -37, -3
    });

    auto encoded_data = encode_quantization(test_data, 50);
    auto decoded_data = decode_quantization(encoded_data, 50);

    ASSERT_EQ(encoded_data, RawChannelData<s16>(8, 8, {
        -26, -3, -6, 2, 2, -1, 0, 0,
        0, -3, 4, 1, 1, 0, 0, 0,
        -3, 1, 5, -1, -1, 0, 0, 0,
        -4, 1, 2, -1, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    })); 

    ASSERT_EQ(decoded_data, RawChannelData<s16>(8, 8, {
        -416, -33,  -60,   32,   48,  -40,    0,    0,
        0,  -36,   56,   19,   26,    0,    0,    0,
        -42  , 13  , 80 , -24 , -40  ,  0  ,  0 ,   0,
        -56,   17 ,  44 , -29,    0 ,   0 ,   0 ,   0,
        18,    0 ,   0  ,  0  ,  0  ,  0 ,   0 ,  0,
        0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0 ,   0,
        0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0 ,   0,
        0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0
    }));

}