#ifndef YCBCR_H
#define YCBCR_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"

/* 
JPEG conversion (ITU-T T.871)
R, G, B   in range [0, 255]
Y, Cb, Cr in range [0, 255]
*/

typedef std::array<ImageChannel<u8>, 3> u8_image;
typedef ImageChannel<u8> u8_ch;

class YCbCrEncoder {
public:
    void encode(u8_image&& rgb_data);
    void decode(u8_image&& ycbcr_data);
private:
    void encode_px(std::array<u8, 3>&& rgb_px);
    void decode_px(std::array<u8, 3>&& ycbcr_px);
};

#endif // YCBCR_H