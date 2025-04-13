#include <gtest/gtest.h>
#include "sampling.h"

const RawImageData test_data() {
    RawImageData data(4, 2, 3);

    data(0, 0, 0) = 102;
    data(0, 0, 1) = 102;
    data(0, 0, 2) = 205;

    data(0, 1, 0) = 102;
    data(0, 1, 1) = 153;
    data(0, 1, 2) = 154;

    data(0, 2, 0) = 102;
    data(0, 2, 1) = 204;
    data(0, 2, 2) = 102;

    data(0, 3, 0) = 102;
    data(0, 3, 1) = 102;
    data(0, 3, 2) = 102;

    data(1, 0, 0) = 154;
    data(1, 0, 1) = 102;
    data(1, 0, 2) = 153;

    data(1, 1, 0) = 204;
    data(1, 1, 1) = 204;
    data(1, 1, 2) = 204;

    data(1, 2, 0) = 153;
    data(1, 2, 1) = 153;
    data(1, 2, 2) = 102;

    data(1, 3, 0) = 204;
    data(1, 3, 1) = 103;
    data(1, 3, 2) = 102;

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
    RawImageData data_R = data[0], data_G = data[1], data_B = data[2];
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
        sampled_data = sampling(data, mode),
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
    EXPECT_EQ(sampled_data[1], RawImageData(4, 2, 1, {
        102, 102, 204, 204,
        102, 102, 153, 153
    }));
    EXPECT_EQ(sampled_data[2], RawImageData(4, 2, 1, {
        205, 205, 102, 102,
        153, 153, 102, 102
    }));
}

TEST(test_sampling, copy_sampling) {
    RawImageData data = test_data();
    RawImageData data_R = data[0], data_G = data[1], data_B = data[2];
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
        sampled_data = sampling(data, mode),
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
    EXPECT_EQ(sampled_data[2], RawImageData(4, 2, 1, {
        205, 154, 102, 102,
        205, 154, 102, 102
    }));
}

TEST(test_sampling, sampling1) {
    RawImageData data = test_data();
    RawImageData data_R = data[0], data_G = data[1], data_B = data[2];
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
        sampled_data = sampling(data, mode),
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
    EXPECT_EQ(sampled_data[1], RawImageData(4, 2, 1, {
        102, 102, 102, 102,
        102, 102, 102, 102
    }));
    EXPECT_EQ(sampled_data[2], RawImageData(4, 2, 1, {
        205, 205, 205, 205,
        153, 153, 153, 153
    }));
}