#include "subsampling.h"

RawImageData sampling(const RawImageData &data, const SamplingMode &mode) {
    
    // TODO: take care about borders

    /* Step 1: Get block sized w x h from every channel */
    std::vector<RawImageData> blocks;
    blocks.reserve(data.size() / mode.channels_count / (mode.w * mode.h));

    u64 block_i = 0, block_j = 0;
    for (u64 i = 0; i < mode.channels_count; i++) {
        blocks.push_back(data[i].get_block(block_i, block_j, mode.w, mode.h));
        block_j += mode.w;
        if (block_j > data.width) {
            block_j = 0;
            block_i += mode.h;
        }
    }

    /* Step 2: For each block run sampling */
    
}