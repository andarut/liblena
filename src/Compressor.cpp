#include "Compressor.h"
#include "BitStream.hpp"
#include "DCT.h"
#include "DPCM.h"
#include "HuffmanEncoder.h"
#include "HuffmanTable.h"
#include "ImageCh.h"
#include "Logger.hpp"
#include "MCU.h"
#include "QuantizationTable.h"
#include "Quantizator.h"
#include "RLC.h"
#include "YCbCrConverter.h"
#include "Zigzag.h"

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

    /* Split into MCUs */
    auto Y_MCUs = splitIntoMCUs(rawData[0], {8, 8});
    auto Cb_MCUs = splitIntoMCUs(rawData[1], {8, 8});
    auto Cr_MCUs = splitIntoMCUs(rawData[2], {8, 8});

    /* DCT + Quantization + Zigzag for each MCU */

    /* Y */
    std::vector<std::vector<s16>> Y_zigzagMCUData;
    {
        Y_zigzagMCUData.resize(Y_MCUs.size());

        for (int i = 0; i < Y_MCUs.size(); i++) {
            ImageCh<s32> s32_MCU(std::move(Y_MCUs[i]));

            /* DCT */
            DCT::FDCT(s32_MCU);
            
            /* Quantization */
            Quantizator::quantize(s32_MCU, lumin_q);

            std::vector<s16> Y_zigzagMCU;
            {
                auto res = zigzag(s32_MCU, Y_zigzagMCU);
                RETURN_IF_ERROR(res, "Error in zigzag for Y\n");
            }
            
            Y_zigzagMCUData[i] = std::move(Y_zigzagMCU);
        }

        /* DPCM */
        DPCM(Y_zigzagMCUData);
    }

    /* Cb */
    std::vector<std::vector<s16>> Cb_zigzagMCUData;
    {
        Cb_zigzagMCUData.resize(Cb_MCUs.size());

        for (int i = 0; i < Cb_MCUs.size(); i++) {
            ImageCh<s32> s32_MCU(std::move(Cb_MCUs[i]));

            /* DCT */
            DCT::FDCT(s32_MCU);
            
            /* Quantization */
            Quantizator::quantize(s32_MCU, chrom_q);

            std::vector<s16> Cb_zigzagMCU;
            {
                auto res = zigzag(s32_MCU, Cb_zigzagMCU);
                RETURN_IF_ERROR(res, "Error in zigzag for Cb\n");
            }
            
            Cb_zigzagMCUData[i] = std::move(Cb_zigzagMCU);
        }

        /* DPCM */
        DPCM(Cb_zigzagMCUData);
    }

    /* Cr */
    std::vector<std::vector<s16>> Cr_zigzagMCUData;
    {
        Cr_zigzagMCUData.resize(Cr_MCUs.size());

        for (int i = 0; i < Cr_MCUs.size(); i++) {
            ImageCh<s32> s32_MCU(std::move(Cr_MCUs[i]));

            /* DCT */
            DCT::FDCT(s32_MCU);
            
            /* Quantization */
            Quantizator::quantize(s32_MCU, chrom_q);

            std::vector<s16> Cr_zigzagMCU;
            {
                auto res = zigzag(s32_MCU, Cr_zigzagMCU);
                RETURN_IF_ERROR(res, "Error in zigzag for Cb\n");
            }
            
            Cr_zigzagMCUData[i] = std::move(Cr_zigzagMCU);
        }

        /* DPCM */
        DPCM(Cr_zigzagMCUData);
    }


    BitStream bs;
    HuffmanEncoder encoder(bs);

    /* Note: no subsampling */
    for (int i = 0; i < Y_zigzagMCUData.size(); i++) {

        /* Encode MCU */
        std::vector<s16> Y_RLC_data;
        std::vector<s16> Cb_RLC_data;
        std::vector<s16> Cr_RLC_data;
        /* Y RLC */
        {
            const auto& zigzagMCU = Y_zigzagMCUData[i];
            auto res = RLC(zigzagMCU, Y_RLC_data);
            RETURN_IF_ERROR(res, "Error in RLC for Y\n");
        }
        /* Cb RLC */
        {
            const auto& zigzagMCU = Cb_zigzagMCUData[i];
            auto res = RLC(zigzagMCU, Cb_RLC_data);
            RETURN_IF_ERROR(res, "Error in RLC for Cb\n");
        }
        /* Cr RLC */
        {
            const auto& zigzagMCU = Cr_zigzagMCUData[i];
            auto res = RLC(zigzagMCU, Cr_RLC_data);
            RETURN_IF_ERROR(res, "Error in RLC for Cr\n");
        }
        /* Huffman encoding */
        {
            auto res = encoder.encodeMCU(MCU_Type::LUMINANCE, Y_zigzagMCUData[i][0], Y_RLC_data);
            RETURN_IF_ERROR(res, "Error in encodeMCU for Y\n");
        }
        {
            auto res = encoder.encodeMCU(MCU_Type::CHROMINANCE, Cb_zigzagMCUData[i][0], Cb_RLC_data);
            RETURN_IF_ERROR(res, "Error in encodeMCU for Cb\n");
        }
        {
            auto res = encoder.encodeMCU(MCU_Type::CHROMINANCE, Cr_zigzagMCUData[i][0], Cr_RLC_data);
            RETURN_IF_ERROR(res, "Error in encodeMCU for Cr\n");
        }
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