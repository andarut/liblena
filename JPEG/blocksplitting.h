#ifndef BLOCKSPLITTING_H
#define BLOCKSPLITTING_H

#include "types.hpp"
#include "utils.hpp"

/*
Supported modes:
- 8x8
- 16x8
- 16x16
*/
struct BlockSplittingMode {
    u64 w, h;
    BlockSplittingMode(u64 _w, u64 _h) : w(_w), h(_w) {}

    BlockSplittingMode() = default;
};

template <typename T>
struct BlockSplitedChannelData {
    BlockSplittingMode mode;
    std::vector<RawChannelData<T>> MCUs;

    /* TODO: add this params into BlockSplitedData */
    u64 original_width, original_height;

    BlockSplitedChannelData() = default;

    void print() {
        for (auto& MSU : MCUs) {
            printf("=== BLOCK ===\n");
            MSU.print();
            printf("=============\n");
        }
    }
    
};

template <typename T>
struct BlockSplitedData : public RawImageData<T> {
    BlockSplittingMode mode;
    std::vector<BlockSplitedChannelData<T>> data;

    BlockSplitedData() = default;

    BlockSplitedData(BlockSplittingMode _mode, std::vector<BlockSplitedChannelData<T>> _data) : \
        mode(_mode), data(_data) {}

    BlockSplitedChannelData<T>& operator[](u64 i) {
        return data[i];
    }

};

template<typename T>
bool operator==(const std::vector<RawChannelData<T>> &lhs, const BlockSplitedChannelData<T> &rhs) {
    return (lhs == rhs.MCUs);
}

template<typename T>
bool operator==(const BlockSplitedChannelData<T> &lhs, const std::vector<RawChannelData<T>> &rhs) {
    return rhs == lhs;
}

/* ENCODING */

template <typename T>
BlockSplitedChannelData<T> encode_blocksplitting(RawChannelData<T> data, BlockSplittingMode mode) {
    BlockSplitedChannelData<T> encoded_data;
    encoded_data.mode = mode;

    encoded_data.original_width = data.width;
    encoded_data.original_height = data.height;

    u64 blocks_count = (data.width * data.height) / (mode.w * mode.h);

    INFO("blocks_count = %lld", blocks_count);

    encoded_data.MCUs.reserve(blocks_count);

    u64 read_i = 0, read_j = 0;
    for (u64 i = 0; i < blocks_count; i++) {
        
        RawChannelData<T> block(mode.w, mode.h);

        for (u64 write_i = 0; write_i < mode.h; write_i++) {
            for (u64 write_j = 0; write_j < mode.w; write_j++) {
                block(write_i, write_j) = data(read_i+write_i, read_j+write_j);
            }
        }

        encoded_data.MCUs.push_back(block);

        read_j += mode.w;
        if (read_j >= data.width) {
            read_j = 0;
            read_i += mode.h;
        }
    }

    return encoded_data;
}

template <typename T>
BlockSplitedData<T> encode_blocksplitting(RawImageData<T> data, BlockSplittingMode mode) {
    return BlockSplitedData<T>(mode, std::vector<BlockSplitedChannelData<T>>({
        encode_blocksplitting(data[0], mode),
        encode_blocksplitting(data[1], mode),
        encode_blocksplitting(data[2], mode)
    }));
}

/* DECODING */

template <typename T>
RawChannelData<T> decode_blocksplitting(BlockSplitedChannelData<T> data) {
    RawChannelData<T> decoded_data(data.original_width, data.original_height);

    u64 write_i = 0, write_j = 0;
    for (auto& MCU : data.MCUs) {
        for (u64 read_i = 0; read_i < data.mode.h; read_i++) {
            for (u64 read_j = 0; read_j < data.mode.w; read_j++) {
                decoded_data(write_i+read_i, write_j+read_j) = MCU(read_i, read_j);
            }
        }
        write_j += data.mode.w;
        if (write_j >= decoded_data.width) {
            write_j = 0;
            write_i += data.mode.h;
        }
    }

    /* TODO: add non-block's pixels techniques */

    return decoded_data;
}

template <typename T>
RawImageData<T> decode_blocksplitting(BlockSplitedData<T> data) {
    return RawImageData<T>(data[0].original_width, data[0].original_height, std::vector<RawChannelData<T>>({
        decode_blocksplitting(data[0]),
        decode_blocksplitting(data[1]),
        decode_blocksplitting(data[2])
    }));
}

#endif // BLOCKSPLITTING_H