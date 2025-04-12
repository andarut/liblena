#ifndef SUBSAMPLING_H
#define SUBSAMPLING_H

#include "utils.h"
#include "types.h"

/*
1. block sized w x h from every color channel
2. rates = {rate_<COLOR_1>, rate_<COLOR_2>, rate_<COLOR_3>, ...}

Example from chrome submpling 4:4:4
----
input:
w = 4
h = 2

rates size = number of colors * height of block = 3 * 2 = 6
rates = {4, 4, 4, 4, 4, 4};
that means that for row of w pixels (there are h of those) - get

row 1 (h = 2):
    - 4 pixels of <CHANNEL_1>
    - 4 pixels of <CHANNEL_2>
    - 4 pixels of <CHANNEL_3>
row 2 (h = 2):
    - 4 pixels of <CHANNEL_1>
    - 4 pixels of <CHANNEL_2>
    - 4 pixels of <CHANNEL_3>


if rate is 0, then copy last row

    +-
Y = | y1



*/
struct SamplingMode {
    u64 w, h;
    u64 channels_count;

    std::vector<u64> rates;

    SamplingMode(u64 _w, u64 _h, u64 _channels_count) \
        : w(_w), h(_h), channels_count(_channels_count) {
        rates.resize(_channels_count * _h);
    }
};

RawImageData sampling(const RawImageData &data, const SamplingMode &mode);

#endif // SUBSAMPLING_H