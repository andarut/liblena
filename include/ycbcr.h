#ifndef YCBCR_H
#define YCBCR_H

#include "utils.h"
#include "types.h"

/* decimation (TODO: try interpolation) */
RawImageData rgb_to_ycbcr(const RawImageData &rgb_data);

#endif // YCBCR_H