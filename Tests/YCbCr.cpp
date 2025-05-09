#include <gtest/gtest.h>

#include "JPEG/YCbCr.hpp"

TEST(YCbCr, FROM_RGB) {
    auto RGB_data = std::vector<ImageChannel<u8>>({
        ImageChannel(3, 3, std::vector<u8>({
            0, 255, 255,
            0, 0, 128,
            123, 45, 200
        })),
        ImageChannel(3, 3, std::vector<u8>({
            0, 255, 0,
            255, 0, 128,
            45, 128, 220
        })),
        ImageChannel(3, 3, std::vector<u8>({
            0, 255, 0,
            0, 255, 128,
            67, 200, 100
        }))
    });
    auto YCbCr_data = std::vector<ImageChannel<u8>>({
        ImageChannel(3, 3, std::vector<u8>({
            0, 255, 76,
            150, 29, 128,
            71, 111, 200
        })),
        ImageChannel(3, 3, std::vector<u8>({
            128, 128, 85,
            44, 255, 128,
            126, 178, 71
        })),
        ImageChannel(3, 3, std::vector<u8>({
            128, 128, 255,
            21, 107, 128,
            165, 81, 128
        }))
    });
    auto TEST_data = RGB2YCbCr(RGB_data);
    EXPECT_EQ(TEST_data, YCbCr_data);
}