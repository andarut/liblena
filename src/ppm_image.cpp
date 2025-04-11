#include "ppm_image.h"

PPMImage read_ppm_image(std::ifstream &ppm_file) {

    // Magic number
    std::string magic_number;
    ppm_file >> magic_number;

    Logger::log_info("magic_number = %s", magic_number.c_str());
    assert(magic_number == "P3" || magic_number == "P6");

    // Skip comments
    char line[1024];
    ppm_file.getline(line, 1024);
    while (ppm_file.peek() == '#') ppm_file.getline(line, 1024);

    // Read header
    u16 width, height, maxVal;
    ppm_file >> width >> height >> maxVal;
    
    Logger::log_info("width = %d", width);
    assert(width > 0);
    
    Logger::log_info("height = %d", height);
    assert(height > 0);
    
    Logger::log_info("maxVal = %d", maxVal);
    assert(maxVal > 0);
    
    PPMImage ppm_image(magic_number, width, height, 3, maxVal);

    // Read data
    // TODO: optimize (test_perfomance takes aroung 1.8 seconds)
    for (u16 i = 0; i < height; i++)
        for (u16 j = 0; j < width; j++)
            for (u8 k = 0; k < 3; k++)
                ppm_file >> ppm_image(i, j, k);

    Logger::log_info("data size = %d", ppm_image.data.size());
    assert(ppm_image.data.size() == width*height*3);

    return ppm_image;
}
