#include "DCT.h"

RawChannelData<f64> encode_DCT(const RawChannelData<u8>& data) {
    RawChannelData<f64> g(data.width, data.height);

    /* Step 1: Recenter around zero */
    for (u64 i = 0; i < data.height; i++)
        for (u64 j = 0; j < data.width; j++)
            g(i, j) = (f64)(data(i, j) - 128);

    RawChannelData<f64> G(data.width, data.height); 

    auto alpha = [](u64 x) { return (x == 0) ? 1.0 / sqrt(2) : 1.0; };

    /* Step 2: Calculate the DCT Coefficients */
    for (u64 u = 0; u < data.height; u++)
        for (u64 v = 0; v < data.width; v++) {
            f64 sum = 0.0;
            for (u64 i = 0; i < data.height; i++)
                for (u64 j = 0; j < data.width; j++)
                    sum += g(i, j) * cos((2.0*i+1.0)*u*M_PI/16.0) * cos((2.0*j+1.0)*v*M_PI/16.0);
            G(u, v) = 0.25 * alpha(u) * alpha(v) * sum;
        }

    return G;
}

RawChannelData<u8> decode_DCT(const RawChannelData<f64>& data) {
    
    /* Step 1: Reverse DCT */
    RawChannelData<f64> f(data.width, data.height);
    auto alpha = [](u64 x) { return (x == 0) ? 1.0 / sqrt(2) : 1.0; };

    for (u64 i = 0; i < data.height; i++)
        for (u64 j = 0; j < data.width; j++) {
            f64 sum = 0.0;
            for (u64 u = 0; u < data.height; u++)
                for (u64 v = 0; v < data.width; v++)
                    sum += alpha(u) * alpha(v) * data(u, v) * cos((2.0*i+1.0)*u*M_PI/16.0) * cos((2.0*j+1.0)*v*M_PI/16.0);
            f(i, j) = 0.25 * sum;
        }
    
    /* Step 2: Calculate the DCT Coefficients */
    RawChannelData<u8> decoded_data(data.width, data.height);
    for (u64 i = 0; i < data.height; i++)
        for (u64 j = 0; j < data.width; j++)
            decoded_data(i, j) = (u8)(f(i, j) + 128.0);

    return decoded_data;
}