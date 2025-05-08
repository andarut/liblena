#ifndef ENTROPY_CODING_H
#define ENTROPY_CODING_H

#include "utils.h"
#include "types.h"
#include "globals.h"

template<typename T>
std::vector<T> zigzag_order(const RawChannelData<T> data) {
    std::vector<T> zigzag(data.data.size());

    u64 read_i = 0, read_j = 0;
    for (u64 i = 0; i < zigzag.size(); i++) {

        zigzag[i] = data(read_i, read_j);

        if ((read_i+read_j) % 2 == 0) { 
            if (read_i > 0) read_i--; 
            if (read_j < data.width-1) read_j++; 
        } else { 
            if (read_i < data.height-1) read_i++; 
            if (read_j > 0) read_j--; 
        }
    }

    return zigzag;
}

/* NOTE: this is jpeg's RLE (rely on many zeros in data and omitting DC) */
template<typename T>
std::vector<T> encode_RLE(const std::vector<T> data) {
    std::vector<T> encoded_data;
    encoded_data.reserve(data.size());

    u64 count = 0;
    for (u64 i = 1; i < data.size(); i++) {
        if (data[i] == 0) count++;
        else {
            encoded_data.push_back(count);
            encoded_data.push_back(log2(std::abs(data[i]))+1);
            encoded_data.push_back(data[i]);
            count = 0;
        }
    }

    // EOB
    encoded_data.push_back(0);
    encoded_data.push_back(0);

    return encoded_data;
}

// BitStream encode_huffman() {
	
// }

#endif // ENTROPY_CODING_H