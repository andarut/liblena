#include <gtest/gtest.h>

#include "Quantization.hpp"

inline auto TEST_quantization_ch = ImageChannel<f64>(8, 8, {
    -415, -33, -58, 35, 58, -51, -15, -12,
    5, -34, 49, 18, 27, 1, -5, 3,
    -46, 14, 80, -35, -50, 19, 7, -18,
    -53, 21, 34, -20, 2, 34, 36, 12,
    9, -2, 9, -5, -32, -15, 45, 37,
    -8, 15, -16, 7, -8, 11, 4, 7,
    19, -28, -2, -26, -2, 7, -44, -21,
    18, 25, -12, -44, 35, 48, -37, -3
});

TEST(TEST_quantization, q_n) {
    ASSERT_EQ(LUMINANCE_Q_50, LUMINANCE_Q_N(50));
    ASSERT_EQ(CHROMINANCE_Q_50, CHROMINANCE_Q_N(50));
}

TEST(TEST_quantization, quantization) {

    auto quantizated_ch = enc::luminance_quantization(TEST_quantization_ch, 50);

    ASSERT_EQ(quantizated_ch, ImageChannel<s16>(8, 8, {
        -26, -3, -6, 2, 2, -1, 0, 0,
        0, -3, 4, 1, 1, 0, 0, 0,
        -3, 1, 5, -1, -1, 0, 0, 0,
        -4, 1, 2, -1, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    })); 
}