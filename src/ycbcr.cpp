#include "ycbcr.h"

/* 
JPEG conversion (ITU-T T.871)
R, G, B   in range [0, 255]
Y, Cb, Cr in range [0, 255]
*/
RawImageData rgb_to_ycbcr(const RawImageData &rgb_data) {
    if (rgb_data.empty()) return rgb_data;
    assert(rgb_data.numberOfChannels == 3);

    RawImageData ycbcr_data(rgb_data.width, rgb_data.height, rgb_data.numberOfChannels);

    for (u64 i = 0; i < rgb_data.height; i++) {
        for (u64 j = 0; j < rgb_data.width; j++) {
            u64
                R = rgb_data[0](i, j),
                G = rgb_data[1](i, j),
                B = rgb_data[2](i, j);
            u64 Y = \
                std::clamp(std::round(  0.299 * R +0.587 * G +0.114 * B),             0.0, 255.0);
            u64 Cb = \
                std::clamp(std::round((-0.299 * R -0.587 * G +0.886 * B)/1.772) +128, 0.0, 255.0);
            u64 Cr = \
                std::clamp(std::round(( 0.701 * R -0.587 * G -0.114 * B)/1.402) +128, 0.0, 255.0);
        
            ycbcr_data[0](i, j) = Y;
            ycbcr_data[1](i, j) = Cb;
            ycbcr_data[2](i, j) = Cr;
        }
    }
    
    return ycbcr_data;
}