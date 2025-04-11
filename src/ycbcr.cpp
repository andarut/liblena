#include "ycbcr.h"

RawImageData rgb_to_ycbcr(const RawImageData &rgb_data) {
    assert(rgb_data.size() % 3 == 0);

    RawImageData ycbcr_data(rgb_data.size());

    for (u64 i = 0; i < rgb_data.size(); i += 3) {
        u64 R = rgb_data[i], G = rgb_data[i+1], B = rgb_data[i+2];

        /* 
        JPEG conversion (ITU-T T.871)
        R, G, B   in range [0, 255]
        Y, Cb, Cr in range [0, 255]
        */
        u64 Y = \
            std::clamp(std::round(  0.299 * R +0.587 * G +0.114 * B),             0.0, 255.0);
        u64 Cb = \
            std::clamp(std::round((-0.299 * R -0.587 * G +0.886 * B)/1.772) +128, 0.0, 255.0);
        u64 Cr = \
            std::clamp(std::round(( 0.701 * R -0.587 * G -0.114 * B)/1.402) +128, 0.0, 255.0);

        ycbcr_data[i] = Y;
        ycbcr_data[i+1] = Cb;
        ycbcr_data[i+2] = Cr;
    }

    return ycbcr_data;
}