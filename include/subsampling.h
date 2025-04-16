#ifndef SUBSAMPLING_H
#define SUBSAMPLING_H

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
struct SubsamplingMode {
    u64 J, a, b;    
    SubsamplingMode(u64 _J, u64 _a, u64 _b) : J(_J), a(_a), b(_b) {}

    friend bool operator==(const SubsamplingMode& mode1, const SubsamplingMode& mode2) {
        return (mode1.J == mode2.J && \
                mode1.a == mode2.a && \
                mode1.b == mode2.b);
    }
};

// subsampling channel
RawChannelData subsampling(const RawChannelData &data, const SubsamplingMode &mode);

// subsampling channels
RawImageData subsampling(const RawImageData &data, const SubsamplingMode &mode);

#endif // SUBSAMPLING_H