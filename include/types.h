#ifndef TYPES_H
#define TYPES_H

#include "utils.h"

struct RawImageData {
    u64 width, height;
    u8 numberOfChannels;
    std::vector<u64> data;

    RawImageData(u64 _width, u64 _height, u8 _numberOfChannels=1) : \
        width(_width), height(_height), numberOfChannels(_numberOfChannels) {
        data.resize(_width * _height * _numberOfChannels);
    }

    u64 size() const {
        return data.size();
    }

    u64& operator()(u64 i, u64 j) {
        assert(numberOfChannels == 1);
        return data[width * i + j];
    }

    u64& operator()(u64 i, u64 j, u64 k) {
        assert(numberOfChannels > 1);
        return data[numberOfChannels * width * i + numberOfChannels * j + k];
    }

    const u64& operator()(u64 i, u64 j, u64 k) const {
        assert(numberOfChannels > 1);
        return data[numberOfChannels * width * i + numberOfChannels * j + k];
    }

    // get channel data
    RawImageData& operator[](u64 ch_i) const {
        RawImageData ch_data(width, height);
        u64 data_i = ch_i;
        for (u64 i = 0; i < height; i++) {
            for (u64 j = 0; j < width; j++) {
                ch_data(i, j) = data[data_i];
                data_i += numberOfChannels;
            }
        }
        return ch_data;
    }
    
    // TODO: support numberOfChannels > 1
    RawImageData& get_block(u64 block_i, u64 block_j, u64 block_width, u64 block_height) const {
        assert(numberOfChannels == 1);
        RawImageData block(block_width, block_height);
        for (u64 i = block_i; i < block_i + block_height; i++)
            for (u64 j = block_j; j < block_j + block_width; j++)
                block(i-block_i, j-block_j) = (i, j);
        return block;
    }

};

typedef RawImageData RawImage;

#endif // TYPES_H