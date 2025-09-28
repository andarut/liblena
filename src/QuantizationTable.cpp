#include "QuantizationTable.h"

int q_n(u8 quality, const ImageCh<u8>& ref_table, QuantizationTable& q_table) {
    if (quality < 1 || quality > 100) {
        return 1;
    }
    u64 S = (quality < 50) ? (5000 / quality) : (200 - 2*quality);

    q_table = ImageCh<u8>(8, 8);
    for (u64 i = 0; i < 8; i++)
        for (u64 j = 0; j < 8; j++)
            q_table(i, j) = std::clamp((S*ref_table(i, j)+50)/100, (u64)1, (u64)255);

    return 0;
}

int lumin_q_n(u8 quality, QuantizationTable& q_table) {
    return q_n(quality, LUMINANCE_Q_50, q_table);
}

int chrom_q_n(u8 quality, QuantizationTable& q_table) {
    return q_n(quality, CHROMINANCE_Q_50, q_table);
}
