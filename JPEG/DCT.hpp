#ifndef DCT_H
#define DCT_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"

namespace enc {

inline ImageChannel<f64> DCT(const ImageChannel<u8>& ch) {
    ImageChannel<f64> g(ch.width(), ch.height());
    g.resize(ch.width(), ch.height());

    /* Step 1: Recenter around zero */
    for (u64 i = 0; i < ch.height(); i++)
        for (u64 j = 0; j < ch.width(); j++)
            g(i, j) = (f64)(ch(i, j) - 128.0);

    ImageChannel<f64> G(ch.width(), ch.height()); 
    G.resize(ch.width(), ch.height()); 

    auto alpha = [](u64 x) { return (x == 0) ? 1.0 / sqrt(2) : 1.0; };

    /* Step 2: Calculate the DCT Coefficients */
    for (u64 u = 0; u < ch.height(); u++)
        for (u64 v = 0; v < ch.width(); v++) {
            f64 sum = 0.0;
            for (u64 i = 0; i < ch.height(); i++)
                for (u64 j = 0; j < ch.width(); j++)
                    sum += g(i, j) * cos((2.0*i+1.0)*u*M_PI/16.0) * cos((2.0*j+1.0)*v*M_PI/16.0);
            G(u, v) = 0.25 * alpha(u) * alpha(v) * sum;
        }

    return G;
}

inline std::vector<ImageChannel<f64>> DCT(const std::vector<ImageChannel<u8>>& _MCUs) {
    g_timers.start("DCT MCUs");

    std::vector<ImageChannel<f64>> DCT_MCUs(_MCUs.size());
    for (u64 i = 0; i < _MCUs.size(); i++) {
        DCT_MCUs[i].resize(_MCUs[i].width(), _MCUs[i].height());
        DCT_MCUs[i] = DCT(_MCUs[i]);
        INFO("BEFORE DCT Y MCU %d\n", i);
		print(_MCUs[i]);
        INFO("IDCT Y MCU %d\n", i);
		print(DCT_MCUs[i]);
    }
    g_timers.end("DCT MCUs");

    u64 duration = g_timers.duration("DCT MCUs");

    INFO("DCT MCUs for %llu MCUs duration = %llu ms\n", _MCUs.size(), duration);

    return DCT_MCUs;
}

} // namespace enc


namespace dec {

inline ImageChannel<u8> DCT(const ImageChannel<f64>& data) {
    
    /* Step 1: Reverse DCT */
    ImageChannel<f64> f(data.width(), data.height());
    f.resize(data.width(), data.height());
    auto alpha = [](u64 x) { return (x == 0) ? 1.0 / sqrt(2) : 1.0; };

    for (u64 i = 0; i < data.height(); i++)
        for (u64 j = 0; j < data.width(); j++) {
            f64 sum = 0;
            for (u64 u = 0; u < data.height(); u++)
                for (u64 v = 0; v < data.width(); v++)
                    sum += alpha(u) * alpha(v) * data(u, v) * cos((2.0*i+1.0)*u*M_PI/16.0) * cos((2.0*j+1.0)*v*M_PI/16.0);
            f(i, j) = 0.25 * sum;
        }
    
    /* Step 2: Calculate the DCT Coefficients */
    ImageChannel<u8> decoded_data(data.width(), data.height());
    decoded_data.resize(data.width(), data.height());
    for (u64 i = 0; i < data.height(); i++)
        for (u64 j = 0; j < data.width(); j++) {
            f64 shifted = f(i,j) + 128.0;
            s16 val = (s16)std::floor(shifted + 0.5);
            decoded_data(i,j) = (u8)std::min(std::max(val, (s16)0), (s16)255);
            // f64 _f = f(i, j) + 128.0;
            // s16 r = (s16)std::round(_f);
            // if (0 <= r && r <= 255) decoded_data(i, j) = static_cast<u8>(r);
            // if (r > 255) decoded_data(i, j) = 255;
            // if (r < 0) decoded_data(i, j) = 0;
        }
            

    return decoded_data;
}

inline std::vector<ImageChannel<u8>> DCT(const std::vector<ImageChannel<f64>>& _MCUs) {
    g_timers.start("IDCT MCUs");

    std::vector<ImageChannel<u8>> DCT_MCUs(_MCUs.size());
    for (u64 i = 0; i < _MCUs.size(); i++) {
        DCT_MCUs[i].resize(_MCUs[i].width(), _MCUs[i].height());
        DCT_MCUs[i] = dec::DCT(_MCUs[i]);
        INFO("BEFORE IDCT Y MCU %d\n", i);
		print(_MCUs[i]);
        INFO("IDCT Y MCU %d\n", i);
		print(DCT_MCUs[i]);
    }
    g_timers.end("IDCT MCUs");

    u64 duration = g_timers.duration("IDCT MCUs");

    INFO("IDCT MCUs for %llu MCUs duration = %llu ms\n", _MCUs.size(), duration);

    return DCT_MCUs;
}


} // namespace dec

#endif // DCT_H