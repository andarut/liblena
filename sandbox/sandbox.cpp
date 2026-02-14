#include "BitStream.hpp"
#include "HuffmanEncoder.h"
#include "HuffmanTable.h"

int main() {
  BitStream bs;
  HuffmanEncoder encoder(bs);

  encoder.writeDC(4, LUMINANCE_DC);
  return 0;
}
