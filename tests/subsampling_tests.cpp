#include <gtest/gtest.h>
#include "subsampling.h"

const RawImageData test_data() {
    RawImageData data(4, 2, std::vector<RawChannelData>({
        // R
        RawChannelData(4, 2, {
            102, 102, 102, 102,
            154, 204, 153, 204
        }),
        // G
        RawChannelData(4, 2, {
            102, 153, 204, 102,
            102, 204, 153, 103
        }),
        // B
        RawChannelData(4, 2, {
            205, 154, 102, 102,
            153, 204, 102, 102
        })
    }));
    return data;
}

TEST(test_sampling, sampling_444) {
    RawImageData data = test_data();

    RawImageData subsampled_data = subsampling(data, SubsamplingMode(4, 4, 4));

    EXPECT_EQ(subsampled_data.data, data.data);
}

TEST(test_sampling, sampling_422) {
    RawImageData data = test_data();

    RawImageData subsampled_data = subsampling(data, SubsamplingMode(4, 2, 2));

    EXPECT_EQ(subsampled_data[0], data[0]);
    EXPECT_EQ(subsampled_data[1], RawChannelData(2, 2, {
        102, 204,
        102, 153
    }));
    EXPECT_EQ(subsampled_data[2], RawChannelData(2, 2, {
        205, 102,
        153, 102
    }));
}

TEST(test_sampling, sampling_411) {
    RawImageData data = test_data();

    RawImageData  subsampled_data = subsampling(data, SubsamplingMode(4, 1, 1));

    EXPECT_EQ(subsampled_data[0], data[0]);
    EXPECT_EQ(subsampled_data[1], RawChannelData(1, 2, {
        102,
        102,
    }));
    EXPECT_EQ(subsampled_data[2], RawChannelData(1, 2, {
        205,
        153,
    }));
}

TEST(test_sampling, sampling_440) {
    RawImageData data = test_data();

    RawImageData  subsampled_data = subsampling(data, SubsamplingMode(4, 4, 0));

    EXPECT_EQ(subsampled_data[0], data[0]);
    EXPECT_EQ(subsampled_data[1], RawChannelData(4, 1, {
        102, 153, 204, 102,
    }));
    EXPECT_EQ(subsampled_data[2], RawChannelData(4, 1, {
        205, 154, 102, 102,
    }));
}

TEST(test_sampling, sampling_420) {
    RawImageData data = test_data();

    RawImageData  subsampled_data = subsampling(data, SubsamplingMode(4, 2, 0));

    EXPECT_EQ(subsampled_data[0], data[0]);
    EXPECT_EQ(subsampled_data[1], RawChannelData(2, 1, {
        102, 204,
    }));
    EXPECT_EQ(subsampled_data[2], RawChannelData(2, 1, {
        205, 102, 
    }));
}

TEST(test_sampling, sampling_410) {
    RawImageData data = test_data();

    RawImageData subsampled_data = subsampling(data, SubsamplingMode(4, 1, 0));

    EXPECT_EQ(subsampled_data[0], data[0]);
    EXPECT_EQ(subsampled_data[1], RawChannelData(1, 1, {
        102
    }));
    EXPECT_EQ(subsampled_data[2], RawChannelData(1, 1, {
        205
    }));
}

TEST(test_sampling, sampling_441) {
    RawImageData data = test_data();

    // 4:4:1 mode is not currently supported
    EXPECT_EXIT({
        subsampling(data, SubsamplingMode(4, 4, 1));
    }, ::testing::ExitedWithCode(1), ".*");

}

TEST(test_sampling, sampling_421) {
    RawImageData data = test_data();

    // 4:2:1 mode is not currently supported
    EXPECT_EXIT({
        subsampling(data, SubsamplingMode(4, 2, 1));
    }, ::testing::ExitedWithCode(1), ".*");

}

TEST(test_sampling, sampling_211) {
    RawImageData data = test_data();

    SubsamplingMode mode(2, 1, 1);

    RawImageData subsampled_data = subsampling(data, SubsamplingMode(2, 1, 1));

    EXPECT_EQ(subsampled_data[0], data[0]);
    EXPECT_EQ(subsampled_data[1], RawChannelData(2, 2, {
        102, 204,
        102, 153
    }));
    EXPECT_EQ(subsampled_data[2], RawChannelData(2, 2, {
        205, 102,
        153, 102
    }));
}