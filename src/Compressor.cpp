#include "Compressor.h"

int Compressor::compress(PPMImageData& rawData) {

    YCbCrConverter::fromRGB(rawData);

    // DCT::FDCT(rawData[0]);

    return 0;
}