#ifndef DCT_H
#define DCT_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"

class DCT {
public:
    static int FDCT(const ImageCh<u8>& ch, ImageCh<f64>& dctCh);
    static int IDCT(ImageCh<u8>& ch);
};

#endif // DCT_H
