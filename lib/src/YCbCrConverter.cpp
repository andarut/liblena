#include "YCbCrConverter.h"

void YCbCrConverter::fromRGB(Image<u8>& rgbData) {
    g_timers.start("YCbCrConverter::fromRGB");

    for (u64 i = 0; i < rgbData.height(); i++) {
        for (u64 j = 0; j < rgbData.width(); j++) {
            double R = rgbData[0](i, j);
            double G = rgbData[1](i, j);
            double B = rgbData[2](i, j);
            u8 Y = \
                static_cast<u8>(std::clamp(std::round(  0.299 * R +0.587 * G +0.114 * B),        0.0, 255.0));
            u8 Cb = \
                static_cast<u8>(std::clamp(std::round(-0.1687 * R -0.3313 * G +0.5 * B + 128.0), 0.0, 255.0));
            u8 Cr = \
                static_cast<u8>(std::clamp(std::round( 0.5 * R -0.4187 * G -0.0813 * B + 128.0), 0.0, 255.0));
            rgbData[0](i, j) = Y;
            rgbData[1](i, j) = Cb;
            rgbData[2](i, j) = Cr;
        }
    }

    g_timers.end("YCbCrConverter::fromRGB");

    INFO("YCbCrConverter::fromRGB duration = %f\n", g_timers.duration("YCbCrConverter::fromRGB"));
}


void YCbCrConverter::toRGB(Image<u8>& ycbcrData) {

    g_timers.start("YCbCrConverter::toRGB");

    for (u64 i = 0; i < ycbcrData.height(); i++) {
        for (u64 j = 0; j < ycbcrData.width(); j++) {
            double Y = ycbcrData[0](i, j);
            double Cb = ycbcrData[1](i, j);
            double Cr = ycbcrData[2](i, j);
            u8 R = \
                std::clamp(std::round(Y + 1.402 * (Cr - 128)),             0.0, 255.0);
            u8 G = \
                std::clamp(std::round(Y - 0.3441*(Cb-128)-0.7141*(Cr-128)), 0.0, 255.0);
            u8 B = \
                std::clamp(std::round(Y + 1.772*(Cb-128)), 0.0, 255.0);
            ycbcrData[0](i, j) = R;
            ycbcrData[1](i, j) = G;
            ycbcrData[2](i, j) = B;
        }
    }

    g_timers.end("YCbCrConverter::toRGB");
}