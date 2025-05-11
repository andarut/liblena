#include "lena.hpp"

namespace enc {

/* ENCODER */
BitStream JPEG(const std::vector<ImageChannel<u8>>& RGB_data) {

    printf("=== ENCODED ===\n");

    /* Color Transform */
    auto YCbCr_data = RGB2YCbCr(RGB_data);

    /* Downsampling */
    auto Cb_downsampled = enc::downsampling(YCbCr_data[1], {4, 4, 4});
    auto Cr_downsampled = enc::downsampling(YCbCr_data[2], {4, 4, 4});

    /* MCUs */
    auto Y_MCUs  = enc::MCUs(YCbCr_data[0],  {8, 8});
    auto Cb_MCUs = enc::MCUs(Cb_downsampled, {8, 8});
    auto Cr_MCUs = enc::MCUs(Cr_downsampled, {8, 8});

    /* DCT */
    auto Y_DCT  = enc::DCT(Y_MCUs);
    auto Cb_DCT = enc::DCT(Cb_MCUs);
    auto Cr_DCT = enc::DCT(Cr_MCUs);

    /* Quantization */
    auto Y_quantizated  = enc::quantization(Y_DCT,  50);
    auto Cb_quantizated = enc::quantization(Cb_DCT, 50);
    auto Cr_quantizated = enc::quantization(Cr_DCT, 50);

    print_ch(YCbCr_data[0]);
    g_visualization.show(YCbCr_data[0]);
    

    /* Entropy Coding */
    return enc::entropy_coding(Y_quantizated, Cb_quantizated, Cr_quantizated);
}

} // namespace enc

namespace dec {

/* DECODER */
std::vector<ImageChannel<u8>> JPEG(BitStream& bs) {

    printf("=== DECODED ===\n");

    // 8x8
    auto chs_MCUs = dec::entropy_coding(bs);

    auto test_data = chs_MCUs[0][0];

    auto dequantizated = dec::quantization(test_data, 50);

    auto IDCT = dec::DCT(dequantizated);

    auto resampled = dec::downsampling<u8>(IDCT, {4, 4, 4});

    auto RGB_data = YCbCr2RGB({resampled, resampled, resampled});

    print_ch(resampled);
    g_visualization.show(resampled);

    std::vector<ImageChannel<u8>> v;
    return v;
}

} // namespace dec

int main() {
    std::filesystem::path resDir(RESOURCE_DIR);
	// std::ifstream ppm_file(resDir / "ppm_image.ppm");
    std::ifstream ppm_file(resDir / "8x8.ppm");
    
    auto RGB_data = PPM(ppm_file);
    
    auto enc_bs = enc::JPEG(RGB_data);
    enc_bs.fwrite("JPEG.bs");

    BitStream dec_bs;
    dec_bs.fread("JPEG.bs");
    auto decoded_RGB_data = dec::JPEG(dec_bs);

    // g_visualization.show(RGB_data);

    return 0;
}