#ifndef TYPES_H
#define TYPES_H

#include "utils.h"
#include "logging.h"

struct RawChannelData {
    u64 width, height;
    std::vector<u64> data;

    RawChannelData() = default;

    RawChannelData(u64 _width, u64 _height) : \
        width(_width), height(_height) {
        data.resize(_width * _height);
    }

    RawChannelData(u64 _width, u64 _height, std::vector<u64> _data) : \
        width(_width), height(_height), data(_data) {}

    friend bool operator==(const RawChannelData& data1, const RawChannelData& data2) {
        return (data1.width == data2.width) && \
               (data1.height == data2.height) && \
               (data1.data == data2.data);
    }

    void print() {
        for (u64 i = 0; i < height; i++) {
            for (u64 j = 0; j < width; j++) {
                printf("%02lld ", data[width * i + j]);
            }
            printf("\n");
        }
    }

    u64& operator()(u64 i, u64 j) {
        Logger::log_info("set data (%d, %d)", i, j);
        return data[width * i + j];
    }

    const u64& operator()(u64 i, u64 j) const {
        Logger::log_info("get data (%d, %d)", i, j);
        return data[width * i + j];
    }

};

struct RawImageData {
    u64 width, height;
    u8 numberOfChannels;
    std::vector<RawChannelData> data;

    RawImageData() = default;

    RawImageData(u64 _width, u64 _height, std::vector<RawChannelData> _data) : \
        width(_width), height(_height), numberOfChannels(_data.size()), data(_data) {}

    RawImageData(u64 _width, u64 _height, u8 _numberOfChannels) : \
        width(_width), height(_height), numberOfChannels(_numberOfChannels) {
        data.resize(_numberOfChannels);
        for (u64 k = 0; k < _numberOfChannels; k++)
            data[k] = RawChannelData(_width, _height);
    }

    RawChannelData& operator[](u64 ch_i) {
        Logger::log_info("get channel data [%d]", ch_i);
        return data[ch_i];
    }

    const RawChannelData& operator[](u64 ch_i) const {
        Logger::log_info("get channel data [%d]", ch_i);
        return data[ch_i];
    }

    RawImageData get_block(u64 block_i, u64 block_j, u64 block_width, u64 block_height) const {
        Logger::log_info("get_block %lld %lld %lld %lld", block_i, block_j, block_width, block_height);
        RawImageData block(block_width, block_height, numberOfChannels);
        for (u64 k = 0; k < numberOfChannels; k++) {
            RawChannelData block_ch(block_width, block_height);
            for (u64 i = block_i; i < block_i + block_height; i++)
                for (u64 j = block_j; j < block_j + block_width; j++)
                    block_ch(i-block_i, j-block_j) = (*this)[k](i, j);
            block[k] = block_ch;
        }
        return block;
    }

    void set_block(u64 set_i, u64 set_j, RawImageData &block) {
        Logger::log_info("set_block %lld %lld %lld %lld", set_i, set_j, block.width, block.height);
        for (u64 k = 0; k < numberOfChannels; k++) {
            for (u64 i = set_i; i < set_j + block.height; i++)
                for (u64 j = set_j; j < set_j + block.width; j++)
                    (*this)[k](i, j) = block[k](i-set_i, j-set_j);
        }
    }

};

typedef RawImageData RawImage;

#endif // TYPES_H