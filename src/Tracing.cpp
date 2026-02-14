#include "Tracing.h"
#include "BitStream.hpp"
#include "Logger.hpp"
#include "PPMReader.h"
#include "ImageCh.h"

#include <algorithm>
#include <utility>

#include "Window.h"
#include "ImageView.h"
#include "MatrixView.h"
#include "TableView.h"

int Tracing::loadFromFile(const std::string& filename) {

    INFO("Loading tracing from file: %s\n", filename.c_str());

    BitStream bs;
    if(bs.readFromFile(filename) != 0) {
        ERROR("Can't open file: %s\n", filename.c_str());
        return -1;
    }
    PPMImageData image_data;

    u64 width = 0;
    {
        auto res = bs.read(width);
        if(res) {
            ERROR("Failed to read source image width from: %s\n", filename.c_str());
            return -2;
        }
    }
    INFO("Image width = %llu\n", width);

    u64 height = 0;
    {
        auto res = bs.read(height);
        if(res) {
            ERROR("Failed to read source image height from: %s\n", filename.c_str());
            return -3;
        }
    }
    INFO("Image height = %llu\n", height);
    
    for(u8 k = 0; k < 3; k++) {
        image_data[k] = ImageCh<u8>(width, height);
        for(u64 i = 0; i < height; i++) {
            for(u64 j = 0; j < width; j++) {
                auto res = bs.read(image_data[k](i, j));
                if(res) {
                    ERROR("Failed to read pixel data: i = %lld, j = %lld\n", i, j);
                    return -4;
                }
            }
        }
    }
    INFO("Loaded source image data\n");

    m_trace.sourceImageData = std::move(image_data);

    {
        PPMImageData colorConvertedImageData;

        u64 width = 0;
        {
            auto res = bs.read(width);
            if(res) {
                ERROR("Failed to read source image width from: %s\n", filename.c_str());
                return -2;
            }
        }
        INFO("Image width = %llu\n", width);

        u64 height = 0;
        {
            auto res = bs.read<u64>(height);
            if(res) {
                ERROR("Failed to read source image height from: %s\n", filename.c_str());
                return -3;
            }
        }
        INFO("Image height = %llu\n", height);
        
        for(u8 k = 0; k < 3; k++) {
            colorConvertedImageData[k] = ImageCh<u8>(width, height);
            for(u64 i = 0; i < height; i++) {
                for(u64 j = 0; j < width; j++) {
                    auto res = bs.read<u8>(colorConvertedImageData[k](i, j));
                    if(res) {
                        ERROR("Failed to read pixel data: i = %lld, j = %lld\n", i, j);
                        return -4;
                    }
                }
            }
        }
        INFO("Loaded color converted image data\n");

        m_trace.colorConvertedImageData = std::move(colorConvertedImageData);
    }

    // Read Y MCUs data
    {
        u64 MCUsCount = 0;
        {
            auto res = bs.read<u64>(MCUsCount);
            RETURN_IF_ERROR(res, "Failed to read MCUs count from %s\n", filename.c_str());
        }
        INFO("MCUs count = %llu\n", MCUsCount);

        m_trace.Y_MCUs.resize(MCUsCount);

        for (u64 mcu_i = 0; mcu_i < MCUsCount; mcu_i++) {
            INFO("Reading MCU %llu/%llu\n", mcu_i + 1, MCUsCount);

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Y_MCUs[mcu_i].inputData[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("Input coeff[%llu] = %d\n", i, m_trace.Y_MCUs[mcu_i].inputData[i]);
            }

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Y_MCUs[mcu_i].DCT_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("DCT coeff[%llu] = %d\n", i, m_trace.Y_MCUs[mcu_i].DCT_coeff[i]);
            }

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Y_MCUs[mcu_i].Quant_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("Quant coeff[%llu] = %d\n", i, m_trace.Y_MCUs[mcu_i].Quant_coeff[i]);
            }

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Y_MCUs[mcu_i].Zigzag_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("Zigzag coeff[%llu] = %d\n", i, m_trace.Y_MCUs[mcu_i].Zigzag_coeff[i]);
            }
        }

        // Read after DPCM
        for (u64 mcu_i = 0; mcu_i < MCUsCount; mcu_i++) {
            INFO("Reading DPCM data for MCU %llu/%llu\n", mcu_i + 1, MCUsCount);
            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Y_MCUs[mcu_i].DPCM_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("DPCM coeff[%llu] = %d\n", i, m_trace.Y_MCUs[mcu_i].DPCM_coeff[i]);
            }
        }
        
        // Read RLC data
        for (u64 mcu_i = 0; mcu_i < MCUsCount; mcu_i++) {
            INFO("Reading RLC data for MCU %llu/%llu\n", mcu_i + 1, MCUsCount);
            u64 RLC_len = 0;
            {
                auto res = bs.read<u64>(RLC_len);
                RETURN_IF_ERROR(res, "Failedl to read RLC len from %s\n", filename.c_str());
            }
            INFO("RLC len = %llu\n", RLC_len);
            m_trace.Y_MCUs[mcu_i].RLC_coeff.resize(RLC_len);

            for(u64 i = 0; i < RLC_len; i++) {
                auto res = bs.read<s16>(m_trace.Y_MCUs[mcu_i].RLC_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read RLC valus: i = %lld\n", i);
            }
        }
    }

    // Read Cb MCUs data
    {
        u64 MCUsCount = 0;
        {
            auto res = bs.read<u64>(MCUsCount);
            RETURN_IF_ERROR(res, "Failed to read MCUs count from %s\n", filename.c_str());
        }
        INFO("MCUs count = %llu\n", MCUsCount);

        m_trace.Cb_MCUs.resize(MCUsCount);

        for (u64 mcu_i = 0; mcu_i < MCUsCount; mcu_i++) {
            INFO("Reading MCU %llu/%llu\n", mcu_i + 1, MCUsCount);

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Cb_MCUs[mcu_i].inputData[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("Input coeff[%llu] = %d\n", i, m_trace.Cb_MCUs[mcu_i].inputData[i]);
            }

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Cb_MCUs[mcu_i].DCT_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("DCT coeff[%llu] = %d\n", i, m_trace.Cb_MCUs[mcu_i].DCT_coeff[i]);
            }

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Cb_MCUs[mcu_i].Quant_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("Quant coeff[%llu] = %d\n", i, m_trace.Cb_MCUs[mcu_i].Quant_coeff[i]);
            }

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read<s16>(m_trace.Cb_MCUs[mcu_i].Zigzag_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("Zigzag coeff[%llu] = %d\n", i, m_trace.Cb_MCUs[mcu_i].Zigzag_coeff[i]);
            }
        }

        // Read after DPCM
        for (u64 mcu_i = 0; mcu_i < MCUsCount; mcu_i++) {
            INFO("Reading DPCM data for MCU %llu/%llu\n", mcu_i + 1, MCUsCount);
            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read<s16>(m_trace.Cb_MCUs[mcu_i].DPCM_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("DPCM coeff[%llu] = %d\n", i, m_trace.Cb_MCUs[mcu_i].DPCM_coeff[i]);
            }
        }
        
        // Read RLC data
        for (u64 mcu_i = 0; mcu_i < MCUsCount; mcu_i++) {
            INFO("Reading RLC data for MCU %llu/%llu\n", mcu_i + 1, MCUsCount);
            u64 RLC_len = 0;
            {
                auto res = bs.read<u64>(RLC_len);
                RETURN_IF_ERROR(res, "Failedl to read RLC len from %s\n", filename.c_str());
            }
            INFO("RLC len = %llu\n", RLC_len);
            m_trace.Cb_MCUs[mcu_i].RLC_coeff.resize(RLC_len);

            for(u64 i = 0; i < RLC_len; i++) {
                auto res = bs.read<s16>(m_trace.Cb_MCUs[mcu_i].RLC_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read RLC valus: i = %lld\n", i);
            }
        }
    }


    // Read Cr MCUs data
    {
        u64 MCUsCount = 0;
        {
            auto res = bs.read<u64>(MCUsCount);
            RETURN_IF_ERROR(res, "Failed to read MCUs count from %s\n", filename.c_str());
        }
        INFO("MCUs count = %llu\n", MCUsCount);

        m_trace.Cr_MCUs.resize(MCUsCount);

        for (u64 mcu_i = 0; mcu_i < MCUsCount; mcu_i++) {
            INFO("Reading MCU %llu/%llu\n", mcu_i + 1, MCUsCount);

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Cr_MCUs[mcu_i].inputData[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("Input coeff[%llu] = %d\n", i, m_trace.Cr_MCUs[mcu_i].inputData[i]);
            }

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Cr_MCUs[mcu_i].DCT_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("DCT coeff[%llu] = %d\n", i, m_trace.Cr_MCUs[mcu_i].DCT_coeff[i]);
            }

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read(m_trace.Cr_MCUs[mcu_i].Quant_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("Quant coeff[%llu] = %d\n", i, m_trace.Cr_MCUs[mcu_i].Quant_coeff[i]);
            }

            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read<s16>(m_trace.Cr_MCUs[mcu_i].Zigzag_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("Zigzag coeff[%llu] = %d\n", i, m_trace.Cr_MCUs[mcu_i].Zigzag_coeff[i]);
            }
        }

        // Read after DPCM
        for (u64 mcu_i = 0; mcu_i < MCUsCount; mcu_i++) {
            INFO("Reading DPCM data for MCU %llu/%llu\n", mcu_i + 1, MCUsCount);
            for(u64 i = 0; i < width * height; i++) {
                auto res = bs.read<s16>(m_trace.Cr_MCUs[mcu_i].DPCM_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read pixel data: i = %lld", i);
                INFO("DPCM coeff[%llu] = %d\n", i, m_trace.Cr_MCUs[mcu_i].DPCM_coeff[i]);
            }
        }
        
        // Read RLC data
        for (u64 mcu_i = 0; mcu_i < MCUsCount; mcu_i++) {
            INFO("Reading RLC data for MCU %llu/%llu\n", mcu_i + 1, MCUsCount);
            u64 RLC_len = 0;
            {
                auto res = bs.read<u64>(RLC_len);
                RETURN_IF_ERROR(res, "Failedl to read RLC len from %s\n", filename.c_str());
            }
            INFO("RLC len = %llu\n", RLC_len);
            m_trace.Cr_MCUs[mcu_i].RLC_coeff.resize(RLC_len);

            for(u64 i = 0; i < RLC_len; i++) {
                auto res = bs.read<s16>(m_trace.Cr_MCUs[mcu_i].RLC_coeff[i]);
                RETURN_IF_ERROR(res, "Failed to read RLC valus: i = %lld\n", i);
            }
        }
    }
    

    return 0;
}

int Tracing::show() {
    
    /* For Input image data*/
    ImageView image_view(m_trace.sourceImageData, "RGB", 50);
    {
        auto res = image_view.init();
        if(res) {
            ERROR("Failed to init first image view\n");
            return 1;
        }
    }

    /* For YCbCr data*/
    ImageView colorConvertedImageView(m_trace.colorConvertedImageData, "YCbCr", 50);
    {
        auto res = colorConvertedImageView.init();
        if(res) {
            ERROR("Failed to init color converted image view\n");
            return 1;
        }
    }

    std::vector<std::unique_ptr<Window>> Y_MCUs_windows(m_trace.Y_MCUs.size() * 6);

    size_t winPosX = 0;
    size_t winPosY = 50;

    auto updateWindowsPositions = [&](std::pair<int, int> size) {
        winPosX += size.first;
        if (winPosX + size.first > 2560) {
            winPosX = 0;
            winPosY += size.second;
        }
    };

    // TODO: put in matrix view
    auto make_matrix_view = []<typename T, size_t N>(std::array<T, N> arr, u64 width, u64 height) {
      return std::make_unique<MatrixView<T, N>>(std::move(arr), width, height);
    };


    for(u64 mcu_i = 0; mcu_i < m_trace.Y_MCUs.size(); mcu_i++) {
        /* For input */
        auto input_view = make_matrix_view(m_trace.Y_MCUs[mcu_i].inputData, (u64)8, (u64)8);
        {
            auto res = input_view->init(std::format("Input MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init Input view");
            input_view->setPos(winPosX, winPosY);
            updateWindowsPositions(input_view->renderSize());
        }
        Y_MCUs_windows[mcu_i] = std::move(input_view);

        /* For DCT */
        auto DCT_view = make_matrix_view(m_trace.Y_MCUs[mcu_i].DCT_coeff, (u64)8, (u64)8);
        {
            auto res = DCT_view->init(std::format("FDCT MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init DCT view");
            DCT_view->setPos(winPosX, winPosY);
            updateWindowsPositions(DCT_view->renderSize());
        }
        Y_MCUs_windows[mcu_i + 1] = std::move(DCT_view);

        /* For Quantizated values */
        auto Quant_view = make_matrix_view(m_trace.Y_MCUs[mcu_i].Quant_coeff, (u64)8, (u64)8);
        {
            auto res = Quant_view->init(std::format("Quantization MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init Quant_view view");
            Quant_view->setPos(winPosX, winPosY);
            updateWindowsPositions(Quant_view->renderSize());
        }
        Y_MCUs_windows[mcu_i + 2] = std::move(Quant_view);

        /* For Zigzag values */
        auto Zigzag_view = make_matrix_view(m_trace.Y_MCUs[mcu_i].Zigzag_coeff, (u64)8, (u64)8);
        {
            auto res = Zigzag_view->init(std::format("Zigzag MCU %llu", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init Zigzag_view view");
            Zigzag_view->setPos(winPosX, winPosY);
            updateWindowsPositions(Zigzag_view->renderSize());
        }
        Y_MCUs_windows[mcu_i + 3] = std::move(Zigzag_view);

        /* For DPCM values */
        auto DPCM_view = make_matrix_view(m_trace.Y_MCUs[mcu_i].DPCM_coeff, (u64)8, (u64)8);
        {
            auto res = DPCM_view->init(std::format("DPCM MCU %llu", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init DPCM view");
            DPCM_view->setPos(winPosX, winPosY);
            updateWindowsPositions(DPCM_view->renderSize());
        }
        Y_MCUs_windows[mcu_i + 4] = std::move(DPCM_view);

        /* For RLC values */
        auto RLC_view = std::make_unique<TableView<s16>>(m_trace.Y_MCUs[mcu_i].RLC_coeff, 3);
        {
            auto res = RLC_view->init(std::format("RLC MCU %llu", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init RLC view");
            RLC_view->setPos(winPosX, winPosY);
            updateWindowsPositions(RLC_view->renderSize());
        }
        Y_MCUs_windows[mcu_i + 5] = std::move(RLC_view);
    }

    winPosX = 0;
    // winPosY += 430;

    std::vector<std::unique_ptr<Window>> Cb_MCUs_windows(m_trace.Cb_MCUs.size() * 6);
    for(u64 mcu_i = 0; mcu_i < m_trace.Cb_MCUs.size(); mcu_i++) {
        /* For input */
        auto input_view = make_matrix_view(m_trace.Cb_MCUs[mcu_i].inputData, (u64)8, (u64)8);
        {
            auto res = input_view->init(std::format("Input MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init Input view");
            input_view->setPos(winPosX, winPosY);
            updateWindowsPositions(input_view->renderSize());
        }
        Cb_MCUs_windows[mcu_i] = std::move(input_view);
        /* For DCT */
        auto DCT_view = make_matrix_view(m_trace.Cb_MCUs[mcu_i].DCT_coeff, (u64)8, (u64)8);
        {
            auto res = DCT_view->init(std::format("FDCT MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init DCT view");
            DCT_view->setPos(winPosX, winPosY);
            updateWindowsPositions(DCT_view->renderSize());
        }
        Cb_MCUs_windows[mcu_i + 1] = std::move(DCT_view);

        /* For Quantizated values */
        auto Quant_view = make_matrix_view(m_trace.Cb_MCUs[mcu_i].Quant_coeff, (u64)8, (u64)8);
        {
            auto res = Quant_view->init(std::format("Quantization MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init Quant_view view");
            Quant_view->setPos(winPosX, winPosY);
            updateWindowsPositions(Quant_view->renderSize());
        }
        Cb_MCUs_windows[mcu_i + 2] = std::move(Quant_view);

        /* For Zigzag values */
        auto Zigzag_view = make_matrix_view(m_trace.Cb_MCUs[mcu_i].Zigzag_coeff, (u64)8, (u64)8);
        {
            auto res = Zigzag_view->init(std::format("Zigzag MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init Zigzag_view view");
            Zigzag_view->setPos(winPosX, winPosY);
            updateWindowsPositions(Zigzag_view->renderSize());
        }
        Cb_MCUs_windows[mcu_i + 3] = std::move(Zigzag_view);

        /* For DPCM values */
        auto DPCM_view = make_matrix_view(m_trace.Cb_MCUs[mcu_i].DPCM_coeff, (u64)8, (u64)8);
        {
            auto res = DPCM_view->init(std::format("DPCM MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init DPCM view");
            DPCM_view->setPos(winPosX, winPosY);
            updateWindowsPositions(DPCM_view->renderSize());
        }
        Cb_MCUs_windows[mcu_i + 4] = std::move(DPCM_view);

        /* For RLC values */
        auto RLC_view = std::make_unique<TableView<s16>>(m_trace.Cb_MCUs[mcu_i].RLC_coeff, 3);
        {
            auto res = RLC_view->init(std::format("RLC MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init RLC view");
            RLC_view->setPos(winPosX, winPosY);
            updateWindowsPositions(RLC_view->renderSize());
        }
        Cb_MCUs_windows[mcu_i + 5] = std::move(RLC_view);
    }

    winPosX = 0;
    // winPosY += 430;

    std::vector<std::unique_ptr<Window>> Cr_MCUs_windows(m_trace.Cr_MCUs.size() * 6);
    for(u64 mcu_i = 0; mcu_i < m_trace.Cr_MCUs.size(); mcu_i++) {
        /* For input */
        auto input_view = make_matrix_view(m_trace.Cr_MCUs[mcu_i].inputData, (u64)8, (u64)8);
        {
            auto res = input_view->init(std::format("Input MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init Input view");
            input_view->setPos(winPosX, winPosY);
            updateWindowsPositions(input_view->renderSize());
        }
        Cr_MCUs_windows[mcu_i] = std::move(input_view);
        /* For DCT */
        auto DCT_view = make_matrix_view(m_trace.Cr_MCUs[mcu_i].DCT_coeff, (u64)8, (u64)8);
        {
            auto res = DCT_view->init(std::format("FDCT MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init DCT view");
            DCT_view->setPos(winPosX, winPosY);
            updateWindowsPositions(DCT_view->renderSize());
        }
        Cr_MCUs_windows[mcu_i + 1] = std::move(DCT_view);

        /* For Quantizated values */
        auto Quant_view = make_matrix_view(m_trace.Cr_MCUs[mcu_i].Quant_coeff, (u64)8, (u64)8);
        {
            auto res = Quant_view->init(std::format("Quantization MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init Quant_view view");
            Quant_view->setPos(winPosX, winPosY);
            updateWindowsPositions(Quant_view->renderSize());
        }
        Cr_MCUs_windows[mcu_i + 2] = std::move(Quant_view);

        /* For Zigzag values */
        auto Zigzag_view = make_matrix_view(m_trace.Cr_MCUs[mcu_i].Zigzag_coeff, (u64)8, (u64)8);
        {
            auto res = Zigzag_view->init(std::format("Zigzag MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init Zigzag_view view");
            Zigzag_view->setPos(winPosX, winPosY);
            updateWindowsPositions(Zigzag_view->renderSize());
        }
        Cr_MCUs_windows[mcu_i + 3] = std::move(Zigzag_view);

        /* For DPCM values */
        auto DPCM_view = make_matrix_view(m_trace.Cr_MCUs[mcu_i].DPCM_coeff, (u64)8, (u64)8);
        {
            auto res = DPCM_view->init(std::format("DPCM MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init DPCM view");
            DPCM_view->setPos(winPosX, winPosY);
            updateWindowsPositions(DPCM_view->renderSize());
        }
        Cr_MCUs_windows[mcu_i + 4] = std::move(DPCM_view);

        /* For RLC values */
        auto RLC_view = std::make_unique<TableView<s16>>(m_trace.Cr_MCUs[mcu_i].RLC_coeff, 3);
        {
            auto res = RLC_view->init(std::format("RLC MCU {}", mcu_i));
            RETURN_IF_ERROR(res, "Failed to init RLC view");
            RLC_view->setPos(winPosX, winPosY);
            updateWindowsPositions(RLC_view->renderSize());
        }
        Cr_MCUs_windows[mcu_i + 5] = std::move(RLC_view);
    }

    while(1) {
        image_view.render();
        colorConvertedImageView.render();
        for(auto& window : Y_MCUs_windows) {
            if(window) {
                window->render();
            }
        }
        for(auto& window : Cb_MCUs_windows) {
            if(window) {
                window->render();
            }
        }
        for(auto& window : Cr_MCUs_windows) {
            if(window) {
                window->render();
            }
        }
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
    INFO("Loaded tracing from file: %s\n", filePath.c_str());
    tracing.show();
    return 0;
}
