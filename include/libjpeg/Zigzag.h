#ifndef ZIGZAG_H
#define ZIGZAG_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"

/*
    NOTE: will reserve space for output data!
*/
template<typename T1, typename T2>
inline int zigzag(const ImageCh<T1>& data, std::vector<T2>& outputData) {
    outputData.reserve(data.size());

	const u64 width = data.width();
	const u64 height = data.height();

    for (u8 sum = 0; sum <= height + width - 2; sum++) {
        if (sum % 2 == 0) {
            u8 i = (sum < height) ? sum : height - 1;
            u8 j = sum - i;
            while (i < height && j < width) {
                outputData.push_back(data(i, j));
                if (i == 0 || j == width - 1) break;
                --i;
                ++j;
            }
        } else {
            u8 j = (sum < width) ? sum : width - 1;
            u8 i = sum - j;
            while (i < height && j < width) {
                outputData.push_back(data(i, j));
                if (j == 0 || i == height - 1) break;
                ++i;
                --j;
            }
        }
    }

    return 0;
}


/*
    NOTE: you should prepare output container
*/
template<typename T>
inline int reverse_zigzag(const std::vector<T>& zigzag, ImageCh<T>& data) {
	const u64 width = data.width();
	const u64 height = data.height();

	u64 idx = 0;

    for (u8 sum = 0; sum <= height + width - 2; sum++) {
        if (sum % 2 == 0) {
            u8 i = (sum < height) ? sum : height - 1;
            u8 j = sum - i;
            while (i < height && j < width) {
                data(i, j) = zigzag[idx++];
                if (i == 0 || j == width - 1) break;
                --i;
                ++j;
            }
        } else {
            u8 j = (sum < width) ? sum : width - 1;
            u8 i = sum - j;
            while (i < height && j < width) {
                data(i, j) = zigzag[idx++];
                if (j == 0 || i == height - 1) break;
                ++i;
                --j;
            }
        }
    }
    return 0;
}


#endif // ZIGZAG_H