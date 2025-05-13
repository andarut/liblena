#ifndef PPM_H
#define PPM_H

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"

inline std::vector<ImageChannel<u8>> PPM(std::ifstream &PPM_file) {

    /* Magic number */
    std::string magic_number;
    PPM_file >> magic_number;

    INFO("magic_number = %s\n", magic_number.c_str());
    assert(magic_number == "P3" || magic_number == "P6");

    /* Skip comments */
    char line[1024];
    PPM_file.getline(line, 1024);
    while (PPM_file.peek() == '#') PPM_file.getline(line, 1024);

    /* Header */
    u16 width, height, maxVal;
    PPM_file >> width >> height >> maxVal;
    
    INFO("width = %d\n", width);
    assert(width > 0);
    
    INFO("height = %d\n", height);
    assert(height > 0);
    
    INFO("maxVal = %d\n", maxVal);
    assert(maxVal > 0);
    assert(maxVal <= 255);

    /* Data */
    PPM_file.get();
    std::vector<char> buffer(width * height * 3);
    PPM_file.read(buffer.data(), buffer.size());
    
    auto R_ch = ImageChannel<u8>(width, height);
    auto G_ch = ImageChannel<u8>(width, height);
    auto B_ch = ImageChannel<u8>(width, height);

    R_ch.resize(width, height);
    G_ch.resize(width, height);
    B_ch.resize(width, height);

    for (u16 i = 0; i < height; i++) {
        for (u16 j = 0; j < width; j++) {
            R_ch(i, j) = static_cast<u8>(buffer[i * width * 3 + j * 3 + 0]);
            G_ch(i, j) = static_cast<u8>(buffer[i * width * 3 + j * 3 + 1]);
            B_ch(i, j) = static_cast<u8>(buffer[i * width * 3 + j * 3 + 2]);
        }
    }
    return {R_ch, G_ch, B_ch};
}

#endif // PPM_H
