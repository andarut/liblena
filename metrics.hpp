#ifndef METRICS_H
#define METRICS_H

#include "types.hpp"

template <typename T>
f64 MSE(const RawImageData<T>& I, const RawImageData<T>& K) {
    f64 mse = 0;
    for (u64 i = 0; i < I.height; i++)
        for (u64 j = 0; j < I.width; j++)
            for (u8 ch = 0; ch < I.numberOfChannels; ch++)
                mse += pow(I[ch](i, j) - K[ch](i, j), 2);
    mse /= (I.height * I.width * I.numberOfChannels);
    return mse;
}

template <typename T>
f64 PSNR(const RawImageData<T>& I, const RawImageData<T>& K) {
    const f64 MAX_I = 255;
    return 20*log10(MAX_I/sqrt(MSE(I,K)));
}

#endif // METRICS_H