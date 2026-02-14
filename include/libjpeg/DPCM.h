#ifndef DPCM_H
#define DPCM_H

#include "Utils.hpp"
#include "Logger.hpp"

template<typename T>
inline T& DC(std::vector<T>& MCU) {
    return MCU[0];
}

template<typename T>
inline T DC(const std::vector<T>& MCU) {
    return MCU[0];
}

template<typename T>
void DPCM(const std::vector<std::vector<T>>& MCUs, std::vector<std::vector<T>>& resultMCUs) {
  DEBUG("START DPCM\n");
  resultMCUs = MCUs;
  for (u64 i = 1; i < MCUs.size(); i++) {
    T newDC = DC(resultMCUs[i]) - DC(MCUs[i-1]);
    DEBUG("%d - %d = %d\n", DC(resultMCUs[i]), DC(MCUs[i-1]), newDC);
    DC(resultMCUs[i]) = newDC;
  }
}

#endif // DPCM_H
