#include <gtest/gtest.h>
#include "blocksplitting.h"

const RawImageData<u8> data() {
	RawImageData<u8> data(8, 8, std::vector<RawChannelData<u8>>({
		// R
		RawChannelData<u8>(8, 8, {
			255, 255, 255, 255, 255, 255, 255, 255,
			154, 154, 154, 154, 154, 154, 154, 154, 
            153, 153, 153, 153, 153, 153, 153, 153,
            102, 102, 102, 102, 102, 102, 102, 102, 
			255, 255, 255, 255, 255, 255, 255, 255,
			154, 154, 154, 154, 154, 154, 154, 154, 
            153, 153, 153, 153, 153, 153, 153, 153,
            102, 102, 102, 102, 102, 102, 102, 102, 
		}),
		// G
		RawChannelData<u8>(8, 8, {
			255, 255, 255, 255, 255, 255, 255, 255,
			154, 154, 154, 154, 154, 154, 154, 154, 
            153, 153, 153, 153, 153, 153, 153, 153,
            102, 102, 102, 102, 102, 102, 102, 102, 
			255, 255, 255, 255, 255, 255, 255, 255,
			154, 154, 154, 154, 154, 154, 154, 154, 
            153, 153, 153, 153, 153, 153, 153, 153,
            102, 102, 102, 102, 102, 102, 102, 102, 
		}),
		// B
		RawChannelData<u8>(8, 8, {
			255, 255, 255, 255, 255, 255, 255, 255,
			154, 154, 154, 154, 154, 154, 154, 154, 
            153, 153, 153, 153, 153, 153, 153, 153,
            102, 102, 102, 102, 102, 102, 102, 102, 
			255, 255, 255, 255, 255, 255, 255, 255,
			154, 154, 154, 154, 154, 154, 154, 154, 
            153, 153, 153, 153, 153, 153, 153, 153,
            102, 102, 102, 102, 102, 102, 102, 102, 
		}),
	}));
	return data;
}

TEST(test_blocksplitting, splitting_8x8) {
	RawImageData input_data = data();

	auto blocksplitted_data = encode_blocksplitting(input_data, BlockSplittingMode(8, 8));
	
    EXPECT_EQ(blocksplitted_data[0], std::vector<RawChannelData<u8>>({input_data[0]}));
    EXPECT_EQ(blocksplitted_data[1], std::vector<RawChannelData<u8>>({input_data[1]}));
    EXPECT_EQ(blocksplitted_data[2], std::vector<RawChannelData<u8>>({input_data[2]}));
    
    RawImageData decoded_data = decode_blocksplitting(blocksplitted_data);

    EXPECT_EQ(decoded_data[0], input_data[0]);
    EXPECT_EQ(decoded_data[1], input_data[1]);
    EXPECT_EQ(decoded_data[2], input_data[2]);
}