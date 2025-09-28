#ifndef DCT_H
#define DCT_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"

class DCT {
public:
    static int FDCT(ImageCh<s32>& ch);
    static int IDCT(ImageCh<u8>& ch);
};

#endif // DCT_H