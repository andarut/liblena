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

    for (u16 i = 0; i < height; i++) {
        for (u16 j = 0; j < width; j++) {
            /* (maxVal <= 255) ? u8 : u16 */
            if (maxVal <= 255) {
                u8 r, g, b;
                ppm_file >> r >> g >> b;
                (*R)(i, j) = static_cast<T>(r);
                (*G)(i, j) = static_cast<T>(g);
                (*B)(i, j) = static_cast<T>(b);
            } else {
                u16 r, g, b;
                ppm_file >> r >> g >> b;
                (*R)(i, j) = static_cast<T>(r);
                (*G)(i, j) = static_cast<T>(g);
                (*B)(i, j) = static_cast<T>(b);
            }
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
