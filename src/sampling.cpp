#include "sampling.h"

RawImageData sampling(const RawImageData &data, const SamplingMode &mode) {
    assert(data.numberOfChannels == mode.channels_count);

    /* Can't copy first row from nothing */
    for (int k = 0; k < mode.channels_count; k++)
        assert(mode[k] != 0);

    RawImageData sampled_data(data.width, data.height, data.numberOfChannels);

    /* Step 1: Get blocks sized w x h from every channel */
    std::vector<RawImageData> blocks((data.width * data.height) / (mode.w * mode.h));

    u64 block_i = 0, block_j = 0;
    for (u64 i = 0; i < blocks.size(); i++) {
        blocks[i] = data.get_block(block_i, block_j, mode.w, mode.h);
        block_j += mode.w;
        if (block_j >= data.width) {

            // TODO: take care about borders
            // if (block_j > data.width)   

            block_j = 0;
            block_i += mode.h;
        }
    }

    /* Step 2: For each block run sampling */
    Logger::log_info("=== SAMPLING START ===");
    u64 k = 0;
    for (auto& block : blocks) {
        u64 end_k = mode.rates.size() + k;
        Logger::log_info("k = %lld", k);
        Logger::log_info("end_k = %lld", end_k);
        
        for (; k < end_k; k++) {
            Logger::log_info("k = %lld", k);
            auto rate = mode.rates[k];
            Logger::log_info("rate = %lld", rate);

            u64 row_i = (k / mode.channels_count);
            Logger::log_info("row_i = %lld", row_i);

            u64 ch_i = (k % mode.channels_count);
            Logger::log_info("ch_i = %lld", ch_i);

            /* Copy row if rate is zero */
            if (rate == 0) {
                for (u64 i = 0; i < mode.w; i++) {
                    block[ch_i](row_i, i) = block[ch_i](row_i-1, i);
                }
                break;
            }

            u64 step = (mode.w / rate);
            Logger::log_info("step = %lld", step);
           
            
            u64 j = 0;
            for (u64 i = 0; i < mode.w; i++) {
                if (i != 0 && i % step == 0) j += step;
                block[ch_i](row_i, i) = block[ch_i](row_i, j);
                
            }
        }
    }
    Logger::log_info("=== SAMPLING END ===");

    /* Step 3: Set new blocks */
    u64 set_i = 0, set_j = 0;
    for (auto& block : blocks) {
        
        sampled_data.set_block(set_i, set_j, block);
        set_j += mode.w;
        if (set_j > data.width) {
            set_j = 0;
            set_i++;
        }
    }

    /* compress samplied data if needed */
    

    return sampled_data;
}