#include <gtest/gtest.h>

#include "HuffmanEncoder.h"

#include "ImageCh.h"

TEST(TEST_ENCODING, BASIC_TEST) {

    auto DC = 0;
    auto ACs = std::vector<s16>({
        0, 2, -3, // -3,
        1, 2, -3, // 0, -3
        0, 2, -2, // -2
        0, 3, -6, // -6
        0, 2,  2, // 2
        0, 3, -4, // -4
        0, 1,  1, // 1
        0, 2, -3, // -3
        0, 1,  1, // 1
        0, 1,  1, // 1
        0, 3,  5, // 5
        0, 1,  1, // 1
        0, 2,  2, // 2
        0, 1, -1, // -1
        0, 1,  1, // 1
        0, 1, -1, // -1
        0, 2,  2, // 2
        5, 1, -1, // 0, 0, 0, 0, 0, -1
        0, 1, -1, // -1
           15, 0, // ZRL (16 zeros)
           15, 0, // ZRL (16 zeros)
           0, 0,  // EOB 0, 0, 0, 0, 0
    });

    BitStream bs;

    // TODO
    // HuffmanEncoder encoder(std::move(bs));
    // encoder.encodeMCU(MCU_Type::LUMINANCE, DC, ACs);

    
}