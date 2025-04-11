#ifndef TYPES_H
#define TYPES_H

#include "utils.h"

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

#endif // TYPES_H