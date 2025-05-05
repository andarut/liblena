#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

#include "utils.h"
#include "types.h"
#include "globals.h"

struct HuffEntry {
	u16 code;
	u8  length;
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

/* Table K.5 - Table for luminance AC coefficients */
static constexpr std::array<HuffEntry, 16*16> LUMINANCE_AC = [](){
    std::array<HuffEntry,256> tbl{};

    auto set = [&](int run, int size, const char* bits){
        uint16_t code = 0;
        uint8_t  len  = 0;
        for (const char* p = bits; *p; ++p, ++len) {
            code = (code << 1) | (*p - '0');
        }
        tbl[(run<<4) | size] = { code, len };
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
	/*  17 */ set(0x1, 0x6, "11111111110000100");
	/*  18 */ set(0x1, 0x7, "11111111110000101");
	/*  19 */ set(0x1, 0x8, "11111111110000110");
	/*  20 */ set(0x1, 0x9, "11111111110000111");
	/*  21 */ set(0x1, 0xA, "11111111110001000");
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

	/* (sheet 3 of 4) */

	/* (sheet 4 of 4) */
	
    return tbl;
}();

/* Table K.6 - Table for chrominane AC coefficients */
static constexpr std::array<HuffEntry, 16*16> CHROMINANCE_AC = [](){
    std::array<HuffEntry,256> tbl{};

    auto set = [&](int run, int size, const char* bits){
        uint16_t code = 0;
        uint8_t  len  = 0;
        for (const char* p = bits; *p; ++p, ++len) {
            code = (code << 1) | (*p - '0');
        }
        tbl[(run<<4) | size] = { code, len };
    };

	/* (sheet 1 of 4) */

	/* (sheet 2 of 4) */

	/* (sheet 3 of 4) */

	/* (sheet 4 of 4) */

}();


#endif // HUFFMAN_CODING_H