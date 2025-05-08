#include <gtest/gtest.h>

#include "Inputs/PPM.hpp"

std::filesystem::path TESTS_DATA(RESOURCE_DIR);

TEST(PPM_test, TEST1) {
    std::ifstream PPM_file(TESTS_DATA / "ppm_image_raw.ppm");
    EXPECT_TRUE(PPM_file.is_open());

    auto PPM_image = PPM(PPM_file);

    EXPECT_EQ(PPM_image[0].width(), 4);
    EXPECT_EQ(PPM_image[0].height(), 4);

    EXPECT_EQ(PPM_image[1].width(), 4);
    EXPECT_EQ(PPM_image[1].height(), 4);

    EXPECT_EQ(PPM_image[2].width(), 4);
    EXPECT_EQ(PPM_image[2].height(), 4);

    PPM_file.close();
}

TEST(PPM_test, TEST2) {
    std::ifstream PPM_file(TESTS_DATA / "ppm_image.ppm");
    EXPECT_TRUE(PPM_file.is_open());

    auto PPM_image = PPM(PPM_file);

    EXPECT_EQ(PPM_image[0].width(), 1920);
    EXPECT_EQ(PPM_image[0].height(), 1280);
    
    EXPECT_EQ(PPM_image[1].width(), 1920);
    EXPECT_EQ(PPM_image[1].height(), 1280);
    
    EXPECT_EQ(PPM_image[2].width(), 1920);
    EXPECT_EQ(PPM_image[2].height(), 1280);

    PPM_file.close();
}

TEST(PPM_test, TEST_PERFOMANCE) {
    std::ifstream PPM_file(TESTS_DATA / "ppm_image_4K.ppm");
    EXPECT_TRUE(PPM_file.is_open());

    auto PPM_image = PPM(PPM_file);

    EXPECT_EQ(PPM_image[0].width(), 5184);
    EXPECT_EQ(PPM_image[0].height(), 3456);

    EXPECT_EQ(PPM_image[1].width(), 5184);
    EXPECT_EQ(PPM_image[1].height(), 3456);

    EXPECT_EQ(PPM_image[2].width(), 5184);
    EXPECT_EQ(PPM_image[2].height(), 3456);

    PPM_file.close();
}