#ifndef YCBCR_H
#define YCBCR_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"

#include "Image.h"


/*
JPEG conversion (ITU-T T.871)
R, G, B   in range [0, 255]
Y, Cb, Cr in range [0, 255]
*/

// typedef Im

/*
    Converts RGB <-> YCbCr inplace
*/
class YCbCrConverter {
public:
    static void fromRGB(Image<u8>& rgbData);
    static void toRGB(Image<u8>& ycbcrData);
};

#endif // YCBCR_H