#include <string>
#include "PPMReader.h"

struct Trace {
  PPMImageData sourceImageData;
  PPMImageData colorConvertedImageData;
  struct MCUTrace {
    std::array<u8, 64> inputData;
    std::array<f64, 64> DCT_coeff;
    std::array<f64, 64> Quant_coeff;
    std::array<s16, 64> Zigzag_coeff;
    std::array<s16, 64> DPCM_coeff;
    std::vector<s16> RLC_coeff; 
  };
  std::vector<MCUTrace> Y_MCUs;
  std::vector<MCUTrace> Cb_MCUs;
  std::vector<MCUTrace> Cr_MCUs;
};

class Tracing {
public:
    int loadFromFile(const std::string& filename);
    int show();
private:
    Trace m_trace;
};
