#include "lena.hpp"

BitStream JPEG(std::vector<ImageChannel<u8>> RGB_data) {
    /* Color Transform */
    auto YCbCr_data = RGB2YCbCr(RGB_data);

    /* Downsampling */
    auto Cb_downsampled = downsampling(YCbCr_data[1], {4, 4, 4});
    auto Cr_downsampled = downsampling(YCbCr_data[2], {4, 4, 4});

    /* MCUs */
    auto Y_MCUs  = MCUs(YCbCr_data[0]);
    auto Cb_MCUs = MCUs(Cb_downsampled);
    auto Cr_MCUs = MCUs(Cr_downsampled);

    /* DCT */
    auto Y_DCT  = DCT(Y_MCUs);
    auto Cb_DCT = DCT(Cb_MCUs);
    auto Cr_DCT = DCT(Cr_MCUs);

    /* Quantization */
    auto Y_quantizated  = quantization(Y_DCT,  50);
    auto Cb_quantizated = quantization(Cb_DCT, 50);
    auto Cr_quantizated = quantization(Cr_DCT, 50);

    /* Entropy Coding */
    return entropy_encoding(Y_quantizated, Cb_quantizated, Cr_quantizated);
}