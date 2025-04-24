#ifndef PPM_IMAGE_H
#define PPM_IMAGE_H

#include "utils.h"
#include "types.h"

template <typename T>
struct PPMImage : RawImageData<T> {
    std::string magicNumber;
    u16 maxVal;

    PPMImage(std::string _magicNumber, u64 _width, u64 _height, u8 _numberOfChannels, u16 _maxVal) : \
        RawImageData<T>(_width, _height, _numberOfChannels), magicNumber(_magicNumber), maxVal(_maxVal) {};
};

template <typename T>
PPMImage<T> read_ppm_image(std::ifstream &ppm_file) {

    // Magic number
    std::string magic_number;
    ppm_file >> magic_number;

    Logger::log_info("magic_number = %s", magic_number.c_str());
    assert(magic_number == "P3" || magic_number == "P6");

    // Skip comments
    char line[1024];
    ppm_file.getline(line, 1024);
    while (ppm_file.peek() == '#') ppm_file.getline(line, 1024);

    // Read header
    u16 width, height, maxVal;
    ppm_file >> width >> height >> maxVal;
    
    Logger::log_info("width = %d", width);
    assert(width > 0);
    
    Logger::log_info("height = %d", height);
    assert(height > 0);
    
    Logger::log_info("maxVal = %d", maxVal);
    assert(maxVal > 0);
    
    PPMImage<T> ppm_image(magic_number, width, height, 3, maxVal);

    RawChannelData<T> *R = &ppm_image[0];
    RawChannelData<T> *G = &ppm_image[1];
    RawChannelData<T> *B = &ppm_image[2];

    ppm_file.get();
    std::vector<char> buffer(width * height * 3 * (maxVal > 255 ? 2 : 1));
    ppm_file.read(buffer.data(), buffer.size());

    int bytes_per_sample = (maxVal > 255 ? 2 : 1);

    for (u16 i = 0; i < height; i++) {
        for (u16 j = 0; j < width; j++) {
            (*R)(i, j) = static_cast<T>(buffer[i * width * 3 + j * 3 + 0]);
            (*G)(i, j) = static_cast<T>(buffer[i * width * 3 + j * 3 + 1]);
            (*B)(i, j) = static_cast<T>(buffer[i * width * 3 + j * 3 + 2]);

        //     size_t pix_idx = static_cast<size_t>(i) * width + j;
        // // base byte offset for this pixel
        // size_t base = pix_idx * 3 * bytes_per_sample;

        // uint16_t sample[3];
        // for (int c = 0; c < 3; ++c) {
        //     size_t off = base + c * bytes_per_sample;
        //     if (bytes_per_sample == 1) {
        //         sample[c] = static_cast<unsigned char>(buffer[off]);
        //     } else {
        //         // big-endian two-byte sample
        //         sample[c] = (static_cast<uint16_t>(
        //                          static_cast<unsigned char>(buffer[off])) << 8)
        //                   | static_cast<uint16_t>(
        //                          static_cast<unsigned char>(buffer[off + 1]));
        //     }
        // }

        // (*R)(i, j) = static_cast<T>(sample[0]);
        // (*G)(i, j) = static_cast<T>(sample[1]);
        // (*B)(i, j) = static_cast<T>(sample[2]);

    //         /* (maxVal <= 255) ? u8 : u16 */
    //         if (maxVal <= 255) {
    //             u8 r, g, b;
    //             ppm_file >> r >> g >> b;
    //             (*R)(i, j) = static_cast<T>(r);
    //             (*G)(i, j) = static_cast<T>(g);
    //             (*B)(i, j) = static_cast<T>(b);
    //         } else {
    //             u16 r, g, b;
    //             ppm_file >> r >> g >> b;
    //             (*R)(i, j) = static_cast<T>(r);
    //             (*G)(i, j) = static_cast<T>(g);
    //             (*B)(i, j) = static_cast<T>(b);
    //         }
        }
    }

    Logger::log_info("size = %d", ppm_image.size());

    assert(ppm_image.size() == width*height*3);

    assert(ppm_image[0].size() == width*height);
    assert(ppm_image[1].size() == width*height);
    assert(ppm_image[2].size() == width*height);

    return ppm_image;
}

#endif // PPM_IMAGE_H
