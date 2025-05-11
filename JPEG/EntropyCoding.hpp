#ifndef ENTROPYCODING_H
#define ENTROPYCODING_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"

template<typename T>
std::vector<T> zigzag(const ImageChannel<T> data) {
    std::vector<T> zigzag(data.size());

    u64 read_i = 0, read_j = 0;
    for (u64 i = 0; i < zigzag.size(); i++) {

        zigzag[i] = data(read_i, read_j);

        if ((read_i+read_j) % 2 == 0) { 
            if (read_i > 0) read_i--; 
            if (read_j < data.width()-1) read_j++; 
        } else { 
            if (read_i < data.height()-1) read_i++; 
            if (read_j > 0) read_j--; 
        }
    }

    return zigzag;
}

/* NOTE: this is jpeg's RLE (rely on many zeros in data and omitting DC) */
template<typename T>
std::vector<T> RLE(const std::vector<T> data) {
    std::vector<T> RLE_data;
    RLE_data.reserve(data.size());

    u64 count = 0;
    for (u64 i = 1; i < data.size(); i++) {
        if (data[i] == 0) {
			count++;
			/* ZRL */
			if (count == 16) {
				RLE_data.push_back(0xF);
				RLE_data.push_back(0);
				count = 0;
			}
		}
        else {
            RLE_data.push_back(count);
            RLE_data.push_back(log2(std::abs(data[i]))+1);
            RLE_data.push_back(data[i]);
            count = 0;
        }
    }

    // EOB
	if (count != 0) {
		RLE_data.push_back(0);
		RLE_data.push_back(0);
	}

    return RLE_data;
}

struct HuffEntry {
	u8  length;
	u16 code;
};

// Table K.3 - Table for luminance DC coefficient differences
static constexpr std::array<HuffEntry,12> LUMINANCE_DC = {{
	/*  0 */ { 2, 0b00 },
	/*  1 */ { 3, 0b010 },
	/*  2 */ { 3, 0b011 },
	/*  3 */ { 3, 0b100 },
	/*  4 */ { 3, 0b101 },
	/*  5 */ { 3, 0b110 },
	/*  6 */ { 4, 0b1110 },
	/*  7 */ { 5, 0b11110 },
	/*  8 */ { 6, 0b111110 },
	/*  9 */ { 7, 0b1111110 },
	/* 10 */ { 8, 0b11111110 },
	/* 11 */ { 9, 0b111111110 },
}};

std::unordered_map<u32, u8> LUMINANCE_DC_MAP() {
    std::unordered_map<u32, u8> dcMap;
    for (uint8_t symbol = 0; symbol < LUMINANCE_DC.size(); ++symbol) {
        auto entry = LUMINANCE_DC[symbol];
        u32 key = (static_cast<u32>(entry.length) << 16) | entry.code;
        dcMap[key] = symbol;
    }
    return dcMap;
}

// Table K.4 - Table for chrominance DC coefficient differences
static constexpr std::array<HuffEntry,12> CHROMINANCE_DC = {{
	/*  0 */ {  2, 0b00 },
	/*  1 */ {  2, 0b01 },
	/*  2 */ {  2, 0b10 },
	/*  3 */ {  3, 0b110 },
	/*  4 */ {  4, 0b1110 },
	/*  5 */ {  5, 0b11110 },
	/*  6 */ {  6, 0b111110 },
	/*  7 */ {  7, 0b1111110 },
	/*  8 */ {  8, 0b11111110 },
	/*  9 */ {  9, 0b111111110 },
	/* 10 */ { 10, 0b1111111110 },
	/* 11 */ { 11, 0b11111111110 },
}};

u16 decode_dc_category(BitStream& bs, const std::unordered_map<u32, u8>& dcMap) {
    u16 code = 0;
    for (uint8_t length = 1; length <= 16; ++length) {
        code = (code << 1) | bs.nextBit();
        uint32_t key = (static_cast<uint32_t>(length) << 16) | code;
        auto it = dcMap.find(key);
        if (it != dcMap.end()) {
            return it->second;  // category 0–11
        }
    }
    throw std::runtime_error("Invalid DC Huffman code");
}

s16 extend_dc_value(uint16_t S, BitStream& bs) {
    if (S == 0) return 0;
    s16 bits = 0;
    for (u16 i = 0; i < S; ++i) {
        bits = (bits << 1) | bs.nextBit();
    }
    // JPEG “extend”: if the leading bit is 0, subtract (2^S - 1)
    s16 threshold = 1 << (S - 1);
    if (bits < threshold)
        bits -= (1 << S) - 1;
    return bits;
}

s16 decode_dc(BitStream& bs, const std::unordered_map<u32, u8>& dcMap) {
    s16 category = decode_dc_category(bs, dcMap);
    return extend_dc_value(category, bs);
}

/* Table K.5 - Table for luminance AC coefficients */
static constexpr std::array<HuffEntry, 16*16> LUMINANCE_AC = [](){
	std::array<HuffEntry,256> tbl{};

	auto set = [&](int run, int size, const char* bits){
		uint16_t code = 0;
		uint8_t  len  = 0;
		for (const char* p = bits; *p; ++p, ++len) {
			code = (code << 1) | (*p - '0');
		}
		// assert(len == sizeof(bits));
		tbl[(run<<4) | size] = { len, code };
	};

	/* (sheet 1 of 4 */
	/*   1 */ set(0x0, 0x0, "1010");
	/*   2 */ set(0x0, 0x1, "00");
	/*   3 */ set(0x0, 0x2, "01");
	/*   4 */ set(0x0, 0x3, "100");
	/*   5 */ set(0x0, 0x4, "1011");
	/*   6 */ set(0x0, 0x5, "11010");
	/*   7 */ set(0x0, 0x6, "1111000");
	/*   8 */ set(0x0, 0x7, "11111000");
	/*   9 */ set(0x0, 0x8, "1111110110");
	/*  10 */ set(0x0, 0x9, "1111111110000010");
	/*  11 */ set(0x0, 0xA, "1111111110000011");
	/*  12 */ set(0x1, 0x1, "1100");
	/*  13 */ set(0x1, 0x2, "11011");
	/*  14 */ set(0x1, 0x3, "1111001");
	/*  15 */ set(0x1, 0x4, "111110110");
	/*  16 */ set(0x1, 0x5, "11111110110");
	/*  17 */ set(0x1, 0x6, "1111111110000100");
	/*  18 */ set(0x1, 0x7, "1111111110000101");
	/*  19 */ set(0x1, 0x8, "1111111110000110");
	/*  20 */ set(0x1, 0x9, "1111111110000111");
	/*  21 */ set(0x1, 0xA, "1111111110001000");
	/*  22 */ set(0x2, 0x1, "11100");
	/*  23 */ set(0x2, 0x2, "11111001");
	/*  24 */ set(0x2, 0x3, "1111110111");
	/*  25 */ set(0x2, 0x4, "111111110100");
	/*  26 */ set(0x2, 0x5, "1111111110001001");
	/*  27 */ set(0x2, 0x6, "1111111110001010");
	/*  28 */ set(0x2, 0x7, "1111111110001011");
	/*  29 */ set(0x2, 0x8, "1111111110001100");
	/*  30 */ set(0x2, 0x9, "1111111110001101");
	/*  31 */ set(0x2, 0xA, "1111111110001110");
	/*  32 */ set(0x3, 0x1, "111010");
	/*  33 */ set(0x3, 0x2, "111110111");
	/*  34 */ set(0x3, 0x3, "111111110101");
	/*  35 */ set(0x3, 0x4, "1111111110001111");
	/*  36 */ set(0x3, 0x5, "1111111110010000");
	/*  37 */ set(0x3, 0x6, "1111111110010001");
	/*  38 */ set(0x3, 0x7, "1111111110010010");
	/*  39 */ set(0x3, 0x8, "1111111110010011");
	/*  40 */ set(0x3, 0x9, "1111111110010100");
	/*  41 */ set(0x3, 0xA, "1111111110010101");

	/* (sheet 2 of 4) */
	/*  42 */ set(0x4, 0x1, "111011");
	/*  43 */ set(0x4, 0x2, "1111111000");
	/*  44 */ set(0x4, 0x3, "1111111110010110");
	/*  45 */ set(0x4, 0x4, "1111111110010111");
	/*  46 */ set(0x4, 0x5, "1111111110011000");
	/*  47 */ set(0x4, 0x6, "1111111110011001");
	/*  48 */ set(0x4, 0x7, "1111111110011010");
	/*  49 */ set(0x4, 0x8, "1111111110011011");
	/*  40 */ set(0x4, 0x9, "1111111110011100");
	/*  51 */ set(0x4, 0xA, "1111111110011101");
	/*  52 */ set(0x5, 0x1, "1111010");
	/*  53 */ set(0x5, 0x2, "11111110111");
	/*  54 */ set(0x5, 0x3, "1111111110011110");
	/*  55 */ set(0x5, 0x4, "1111111110011111");
	/*  56 */ set(0x5, 0x5, "1111111110100000");
	/*  57 */ set(0x5, 0x6, "1111111110100001");
	/*  58 */ set(0x5, 0x7, "1111111110100010");
	/*  59 */ set(0x5, 0x8, "1111111110100011");
	/*  50 */ set(0x5, 0x9, "1111111110100100");
	/*  61 */ set(0x5, 0xA, "1111111110100101");
	/*  62 */ set(0x6, 0x1, "1111011");
	/*  63 */ set(0x6, 0x2, "111111110110");
	/*  64 */ set(0x6, 0x3, "1111111110100110");
	/*  65 */ set(0x6, 0x4, "1111111110100111");
	/*  66 */ set(0x6, 0x5, "1111111110101000");
	/*  67 */ set(0x6, 0x6, "1111111110101001");
	/*  68 */ set(0x6, 0x7, "1111111110101010");
	/*  69 */ set(0x6, 0x8, "1111111110101011");
	/*  60 */ set(0x6, 0x9, "1111111110101100");
	/*  71 */ set(0x6, 0xA, "1111111110101101");
	/*  72 */ set(0x7, 0x1, "11111010");
	/*  73 */ set(0x7, 0x2, "111111110111");
	/*  74 */ set(0x7, 0x3, "1111111110101110");
	/*  75 */ set(0x7, 0x4, "1111111110101111");
	/*  76 */ set(0x7, 0x5, "1111111110110000");
	/*  77 */ set(0x7, 0x6, "1111111110110001");
	/*  78 */ set(0x7, 0x7, "1111111110110010");
	/*  79 */ set(0x7, 0x8, "1111111110110011");
	/*  70 */ set(0x7, 0x9, "1111111110110100");
	/*  81 */ set(0x7, 0xA, "1111111110110101");
	/*  82 */ set(0x8, 0x1, "111111000");
	/*  83 */ set(0x8, 0x2, "111111111000000");

	/* (sheet 3 of 4) */
	/*  84 */ set(0x8, 0x3, "1111111110110110");
	/*  85 */ set(0x8, 0x4, "1111111110110111");
	/*  86 */ set(0x8, 0x5, "1111111110111000");
	/*  87 */ set(0x8, 0x6, "1111111110111001");
	/*  88 */ set(0x8, 0x7, "1111111110111010");
	/*  89 */ set(0x8, 0x8, "1111111110111011");
	/*  90 */ set(0x8, 0x9, "1111111110111100");
	/*  91 */ set(0x8, 0xA, "1111111110111101");
	/*  92 */ set(0x9, 0x1, "111111001");
	/*  93 */ set(0x9, 0x2, "1111111110111110");
	/*  94 */ set(0x9, 0x3, "1111111110111111");
	/*  95 */ set(0x9, 0x4, "1111111111000000");
	/*  96 */ set(0x9, 0x5, "1111111111000001");
	/*  97 */ set(0x9, 0x6, "1111111111000010");
	/*  98 */ set(0x9, 0x7, "1111111111000011");
	/*  99 */ set(0x9, 0x8, "1111111111000100");
	/* 100 */ set(0x9, 0x9, "1111111111000101");
	/* 101 */ set(0x9, 0xA, "1111111111000110");
	/* 102 */ set(0xA, 0x1, "111111010");
	/* 103 */ set(0xA, 0x2, "1111111111000111");
	/* 104 */ set(0xA, 0x3, "1111111111001000");
	/* 105 */ set(0xA, 0x4, "1111111111001001");
	/* 106 */ set(0xA, 0x5, "1111111111001010");
	/* 107 */ set(0xA, 0x6, "1111111111001011");
	/* 108 */ set(0xA, 0x7, "1111111111001100");
	/* 109 */ set(0xA, 0x8, "1111111111001101");
	/* 110 */ set(0xA, 0x9, "1111111111001110");
	/* 111 */ set(0xA, 0xA, "1111111111001111");
	/* 112 */ set(0xB, 0x1, "1111111001");
	/* 113 */ set(0xB, 0x2, "1111111111010000");
	/* 114 */ set(0xB, 0x3, "1111111111010001");
	/* 115 */ set(0xB, 0x4, "1111111111010010");
	/* 116 */ set(0xB, 0x5, "1111111111010011");
	/* 117 */ set(0xB, 0x6, "1111111111010100");
	/* 118 */ set(0xB, 0x7, "1111111111010101");
	/* 119 */ set(0xB, 0x8, "1111111111010110");
	/* 120 */ set(0xB, 0x9, "1111111111010111");
	/* 121 */ set(0xB, 0xA, "1111111111011000");
	/* 122 */ set(0xC, 0x1, "1111111010");
	/* 123 */ set(0xC, 0x2, "1111111111011001");
	/* 124 */ set(0xC, 0x3, "1111111111011010");
	/* 125 */ set(0xC, 0x4, "1111111111011011");

	/* (sheet 4 of 4) */
	/* 126 */ set(0xC, 0x5, "1111111111011100");
	/* 127 */ set(0xC, 0x6, "1111111111011101");
	/* 128 */ set(0xC, 0x7, "1111111111011110");
	/* 129 */ set(0xC, 0x8, "1111111111011111");
	/* 130 */ set(0xC, 0x9, "1111111111100000");
	/* 131 */ set(0xC, 0xA, "1111111111100001");
	/* 132 */ set(0xD, 0x1, "11111111000");
	/* 133 */ set(0xD, 0x2, "1111111111100010");
	/* 134 */ set(0xD, 0x3, "1111111111100011");
	/* 135 */ set(0xD, 0x4, "1111111111100100");
	/* 136 */ set(0xD, 0x5, "1111111111100101");
	/* 137 */ set(0xD, 0x6, "1111111111100110");
	/* 138 */ set(0xD, 0x7, "1111111111100111");
	/* 139 */ set(0xD, 0x8, "1111111111101000");
	/* 140 */ set(0xD, 0x9, "1111111111101001");
	/* 141 */ set(0xD, 0xA, "1111111111101010");
	/* 142 */ set(0xE, 0x1, "1111111111101011");
	/* 143 */ set(0xE, 0x2, "1111111111101100");
	/* 144 */ set(0xE, 0x3, "1111111111101101");
	/* 145 */ set(0xE, 0x4, "1111111111101110");
	/* 146 */ set(0xE, 0x5, "1111111111101111");
	/* 147 */ set(0xE, 0x6, "1111111111110000");
	/* 148 */ set(0xE, 0x7, "1111111111110001");
	/* 149 */ set(0xE, 0x8, "1111111111110010");
	/* 150 */ set(0xE, 0x9, "1111111111110011");
	/* 151 */ set(0xE, 0xA, "1111111111110100");
	/* 152 */ set(0xF, 0x0, "11111111001");
	/* 153 */ set(0xF, 0x1, "1111111111110101");
	/* 154 */ set(0xF, 0x2, "1111111111110110");
	/* 155 */ set(0xF, 0x3, "1111111111110111");
	/* 156 */ set(0xF, 0x4, "1111111111111000");
	/* 157 */ set(0xF, 0x5, "1111111111111001");
	/* 158 */ set(0xF, 0x6, "1111111111111010");
	/* 159 */ set(0xF, 0x7, "1111111111111011");
	/* 160 */ set(0xF, 0x8, "1111111111111100");
	/* 161 */ set(0xF, 0x9, "1111111111111101");
	/* 162 */ set(0xF, 0xA, "1111111111111110");
	
	return tbl;
}();

std::unordered_map<u32, u8> LUMINANCE_AC_MAP(){
    std::unordered_map<u32, u8> map{};
    for (uint16_t idx = 0; idx < LUMINANCE_AC.size(); ++idx) {
        auto e = LUMINANCE_AC[idx];
        if (e.length == 0) continue;
        uint32_t key = (uint32_t(e.length) << 16) | e.code;
        map[key] = idx; // idx encodes run<<4 | size
    }
    return map;
}

/* Table K.6 - Table for chrominane AC coefficients */
static constexpr std::array<HuffEntry, 16*16> CHROMINANCE_AC = [](){
	std::array<HuffEntry,256> tbl{};

	auto set = [&](int run, int size, const char* bits){
		uint16_t code = 0;
		uint8_t  len  = 0;
		for (const char* p = bits; *p; ++p, ++len) {
			code = (code << 1) | (*p - '0');
		}
		// assert(len == sizeof(bits));
		tbl[(run<<4) | size] = { len, code };
	};

	/* (sheet 1 of 4) */
	/*   1 */ set(0x0, 0x0, "00");
	/*   2 */ set(0x0, 0x1, "01");
	/*   3 */ set(0x0, 0x2, "100");
	/*   4 */ set(0x0, 0x3, "1010");
	/*   5 */ set(0x0, 0x4, "11000");
	/*   6 */ set(0x0, 0x5, "11001");
	/*   7 */ set(0x0, 0x6, "111000");
	/*   8 */ set(0x0, 0x7, "1111000");
	/*   9 */ set(0x0, 0x8, "111110100");
	/*  10 */ set(0x0, 0x9, "1111110110");
	/*  11 */ set(0x0, 0xA, "111111110100");
	/*  12 */ set(0x1, 0x1, "1011");
	/*  13 */ set(0x1, 0x2, "111001");
	/*  14 */ set(0x1, 0x3, "11110110");
	/*  15 */ set(0x1, 0x4, "111110101");
	/*  16 */ set(0x1, 0x5, "11111110110");
	/*  17 */ set(0x1, 0x6, "111111110101");
	/*  18 */ set(0x1, 0x7, "1111111110001000");
	/*  19 */ set(0x1, 0x8, "1111111110001001");
	/*  20 */ set(0x1, 0x9, "1111111110001010");
	/*  21 */ set(0x1, 0xA, "1111111110001011");
	/*  22 */ set(0x2, 0x1, "11010");
	/*  23 */ set(0x2, 0x2, "11110111");
	/*  24 */ set(0x2, 0x3, "1111110111");
	/*  25 */ set(0x2, 0x4, "111111110110");
	/*  26 */ set(0x2, 0x5, "111111111000010");
	/*  27 */ set(0x2, 0x6, "1111111110001100");
	/*  28 */ set(0x2, 0x7, "1111111110001101");
	/*  29 */ set(0x2, 0x8, "1111111110001110");
	/*  30 */ set(0x2, 0x9, "1111111110001111");
	/*  31 */ set(0x2, 0xA, "1111111110010000");
	/*  32 */ set(0x3, 0x1, "11011");
	/*  33 */ set(0x3, 0x2, "11111000");
	/*  34 */ set(0x3, 0x3, "1111111000");
	/*  35 */ set(0x3, 0x4, "111111110111");
	/*  36 */ set(0x3, 0x5, "1111111110010001");
	/*  37 */ set(0x3, 0x6, "1111111110010010");
	/*  38 */ set(0x3, 0x7, "1111111110010011");
	/*  39 */ set(0x3, 0x8, "1111111110010100");
	/*  40 */ set(0x3, 0x9, "1111111110010101");
	/*  41 */ set(0x3, 0xA, "1111111110010110");
	/*  42 */ set(0x4, 0x1, "111010");

	/* (sheet 2 of 4) */
	/*  43 */ set(0x4, 0x2, "111110110");
	/*  44 */ set(0x4, 0x3, "1111111110010111");
	/*  45 */ set(0x4, 0x4, "1111111110011000");
	/*  46 */ set(0x4, 0x5, "1111111110011001");
	/*  47 */ set(0x4, 0x6, "1111111110011010");
	/*  48 */ set(0x4, 0x7, "1111111110011011");
	/*  49 */ set(0x4, 0x8, "1111111110011100");
	/*  50 */ set(0x4, 0x9, "1111111110011101");
	/*  51 */ set(0x4, 0xA, "1111111110011110");
	/*  52 */ set(0x5, 0x1, "111011");
	/*  53 */ set(0x5, 0x2, "1111111001");
	/*  54 */ set(0x5, 0x3, "1111111110011111");
	/*  55 */ set(0x5, 0x4, "1111111110100000");
	/*  56 */ set(0x5, 0x5, "1111111110100001");
	/*  57 */ set(0x5, 0x6, "1111111110100010");
	/*  58 */ set(0x5, 0x7, "1111111110100011");
	/*  59 */ set(0x5, 0x8, "1111111110100100");
	/*  60 */ set(0x5, 0x9, "1111111110100101");
	/*  61 */ set(0x5, 0xA, "1111111110100110");
	/*  62 */ set(0x6, 0x1, "1111001");
	/*  63 */ set(0x6, 0x2, "11111110111");
	/*  64 */ set(0x6, 0x3, "1111111110100111");
	/*  65 */ set(0x6, 0x4, "1111111110101000");
	/*  66 */ set(0x6, 0x5, "1111111110101001");
	/*  67 */ set(0x6, 0x6, "1111111110101010");
	/*  68 */ set(0x6, 0x7, "1111111110101011");
	/*  69 */ set(0x6, 0x8, "1111111110101100");
	/*  70 */ set(0x6, 0x9, "1111111110101101");
	/*  71 */ set(0x6, 0xA, "1111111110101110");
	/*  72 */ set(0x7, 0x1, "1111010");
	/*  73 */ set(0x7, 0x2, "11111111000");
	/*  74 */ set(0x7, 0x3, "1111111110101111");
	/*  75 */ set(0x7, 0x4, "1111111110110000");
	/*  76 */ set(0x7, 0x5, "1111111110110001");
	/*  77 */ set(0x7, 0x6, "1111111110110010");
	/*  78 */ set(0x7, 0x7, "1111111110110011");
	/*  79 */ set(0x7, 0x8, "1111111110110100");
	/*  80 */ set(0x7, 0x9, "1111111110110101");
	/*  81 */ set(0x7, 0xA, "1111111110110110");
	/*  82 */ set(0x8, 0x1, "11111001");
	/*  83 */ set(0x8, 0x2, "1111111110110111");
	/*  84 */ set(0x8, 0x3, "1111111110111000");

	/* (sheet 3 of 4) */
	/*  85 */ set(0x8, 0x4, "1111111110111001");
	/*  86 */ set(0x8, 0x5, "1111111110111010");
	/*  87 */ set(0x8, 0x6, "1111111110111011");
	/*  88 */ set(0x8, 0x7, "1111111110111100");
	/*  89 */ set(0x8, 0x8, "1111111110111101");
	/*  90 */ set(0x8, 0x9, "1111111110111110");
	/*  91 */ set(0x8, 0xA, "1111111110111111");
	/*  92 */ set(0x9, 0x1, "111110111");
	/*  93 */ set(0x9, 0x2, "1111111111000000");
	/*  94 */ set(0x9, 0x3, "1111111111000001");
	/*  95 */ set(0x9, 0x4, "1111111111000010");
	/*  96 */ set(0x9, 0x5, "1111111111000011");
	/*  97 */ set(0x9, 0x6, "1111111111000100");
	/*  98 */ set(0x9, 0x7, "1111111111000101");
	/*  99 */ set(0x9, 0x8, "1111111111000110");
	/* 100 */ set(0x9, 0x9, "1111111111000111");
	/* 101 */ set(0x9, 0xA, "1111111111001000");
	/* 102 */ set(0xA, 0x1, "111111000");
	/* 103 */ set(0xA, 0x2, "1111111111001001");
	/* 104 */ set(0xA, 0x3, "1111111111001010");
	/* 105 */ set(0xA, 0x4, "1111111111001011");
	/* 106 */ set(0xA, 0x5, "1111111111001100");
	/* 107 */ set(0xA, 0x6, "1111111111001101");
	/* 108 */ set(0xA, 0x7, "1111111111001110");
	/* 109 */ set(0xA, 0x8, "1111111111001111");
	/* 110 */ set(0xA, 0x9, "1111111111010000");
	/* 111 */ set(0xA, 0xA, "1111111111010001");
	/* 112 */ set(0xB, 0x1, "111111001");
	/* 113 */ set(0xB, 0x2, "1111111111010010");
	/* 114 */ set(0xB, 0x3, "1111111111010011");
	/* 115 */ set(0xB, 0x4, "1111111111010100");
	/* 116 */ set(0xB, 0x5, "1111111111010101");
	/* 117 */ set(0xB, 0x6, "1111111111010110");
	/* 118 */ set(0xB, 0x7, "1111111111010111");
	/* 119 */ set(0xB, 0x8, "1111111111011000");
	/* 120 */ set(0xB, 0x9, "1111111111011001");
	/* 121 */ set(0xB, 0xA, "1111111111011010");
	/* 122 */ set(0xC, 0x1, "111111010");
	/* 123 */ set(0xC, 0x2, "1111111111011011");
	/* 124 */ set(0xC, 0x3, "1111111111011100");
	/* 125 */ set(0xC, 0x4, "1111111111011101");
	/* 126 */ set(0xC, 0x5, "1111111111011110");

	/* (sheet 4 of 4) */
	/* 127 */ set(0xC, 0x6, "1111111111011111");
	/* 128 */ set(0xC, 0x7, "1111111111100000");
	/* 129 */ set(0xC, 0x8, "1111111111100001");
	/* 130 */ set(0xC, 0x9, "1111111111100010");
	/* 131 */ set(0xC, 0xA, "1111111111100011");
	/* 132 */ set(0xD, 0x1, "11111111001");
	/* 133 */ set(0xD, 0x2, "1111111111100100");
	/* 134 */ set(0xD, 0x3, "1111111111100101");
	/* 135 */ set(0xD, 0x4, "1111111111100110");
	/* 136 */ set(0xD, 0x5, "1111111111100111");
	/* 137 */ set(0xD, 0x6, "1111111111101000");
	/* 138 */ set(0xD, 0x7, "1111111111101001");
	/* 139 */ set(0xD, 0x8, "1111111111101010");
	/* 140 */ set(0xD, 0x9, "1111111111101011");
	/* 141 */ set(0xE, 0xA, "1111111111101100");
	/* 142 */ set(0xE, 0x1, "11111111100000");
	/* 143 */ set(0xE, 0x2, "1111111111101101");
	/* 144 */ set(0xE, 0x3, "1111111111101110");
	/* 145 */ set(0xE, 0x4, "1111111111101111");
	/* 146 */ set(0xE, 0x5, "1111111111110000");
	/* 147 */ set(0xE, 0x6, "1111111111110001");
	/* 148 */ set(0xE, 0x7, "1111111111110010");
	/* 149 */ set(0xE, 0x8, "1111111111110011");
	/* 150 */ set(0xE, 0x9, "1111111111110100");
	/* 151 */ set(0xE, 0xA, "1111111111110101");
	/* 152 */ set(0xF, 0x0, "1111111010"); // ZRL
	/* 153 */ set(0xF, 0x1, "111111111000011");
	/* 154 */ set(0xF, 0x2, "1111111111110110");
	/* 155 */ set(0xF, 0x3, "1111111111110111");
	/* 156 */ set(0xF, 0x4, "1111111111111000");
	/* 157 */ set(0xF, 0x5, "1111111111111001");
	/* 158 */ set(0xF, 0x6, "1111111111111010");
	/* 159 */ set(0xF, 0x7, "1111111111111011");
	/* 160 */ set(0xF, 0x8, "1111111111111100");
	/* 161 */ set(0xF, 0x9, "1111111111111101");
	/* 162 */ set(0xF, 0xA, "1111111111111110");

	return tbl;
}();

u8 decode_ac_symbol(BitStream& bs,
                         const std::unordered_map<uint32_t,uint8_t>& acMap) {
    uint16_t code = 0;
    for (uint8_t len = 1; len <= 16; ++len) {
        code = (code << 1) | bs.nextBit();
        uint32_t key = (uint32_t(len)<<16) | code;
        auto it = acMap.find(key);
        if (it != acMap.end()) return it->second;
    }
    throw std::runtime_error("Invalid AC Huffman code");
}


void writeVLI(BitStream &bs, s32 amplitude, u16 size) {
	u32 abs_amplitude = std::abs(amplitude);

	for (int i = int(size) - 1; i >= 0; --i) {
		uint32_t bit = (abs_amplitude >> i) & 1u;
		if (amplitude < 0) bit ^= 1u;
		bs.write_bits(bit, 1);
	}
}

u8 categoryDC(s32 diff) {
	u32 abs_amplitude = std::abs(diff);
	u8 cat = 0;
	while (abs_amplitude >>= 1) ++cat;
	return cat;
}

void writeLuminanceDC(BitStream &bs, s32 diff) {
	INFO("diff = %llu\n", diff);
	u8 cat = categoryDC(diff);
	assert(cat >= 0 && cat < 12);
	auto e = LUMINANCE_DC[cat];
	INFO("cat = %llu, code = %llu, length = %d\n", cat, e.code, e.length);
	bs.write_bits(e.code, e.length);

	if (cat > 0)
		writeVLI(bs, diff, cat);
}

void writeChrominanceDC(BitStream &bs, s32 diff) {
	u8 cat = categoryDC(diff);
	assert(cat >= 0 && cat < 12);

	auto entry = CHROMINANCE_DC[cat];
	bs.write_bits(entry.code, entry.length);

	if (cat > 0)
		writeVLI(bs, diff, cat);
}

void writeLuminanceAC(BitStream &bs, u32 run, u16 size, s32 amplitude) {
	INFO("run = %llu, size = %llu, ampl = %d\n", run, size, amplitude);
	auto &e = LUMINANCE_AC[(run<<4)|size];
	INFO("code = %llu, length = %llu\n", e.code, e.length);
	bs.write_bits(e.code, e.length);
	writeVLI(bs, amplitude, size);
}

void writeChrominanceAC(BitStream &bs, u32 run, u16 size, s32 amplitude) {
	auto &e = CHROMINANCE_AC[(run<<4)|size];
	bs.write_bits(e.code, e.length);
	writeVLI(bs, amplitude, size);
}

namespace enc {

BitStream entropy_coding(std::vector<ImageChannel<s16>>  Y_MCUs, \
                         std::vector<ImageChannel<s16>> Cb_MCUs, \
						 std::vector<ImageChannel<s16>> Cr_MCUs) {
    BitStream bs;

    auto first_DC = Y_MCUs[0](0, 0);
	// writeLuminanceDC(bs, first_DC);
    for (auto& Y_MCU : Y_MCUs) {
        writeLuminanceDC(bs, Y_MCU(0, 0) - first_DC);
        auto zigzag_order = zigzag(Y_MCU);
        auto RLE_data = RLE(zigzag_order);
		for (u64 i = 0; i < RLE_data.size(); i++)
			printf("%d ", RLE_data[i]);
		printf("\n");
        auto EOB = (RLE_data.size() % 3 == 0) ? false : true;
        auto size = RLE_data.size() - (EOB ? 2 : 0);
        for (u64 i = 0; i < size; i += 3) {
            auto run =  RLE_data[i];
            auto size = RLE_data[i+1];
            auto ampl = RLE_data[i+2];
            writeLuminanceAC(bs, run, size, ampl);
        }
        if (EOB) {
            bs.write_bits(0b1010, 4);
        }
    } 

    return bs;
}

} // namespace enc

namespace dec {

template<typename T>
ImageChannel<T> zigzag(const std::vector<T> zigzag, u64 w) {
	assert(zigzag.size() % w == 0);
    ImageChannel<T> data(w, zigzag.size()/w);
	data.resize(w, zigzag.size()/w);

    u64 write_i = 0, write_j = 0;
    for (u64 i = 0; i < zigzag.size(); i++) {

        data(write_i, write_j) = zigzag[i];

        if ((write_i+write_j) % 2 == 0) { 
            if (write_i > 0) write_i--; 
            if (write_j < data.width()-1) write_j++; 
        } else { 
            if (write_i < data.height()-1) write_i++; 
            if (write_j > 0) write_j--; 
        }
    }

    return data;
}

/* NOTE: this is jpeg's RLE (rely on many zeros in data and omitting DC) */
template<typename T>
std::vector<T> RLE(s16 DC, const std::vector<T> RLE_data) {
    std::vector<T> data(64);
	data[0] = DC;
	
	u64 write_i = 1;

	for (u64 i = 0; i < RLE_data.size(); i += 3) {
		u8 run  = RLE_data[i];
		u8 size = RLE_data[i+1];
		if (size == 0 && run == 0) break;
		if (size == 0 && run == 0xF) break;
		s16 ampl = RLE_data[i+2];
		INFO("run = %d size = %d, ampl = %d\n", run, size, ampl);
		write_i += run;
		data[write_i] = ampl;
		write_i++;
	}
    return data;
}

std::array<std::vector<ImageChannel<s16>>, 3> entropy_coding(BitStream& bs) {
	print(bs);

	std::array<std::vector<ImageChannel<s16>>, 3> v;
	s16 DC = decode_dc(bs, LUMINANCE_DC_MAP());
	INFO("DC = %d\n", DC);

	std::vector<s16> RLE_data;
	while(1) {
		s16 AC = decode_ac_symbol(bs, LUMINANCE_AC_MAP());
		uint8_t run  = AC >> 4;
		uint8_t size = AC & 0x0F;
		INFO("run = %d size = %d\n", run, size);
		RLE_data.push_back(run);
		RLE_data.push_back(size);
		if (size == 0 && run == 0) break;
		s16 ampl = extend_dc_value(size, bs);
		INFO("ampl = %d\n", ampl);
		RLE_data.push_back(ampl);
	}

	std::vector<s16> RLE_decoded = dec::RLE(DC, RLE_data);

	ImageChannel<s16> zigzag_decoded = dec::zigzag(RLE_decoded, 8);
	v[0].resize(1);
	v[0][0] = zigzag_decoded;
	return v;
}

} // namespace dec

#endif // ENTROPYCODING_H