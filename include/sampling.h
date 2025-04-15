#ifndef SAMPLING_H
#define SAMPLING_H

#include "utils.h"
#include "types.h"

/*
Supported mods are:
- 4:4:4
- 4:2:2
- 4:1:1
- 4:4:0
- 4:2:0
- 4:1:0

Unsupported mods are:
- 4:4:1
- 4:2:1
*/
struct SamplingMode {
    u64 J, a, b;    
    SamplingMode(u64 _J, u64 _a, u64 _b) : J(_J), a(_a), b(_b) {}

    friend bool operator==(const SamplingMode& mode1, const SamplingMode& mode2) {
        return (mode1.J == mode2.J && \
                mode1.a == mode2.a && \
                mode1.b == mode2.b);
    }
};

// sampling channel
RawChannelData sampling(const RawChannelData &data, const SamplingMode &mode);

// sampling channels
RawImageData sampling(const RawImageData &data, const SamplingMode &mode);

#endif // SAMPLING_H