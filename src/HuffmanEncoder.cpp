#include "HuffmanEncoder.h"

int HuffmanEncoder::encodeMCU(MCU_Type type, s16 DC, const std::vector<s16>& RLC_ACs) {
    writeDC(DC, type == LUMINANCE ? LUMINANCE_DC : CHROMINANCE_DC);
    for (u64 i = 0; i < RLC_ACs.size(); i += 3) {
        auto run =  RLC_ACs[i];
        auto size = RLC_ACs[i+1];
        if (size == 0 && run == 0) {
            INFO("WRITE EOB\n");
            writeAC(run, size, type == LUMINANCE ? LUMINANCE_AC : CHROMINANCE_AC);
            break;
        }
        if (size == 0 && run == 0xF) {
            INFO("WRITE ZRL\n");
            writeAC(run, size, type == LUMINANCE ? LUMINANCE_AC : CHROMINANCE_AC);
            i--;
            continue;
        }

        auto ampl = RLC_ACs[i+2];
        INFO("ENCODING AC = %d\n", ampl);
        writeAC(run, size, ampl, type == LUMINANCE ? LUMINANCE_AC : CHROMINANCE_AC);
    }
    // print(mStream);
    return 0;
}

int HuffmanEncoder::writeDC(s32 DC_diff, HuffDCTable& table) {
	INFO("DC_diff = %d\n", DC_diff);
	u8 cat = categoryDC(DC_diff);
	if (cat < 0 || cat > 12) {
    return 1;
  }
	auto &e = table[cat];
	INFO("cat = %llu, code = %llu, length = %d\n", cat, e.code, e.length);
	mStream.write_bits(e.code, e.length);

	if (cat > 0) {
		writeVLI(DC_diff, cat);
  }
  return 0;
}

u8 HuffmanEncoder::categoryDC(s32 diff) {
	if (diff == 0) return 0;
	u32 abs_amplitude = std::abs(diff);
	u8 cat = 1;
	while (abs_amplitude >>= 1) ++cat;
	return cat;
}

void HuffmanEncoder::writeVLI(s32 ampl, u16 size) {
	u32 abs_ampl = std::abs(ampl);

	INFO("amplitude = %d, size = %d, VLI = %s\n", ampl, size, bit_string(abs_ampl, size).c_str());

	for (int i = int(size) - 1; i >= 0; --i) {
		uint32_t bit = (abs_ampl >> i) & 1u;
		if (ampl < 0) bit ^= 1u;
		DEBUG("write bit = %d\n", bit);
		mStream.write_bits(bit, 1);
	}
}

void HuffmanEncoder::writeAC(u32 run, u16 size, HuffACTable& table) {
	INFO("run = %llu, size = %llu\n", run, size);
	auto &e = table[(run<<4)|size];
	INFO("code = %s / %02x, length = %d\n", bit_string(e.code, e.length).c_str(), e.code, e.length);
	mStream.write_bits(e.code, e.length);
}

void HuffmanEncoder::writeAC(u32 run, u16 size, s32 ampl, HuffACTable& table) {
	writeAC(run, size, table);
	writeVLI(ampl, size);
}

// static u16 decode_dc_category(BitStream& bs, const std::array<KeySymbol>& dcMap) {
//     u16 code = 0;
//     for (uint8_t length = 1; length <= 16; ++length) {
//         code = (code << 1) | bs.nextBit();
//         uint32_t key = (static_cast<uint32_t>(length) << 16) | code;
//         auto it = dcMap.find(key);
//         if (it != dcMap.end()) {
//             return it->second;  // category 0–11
//         }
//     }
//     throw std::runtime_error("Invalid DC Huffman code");
// }

// static s16 extend_dc_value(uint16_t S, BitStream& bs) {
//     if (S == 0) return 0;
//     s16 bits = 0;
//     for (u16 i = 0; i < S; ++i) {
//         bits = (bits << 1) | bs.nextBit();
//     }
//     // JPEG “extend”: if the leading bit is 0, subtract (2^S - 1)
//     s16 threshold = 1 << (S - 1);
//     if (bits < threshold)
//         bits -= (1 << S) - 1;
//     return bits;
// }


// static s16 decode_dc(BitStream& bs, const std::unordered_map<u32, u8>& dcMap) {
//     s16 category = decode_dc_category(bs, dcMap);
//     return extend_dc_value(category, bs);
// }


// static u8 decode_ac_symbol(BitStream& bs,
//                          const std::unordered_map<uint32_t,uint8_t>& acMap) {
//     uint16_t code = 0;
//     for (uint8_t len = 1; len <= 16; ++len) {
//         code = (code << 1) | bs.nextBit();
//         uint32_t key = (uint32_t(len)<<16) | code;
//         auto it = acMap.find(key);
//         if (it != acMap.end()) return it->second;
//     }
//     throw std::runtime_error("Invalid AC Huffman code");
// }
