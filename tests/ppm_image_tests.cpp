#include <gtest/gtest.h>

#include "ppm_image.h"

std::filesystem::path resDir(RESOURCE_DIR);

TEST(test_ppm_image, test_read_raw) {
    std::ifstream ppm_file(resDir / "ppm_image_raw.ppm");
    EXPECT_TRUE(ppm_file.is_open());

    PPMImage ppm_image = read_ppm_image(ppm_file);
    EXPECT_EQ(ppm_image.magicNumber, "P3");
    EXPECT_EQ(ppm_image.width, 4);
    EXPECT_EQ(ppm_image.height, 4);
    EXPECT_EQ(ppm_image.maxVal, 255);

    ppm_file.close();
}

TEST(test_ppm_image, test_read) {
    std::ifstream ppm_file(resDir / "ppm_image.ppm");
    EXPECT_TRUE(ppm_file.is_open());

    PPMImage ppm_image = read_ppm_image(ppm_file);
    EXPECT_EQ(ppm_image.magicNumber, "P6");
    EXPECT_EQ(ppm_image.width, 1920);
    EXPECT_EQ(ppm_image.height, 1280);
    EXPECT_EQ(ppm_image.maxVal, 255);

    ppm_file.close();
}

TEST(test_ppm_image, test_read_perfomance) {
    std::ifstream ppm_file(resDir / "ppm_image_4K.ppm");
    EXPECT_TRUE(ppm_file.is_open());

    PPMImage ppm_image = read_ppm_image(ppm_file);
    EXPECT_EQ(ppm_image.magicNumber, "P6");
    EXPECT_EQ(ppm_image.width, 5184);
    EXPECT_EQ(ppm_image.height, 3456);
    EXPECT_EQ(ppm_image.maxVal, 255);

    ppm_file.close();
}