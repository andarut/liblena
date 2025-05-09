#include "lena.hpp"

BitStream JPEG(std::vector<ImageChannel<u8>> RGB_data) {
    /* Color Transform */
    auto YCbCr_data = RGB2YCbCr(RGB_data);

    /* Downsampling */
    auto Cb_downsampled = downsampling(YCbCr_data[1], {4, 2, 2});
    auto Cr_downsampled = downsampling(YCbCr_data[2], {4, 2, 2});

    /* MCUs */
    auto Y_MCUs  = MCUs(YCbCr_data[0],  {8, 8});
    auto Cb_MCUs = MCUs(Cb_downsampled, {8, 8});
    auto Cr_MCUs = MCUs(Cr_downsampled, {8, 8});

    /* DCT */
    // auto Y_DCT  = DCT(Y_MCUs);
    // auto Cb_DCT = DCT(Cb_MCUs);
    // auto Cr_DCT = DCT(Cr_MCUs);

    // /* Quantization */
    // auto Y_quantizated  = quantization(Y_DCT,  50);
    // auto Cb_quantizated = quantization(Cb_DCT, 50);
    // auto Cr_quantizated = quantization(Cr_DCT, 50);

    // /* Entropy Coding */
    // return entropy_encoding(Y_quantizated, Cb_quantizated, Cr_quantizated);
    BitStream bs;
    return bs;
}

int main() {
    std::filesystem::path resDir(RESOURCE_DIR);
	std::ifstream ppm_file(resDir / "ppm_image_4K.ppm");
    std::vector<ImageChannel<u8>> RGB_data = PPM(ppm_file);
    JPEG(RGB_data);
    return 0;
}