#include <gtest/gtest.h>

#include "ppm_image.h"
#include "ycbcr.h"
#include "subsampling.h"


TEST(test_jpeg, test_ppm_ycbcr_subsampling) {
	std::filesystem::path resDir(RESOURCE_DIR);
	std::ifstream ppm_file(resDir / "4x2.ppm");
	EXPECT_TRUE(ppm_file.is_open());

	PPMImage ppm_image = read_ppm_image<u8>(ppm_file);
	EXPECT_EQ(ppm_image.magicNumber, "P6");
	EXPECT_EQ(ppm_image.width, 4);
	EXPECT_EQ(ppm_image.height, 2);
	EXPECT_EQ(ppm_image.maxVal, 255);

	RawImageData ycbcr_data = rgb_to_ycbcr<u8>(ppm_image);

	EXPECT_EQ(ycbcr_data[0], RawChannelData<u8>(4, 2, {
		 29, 150,  76, 226,
		179, 255,   0, 105        
	}));

	EXPECT_EQ(ycbcr_data[1], RawChannelData<u8>(4, 2, {
		255,  44,  85,   0,
		171, 128, 128, 212
	}));

	EXPECT_EQ(ycbcr_data[2], RawChannelData<u8>(4, 2, {
		107,  21, 255, 149,
		  0, 128, 128, 235
	}));
	
	SubsampledImageData subsampled_ycbcr_data = encode_subsampling(ycbcr_data, SubsamplingMode(4, 2, 2));

	EXPECT_EQ(subsampled_ycbcr_data[1], RawChannelData<u8>(2, 2, {
		255,  85,
		171, 128
	}));

	EXPECT_EQ(subsampled_ycbcr_data[2], RawChannelData<u8>(2, 2, {
		107, 255,
		  0, 128
	}));

	ppm_file.close();
}

TEST(test_jpeg, test_ppm_ycbcr_subsampling_blocks) {
	std::filesystem::path resDir(RESOURCE_DIR);
	std::ifstream ppm_file(resDir / "8x4.ppm");
	EXPECT_TRUE(ppm_file.is_open());

	PPMImage ppm_image = read_ppm_image<u8>(ppm_file);
	EXPECT_EQ(ppm_image.magicNumber, "P6");
	EXPECT_EQ(ppm_image.width, 8);
	EXPECT_EQ(ppm_image.height, 4);
	EXPECT_EQ(ppm_image.maxVal, 255);

	RawImageData ycbcr_data = rgb_to_ycbcr<u8>(ppm_image);

	EXPECT_EQ(ycbcr_data[0], RawChannelData<u8>(8, 4, {
		29, 29, 150, 150, 76, 76, 226, 226,
		29, 29, 150, 150, 76, 76, 226, 226,
		179, 179, 255, 255, 0, 0, 105, 105,
		179, 179, 255, 255, 0, 0, 105, 105
	}));

	EXPECT_EQ(ycbcr_data[1], RawChannelData<u8>(8, 4, {
		255, 255, 44, 44, 85, 85, 0, 0,
		255, 255, 44, 44, 85, 85, 0, 0,
		171, 171, 128, 128, 128, 128, 212, 212,
		171, 171, 128, 128, 128, 128, 212, 212
	}));

	EXPECT_EQ(ycbcr_data[2], RawChannelData<u8>(8, 4, {
		107, 107, 21, 21, 255, 255, 149, 149,
		107, 107, 21, 21, 255, 255, 149, 149,
		0, 0, 128, 128, 128, 128, 235, 235,
		0, 0, 128, 128, 128, 128, 235, 235
	}));

	SubsampledImageData subsampled_ycbcr_data = encode_subsampling(ycbcr_data, SubsamplingMode(4, 2, 2));


	EXPECT_EQ(subsampled_ycbcr_data[1], RawChannelData<u8>(4, 4, {
		255, 44, 85, 0,
		255, 44, 85, 0,
		171, 128, 128, 212,
		171, 128, 128, 212
	}));

	EXPECT_EQ(subsampled_ycbcr_data[2], RawChannelData<u8>(4, 4, {
		107, 21, 255, 149,
		107, 21, 255, 149,
		0, 128, 128, 235,
		0, 128, 128, 235
	}));

	ppm_file.close();
}

TEST(test_jpeg, test_ppm_ycbcr_subsampling_blocks_411) {
	std::filesystem::path resDir(RESOURCE_DIR);
	std::ifstream ppm_file(resDir / "8x4.ppm");
	EXPECT_TRUE(ppm_file.is_open());

	PPMImage ppm_image = read_ppm_image<u8>(ppm_file);
	EXPECT_EQ(ppm_image.magicNumber, "P6");
	EXPECT_EQ(ppm_image.width, 8);
	EXPECT_EQ(ppm_image.height, 4);
	EXPECT_EQ(ppm_image.maxVal, 255);

	RawImageData ycbcr_data = rgb_to_ycbcr<u8>(ppm_image);

	EXPECT_EQ(ycbcr_data[0], RawChannelData<u8>(8, 4, {
		29, 29, 150, 150, 76, 76, 226, 226,
		29, 29, 150, 150, 76, 76, 226, 226,
		179, 179, 255, 255, 0, 0, 105, 105,
		179, 179, 255, 255, 0, 0, 105, 105
	}));

	EXPECT_EQ(ycbcr_data[1], RawChannelData<u8>(8, 4, {
		255, 255, 44, 44, 85, 85, 0, 0,
		255, 255, 44, 44, 85, 85, 0, 0,
		171, 171, 128, 128, 128, 128, 212, 212,
		171, 171, 128, 128, 128, 128, 212, 212

	}));

	EXPECT_EQ(ycbcr_data[2], RawChannelData<u8>(8, 4, {
		107, 107, 21, 21, 255, 255, 149, 149,
		107, 107, 21, 21, 255, 255, 149, 149,
		0, 0, 128, 128, 128, 128, 235, 235,
		0, 0, 128, 128, 128, 128, 235, 235
	}));

	SubsampledImageData subsampled_ycbcr_data = encode_subsampling(ycbcr_data, SubsamplingMode(4, 1, 1));

	EXPECT_EQ(subsampled_ycbcr_data[1], RawChannelData<u8>(2, 4, {
		255, 85,
		255, 85,
		171, 128,
		171, 128
	}));

	EXPECT_EQ(subsampled_ycbcr_data[2], RawChannelData<u8>(2, 4, {
		107, 255,
		107, 255,
		0, 128,
		0, 128,
	}));

	ppm_file.close();
}

TEST(test_jpeg, test_ppm_ycbcr_subsampling_perfomance) {
	std::filesystem::path resDir(RESOURCE_DIR);
	std::ifstream ppm_file(resDir / "ppm_image_4K.ppm");
	EXPECT_TRUE(ppm_file.is_open());

	PPMImage ppm_image = read_ppm_image<u8>(ppm_file);
	EXPECT_EQ(ppm_image.magicNumber, "P6");
	EXPECT_EQ(ppm_image.maxVal, 255);

	printf("width = %lld\n", ppm_image.width);
	printf("height = %lld\n", ppm_image.height);

	RawImageData ycbcr_data = rgb_to_ycbcr<u8>(ppm_image);

	SubsampledImageData subsampled_ycbcr_data = encode_subsampling<u8>(ycbcr_data, SubsamplingMode(4, 2, 2));

	ppm_file.close();
}
