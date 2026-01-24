#ifndef ENTROPYCODING_H
#define ENTROPYCODING_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"
#include "BitStream.hpp"
#include "HuffmanTable.h"

enum MCU_Type {
	LUMINANCE,
	CHROMINANCE
};

/*
	NOTE: Using predetermined huffman table
*/
class HuffmanEncoder {
public:
	HuffmanEncoder(BitStream& stream) : mStream(stream) {}
	int encodeMCU(MCU_Type type, s16 DC, const std::vector<s16>& ACs);
//private:
	int writeDC(s32 DC_diff, HuffDCTable& table);
	void writeAC(u32 run, u16 size, s32 ampl, HuffACTable& table);
	void writeAC(u32 run, u16 size, HuffACTable& table);
	void writeVLI(s32 ampl, u16 size);
private:
	static u8 categoryDC(s32 diff);
private:
	BitStream& mStream;
};


/*
	TODO: HuffmanDecoder
*/

// namespace dec {

// inline std::array<std::vector<ImageChannel<s16>>, 3> entropy_coding(BitStream& bs, \
// 	u64 Y_MCUs_count, u64 Cb_MCUs_count, u64 Cr_MCUs_count) {
// 	std::array<std::vector<ImageChannel<s16>>, 3> chs_MCUs;

// 	chs_MCUs[0].resize(Y_MCUs_count);
// 	chs_MCUs[1].resize(Cb_MCUs_count);
// 	chs_MCUs[2].resize(Cr_MCUs_count);

// 	auto Y_first_DC  = decode_dc(bs, LUMINANCE_DC_MAP());
// 	auto Cb_first_DC = decode_dc(bs, CHROMINANCE_DC_MAP());
// 	auto Cr_first_DC = decode_dc(bs, CHROMINANCE_DC_MAP());

// 	INFO("DECODED Y_first_DC = %d\n", Y_first_DC);
// 	INFO("DECODED Cb_first_DC = %d\n", Cb_first_DC);
// 	INFO("DECODED Cr_first_DC = %d\n", Cr_first_DC);
	
// 	/* Y */
// 	for (u64 i = 0; i < Y_MCUs_count; i++) {
		
// 		/* Reverse DPCM */
// 		s16 DC = Y_first_DC + decode_dc(bs, LUMINANCE_DC_MAP());
// 		INFO("MCU %d Y DC = %d\n", i, DC);

// 		/* RLE */
// 		std::vector<s16> RLE_data;
// 		while(1) {
// 			s16 AC = decode_ac_symbol(bs, LUMINANCE_AC_MAP());
// 			uint8_t run  = AC >> 4;
// 			uint8_t size = AC & 0x0F;
// 			RLE_data.push_back(run);
// 			RLE_data.push_back(size);
			
// 			if (size == 0 && run == 0) break;
// 			if (size == 0 && run == 0xF) continue;
// 			s16 ampl = extend_dc_value(size, bs);
// 			RLE_data.push_back(ampl);
// 		}
// 		INFO("MCU %d RLE_data\n", i);
// 		print_rle(RLE_data);

// 		chs_MCUs[0][i] = dec::zigzag(dec::RLE(DC, RLE_data), 8);
// 		INFO("DECODED Y MCU %d\n", i);
// 		print(chs_MCUs[0][i]);
// 	}

// 	/* Cb */
// 	for (u64 i = 0; i < Cb_MCUs_count; i++) {
		
// 		/* Reverse DPCM */
// 		s16 DC = Cb_first_DC + decode_dc(bs, CHROMINANCE_DC_MAP());
// 		INFO("MCU %d Cb DC = %d\n", i, DC);

// 		/* RLE */
// 		std::vector<s16> RLE_data;
// 		while(1) {
// 			s16 AC = decode_ac_symbol(bs, CHROMINANCE_AC_MAP());
// 			uint8_t run  = AC >> 4;
// 			uint8_t size = AC & 0x0F;
// 			RLE_data.push_back(run);
// 			RLE_data.push_back(size);
			
// 			if (size == 0 && run == 0) break;
// 			if (size == 0 && run == 0xF) continue;
// 			s16 ampl = extend_dc_value(size, bs);
// 			RLE_data.push_back(ampl);
// 		}
// 		INFO("MCU %d Cb RLE_data\n", i);
// 		print_rle(RLE_data);

// 		chs_MCUs[1][i] = dec::zigzag(dec::RLE(DC, RLE_data), 8);
// 		INFO("DECODED Cb MCU %d\n", i);
// 		print(chs_MCUs[1][i]);
// 	}

// 	/* Cb */
// 	for (u64 i = 0; i < Cr_MCUs_count; i++) {
		
// 		/* Reverse DPCM */
// 		s16 DC = Cr_first_DC + decode_dc(bs, CHROMINANCE_DC_MAP());
// 		INFO("MCU %d Cr DC = %d\n", i, DC);

// 		/* RLE */
// 		std::vector<s16> RLE_data;
// 		while(1) {
// 			s16 AC = decode_ac_symbol(bs, CHROMINANCE_AC_MAP());
// 			uint8_t run  = AC >> 4;
// 			uint8_t size = AC & 0x0F;
// 			RLE_data.push_back(run);
// 			RLE_data.push_back(size);
			
// 			if (size == 0 && run == 0) break;
// 			if (size == 0 && run == 0xF) continue;
// 			s16 ampl = extend_dc_value(size, bs);
// 			RLE_data.push_back(ampl);
// 		}
// 		INFO("MCU %d Cr RLE_data\n", i);
// 		print_rle(RLE_data);

// 		chs_MCUs[2][i] = dec::zigzag(dec::RLE(DC, RLE_data), 8);
// 		INFO("DECODED Cr MCU %d\n", i);
// 		print(chs_MCUs[2][i]);
// 	}
	
// 	return chs_MCUs;
// }

// } // namespace dec

#endif // ENTROPYCODING_H
