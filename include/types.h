#ifndef TYPES_H
#define TYPES_H

#include "utils.h"
#include "logging.h"

/* 
TODO: incapsulate data to avoid messing up data.size() and .size()
TODO: regulate resize and reverse for data
*/

template <typename T>
struct RawChannelData {
    u64 width, height;
    std::vector<T> data;

    RawChannelData() = default;
    
    RawChannelData(u64 _width, u64 _height) : \
        width(_width), height(_height) {
        data.resize(_width * _height);
    }

    RawChannelData(u64 _width, u64 _height, std::vector<T> _data) : \
        width(_width), height(_height), data(_data) {
        }

    friend bool operator==(const RawChannelData<T>& data1, const RawChannelData<T>& data2) {
        return (data1.width == data2.width) && \
               (data1.height == data2.height) && \
               (data1.data == data2.data);
    }

    void print() const {
        printf("print width = %lld\n", width);
        printf("print height = %lld\n", height);
        printf("print data size = %zu\n", data.size());
        if (data.size() != width * height) {
            for (u64 i = 0; i < data.size(); i++)
                printf("%hhu ", data[i]);
            return;
        }
        for (u64 i = 0; i < height; i++) {
            for (u64 j = 0; j < width; j++) {
                printf("%hhu ", data[width * i + j]);
            }
            printf("\n");
        }
    }
    
    T& operator()(u64 i, u64 j) {
        // Logger::log_info("set data (%d, %d)", i, j);
        return data[width * i + j];
    }

    const T& operator()(u64 i, u64 j) const {
        // Logger::log_info("get data (%d, %d)", i, j);
        return data[width * i + j];
    }

    RawChannelData<T> get_block(u64 block_i, u64 block_j, u64 block_width, u64 block_height) const {
        Logger::log_info("get_block %lld %lld %lld %lld", block_i, block_j, block_width, block_height);
        RawChannelData<T> block(block_width, block_height);
        for (u64 i = block_i; i < block_i + block_height; i++)
            for (u64 j = block_j; j < block_j + block_width; j++)
                block(i-block_i, j-block_j) = (*this)(i, j);
        return block;
    }

    void set_block(u64 set_i, u64 set_j, RawChannelData<T> &block) {
        Logger::log_info("set_block %lld %lld %lld %lld", set_i, set_j, block.width, block.height);
        for (u64 i = set_i; i < set_i + block.height; i++)
            for (u64 j = set_j; j < set_j + block.width; j++)
                (*this)(i, j) = block(i-set_i, j-set_j);
    }

    void remove(u64 i, u64 j) {
        Logger::log_info("remove %lld %lld\n", i, j);
        data.erase(data.begin() + (i * width + j));
    }

    u64 size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }

};

template <typename T>
struct RawImageData {
    u64 width, height;
    u8 numberOfChannels;
    std::vector<RawChannelData<T>> data;

    RawImageData() = default;

    RawImageData(u64 _width, u64 _height, std::vector<RawChannelData<T>> _data) : \
        width(_width), height(_height), numberOfChannels(_data.size()), data(_data) {}

    RawImageData(u64 _width, u64 _height, u8 _numberOfChannels) : \
        width(_width), height(_height), numberOfChannels(_numberOfChannels) {
        data.resize(_numberOfChannels);
        for (u64 k = 0; k < _numberOfChannels; k++)
            data[k] = RawChannelData<T>(_width, _height);
    }

    RawImageData(const std::initializer_list<T> _data) : \
        width(1), height(1), numberOfChannels(_data.size()) {
        if (_data.size() == 0) {
            width = 0;
            height = 0;
        }
        data.reserve(_data.size());
        for (auto& d : _data)
            data.push_back(RawChannelData<T>(1, 1, {d}));
    }

    void print() const {
        printf("print width = %lld\n", width);
        printf("print height = %lld\n", height);
        printf("print data size = %zu\n", data.size());
        for (u64 i = 0; i < height; i++) {
            for (u64 j = 0; j < width; j++) {
                printf("(%03hhu %03hhu %03hhu) ", data[0](i, j), data[1](i, j), data[2](i, j));
            }
            printf("\n");
        }
    }

    RawChannelData<T>& operator[](u64 ch_i) {
        // Logger::log_info("set channel data [%d]", ch_i);
        return data[ch_i];
    }

    const RawChannelData<T>& operator[](u64 ch_i) const {
        // Logger::log_info("get channel data [%d]", ch_i);
        return data[ch_i];
    }

    RawImageData<T> get_block(u64 block_i, u64 block_j, u64 block_width, u64 block_height) const {
        Logger::log_info("get_block %lld %lld %lld %lld", block_i, block_j, block_width, block_height);
        RawImageData<T> block(block_width, block_height, numberOfChannels);
        for (u64 k = 0; k < numberOfChannels; k++) {
            RawChannelData<T> block_ch(block_width, block_height);
            for (u64 i = block_i; i < block_i + block_height; i++)
                for (u64 j = block_j; j < block_j + block_width; j++)
                    block_ch(i-block_i, j-block_j) = (*this)[k](i, j);
            block[k] = block_ch;
        }
        return block;
    }

    void set_block(u64 set_i, u64 set_j, RawImageData<T> &block) {
        Logger::log_info("set_block %lld %lld %lld %lld", set_i, set_j, block.width, block.height);
        for (u64 k = 0; k < numberOfChannels; k++) {
            for (u64 i = set_i; i < set_j + block.height; i++)
                for (u64 j = set_j; j < set_j + block.width; j++)
                    (*this)[k](i, j) = block[k](i-set_i, j-set_j);
        }
    }

    friend bool operator==(const RawImageData<T>& data1, const RawImageData<T>& data2) {
        return (data1.width == data2.width) && \
               (data1.height == data2.height) && \
               (data1.numberOfChannels == data2.numberOfChannels) && \
               (data1.data == data2.data);
    }

    u64 size() const {
        u64 _size = 0;
        for (u64 k = 0; k < numberOfChannels; k++)
            _size += (*this)[k].size();
        return _size;
    }

    bool empty() const {
        bool _empty = true;
        for (u64 k = 0; k < numberOfChannels; k++)
            _empty &= (*this)[k].empty();
        return _empty; 
    }

};

#endif // TYPES_H