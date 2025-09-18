#include "ycbcr.h"

void YCbCrEncoder::encode(u8_image&& rgb_data) {
    g_timers.start("YCbCrEncoder::encode");

    for (u64 i = 0; i < rgb_data[0].height(); i++) {
        for (u64 j = 0; j < rgb_data[0].width(); j++) {
            double R = rgb_data[0](i, j);
            double G = rgb_data[1](i, j);
            double B = rgb_data[2](i, j);
            u8 Y = \
                static_cast<u8>(std::clamp(std::round(  0.299 * R +0.587 * G +0.114 * B),             0.0, 255.0));
            u8 Cb = \
                static_cast<u8>(std::clamp(std::round(-0.1687 * R -0.3313 * G +0.5 * B + 128.0) , 0.0, 255.0));
            u8 Cr = \
                static_cast<u8>(std::clamp(std::round( 0.5 * R -0.4187 * G -0.0813 * B + 128.0), 0.0, 255.0));
            rgb_data[0](i, j) = Y;
            rgb_data[1](i, j) = Cb;
            rgb_data[2](i, j) = Cr;
        }
    }

    g_timers.end("YCbCrEncoder::encode");

    u64 duration = g_timers.duration("YCbCrEncoder::encode");

    INFO("YCbCrEncoder::encode duration = %llu ms\n", duration);
}


void YCbCrEncoder::decode(u8_image&& ycbcr_data) {
    g_timers.start("YCbCrEncoder::decode");

    for (u64 i = 0; i < ycbcr_data[0].height(); i++) {
        for (u64 j = 0; j < ycbcr_data[0].width(); j++) {
            decode_px(std::move(std::array<u8, 3>(
                {
                    ycbcr_data[0](i, j),
                    ycbcr_data[1](i, j),
                    ycbcr_data[2](i, j)
                }
            )));
        }
    }

    g_timers.end("YCbCrEncoder::decode");

    u64 duration = g_timers.duration("YCbCrEncoder::decode");

    INFO("YCbCrEncoder::decode duration = %llu ms\n", duration);
}

void YCbCrEncoder::encode_px(std::array<u8, 3>&& rgb_px) {
    double R = rgb_px[0];
    double G = rgb_px[1];
    double B = rgb_px[2];
    u8 Y = \
        static_cast<u8>(std::clamp(std::round(  0.299 * R +0.587 * G +0.114 * B),             0.0, 255.0));
    u8 Cb = \
        static_cast<u8>(std::clamp(std::round(-0.1687 * R -0.3313 * G +0.5 * B + 128.0) , 0.0, 255.0));
    u8 Cr = \
        static_cast<u8>(std::clamp(std::round( 0.5 * R -0.4187 * G -0.0813 * B + 128.0), 0.0, 255.0));
    rgb_px[0] = Y;
    rgb_px[1] = Cb;
    rgb_px[2] = Cr;
}

// TODO: redo on 4 decimal floating point
void YCbCrEncoder::decode_px(std::array<u8, 3>&& ycbcr_px) {
    double Y = ycbcr_px[0];
    double Cb = ycbcr_px[1];
    double Cr = ycbcr_px[2];

    u8 R = \
        std::clamp(std::round(Y + 1.402 * (Cr - 128)),             0.0, 255.0);
    u8 G = \
        std::clamp(std::round(Y - 0.3441*(Cb-128)-0.7141*(Cr-128)), 0.0, 255.0);
    u8 B = \
        std::clamp(std::round(Y + 1.772*(Cb-128)), 0.0, 255.0);
    
    ycbcr_px[0] = R;
    ycbcr_px[1] = G;
    ycbcr_px[2] = B;
}