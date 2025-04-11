#include <gtest/gtest.h>

#include "ycbcr.h"

TEST(test_ycbcr, test_rgb_to_ycbcr) {
    // RGB                                    // YCbCr
    EXPECT_EQ(rgb_to_ycbcr({}),              RawImageData({}));
    EXPECT_EQ(rgb_to_ycbcr({0, 0, 0}),       RawImageData({0, 128, 128}));
    EXPECT_EQ(rgb_to_ycbcr({255, 255, 255}), RawImageData({255, 128, 128}));
    
    EXPECT_EQ(rgb_to_ycbcr({255, 0, 0}),     RawImageData({76, 85, 255}));
    EXPECT_EQ(rgb_to_ycbcr({0, 255, 0}),     RawImageData({150, 44, 21}));
    EXPECT_EQ(rgb_to_ycbcr({0, 0, 255}),     RawImageData({29, 255, 107}));
    
    EXPECT_EQ(rgb_to_ycbcr({128, 128, 128}), RawImageData({128, 128, 128}));

    EXPECT_EQ(rgb_to_ycbcr({123, 45, 67}),   RawImageData({71, 126, 165}));
    EXPECT_EQ(rgb_to_ycbcr({45, 128, 200}),  RawImageData({111, 178, 81}));
    EXPECT_EQ(rgb_to_ycbcr({200, 220, 100}), RawImageData({200, 71, 128}));
    EXPECT_EQ(rgb_to_ycbcr({10, 123, 234}),  RawImageData({102, 203, 62}));
}