#include "Quantizator.h"

int Quantizator::quantize(ImageCh<f64>& block, const QuantizationTable& q_table) {
    if (block.width() != q_table.width() || block.height() != q_table.height()) {
        ERROR("dimensions differ");
        return 1;
    }
    for (u64 i = 0; i < block.height(); i++) {
        for (u64 j = 0; j < block.width(); j++) {
            block(i, j) = std::round(block(i, j) / static_cast<f64>(q_table(i, j)));
        }
    }
    return 0;
}

int Quantizator::dequantize(ImageCh<s32>& block, const QuantizationTable& q_table) {
    if (block.width() != q_table.width() || block.height() != q_table.height()) {
        ERROR("dimensions differ");
        return 1;
    }
    for (u64 i = 0; i < block.height(); i++) {
        for (u64 j = 0; j < block.width(); j++) {
            block(i, j) = std::round(static_cast<f32>(block(i, j)) * static_cast<f32>(q_table(i, j)));
        }
    }
    return 0;
}
