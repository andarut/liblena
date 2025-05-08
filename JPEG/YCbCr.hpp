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
std::vector<ImageChannel<u8>> RGB2YCbCr(const std::vector<ImageChannel<u8>> &RGB_data) {
    assert(!RGB_data.empty());
    assert(RGB_data.size() == 3);

    g_timers.start("RGB2YCbCr");

    std::vector<ImageChannel<u8>> YCbCr_data(3);

    const ImageChannel<u8> *R_ch = &RGB_data[0];
    const ImageChannel<u8> *G_ch = &RGB_data[1];
    const ImageChannel<u8> *B_ch = &RGB_data[2];

    ImageChannel<u8> *Y_ch  = &YCbCr_data[0];
    ImageChannel<u8> *Cb_ch = &YCbCr_data[1];
    ImageChannel<u8> *Cr_ch = &YCbCr_data[2];

    for (u64 i = 0; i < RGB_data[0].height; i++) {
        for (u64 j = 0; j < RGB_data[0].width; j++) {
            u8 R = (*R_ch)(i, j);
            u8 G = (*G_ch)(i, j);
            u8 B = (*B_ch)(i, j);
            u8 Y = \
                std::clamp(std::round(  0.299 * R +0.587 * G +0.114 * B),             0.0, 255.0);
            u8 Cb = \
                std::clamp(std::round((-0.299 * R -0.587 * G +0.886 * B)/1.772) +128, 0.0, 255.0);
            u8 Cr = \
                std::clamp(std::round(( 0.701 * R -0.587 * G -0.114 * B)/1.402) +128, 0.0, 255.0);
        
            (*Y_ch)(i, j)  = Y;
            (*Cb_ch)(i, j) = Cb;
            (*Cr_ch)(i, j) = Cr;
        }
    }

    g_timers.end("RGB2YCbCr");

    u64 duration = g_timers.duration("RGB2YCbCr");

    INFO("RGB2YCbCr duration = %llu ms\n", duration);

    return YCbCr_data;
}

#endif // YCBCR_H