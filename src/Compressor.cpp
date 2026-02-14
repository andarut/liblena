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
#include "Utils.hpp"
#include "YCbCrConverter.h"
#include "Zigzag.h"

int Compressor::compress(PPMImageData& rawData) {

    DEBUG("fuck\n");

    BitStream tracing;

    auto writeMCU = [&tracing]<typename T>(const ImageCh<T>& mcu) {
      for(u64 i = 0; i < mcu.height(); i++) {
        for(u64 j = 0; j < mcu.width(); j++) {
          tracing.write(mcu(i, j));
        }
      }
    };


    tracing.write<u64>(rawData[0].width());
    tracing.write<u64>(rawData[0].height());

    for(int c = 0; c < 3; c++) {
      writeMCU(rawData[c]);
    }

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

    tracing.write(rawData[0].width());
    tracing.write(rawData[0].height());

    for(int c = 0; c < 3; c++) {
      writeMCU(rawData[c]);
    }

    /* Split into MCUs */
    auto Y_MCUs = splitIntoMCUs(rawData[0], {8, 8});
    INFO("Y_MCUs: %d\n", Y_MCUs.size());

    auto Cb_MCUs = splitIntoMCUs(rawData[1], {8, 8});
    INFO("Cb_MCUs: %d\n", Cb_MCUs.size());

    auto Cr_MCUs = splitIntoMCUs(rawData[2], {8, 8});
    INFO("Cr_MCUs: %d\n", Cr_MCUs.size());

    BitStream bs;
    HuffmanEncoder encoder(bs);


    auto encodeChannel = [&writeMCU, &tracing, &lumin_q, &chrom_q]<typename T>(const std::vector<ImageCh<T>>& MCUs, MCU_Type mcuType, std::vector<std::vector<s16>>& DPCMMCUs, std::vector<std::vector<s16>>& RLCMCUs) {

    std::vector<std::vector<s16>> zigzagMCUData(MCUs.size());
    tracing.write<u64>(MCUs.size());

    /* DCT + Quantization + Zigzag for each MCU */
    for (int i = 0; i < MCUs.size(); i++) {
      // Write MCU to tracing
      writeMCU(MCUs[i]);

      /* DCT */
      ImageCh<f64> dctMCU(MCUs[i].width(), MCUs[i].height());
      DCT::FDCT(MCUs[i], dctMCU);

      // Write DCT coeffs to tracing
      writeMCU(dctMCU);

      /* Quantization */
      Quantizator::quantize(dctMCU, mcuType == MCU_Type::LUMINANCE ? lumin_q : chrom_q);

      // Write quantized coeffs to tracing
      writeMCU(dctMCU);

      std::vector<s16> zigzagMCU;
      {
        auto res = zigzag(dctMCU, zigzagMCU);
        RETURN_IF_ERROR(res, "Error in zigzag\n");
      }

      // Write zigzag coeffs to tracing
      tracing.write(zigzagMCU);
      
      zigzagMCUData[i] = std::move(zigzagMCU);
    }

    /* DPCM */
    DPCM(zigzagMCUData, DPCMMCUs);

    // Write DPCM coeffs to tracing
    for(u64 mcu_idx = 0; mcu_idx < DPCMMCUs.size(); mcu_idx++) {
      tracing.write(DPCMMCUs[mcu_idx]);
    }

    /* RLC */
    for(u64 mcu_idx = 0; mcu_idx < DPCMMCUs.size(); mcu_idx++) {
      auto res = RLC(DPCMMCUs[mcu_idx], RLCMCUs[mcu_idx]);
      RETURN_IF_ERROR(res, "Error in RLC\n");

      tracing.write<u64>(RLCMCUs[mcu_idx].size());
      tracing.write(RLCMCUs[mcu_idx]);
    }


    return 0;
  };


  std::vector<std::vector<s16>> Y_DPCM(Y_MCUs.size());
  std::vector<std::vector<s16>> Y_RLC(Y_MCUs.size());
  INFO("encode Y channel\n");
  {
    auto res = encodeChannel(Y_MCUs, MCU_Type::LUMINANCE, Y_DPCM, Y_RLC);
    RETURN_IF_ERROR(res, "Error in encodeChannel\n");
  }
  std::vector<std::vector<s16>> Cb_DPCM(Cb_MCUs.size());
  std::vector<std::vector<s16>> Cb_RLC(Cb_MCUs.size());
  INFO("encode Cb channel\n");
  {
    auto res = encodeChannel(Cb_MCUs, MCU_Type::CHROMINANCE, Cb_DPCM, Cb_RLC);
    RETURN_IF_ERROR(res, "Error in encodeChannel\n");
  }
  std::vector<std::vector<s16>> Cr_DPCM(Cr_MCUs.size());
  std::vector<std::vector<s16>> Cr_RLC(Cr_MCUs.size());
  INFO("encode Cr channel\n");
  { 
    auto res = encodeChannel(Cr_MCUs, MCU_Type::CHROMINANCE, Cr_DPCM, Cr_RLC);
    RETURN_IF_ERROR(res, "Error in encodeChannel\n");
  }

  /* Huffman */
  for (int i = 0; i < Y_MCUs.size(); i++) {
    {
      auto res = encoder.encodeMCU(MCU_Type::LUMINANCE, Y_DPCM[i][0], Y_RLC[i]);
      RETURN_IF_ERROR(res, "Error in encodeMCU\n");
    }
    {
      auto res = encoder.encodeMCU(MCU_Type::CHROMINANCE, Cb_DPCM[i][0], Cb_RLC[i]);
      RETURN_IF_ERROR(res, "Error in encodeMCU\n");
    }
    {
      auto res = encoder.encodeMCU(MCU_Type::CHROMINANCE, Cr_DPCM[i][0], Cr_RLC[i]);
      RETURN_IF_ERROR(res, "Error in encodeMCU\n");
    }
  }


    writeSOI();
    writeAPP0();

    writeDQT(lumin_q, 0x00);
    writeDQT(chrom_q, 0x01);

    writeSOF0(rawData[0].width(), rawData[0].height());

    // writeDHT();
    writeStandardHuffmanTables(mStream);
    
    /* TODO: writeSOS */
    mStream.write_u8(magic);
    mStream.write_u8(SOS);

    /* length */
    mStream.write<u16>(12);

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
        const auto& b = entropyData[i];
        INFO("byte = %02X\n", b);
        mStream.write_u8(b);
        if (b == 0xFF) {
            mStream.write_u8(0x00);
        }
    }
    
    writeEOI();

    tracing.fwrite("tracing.bin");

    return 0;
}

void Compressor::writeSOI() {
  DEBUG("writeSOI\n");
    mStream.write_u8(magic);
    mStream.write_u8(SOI);
}

void Compressor::writeAPP0() {
    /* marker */
    mStream.write_u8(magic);
    mStream.write_u8(APP0);

    /* length */
    mStream.write<u16>(16);

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
    mStream.write<u16>(1);

    /* densixy y */
    mStream.write<u16>(1);

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
    mStream.write<u16>(67);

    /* precision */
    mStream.write_u8(tq);

    /* table */
    std::vector<u8> zigzag_table;
    auto res = zigzag(qtable, zigzag_table);
    for (int i = 0; i < zigzag_table.size(); i++) {
        mStream.write_u8(zigzag_table[i]);
    }
}

void Compressor::writeSOF0(u16 imageWidth, u16 imageHeight) {
    /* marker */
    mStream.write_u8(magic);
    mStream.write_u8(SOF0);

    /* length */
    mStream.write<u16>(17);

    /* bit per sample */
    mStream.write_u8(8);

    /* image height */
    mStream.write<u16>(imageHeight);
    
    /* image width */
    mStream.write<u16>(imageWidth);

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
