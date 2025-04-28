#ifndef YCBCR_H
#define YCBCR_H

#include "utils.h"
#include "types.h"


/* TODO: move to u8 type only, because [0, 255] formula */

/* decimation (TODO: try interpolation) */
/* 
JPEG conversion (ITU-T T.871)
R, G, B   in range [0, 255]
Y, Cb, Cr in range [0, 255]
*/
template <typename T>
RawImageData<T> rgb_to_ycbcr(const RawImageData<T> &rgb_data) {
    if (rgb_data.empty()) return rgb_data;
    assert(rgb_data.numberOfChannels == 3);

    g_timers.start("rgb_to_ycbcr");

    RawImageData<T> ycbcr_data(rgb_data.width, rgb_data.height, rgb_data.numberOfChannels);

    const RawChannelData<T> *_R = &rgb_data[0];
    const RawChannelData<T> *_G = &rgb_data[1];
    const RawChannelData<T> *_B = &rgb_data[2];

    RawChannelData<T> *_Y  = &ycbcr_data[0];
    RawChannelData<T> *_Cb = &ycbcr_data[1];
    RawChannelData<T> *_Cr = &ycbcr_data[2];

    for (u64 i = 0; i < rgb_data.height; i++) {
        for (u64 j = 0; j < rgb_data.width; j++) {
            T R = (*_R)(i, j);
            T G = (*_G)(i, j);
            T B = (*_B)(i, j);
            T Y = \
                std::clamp(std::round(  0.299 * R +0.587 * G +0.114 * B),             0.0, 255.0);
            T Cb = \
                std::clamp(std::round((-0.299 * R -0.587 * G +0.886 * B)/1.772) +128, 0.0, 255.0);
            T Cr = \
                std::clamp(std::round(( 0.701 * R -0.587 * G -0.114 * B)/1.402) +128, 0.0, 255.0);
        
            (*_Y)(i, j)  = Y;
            (*_Cb)(i, j) = Cb;
            (*_Cr)(i, j) = Cr;
        }
    }
    g_timers.end("rgb_to_ycbcr");

    u64 duration = g_timers.duration("rgb_to_ycbcr");

    printf("rgb_to_ycbcr duration = %lld ms\n", duration);

    return ycbcr_data;
}

/* 
JPEG conversion (ITU-T T.871)
R, G, B   in range [0, 255]
Y, Cb, Cr in range [0, 255]
*/
template <typename T>
RawImageData<T> ycbcr_to_rgb(const RawImageData<T> &ycbcr_data) {
    if (ycbcr_data.empty()) return ycbcr_data;
    assert(ycbcr_data.numberOfChannels == 3);

    g_timers.start("ycbcr_to_rgb");

    RawImageData<T> rgb_data(ycbcr_data.width, ycbcr_data.height, ycbcr_data.numberOfChannels);

    const RawChannelData<T> *_Y  = &ycbcr_data[0];
    const RawChannelData<T> *_Cb = &ycbcr_data[1];
    const RawChannelData<T> *_Cr = &ycbcr_data[2];

    RawChannelData<T> *_R = &rgb_data[0];
    RawChannelData<T> *_G = &rgb_data[1];
    RawChannelData<T> *_B = &rgb_data[2];

    for (u64 i = 0; i < rgb_data.height; i++) {
        for (u64 j = 0; j < rgb_data.width; j++) {
            T Y  = (*_Y)(i, j);
            T Cb = (*_Cb)(i, j);
            T Cr = (*_Cr)(i, j);
            T R = \
                std::clamp(std::round(Y + 1.402 * (Cr - 128)),             0.0, 255.0);
            T G = \
                std::clamp(std::round(Y - (0.114*1.772*(Cb-128)+0.299*1.402*(Cr-128))/0.587), 0.0, 255.0);
            T B = \
                std::clamp(std::round(Y + 1.772*(Cb-128)), 0.0, 255.0);
        
            (*_R)(i, j) = R;
            (*_G)(i, j) = G;
            (*_B)(i, j) = B;
        }
    }
    g_timers.end("ycbcr_to_rgb");

    u64 duration = g_timers.duration("ycbcr_to_rgb");

    printf("ycbcr_to_rgb duration = %lld ms\n", duration);

    return rgb_data;
}

#endif // YCBCR_H