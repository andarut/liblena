#include <gtest/gtest.h>

#include "ppm_image.h"

TEST(test_ppmimage, test_read_raw) {
    std::filesystem::path resDir(RESOURCE_DIR);
    std::filesystem::path filePath = resDir / "ppm_image_raw.ppm";

    std::ifstream ppm_file;
    ppm_file.open(filePath);
    EXPECT_TRUE(ppm_file.is_open());
    PPMImage ppmimage = read_ppm_image(ppm_file);
    ppm_file.close();
}

TEST(test_ppmimage, test_read) {
    std::filesystem::path resDir(RESOURCE_DIR);
    std::filesystem::path filePath = resDir / "ppm_image.ppm";

    std::ifstream ppm_file;
    ppm_file.open(filePath);
    EXPECT_TRUE(ppm_file.is_open());
    PPMImage ppmimage = read_ppm_image(ppm_file);
    ppm_file.close();
}

TEST(test_ppmimage, test_perfomance) {
    std::filesystem::path resDir(RESOURCE_DIR);
    std::filesystem::path filePath = resDir / "ppm_image_4K.ppm";

    std::ifstream ppm_file;
    ppm_file.open(filePath);
    EXPECT_TRUE(ppm_file.is_open());
    PPMImage ppmimage = read_ppm_image(ppm_file);
    ppm_file.close();
}