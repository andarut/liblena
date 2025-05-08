#ifndef PPM_H
#define PPM_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"

template <typename T>
std::vector<ImageChannel<T>> PPM(std::ifstream &PPM_file) {

    /* Magic number */
    std::string magic_number;
    PPM_file >> magic_number;

    INFO("magic_number = %s", magic_number.c_str());
    assert(magic_number == "P3" || magic_number == "P6");

    /* Skip comments */
    char line[1024];
    PPM_file.getline(line, 1024);
    while (PPM_file.peek() == '#') PPM_file.getline(line, 1024);

    /* Header */
    u16 width, height, maxVal;
    PPM_file >> width >> height >> maxVal;
    
    INFO("width = %d", width);
    assert(width > 0);
    
    INFO("height = %d", height);
    assert(height > 0);
    
    INFO("maxVal = %d", maxVal);
    assert(maxVal > 0);
    
    /* Data */

    auto R_ch = ImageChannel<T>(width, height);
    R_ch.resize(width, height);
    auto G_ch = ImageChannel<T>(width, height);
    G_ch.resize(width, height);
    auto B_ch = ImageChannel<T>(width, height);
    B_ch.resize(width, height);

    PPM_file.get();
    std::vector<char> buffer(width * height * 3 * (maxVal > 255 ? 2 : 1));
    PPM_file.read(buffer.data(), buffer.size());

    u8 bytes_per_sample = (maxVal > 255 ? 2 : 1);

    for (u16 i = 0; i < height; i++) {
        for (u16 j = 0; j < width; j++) {
            R_ch(i, j) = static_cast<T>(buffer[i * width * 3 + j * 3 + 0]);
            G_ch(i, j) = static_cast<T>(buffer[i * width * 3 + j * 3 + 1]);
            B_ch(i, j) = static_cast<T>(buffer[i * width * 3 + j * 3 + 2]);

        //     size_t pix_idx = static_cast<size_t>(i) * width + j;
        // // base byte offset for this pixel
        // size_t base = pix_idx * 3 * bytes_per_sample;

        // uint16_t sample[3];
        // for (int c = 0; c < 3; ++c) {
        //     size_t off = base + c * bytes_per_sample;
        //     if (bytes_per_sample == 1) {
        //         sample[c] = static_cast<unsigned char>(buffer[off]);
        //     } else {
        //         // big-endian two-byte sample
        //         sample[c] = (static_cast<uint16_t>(
        //                          static_cast<unsigned char>(buffer[off])) << 8)
        //                   | static_cast<uint16_t>(
        //                          static_cast<unsigned char>(buffer[off + 1]));
        //     }
        // }

        // (*R)(i, j) = static_cast<T>(sample[0]);
        // (*G)(i, j) = static_cast<T>(sample[1]);
        // (*B)(i, j) = static_cast<T>(sample[2]);

    //         /* (maxVal <= 255) ? u8 : u16 */
    //         if (maxVal <= 255) {
    //             u8 r, g, b;
    //             ppm_file >> r >> g >> b;
    //             (*R)(i, j) = static_cast<T>(r);
    //             (*G)(i, j) = static_cast<T>(g);
    //             (*B)(i, j) = static_cast<T>(b);
    //         } else {
    //             u16 r, g, b;
    //             ppm_file >> r >> g >> b;
    //             (*R)(i, j) = static_cast<T>(r);
    //             (*G)(i, j) = static_cast<T>(g);
    //             (*B)(i, j) = static_cast<T>(b);
    //         }
        }
    }

    return {R_ch, G_ch, B_ch};
}

#endif // PPM_H
