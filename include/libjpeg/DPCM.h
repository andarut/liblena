#ifndef DPCM_H
#define DPCM_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"

template<typename T>
inline T& DC(ImageCh<T>& MCU) {
    return MCU(0, 0);
}

template<typename T>
void DPCM(std::vector<ImageCh<T>>& MCUs) {
    T DC_ref = DC(MCUs[0]);
    for (u64 i = 1; i < MCUs.size(); i++) {
        DC(MCUs[i]) -= DC_ref;
    }
}

#endif // DPCM_H