#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "PPMReader.h"

#include "YCbCrConverter.h"
#include "DCT.h"

#include "BitStream.hpp"
#include "QuantizationTable.h"
#include "Zigzag.h"
#include "Quantizator.h"

#include "DPCM.h"
#include "RLC.h"
#include "HuffmanEncoder.h"

const u8 magic = 0xFF;

const u8 SOI = 0xD8;
const u8 APP0 = 0xE0;
const u8 DQT = 0xDB;
const u8 SOF0 = 0xC0;
const u8 DHT = 0xC4;
const u8 SOS = 0xDA;
const u8 EOI = 0xD9;

class Compressor {
public:
    Compressor(BitStream& stream) : mStream(stream) {}
    int compress(PPMImageData& rawData);
private:
    /* Start Of Image */
    void writeSOI();

    /* APP0 */
    void writeAPP0();

    /* Start Of Frame */
    void writeSOF0();

    /* Define Quantization Table */
    void writeDQT(const QuantizationTable& qtable, u8 tq);
    
    /* Define Huffman Table */
    void writeDHT();
    
    /* Start Of Scan */
    // void writeSOS();

    /* End Of Image */
    void writeEOI();
private:
    BitStream& mStream;
};

#endif // COMPRESSOR_H