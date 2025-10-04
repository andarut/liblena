#include "Compressor.h"
#include "BitStream.hpp"
#include "DCT.h"
#include "HuffmanEncoder.h"
#include "HuffmanTable.h"
#include "Logger.hpp"
#include "QuantizationTable.h"
#include "Quantizator.h"
#include "RLC.h"
#include "YCbCrConverter.h"
#include "Zigzag.h"

/* TODO: MCUs support */
int Compressor::compress(PPMImageData& rawData) {

    QuantizationTable lumin_q;
    {
        auto res = lumin_q_n(50, lumin_q);   
        RETURN_IF_ERROR(res, "Error in creating chrominance q table for quality = %d\n", 50);
    }
    QuantizationTable chrom_q;
    {
        auto res = chrom_q_n(50, chrom_q);
        RETURN_IF_ERROR(res, "Error in creating luminance q table for quality = %d\n", 50);
    }

    /* YCbCr conversion */
    YCbCrConverter::fromRGB(rawData);
    
    /* DCT */
    ImageCh<s32> Y_data(std::move(rawData[0]));
    ImageCh<s32> Cb_data(std::move(rawData[1]));
    ImageCh<s32> Cr_data(std::move(rawData[2]));
    DCT::FDCT(Y_data);
    DCT::FDCT(Cb_data);
    DCT::FDCT(Cr_data);

    /* Quantization */
    Quantizator::quantize(Y_data, lumin_q);
    Quantizator::quantize(Cb_data, chrom_q);
    Quantizator::quantize(Cr_data, chrom_q);

    /* Zigzag */
    std::array<std::vector<s16>, 3> zigzagData;
    {
        auto res = zigzag(Y_data, zigzagData[0]);
        RETURN_IF_ERROR(res, "Error in zigzag for Y\n");
    }
    {
        auto res = zigzag(Cb_data, zigzagData[1]);
        RETURN_IF_ERROR(res, "Error in zigzag for Cb\n");
    }
    {
        auto res = zigzag(Cr_data, zigzagData[2]);
        RETURN_IF_ERROR(res, "Error in zigzag for Cr\n");
    }

    /* DPCM */
    // skip for 8x8

    /* RLC */
    std::array<std::vector<s16>, 3> RLCData;
    {
        auto res = RLC(zigzagData[0], RLCData[0]);
        RETURN_IF_ERROR(res, "Error in RLC for Y\n");
    }
    {
        auto res = RLC(zigzagData[1], RLCData[1]);
        RETURN_IF_ERROR(res, "Error in RLC for Cb\n");
    }
    {
        auto res = RLC(zigzagData[2], RLCData[2]);
        RETURN_IF_ERROR(res, "Error in RLC for Cr\n");
    }

    writeSOI();
    writeAPP0();

    writeDQT(lumin_q, 0x00);
    writeDQT(chrom_q, 0x01);

    writeSOF0();

    // writeDHT();
    write_all_standard_tables(mStream);
    
    /* TODO: writeSOS */
    mStream.write_u8(magic);
    mStream.write_u8(SOS);

    /* length */
    mStream.write_u16(12);

    /* number of compontnts */
    mStream.write_u8(3);

    /* Y component */
    mStream.write_u8(0x01);
    mStream.write_u8(0x00);

    /* Cb component */
    mStream.write_u8(0x02);
    mStream.write_u8(0x11);

    /* Cr compontnt */
    mStream.write_u8(0x03);
    mStream.write_u8(0x11);

    /* start spectrum */
    mStream.write_u8(0x00);

    /* end spectrum */
    mStream.write_u8(0x3F);

    /* appl bit */
    mStream.write_u8(0x00);


    /* Huffman coding */
    BitStream bs;
    HuffmanEncoder encoder(bs);
    {
        auto res = encoder.encodeMCU(MCU_Type::LUMINANCE, RLCData[0][0], RLCData[0]);
        RETURN_IF_ERROR(res, "Error in encodeMCU for Y\n");
    }
    {
        auto res = encoder.encodeMCU(MCU_Type::CHROMINANCE, RLCData[1][0], RLCData[1]);
        RETURN_IF_ERROR(res, "Error in encodeMCU for Cb\n");
    }
    {
        auto res = encoder.encodeMCU(MCU_Type::CHROMINANCE, RLCData[2][0], RLCData[2]);
        RETURN_IF_ERROR(res, "Error in encodeMCU for Cr\n");
    }

    // writeSOS();

    /* end bitstream with 1s instead of 0s */
    while (bs.bits_size() % 8 != 0) {
        bs.write_bits(1, 1);
    }
    INFO("bs size = %d\n", bs.bits_size());

    std::vector<u8> entropyData = bs.getBuf();

    size_t size = bs.bytes_size();
    INFO("Entropy data size = %d\n", size);
    /* byte padding */
    for (int i = 0; i < size; i++) {
        auto b = entropyData[i];
        INFO("byte = %02X\n", b);
        mStream.write_u8(b);
        if (b == 0xFF) {
            mStream.write_u8(0x00);
        }
    }
    
    writeEOI();

    return 0;
}

void Compressor::writeSOI() {
    mStream.write_u8(magic);
    mStream.write_u8(SOI);
}

void Compressor::writeAPP0() {
    /* marker */
    mStream.write_u8(magic);
    mStream.write_u8(APP0);

    /* length */
    mStream.write_u16(16);

    /* JFIF\0 string */
    mStream.write_u8('J');
    mStream.write_u8('F');
    mStream.write_u8('I');
    mStream.write_u8('F');
    mStream.write_u8('\0');

    /* version major */
    mStream.write_u8(1);
    
    /* version minor */
    mStream.write_u8(1);

    /* density unit */
    mStream.write_u8(0x00);

    /* densixy x */
    mStream.write_u16(1);

    /* densixy y */
    mStream.write_u16(1);

    /* thumbnail x */
    mStream.write_u8(0);

    /* thumbnail y */
    mStream.write_u8(0);
}

void Compressor::writeDQT(const QuantizationTable& qtable, u8 tq) {
    /* marker */
    mStream.write_u8(magic);
    mStream.write_u8(DQT);

    /* length */
    mStream.write_u16(67);

    /* precision */
    mStream.write_u8(tq);

    /* table */
    std::vector<u8> zigzag_table;
    auto res = zigzag(qtable, zigzag_table);
    for (int i = 0; i < zigzag_table.size(); i++) {
        mStream.write_u8(zigzag_table[i]);
    }
}

void Compressor::writeSOF0() {
    /* marker */
    mStream.write_u8(magic);
    mStream.write_u8(SOF0);

    /* length */
    mStream.write_u16(17);

    /* bit per sample */
    mStream.write_u8(8);

    /* image height */
    mStream.write_u16(8);
    
    /* image width */
    mStream.write_u16(8);

    /* number of components */
    mStream.write_u8(3);

    /* Y component */
    mStream.write_u8(0x01);
    mStream.write_u8(0x11); // sampling factors
    mStream.write_u8(0); // quantization table id

    /* Cb component */
    mStream.write_u8(0x02);
    mStream.write_u8(0x11); // sampling factors
    mStream.write_u8(1); // quantization table id

    /* Cr component */
    mStream.write_u8(0x03);
    mStream.write_u8(0x11); // sampling factors
    mStream.write_u8(1); // quantization table id
}

// void Compressor::writeDHT() {
//     /* marker */
//     mStream.write_u8(magic);
//     mStream.write_u8(DHT);
    
//     /*  */
// }

// void Compressor::writeSOS() {
//     /* marker */
    

    
// }

void Compressor::writeEOI() {
    /* marker */
    mStream.write_u8(magic);
    mStream.write_u8(EOI);
}