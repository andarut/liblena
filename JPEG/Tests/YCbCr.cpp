#include <gtest/gtest.h>

#include "YCbCr.h"

TEST(test_ycbcr, test_rgb_to_ycbcr) {
    // RGB                                    // YCbCr
    EXPECT_EQ(rgb_to_ycbcr<u8>({}),              RawImageData<u8>({}));
    EXPECT_EQ(rgb_to_ycbcr<u8>({0, 0, 0}),       RawImageData<u8>({0, 128, 128}));
    EXPECT_EQ(rgb_to_ycbcr<u8>({255, 255, 255}), RawImageData<u8>({255, 128, 128}));
    
    EXPECT_EQ(rgb_to_ycbcr<u8>({255, 0, 0}),     RawImageData<u8>({76, 85, 255}));
    EXPECT_EQ(rgb_to_ycbcr<u8>({0, 255, 0}),     RawImageData<u8>({150, 44, 21}));
    EXPECT_EQ(rgb_to_ycbcr<u8>({0, 0, 255}),     RawImageData<u8>({29, 255, 107}));
    
    EXPECT_EQ(rgb_to_ycbcr<u8>({128, 128, 128}), RawImageData<u8>({128, 128, 128}));

    EXPECT_EQ(rgb_to_ycbcr<u8>({123, 45, 67}),   RawImageData<u8>({71, 126, 165}));
    EXPECT_EQ(rgb_to_ycbcr<u8>({45, 128, 200}),  RawImageData<u8>({111, 178, 81}));
    EXPECT_EQ(rgb_to_ycbcr<u8>({200, 220, 100}), RawImageData<u8>({200, 71, 128}));
    EXPECT_EQ(rgb_to_ycbcr<u8>({10, 123, 234}),  RawImageData<u8>({102, 203, 62}));
}