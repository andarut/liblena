#include <string>
#include "PPMReader.h"

struct Trace {
    PPMImageData sourceImageData;
    struct MCUTrace {
        std::array<s32, 64> DCT_coeff;
        std::array<s32, 64> Quant_coeff;
        std::array<s16, 64> Zigzag_coeff;
        std::vector<s16> RLC_coeff; 
    };
    MCUTrace mcu;
};

class Tracing {
public:
    int loadFromFile(const std::string& filename);
    int show();
private:
    Trace m_trace;
};