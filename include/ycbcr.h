#ifndef YCBCR_H
#define YCBCR_H

#include "utils.h"
#include "types.h"

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
#endif // YCBCR_H