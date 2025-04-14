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

TEST(test_sampling, no_sampling) {
    RawImageData data = test_data();

    SamplingMode mode(data.width, data.height, data.numberOfChannels);
    for (u64 i = 0; i < mode.h; i++)
        for (u64 k = 0; k < mode.channels_count; k++)
            mode[i * mode.channels_count + k] = mode.w;

    RawImageData sampled_data = sampling(data, mode);

    EXPECT_EQ(sampled_data.data, data.data);
}

TEST(test_sampling, simple_sampling) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(data.width, data.height, data.numberOfChannels);
    
    // row 0
    mode[0] = 4;
    mode[1] = 2;
    mode[2] = 2;

    // row 1
    mode[3] = 4;
    mode[4] = 2;
    mode[5] = 2;

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
    EXPECT_EQ(sampled_data[1], RawChannelData(4, 2, {
        102, 102, 204, 204,
        102, 102, 153, 153
    }));
    EXPECT_EQ(sampled_data[2], RawChannelData(4, 2, {
        205, 205, 102, 102,
        153, 153, 102, 102
    }));
}

TEST(test_sampling, copy_sampling) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(data.width, data.height, data.numberOfChannels);
    
    // row 0
    mode[0] = 4;
    mode[1] = 4;
    mode[2] = 4;

    // row 1
    mode[3] = 4;
    mode[4] = 4;
    mode[5] = 0;

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
    EXPECT_EQ(sampled_data[1], data[1]);
    EXPECT_EQ(sampled_data[2], RawChannelData(4, 2, {
        205, 154, 102, 102,
        205, 154, 102, 102
    }));
}

TEST(test_sampling, sampling1) {
    RawImageData data = test_data();
    RawChannelData data_R = data[0], data_G = data[1], data_B = data[2];
    printf("R = \n");
    data_R.print();
    printf("G = \n");
    data_G.print();
    printf("B = \n");
    data_B.print();

    SamplingMode mode(data.width, data.height, data.numberOfChannels);
    
    // row 0
    mode[0] = 4;
    mode[1] = 1;
    mode[2] = 1;

    // row 1
    mode[3] = 4;
    mode[4] = 1;
    mode[5] = 1;

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
    EXPECT_EQ(sampled_data[1], RawChannelData(4, 2, {
        102, 102, 102, 102,
        102, 102, 102, 102
    }));
    EXPECT_EQ(sampled_data[2], RawChannelData(4, 2, {
        205, 205, 205, 205,
        153, 153, 153, 153
    }));
}