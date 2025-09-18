#ifndef METRICS_H
#define METRICS_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"
#include "Print.hpp"

template <typename T>
f64 MSE(const ImageChannel<T>& I, const ImageChannel<T>& K) {
    f64 mse = 0;
    for (u64 i = 0; i < I.height(); i++)
        for (u64 j = 0; j < I.width(); j++)
            mse += pow(I(i, j) - K(i, j), 2);
    mse /= (I.height() * I.width());
    return mse;
}

template <typename T>
f64 MSE(const std::vector<ImageChannel<T>>& I, const std::vector<ImageChannel<T>>& K) {
    f64 mse = 0;
    for (u8 ch = 0; ch < I.size(); ch++)
        mse += MSE(I[ch], K[ch]);
    mse /= I.size();
    return mse;
}

template <typename T>
f64 PSNR(const std::vector<ImageChannel<T>>& I, const std::vector<ImageChannel<T>>& K) {
    const f64 MAX_I = 255;
    return 20*log10(MAX_I/sqrt(MSE(I,K)));
}

#endif // METRICS_H