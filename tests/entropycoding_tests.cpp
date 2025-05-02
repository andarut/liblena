#include <gtest/gtest.h>
#include "entropycoding.h"

TEST(entropycoding, zigzag) {
    auto test_data = RawChannelData<s16>(8, 8, {
        -26, -3, -6, 2, 2, -1, 0, 0,
         0, -2, -4, 1, 1, 0, 0, 0,
        -3, 1, 5, -1, -1, 0, 0, 0,
        -3, 1, 2, -1, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    });
    auto encoded_data = zigzag_order(test_data);
    
    ASSERT_EQ(encoded_data, std::vector<s16>({
        -26, -3, 0, -3, -2, -6, 2, -4,
        1, -3, 1, 1, 5, 1 ,2 ,-1,
        1, -1, 2, 0, 0, 0 ,0 ,0 ,
        -1, -1, 0, 0, 0 ,0 ,0, 0, 
        0, 0, 0 ,0 ,0, 0 ,0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0 ,0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    }));
}