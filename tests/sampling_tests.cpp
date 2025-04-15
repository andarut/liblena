#include <gtest/gtest.h>
#include "sampling.h"

const RawImageData test_data() {
    RawImageData data(4, 2, std::vector<RawChannelData>({
        // R
        RawChannelData(4, 2, std::vector<u64>({
            102, 102, 102, 102,
            154, 204, 153, 204
        })),
        // G
        RawChannelData(4, 2, std::vector<u64>({
            102, 153, 204, 102,
            102, 204, 153, 103
        })),
        // B
        RawChannelData(4, 2, std::vector<u64>({
            205, 154, 102, 102,
            153, 204, 102, 102
        }))
    }));
    return data;
}

TEST(test_sampling, sampling_444) {
    RawImageData data = test_data();

    SamplingMode mode(4, 4, 4);

    RawImageData sampled_data = sampling(data, mode);

    EXPECT_EQ(sampled_data.data, data.data);
}

TEST(test_sampling, sampling_422) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(4, 2, 2);

    RawImageData 
        sampled_data = sampling(data, mode);
    RawChannelData
        sampled_data_R = sampled_data[0],
        sampled_data_G = sampled_data[1], 
        sampled_data_B = sampled_data[2];

    printf("R = \n");
    sampled_data_R.print();
    printf("G = \n");
    sampled_data_G.print();
    printf("B = \n");
    sampled_data_B.print();

    EXPECT_EQ(sampled_data[0], data[0]);
    EXPECT_EQ(sampled_data[1], RawChannelData(2, 2, {
        102, 204,
        102, 153,
    }));
    EXPECT_EQ(sampled_data[2], RawChannelData(2, 2, {
        205, 102,
        153, 102,
    }));
}

TEST(test_sampling, sampling_411) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(4, 1, 1);

    RawImageData 
        sampled_data = sampling(data, mode);
    RawChannelData
        sampled_data_R = sampled_data[0],
        sampled_data_G = sampled_data[1], 
        sampled_data_B = sampled_data[2];

    printf("R = \n");
    sampled_data_R.print();
    printf("G = \n");
    sampled_data_G.print();
    printf("B = \n");
    sampled_data_B.print();

    EXPECT_EQ(sampled_data[0], data[0]);
    EXPECT_EQ(sampled_data[1], RawChannelData(1, 2, {
        102,
        102,
    }));
    EXPECT_EQ(sampled_data[2], RawChannelData(1, 2, {
        205,
        153,
    }));
}

TEST(test_sampling, sampling_440) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(4, 4, 0);

    RawImageData 
        sampled_data = sampling(data, mode);
    RawChannelData
        sampled_data_R = sampled_data[0],
        sampled_data_G = sampled_data[1], 
        sampled_data_B = sampled_data[2];

    printf("R = \n");
    sampled_data_R.print();
    printf("G = \n");
    sampled_data_G.print();
    printf("B = \n");
    sampled_data_B.print();

    EXPECT_EQ(sampled_data[0], data[0]);
    EXPECT_EQ(sampled_data[1], RawChannelData(4, 1, {
        102, 153, 204, 102,
    }));
    EXPECT_EQ(sampled_data[2], RawChannelData(4, 1, {
        205, 154, 102, 102,
    }));
}

TEST(test_sampling, sampling_420) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(4, 2, 0);

    RawImageData 
        sampled_data = sampling(data, mode);
    RawChannelData
        sampled_data_R = sampled_data[0],
        sampled_data_G = sampled_data[1], 
        sampled_data_B = sampled_data[2];

    printf("R = \n");
    sampled_data_R.print();
    printf("G = \n");
    sampled_data_G.print();
    printf("B = \n");
    sampled_data_B.print();

    EXPECT_EQ(sampled_data[0], data[0]);
    EXPECT_EQ(sampled_data[1], RawChannelData(2, 1, {
        102, 204,
    }));
    EXPECT_EQ(sampled_data[2], RawChannelData(2, 1, {
        205, 102, 
    }));
}

TEST(test_sampling, sampling_410) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(4, 1, 0);

    RawImageData 
        sampled_data = sampling(data, mode);
    RawChannelData
        sampled_data_R = sampled_data[0],
        sampled_data_G = sampled_data[1], 
        sampled_data_B = sampled_data[2];

    printf("R = \n");
    sampled_data_R.print();
    printf("G = \n");
    sampled_data_G.print();
    printf("B = \n");
    sampled_data_B.print();

    EXPECT_EQ(sampled_data[0], data[0]);
    EXPECT_EQ(sampled_data[1], RawChannelData(1, 1, {
        102
    }));
    EXPECT_EQ(sampled_data[2], RawChannelData(1, 1, {
        205
    }));
}

TEST(test_sampling, sampling_441) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(4, 4, 1);

    // 4:4:1 mode is not currently supported
    EXPECT_EXIT({
        RawImageData 
            sampled_data = sampling(data, mode);
    }, ::testing::ExitedWithCode(1), ".*");

}

TEST(test_sampling, sampling_421) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(4, 2, 1);

    // 4:2:1 mode is not currently supported
    EXPECT_EXIT({
        RawImageData 
            sampled_data = sampling(data, mode);
    }, ::testing::ExitedWithCode(1), ".*");

}