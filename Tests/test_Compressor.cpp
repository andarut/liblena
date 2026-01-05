// #include <gtest/gtest.h>
#include <filesystem>
#include <format>

#include "BitStream.hpp"
#include "Logger.hpp"
#include "PPMReader.h"
#include "Compressor.h"

inline std::filesystem::path TESTS_DATA(RESOURCE_DIR);

int basicTestCase(const std::string& filename) {

    const std::string& ppmFilename = filename + ".ppm";
    const std::string& jpegFilename = filename + ".jpeg";
    const std::string& outputFilename = "my_" + jpegFilename;

    std::ifstream PPM_fstream(TESTS_DATA / ppmFilename);
    if (!PPM_fstream.is_open()) {
        ERROR("Input stream is not opened, path = %s\n", (TESTS_DATA / ppmFilename).c_str());
        return 1;
    }

    PPMImageData data;
    {
        auto res = readPPM(PPM_fstream, data);
        RETURN_IF_ERROR(res, "Error in readPPM\n");
    }

    BitStream bs;

    Compressor compressor(bs);
    {
        auto res = compressor.compress(data);
        RETURN_IF_ERROR(res, "Error in compress\n");
    }

    bs.fwrite(outputFilename);

    BitStream orig_bs;
    orig_bs.readFromFile(TESTS_DATA / jpegFilename);

    if (bs != orig_bs) {
        ERROR("Bitstream not equal for %s and %s\n", jpegFilename.c_str(), outputFilename.c_str());
        return 1;
    }
    INFO("Equal bitstreams\n");
    return 0;
}

int main() {
    // RETURN_IF_ERROR(basicTestCase("8x8"), "Test 8x8 failed\n");

    // works
    //RETURN_IF_ERROR(basicTestCase("1"), "Test 1 failed\n");
    //RETURN_IF_ERROR(basicTestCase("2"), "Test 2 failed\n");
    //RETURN_IF_ERROR(basicTestCase("3"), "Test 3 failed\n");
    //RETURN_IF_ERROR(basicTestCase("4"), "Test 4 failed\n");
    //RETURN_IF_ERROR(basicTestCase("16x16"), "Test 16x16 failed\n");
    
    // but not this
    //RETURN_IF_ERROR(basicTestCase("lenna"), "Test lenna failed\n");
    RETURN_IF_ERROR(basicTestCase("block_3"), "Test block 3 failed\n");
    /*
    for(size_t i = 0; i < 4096; i++) {
      std::string s = "block_" + std::to_string(i);
      INFO("Running test %d\n", i);
      RETURN_IF_ERROR(basicTestCase(s), "Test failed\n");

    }
    */
    INFO("All tests passed\n");
    return 0;
}

// TEST(TEST_Compressor, test_8x8) {
//     ASSERT_EQ(basicTestCase("8x8"), 0);
// }


// TEST(TEST_Compressor, 16x16) {
//     ASSERT_EQ(basicTestCase("16x16"), 0);
// }

// TEST(TEST_Compressor, test_512x512) {
//     ASSERT_EQ(basicTestCase("lenna"), 0);
// }
