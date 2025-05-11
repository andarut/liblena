#include <gtest/gtest.h>

#include "DCT.hpp"

inline const auto TEST_ch = ImageChannel<u8>(8, 8, {
    52, 55, 61, 66,   70, 61,  64, 73,
    63, 59, 55, 90,  109, 85,  69, 72,
    62, 59, 68, 113, 144, 104, 66, 73,
    63, 58, 71, 122, 154, 106, 70, 69,
    67, 61, 68, 104, 126, 88,  68, 70,
    79, 65, 60, 70,   77, 68,  58, 75,
    85, 71, 64, 59,   55, 61,  65, 83,
    87, 79, 69, 68,   65, 76,  78, 94
});

TEST(TEST_DCT, _8x8) {
    auto DCT_data = enc::DCT(TEST_ch);
    EXPECT_EQ(DCT_data.width(), TEST_ch.width());
    EXPECT_EQ(DCT_data.height(), TEST_ch.height());
}