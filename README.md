<div align="center">

<picture>
  <source media="(prefers-color-scheme: light)" srcset="/docs/liblena_logo_light.svg">
  <img alt="liblena logo" src="/docs/liblena_logo_dark.svg" width="50%" height="50%">
</picture>

for experiments with data compression techniques

<h3>
</h3>

[![macos-x64](https://github.com/andarut/liblena/actions/workflows/macos_x64.yml/badge.svg?branch=main&event=push)](https://github.com/andarut/liblena/actions/workflows/macos_x64.yml)
[![macos-arm](https://github.com/andarut/liblena/actions/workflows/macos_arm.yml/badge.svg?branch=main&event=push)](https://github.com/andarut/liblena/actions/workflows/macos_arm.yml)
[![linux-x64](https://github.com/andarut/liblena/actions/workflows/linux_x64.yml/badge.svg?branch=main&event=push)](https://github.com/andarut/liblena/actions/workflows/linux_x64.yml)

</div>

---

This is library for experiments with different compression techniques.

Due to its extreme simplicity, it aims to be the perfect sandbox to implement, test or validate differect ideas for codecs.

## Features
### JPEG encoder
```cpp
BitStream JPEG(const std::vector<ImageChannel<u8>>& RGB_data) {
    BitStream bs;
    bs.write_u64(RGB_data[0].width());
    bs.write_u64(RGB_data[0].height());

    /* Color Transform */
    auto YCbCr_data = RGB2YCbCr(RGB_data);

    /* Downsampling */
    auto Cb_downsampled = enc::downsampling(YCbCr_data[1], {4, 1, 1});
    auto Cr_downsampled = enc::downsampling(YCbCr_data[2], {4, 1, 1});

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
```

### JPEG decoder
```cpp
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
    auto Cb_resampled = dec::downsampling<u8>(Cb, {4, 1, 1}, width, height);
    auto Cr_resampled = dec::downsampling<u8>(Cr, {4, 1, 1}, width, height);

    /* Color Transform */
    auto RGB_data = YCbCr2RGB({Y, Cb_resampled, Cr_resampled});

    return RGB_data;
}
```

### Buffer visualizations
```cpp
g_visualization.show(decoded_RGB_data);
```

<picture>
  <img alt="liblena demo" src="/docs/demo.png" width="50%" height="50%">
</picture>

## Requirements

Currently library building with `conan` inside **virtualenv**, the only requirements are `Python (>= 3.6)` and C++ compiler.

**IMPORTANT! On Intel Macs you should use `clang` installed from `brew` (to enable support for AddressSanitizer)**

**IMPORTANT! Run with `source` command, so virtualenv can be kept opened.**
```
source ./scripts/prepare_<OS>_<ARCH>_<BUILD_TYPE>.sh
```

ExampleS:
```
source ./scripts/prepare_macos_x64.sh
```
```
source ./scripts/prepare_macos_arm.sh
```
```
source ./scripts/prepare_linux_x64.sh
```

This script will install dependencies and link installed `cmake` into `.venv/bin/cmake`.

## Configure
FIRST SEE [Requirements](Requirements) !

From now on you can build with **cmake configs**. Main is `conan-debug`.

```
cmake --preset 'conan-debug'
```

## Build
```
make -j$(nproc) -C build/Debug
```

On macos you can use `sysctl -n hw.ncpu` instead of `nproc`. `nproc` can be installed with `brew install coreutils`. 
```
make -j$(sysctl -n hw.ncpu) -C build/Debug
```
or
```
make -j$(sysctl -n hw.ncpu) -C build/Release
```

## Demo
```
./build/Debug/liblena_tests
```

## Releases
