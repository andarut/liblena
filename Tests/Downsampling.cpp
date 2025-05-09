#include <gtest/gtest.h>

#include "Downsampling.hpp"

inline const auto TEST_ch = ImageChannel<u8>(4, 2, {
	102, 102, 102, 102,
	154, 204, 153, 204
});

TEST(Downsampling, no_downsampling) {
	auto downsampled_ch = downsampling(TEST_ch, {4, 4, 4});
	
	EXPECT_EQ(downsampled_ch, TEST_ch);
}

// TEST(test_sampling, sampling_441) {
// 	RawImageData input_data = test_data();

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 4, 1));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(input_data.width, decoded_data.width);
// 	EXPECT_EQ(input_data.height, decoded_data.height);
// 	EXPECT_EQ(input_data.numberOfChannels, decoded_data.numberOfChannels);
	
// 	EXPECT_EQ(input_data[0], decoded_data[0]);

// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(4, 2, {
// 		102, 153, 204, 102,
// 		102, 102, 102, 102
// 	}));

// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(4, 2, {
// 		205, 154, 102, 102,
// 		153, 153, 153, 153
// 	}));
// }

// TEST(test_sampling, sampling_422) {
// 	RawImageData input_data = test_data();

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 2, 2));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);
// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(4, 2, {
// 		102, 102, 204, 204,
// 		102, 102, 153, 153
// 	}));
// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(4, 2, {
// 		205, 205, 102, 102,
// 		153, 153, 102, 102
// 	}));
// }

// TEST(test_sampling, sampling_411) {
// 	RawImageData input_data = test_data();

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 1, 1));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);
// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(4, 2, {
// 		102, 102, 102, 102,
// 		102, 102, 102, 102
// 	}));
// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(4, 2, {
// 		205, 205, 205, 205,
// 		153, 153, 153, 153
// 	}));
// }

// TEST(test_sampling, sampling_440) {
// 	RawImageData input_data = test_data();

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 4, 0));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);
// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(4, 2, {
// 		102, 153, 204, 102,
// 		102, 153, 204, 102,
// 	}));
// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(4, 2, {
// 		205, 154, 102, 102,
// 		205, 154, 102, 102,
// 	}));
// }

// TEST(test_sampling, sampling_420) {
// 	RawImageData input_data = test_data();

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 2, 0));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);
// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(4, 2, {
// 		102, 102, 204, 204,
// 		102, 102, 204, 204,
// 	}));
// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(4, 2, {
// 		205, 205, 102, 102,
// 		205, 205, 102, 102,
// 	}));
// }

// TEST(test_sampling, sampling_410) {
// 	RawImageData input_data = test_data();

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 1, 0));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);
// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(4, 2, {
// 		102, 102, 102, 102,
// 		102, 102, 102, 102,
// 	}));
// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(4, 2, {
// 		205, 205, 205, 205,
// 		205, 205, 205, 205,
// 	}));
// }


// TEST(test_sampling, sampling_421) {
// 	RawImageData input_data = test_data();

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 2, 1));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);
// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(4, 2, {
// 		102, 102, 204, 204,
// 		102, 102, 102, 102,
// 	}));
// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(4, 2, {
// 		205, 205, 102, 102,
// 		153, 153, 153, 153,
// 	}));
// }

// TEST(test_sampling, sampling_211) {
// 	RawImageData input_data = test_data();

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(2, 1, 1));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);
// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(4, 2, {
// 		102, 102, 204, 204,
// 		102, 102, 153, 153,
// 	}));
// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(4, 2, {
// 		205, 205, 102, 102,
// 		153, 153, 102, 102,
// 	}));
// }

// TEST(test_sampling, sampling_111) {
// 	RawImageData input_data = test_data();
	
// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(1, 1, 1));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);
	
// 	EXPECT_EQ(decoded_data[0], input_data[0]);
// 	EXPECT_EQ(decoded_data[1], input_data[1]);
// 	EXPECT_EQ(decoded_data[2], input_data[2]);
// }

// TEST(test_sampling, sampling_5x3) {
// 	RawImageData<u8> input_data(4, 2, std::vector<RawChannelData<u8>>({
// 		// R
// 		RawChannelData<u8>(5, 3, {
// 			255, 255, 255, 255, 255,
// 			128, 128, 128, 128, 128,
// 			  0,   0,   0,   0,   0
// 		}),
// 		// G
// 		RawChannelData<u8>(5, 3, {
// 			  0,   0,   0,   0,   0,
// 			128, 128, 128, 128, 128,
// 			255, 255, 255, 255, 255
// 		}),
// 		// B
// 		RawChannelData<u8>(5, 3, {
// 			255, 128,   0, 128, 255,
// 			255, 128,   0, 128, 255,
// 			255, 128,   0, 128, 255
// 		})
// 	}));

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 2, 2));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);
	
// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(5, 3, {
// 		0, 0, 0, 0, 0,
// 		128, 128, 128, 128, 128,
// 		255, 255, 255, 255, 255
// 	}));

// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(5, 3, {
// 		255, 255, 0, 0, 255,
// 		255, 255, 0, 0, 255,
// 		255, 255, 0, 0, 255
// 	}));
// }

// TEST(test_sampling, sampling_3x5) {
// 	RawImageData input_data(3, 5, std::vector<RawChannelData<u8>>({
// 		// R
// 		RawChannelData<u8>(3, 5, {
// 			255, 128,   0,
// 			128, 255, 128,
// 			  0, 128, 255,
// 			128,   0, 128,
// 			255, 128,   0
// 		}),
// 		// G
// 		RawChannelData<u8>(3, 5, {
// 			  0, 128, 255,
// 			128,   0, 128,
// 			255, 128,   0,
// 			128, 255, 128,
// 			  0, 128, 255
// 		}),
// 		// B
// 		RawChannelData<u8>(3, 5, {
// 			128, 128, 128,
// 			255, 255, 255,
// 			  0,   0,   0,
// 			255, 255, 255,
// 			128, 128, 128
// 		})
// 	}));

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 2, 2));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);


// 	EXPECT_EQ(decoded_data[0], input_data[0]);

// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(3, 5, {
// 		  0,  0, 255,
// 		  128, 128, 128,
// 		  255, 255, 0,
// 		  128, 128, 128,
// 		  0, 0, 255
// 	}));

// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(3, 5, {
// 		128, 128, 128,
// 		255, 255, 255,
// 		0, 0, 0,
// 		255, 255, 255,
// 		128, 128, 128
// 	}));
// }

// TEST(test_sampling, sampling_6x3) {
// 	RawImageData<u8> input_data(6, 3, std::vector<RawChannelData<u8>>({
// 		// R
// 		RawChannelData<u8>(6, 3, {
// 			255, 255, 255, 255, 255, 255,
// 			128, 128, 128, 128, 128, 128,
// 			  0,   0,   0,   0,   0,   0
// 		}),
// 		// G
// 		RawChannelData<u8>(6, 3, {
// 			  0,   0,   0,   0,   0,   0,
// 			128, 128, 128, 128, 128, 128,
// 			255, 255, 255, 255, 255, 255
// 		}),
// 		// B
// 		RawChannelData<u8>(6, 3, {
// 			255, 128,   0, 128, 255, 128,
// 			255, 128,   0, 128, 255, 128,
// 			255, 128,   0, 128, 255, 128
// 		})
// 	}));

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 2, 2));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);

// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(6, 3, {
// 		0, 0, 0, 0, 0, 0,
// 		128, 128, 128, 128, 128, 128,
// 		255, 255, 255, 255, 255, 255
// 	}));

// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(6, 3, {
// 		255, 255,   0, 0, 255, 255,
// 		255, 255,   0, 0, 255, 255,
// 		255, 255,   0, 0, 255, 255
// 	}));
// }

// TEST(test_sampling, sampling_7x4) {
// 	RawImageData<u8> input_data(7, 4, std::vector<RawChannelData<u8>>({
// 		// R
// 		RawChannelData<u8>(7, 4, {
// 			255, 255, 255, 255, 255, 255, 255,
// 			128, 128, 128, 128, 128, 128, 128,
// 			  0,   0,   0,   0,   0,   0,   0,
// 			255, 255, 255, 255, 255, 255, 255
// 		}),
// 		// G
// 		RawChannelData<u8>(7, 4, {
// 			  0,   0,   0,   0,   0,   0,   0,
// 			128, 128, 128, 128, 128, 128, 128,
// 			255, 255, 255, 255, 255, 255, 255,
// 			  0,   0,   0,   0,   0,   0,   0
// 		}),
// 		// B
// 		RawChannelData<u8>(7, 4, {
// 			255, 128,   0, 128, 255, 128,   0,
// 			255, 128,   0, 128, 255, 128,   0,
// 			255, 128,   0, 128, 255, 128,   0,
// 			255, 128,   0, 128, 255, 128,   0
// 		})
// 	}));

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 2, 2));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);

// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(7, 4, {
// 		0,   0,   0,   0,   0,   0,   0,
// 		128, 128, 128, 128, 128, 128, 128,
// 		255, 255, 255, 255, 255, 255, 255,
// 		  0,   0,   0,   0,   0,   0,   0
// 	}));

// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(7, 4, {
// 		255, 255, 0, 0, 255, 255, 0,
// 		255, 255, 0, 0, 255, 255, 0,
// 		255, 255, 0, 0, 255, 255, 0,
// 		255, 255, 0, 0, 255, 255, 0
// 	}));
// }

// TEST(test_sampling, sampling_4x4) {
// 	RawImageData<u8> input_data(4, 4, std::vector<RawChannelData<u8>>({
// 		// R
// 		RawChannelData<u8>(4, 4, {
// 			255, 255, 255, 255,
// 			128, 128, 128, 128,
// 			  0,   0,   0,   0,
// 			255, 255, 255, 255
// 		}),
// 		// G
// 		RawChannelData<u8>(4, 4, {
// 			  0,   0,   0,   0,
// 			128, 128, 128, 128,
// 			255, 255, 255, 255,
// 			  0,   0,   0,   0
// 		}),
// 		// B
// 		RawChannelData<u8>(4, 4, {
// 			255, 128,   0, 128,
// 			255, 128,   0, 128,
// 			255, 128,   0, 128,
// 			255, 128,   0, 128
// 		})
// 	}));

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 2, 2));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);

// 	EXPECT_EQ(decoded_data[1], RawChannelData<u8>(4, 4, {
// 		0, 0, 0, 0,
// 		128, 128, 128, 128,
// 		255, 255, 255, 255,
// 		0,0,0,0
// 	}));

// 	EXPECT_EQ(decoded_data[2], RawChannelData<u8>(4, 4, {
// 		255, 255, 0, 0,
// 		255, 255, 0, 0,
// 		255, 255, 0, 0,
// 		255, 255, 0, 0
// 	}));
// }

// TEST(test_sampling, sampling_1x1) {
// 	RawImageData<u8> input_data(1, 1, std::vector<RawChannelData<u8>>({
// 		// R
// 		RawChannelData<u8>(1, 1, {255}),
// 		// G
// 		RawChannelData<u8>(1, 1, {128}),
// 		// B
// 		RawChannelData<u8>(1, 1, {0})
// 	}));

// 	SubsampledImageData encoded_data = encode_subsampling(input_data, SubsamplingMode(4, 2, 2));
// 	RawImageData        decoded_data = decode_subsampling(encoded_data);

// 	EXPECT_EQ(decoded_data[0], input_data[0]);
// 	EXPECT_EQ(decoded_data[1], input_data[1]);
// 	EXPECT_EQ(decoded_data[2], input_data[2]);
// }