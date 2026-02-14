#include <gtest/gtest.h>

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
      INFO("FUCK\n");
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
  std::vector<std::string> testCases = {
    "8x8",
    "1",
    "2",
    "3",
    "4",
    "16x16",
    "128",
    "129"
  };
  for(size_t i = 0; i < 4096; i++) {
    std::string s = "block_" + std::to_string(i);
 //   testCases.push_back(s);
  }
  // MSE = 0.1283
  //testCases.push_back("lenna");


  for(const auto& testCase : testCases) {
    auto res = basicTestCase(testCase);
    RETURN_IF_ERROR(res, "Error in test case: %s\n", testCase.c_str());
  }
  INFO("All tests passed\n");
  
  return 0;
}

