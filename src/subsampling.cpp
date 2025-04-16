#include "subsampling.h"

RawChannelData subsampling(const RawChannelData &data, const SubsamplingMode &mode) {
    if (mode == SubsamplingMode(4, 4, 4)) return data;

    if (mode.b != mode.a && mode.b != 0) exit(1);

    u64 block_w = mode.J;
    u64 block_h = 2;

    u64 new_block_w = std::max(mode.a, mode.b);
    u64 new_block_h = (mode.b == 0) ? 1 : 2;
    
    u64 subsampled_data_width = (data.width / block_w) * new_block_w;
    u64 subsampled_data_height = (data.height / block_h) * new_block_h;

    Logger::log_info("mode = %lld:%lld:%lld", mode.J, mode.a, mode.b);

    Logger::log_info("width = %lld", data.width);
    Logger::log_info("height = %lld", data.height);

    Logger::log_info("subsampled_data_width = %lld", subsampled_data_width);
    Logger::log_info("subsampled_data_height = %lld", subsampled_data_height);

    RawChannelData subsampled_data(subsampled_data_width, subsampled_data_height);

    u64 write_i = 0;
    for (u64 read_i = 0; read_i < data.height; read_i++) {
        u64 write_j = 0;
        u64 samples_count = (read_i % 2 == 0) ? mode.a : mode.b;
        if (read_i % 2 != 0 && mode.b == 0) {
            write_i++;
            continue;
        }
        for (u64 read_j = 0; read_j < data.width; read_j++) {
            if ((read_j % mode.J) % samples_count != 0 && samples_count != mode.J) continue;
            subsampled_data(write_i, write_j) = data(read_i, read_j);
            write_j++;
            if (samples_count == 1) read_j+=block_w-1;
        }
        write_i++;
    }

    return subsampled_data;
}

RawImageData subsampling(const RawImageData &data, const SubsamplingMode &mode) {
    if (mode == SubsamplingMode(4, 4, 4)) return data;
    return RawImageData(data.width, data.height, std::vector<RawChannelData>({
        subsampling(data[0], SubsamplingMode(4, 4, 4)),
        subsampling(data[1], mode),
        subsampling(data[2], mode)
    }));
}