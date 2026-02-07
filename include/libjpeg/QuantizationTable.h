#ifndef QUANTIZATION_TABLE_H
#define QUANTIZATION_TABLE_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"

typedef ImageCh<u8> QuantizationTable;

/* Table K.1 */
inline auto LUMINANCE_Q_50 = QuantizationTable(8, 8, {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
});

/* Table K.2 */
inline auto CHROMINANCE_Q_50 = QuantizationTable(8, 8, {
    17,	18,	24,	47,	99,	99,	99,	99,
    18,	21,	26,	66,	99,	99,	99,	99,
    24,	26,	56,	99,	99,	99,	99,	99,
    47,	66,	99,	99,	99,	99,	99,	99,
    99,	99,	99,	99,	99,	99,	99,	99,
    99,	99,	99,	99,	99,	99,	99,	99,
    99,	99,	99,	99,	99,	99,	99,	99,
    99,	99,	99,	99,	99,	99,	99,	99,
});

int lumin_q_n(u8 quality, QuantizationTable& q_table);
int chrom_q_n(u8 quality, QuantizationTable& q_table);

#endif // QUANTIZATION_TABLE_H
