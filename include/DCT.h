#ifndef DCT_H
#define DCT_H

#include "utils.h"
#include "types.h"

RawChannelData<f64> encode_DCT(RawChannelData<u8>& data) {
    RawChannelData<f64> g(data.width, data.height);

    /* Step 1: Recenter around zero */
    for (u64 i = 0; i < data.height; i++)
        for (u64 j = 0; j < data.width; j++)
            g(i, j) = (f64)(data(i, j) - 128);

    g.print();

    RawChannelData<f64> G(data.width, data.height);

    auto alpha = [](u64 x) { return (x == 0) ? 1.0 / sqrt(2) : 1.0; };

    /* Step 2: Calculate the DCT Coefficients */
    for (u64 u = 0; u < data.height; u++)
        for (u64 v = 0; v < data.width; v++) {
            f64 sum = 0.0;
            for (u64 i = 0; i < data.height; i++)
                for (u64 j = 0; j < data.width; j++)
                    sum += g(i, j);
                    // sum += g(i, j) * cos((2.0*i+1.0)*u*M_PI/16.0) * cos((2.0*j+1.0)*v*M_PI/16.0);
            if (u == 0 && v == 0) {
                std::cout << "raw_sum = " << sum << "\n";
                std::cout << "G00 before norm = " << sum << "\n";
            }
            G(u, v) = 0.25 * alpha(u) * alpha(v) * sum;
        }

    return G;
}

#endif // DCT_H