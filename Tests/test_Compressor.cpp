// #include <gtest/gtest.h>
#include <filesystem>

#include "BitStream.hpp"
#include "Logger.hpp"
#include "PPMReader.h"
#include "Compressor.h"

inline std::filesystem::path TESTS_DATA(RESOURCE_DIR);

int main() {
    
    std::ifstream PPM_fstream(TESTS_DATA / "8x8.ppm");
    if (!PPM_fstream.is_open()) {
        ERROR("Input stream is not opened\n");
    }

    PPMImageData data;
    {
        auto res = readPPM(PPM_fstream, data);
        INFO("res = %d\n", res);
        RETURN_IF_ERROR(res, "Error in readPPM, res = %d\n", res);
    }

    BitStream bs;

    Compressor compressor(bs);
    {
        auto res = compressor.compress(data);
        RETURN_IF_ERROR(res, "Error in compress\n");
    }

    print(bs);

    INFO("Writing data into my.jpeg");
    bs.fwrite("my.jpeg");

    BitStream orig_bs;
    orig_bs.fread(TESTS_DATA / "8x8.jpeg");

    if (bs != orig_bs) {
        ERROR("BITSTREAMS NOT EQUAL!");
        return 1;
    }

    INFO("EQUAL BITSTREMS!");

    return 0;
}