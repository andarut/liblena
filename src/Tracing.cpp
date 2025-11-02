#include "Tracing.h"
#include "BitStream.hpp"
#include "Logger.hpp"
#include "PPMReader.h"
#include "ImageCh.h"

#include <algorithm>

#include "Window.h"
#include "ImageView.h"
#include "MatrixView.h"
#include "TableView.h"

int Tracing::loadFromFile(const std::string& filename) {
    BitStream bs;
    if(bs.readFromFile(filename) != 0) {
        ERROR("Can't open file: %s\n", filename.c_str());
        return -1;
    }
    PPMImageData image_data;

    u64 width = 0;
    {
        auto res = bs.read<u64>(width);
        if(res) {
            ERROR("Failed to read source image width from: %s\n", filename.c_str());
            return -2;
        }
    }

    u64 height = 0;
    {
        auto res = bs.read<u64>(height);
        if(res) {
            ERROR("Failed to read source image height from: %s\n", filename.c_str());
            return -3;
        }
    }
    
    for(u8 k = 0; k < 3; k++) {
        image_data[k] = ImageCh<u8>(width, height);
        for(u64 i = 0; i < height; i++) {
            for(u64 j = 0; j < width; j++) {
                auto res = bs.read<u8>(image_data[k](i, j));
                if(res) {
                    ERROR("Failed to read pixel data: i = %lld, j = %lld\n", i, j);
                    return -4;
                }
            }
        }
    }

    m_trace.sourceImageData = std::move(image_data);

    u64 MCUsCount = 0;
    {
        auto res = bs.read<u64>(MCUsCount);
        RETURN_IF_ERROR(res, "Failed to read MCUs count from %s\n", filename.c_str());
    }

    for(u64 i = 0; i < width * height; i++) {
        auto res = bs.read<s32>(m_trace.mcu.DCT_coeff[i]);
        RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
    }

    for(u64 i = 0; i < width * height; i++) {
        auto res = bs.read<s32>(m_trace.mcu.Quant_coeff[i]);
        RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
    }

    for(u64 i = 0; i < width * height; i++) {
        auto res = bs.read<s16>(m_trace.mcu.Zigzag_coeff[i]);
        RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
    }

    u64 RLC_len = 0;
    {
        auto res = bs.read<u64>(RLC_len);
        RETURN_IF_ERROR(res, "Failedl to read RLE len from %s\n", filename.c_str());
    }

    m_trace.mcu.RLC_coeff.resize(RLC_len);

    for(u64 i = 0; i < RLC_len; i++) {
        auto res = bs.read<s16>(m_trace.mcu.RLC_coeff[i]);
        RETURN_IF_ERROR(res, "Failed to read RLC valus: i = %lld\n", i);
    }

    return 0;
}

int Tracing::show() {
    
    /* For Input image data*/
    ImageView image_view(m_trace.sourceImageData, 50);
    {
        auto res = image_view.init();
        if(res) {
            ERROR("Failed to init first image view\n");
            return 1;
        }
    }

    /* For DCT */
    MatrixView<s32> DCT_view(m_trace.mcu.DCT_coeff, (u64)8, (u64)8);
    {
        auto res = DCT_view.init("DCT coeff");
        RETURN_IF_ERROR(res, "Failed to init DCT view");
    }

    /* For Quantizated values */
    MatrixView<s32> Quant_view(m_trace.mcu.Quant_coeff, (u64)8, (u64)8);
    {
        auto res = Quant_view.init("Quantization coeff");
        RETURN_IF_ERROR(res, "Failed to init Quant_view view");
    }

    /* For Zigzag values */
    MatrixView<s16> Zigzag_view(m_trace.mcu.Zigzag_coeff, (u64)8, (u64)8);
    {
        auto res = Zigzag_view.init("Zigzag Order");
        RETURN_IF_ERROR(res, "Failed to init Zigzag_view view");
    }

    /* For RLC values */
    TableView<s16> RLC_view(m_trace.mcu.RLC_coeff, 3);
    {
        auto res = RLC_view.init("RLC coeff");
        RETURN_IF_ERROR(res, "Failed to ini RLC view");
    }

    while(1) {
        image_view.render();
        DCT_view.render();
        Quant_view.render();
        Zigzag_view.render();
        RLC_view.render();
    }
    return 0;
}

int main(int argc, char* argv[]) {
    Tracing tracing;
    if(argc < 2) {
        ERROR("Usage: %s <path-to-file>\n", argv[0]);
        return 1;
    }

    std::string filePath = argv[1];
    {
        auto res = tracing.loadFromFile(filePath);
        if(res) {
            ERROR("Can't load tracing from path: %s\n", filePath.c_str());
            return res;
        }
    }
    tracing.show();
    return 0;
}