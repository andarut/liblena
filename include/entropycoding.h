#ifndef ENTROPY_CODING_H
#define ENTROPY_CODING_H

#include "utils.h"
#include "types.h"
#include "globals.h"

template<typename T>
std::vector<T> zigzag_order(const RawChannelData<T> data) {
    std::vector<T> zigzag(data.data.size());

    u64 read_i = 0;
    u64 read_j = 0;
    for (u64 i = 0; i < zigzag.size(); i++) {
        zigzag[i] = data(read_i, read_j);

        /* TODO: clamp after main without edges? */

        /* edges: correct direction */
        if (read_i == 0 || read_i == data.height-1) {
            if (read_j % 2 == 0) read_j++;
            else {
                if (read_i == 0) { read_i++; read_j--; } 
                else { read_i--; read_j++; }
            }
        } else if (read_j == 0 || read_j == data.width-1) {
            if (read_i % 2 == 0) {
                if (read_j == 0) { read_i--; read_j++; } 
                else { read_i++; read_j--; }
            } else read_i++;
        }

        /* main: save direction */
        else {
            if ((read_i+read_j) % 2 == 0) { read_i--; read_j++; } 
            else { read_i++; read_j--; }
        }

    }

    return zigzag;
}

#endif // ENTROPY_CODING_H