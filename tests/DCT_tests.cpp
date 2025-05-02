#include <gtest/gtest.h>
#include "DCT.h"

TEST(test_DCT, DCT_8x8) {
    auto input_data = RawChannelData<u8>(8, 8, {
        52, 55, 61, 66,   70, 61,  64, 73,
        63, 59, 55, 90,  109, 85,  69, 72,
        62, 59, 68, 113, 144, 104, 66, 73,
        63, 58, 71, 122, 154, 106, 70, 69,
        67, 61, 68, 104, 126, 88,  68, 70,
        79, 65, 60, 70,   77, 68,  58, 75,
        85, 71, 64, 59,   55, 61,  65, 83,
        87, 79, 69, 68,   65, 76,  78, 94
    });

    auto DCT_data = encode_DCT(input_data);
    auto decoded_DCT = decode_DCT(DCT_data);

    DCT_data.print();

    decoded_DCT.print();

    exit(1);

}