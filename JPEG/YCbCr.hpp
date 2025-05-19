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
inline std::vector<ImageChannel<u8>> RGB2YCbCr(const std::vector<ImageChannel<u8>> &RGB_data) {
    assert(!RGB_data.empty());

    INFO("data chs = %zu\n", RGB_data.size());
    assert(RGB_data.size() == 3);

    INFO("data size = %llu x %llu\n", RGB_data[0].width(), RGB_data[0].height());

    g_timers.start("RGB2YCbCr");

    std::vector<ImageChannel<u8>> YCbCr_data(3);
    YCbCr_data[0].resize(RGB_data[0].width(), RGB_data[0].height());
    YCbCr_data[1].resize(RGB_data[0].width(), RGB_data[0].height());
    YCbCr_data[2].resize(RGB_data[0].width(), RGB_data[0].height());

    const ImageChannel<u8> *R_ch = &RGB_data[0];
    const ImageChannel<u8> *G_ch = &RGB_data[1];
    const ImageChannel<u8> *B_ch = &RGB_data[2];

    ImageChannel<u8> *Y_ch  = &YCbCr_data[0];
    ImageChannel<u8> *Cb_ch = &YCbCr_data[1];
    ImageChannel<u8> *Cr_ch = &YCbCr_data[2];

    for (u64 i = 0; i < RGB_data[0].height(); i++) {
        for (u64 j = 0; j < RGB_data[0].width(); j++) {
            u8 R = (*R_ch)(i, j);
            u8 G = (*G_ch)(i, j);
            u8 B = (*B_ch)(i, j);
            u8 Y = \
                std::clamp(std::round(  0.299 * R +0.587 * G +0.114 * B),             0.0, 255.0);
            u8 Cb = \
                std::clamp(std::round((-0.299 * R -0.587 * G +0.886 * B)/1.772) +128.0, 0.0, 255.0);
            u8 Cr = \
                std::clamp(std::round(( 0.701 * R -0.587 * G -0.114 * B)/1.402) +128.0, 0.0, 255.0);
        
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

/* 
JPEG conversion (ITU-T T.871)
R, G, B   in range [0, 255]
Y, Cb, Cr in range [0, 255]
*/
inline std::vector<ImageChannel<u8>> YCbCr2RGB(const std::vector<ImageChannel<u8>> &YCbCr_data) {
    assert(!YCbCr_data.empty());

    INFO("data chs = %zu\n", YCbCr_data.size());
    assert(YCbCr_data.size() == 3);

    INFO("data size = %llu x %llu\n", YCbCr_data[0].width(), YCbCr_data[0].height());

    g_timers.start("YCbCr2RGB");

    std::vector<ImageChannel<u8>> RGB_data(3);
    RGB_data[0].resize(YCbCr_data[0].width(), YCbCr_data[0].height());
    RGB_data[1].resize(YCbCr_data[0].width(), YCbCr_data[0].height());
    RGB_data[2].resize(YCbCr_data[0].width(), YCbCr_data[0].height());

    const ImageChannel<u8> *Y_ch  = &YCbCr_data[0];
    const ImageChannel<u8> *Cb_ch = &YCbCr_data[1];
    const ImageChannel<u8> *Cr_ch = &YCbCr_data[2];

    ImageChannel<u8> *R_ch = &RGB_data[0];
    ImageChannel<u8> *G_ch = &RGB_data[1];
    ImageChannel<u8> *B_ch = &RGB_data[2];

    for (u64 i = 0; i < YCbCr_data[0].height(); i++) {
        for (u64 j = 0; j < YCbCr_data[0].width(); j++) {
            u8 Y = (*Y_ch)(i, j);
            u8 Cb = (*Cb_ch)(i, j);
            u8 Cr = (*Cr_ch)(i, j);
            
            u8 R = \
                std::clamp(std::round(Y + 1.402 * (Cr - 128)),             0.0, 255.0);
            u8 G = \
                std::clamp(std::round(Y - (0.114*1.772*(Cb-128)+0.299*1.402*(Cr-128))/0.587), 0.0, 255.0);
            u8 B = \
                std::clamp(std::round(Y + 1.772*(Cb-128)), 0.0, 255.0);
        
            (*R_ch)(i, j)  = R;
            (*G_ch)(i, j) = G;
            (*B_ch)(i, j) = B;
        }
    }

    g_timers.end("YCbCr2RGB");

    u64 duration = g_timers.duration("YCbCr2RGB");

    INFO("YCbCr2RGB duration = %llu ms\n", duration);

    return RGB_data;
}

#endif // YCBCR_H