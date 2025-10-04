#ifndef RLC_H
#define RLC_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"

/* NOTE: this is jpeg's RLE (rely on many zeros in data and omitting DC) */

template<typename T1, typename T2>
inline int RLC(const std::vector<T1> data, std::vector<T2>& RLC_data) {
    RLC_data.reserve(data.size());

    u64 count = 0;
    for (u64 i = 1; i < data.size(); i++) {
        if (data[i] == 0) {
			count++;
			/* ZRL */
			if (count == 16) {
				RLC_data.push_back(0xF);
				RLC_data.push_back(0);
				count = 0;
			}
		}
        else {
            RLC_data.push_back(count);
            RLC_data.push_back(log2(std::abs(data[i]))+1);
            RLC_data.push_back(data[i]);
            count = 0;
        }
    }

	/* ! TODO: peak at decoding, EOB should not be explicit ! */
    // EOB
	// if (count != 0) {
	RLC_data.push_back(0);
	RLC_data.push_back(0);
	// }
    return 0;
}


/* NOTE: this is jpeg's RLE (rely on many zeros in data and omitting DC) */
template<typename T>
inline int decodeRLE(const std::vector<T> RLC_data, std::vector<T>& data) {
	
	u64 write_i = 1;

	for (u64 i = 0; i < RLC_data.size(); i += 3) {
		u8 run  = RLC_data[i];
		u8 size = RLC_data[i+1];
		if (size == 0 && run == 0) {
			INFO("DECODED EOB\n");
			break;
		}
		if (size == 0 && run == 0xF) {
			INFO("DECODED ZRL\n");
			i--;
			continue;
		}
		s16 ampl = RLC_data[i+2];
		INFO("DECODED run = %d size = %d, ampl = %d\n", run, size, ampl);
		write_i += run;
		data[write_i] = ampl;
		write_i++;
	}
    return 0;
}


#endif // RLC_H