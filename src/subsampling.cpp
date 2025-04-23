// #include "subsampling.h"

// /* ENCODING */
// template <typename T>
// SubsampledChannelData<T> encode_subsampling(const RawChannelData<T> &data, const SubsamplingMode &mode) {
//     if (mode.b != mode.a && mode.b != 0) exit(1);

//     u64 block_w = mode.J;
//     u64 block_h = 2;

//     Logger::log_info("mode = %lld:%lld:%lld", mode.J, mode.a, mode.b);

//     Logger::log_info("width = %lld", data.width);
//     Logger::log_info("height = %lld", data.height);

//     SubsampledChannelData<T> subsampled_data(data.width * data.height, mode);

//     u64 write_i = 0;
//     for (u64 read_i = 0; read_i < data.height; read_i++) {
//         u64 write_j = 0;
//         u64 samples_count = (read_i % 2 == 0) ? mode.a : mode.b;
//         if (read_i % 2 != 0 && mode.b == 0) continue;
//         for (u64 read_j = 0; read_j < data.width; read_j++) {
//             if ((read_j % mode.J) % samples_count != 0 && samples_count != mode.J) continue;
//             subsampled_data.data.push_back(data(read_i, read_j));
//             write_j++;
//             if (samples_count == 1) read_j+=block_w-1;
//         }
//         if (read_i == 0) subsampled_data.width = subsampled_data.data.size();
//         write_i++;
//     }
//     subsampled_data.height = write_i;

//     return subsampled_data;
// }

// template <typename T>
// SubsampledImageData<T> encode_subsampling(const RawImageData<T> &data, const SubsamplingMode &mode) {
//     return RawImageData<T>(data.width, data.height, std::vector<RawChannelData<T>>({
//         subsampling(data[0], SubsamplingMode(4, 4, 4)),
//         subsampling(data[1], mode),
//         subsampling(data[2], mode)
//     }));
// }

// /* DECODING */
// template <typename T>
// RawChannelData<T> decode_subsampling(const SubsampledChannelData<T> &subsampled_data) {

// }

// template <typename T>
// RawImageData<T>   decode_subsampling(const SubsampledImageData<T>   &subsampled_data) {
//     return RawImageData<T>(subsampled_data[0].width, subsampled_data[0].height, std::vector<RawChannelData<T>>({
//         decode_subsampling(subsampled_data[0], SubsamplingMode(4, 4, 4)),
//         decode_subsampling(subsampled_data[1], subsampled_data.mode),
//         decode_subsampling(subsampled_data[2], subsampled_data.mode)
//     }));
// }
