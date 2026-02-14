#ifndef PPM_H
#define PPM_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"

typedef std::array<ImageCh<u8>, 3> PPMImageData;

inline int readPPM(std::ifstream &PPM_ifstream, PPMImageData& PPM_data) {

    /* Magic number */
    std::string magic_number;
    PPM_ifstream >> magic_number;

    INFO("magic_number = %s\n", magic_number.c_str());
    assert(magic_number == "P3" || magic_number == "P6");

    /* Skip comments */
    char line[1024];
    PPM_ifstream.getline(line, 1024);
    while (PPM_ifstream.peek() == '#') PPM_ifstream.getline(line, 1024);

    /* Header */
    u16 width, height, maxVal;
    PPM_ifstream >> width >> height >> maxVal;
    
    INFO("width = %d\n", width);
    if (width <= 0) {
        return 1;
    }
    
    INFO("height = %d\n", height);
    if (height <= 0) {
        return 1;
    }

    INFO("maxVal = %d\n", maxVal);
    if (maxVal <= 0 || maxVal > 255) {
        return 1;
    }

    auto R_ch = ImageCh<u8>(width, height);
    auto G_ch = ImageCh<u8>(width, height);
    auto B_ch = ImageCh<u8>(width, height);

    /* Data */
    if (magic_number == "P3") {
        for (u16 i = 0; i < height; i++) {
            for (u16 j = 0; j < width; j++) {
                u32 R, G, B;
                PPM_ifstream >> R >> G >> B;
                R_ch(i, j) = static_cast<u8>(R);
                G_ch(i, j) = static_cast<u8>(G);
                B_ch(i, j) = static_cast<u8>(B);
            }
        }
    } else if (magic_number == "P6") {
        std::vector<u8> buffer(width * height * 3);
        PPM_ifstream.get();
        PPM_ifstream.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        for (u16 i = 0; i < height; i++) {
            for (u16 j = 0; j < width; j++) {
                R_ch(i, j) = static_cast<u8>(buffer[i * width * 3 + j * 3 + 0]);
                G_ch(i, j) = static_cast<u8>(buffer[i * width * 3 + j * 3 + 1]);
                B_ch(i, j) = static_cast<u8>(buffer[i * width * 3 + j * 3 + 2]);
            }
        }
    }
    
    PPM_data[0] = std::move(R_ch);
    PPM_data[1] = std::move(G_ch);
    PPM_data[2] = std::move(B_ch);
    return 0;
}

#endif // PPM_H
