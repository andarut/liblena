#include "sampling.h"

RawChannelData channel_sampling(const RawChannelData &data, const SamplingMode &mode) {
    if (mode == SamplingMode(4, 4, 4)) return data;

    u64 block_w = mode.J;
    u64 block_h = 2;

    u64 new_block_w = std::max(mode.a, mode.b);
    u64 new_block_h = (mode.b == 0) ? 1 : 2;
    
    u64 sampled_data_width = (data.width / block_w) * new_block_w;
    u64 sampled_data_height = (data.height / block_h) * new_block_h;

    Logger::log_info("mode = %lld:%lld:%lld", mode.J, mode.a, mode.b);

    Logger::log_info("block_w = %lld", block_w);
    Logger::log_info("block_h = %lld", block_h);

    Logger::log_info("new_block_w = %lld", new_block_w);
    Logger::log_info("new_block_h = %lld", new_block_h);

    Logger::log_info("sampled_data_width = %lld", sampled_data_width);
    Logger::log_info("sampled_data_height = %lld", sampled_data_height);

    RawChannelData sampled_data(sampled_data_width, sampled_data_height);

    for (u64 block_i = 0; block_i < data.height; block_i+=block_h) {
        for (u64 block_j = 0; block_j < data.width; block_j+=block_w) {
            auto block = data.get_block(block_i, block_j, block_w, block_h);
            auto new_block = RawChannelData(new_block_w, new_block_h);
            for (u64 i = 0; i < block_h; i++) {
                if (i % 2 != 0 && mode.b == 0) break;
                u64 step = (i % 2 == 0) ? (mode.J / mode.a) : (mode.J / mode.b);
                if (i % 2 != 0) { // b
                    if (mode.b != mode.a && mode.b != 0) {
                        // TODO: Think about support and change this behavior
                        Logger::log_warning("mode = %lld:%lld:%lld not supported yet", mode.J, mode.a, mode.b);
                        exit(1);
                    }
                }
                for (u64 j = 0; j < block_w; j+=step) {
                    if (j > 1)
                        new_block(i, j-step+1) = block(i, j);
                    else
                        new_block(i, j) = block(i, j);
                }
            }
            if (block_i > 1)
                if (block_j > 1)
                    sampled_data.set_block(block_i-block_h+1, block_j-block_w+1, new_block);
                else
                    sampled_data.set_block(block_i-block_h+1, block_j, new_block);
            else
                if (block_j > 1)
                    sampled_data.set_block(block_i, block_j-block_w+1, new_block);
                else
                    sampled_data.set_block(block_i, block_j, new_block);
        }
    }

    return sampled_data;
}

RawImageData sampling(const RawImageData &data, const SamplingMode &mode) {
    if (mode == SamplingMode(4, 4, 4)) return data;
    return RawImageData(data.width, data.height, std::vector<RawChannelData>({
        channel_sampling(data[0], SamplingMode(4, 4, 4)),
        channel_sampling(data[1], mode),
        channel_sampling(data[2], mode)
    }));
}