// #include "ppm_image.h"

// PPMImage read_ppm_image(std::ifstream &ppm_file) {

//     // Magic number
//     std::string magic_number;
//     ppm_file >> magic_number;

//     Logger::log_info("magic_number = %s", magic_number.c_str());
//     assert(magic_number == "P3" || magic_number == "P6");

//     // Skip comments
//     char line[1024];
//     ppm_file.getline(line, 1024);
//     while (ppm_file.peek() == '#') ppm_file.getline(line, 1024);

//     // Read header
//     u16 width, height, maxVal;
//     ppm_file >> width >> height >> maxVal;
    
//     Logger::log_info("width = %d", width);
//     assert(width > 0);
    
//     Logger::log_info("height = %d", height);
//     assert(height > 0);
    
//     Logger::log_info("maxVal = %d", maxVal);
//     assert(maxVal > 0);
    
//     PPMImage ppm_image(magic_number, width, height, 3, maxVal);

//     RawChannelData *R = &ppm_image[0];
//     RawChannelData *G = &ppm_image[1];
//     RawChannelData *B = &ppm_image[2];

//     for (u16 i = 0; i < height; i++) {
//         for (u16 j = 0; j < width; j++) {
//             /* (maxVal <= 255) ? u8 : u16 */
//             if (maxVal <= 255) {
//                 u8 r, g, b;
//                 ppm_file >> r >> g >> b;
//                 (*R)(i, j) = static_cast<u64>(r);
//                 (*G)(i, j) = static_cast<u64>(g);
//                 (*B)(i, j) = static_cast<u64>(b);
//             } else {
//                 u16 r, g, b;
//                 ppm_file >> r >> g >> b;
//                 (*R)(i, j) = static_cast<u64>(r);
//                 (*G)(i, j) = static_cast<u64>(g);
//                 (*B)(i, j) = static_cast<u64>(b);
//             }
//         }
//     }

//     Logger::log_info("size = %d", ppm_image.size());

//     assert(ppm_image.size() == width*height*3);

//     assert(ppm_image[0].size() == width*height);
//     assert(ppm_image[1].size() == width*height);
//     assert(ppm_image[2].size() == width*height);

//     return ppm_image;
// }
