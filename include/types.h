#ifndef TYPES_H
#define TYPES_H

#include "utils.h"
#include "logging.h"

struct RawImageData {
    u64 width, height;
    u8 numberOfChannels;
    std::vector<u64> data;

    RawImageData() = default;

    RawImageData(u64 _width, u64 _height, u8 _numberOfChannels=1) : \
        width(_width), height(_height), numberOfChannels(_numberOfChannels) {
        data.resize(_width * _height * _numberOfChannels);
    }

    RawImageData(u64 _width, u64 _height, u8 _numberOfChannels, std::vector<u64> _data) : \
        width(_width), height(_height), numberOfChannels(_numberOfChannels) {
        data = _data;
    };

    u64 size() const {
        return data.size();
    }

    friend bool operator==(const RawImageData& data1, const RawImageData& data2) {
        return (data1.width == data2.width) && \
               (data1.height == data2.height) && \
               (data1.numberOfChannels == data2.numberOfChannels) && \
               (data1.data == data2.data);
    }

    void print() {
        assert(numberOfChannels == 1);
        for (u64 i = 0; i < height; i++) {
            for (u64 j = 0; j < width; j++) {
                printf("%02lld ", data[width * i + j]);
            }
            printf("\n");
        }
    }

    u64& operator()(u64 i, u64 j) {
        Logger::log_info("set data (%d, %d)", i, j);
        assert(numberOfChannels == 1);
        return data[width * i + j];
    }

    u64& operator()(u64 i, u64 j, u64 k) {
        Logger::log_info("set data (%d, %d, %d)", i, j, k);
        assert(numberOfChannels > 1);
        return data[numberOfChannels * width * i + numberOfChannels * j + k];
    }

    const u64& operator()(u64 i, u64 j, u64 k) const {
        Logger::log_info("get data (%d, %d, %d)", i, j, k);
        assert(numberOfChannels > 1);
        return data[numberOfChannels * width * i + numberOfChannels * j + k];
    }

    // channel data
    RawImageData operator[](u64 ch_i) const {
        Logger::log_info("get channel data [%d]", ch_i);
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

    RawImageData get_block(u64 block_i, u64 block_j, u64 block_width, u64 block_height) const {
        Logger::log_info("get_block %lld %lld %lld %lld", block_i, block_j, block_width, block_height);
        RawImageData block(block_width, block_height, numberOfChannels);
        for (u64 k = 0; k < numberOfChannels; k++) {
            for (u64 i = block_i; i < block_i + block_height; i++)
                for (u64 j = block_j; j < block_j + block_width; j++)
                    block(i-block_i, j-block_j, k) = (*this)(i, j, k);
        }
        return block;
    }

    void set_block(u64 set_i, u64 set_j, RawImageData &block) {
        Logger::log_info("set_block %lld %lld", set_i, set_j);
        for (u64 k = 0; k < numberOfChannels; k++) {
            for (u64 i = set_i; i < set_j + block.height; i++)
                for (u64 j = set_j; j < set_j + block.width; j++)
                    (*this)(i, j, k) = block(i-set_i, j-set_j, k);
        }
    }

};

typedef RawImageData RawImage;

#endif // TYPES_H