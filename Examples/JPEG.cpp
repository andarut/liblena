// #include "lena.hpp"

// /*
// working

// cjpeg -debug -quality 50 -baseline -dct float -outfile test.jpg -sample 1x1,1x1,1x1 8x8.ppm 

// 4x4, 8x8, 50

// */

// namespace enc {

// /* ENCODER */
// BitStream JPEG(const std::vector<ImageChannel<u8>>& RGB_data) {
//     printf("=== ENCODED ===\n");

//     /* Color Transform */
//     auto YCbCr_data = RGB2YCbCr(RGB_data);

//     printf("ENCODED Y_data\n");
//     print_ch(YCbCr_data[0]);

//     /* Downsampling */
//     auto Cb_downsampled = enc::downsampling(YCbCr_data[1], {4, 4, 4});
//     auto Cr_downsampled = enc::downsampling(YCbCr_data[2], {4, 4, 4});

//     /* MCUs */
//     auto Y_MCUs  = enc::MCUs(YCbCr_data[0],  {8, 8});
//     auto Cb_MCUs = enc::MCUs(Cb_downsampled, {8, 8});
//     auto Cr_MCUs = enc::MCUs(Cr_downsampled, {8, 8});

//     /* DCT */
//     auto Y_DCT  = enc::DCT(Y_MCUs);
//     auto Cb_DCT = enc::DCT(Cb_MCUs);
//     auto Cr_DCT = enc::DCT(Cr_MCUs);

//     printf("ENCODED Y_DCT\n");
//     print_ch(Y_DCT[0]);

//     /* Quantization */
//     auto Y_quantizated  = enc::luminance_quantization(Y_DCT,  50);
//     auto Cb_quantizated = enc::chrominance_quantization(Cb_DCT, 50);
//     auto Cr_quantizated = enc::chrominance_quantization(Cr_DCT, 50);

//     printf("ENCODED Y_quantizated\n");
//     print_ch(Y_quantizated[0]);


//     /* Entropy Coding */
//     return enc::entropy_coding(Y_quantizated, Cb_quantizated, Cr_quantizated);
// }

// } // namespace enc

// namespace dec {

// /* DECODER */
// std::vector<ImageChannel<u8>> JPEG(BitStream& bs) {
//     bs.rewind();
//     printf("=== DECODED ===\n");

//     auto chs_MCUs = dec::entropy_coding(bs);
//     auto Y_MCUs  = chs_MCUs[0];
//     auto Cb_MCUs = chs_MCUs[1];
//     auto Cr_MCUs = chs_MCUs[2];

//     printf("DECODED readed Y_data\n");
//     print_ch(Y_MCUs[0]);

//     printf("DECODED readed Cb_data\n");
//     print_ch(Cb_MCUs[0]);

//     printf("DECODED readed Cr_data\n");
//     print_ch(Cr_MCUs[0]);

//     auto Y_dequantizated_MCUs  = dec::luminance_quantization(Y_MCUs, 50);
//     auto Cb_dequantizated_MCUs = dec::chrominance_quantization(Cb_MCUs, 50);
//     auto Cr_dequantizated_MCUs = dec::chrominance_quantization(Cr_MCUs, 50);

//     printf("DECODED Y_dequantizated_MCUs\n");
//     print_ch(Y_dequantizated_MCUs[0]);

//     printf("DECODED Cb_dequantizated_MCUs\n");
//     print_ch(Cb_dequantizated_MCUs[0]);

//     printf("DECODED Cr_dequantizated_MCUs\n");
//     print_ch(Cr_dequantizated_MCUs[0]);

//     auto Y_IDCT_MCUs  = dec::DCT(Y_dequantizated_MCUs);
//     auto Cb_IDCT_MCUs = dec::DCT(Cb_dequantizated_MCUs);
//     auto Cr_IDCT_MCUs = dec::DCT(Cr_dequantizated_MCUs);

//     printf("DECODED Y_IDCT\n");
//     print_ch(Y_IDCT_MCUs[0]);

//     printf("DECODED Cb_IDCT\n");
//     print_ch(Cb_IDCT_MCUs[0]);

//     printf("DECODED Cr_IDCT\n");
//     print_ch(Cr_IDCT_MCUs[0]);

//     auto Y  = dec::MCUs(Y_IDCT_MCUs, {8, 8});
//     auto Cb = dec::MCUs(Cb_IDCT_MCUs, {8, 8});
//     auto Cr = dec::MCUs(Cr_IDCT_MCUs, {8, 8});

//     auto Cb_resampled = dec::downsampling<u8>(Cb, {4, 4, 4});
//     auto Cr_resampled = dec::downsampling<u8>(Cr, {4, 4, 4});

//     printf("DECODED Y_data\n");
//     print_ch(Y);

//     printf("DECODED Cb_data\n");
//     print_ch(Cb);

//     printf("DECODED Cr_data\n");
//     print_ch(Cr);
    
//     auto RGB_data = YCbCr2RGB({Y, Cb, Cr});

//     // g_visualization.show(Y);
//     // g_visualization.show(Cb);
//     // g_visualization.show(Cr);
//     printf("bistream size = %d bytes\n", bs.bytes_size());
//     g_visualization.show(RGB_data);

//     printf("DECODED R_data\n");
//     print_ch(RGB_data[0]);
//     printf("DECODED G_data\n");
//     print_ch(RGB_data[1]);
//     printf("DECODED B_data\n");
//     print_ch(RGB_data[2]);

//     return RGB_data;
// }

// } // namespace dec

// int main() {
//     std::filesystem::path resDir(RESOURCE_DIR);
// 	std::ifstream ppm_file(resDir / "ppm_image.ppm");
//     // std::ifstream ppm_file(resDir / "8x8.ppm");
    
//     auto RGB_data = PPM(ppm_file);
    
//     auto enc_bs = enc::JPEG(RGB_data);
//     enc_bs.fwrite("JPEG.bs");
//     print(enc_bs);

//     BitStream dec_bs;
//     dec_bs.fread("JPEG.bs");
//     auto decoded_RGB_data = dec::JPEG(dec_bs);

//     // g_visualization.show(RGB_data);

//     return 0;
// }

int main() {}