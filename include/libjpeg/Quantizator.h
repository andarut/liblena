#ifndef QUANTIZATOR_H
#define QUANTIZATOR_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "QuantizationTable.h"

class Quantizator {
public:
    static int quantize(ImageCh<s32>& block, const QuantizationTable& q_table);
    static int dequantize(ImageCh<s32>& block, const QuantizationTable& q_table);
};

#endif // QUANTIZATOR_H