#include "DCT.h"

/*
    TODO: inplace?
*/
int DCT::FDCT(ImageCh<s32>& ch) {

    /* Step 1: Recenter around zero */
    for (u64 i = 0; i < ch.height(); i++)
        for (u64 j = 0; j < ch.width(); j++)
            ch(i, j) = ch(i, j) - 128;

    ImageCh<s32> G(ch.width(), ch.height());

    auto alpha = [](u64 x) { return (x == 0) ? 1.0 / sqrt(2) : 1.0; };

    /* Step 2: Calculate the DCT Coefficients */
    for (u64 u = 0; u < ch.height(); u++)
        for (u64 v = 0; v < ch.width(); v++) {
            f64 sum = 0.0;
            for (u64 i = 0; i < ch.height(); i++)
                for (u64 j = 0; j < ch.width(); j++)
                    sum += ch(i, j) * cos((2.0*i+1.0)*u*M_PI/16.0) * cos((2.0*j+1.0)*v*M_PI/16.0);
            G(u, v) = static_cast<s32>(std::round(0.25 * alpha(u) * alpha(v) * sum));
        }

    ch = std::move(G);

    return 0;
}


/*
    TODO: inplace?
*/
int DCT::IDCT(ImageCh<u8>& ch) {

    /* Step 1: Reverse DCT */
    ImageCh<u8> f(ch.width(), ch.height());

    auto alpha = [](u64 x) { return (x == 0) ? 1.0 / sqrt(2) : 1.0; };

    for (u64 i = 0; i < ch.height(); i++)
        for (u64 j = 0; j < ch.width(); j++) {
            f64 sum = 0;
            for (u64 u = 0; u < ch.height(); u++)
                for (u64 v = 0; v < ch.width(); v++)
                    sum += alpha(u) * alpha(v) * ch(u, v) * cos((2.0*i+1.0)*u*M_PI/16.0) * cos((2.0*j+1.0)*v*M_PI/16.0);
            /* Step 2: Calculate the DCT Coefficients */
            f(i, j) = std::clamp(
                static_cast<u8>(std::round(0.25 * sum + 128.0)),
                static_cast<u8>(0),
                static_cast<u8>(255)
            );
        }

    return 0;
}