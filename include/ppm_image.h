#ifndef PPMIMAGE_H
#define PPMIMAGE_H

#include "utils.h"


// TODO: move
typedef std::vector<u64> RawImageData;

struct RawImage {
    u64 width, height;
    u8 numberOfChannels;
    RawImageData data;

    RawImage(u64 _width, u64 _height, u8 _numberOfChannels) : \
        width(_width), height(_height), numberOfChannels(_numberOfChannels) {
        data.resize(_width * _height * _numberOfChannels);
    }

    u64& operator()(u64 i, u64 j, u64 k) {
        return data[numberOfChannels * width * i + numberOfChannels * j + k];
    }

    const u64& operator()(u64 i, u64 j, u64 k) const {
        return data[numberOfChannels * width * i + numberOfChannels * j + k];
    }

};
///

struct PPMImage : RawImage {
    std::string magicNumber;
    u16 maxVal;

    PPMImage(std::string _magicNumber, u64 _width, u64 _height, u8 _numberOfChannels, u16 _maxVal) : \
        RawImage(_width, _height, _numberOfChannels), magicNumber(_magicNumber), maxVal(_maxVal) {};
};

PPMImage read_ppm_image(std::ifstream &ppm_file);

#endif // PPMIMAGE_H
