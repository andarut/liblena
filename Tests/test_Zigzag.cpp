#include <gtest/gtest.h>

#include "Zigzag.h"

#include "ImageCh.h"

TEST(TEST_Zigzag, zigzag_order) {
    auto TEST_data = ImageCh<s16>(8, 8, {
        -26, -3, -6, 2, 2, -1, 0, 0,
         0, -2, -4, 1, 1, 0, 0, 0,
        -3, 1, 5, -1, -1, 0, 0, 0,
        -3, 1, 2, -1, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    });
    
    std::vector<s16> outputData;
    auto res = zigzag(TEST_data, outputData);
    
    ASSERT_EQ(outputData, std::vector<s16>({
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

TEST(TEST_Zigzag, reverse_zigzag_order) {
    auto TEST_data = std::vector<s16>({
        -26, -3, 0, -3, -2, -6, 2, -4,
        1, -3, 1, 1, 5, 1 ,2 ,-1,
        1, -1, 2, 0, 0, 0 ,0 ,0 ,
        -1, -1, 0, 0, 0 ,0 ,0, 0, 
        0, 0, 0 ,0 ,0, 0 ,0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0 ,0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    });
    
    ImageCh<s16> outputData(8, 8);
    auto res = reverse_zigzag(TEST_data, outputData);
    
    ASSERT_EQ(outputData, ImageCh<s16>(8, 8, {
        -26, -3, -6, 2, 2, -1, 0, 0,
         0, -2, -4, 1, 1, 0, 0, 0,
        -3, 1, 5, -1, -1, 0, 0, 0,
        -3, 1, 2, -1, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    }));
}