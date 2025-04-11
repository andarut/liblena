#ifndef PPM_IMAGE_H
#define PPM_IMAGE_H

#include "utils.h"
#include "types.h"

struct PPMImage : RawImage {
    std::string magicNumber;
    u16 maxVal;

    PPMImage(std::string _magicNumber, u64 _width, u64 _height, u8 _numberOfChannels, u16 _maxVal) : \
        RawImage(_width, _height, _numberOfChannels), magicNumber(_magicNumber), maxVal(_maxVal) {};
};

PPMImage read_ppm_image(std::ifstream &ppm_file);

#endif // PPM_IMAGE_H
