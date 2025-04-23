#ifndef SUBSAMPLING_H
#define SUBSAMPLING_H

#include "utils.h"
#include "types.h"

/*
Supported mods are:
- 4:4:4
- 4:2:2
- 4:1:1
- 4:4:0
- 4:2:0
- 4:1:0
- 4:4:1
- 4:2:1
*/
struct SubsamplingMode {
    u64 J, a, b;    
    SubsamplingMode(u64 _J, u64 _a, u64 _b) : J(_J), a(_a), b(_b) {}

    friend bool operator==(const SubsamplingMode& mode1, const SubsamplingMode& mode2) {
        return (mode1.J == mode2.J && \
                mode1.a == mode2.a && \
                mode1.b == mode2.b);
    }
};

template <typename T>
struct SubsampledChannelData : public RawChannelData<T> {
    SubsamplingMode mode;
    std::vector<T> data;

    u64 original_width, original_height; /* because of 3x5 case */

    SubsampledChannelData() = default;
    SubsampledChannelData(u64 size, SubsamplingMode _mode) : \
        mode(_mode) {
        data.reserve(size);
    }

    void print() const {
        if (data.size() != this->width * this->height) {
            for (u64 i = 0; i < data.size(); i++)
                printf("%hhu ", data[i]);
            return;
        }
        for (u64 i = 0; i < this->height; i++) {
            for (u64 j = 0; j < this->width; j++) {
                printf("%hhu ", data[this->width * i + j]);
            }
            printf("\n");
        }
    }
};

template <typename T>
struct SubsampledImageData : RawImageData<T> {
    u64 width, height;
    SubsamplingMode mode;
    std::vector<SubsampledChannelData<T>> data;

    SubsampledImageData() = default;
    SubsampledImageData(u64 _width, u64 _height, SubsamplingMode _mode, \
        std::vector<SubsampledChannelData<T>> _data) : width(_width), height(_height), mode(_mode), \
        data(_data) {}

    const SubsampledChannelData<T>& operator[](u64 ch_i) const {
        return data[ch_i];
    } 
};

/* ENCODING */
template <typename T>
SubsampledChannelData<T> encode_subsampling(const RawChannelData<T> &data, const SubsamplingMode &mode) {

    g_timers.start("encoding");

    u64 block_w = mode.J;
    u64 block_h = 2;

    Logger::log_info("mode = %lld:%lld:%lld", mode.J, mode.a, mode.b);

    Logger::log_info("width = %lld", data.width);
    Logger::log_info("height = %lld", data.height);

    SubsampledChannelData<T> subsampled_data(data.width * data.height, mode);

    u64 write_i = 0;
    for (u64 read_i = 0; read_i < data.height; read_i++) {
        u64 write_j = 0;
        u64 samples_count = (read_i % 2 == 0) ? mode.a : mode.b;
        if (read_i % 2 != 0 && mode.b == 0) continue;
        for (u64 read_j = 0; read_j < data.width; read_j++) {
            if ((read_j % mode.J) % samples_count != 0 && samples_count != mode.J) continue;
            subsampled_data.data.push_back(data(read_i, read_j));
            write_j++;
            if (samples_count == 1) read_j+=block_w-1;
        }
        if (read_i == 0) subsampled_data.width = subsampled_data.data.size();
        write_i++;
    }
    subsampled_data.height = write_i;

    g_timers.end("encoding");

    printf("subsampled_data.data.size() = %lld\n", subsampled_data.data.size());

    printf("encoding duration = %lld\n", g_timers.duration("encoding"));

    printf("=== ENCODED ===\n");
    subsampled_data.print();

    subsampled_data.original_width = data.width;
    subsampled_data.original_height = data.height;

    return subsampled_data;
}

template <typename T>
SubsampledImageData<T> encode_subsampling(const RawImageData<T> &data, const SubsamplingMode &mode) {
    return SubsampledImageData<T>(data.width, data.height, mode, std::vector<SubsampledChannelData<T>>({
        encode_subsampling(data[0], SubsamplingMode(4, 4, 4)),
        encode_subsampling(data[1], mode),
        encode_subsampling(data[2], mode)
    }));
}

/* DECODING */
template <typename T>
RawChannelData<T> decode_subsampling(const SubsampledChannelData<T> &subsampled_data) {
    
    auto mode = subsampled_data.mode;
    Logger::log_info("decode_subsampling mode = %lld:%lld:%lld", mode.J, mode.a, mode.b);

    RawChannelData<T> decoded_data(0, 0);
    decoded_data.width = subsampled_data.original_width;
    decoded_data.height = subsampled_data.original_height;
    decoded_data.data.resize(decoded_data.width * decoded_data.height);

    printf("width = %lld\n",  subsampled_data.original_width);
    printf("height = %lld\n",  subsampled_data.original_height);

    printf("=== TO BE DECODED ===\n");
    subsampled_data.print();

    printf("=== START DECODING ===\n");
    u64 read_i = 0, read_j = 0;
    u64 write_i = 0, write_j = 0;

    for (read_i = 0; read_i < subsampled_data.height; read_i++) {
        for (read_j = 0; read_j < subsampled_data.width; read_j++) {
            u64 read_index = read_i * subsampled_data.width + read_j;
            u64 element;
            if (read_index >= subsampled_data.data.size()) {
                element = subsampled_data.data.size()-1;
            } else element = subsampled_data.data[read_index];

            u8 a_or_b = (read_i % 2 == 0) ? mode.a : mode.b;
            if (a_or_b == 0) {
                for (u64 copy_j = 0; copy_j < decoded_data.width; copy_j++) {
                    decoded_data(write_i, copy_j) = decoded_data(write_i-1, copy_j);
                    write_j++;
                }
                continue;
            }

            u8 copy_count = mode.J / a_or_b;
            for (u64 i = 0; i < copy_count; i++) {
                if (write_j == decoded_data.width) break;
                printf("adding %lld, %lld = %lld\n", write_i, write_j, element);
                decoded_data(write_i, write_j) = element;
                write_j++;
            }
           
        }
        write_i++;
        write_j = 0;
    }

    if (subsampled_data.height == 1 && mode.b == 0) {
        for (u64 copy_j = 0; copy_j < decoded_data.width; copy_j++) {
            decoded_data(write_i, copy_j) = decoded_data(write_i-1, copy_j);
            write_j++;
        }
    }
    
    printf("=== DECODED ===\n");
    decoded_data.print();

    return decoded_data;
}

template <typename T>
RawImageData<T>   decode_subsampling(const SubsampledImageData<T>   &subsampled_data) {
    return RawImageData<T>(subsampled_data.width, subsampled_data.height, std::vector<RawChannelData<T>>({
        decode_subsampling(subsampled_data[0]),
        decode_subsampling(subsampled_data[1]),
        decode_subsampling(subsampled_data[2])
    }));
}

#endif // SUBSAMPLING_H