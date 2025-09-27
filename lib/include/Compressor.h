#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "PPMReader.h"

#include "YCbCrConverter.h"
#include "DCT.h"

#include "BitStream.hpp"

class Compressor {
public:
    Compressor(BitStream& stream) : mStream(stream) {}
    int compress(PPMImageData& rawData);
private:
    /* Start Of Image */
    void writeSOI();

    /* Start Of Frame */
    void writeSOF0();

    /* Define Quantization Table */
    void writeDQT();
    
    /* Define Huffman Table */
    void writeDHT();
    
    /* Start Of Scan */
    void writeSOS();

    /* End Of Image */
    void writeEOI();
private:
    BitStream& mStream;
};

#endif // COMPRESSOR_H