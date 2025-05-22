#include <gtest/gtest.h>
#include <filesystem>

#include "lena.hpp"

template <typename T>
void _ASSERT_EQ(std::vector<ImageChannel<T>> data1, std::vector<ImageChannel<T>> data2) {
    f64 _PSNR = PSNR(data1, data2);
    INFO("COMPARING PSNR = %.2f\n", _PSNR);
    assert(_PSNR > 40.0); // good to acceptable quality 
}

namespace enc {

/* ENCODER */
BitStream JPEG(const std::vector<ImageChannel<u8>>& RGB_data) {
    INFO("=== ENCODING ===\n");

    BitStream bs;
    bs.write_u64(RGB_data[0].width());
    bs.write_u64(RGB_data[0].height());

    /* Color Transform */
    auto YCbCr_data = RGB2YCbCr(RGB_data);

    /* Downsampling */
    auto Cb_downsampled = enc::downsampling(YCbCr_data[1], {4, 2, 2});
    auto Cr_downsampled = enc::downsampling(YCbCr_data[2], {4, 2, 2});

    /* MCUs */
    auto Y_MCUs  = enc::MCUs(YCbCr_data[0], {8, 8});
    auto Cb_MCUs = enc::MCUs(Cb_downsampled, {8, 8});
    auto Cr_MCUs = enc::MCUs(Cr_downsampled, {8, 8});

    bs.write_u64(Y_MCUs.size());
    bs.write_u64(Cb_MCUs.size());
    bs.write_u64(Cr_MCUs.size());

    /* DCT */
    auto Y_DCT  = enc::DCT(Y_MCUs);
    auto Cb_DCT = enc::DCT(Cb_MCUs);
    auto Cr_DCT = enc::DCT(Cr_MCUs);

    /* Quantization */
    auto Y_quantizated  = enc::luminance_quantization(Y_DCT,  50);
    auto Cb_quantizated = enc::chrominance_quantization(Cb_DCT, 50);
    auto Cr_quantizated = enc::chrominance_quantization(Cr_DCT, 50);

    /* Entropy Coding */
    enc::entropy_coding(bs, Y_quantizated, Cb_quantizated, Cr_quantizated);

    return bs;
}

} // namespace enc

namespace dec {

/* DECODER */
std::vector<ImageChannel<u8>> JPEG(BitStream& bs) {
    bs.rewind();
    INFO("=== DECODING ===\n");

    u64 width = bs.read_u64();
    u64 height = bs.read_u64();

    u64 Y_MCUs_count = bs.read_u64();
    u64 Cb_MCUs_count = bs.read_u64();
    u64 Cr_MCUs_count = bs.read_u64();

    /* Entropy Coding */
    auto chs_MCUs = dec::entropy_coding(bs, Y_MCUs_count, Cb_MCUs_count, Cr_MCUs_count);
    auto Y_MCUs  = chs_MCUs[0];
    auto Cb_MCUs = chs_MCUs[1];
    auto Cr_MCUs = chs_MCUs[2];

    /* Dequantization */
    auto Y_dequantizated_MCUs  = dec::luminance_quantization(Y_MCUs, 50);
    auto Cb_dequantizated_MCUs = dec::chrominance_quantization(Cb_MCUs, 50);
    auto Cr_dequantizated_MCUs = dec::chrominance_quantization(Cr_MCUs, 50);

    /* IDCT */
    auto Y_IDCT_MCUs  = dec::DCT(Y_dequantizated_MCUs);
    auto Cb_IDCT_MCUs = dec::DCT(Cb_dequantizated_MCUs);
    auto Cr_IDCT_MCUs = dec::DCT(Cr_dequantizated_MCUs);

    /* MCUs */
    auto Y  = dec::MCUs(Y_IDCT_MCUs,  {8, 8}, width, height);
    auto Cb = dec::MCUs(Cb_IDCT_MCUs, {8, 8}, width, height);
    auto Cr = dec::MCUs(Cr_IDCT_MCUs, {8, 8}, width, height);
    
    /* Resample */
    auto Cb_resampled = dec::downsampling<u8>(Cb, {4, 2, 2}, width, height);
    auto Cr_resampled = dec::downsampling<u8>(Cr, {4, 2, 2}, width, height);

    /* Color Transform */
    auto RGB_data = YCbCr2RGB({Y, Cb_resampled, Cr_resampled});

    return RGB_data;
}

} // namespace dec

// TEST(TEST_JPEG, JPEG_ppm_image) {
//     /* INPUT */
//     std::filesystem::path resDir(RESOURCE_DIR);

//     std::ifstream expected_ppm_file(resDir / "decoded/ppm_image.ppm");
//     ASSERT_TRUE(expected_ppm_file.is_open());
//     auto expected_RGB_data = PPM(expected_ppm_file);

//     std::ifstream ppm_file(resDir / "ppm_image.ppm");

//     auto RGB_data = PPM(ppm_file);
    
//     /* RUN ENCODING */
//     auto enc_bs = enc::JPEG(RGB_data);
//     enc_bs.fwrite("ppm_image.bs");

//     /* RUN DECODING */
//     BitStream dec_bs;
//     dec_bs.fread("ppm_image.bs");
//     auto decoded_RGB_data = dec::JPEG(dec_bs);

//     /* COMPARING WITH LIBJPEG */
//     _ASSERT_EQ(decoded_RGB_data, expected_RGB_data);
// }

TEST(TEST_JPEG, JPEG_lena_image) {
    /* INPUT */
    std::filesystem::path resDir(RESOURCE_DIR);

    std::ifstream expected_ppm_file(resDir / "decoded/Lenna.ppm");
    ASSERT_TRUE(expected_ppm_file.is_open());
    auto expected_RGB_data = PPM(expected_ppm_file);

    std::ifstream ppm_file(resDir / "Lenna.ppm");

    auto RGB_data = PPM(ppm_file);
    
    /* RUN ENCODING */
    auto enc_bs = enc::JPEG(RGB_data);
    enc_bs.fwrite("Lenna.bs");

    /* RUN DECODING */
    BitStream dec_bs;
    dec_bs.fread("Lenna.bs");
    auto decoded_RGB_data = dec::JPEG(dec_bs);

    g_visualization.show(decoded_RGB_data);

    /* COMPARING WITH LIBJPEG */
    _ASSERT_EQ(decoded_RGB_data, expected_RGB_data);
}