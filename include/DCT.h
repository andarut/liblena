#ifndef DCT_H
#define DCT_H

#include "utils.h"
#include "types.h"

#include "blocksplitting.h"

/* ENCODING */
RawChannelData<f64> encode_DCT(const RawChannelData<u8>& data);

/* DECODING */
RawChannelData<u8> decode_DCT(const RawChannelData<f64>& data);

#endif // DCT_H