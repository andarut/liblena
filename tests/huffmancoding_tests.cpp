#include <gtest/gtest.h>
#include "huffmancoding.h"

TEST(huffmancoding, test0) {
    const auto test_data = std::vector<s16>({
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
            0, 0, // EOB 0, 0, 0, 0, 0, ...
    });
    BitStream bs;
    // writeLuminanceDC(bs, -26);
    writeLuminanceAC(bs, 0, 2, -3);
    bs.print();
}