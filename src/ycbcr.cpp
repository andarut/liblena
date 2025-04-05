#include "ycbcr.h"

// JPEG conversion
std::vector<int> rgb2ycbcr(int r, int g, int b) {
	int y =    0 + (0.299 * r) + (0.587 * g) + (0.114 * b);
	int cb = 128 + (-0.169 * r) + (-0.331 * g) + (0.5 * b);
	int cr = 128 + (0.5 * r) + (-0.419 * g) + (-0.081 * b);
	return {y, cb, cr};
}

// JPEG conversion
std::vector<int> ycbcr2rgb(int y, int cb, int cr) {
	int r = 1.000 * y + 0.000 * (cb-128) + 1.400 * (cr - 128);
	int g = 1.000 * y - 0.343 * (cb-128) - 0.711 * (cr - 128);
	int b = 1.000 * y + 1.765 * (cb-128) + 0.000 * (cr - 128);
	return {r, g, b};
}

bool ycbcr_test() {
	std::vector<int> rgb = {255, 255, 255};
	std::vector<int> ycbcr = rgb2ycbcr(rgb[0], rgb[1], rgb[2]);
	std::vector<int> result_rgb = ycbcr2rgb(ycbcr[0], ycbcr[1], ycbcr[2]);
	if (rgb == result_rgb) return true;
	return false;
}