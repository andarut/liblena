#include <gtest/gtest.h>

#include "ycbcr.h"

double MSE(const u8_ch& ch1, const u8_ch& ch2) {
    double mse = 0.0;
    for (size_t i = 0; i < ch1.height(); i++) {
        for (size_t j = 0; j < ch1.width(); j++) {
            mse += pow(static_cast<double>(ch1(i, j)) - static_cast<double>(ch2(i, j)), 2);
        }
    }
    mse /= (ch1.width() * ch1.height());
    return mse;
}

void APPROX_ASSERT(const u8_image& im1, const u8_image& im2) {
    for (size_t i = 0; i < 3; i++) {
        double mse = MSE(im1[i], im2[i]);
        INFO("MSE = %f\n", mse);
        ASSERT_TRUE(mse < 0.1);
    }
}

TEST(TEST_YCbCr, FROM_RGB) {
    YCbCrEncoder encoder;
    auto RGB_data = std::array<ImageChannel<u8>, 3>({
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
    auto YCbCr_data = std::array<ImageChannel<u8>, 3>({
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
    encoder.encode(std::move(RGB_data));
    APPROX_ASSERT(RGB_data, YCbCr_data);
}