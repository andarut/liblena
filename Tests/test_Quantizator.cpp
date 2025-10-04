#include <gtest/gtest.h>

#include "Quantizator.h"

#include "ImageCh.h"

// TEST(TEST_quantization, lumin_q_n) {
//     int res = lumin_q_n(50, );
// }

TEST(TEST_quantization, quantization) {

    auto testData = ImageCh<s32>(8, 8, {
        -415, -33, -58, 35, 58, -51, -15, -12,
        5, -34, 49, 18, 27, 1, -5, 3,
        -46, 14, 80, -35, -50, 19, 7, -18,
        -53, 21, 34, -20, 2, 34, 36, 12,
        9, -2, 9, -5, -32, -15, 45, 37,
        -8, 15, -16, 7, -8, 11, 4, 7,
        19, -28, -2, -26, -2, 7, -44, -21,
        18, 25, -12, -44, 35, 48, -37, -3
    });

    QuantizationTable q_table;
    auto res = lumin_q_n(50, q_table);

    print("", q_table);

    {
        auto res = Quantizator::quantize(testData, q_table);
    }
    ASSERT_EQ(testData, ImageCh<s32>(8, 8, {
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